#include "pch.h"
#include "CExpCalc.h"
#define _USE_MATH_DEFINES
#include <math.h> //Ҫ�ŵ�iostreamǰ�� ����
#include <iostream>
#include "CExpAnalysis.h"

CExpCalc::CExpCalc()
{
}
//����
void CExpCalc::Reset() {
    m_exp = "";
    SuffixExp.clear();
}

//�������ʽ
//��������׺���ʽ�ַ���
bool CExpCalc::CreateExpression(std::string exp)
{
    m_exp = exp;
    variable_map = nullptr;
    SuffixExp.clear();
    if (!ExpPreprocessing())
        return false;

    return true;
}

//���ñ�����
//����: ����ֵ��Mapָ��
void CExpCalc::SetVariableMap(map<std::string, double>* datamap)
{
    if (datamap != nullptr)
        variable_map = datamap;
    else
        variable_map = nullptr;
}

//�������ս��
//����ֵ:   ���ʽ�Ľ���ַ���
//          #Err��  ���ʽ����
//          #DIV/0������0����
//          #NA��   ����û�еı�����
//          �����ַ����� 6λС��
std::string CExpCalc::GetClacResult() {
    m_rs = CalcExp(SuffixExp);
    return m_rs;
}

//�ǲ��������
bool CExpCalc::isOp(char s)
{
    switch (s)
    {
    case '+':
    case '-':
    case '*':
    case '/':
    case '(':
    case ')':
    case '^'://������
    case 's'://sin
    case 'c'://cos
    case 't'://tan
    case 'i'://arcsin
    case 'o'://arccos
    case 'l'://log10
    case 'n'://ln
    case 'a'://absf
        return true;
    default:
        return false;
    }
}
bool CExpCalc::isOp(std::string str)
{
    char s;
    if (str.size() > 1) return false;

    s = str[0];
    switch (s)
    {
    case '+':
    case '-':
    case '*':
    case '/':
    case '(':
    case ')':
    case '^'://������
    case 's'://sin
    case 'c'://cos
    case 't'://tan
    case 'i'://arcsin
    case 'o'://arccos
    case 'l'://log10
    case 'n'://ln
    case 'a'://absf
        return true;
    default:
        return false;
    }
}

//�ǲ������֣���������-����ҲΪ���֣�
bool CExpCalc::isNumber(const std::string& str) {
    if (str.size() == 1 && str == "-") return false;

    if (str[0] >= '\xb0' && str[0] <= '\xf7') return true;
    //if ((str.size() > 1 && str[0] >= 'A' && str[0] <= 'z') 
    if (str.size() > 1 && str[0] == '[')
        return true;

    for (char const& c : str) {
        if (c == '-' || c == '.' || (c >= '0' && c <= '9')) continue;
        else
            return false;
    }
    return true;
}

//���ȼ��Ƚ�
char CExpCalc::Precede(char op1, char op2)
{
    int a, b;
    a = Opid(op1); b = Opid(op2);
    if (a < 0 || b < 0) return ' ';
    return(Prior[a][b]);
}
char CExpCalc::Precede(std::string op1, std::string op2)
{
    if (op1 == "") return '<';
    if (op2 == "") return '>';
    int a, b;
    a = Opid(op1[0]); b = Opid(op2[0]);
    if (a < 0 || b < 0) return ' ';
    return(Prior[a][b]);
}
int CExpCalc::Opid(char op1) {
    switch (op1)
    {
    case'+':return 0;
    case'-':return 1;
    case'*':return 2;
    case'/':return 3;
    case'(':return 4;
    case')':return 5;
    case'#':return 6;
    case'^':return 7;
    case's':return 8;
    case'c':return 8;
    case't':return 8;
    case'i':return 8;
    case'o':return 8;
    case'l':return 8;
    case'n':return 8;
    case'a':return 8;
    default:return -1;
    }
}

//Ԥ����
//תΪСд��������Ƿ���һ����ȷ�ı��ʽ
bool CExpCalc::ExpPreprocessing()
{
    //ȥ�ո�
    m_exp.erase(std::remove(m_exp.begin(), m_exp.end(), ' '), m_exp.end());

    //תСд ��[]�е�Ӣ�ı�������תСд��
    for (size_t i = 0; i < m_exp.size(); i++) {
        if (m_exp[i] == '[') {
            while (m_exp[i] != ']') i++;
        }
        else
            m_exp[i] = tolower(m_exp[i]);
    }
    //ȫ��תСд
    //transform(m_exp.begin(), m_exp.end(), m_exp.begin(), ::tolower);

    //Ԥ����sin=>s cos=>c tan=>t arcsin=>i arccos=>o lg=>l ln=>n abs=>a
    while (true) {
        bool finded = false;
        size_t a = m_exp.find("sin");
        if (a != std::string::npos)
            m_exp.replace(a, 3, "s");
        finded |= (a != std::string::npos);

        a = m_exp.find("cos");
        if (a != std::string::npos)
            m_exp.replace(a, 3, "c");
        finded |= (a != std::string::npos);

        a = m_exp.find("tan");
        if (a != std::string::npos)
            m_exp.replace(a, 3, "t");
        finded |= (a != std::string::npos);

        a = m_exp.find("arcsin");
        if (a != std::string::npos)
            m_exp.replace(a, 6, "i");
        finded |= (a != std::string::npos);

        a = m_exp.find("arccos");
        if (a != std::string::npos)
            m_exp.replace(a, 6, "o");
        finded |= (a != std::string::npos);

        a = m_exp.find("lg");
        if (a != std::string::npos)
            m_exp.replace(a, 2, "l");
        finded |= (a != std::string::npos);

        a = m_exp.find("ln");
        if (a != std::string::npos)
            m_exp.replace(a, 2, "n");
        finded |= (a != std::string::npos);

        a = m_exp.find("abs");
        if (a != std::string::npos)
            m_exp.replace(a, 3, "a");
        finded |= (a != std::string::npos);

        a = m_exp.find("@pi");
        if (a != std::string::npos)
            m_exp.replace(a, 3, std::to_string(M_PI));

        a = m_exp.find("@e");
        if (a != std::string::npos)
            m_exp.replace(a, 2, std::to_string(M_E));

        finded |= (a != std::string::npos);

        if (finded == false) break;
    }

    //��׺���ʽ��֣����Ը���������
    vector<std::string> tmp = splitExp(m_exp);
    std::cout << endl << "���������׺���ʽ��     " << endl;
    for (vector<std::string>::iterator iter = tmp.begin(); iter != tmp.end(); iter++)
        std::cout << *iter << " ";
    std::cout << endl;

    //�﷨���� 
    CInfixExpAnalysis* p_analysis = new CInfixExpAnalysis(&tmp);
    if (!p_analysis->IsRight()) {
        delete p_analysis;
        std::cout << "��׺���ʽ����" << endl;
        return false;
    }
    delete p_analysis;

    //ת��׺���ʽ
    if (toSuffixExp(tmp, SuffixExp)) {
        std::cout << endl << "��׺���ʽ��     " << endl;
        for (vector<std::string>::iterator iter = SuffixExp.begin(); iter != SuffixExp.end(); iter++)
            std::cout << *iter << " ";
        std::cout << endl;
    }
    else {
        std::cout << endl << "��׺���ʽ��     ����" << endl << endl;
        return false;
    }
    return true;

    ////�����׺���ʽ
    //std::string rs = CalcExp(SuffixExp);
    //m_rs = rs;
    //std::cout << endl << "��������     " << rs << endl;
    //return true;
}

//��׺���ʽ���������������������ֵ�vector�����Ը���������
//����:   ��׺���ʽ�ַ���
//����ֵ: �ָ�õ���׺���ʽ����
vector<std::string> CExpCalc::splitExp(std::string s)
{
    vector<std::string> tmp;
    for (size_t i = 0; i < s.size(); i++) {
        std::string num;
        //�����ֵĴ���
        while (i < s.size() && ((s[i] >= '0' && s[i] <= '9') || s[i] == '.')) {
            num += s[i];
            i++;
        }

        if (i < s.size() && (s[i] >= '\xa1' && s[i] <= '\xfe')) {//�Ǻ��ֿ�ͷ��ֱ����һ�������
            while (i < s.size() && !isOp(s[i])) {
                num += s[i];
                i++;
            }
        }
        if (i < s.size() && s[i] == '[') {//��ǩ ֱ��']'
            while (i < s.size()) {
                //ȥ��[] 
                //if (s[i] == '[') { i++; continue; }
                //if (s[i] == ']') { break; }
                //num += s[i];

                //����[]���ں�׺���ʽ��ȥ[]
                num += s[i];
                if (s[i] == ']') break;
                i++;
            }
        }
        //��Ԥ����ʱ�ѳ���ֱ�ӱ�Ϊֵ������Ҫ�Գ������д���
        //if (s[i] == '@') {
        //    while (!isOp(s[i])) {
        //        num += s[i];
        //        ++i;
        //    }
        //}

        if (num.size()) {
            tmp.push_back(num);
        }

        if (i < s.size() && isOp(s[i])) {
            std::string ss;
            ss += s[i];
            tmp.push_back(ss);
        }
    }

    ////�������
    for (size_t i = 0; i < tmp.size() - 1; i++) {
        //��һ����"-",���Һ��������֡����֡�"("����Ϊ���ţ������ּӸ���
        if (i == 0 && tmp.at(i) == "-" && tmp.size() > 1) {
            if ((tmp.at(i + 1)[0] >= '\xb0' && tmp.at(i + 1)[0] <= '\xf7')  //�����һ���Ǻ���
               // || (tmp.at(i+1).size()>1 && tmp.at(i + 1)[0]>='A' && tmp.at(i + 1)[0] <= 'z')) {   //��Ӣ�ı�ǩ ��[]
                || (tmp.at(i + 1).size() > 1 && tmp.at(i + 1)[0] >= '[')) {  //��Ӣ�ı�ǩ []
                tmp.insert(tmp.begin() + i, "0");
                tmp.insert(tmp.begin() + i, "(");
                tmp.insert(tmp.begin() + i + 4, ")");
            }
            else if (isdigit(tmp.at(i + 1)[0])) { //�����һ��������
                tmp.at(i + 1).insert(0, "-");
                tmp.erase(tmp.begin() + i);
            }
            else if (tmp.at(i + 1) == "(") { //�����һ����"("
                tmp.insert(tmp.begin(), "0");
            }
        }

        //���ǿ�ͷ����ǰ��'-' ,��һ����������\���Ǻ���\����')'����һ�������ֻ��ߺ��֣���Ϊ���ţ������ּӸ���
        //else if (i > 0 && tmp.at(i) == "-" && !isdigit(tmp.at(i - 1)[0]) && !(tmp.at(i - 1)[0] >= '\xb0' && tmp.at(i - 1)[0] <= '\xf7') && tmp.at(i - 1)[0] != ')' && i != tmp.size() - 2) {
        //���ǿ�ͷ����ǰ��'-'����һ���ǳ���')'��������������һ�������ֻ��߱�ǩ����Ϊ����,�������һ��
        else if (i > 0 && tmp.at(i) == "-" && isOp(tmp.at(i - 1)) && tmp.at(i - 1)[0] != ')') {
            if ((tmp.at(i + 1)[0] >= '\xb0' && tmp.at(i + 1)[0] <= '\xf7')
                //|| (tmp.at(i + 1).size() > 1 && tmp.at(i + 1)[0] >= 'A' && tmp.at(i + 1)[0] <= 'z')) {   //�����һ���Ǻ��ֻ�Ӣ�ı�ǩ
                || (tmp.at(i + 1).size() > 1 && tmp.at(i + 1)[0] >= '[')) {  //���ǩ []
                tmp.insert(tmp.begin() + i, "0");
                tmp.insert(tmp.begin() + i, "(");
                tmp.insert(tmp.begin() + i + 4, ")");
            }
            else if (isdigit(tmp.at(i + 1)[0])) { //�����һ��������
                tmp.at(i + 1).insert(0, "-");
                tmp.erase(tmp.begin() + i);
            }
        }
    }
    return tmp;
}

//��׺���ʽת��׺���ʽ
//����1: ��׺���ʽ�ָ�������
//����2: ��׺���ʽ����ָ��
//����ֵ: �Ƿ�ɹ�
bool CExpCalc::toSuffixExp(vector<std::string>& expvector, vector<std::string>& suffixexp)
{
    std::string x;
    stack<std::string> opstk;    //�����ջ
    size_t i;
    for (i = 0; i < expvector.size(); ) {
        if (isNumber(expvector.at(i))) {
            suffixexp.push_back(expvector.at(i));
            i++;
            continue;
        }
        else if (opstk.size() == 0) {
            opstk.push(expvector.at(i));
            i++;
        }
        else if (isOp(expvector.at(i)[0])) {
            switch (Precede(opstk.top(), expvector.at(i)))
            {
            case'<':
                opstk.push(expvector.at(i));
                i++;
                break;
            case'=':
                x = opstk.top();
                opstk.pop();
                i++;
                break;
            case'>':
                suffixexp.push_back(opstk.top());
                x = opstk.top();
                opstk.pop();
                break;
            default:
                suffixexp.clear();
                return false;
                break;
            }
        }
    }
    size_t size = opstk.size();
    while (opstk.size() != 0) {
        suffixexp.push_back(opstk.top());
        opstk.pop();
    }
    return true;
}


//�����׺���ʽ
//����:   ��׺���ʽ����
//����ֵ: �������ַ���
std::string CExpCalc::CalcExp(vector<std::string> suffixexp) {
    double a, b, rs;
    stack<double> num;
    bool calcerror = false;
    if (suffixexp.size() <= 0)
        return std::string("#ERR");
    vector<std::string>::iterator it = suffixexp.begin();
    while (it != suffixexp.end() && !calcerror) {
        if (!isNumber(*it)) {
            switch ((*it)[0]) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
                a = num.top();
                num.pop();
                b = num.top();
                num.pop();
                if (Calc(a, (*it)[0], b, rs))
                    num.push(rs);
                else {
                    calcerror = true;
                    return std::string("#DIV/0");
                }
                break;

            case 's':
            case 'c':
            case 't':
            case 'i':
            case 'o':
            case 'l':
            case 'n':
            case 'a':
                a = num.top();
                num.pop();
                if (Calc((*it)[0], a, rs))
                    num.push(rs);
                else {
                    calcerror = true;
                    return std::string("#LOG-");
                }
                break;
            }
        }
        else {
            if (((*it)[0] >= '\xb0' && (*it)[0] <= '\xf7') //����
                //|| ((*it)[0] >= 'A' && (*it)[0] <= 'z')) { //Ӣ�ı�ǩ
                || ((*it)[0] >= '[')) { //Ӣ�ı�ǩ
                //transform(it->begin(), it->end(), it->begin(), ::toupper);
                if (variable_map == nullptr)
                    return std::string("#REF!");

                if (variable_map->size() <= 0)
                    return std::string("#REF!");

                std::map<std::string, double>::iterator mapit;
                std::string titel;
                titel = *it;
                if (titel[0] == '[') titel = titel.substr(1, titel.length() - 2);
                mapit = variable_map->find(titel);
                if (mapit != variable_map->end())
                    num.push(mapit->second);
                else
                    return std::string("#REF!");
            }
            else
                num.push(atof(it->c_str()));
        }
        it++;
    }
    return to_string(num.top());
}

//˫Ŀ���������
//����1: ��ֵ
//����2: �����
//����3: ǰֵ
//����4: ��ż�������ָ��
bool CExpCalc::Calc(double a, char op, double b, double& rs)
{
    switch (op)
    {
    case '+':
        rs = b + a;
        break;
    case '-':
        rs = b - a;
        break;
    case '*':
        rs = b * a;
        break;
    case '/':
        if (a == 0)
            return false;
        else
            rs = b / a;
        break;
    case '^': //������
        rs = pow(b, a);
        break;
    default:
        return false;
    }
    return true;
}

//��Ŀ���������
//����1: �����
//����2: ��ֵ
//����3: ��ż�������ָ��
bool CExpCalc::Calc(char op, double b, double& rs)
{
    rs = 0.0;
    switch (op)
    {
    case 's'://sin
        rs = sin(b);
        break;
    case 'c'://cos
        rs = cos(b);
        break;
    case 't'://tan
        if (fabs(fmod((b - M_PI_2), M_PI)) < 0.000001)
            return false;
        rs = tan(b);
        break;
    case 'i'://arcsin
        rs = asin(b);
        break;
    case 'o'://arccos
        rs = acos(b);
        break;
    case 'l'://lg
        if (b > 0)
            rs = log10(b);
        else
            return false;
        break;
    case 'n'://ln
        if (b > 0)
            rs = log(b);
        else
            return false;
        break;
    case 'a'://absf
        rs = abs(b);
        break;
    default:
        return false;
    }
    return true;
}


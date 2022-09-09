#include "pch.h"
#include "CExpCalc.h"
#define _USE_MATH_DEFINES
#include <math.h> //要放到iostream前面 才行
#include <iostream>
#include "CExpAnalysis.h"

CExpCalc::CExpCalc()
{
}
//重置
void CExpCalc::Reset() {
    m_exp = "";
    SuffixExp.clear();
}

//创建表达式
//参数：中缀表达式字符串
bool CExpCalc::CreateExpression(std::string exp)
{
    m_exp = exp;
    variable_map = nullptr;
    SuffixExp.clear();
    if (!ExpPreprocessing())
        return false;

    return true;
}

//设置变量表
//参数: 变量值的Map指针
void CExpCalc::SetVariableMap(map<std::string, double>* datamap)
{
    if (datamap != nullptr)
        variable_map = datamap;
    else
        variable_map = nullptr;
}

//计算最终结果
//返回值:   表达式的结果字符串
//          #Err：  表达式错误
//          #DIV/0：除以0错误
//          #NA：   调用没有的变量名
//          数字字符串： 6位小数
std::string CExpCalc::GetClacResult() {
    m_rs = CalcExp(SuffixExp);
    return m_rs;
}

//是不是运算符
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
    case '^'://幂运算
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
    case '^'://幂运算
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

//是不是数字（包含变量-变量也为数字）
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

//优先级比较
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

//预处理
//转为小写，并检查是否是一个正确的表达式
bool CExpCalc::ExpPreprocessing()
{
    //去空格
    m_exp.erase(std::remove(m_exp.begin(), m_exp.end(), ' '), m_exp.end());

    //转小写 （[]中的英文变量名不转小写）
    for (size_t i = 0; i < m_exp.size(); i++) {
        if (m_exp[i] == '[') {
            while (m_exp[i] != ']') i++;
        }
        else
            m_exp[i] = tolower(m_exp[i]);
    }
    //全部转小写
    //transform(m_exp.begin(), m_exp.end(), m_exp.begin(), ::tolower);

    //预处理：sin=>s cos=>c tan=>t arcsin=>i arccos=>o lg=>l ln=>n abs=>a
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

    //中缀表达式拆分，并对负号做处理
    vector<std::string> tmp = splitExp(m_exp);
    std::cout << endl << "处理过的中缀表达式：     " << endl;
    for (vector<std::string>::iterator iter = tmp.begin(); iter != tmp.end(); iter++)
        std::cout << *iter << " ";
    std::cout << endl;

    //语法分析 
    CInfixExpAnalysis* p_analysis = new CInfixExpAnalysis(&tmp);
    if (!p_analysis->IsRight()) {
        delete p_analysis;
        std::cout << "中缀表达式错误" << endl;
        return false;
    }
    delete p_analysis;

    //转后缀表达式
    if (toSuffixExp(tmp, SuffixExp)) {
        std::cout << endl << "后缀表达式：     " << endl;
        for (vector<std::string>::iterator iter = SuffixExp.begin(); iter != SuffixExp.end(); iter++)
            std::cout << *iter << " ";
        std::cout << endl;
    }
    else {
        std::cout << endl << "后缀表达式：     错误" << endl << endl;
        return false;
    }
    return true;

    ////计算后缀表达式
    //std::string rs = CalcExp(SuffixExp);
    //m_rs = rs;
    //std::cout << endl << "计算结果：     " << rs << endl;
    //return true;
}

//中缀表达式把运算符，变量，常量拆分到vector，并对负号做处理
//参数:   中缀表达式字符串
//返回值: 分割好的中缀表达式数组
vector<std::string> CExpCalc::splitExp(std::string s)
{
    vector<std::string> tmp;
    for (size_t i = 0; i < s.size(); i++) {
        std::string num;
        //是数字的处理
        while (i < s.size() && ((s[i] >= '0' && s[i] <= '9') || s[i] == '.')) {
            num += s[i];
            i++;
        }

        if (i < s.size() && (s[i] >= '\xa1' && s[i] <= '\xfe')) {//是汉字开头，直到下一个运算符
            while (i < s.size() && !isOp(s[i])) {
                num += s[i];
                i++;
            }
        }
        if (i < s.size() && s[i] == '[') {//标签 直到']'
            while (i < s.size()) {
                //去除[] 
                //if (s[i] == '[') { i++; continue; }
                //if (s[i] == ']') { break; }
                //num += s[i];

                //保留[]，在后缀表达式中去[]
                num += s[i];
                if (s[i] == ']') break;
                i++;
            }
        }
        //在预处理时把常数直接变为值，不需要对常数进行处理
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

    ////处理减号
    for (size_t i = 0; i < tmp.size() - 1; i++) {
        //第一个是"-",并且后面是数字、汉字、"("，则为负号，给数字加负号
        if (i == 0 && tmp.at(i) == "-" && tmp.size() > 1) {
            if ((tmp.at(i + 1)[0] >= '\xb0' && tmp.at(i + 1)[0] <= '\xf7')  //如果下一个是汉字
               // || (tmp.at(i+1).size()>1 && tmp.at(i + 1)[0]>='A' && tmp.at(i + 1)[0] <= 'z')) {   //或英文标签 无[]
                || (tmp.at(i + 1).size() > 1 && tmp.at(i + 1)[0] >= '[')) {  //或英文标签 []
                tmp.insert(tmp.begin() + i, "0");
                tmp.insert(tmp.begin() + i, "(");
                tmp.insert(tmp.begin() + i + 4, ")");
            }
            else if (isdigit(tmp.at(i + 1)[0])) { //如果下一个是数字
                tmp.at(i + 1).insert(0, "-");
                tmp.erase(tmp.begin() + i);
            }
            else if (tmp.at(i + 1) == "(") { //如果下一个是"("
                tmp.insert(tmp.begin(), "0");
            }
        }

        //不是开头，当前是'-' ,上一个不是数字\不是汉字\不是')'，下一个是数字或者汉字，则为负号，给数字加负号
        //else if (i > 0 && tmp.at(i) == "-" && !isdigit(tmp.at(i - 1)[0]) && !(tmp.at(i - 1)[0] >= '\xb0' && tmp.at(i - 1)[0] <= '\xf7') && tmp.at(i - 1)[0] != ')' && i != tmp.size() - 2) {
        //不是开头，当前是'-'，上一个是除了')'以外的运算符，下一个是数字或者标签，则为负号,不是最后一个
        else if (i > 0 && tmp.at(i) == "-" && isOp(tmp.at(i - 1)) && tmp.at(i - 1)[0] != ')') {
            if ((tmp.at(i + 1)[0] >= '\xb0' && tmp.at(i + 1)[0] <= '\xf7')
                //|| (tmp.at(i + 1).size() > 1 && tmp.at(i + 1)[0] >= 'A' && tmp.at(i + 1)[0] <= 'z')) {   //如果下一个是汉字或英文标签
                || (tmp.at(i + 1).size() > 1 && tmp.at(i + 1)[0] >= '[')) {  //或标签 []
                tmp.insert(tmp.begin() + i, "0");
                tmp.insert(tmp.begin() + i, "(");
                tmp.insert(tmp.begin() + i + 4, ")");
            }
            else if (isdigit(tmp.at(i + 1)[0])) { //如果下一个是数字
                tmp.at(i + 1).insert(0, "-");
                tmp.erase(tmp.begin() + i);
            }
        }
    }
    return tmp;
}

//中缀表达式转后缀表达式
//参数1: 中缀表达式分割后的数组
//参数2: 后缀表达式数组指针
//返回值: 是否成功
bool CExpCalc::toSuffixExp(vector<std::string>& expvector, vector<std::string>& suffixexp)
{
    std::string x;
    stack<std::string> opstk;    //运算符栈
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


//计算后缀表达式
//参数:   后缀表达式数组
//返回值: 计算结果字符串
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
            if (((*it)[0] >= '\xb0' && (*it)[0] <= '\xf7') //汉字
                //|| ((*it)[0] >= 'A' && (*it)[0] <= 'z')) { //英文标签
                || ((*it)[0] >= '[')) { //英文标签
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

//双目运算符计算
//参数1: 后值
//参数2: 运算符
//参数3: 前值
//参数4: 存放计算结果的指针
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
    case '^': //幂运算
        rs = pow(b, a);
        break;
    default:
        return false;
    }
    return true;
}

//单目运算符计算
//参数1: 运算符
//参数2: 数值
//参数3: 存放计算结果的指针
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


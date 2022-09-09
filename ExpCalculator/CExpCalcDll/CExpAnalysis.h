/**************************************************
*  ����: momo
*        maozi888888@hotmail.com
*  ����:2022-09-06
*  ��������:
*      CInfixExpAnalysis������֤CExpCalc��׺�������ʽ�Ƿ���ϱ��ʽ�﷨
* 
*  ��ѧ���ʽ�ķ�����:
*    G(E)
*    E -> E+T | E-T              
*    T -> T*F | T/F | F     
*    F -> OpF | (E) | d                (Op:��Ŀ�����; d:����)
*  ��ݹ�����
*    E ->  TE'
*    E'-> +TE' | -TE' | ��
*    T ->  FT'
*    T'-> *FT' | /FT' | ��
*    F -> OpF  | (E) | d
* 
*   ��������ͣ�
*      '+':1    
*      '-':2    ��+,- ������һ�����ͣ�δ�ϲ���
*      '*':3;
*      '^':3;
*      '/':4;   ��*./,^ ��Щ˫Ŀ�����������һ�����ͣ��ϲ���*��^��
*        d:5    �����֣����� ��Ϊ�����֣�
*      '(':6;
*      ')':7;
*      's','c','t','i','o','l','n','a': 8  �����Ǻ���,����,����ֵ�ȵ�Ŀ����� һ�����ͣ�
*
**************************************************/

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility>
using namespace std;


class CInfixExpAnalysis
{
public:
    CInfixExpAnalysis(vector<std::string>* exp) {
        idx = 0;
        err = 0;
        sym = 0;
        m_result = false;
        CreateWord(exp);
    }

    bool IsRight() {
        return ExpIsRight();
        //return m_result;
    }

private:
    vector<pair<std::string, int>> m_word;
    int sym;
    size_t idx;
    int err;
    bool m_result;

    //�����ʷ���������
    void CreateWord(vector<std::string>* exp) {
        vector<std::string>::iterator it = exp->begin();
        while (it != exp->end()) {
            m_word.push_back(make_pair(*it, GetType(*it)));
            it++;
        }
        //ExpIsRight();
    }

    //�Ƿ������֣���������-����ҲΪ���֣�
    bool isNumber(const std::string& str) {
        if (str.size() == 1 && str == "-") return false;

        if ((str[0] >= '\xb0' && str[0] <= '\xf7')
            || (str[0] == '[' && str[str.size() - 1] == ']')) 
            return true;

        for (char const& c : str) {
            if (c == '-' || c == '.' || (c >= '0' && c <= '9')) continue;
            else
                return false;
        }
        return true;
    }

    int GetType(std::string str) {
        if (isNumber(str))  //����
            return 5;
        else {
            switch (str[0]) {//�����           
            case '+':
                return 1;
                break;
            case '-':
                return 2;
                break;
            case '*':
                return 3;
                break;
            case '^':
                return 3;
                break;
            case '/':
                return 4;
                break;
            case '(':
                return 6;
                break;
            case ')':
                return 7;
                break;
            case 's':
            case 'c':
            case 't':
            case 'i':
            case 'o':
            case 'l':
            case 'n':
            case 'a':
                return 8;
                break;
            default:
                return -1;
            }
        }
        return -1;
    }

    void Next()
    {
        if (idx < m_word.size())
            sym = m_word[idx++].second;
        else
            sym = 0;
    }

    // E �� TE' 
    void E()
    {
        T();
        E1();
    }

    // E' �� +TE' | -TE' | �� 
    void E1()
    {
        if (sym == 1 || sym == 2) {
            Next();
            T();
            E1();
        }
        else if (sym != 7 && sym != 0) {
            err = -1;
        }
    }

    // T �� FT'
    void T()
    {
         F();
        T1();
    }

    // T' �� *FT' | /FT' | �� 
    void T1()
    {
        if (sym == 3)
        {
            Next();
            F();
            T1();
        }
        else if (sym == 4)
        {
            Next();
            F();
            T1();
        }
        else if (sym != 1 && sym != 2 && sym != 7 && sym != 0)
        {
            err = -1;
        }
    }

    // F �� opF |(E) | d
    void F()
    {
        if (sym == 8)
        {
            Next();
            F();
        }
        else if (sym == 5)
        {
            Next();
        }
        else if (sym == 6)
        {
            Next();
            E();
            if (sym == 7)
            {
                Next();
            }
            else
            {
                err = -1;
            }
        }
        else
        {
            err = -1;
        }
    }

    //����﷨�Ƿ���ȷ
    bool ExpIsRight(){
        // �ʷ���ȷ�������﷨����
        Next();
        E();
        m_result = (sym == 0 && err == 0);  // Ҫ�ж���������
        return m_result;
    }
};


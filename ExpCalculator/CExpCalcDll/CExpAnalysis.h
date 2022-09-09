/**************************************************
*  作者: momo
*        maozi888888@hotmail.com
*  日期:2022-09-06
*  功能描述:
*      CInfixExpAnalysis用于验证CExpCalc中缀算数表达式是否符合表达式语法
* 
*  数学表达式文法如下:
*    G(E)
*    E -> E+T | E-T              
*    T -> T*F | T/F | F     
*    F -> OpF | (E) | d                (Op:单目运算符; d:数字)
*  左递归消除
*    E ->  TE'
*    E'-> +TE' | -TE' | ε
*    T ->  FT'
*    T'-> *FT' | /FT' | ε
*    F -> OpF  | (E) | d
* 
*   运算符类型：
*      '+':1    
*      '-':2    （+,- 可以是一个类型，未合并）
*      '*':3;
*      '^':3;
*      '/':4;   （*./,^ 这些双目运算符可以是一个类型，合并了*和^）
*        d:5    （数字，变量 认为是数字）
*      '(':6;
*      ')':7;
*      's','c','t','i','o','l','n','a': 8  （三角函数,对数,绝对值等单目运算符 一个类型）
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

    //创建词法类型数组
    void CreateWord(vector<std::string>* exp) {
        vector<std::string>::iterator it = exp->begin();
        while (it != exp->end()) {
            m_word.push_back(make_pair(*it, GetType(*it)));
            it++;
        }
        //ExpIsRight();
    }

    //是否是数字（包含变量-变量也为数字）
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
        if (isNumber(str))  //数字
            return 5;
        else {
            switch (str[0]) {//运算符           
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

    // E → TE' 
    void E()
    {
        T();
        E1();
    }

    // E' → +TE' | -TE' | ε 
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

    // T → FT'
    void T()
    {
         F();
        T1();
    }

    // T' → *FT' | /FT' | ε 
    void T1()
    {
        if (sym == 3) {
            Next();
            F();
            T1();
        }
        else if (sym == 4) {
            Next();
            F();
            T1();
        }
        else if (sym != 1 && sym != 2 && sym != 7 && sym != 0) {
            err = -1;
        }
    }

    // F → opF |(E) | d
    void F()
    {
        if (sym == 8) {
            Next();
            F();
        }
        else if (sym == 5) {
            Next();
        }
        else if (sym == 6) {
            Next();
            E();
            if (sym == 7)  {
                Next();
            }
            else {
                err = -1;
            }
        }
        else {
            err = -1;
        }
    }

    //检查语法是否正确
    bool ExpIsRight(){
        // 词法正确，进行语法分析
        Next();
        E();
        m_result = (sym == 0 && err == 0);  // 要判断两个条件
        return m_result;
    }
};


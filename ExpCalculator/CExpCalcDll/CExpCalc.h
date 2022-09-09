/**************************************************
*  作者: momo 
*        maozi888888@hotmail.com
*  日期:2022-08-31
*  功能描述:
*      CExpCalc用于计算算数计算表达式的结果
* 
*      数学表达式支持运算符：
*          sin     --- 正弦
*          cos     --- 余弦
*          tan     --- 正切
*          arcsin  --- 反正弦
*          arccos  --- 反余弦
*          lg      --- 10为底对数
*          ln      --- e为底对数
*          abs     --- 绝对值
*      三角函数使用弧度单位
*      
*      支持常数 @开头: 
*          @pi      --- 圆周率
*          @e       --- 自然常数
* 
*      支持标签变量运算， 中文标签，英文标签需要加"[]"
*      如：　速度*力
*            [Speed]*[Force]
*      
*      用一个 map<string, double> 存储标签和变量的值，带入计算  
* 
*      计算结果：返回6位小数的字符串或者错误信息
*      计算错误信息：
*         #REF!   --- 调用了不存在的标签
*         #LOG-   --- 对负数求对数
*         #DIV/0  --- 除数为0
*         #ERR    --- 表达式错误
* 
**************************************************/

#if !defined(AFX_EXPCALC_H__FEE57418_37DC_4347_B8FF_4F75D0F65A6F__INCLUDED_)
#define AFX_EXPCALC_H__FEE57418_37DC_4347_B8FF_4F75D0F65A6F__INCLUDED_

#pragma once

#include <string>
#include <vector>
#include <stack>
#include <map>
#include "ExpCalcInterface.h"
 

using namespace std;

class CExpCalc:public CExpCalcInterface
{
public:
    CExpCalc();
    virtual ~CExpCalc() {}

    //重置
    void Reset();

    //创建表达式
    bool CreateExpression(std::string exp);

    //设置变量表
    void SetVariableMap(map<std::string, double>* datamap);

    //得到最终结果
    std::string GetClacResult();

private:
    //中缀表达式
    std::string m_exp;

    //中缀表达式分割得到 数字、变量、运算符 的数组
    vector<std::string> splitExp(std::string s);

    //后缀表达式分割数组
    vector<std::string> SuffixExp;

    //结果字符串，计算表达式的值，返回#ERR，或数字
    std::string m_rs;

    //计算结果
    std::string calcstr;

    //变量Map
    map<std::string, double>* variable_map;

private:
    //预处理
    bool ExpPreprocessing();

    //表达式正确性
    //bool isError();

    //中缀表达式转后缀表达式
    bool toSuffixExp(vector<std::string>& expvector, vector<std::string>& suffixexp);

    //是否是运算符
    bool isOp(char s);
    bool isOp(std::string str);
    
    //是否是数字（包含变量-变量也为数字）
    bool isNumber(const std::string& str);

    //运算符优先级矩阵
    const char Prior[10][10] =
    { // 运算符优先级表   
              // '+' '-'   '*'  '/'  '('  ')'  '#'  '^'  's,a,t'
        /*'+'*/{ '>', '>', '<', '<', '<', '>', '>' , '<', '<'},
        /*'-'*/{ '>', '>', '<', '<', '<', '>', '>' , '<', '<'},
        /*'*'*/{ '>', '>', '>', '>', '<', '>', '>' , '<', '<'},
        /*'/'*/{ '>', '>', '>', '>', '<', '>', '>' , '<', '<'},
        /*'('*/{ '<', '<', '<', '<', '<', '=', '<' , '<', '<'},
        /*')'*/{ '>', '>', '>', '>', ' ', '>', '>' , '>', '>'},
        /*'#'*/{ '<', '<', '<', '<', '<', '>', '=' , '<', '<'},
        /*'^'*/{ '>', '>', '>', '>', '<', '>', '>' , '>', '<'},
        /*'s'*/{ '>', '>', '>', '>', '<', '>', '>' , '>', '>'},
        /*'a'*/{ '>', '>', '>', '>', '<', '>', '>' , '>', '>'},
    };

    //运算符优先级比较
    char Precede(char op1, char op2);
    char Precede(std::string op1, std::string op2);
    int Opid(char op1);

    //运算符运算
    bool Calc(char op, double b, double& rs);   //单目运算
    bool Calc(double a, char op, double b, double& rs); //双目运算

    //计算后缀表达式
    std::string CalcExp(vector<std::string> suffixexp);

};

#endif
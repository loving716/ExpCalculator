/**************************************************
*  ����: momo 
*        maozi888888@hotmail.com
*  ����:2022-08-31
*  ��������:
*      CExpCalc���ڼ�������������ʽ�Ľ��
* 
*      ��ѧ���ʽ֧���������
*          sin     --- ����
*          cos     --- ����
*          tan     --- ����
*          arcsin  --- ������
*          arccos  --- ������
*          lg      --- 10Ϊ�׶���
*          ln      --- eΪ�׶���
*          abs     --- ����ֵ
*      ���Ǻ���ʹ�û��ȵ�λ
*      
*      ֧�ֳ��� @��ͷ: 
*          @pi      --- Բ����
*          @e       --- ��Ȼ����
* 
*      ֧�ֱ�ǩ�������㣬 ���ı�ǩ��Ӣ�ı�ǩ��Ҫ��"[]"
*      �磺���ٶ�*��
*            [Speed]*[Force]
*      
*      ��һ�� map<string, double> �洢��ǩ�ͱ�����ֵ���������  
* 
*      ������������6λС�����ַ������ߴ�����Ϣ
*      ���������Ϣ��
*         #REF!   --- �����˲����ڵı�ǩ
*         #LOG-   --- �Ը��������
*         #DIV/0  --- ����Ϊ0
*         #ERR    --- ���ʽ����
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

    //����
    void Reset();

    //�������ʽ
    bool CreateExpression(std::string exp);

    //���ñ�����
    void SetVariableMap(map<std::string, double>* datamap);

    //�õ����ս��
    std::string GetClacResult();

private:
    //��׺���ʽ
    std::string m_exp;

    //��׺���ʽ�ָ�õ� ���֡������������ ������
    vector<std::string> splitExp(std::string s);

    //��׺���ʽ�ָ�����
    vector<std::string> SuffixExp;

    //����ַ�����������ʽ��ֵ������#ERR��������
    std::string m_rs;

    //������
    std::string calcstr;

    //����Map
    map<std::string, double>* variable_map;

private:
    //Ԥ����
    bool ExpPreprocessing();

    //���ʽ��ȷ��
    //bool isError();

    //��׺���ʽת��׺���ʽ
    bool toSuffixExp(vector<std::string>& expvector, vector<std::string>& suffixexp);

    //�Ƿ��������
    bool isOp(char s);
    bool isOp(std::string str);
    
    //�Ƿ������֣���������-����ҲΪ���֣�
    bool isNumber(const std::string& str);

    //��������ȼ�����
    const char Prior[10][10] =
    { // ��������ȼ���   
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

    //��������ȼ��Ƚ�
    char Precede(char op1, char op2);
    char Precede(std::string op1, std::string op2);
    int Opid(char op1);

    //���������
    bool Calc(char op, double b, double& rs);   //��Ŀ����
    bool Calc(double a, char op, double b, double& rs); //˫Ŀ����

    //�����׺���ʽ
    std::string CalcExp(vector<std::string> suffixexp);

};

#endif
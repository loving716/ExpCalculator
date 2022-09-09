#pragma once

#include <string>
#include <vector>
#include <stack>
#include <map>
using namespace std;

#ifdef EXPCALC_EXPORTS
#define CLASS_CEXP_INT __declspec(dllexport)//��ʾ����Ҫ���ർ��//
#else
#define CLASS_CEXP_INT __declspec(dllimport)//��ʾ����Ҫ���ർ��//
#endif

class CLASS_CEXP_INT CExpCalcInterface {
public:
    CExpCalcInterface() {};
    virtual ~CExpCalcInterface() {};

public:
    //����
    virtual void Reset() = 0;

    //�������ʽ
    virtual bool CreateExpression(std::string exp) = 0;

    //���ñ�����
    virtual void SetVariableMap(map<std::string, double>* datamap) = 0;

    //�õ����ս��
    virtual std::string GetClacResult() = 0;

};


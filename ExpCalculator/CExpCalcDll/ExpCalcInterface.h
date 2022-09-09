#pragma once

#include <string>
#include <vector>
#include <stack>
#include <map>
using namespace std;

#ifdef EXPCALC_EXPORTS
#define CLASS_CEXP_INT __declspec(dllexport)//表示这里要把类导出//
#else
#define CLASS_CEXP_INT __declspec(dllimport)//表示这里要把类导出//
#endif

class CLASS_CEXP_INT CExpCalcInterface {
public:
    CExpCalcInterface() {};
    virtual ~CExpCalcInterface() {};

public:
    //重置
    virtual void Reset() = 0;

    //创建表达式
    virtual bool CreateExpression(std::string exp) = 0;

    //设置变量表
    virtual void SetVariableMap(map<std::string, double>* datamap) = 0;

    //得到最终结果
    virtual std::string GetClacResult() = 0;

};


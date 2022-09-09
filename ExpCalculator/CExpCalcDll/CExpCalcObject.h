#pragma once

#include "ExpCalcInterface.h"

#ifdef EXPCALC_EXPORTS
#define CLASS_CEXP __declspec(dllexport)//表示这里要把类导出//
#else
#define CLASS_CEXP __declspec(dllimport)//表示这里要把类导出//
#endif

class CLASS_CEXP CExpCalcObject
{
public:
    CExpCalcObject();
    ~CExpCalcObject();

    CExpCalcInterface* CreateObj();
    void DelObj(CExpCalcInterface* p_interface);
private:
    CExpCalcInterface* m_pObj;
};


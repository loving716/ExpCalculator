#pragma once

#include "ExpCalcInterface.h"

#ifdef EXPCALC_EXPORTS
#define CLASS_CEXP __declspec(dllexport)//��ʾ����Ҫ���ർ��//
#else
#define CLASS_CEXP __declspec(dllimport)//��ʾ����Ҫ���ർ��//
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


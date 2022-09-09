#include "pch.h"
#include "CExpCalcObject.h"
#include "CExpCalc.h"

CExpCalcObject::CExpCalcObject() {
    m_pObj = nullptr;
}

CExpCalcObject::~CExpCalcObject() {
    DelObj(m_pObj);
}

CExpCalcInterface* CExpCalcObject::CreateObj() {
    CExpCalcInterface *p =  new CExpCalc();
    m_pObj = p;
    return p;
}

void CExpCalcObject::DelObj(CExpCalcInterface* p_interface){
    if (p_interface)
        delete p_interface;
    m_pObj = nullptr;
}
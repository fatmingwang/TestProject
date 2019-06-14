#include "stdafx.h"
#include "TestObjectHay.h"

TYPDE_DEFINE_MARCO(cTestObjectHay);

void cTestObjectHay::InternalInit()
{
}

void cTestObjectHay::InternalUpdate(float e_fElpaseTime)
{
}

void cTestObjectHay::InternalRender()
{
}

cTestObjectHay::cTestObjectHay()
{
}

cTestObjectHay::cTestObjectHay(cTestObjectHay * e_pTestObjectBase):cTestObjectBase(e_pTestObjectBase)
{
}

cTestObjectHay::cTestObjectHay(TiXmlElement * e_pTiXmlElement): cTestObjectBase(e_pTiXmlElement)
{
}

cTestObjectHay::~cTestObjectHay()
{
}

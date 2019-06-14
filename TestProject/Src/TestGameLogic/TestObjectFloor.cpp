#include "stdafx.h"
#include "TestObjectFloor.h"

TYPDE_DEFINE_MARCO(cTestObjectFloor);

void cTestObjectFloor::InternalInit()
{
}

void cTestObjectFloor::InternalUpdate(float e_fElpaseTime)
{
}

void cTestObjectFloor::InternalRender()
{
}

cTestObjectFloor::cTestObjectFloor()
{
}

cTestObjectFloor::cTestObjectFloor(cTestObjectFloor * e_pTestObjectBase):cTestObjectBase(e_pTestObjectBase)
{
}

cTestObjectFloor::cTestObjectFloor(TiXmlElement * e_pTiXmlElement): cTestObjectBase(e_pTiXmlElement)
{
}

cTestObjectFloor::~cTestObjectFloor()
{
}

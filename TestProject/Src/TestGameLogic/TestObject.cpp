#include "stdafx.h"
#include "TestObject.h"

cTestObjectBase::cTestObjectBase()
{
}

cTestObjectBase::cTestObjectBase(cTestObjectBase * e_pTestObjectBase)
{
}
//<Wall ID = "0" Name = "Wall" HP = "100" PI = "TestGame/AA.pi" PIUnit = "3" >
cTestObjectBase::cTestObjectBase(TiXmlElement * e_pTiXmlElement)
{

}

cTestObjectBase::~cTestObjectBase()
{
}

void cTestObjectBase::Init()
{
}

void cTestObjectBase::Update(float e_fElpaseTime)
{
}

void cTestObjectBase::Render()
{
}

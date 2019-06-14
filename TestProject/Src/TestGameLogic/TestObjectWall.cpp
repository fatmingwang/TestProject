#include "stdafx.h"
#include "TestObjectWall.h"

TYPDE_DEFINE_MARCO(cTestObjectWall);

void cTestObjectWall::InternalInit()
{
}

void cTestObjectWall::InternalUpdate(float e_fElpaseTime)
{
}

void cTestObjectWall::InternalRender()
{
}

cTestObjectWall::cTestObjectWall()
{
}

cTestObjectWall::cTestObjectWall(cTestObjectWall * e_pTestObjectBase) :cTestObjectBase(e_pTestObjectBase)
{
}

cTestObjectWall::cTestObjectWall(TiXmlElement * e_pTiXmlElement) : cTestObjectBase(e_pTiXmlElement)
{
}

cTestObjectWall::~cTestObjectWall()
{
}

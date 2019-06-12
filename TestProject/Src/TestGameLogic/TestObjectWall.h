#pragma once

#include "TestObject.h"

class cTestObjectWall :public cTestObjectBase
{
	virtual	void	InternalInit()override;
	virtual	void	InternalUpdate(float e_fElpaseTime)override;
	virtual	void	InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	cTestObjectWall();
	cTestObjectWall(cTestObjectWall*e_pTestObjectBase);
	cTestObjectWall(TiXmlElement*e_pTiXmlElement);
	virtual ~cTestObjectWall();
	CLONE_MYSELF(cTestObjectWall);
	GET_ME_WITH_BASE_TYPE(cTestObjectWall, cTestObjectBase);
};
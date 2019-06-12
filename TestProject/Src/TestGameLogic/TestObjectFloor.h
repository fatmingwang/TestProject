#pragma once

#include "TestObject.h"

class cTestObjectFloor :public cTestObjectBase
{
	virtual	void	InternalInit()override;
	virtual	void	InternalUpdate(float e_fElpaseTime)override;
	virtual	void	InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	cTestObjectFloor();
	cTestObjectFloor(cTestObjectFloor*e_pTestObjectBase);
	cTestObjectFloor(TiXmlElement*e_pTiXmlElement);
	virtual ~cTestObjectFloor();
	CLONE_MYSELF(cTestObjectFloor);
	GET_ME_WITH_BASE_TYPE(cTestObjectFloor, cTestObjectBase);
};
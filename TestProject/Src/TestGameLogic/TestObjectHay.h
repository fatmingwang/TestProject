#pragma once

#include "TestObject.h"


class cTestObjectHay :public cTestObjectBase
{
	virtual	void	InternalInit()override;
	virtual	void	InternalUpdate(float e_fElpaseTime)override;
	virtual	void	InternalRender()override;
public:
	DEFINE_TYPE_INFO();
	cTestObjectHay();
	cTestObjectHay(cTestObjectHay*e_pTestObjectBase);
	cTestObjectHay(TiXmlElement*e_pTiXmlElement);
	virtual ~cTestObjectHay();
	CLONE_MYSELF(cTestObjectHay);
	GET_ME_WITH_BASE_TYPE(cTestObjectHay, cTestObjectBase);
};
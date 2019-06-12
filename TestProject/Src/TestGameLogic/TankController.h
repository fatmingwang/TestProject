#pragma once

#include "TestTank.h"

class cTestTankController :public cRenderObject,public cNamedTypedObjectVector<cTestTank>
{
	cTestTank*	m_pSelectedTank;

	cTestTank*	SelectNextTank();
	//
	bool										KeyEventFuntion(void *e_pData);
public:
	cTestTankController();
	virtual ~cTestTankController();
	virtual	NamedTypedObject*	Clone()override;
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render()override;
	virtual	void	DebugRender()override;
};
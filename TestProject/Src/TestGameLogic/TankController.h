#pragma once

#include "TestTank.h"
#include "TankBullet.h"
//bullet collision is here!.
class cTestTankController :public cRenderObject,public cNamedTypedObjectVector<cTestTank>, public cNodeISAX,public cMessageSender
{
	virtual	bool									MyParse(TiXmlElement*e_pRoot)override;
	//
	cWaitForFetchFunctionObjectList<cTankBullet>*	m_pTankBulletVector;
	//
	cTestTank*	m_pSelectedTank;
	//
	bool											FireEvent(void *e_pData);
	//
	void											BulletUpdate(float e_fElpaseTime);
	void											TankUpdate(float e_fElpaseTime);
	void											BulletRender();
	void											TankRender();
public:
	cTestTankController();
	virtual ~cTestTankController();
	virtual	NamedTypedObject*	Clone()override;
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render()override;
	virtual	void	DebugRender()override;

	cTestTank*		SelectNextTank();
	cTestTank*		GetSelectedTank() { return m_pSelectedTank; }
};
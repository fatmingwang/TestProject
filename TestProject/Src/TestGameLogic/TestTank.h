#pragma once

#include <functional>

struct sMoving
{

};

class cTestTank :public cRenderObject,public cMessageSender
{
	enum eTankStatus
	{
		eTS_IDLE = 0,
		eTS_MOVING,
		eTS_FIRE,
		eTS_CHANGE_DIRECTION,
		eTS_MAX,
	};
	enum eKey
	{
		eK_LEFT = 0,
		eK_UP,
		eK_RIGHT,
		eK_DOWN,
		eK_FIRE,
		eK_MAX
	};
	eKey										GetKey(const wchar_t*e_strKey);
	eTankStatus									GetTankStatus(const wchar_t*e_strKey);
	unsigned char								m_ucKey[eK_MAX];
	cMPDI*										m_Animation[eTS_MAX];
	bool										m_bAllowToDoNewCommand;
	//
	eDirection									m_eDirection;
	void										DirectionUpdate(float e_fElpaseTime);
	void										MovingUpdate(float e_fElpaseTime);
	void										FireUpdate(float e_fElpaseTime);
	void										IdleUpdate(float e_fElpaseTime);
	//
	std::vector<std::function<void(float)> >	m_UpdateFunctionVector;
	//
	bool										KeyEventFuntion(void *e_pData);
	//
	void										ProcessKeyBindingData(TiXmlElement*e_pTiXmlElement);
	void										ProcessAnimationData(TiXmlElement*e_pTiXmlElement);
public:
	cTestTank();
	cTestTank(TiXmlElement*e_pTiXmlElement);
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render()override;
	virtual	void	DebugRender()override;
	bool			IsAllowToDoNewCommand();//allow to seitch tank.
};
#pragma once

#include <functional>

class cTestTank :public cRenderObject,public cMessageSender
{
	friend class		cTestTankController;
	enum eTankStatus
	{
		eTS_LEFT = 0,
		eTS_UP,
		eTS_RIGHT,
		eTS_DOWN,
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
	eTankStatus									m_CurrentTankStatus;
	eDirection									m_eDirection;
	//unsigned char								m_ucKey[eK_MAX];//lazy lah
	Vector4										m_vColor;
	int											m_iPower;
	cBaseImage*									m_pAnimation[eTS_MAX];//lazy larh
	bool										m_bAllowToDoNewCommand;//lazy larh
	//
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
	//
	int											m_iTableIndex;
public:
	cTestTank();
	cTestTank(TiXmlElement*e_pTiXmlElement);
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render()override;
	virtual	void	DebugRender()override;
	bool			IsAllowToDoNewCommand();//allow to seitch tank.
	int				GetTableIndex();
	void			SetTableIndex(int e_iTableIndex,eDirection e_eDirection);
	void			Fire();
};
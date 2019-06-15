#pragma once

class cTankBullet:public cRenderObject
{
	Vector4				m_vColor;
	friend class		cTestTankController;
	UT::sTimeCounter	m_LifeTC;
	cBaseImage*			m_pBulletImage;
	UT::sTimeCounter	m_MoveSpeedTC;
	UT::sTimeCounter	m_LifeTimeTC;
	const float			m_fMaxSpeed = 50.f;
	const float			m_fIncreaseSpeed = 0.36f;
	float				m_fAcceleration;
	int					m_iTableIndex;
	POINT				m_Velocity;
	eDirection			m_eMovingDirection;
	int					m_iDamage;
public:
	cTankBullet();
	cTankBullet(TiXmlElement*e_pTiXmlElement);
	cTankBullet(cTankBullet*e_pTankBullet);
	virtual ~cTankBullet();
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render()override;
	void			Fire(int e_iTableIndex,POINT e_Velocity,int e_iDamage,Vector4 e_vColor);
	bool			IsOutOfFuel();
};
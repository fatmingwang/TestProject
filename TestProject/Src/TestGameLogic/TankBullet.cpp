#include "stdafx.h"
#include "TankBullet.h"
#include "TestMap.h"
cTankBullet::cTankBullet()
{
	m_iDamage = 0;
	m_fAcceleration = 1.f;
	m_pBulletImage = nullptr;
}
////<TestBullet PI = "Test/Image.pi" PIUnit = "NPC_2" / >
cTankBullet::cTankBullet(TiXmlElement * e_pTiXmlElement)
{
	m_fAcceleration = 1.f;
	m_pBulletImage = nullptr;
	auto l_pGridData = cTestMap::GetInstance()->GetGridData();
	cPuzzleImage*l_pPI = nullptr;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("PI")
		{
			l_pPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
		}
		else
		COMPARE_NAME("PIUnit")
		{
			if (l_pPI)
			{
				m_pBulletImage = l_pPI->GetObject(l_strValue);
				if (m_pBulletImage)
				{
					m_pBulletImage->SetHeight(l_pGridData->iGridHeight);
					m_pBulletImage->SetWidth(l_pGridData->iGridWidth);
				}
			}
		}
	TO_NEXT_VALUE
}

cTankBullet::cTankBullet(cTankBullet * e_pTankBullet)
{
	m_pBulletImage = e_pTankBullet->m_pBulletImage;
	m_fAcceleration = e_pTankBullet->m_fAcceleration;
}

cTankBullet::~cTankBullet()
{
}

void cTankBullet::Init()
{
	m_fAcceleration = 1.f;
	this->m_bVisible = true;
}

void cTankBullet::Update(float e_fElpaseTime)
{
	m_LifeTimeTC.Update(e_fElpaseTime);
	if (m_LifeTimeTC.bTragetTimrReached)
	{
		return;
	}
	m_MoveSpeedTC.Update(e_fElpaseTime*m_fAcceleration);
	m_fAcceleration += m_fIncreaseSpeed;
	if (m_fAcceleration > m_fMaxSpeed)
		m_fAcceleration = m_fMaxSpeed;
}

void cTankBullet::Render()
{
	if (this->m_bVisible && this->m_pBulletImage && m_fAcceleration != 1.f)
	{
		m_pBulletImage->SetLocalPosition(this->GetWorldPosition());
		m_pBulletImage->SetColor(m_vColor);
		m_pBulletImage->Render();
	}
	//UT::sTimeCounter	m_MoveSpeedTC;
	//const float			m_fMaxSpeed = 50.f;
	//const float			m_fIncreaseSpeed = 0.36f;
}

void cTankBullet::Fire(int e_iTableIndex, POINT e_Velocity,int e_iDamage, Vector4 e_vColor)
{
	m_vColor = e_vColor;
	m_iDamage = e_iDamage;
	m_iTableIndex = e_iTableIndex;
	m_Velocity = e_Velocity;
	if (m_Velocity.x > 0)
	{
		m_eMovingDirection = eDirection::eD_RIGHT;
	}
	else
	if (m_Velocity.x < 0)
	{
		m_eMovingDirection = eDirection::eD_LEFT;
	}
	else
	if (m_Velocity.y > 0)
	{
		m_eMovingDirection = eDirection::eD_DOWN;
	}
	else
	if (m_Velocity.y < 0)
	{
		m_eMovingDirection = eDirection::eD_UP;
	}
	m_LifeTimeTC.Start();
	m_MoveSpeedTC.bTragetTimrReached = true;
	m_fAcceleration = 1.f;
}

bool cTankBullet::IsOutOfFuel()
{
	return m_LifeTimeTC.bTragetTimrReached;
}

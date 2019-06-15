#include "stdafx.h"
#include "TestTank.h"
#include "LogID.h"
#include "Event.h"
#include "TestMap.h"

cTestTank::cTestTank()
{
	m_iPower = 0;
	//m_UpdateFunctionVector.push_back(std::bind(&cTestTank::DirectionUpdate, this, std::placeholders::_1));
	//m_UpdateFunctionVector.push_back(std::bind(&cTestTank::MovingUpdate, this, std::placeholders::_1));
	//m_UpdateFunctionVector.push_back(std::bind(&cTestTank::FireUpdate, this, std::placeholders::_1));
	//m_UpdateFunctionVector.push_back(std::bind(&cTestTank::IdleUpdate, this, std::placeholders::_1));
	//if(m_UpdateFunctionVector.size() != eTS_MAX)
		//FMLog::LogWithFlag("UpdateFunction size not match,new status forgot to add update function!?", LOG_ID_TANK, false);
	m_CurrentTankStatus = eTS_DOWN;
	for (int i = 0; i < eTS_MAX; ++i)
	{
		m_pAnimation[i] = nullptr;
	}
	REG_EVENT(eT_E_KEY_DOWN, &cTestTank::KeyEventFuntion);
}

cTestTank::cTestTank(TiXmlElement * e_pTiXmlElement)
{
	m_iPower = 0;
	//m_UpdateFunctionVector.push_back(std::bind(&cTestTank::DirectionUpdate, this, std::placeholders::_1));
	//m_UpdateFunctionVector.push_back(std::bind(&cTestTank::MovingUpdate, this, std::placeholders::_1));
	//m_UpdateFunctionVector.push_back(std::bind(&cTestTank::FireUpdate, this, std::placeholders::_1));
	//m_UpdateFunctionVector.push_back(std::bind(&cTestTank::IdleUpdate, this, std::placeholders::_1));
	for (int i = 0; i < eTS_MAX; ++i)
	{
		m_pAnimation[i] = nullptr;
	}
	auto l_strColor = e_pTiXmlElement->Attribute(L"Color");
	if (l_strColor)
	{
		this->m_vColor = GetVector4(l_strColor);
	}
	auto l_strTankName = e_pTiXmlElement->Attribute(L"Name");
	if (l_strTankName)
	{
		this->SetName(l_strTankName);
	}
	auto l_strPower = e_pTiXmlElement->Attribute(L"Power");
	if (l_strPower)
	{
		m_iPower = GetInt(l_strPower);
	}
	m_CurrentTankStatus = eTS_DOWN;
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pTiXmlElement)
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pTiXmlElement, L"KeyBinding")
		{
			//ProcessKeyBindingData(e_pTiXmlElement);
		}
		else
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pTiXmlElement, L"AnimationData")
		{
			ProcessAnimationData(e_pTiXmlElement);
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pTiXmlElement)
}

bool cTestTank::KeyEventFuntion(void * e_pData)
{
	//if (m_bAllowToDoNewCommand)
	//{
	//	seT_E_KEY_DOWNData*l_peT_E_KEY_DOWNData = (seT_E_KEY_DOWNData*)e_pData;
	//	if (l_peT_E_KEY_DOWNData)
	//	{
	//		l_peT_E_KEY_DOWNData->ucKey;
	//	}
	//}
	return true;
}
cTestTank::eKey cTestTank::GetKey(const wchar_t * e_strKey)
{
	if(wcscmp(L"Left", e_strKey) == 0)
	{
		return eK_LEFT;
	}
	else
	if(wcscmp(L"Up", e_strKey) == 0)
	{
		return eK_UP;
	}
	else
	if(wcscmp(L"Right", e_strKey) == 0)
	{
		return eK_RIGHT;
	}
	else
	if(wcscmp(L"Down", e_strKey) == 0)
	{
		return eK_DOWN;
	}
	else
	if(wcscmp(L"Fire", e_strKey) == 0)
	{
		return eK_FIRE;
	}
	FMLog::LogWithFlag(UT::ComposeMsgByFormat(L"unsupport key:%ls", e_strKey), LOG_ID_TANK, true);
	return eK_MAX;
}

cTestTank::eTankStatus cTestTank::GetTankStatus(const wchar_t * e_strKey)
{
	if(wcscmp(L"Left", e_strKey) == 0)
	{
		return eTS_LEFT;
	}
	else
	if(wcscmp(L"Up", e_strKey) == 0)
	{
		return eTS_UP;
	}
	else
	if(wcscmp(L"Right", e_strKey) == 0)
	{
		return eTS_RIGHT;
	}
	else
	if(wcscmp(L"Down", e_strKey) == 0)
	{
		return eTS_DOWN;
	}

	FMLog::LogWithFlag(UT::ComposeMsgByFormat(L"unsupport TankStatus key:%ls", e_strKey), LOG_ID_TANK, true);
	return eTS_MAX;
}

//========================
//<KeyBinding Left="" Up="" Right="" Dowan="" Fire="" />
//========================
void cTestTank::ProcessKeyBindingData(TiXmlElement * e_pTiXmlElement)
{
	//lazy larh
	//PARSE_ELEMENT_START(e_pTiXmlElement)
	//	auto l_eKey = GetKey(l_strName);
	//	if(l_eKey != eK_MAX)
	//	{
	//		m_ucKey[l_eKey] = GetInt(l_strValue);
	//	}
	//TO_NEXT_VALUE
}
//========================
//<AnimationData PI="" Left="" Up="" Right="" Down="" />
//========================
void cTestTank::ProcessAnimationData(TiXmlElement * e_pTiXmlElement)
{
	auto l_pGridData = cTestMap::GetInstance()->GetGridData();
	cPuzzleImage*l_pPI = nullptr;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("PI")
		{
			l_pPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
		}
		else
		if(l_pPI)
		{
			auto l_eTS_MAX = GetTankStatus(l_strName);
			if(l_eTS_MAX != eTS_MAX)
			{
				auto l_pPIUnit = l_pPI->GetObject(l_strValue);
				if (l_pPIUnit)
				{
					m_pAnimation[l_eTS_MAX] = l_pPIUnit;
					l_pPIUnit->SetWidth(l_pGridData->iGridWidth);
					l_pPIUnit->SetHeight(l_pGridData->iGridHeight);
				}
			}
		}
	TO_NEXT_VALUE	
}

void cTestTank::Init()
{
	this->m_bVisible = true;
}

void cTestTank::Update(float e_fElpaseTime)
{
	//if (m_UpdateFunctionVector.size() > m_CurrentTankStatus)
	//{
	//	m_UpdateFunctionVector[this->m_CurrentTankStatus](e_fElpaseTime);
	//}
	if (m_pAnimation[m_CurrentTankStatus])
	{
		m_pAnimation[m_CurrentTankStatus]->Update(e_fElpaseTime);
	}
}

void cTestTank::Render()
{
	if (this->m_bVisible && m_pAnimation[m_CurrentTankStatus])
	{
		auto l_vPos = this->GetWorldPosition();
		m_pAnimation[m_CurrentTankStatus]->SetLocalPosition(l_vPos);
		m_pAnimation[m_CurrentTankStatus]->SetColor(m_vColor);
		m_pAnimation[m_CurrentTankStatus]->Render();
	}
}

void cTestTank::DebugRender()
{
}

void cTestTank::DirectionUpdate(float e_fElpaseTime)
{
	{
//		m_bAllowToDoNewCommand = true;
	}
}

void cTestTank::MovingUpdate(float e_fElpaseTime)
{
}

void cTestTank::FireUpdate(float e_fElpaseTime)
{
}

void cTestTank::IdleUpdate(float e_fElpaseTime)
{
}

bool cTestTank::IsAllowToDoNewCommand()
{
	return m_bAllowToDoNewCommand;
}

int cTestTank::GetTableIndex()
{
	return m_iTableIndex;
}

void cTestTank::SetTableIndex(int e_iTableIndex, eDirection e_eDirection)
{
	m_eDirection = e_eDirection;
	switch (e_eDirection)
	{
		case eDirection::eD_LEFT:
			m_CurrentTankStatus = eTS_LEFT;
			break;
		case eDirection::eD_UP:
			m_CurrentTankStatus = eTS_UP;
			break;
		case eDirection::eD_RIGHT:
			m_CurrentTankStatus = eTS_RIGHT;
			break;
		case eDirection::eD_DOWN:
			m_CurrentTankStatus = eTS_DOWN;
			break;
		default:
			break;
	}
	m_iTableIndex = e_iTableIndex;
}

void cTestTank::Fire()
{
	seT_E_FIRE l_seT_E_FIRE;
	l_seT_E_FIRE.iDamage = this->m_iPower;
	l_seT_E_FIRE.vColor = this->m_vColor;
	l_seT_E_FIRE.iTableIndex = this->m_iTableIndex;
	l_seT_E_FIRE.Direction = this->m_eDirection;
	cGameApp::EventMessageShot(eT_E_FIRE, &l_seT_E_FIRE, sizeof(l_seT_E_FIRE));
}

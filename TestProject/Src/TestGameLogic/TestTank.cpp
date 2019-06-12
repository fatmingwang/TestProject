#include "stdafx.h"
#include "TestTank.h"
#include "LogID.h"
#include "Event.h"

cTestTank::cTestTank()
{
	m_UpdateFunctionVector.push_back(std::bind(&cTestTank::DirectionUpdate, this, std::placeholders::_1));
	m_UpdateFunctionVector.push_back(std::bind(&cTestTank::MovingUpdate, this, std::placeholders::_1));
	m_UpdateFunctionVector.push_back(std::bind(&cTestTank::FireUpdate, this, std::placeholders::_1));
	m_UpdateFunctionVector.push_back(std::bind(&cTestTank::IdleUpdate, this, std::placeholders::_1));
	if(m_UpdateFunctionVector.size() != eTS_MAX)
		FMLog::LogWithFlag("UpdateFunction size not match,new status forgot to add update function!?", LOG_ID_TANK, false);
	REG_EVENT(eT_E_KEY_DOWN, &cTestTank::KeyEventFuntion);
	
}

cTestTank::cTestTank(TiXmlElement * e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_VALUE("KeyBinding")
		{
			ProcessKeyBindingData(e_pTiXmlElement);
		}
		else
		COMPARE_VALUE("AnimationData")
		{
			ProcessAnimationData(e_pTiXmlElement);
		}
	PARSE_NAME_VALUE_END
}

bool cTestTank::KeyEventFuntion(void * e_pData)
{
	if (m_bAllowToDoNewCommand)
	{
		seT_E_KEY_DOWNData*l_peT_E_KEY_DOWNData = (seT_E_KEY_DOWNData*)e_pData;
		if (l_peT_E_KEY_DOWNData)
		{
			l_peT_E_KEY_DOWNData->ucKey;
		}
	}
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
	if(wcscmp(L"Idle", e_strKey) == 0)
	{
		return eTS_IDLE;
	}
	else
	if(wcscmp(L"Moving", e_strKey) == 0)
	{
		return eTS_MOVING;
	}
	else
	if(wcscmp(L"Fire", e_strKey) == 0)
	{
		return eTS_FIRE;
	}
	else
	if(wcscmp(L"eTS_CHANGE_DIRECTION", e_strKey) == 0)
	{
		return eTS_CHANGE_DIRECTION;
	}

	FMLog::LogWithFlag(UT::ComposeMsgByFormat(L"unsupport TankStatus key:%ls", e_strKey), LOG_ID_TANK, true);
	return eTS_MAX;
}

//========================
//<KeyBinding Left="" Up="" Right="" Dowan="" Fire="" />
//========================
void cTestTank::ProcessKeyBindingData(TiXmlElement * e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		auto l_eKey = GetKey(l_strName);
		if(l_eKey != eK_MAX)
		{
			m_ucKey[l_eKey] = GetInt(l_strValue);
		}
	TO_NEXT_VALUE
}
//========================
//<AnimationData cMPDIList="" Idle="" Moving="" Fire="" ChangeDirection="" />
//========================
void cTestTank::ProcessAnimationData(TiXmlElement * e_pTiXmlElement)
{
	cMPDIList*l_pMPDIList = nullptr;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("cMPDIList")
		{
			l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue);
		}
		else
		if(l_pMPDIList)
		{
			auto l_eTS_MAX = GetTankStatus(l_strName);
			if(l_eTS_MAX != eTS_MAX)
			{
				auto l_pMPDI = l_pMPDIList->GetObject(l_strValue);
				if (l_pMPDI)
				{
					m_Animation[l_eTS_MAX] = new cMPDI(l_pMPDI);
				}
			}
		}
	TO_NEXT_VALUE	
}

void cTestTank::Init()
{
}

void cTestTank::Update(float e_fElpaseTime)
{
	if (m_UpdateFunctionVector.size() > m_eDirection)
	{
		m_UpdateFunctionVector[this->m_eDirection](e_fElpaseTime);
	}
}

void cTestTank::Render()
{
}

void cTestTank::DebugRender()
{
}

void cTestTank::DirectionUpdate(float e_fElpaseTime)
{
	if (m_Animation[m_eDirection])
	{
		m_Animation[m_eDirection]->Update(e_fElpaseTime);
	}
	else
	{
		m_bAllowToDoNewCommand = true;
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

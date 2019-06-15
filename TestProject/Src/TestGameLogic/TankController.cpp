#include"stdafx.h"
#include "TankController.h"
#include "TestMap.h"
#include "Event.h"

//1.declare define
//cWaitForFetchFunctionObjectList<cMPDI>*		m_pVector;
//m_pVector = new cWaitForFetchFunctionObjectList<cMPDI>();
//2.push objects
//m_pVector->push_back(l_pCloneMPDI);
//3.to wait fetch list
//m_pVector->PushToWaitFetchList();
//require a object,if object is not enough it will new a instance with a warning message
//auto l_pMPDI = m_pVector->RequireAvaliableObject();
//4.do objects thing while object finish make it Finish
//for (int i = 0; i < m_pVector->m_WorkingList.m_iNumWorking; i++)
//{
//	auto l_pObject = m_pVector->m_WorkingList.m_ppObjects[i];
//	l_pObject->Update(e_fElpaseTime);
//	if (l_pObject->IsAnimationDone())
//	{
//		m_pVector->WorkObjectFinish(i);
//		--i;
//	}
//}

cTestTankController::cTestTankController()
{
	m_pTankBulletVector = new cWaitForFetchFunctionObjectList<cTankBullet>();
	m_pSelectedTank = nullptr;
	REG_EVENT(eT_E_FIRE, &cTestTankController::FireEvent);
}

cTestTankController::~cTestTankController()
{
	SAFE_DELETE(m_pTankBulletVector);
}

NamedTypedObject * cTestTankController::Clone()
{
	return nullptr;
}

bool cTestTankController::MyParse(TiXmlElement * e_pRoot)
{
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pRoot)
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pRoot, L"TestTank")
		{
			cTestTank*l_pTestTank = new cTestTank(e_pRoot);
			this->AddObjectNeglectExist(l_pTestTank);
		}
		else
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pRoot, L"TestBullet")
		{
			cTankBullet*l_pTankBullet = new cTankBullet(e_pRoot);
			m_pTankBulletVector->push_back(l_pTankBullet);
			for (int i = 0; i < 30; ++i)
			{
				cTankBullet*l_pCloneTankBullet = new cTankBullet(l_pTankBullet);
				m_pTankBulletVector->push_back(l_pCloneTankBullet);
			}
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pRoot)
	return true;
}

cTestTank * cTestTankController::SelectNextTank()
{
	int l_iCount = this->Count();
	int l_iIndex = this->GetObjectIndexByPointer(m_pSelectedTank);
	l_iIndex = UT::GetLoopIndex(l_iIndex+1, l_iCount);
	this->m_pSelectedTank = this->GetObject(l_iIndex);
	return m_pSelectedTank;
}

bool cTestTankController::FireEvent(void * e_pData)
{
	if (this->m_pTankBulletVector)
	{
		auto l_pBullet = m_pTankBulletVector->RequireAvaliableObject();
		seT_E_FIRE*l_peT_E_FIRE = (seT_E_FIRE*)e_pData;
		POINT l_Velocity = { 0,0 };
		switch (l_peT_E_FIRE->Direction)
		{
			case eDirection::eD_LEFT:	l_Velocity.x = -1; break;
			case eDirection::eD_UP:		l_Velocity.y = -1; break;
			case eDirection::eD_RIGHT:	l_Velocity.x = 1; break;
			case eDirection::eD_DOWN:	l_Velocity.y = 1; break;
			default:break;
		}
		l_pBullet->Fire(l_peT_E_FIRE->iTableIndex, l_Velocity, l_peT_E_FIRE->iDamage, l_peT_E_FIRE->vColor);
		auto l_pTestMapInstance = cTestMap::GetInstance();
		auto l_pGridData = l_pTestMapInstance->GetGridData();
		auto l_Table = l_pTestMapInstance->TableIndexToLocalXY(l_pBullet->m_iTableIndex);
		l_pBullet->SetLocalPosition(Vector3((float)l_Table.x * l_pGridData->iGridWidth, (float)l_Table.y * l_pGridData->iGridHeight, 0));
	}
	return true;
}

void cTestTankController::BulletUpdate(float e_fElpaseTime)
{
	auto l_pTestMapInstance = cTestMap::GetInstance();
	auto l_pGridData = l_pTestMapInstance->GetGridData();
	for (int i = 0; i < m_pTankBulletVector->m_WorkingList.m_iNumWorking; i++)
	{
		auto l_pObject = m_pTankBulletVector->m_WorkingList.m_ppObjects[i];
		l_pObject->Update(e_fElpaseTime);
		bool	l_bCollided = false;
		if (l_pObject->m_MoveSpeedTC.bTragetTimrReached)
		{
			l_pObject->m_iTableIndex = l_pTestMapInstance->GetTableIndexByDirection(l_pObject->m_iTableIndex, l_pObject->m_eMovingDirection);
			l_pObject->m_MoveSpeedTC.Start();
			int l_iNewIndex;
			if (!l_pTestMapInstance->IsMoveAble(l_pObject->m_iTableIndex, l_iNewIndex))
			{
				l_bCollided = true;
				auto l_pTestObject = l_pGridData->ppTestObjectArray[l_pObject->m_iTableIndex];
				if (l_pTestObject)
				{
					int l_iOriginalHP = l_pTestObject->GetHP();
					if (l_iOriginalHP > 0)
					{
						int l_iNewHP = l_pTestObject->GetHP() - l_pObject->m_iDamage;
						if (l_iNewHP < 0)
						{
							l_pTestObject->SetHP(0);
						}
						else
						{
							l_pTestObject->SetHP(l_iNewHP);
						}
					}
				}
			}
			//auto l_Table = l_pTestMapInstance->TableIndexToWorldXY(l_pObject->m_iTableIndex);
			//bool l_bVisible = l_pTestMapInstance->IsXYInLocalView(l_Table);
			//l_pObject->SetVisible(l_bVisible);
		}
		else
		{
			auto l_Table = l_pTestMapInstance->TableIndexToLocalXY(l_pObject->m_iTableIndex);
			l_pObject->SetLocalPosition(Vector3((float)l_Table.x * l_pGridData->iGridWidth, (float)l_Table.y * l_pGridData->iGridHeight, 0));
		}
		if (l_pObject->IsOutOfFuel() || l_bCollided)
		{
			l_pObject->Init();
			m_pTankBulletVector->WorkObjectFinish(i);
			--i;
		}
	}
}

void cTestTankController::TankUpdate(float e_fElpaseTime)
{
	auto l_pTestMapInstance = cTestMap::GetInstance();
	auto l_pGridData = l_pTestMapInstance->GetGridData();
	int l_iCount = this->Count();
	for (int i = 0; i < l_iCount; ++i)
	{
		cTestTank*l_pTestTank = this->GetObject(i);
		l_pTestTank->Update(e_fElpaseTime);
		auto l_Table = l_pTestMapInstance->TableIndexToLocalXY(l_pTestTank->m_iTableIndex);
		l_pTestTank->SetLocalPosition(Vector3((float)l_Table.x * l_pGridData->iGridWidth, (float)l_Table.y * l_pGridData->iGridHeight, 0));
		//bool l_bVisible = l_pTestMapInstance->IsXYInLocalView(l_Table);
		//l_pTestTank->SetVisible(l_bVisible);
	}
}

void cTestTankController::BulletRender()
{
	auto l_pTestMapInstance = cTestMap::GetInstance();
	auto l_pGridData = l_pTestMapInstance->GetGridData();
	for (int i = 0; i < m_pTankBulletVector->m_WorkingList.m_iNumWorking; i++)
	{
		auto l_pObject = m_pTankBulletVector->m_WorkingList.m_ppObjects[i];
		if (l_pObject)
		{
			l_pObject->Render();
		}
	}
}

void cTestTankController::TankRender()
{
	auto l_pTestMapInstance = cTestMap::GetInstance();
	int l_iCount = this->Count();
	for (int i = 0; i < l_iCount; ++i)
	{
		cTestTank*l_pTestTank = this->GetObject(i);
		if (l_pTestTank)
		{
			l_pTestTank->Render();
		}
	}
}


void cTestTankController::Init()
{
	m_pSelectedTank = this->GetObject(0);
	if (m_pTankBulletVector)
	{
		m_pTankBulletVector->PushToWaitFetchList();
	}
}

void cTestTankController::Update(float e_fElpaseTime)
{
	BulletUpdate(e_fElpaseTime);
	TankUpdate(e_fElpaseTime);
}

void cTestTankController::Render()
{
	BulletRender();
	TankRender();
}

void cTestTankController::DebugRender()
{
}

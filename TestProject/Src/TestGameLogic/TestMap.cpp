#include "stdafx.h"
#include "TestMap.h"
#include "TestObject.h"
#include "TestObjectFloor.h"
#include "TestObjectHay.h"
#include "TestObjectWall.h"
#include "LogID.h"
#include "Event.h"
#include "TankController.h"

POINT cTestMap::sGridData::GetTankOffsetPos()
{
	POINT l_Pos = { this->iViewableRow / 2,this->iViewableColumn / 2 };
	return l_Pos;
}
cTestMap::sGridData::sGridData()
{
	const int l_ciDefaultWidth = 30;
	const int l_ciDefaultHeight = 30;
	iViewableRow = (int)cGameApp::m_svGameResolution.x / l_ciDefaultWidth;;
	iViewableColumn = (int)cGameApp::m_svGameResolution.y / l_ciDefaultHeight;;
	iRow = iViewableRow * 5;
	iColumn = iViewableColumn * 5;
	iGridWidth = l_ciDefaultWidth;
	iGridHeight = l_ciDefaultHeight;
	CurrentFirstIndex = { iRow/2,iColumn /2};
	ppTestObjectArray = nullptr;
}

cTestMap::sGridData::~sGridData()
{
	Destroy();
}

void cTestMap::sGridData::Destroy()
{
	if (ppTestObjectArray)
	{
		int l_iTotal = iRow * iColumn;
		for (int i = 0; i < l_iTotal; ++i)
		{
			SAFE_DELETE(ppTestObjectArray[i]);
		}
		SAFE_DELETE(ppTestObjectArray);
	}
}

cTestMap::cTestMap()
{
	REGISTER_CLONE_FUNCTION_TO_MANGER(cTestObjectFloor);
	REGISTER_CLONE_FUNCTION_TO_MANGER(cTestObjectHay);
	REGISTER_CLONE_FUNCTION_TO_MANGER(cTestObjectWall);
	REG_EVENT(eT_E_KEY_DOWN, &cTestMap::KeyEventFuntion);
	m_NextTimeAllowToMoveTC.SetTargetTime(0.3f);
	m_fMoveSpeedUp = 1.f;
	m_pTestTankController = new cTestTankController();
}

cTestMap::~cTestMap()
{
	m_GridData.Destroy();
	SAFE_DELETE(m_pTestTankController);
}

NamedTypedObject * cTestMap::Clone()
{
	assert(0&&"cTestMap not allow to clone!");
	return nullptr;
}

int cTestMap::sGridData::GetConvertTableIndex(int e_iLocalX, int e_iLocalY, int&e_iConvertedX, int&e_iConvertedY)
{
	e_iConvertedX = UT::GetLoopIndex(this->CurrentFirstIndex.x+ e_iLocalX, iRow);
	e_iConvertedY = UT::GetLoopIndex(this->CurrentFirstIndex.y+ e_iLocalY, iColumn);
	int l_iMapIndex = (e_iConvertedY* iRow) + e_iConvertedX;
	if (l_iMapIndex < 0 || l_iMapIndex >= this->iTotal)
	{
		return -1;
	}
	return l_iMapIndex;
}

int cTestMap::sGridData::GetTankIndex()
{
	int l_iConvertedX = 0;
	int l_iConvertedY = 0;
	int l_iTankIndex = GetConvertTableIndex(iViewableRow / 2, iViewableColumn / 2, l_iConvertedX, l_iConvertedY);
	return l_iTankIndex;
}


bool cTestMap::KeyEventFuntion(void * e_pData)
{
	seT_E_KEY_DOWNData*l_peT_E_KEY_DOWNData = (seT_E_KEY_DOWNData*)e_pData;
	cTestTank*l_pTestTank = nullptr;
	if (m_pTestTankController)
		l_pTestTank = m_pTestTankController->GetSelectedTank();
	if (l_peT_E_KEY_DOWNData->bPressed)
	{
		if (!m_NextTimeAllowToMoveTC.bTragetTimrReached)
			return true;
		m_NextTimeAllowToMoveTC.Start();
		if(m_fMoveSpeedUp <= m_fMaxSpeed)
			m_fMoveSpeedUp += m_fIncreaseSpeed;
		int l_iTankIndex = this->m_GridData.GetTankIndex();
		int l_iFinalIndex = -1;
		eDirection l_Direction = eDirection::eD_MAX;
		//37,38,39,40
		//left,up,right,down
		switch (l_peT_E_KEY_DOWNData->ucKey)
		{
			case 37:
				if (IsMoveAble(l_iTankIndex - 1, l_iTankIndex))
				{
					--this->m_GridData.CurrentFirstIndex.x;
					l_Direction = eD_LEFT;
				}
				break;
			case 38:
				if (IsMoveAble(l_iTankIndex - m_GridData.iRow, l_iTankIndex))
				{
					--this->m_GridData.CurrentFirstIndex.y;
					l_Direction = eD_UP;
				}
				break;
			case 39:
				if (IsMoveAble(l_iTankIndex + 1, l_iTankIndex))
				{
					++this->m_GridData.CurrentFirstIndex.x;
					l_Direction = eD_RIGHT;
				}
				break;
			case 40:
				if (IsMoveAble(l_iTankIndex + m_GridData.iRow, l_iTankIndex))
				{
					++this->m_GridData.CurrentFirstIndex.y;
					l_Direction = eD_DOWN;
				}
				break;
			case 9://tab switch tank
			{
				if (m_pTestTankController)
				{
					auto l_pTank = m_pTestTankController->SelectNextTank();
					CameraJumpTo(l_pTank->GetTableIndex());
				}
			}
				break;
			default:
				break;
		}
		if (l_pTestTank && l_Direction != eDirection::eD_MAX)
		{
			l_pTestTank->SetTableIndex(l_iTankIndex, l_Direction);
		}
	}
	else
	{
		m_NextTimeAllowToMoveTC.bTragetTimrReached = true;
		m_fMoveSpeedUp = 1.f;
		if(l_peT_E_KEY_DOWNData->ucKey == 17 && l_pTestTank)
		{
			l_pTestTank->Fire();
		}
	}
	return false;
}

void cTestMap::CameraJumpTo(int e_iTableIndex)
{
	POINT l_TableIndexXY = TableIndexToWorldXY(e_iTableIndex);
	auto l_Offset = this->m_GridData.GetTankOffsetPos();
	l_TableIndexXY.x -= l_Offset.x;
	l_TableIndexXY.y -= l_Offset.y;
	m_GridData.CurrentFirstIndex = l_TableIndexXY;
}

void cTestMap::GenerateMap(sIDAndProbability&e_IDAndProbability)
{
	assert(e_IDAndProbability.ObjectIDVector.size() == e_IDAndProbability.ProbabilityValue.ProbabilityVector.size()&&"sIDAndProbability data not correct");
	m_GridData.Destroy();
	int l_iTotal = m_GridData.iTotal;
	m_GridData.ppTestObjectArray = new cTestObjectBase*[l_iTotal];
	m_GridData.CurrentFirstIndex.x = m_GridData.iRow/2;
	m_GridData.CurrentFirstIndex.y = m_GridData.iColumn / 2;
	for (int i = 0; i < m_GridData.iColumn; ++i)
	{
		for (int j = 0; j < m_GridData.iRow; ++j)
		{
			int l_iIndex = e_IDAndProbability.ProbabilityValue.GetIndexByProbability();
			int l_iMapIndex = (i* m_GridData.iRow)+j;
			m_GridData.ppTestObjectArray[l_iMapIndex] = nullptr;
			auto l_pObject = this->GetObject(l_iIndex);
			if (l_iMapIndex == 1944)
			{
				int a = 0;
			}
			if (l_pObject)
			{
				auto l_pClone = dynamic_cast<cTestObjectBase*>(l_pObject->Clone());
				l_pClone->SetTableIndex(l_iMapIndex);
				m_GridData.ppTestObjectArray[l_iMapIndex] = l_pClone;
			}
			else
			{
				int a = 0;
			}
		}
	}
}

void cTestMap::Init()
{
	FMLog::LogWithFlag("cTestMap::Init() start", LOG_ID_MAP, false);
	if (this->Count() == 0)
	{
		const char*l_strFileName = "Test/TestMap.xml";
		PARSEWITHMYPARSE_FAILED_MESSAGE_BOX(this, l_strFileName);
	}
	if (m_pTestTankController)
	{
		const char*l_strFileName = "Test/TestTank.xml";
		PARSEWITHMYPARSE_FAILED_MESSAGE_BOX(m_pTestTankController,l_strFileName);
		m_pTestTankController->Init();
		int l_iCount = m_pTestTankController->Count();
		for (int i = 0; i < l_iCount; ++i)
		{
			auto l_pTank = m_pTestTankController->GetObject(i);
			int l_iTankIndex = this->m_GridData.GetTankIndex();
			l_pTank->SetTableIndex(l_iTankIndex+i, eDirection::eD_DOWN);
		}
		auto l_pTank = m_pTestTankController->SelectNextTank();
		CameraJumpTo(l_pTank->GetTableIndex());
	}

	FMLog::LogWithFlag("cTestMap::Init() end", LOG_ID_MAP, false);
}

void cTestMap::Update(float e_fElpaseTime)
{
	m_NextTimeAllowToMoveTC.Update(e_fElpaseTime*m_fMoveSpeedUp);
	Vector3 l_vMapPos = this->GetWorldPosition();
	int l_iTotal = m_GridData.iRow*m_GridData.iColumn;
	for (int i = 0; i < m_GridData.iViewableRow; ++i)
	{
		for (int j = 0; j < m_GridData.iViewableColumn; ++j)
		{
			int	l_iIndexX = 0;
			int	l_iIndexY = 0;
			int l_iMapIndex = this->m_GridData.GetConvertTableIndex(i, j, l_iIndexX, l_iIndexY);
			if (l_iMapIndex == -1)
			{
				assert(0 && "cTestMap::Update l_iMapIndex wrong!");
				continue;
			}
			auto l_pObject = m_GridData.ppTestObjectArray[l_iMapIndex];
			if (l_pObject)
			{
				Vector3 l_vPos = l_vMapPos;
				l_vPos.x += (float)m_GridData.iGridWidth*i;
				l_vPos.y += (float)m_GridData.iGridHeight*j;
				l_pObject->SetLocalPosition(l_vPos);
				l_pObject->Update(e_fElpaseTime);
			}
		}
	}
	if (m_pTestTankController)
	{
		m_pTestTankController->Update(e_fElpaseTime);
	}
}

void cTestMap::Render()
{
	glEnable2D(m_GridData.vResoultion.x, m_GridData.vResoultion.y);
	for (int i = 0; i < m_GridData.iViewableRow; ++i)
	{
		for (int j = 0; j < m_GridData.iViewableColumn; ++j)
		{
			int	l_iIndexX = 0;
			int	l_iIndexY = 0;
			int l_iMapIndex = this->m_GridData.GetConvertTableIndex(i, j, l_iIndexX, l_iIndexY);
			if (l_iMapIndex == -1)
			{
				continue;
			}
			auto l_pObject = m_GridData.ppTestObjectArray[l_iMapIndex];
			if (l_pObject)
			{
				l_pObject->Render();
			}
		}
	}
	if (m_pTestTankController)
	{
		m_pTestTankController->Render();
	}
	glEnable2D(cGameApp::m_svGameResolution.x, cGameApp::m_svGameResolution.y);
}

void cTestMap::DebugRender()
{
	glEnable2D(m_GridData.vResoultion.x, m_GridData.vResoultion.y);
	cGameApp::m_spGlyphFontRender->SetScale(0.7f);
	for (int i = 0; i < m_GridData.iViewableRow; ++i)
	{
		for (int j = 0; j < m_GridData.iViewableColumn; ++j)
		{
			int	l_iIndexX = 0;
			int	l_iIndexY = 0;
			int l_iMapIndex = this->m_GridData.GetConvertTableIndex(i, j, l_iIndexX, l_iIndexY);
			if (l_iMapIndex == -1)
			{
				continue;
			}
			auto l_pObject = m_GridData.ppTestObjectArray[l_iMapIndex];
			if (l_pObject)
			{
				auto l_vPos = l_pObject->GetWorldPosition();
				//cGameApp::RenderFont(l_vPos.x, l_vPos.y,UT::ComposeMsgByFormat(L"ID:%d,Index:%d,%d,HP:%d", l_pObject->GetID(), l_iIndexX, l_iIndexY, l_pObject->GetHP()));
				cGameApp::RenderFont(l_vPos.x+5, l_vPos.y+5, UT::ComposeMsgByFormat(L"%d", l_iMapIndex));
			}
		}
	}
	cGameApp::m_spGlyphFontRender->SetScale(1.f);
	Vector2 l_vPos[2];
	Vector3 l_vMapPos = this->GetWorldPosition();
	for (int i = 0; i < m_GridData.iViewableColumn; ++i)
	{
		l_vPos[0] = Vector2(0 + l_vMapPos.x, m_GridData.iGridHeight*i + l_vMapPos.y);
		l_vPos[1] = Vector2(m_GridData.vResoultion.x + l_vMapPos.x, m_GridData.iGridHeight*(i)+l_vMapPos.y);
		GLRender::RenderLine((float*)l_vPos, 2, Vector4::One, 2);
	}
	for (int i = 0; i < m_GridData.iViewableRow; ++i)
	{
		l_vPos[0] = Vector2(m_GridData.iGridWidth*i + l_vMapPos.x, 0 + l_vMapPos.y);
		l_vPos[1] = Vector2(m_GridData.iGridWidth*i + l_vMapPos.x, m_GridData.vResoultion.y + l_vMapPos.y);
		GLRender::RenderLine((float*)l_vPos, 2, Vector4::One, 2);
	}		
	Vector2 l_vTankPos = Vector2(m_GridData.iViewableRow / 2* m_GridData.iGridWidth, m_GridData.iViewableColumn / 2* m_GridData.iGridHeight);
	GLRender::RenderRectangle(l_vTankPos,(float)m_GridData.iGridWidth, (float)m_GridData.iGridHeight,Vector4::Red);
	glEnable2D(cGameApp::m_svGameResolution.x, cGameApp::m_svGameResolution.y);
}

bool cTestMap::IsMoveAble(int e_iTableIndex, int& e_iNewTableIndex)
{
	int l_iIndex = UT::GetLoopIndex(e_iTableIndex, m_GridData.iTotal);
	e_iNewTableIndex = l_iIndex;
	if (m_GridData.ppTestObjectArray[l_iIndex]->GetHP() == 0)
		return true;
	return false;
}


int	cTestMap::GetTableIndexByDirection(int e_iTableIndex, eDirection e_eDirection)
{
	int l_iIndex = -1;
	switch (e_eDirection)
	{
	case eD_LEFT:
		l_iIndex = UT::GetLoopIndex(e_iTableIndex - 1, m_GridData.iTotal);
		break;
	case eD_UP:
		l_iIndex = UT::GetLoopIndex(e_iTableIndex - m_GridData.iRow, m_GridData.iTotal);
		break;
	case eD_RIGHT:
		l_iIndex = UT::GetLoopIndex(e_iTableIndex + 1, m_GridData.iTotal);
		break;
	case eD_DOWN:
		l_iIndex = UT::GetLoopIndex(e_iTableIndex + m_GridData.iRow, m_GridData.iTotal);
		break;
	default:
		return -1;
		break;
	}
	return l_iIndex;
}

POINT cTestMap::TableIndexToLocalXY(int e_iTableIndex)
{
	int l_iY = e_iTableIndex/this->m_GridData.iRow;
	int l_iX = e_iTableIndex - (l_iY * this->m_GridData.iRow);
	l_iX = l_iX-this->m_GridData.CurrentFirstIndex.x;
	l_iY = l_iY-this->m_GridData.CurrentFirstIndex.y;
	POINT l_Result = { l_iX ,l_iY };
	return l_Result;
}

POINT cTestMap::TableIndexToWorldXY(int e_iTableIndex)
{
	int l_iY = e_iTableIndex / this->m_GridData.iRow;
	int l_iX = e_iTableIndex - l_iY * this->m_GridData.iRow;
	POINT l_Result = { l_iX ,l_iY };
	return l_Result;
}

bool	cTestMap::IsXYInLocalView(POINT e_Pos)
{
	if (e_Pos.x >= m_GridData.CurrentFirstIndex.x &&
		e_Pos.x <= m_GridData.CurrentFirstIndex.x &&
		e_Pos.y >= m_GridData.CurrentFirstIndex.y &&
		e_Pos.y <= m_GridData.CurrentFirstIndex.y)
	{
		return true;
	}
	return false;
}
#include "stdafx.h"
#include "TestMap.h"
#include "TestObject.h"
#include "TestObjectFloor.h"
#include "TestObjectHay.h"
#include "TestObjectWall.h"
#include "LogID.h"
#include "Event.h"
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

void cTestMap::sGridData::Setup(TiXmlElement * e_pElement)
{
	Vector2 l_vResolution = cGameApp::m_svGameResolution;
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("Resolution")
		{
			l_vResolution = GetVector2(l_strValue);
			vResoultion = l_vResolution;
		}
		else
		COMPARE_NAME("GridRow")
		{
			iRow = GetInt(l_strValue);
		}
		else
		COMPARE_NAME("GridColumn")
		{
			iColumn = GetInt(l_strValue);
		}
		else
		COMPARE_NAME("ViewableRow")
		{
			iViewableRow = GetInt(l_strValue);
		}
		else
		COMPARE_NAME("ViewableColumn")
		{
			iViewableColumn = GetInt(l_strValue);
		}
	PARSE_NAME_VALUE_END
	iGridWidth = (int)l_vResolution.x/ iViewableRow;
	iGridHeight = (int)l_vResolution.y / iViewableColumn;
	iTotal = iRow*iColumn;
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
	REG_EVENT(eT_E_KEY_DOWN, &cTestMap::KeyEventFuntion);
	m_NextTimeAllowToMoveTC.SetTargetTime(0.3f);
	m_fMoveSpeedUp = 1.f;
}

cTestMap::~cTestMap()
{
	m_GridData.Destroy();
}

NamedTypedObject * cTestMap::Clone()
{
	assert(0&&"cTestMap not allow to clone!");
	return nullptr;
}

int cTestMap::sGridData::GetConvertIndex(int e_iX, int e_iY, int&e_iConvertedX, int&e_iConvertedY)
{
	e_iConvertedX = UT::GetLoopIndex(this->CurrentFirstIndex.x+ e_iX, iRow);
	e_iConvertedY = UT::GetLoopIndex(this->CurrentFirstIndex.y+ e_iY, iColumn);
	int l_iMapIndex = (e_iConvertedY* iRow) + e_iConvertedX;
	if (l_iMapIndex < 0 || l_iMapIndex >= this->iTotal)
	{
		return -1;
	}
	return l_iMapIndex;
}

int cTestMap::sGridData::GetTankIndex()
{
	int l_iTankIndex = CurrentFirstIndex.x + (CurrentFirstIndex.y*iRow) + iViewableRow / 2 + (iViewableColumn / 2 * iRow);
	return l_iTankIndex;
}

template<>
void	cTestMap::AddCloneRegisterFunction()
{
	REGISTER_CLONE_FUNCTION_TO_MANGER(cTestObjectFloor);
	REGISTER_CLONE_FUNCTION_TO_MANGER(cTestObjectHay);
	REGISTER_CLONE_FUNCTION_TO_MANGER(cTestObjectWall);
}
bool cTestMap::KeyEventFuntion(void * e_pData)
{
	seT_E_KEY_DOWNData*l_peT_E_KEY_DOWNData = (seT_E_KEY_DOWNData*)e_pData;
	if (l_peT_E_KEY_DOWNData->bPressed)
	{
		if (!m_NextTimeAllowToMoveTC.bTragetTimrReached)
			return true;
		m_NextTimeAllowToMoveTC.Start();
		if(m_fMoveSpeedUp <= 50.f)
			m_fMoveSpeedUp += 0.36f;
		int l_iTankIndex = this->m_GridData.GetTankIndex();
		int l_iFinalIndex = -1;
		//37,38,39,40
		//left,up,right,down
		switch (l_peT_E_KEY_DOWNData->ucKey)
		{
			case 37:
				l_iFinalIndex = UT::GetLoopIndex(l_iTankIndex-1, m_GridData.iTotal);
				if(m_GridData.ppTestObjectArray[l_iFinalIndex]->GetHP() == 0)
					--this->m_GridData.CurrentFirstIndex.x;
				break;
			case 38:
				l_iFinalIndex = UT::GetLoopIndex(l_iTankIndex - m_GridData.iRow, m_GridData.iTotal);
				if (m_GridData.ppTestObjectArray[l_iFinalIndex]->GetHP() == 0)
					--this->m_GridData.CurrentFirstIndex.y;
				break;
			case 39:
				l_iFinalIndex = UT::GetLoopIndex(l_iTankIndex + 1, m_GridData.iTotal);
				if (m_GridData.ppTestObjectArray[l_iFinalIndex]->GetHP() == 0)
					++this->m_GridData.CurrentFirstIndex.x;
				break;
			case 40:
				l_iFinalIndex = UT::GetLoopIndex(l_iTankIndex + m_GridData.iRow, m_GridData.iTotal);
				if (m_GridData.ppTestObjectArray[l_iFinalIndex]->GetHP() == 0)
					++this->m_GridData.CurrentFirstIndex.y;
				break;
			default:
				break;
		}
	}
	else
	{
		m_NextTimeAllowToMoveTC.bTragetTimrReached = true;
		m_fMoveSpeedUp = 1.f;
	}
	return false;
}
//<Map>
//	<GridData Resolution = "720,1080" RenderPos = "0,200" GridRow = "20" GridColumn = "20" MinimumRowAndColumn = "6"/>
//	<TestObjectData>
		//<Hay ID="0" Name=""Hay HP="100" PI="TestGame/AA.pi" PIUnit="3" >
		//</Hay>
		//<Floor ID="1" HP="0">
		//</Floor>
		//<Wall ID="0" HP="100" PI="TestGame/AA.pi" PIUnit="3" >
		//</Wall>
//	</TestObjectData>
//	<GeneratingRule>
	//	<ObjectData ID="0" Probability="0.03" />
	//	<ObjectData ID="1" Probability="0.03" />
	//	<ObjectData ID="2" Probability="0.03" />
//	</GeneratingRule>
//<Map>
bool cTestMap::MyParse(TiXmlElement * e_pRoot)
{
	auto l_strRenderPos = e_pRoot->Attribute(L"RenderPos");
	if(l_strRenderPos)
	{
			auto l_vPos = GetVector2(l_strRenderPos);
			this->SetLocalPosition(Vector3(l_vPos.x, l_vPos.y,0.f));
	}
	auto l_strMoveTC = e_pRoot->Attribute(L"MoveTC");
	if (l_strMoveTC)
	{
		m_NextTimeAllowToMoveTC.SetTargetTime(GetFloat(l_strMoveTC));
	}
	
	sIDAndProbability l_IDAndProbability;
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pRoot)
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pRoot, L"GeneratingRule")
		{
			ProcessGeneratingRuleData(e_pRoot, l_IDAndProbability);
		}
		else
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pRoot, L"GridData")
		{
			ProcessGridData(e_pRoot);
		}
		else
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pRoot, L"TestObjectData")
		{
			ProcessTestObjectData(e_pRoot);
		}
		
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pRoot)
	GenerateMap(l_IDAndProbability);
	return true;
}
void	cTestMap::ProcessTestObjectData(TiXmlElement*e_pElement)
{
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pElement)
		cTestObjectBase*l_pTestObjectBase = GetObjectByParseXmlElement(e_pElement);
		if (l_pTestObjectBase)
		{
			this->AddObjectNeglectExist(l_pTestObjectBase);
		}
		else
		{
			UT::ErrorMsg(e_pElement->Value(), L"not support type");
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pElement)
}
//	<GridData Resolution="720,1080" RenderPos="0,200" ViewableRow="24" ViewableColumn="39" GridRow = "240" GridColumn = "390"/>
void cTestMap::ProcessGridData(TiXmlElement * e_pElement)
{
	m_GridData.Setup(e_pElement);
}
//	<GeneratingRule>
	//	<ObjectData ID="0" Probability="3" />
	//	<ObjectData ID="1" Probability="3" />
	//	<ObjectData ID="2" Probability="3" />
//	</GeneratingRule>
void cTestMap::ProcessGeneratingRuleData(TiXmlElement * e_pElement, sIDAndProbability & e_IDAndProbability)
{
	std::vector<int> l_ProbabilityVector;
	std::vector<int> l_ObjectIDVector;
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pElement)
		auto l_strID = e_pElement->Attribute(L"ID");
		auto l_strProbability = e_pElement->Attribute(L"Probability");
		if (l_strID && l_strProbability)
		{
			l_ProbabilityVector.push_back(GetInt(l_strProbability));
			l_ObjectIDVector.push_back(GetInt(l_strID));
		}
		else
		{
			FMLog::LogWithFlag("cTestMap::ProcessGeneratingRuleData data not match!", LOG_ID_MAP, false);
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pElement)
	e_IDAndProbability.ObjectIDVector = l_ObjectIDVector;
	e_IDAndProbability.ProbabilityValue.SetupProbabilityData(l_ProbabilityVector);
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
			int l_iMapIndex = this->m_GridData.GetConvertIndex(i, j, l_iIndexX, l_iIndexY);
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
}

void cTestMap::Render()
{
	for (int i = 0; i < m_GridData.iViewableRow; ++i)
	{
		for (int j = 0; j < m_GridData.iViewableColumn; ++j)
		{
			int	l_iIndexX = 0;
			int	l_iIndexY = 0;
			int l_iMapIndex = this->m_GridData.GetConvertIndex(i, j, l_iIndexX, l_iIndexY);
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
}

void cTestMap::DebugRender()
{
	cGameApp::m_spGlyphFontRender->SetScale(0.5f);
	for (int i = 0; i < m_GridData.iViewableRow; ++i)
	{
		for (int j = 0; j < m_GridData.iViewableColumn; ++j)
		{
			int	l_iIndexX = 0;
			int	l_iIndexY = 0;
			int l_iMapIndex = this->m_GridData.GetConvertIndex(i, j, l_iIndexX, l_iIndexY);
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
	GLRender::RenderRectangle(l_vTankPos,m_GridData.iGridWidth, m_GridData.iGridHeight,Vector4::Red);
}

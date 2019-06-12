#include "stdafx.h"
#include "TestMap.h"
#include "TestObject.h"
#include "TestObjectFloor.h"
#include "TestObjectHay.h"
#include "TestObjectWall.h"
#include "LogID.h"
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
	return l_iMapIndex;
}

template<>
void	cTestMap::AddCloneRegisterFunction()
{
	REGISTER_CLONE_FUNCTION_TO_MANGER(cTestObjectFloor);
	REGISTER_CLONE_FUNCTION_TO_MANGER(cTestObjectHay);
	REGISTER_CLONE_FUNCTION_TO_MANGER(cTestObjectWall);
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
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pRoot, L"TestObject")
		{
			ProcessTestObjectData(e_pRoot);
		}
		
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pRoot)
	GenerateMap(l_IDAndProbability);
	return false;
}
void	cTestMap::ProcessTestObjectData(TiXmlElement*e_pElement)
{
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pElement)
		cTestObjectBase*l_pTestObjectBase = GetObjectByParseXmlElement(e_pElement);
		if (l_pTestObjectBase)
		{
			this->AddObject(l_pTestObjectBase);
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
	Vector2 l_vResolution = cGameApp::m_svGameResolution;
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("Resolution")
		{
			l_vResolution = GetVector2(l_strValue);
		}
		else
		COMPARE_NAME("RenderPos")
		{
			auto l_vPos = GetVector2(l_strValue);
			this->SetLocalPosition(Vector3(l_vPos.x, l_vPos.y,0.f));
		}
		else
		COMPARE_NAME("GridRow")
		{
			m_GridData.iRow = GetInt(l_strValue);
		}
		else
		COMPARE_NAME("GridColumn")
		{
			m_GridData.iColumn = GetInt(l_strValue);
		}
		else
		COMPARE_NAME("ViewableRow")
		{
			m_GridData.iViewableRow = GetInt(l_strValue);
		}
		else
		COMPARE_NAME("ViewableColumn")
		{
			m_GridData.iViewableColumn = GetInt(l_strValue);
		}
	PARSE_NAME_VALUE_END
	m_GridData.iGridWidth = (int)l_vResolution.x/ m_GridData.iViewableRow;
	m_GridData.iGridHeight = (int)l_vResolution.y / m_GridData.iViewableColumn;
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
	int l_iTotal = m_GridData.iRow * m_GridData.iColumn;
	m_GridData.ppTestObjectArray = new cTestObjectBase*[l_iTotal];
	m_GridData.CurrentFirstIndex.x = m_GridData.iRow/2;
	m_GridData.CurrentFirstIndex.y = m_GridData.iColumn / 2;
	for (int i = 0; i < m_GridData.iRow; ++i)
	{
		for (int j = 0; j < m_GridData.iColumn; ++j)
		{
			int l_iIndex = e_IDAndProbability.ProbabilityValue.GetIndexByProbability();
			int l_iMapIndex = (j* m_GridData.iRow)+i;
			auto l_pObject = this->GetObject(l_iIndex);
			if (l_pObject)
			{
				auto l_pClone = dynamic_cast<cTestObjectBase*>(l_pObject->Clone());
				//this is a loop map so don't do this.
				//l_pClone->SetLocalPosition(Vector3((float)m_GridData.iGridWidth*i, (float)m_GridData.iGridHeight*j,0.f));
				//l_pClone->SetParent(this);
				m_GridData.ppTestObjectArray[l_iMapIndex] = l_pClone;
			}
		}
	}
}

void cTestMap::Init()
{
	if (this->Count() == 0)
	{
		const char*l_strFileName = "Test/TestMap.xml";
		PARSEWITHMYPARSE_FAILED_MESSAGE_BOX(this, l_strFileName);
	}
}

void cTestMap::Update(float e_fElpaseTime)
{
	Vector3 l_vMapPos = this->GetWorldPosition();
	POINT l_CurrentFirstIndex = m_GridData.CurrentFirstIndex;
	for (int i = 0; i < m_GridData.iViewableRow; ++i)
	{
		for (int j = 0; j < m_GridData.iViewableColumn; ++j)
		{
			int	l_iIndexX = 0;
			int	l_iIndexY = 0;
			int l_iMapIndex = this->m_GridData.GetConvertIndex(i, j, l_iIndexX, l_iIndexY);
			auto l_pObject = this->GetObject(l_iMapIndex);
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
	POINT l_CurrentFirstIndex = m_GridData.CurrentFirstIndex;
	for (int i = 0; i < m_GridData.iViewableRow; ++i)
	{
		for (int j = 0; j < m_GridData.iViewableColumn; ++j)
		{
			int	l_iIndexX = 0;
			int	l_iIndexY = 0;
			int l_iMapIndex = this->m_GridData.GetConvertIndex(i, j, l_iIndexX, l_iIndexY);
			auto l_pObject = this->GetObject(l_iMapIndex);
			if (l_pObject)
			{
				l_pObject->Render();
			}
		}
	}
}

void cTestMap::DebugRender()
{
	for (int i = 0; i < m_GridData.iViewableRow; ++i)
	{
		for (int j = 0; j < m_GridData.iViewableColumn; ++j)
		{
			int	l_iIndexX = 0;
			int	l_iIndexY = 0;
			int l_iMapIndex = this->m_GridData.GetConvertIndex(i, j, l_iIndexX, l_iIndexY);
			auto l_pObject = this->GetObject(l_iMapIndex);
			if (l_pObject)
			{
				auto l_vPos = l_pObject->GetWorldPosition();
				cGameApp::RenderFont(l_vPos.x, l_vPos.y,UT::ComposeMsgByFormat(L"ID:%d,Index:%d,%d,HP:%d", l_pObject->GetID(), l_iIndexX, l_iIndexY, l_pObject->GetHP()));
			}
		}
	}
}

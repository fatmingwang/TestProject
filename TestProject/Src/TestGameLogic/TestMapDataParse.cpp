#include "stdafx.h"
#include "TestMap.h"
#include "TestObject.h"
#include "TestObjectFloor.h"
#include "TestObjectHay.h"
#include "TestObjectWall.h"
#include "LogID.h"
#include "Event.h"
#include "../Core/GameplayUT/CommonRegister/CommonRegisterManager.h"
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
	if (iViewableColumn < 3)
	{
		iViewableColumn = 3;
		FMLog::LogWithFlag("iViewableColumn smaller than 3!!!?magic increase happen!", LOG_ID_MAP, false);
	}
	if (iViewableRow < 3)
	{
		iViewableRow = 3;
		FMLog::LogWithFlag("iViewableRow smaller than 3!!!?magic increase happen!", LOG_ID_MAP, false);
	}
	if (iRow <= iViewableColumn)
	{
		iRow = iViewableColumn * 2;
		FMLog::LogWithFlag("iViewableColumn bigger than iRow!!!?magic increase happen!", LOG_ID_MAP, false);
	}
	if (iColumn <= iViewableRow)
	{
		iColumn = iViewableRow * 2;
		FMLog::LogWithFlag("iViewableRow bigger than iColumn!!!?magic increase happen!", LOG_ID_MAP, false);
	}
	iGridWidth = (int)l_vResolution.x / iViewableRow;
	iGridHeight = (int)l_vResolution.y / iViewableColumn;
	iTotal = iRow * iColumn;

}
//cTestMap
//cCommonRegisterManager
//template<>
//void	cTestMap::AddCloneRegisterFunction()
//{
	//REGISTER_CLONE_FUNCTION_TO_MANGER(cTestObjectFloor);
	//REGISTER_CLONE_FUNCTION_TO_MANGER(cTestObjectHay);
	//REGISTER_CLONE_FUNCTION_TO_MANGER(cTestObjectWall);
//}
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
	if (l_strRenderPos)
	{
		auto l_vPos = GetVector2(l_strRenderPos);
		this->SetLocalPosition(Vector3(l_vPos.x, l_vPos.y, 0.f));
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

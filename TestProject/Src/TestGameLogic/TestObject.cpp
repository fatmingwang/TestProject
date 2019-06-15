#include "stdafx.h"
#include "TestObject.h"
#include "TestMap.h"
cTestObjectBase::cTestObjectBase()
{
	m_pRenderObject = nullptr;
	m_iTableIndex = 0;
	this->m_iHP = 0;
	this->m_iID = -1;
	m_iOriginalHP = 0;
}

cTestObjectBase::cTestObjectBase(cTestObjectBase * e_pTestObjectBase)
{
	m_pRenderObject = e_pTestObjectBase->m_pRenderObject;
	this->m_iHP = e_pTestObjectBase->m_iHP;
	this->m_iID = e_pTestObjectBase->m_iID;
	m_iOriginalHP = e_pTestObjectBase->m_iOriginalHP;
	m_iTableIndex = 0;
	
}
//<Wall ID = "0" Name = "Wall" HP = "100" PI = "TestGame/AA.pi" PIUnit = "3" >
cTestObjectBase::cTestObjectBase(TiXmlElement * e_pTiXmlElement)
{
	m_iTableIndex = 0;
	m_pRenderObject = nullptr;
	cPuzzleImage*l_pPI = nullptr;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("ID")
		{
			this->m_iID = GetInt(l_strValue);
		}
		else
		COMPARE_NAME("Name")
		{
			this->SetName(l_strValue);
		}
		else
		COMPARE_NAME("HP")
		{
			m_iOriginalHP = this->m_iHP = GetInt(l_strValue);
		}
		else
		COMPARE_NAME("PI")
		{
			l_pPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
			if (!l_pPI)
			{
				int a = 0;
			}
		}
		else
		COMPARE_NAME("PIUnit")
		{
			if(l_pPI)
			{
				this->m_pRenderObject = l_pPI->GetObject(l_strValue);
			}
		}
		else
		COMPARE_NAME("Size")
		{
			if(m_pRenderObject)
			{
				auto l_Size = GetPoint(l_strValue);
				this->m_pRenderObject->SetWidth(l_Size.x);
				this->m_pRenderObject->SetHeight(l_Size.y);
			}
		}
	PARSE_NAME_VALUE_END
	if(m_pRenderObject)
	{
		auto l_pGridData = cTestMap::GetInstance()->GetGridData();
		this->m_pRenderObject->SetWidth(l_pGridData->iGridWidth);
		this->m_pRenderObject->SetHeight(l_pGridData->iGridHeight);
		int a = 0;
	}
}

cTestObjectBase::~cTestObjectBase()
{
}

void cTestObjectBase::Init()
{
}

void cTestObjectBase::Update(float e_fElpaseTime)
{
	this->InternalUpdate(e_fElpaseTime);
}

void cTestObjectBase::Render()
{
	if (m_pRenderObject && this->m_iHP != 0)
	{
		this->InternalRender();
		auto l_vPos = this->GetWorldPosition();
		m_pRenderObject->SetPos(this->GetWorldPosition());
		m_pRenderObject->Render();
		//cGameApp::RenderFont(l_vPos.x, l_vPos.y, ValueToStringW(this->m_iTableIndex));
	}
}
#include "stdafx.h"
#include "TestObjectHay.h"

TYPDE_DEFINE_MARCO(cTestObjectHay);

void cTestObjectHay::InternalInit()
{
}

void cTestObjectHay::InternalUpdate(float e_fElpaseTime)
{
}

void cTestObjectHay::InternalRender()
{
	if (this->m_pRenderObject)
	{
		float l_fPercentage = (float)m_iHP/ m_iOriginalHP;
		Vector4 l_vColor(1, l_fPercentage, l_fPercentage, l_fPercentage+0.5f);
		m_pRenderObject->SetColor(l_vColor);
	}
}

cTestObjectHay::cTestObjectHay()
{

}

cTestObjectHay::cTestObjectHay(cTestObjectHay * e_pTestObjectBase):cTestObjectBase(e_pTestObjectBase)
{
}

cTestObjectHay::cTestObjectHay(TiXmlElement * e_pTiXmlElement): cTestObjectBase(e_pTiXmlElement)
{
}

cTestObjectHay::~cTestObjectHay()
{
}

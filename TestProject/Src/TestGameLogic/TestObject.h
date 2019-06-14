#pragma once

class cTestObjectBase:public cRenderObject
{
	GET_SET_DEC(int,m_iID,GetID,SetID);
	GET_SET_DEC(int,m_iHP,GetHP,SetHP);
	GET_SET_DEC(int,m_iTableIndex, GetTableIndex, SetTableIndex);
	cBaseImage*		m_pRenderObject;
	//
	void			ProcessRenderObjectData(TiXmlElement*e_pElement);
	//
	virtual	void	InternalInit() = 0;
	virtual	void	InternalUpdate(float e_fElpaseTime) = 0;
	virtual	void	InternalRender() = 0;
public:
	cTestObjectBase();
	cTestObjectBase(cTestObjectBase*e_pTestObjectBase);
	cTestObjectBase(TiXmlElement*e_pTiXmlElement);
	virtual ~cTestObjectBase();
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render()override;
};
#pragma once

#include "TestObject.h"

class cTestMap:public cCommonRegisterManager<cTestObjectBase>, public cNamedTypedObjectVector<cTestObjectBase>,public cRenderObject,public cNodeISAX, cSingltonTemplate<cTestMap>,public cMessageSender
{
	struct sIDAndProbability
	{
		sProbabilityVector<int>	ProbabilityValue;
		std::vector<int>		ObjectIDVector;
	};
	//
	float						m_fMoveSpeedUp;
	const float					m_fMaxSpeed = 50.f;
	const float					m_fIncreaseSpeed = 0.36f;
	UT::sTimeCounter			m_NextTimeAllowToMoveTC;
	bool						KeyEventFuntion(void *e_pData);
	//
	class cTestTankController*	m_pTestTankController;
	void						CameraJumpTo(int e_iTableIndex);
public:
	struct sGridData
	{
		int		iViewableRow;
		int		iViewableColumn;
		int		iTotal;//iRow*iColumn
		int		iRow;
		int		iColumn;
		int		iGridWidth;
		int		iGridHeight;
		Vector2	vResoultion;
		POINT	CurrentFirstIndex;
		int		GetConvertTableIndex(int e_iLocalX, int e_iLocalY,int&e_iConvertedX, int&e_iConvertedY);
		int		GetTankIndex();//the index related to CurrentFirstIndex
		cTestObjectBase**ppTestObjectArray;
		POINT	GetTankOffsetPos();
		sGridData();
		~sGridData();
		void	Setup(TiXmlElement*e_pElement);
		void	Destroy();
	};
protected:
	sGridData		m_GridData;
	//
	virtual	bool	MyParse(TiXmlElement*e_pRoot)override;
	void			ProcessTestObjectData(TiXmlElement*e_pElement);
	void			ProcessGridData(TiXmlElement*e_pElement);
	void			ProcessGeneratingRuleData(TiXmlElement*e_pElement, sIDAndProbability&e_IDAndProbability);
	void			GenerateMap(sIDAndProbability&e_IDAndProbability);
	//
public:
	SINGLETON_BASIC_FUNCTION(cTestMap);
	cTestMap();
	virtual ~cTestMap();
	virtual	NamedTypedObject*	Clone()override;
	virtual	void				Init()override;
	virtual	void				Update(float e_fElpaseTime)override;
	virtual	void				Render()override;
	virtual	void				DebugRender()override;
	const sGridData*			GetGridData() {return &m_GridData;}
	bool						IsMoveAble(int e_iTableIndex,int& e_iNewTableIndex);
	int							GetTableIndexByDirection(int e_iTableIndex,eDirection e_eDirection);
	POINT						TableIndexToLocalXY(int e_iTableIndex);
	POINT						TableIndexToWorldXY(int e_iTableIndex);
	bool						IsXYInLocalView(POINT e_Pos);
};
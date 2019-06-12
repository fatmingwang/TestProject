#pragma once

class cTestObjectBase;

class cTestMap:public cCommonRegisterManager<cTestObjectBase>, public cNamedTypedObjectVector<cTestObjectBase>,public cRenderObject,public cNodeISAX
{
	struct sIDAndProbability
	{
		sProbabilityVector<int>	ProbabilityValue;
		std::vector<int>		ObjectIDVector;
	};

	struct sGridData
	{
		int		iViewableRow;
		int		iViewableColumn;
		int		iRow;
		int		iColumn;
		int		iGridWidth;
		int		iGridHeight;
		POINT	CurrentFirstIndex;
		int		GetConvertIndex(int e_iX, int e_iY,int&e_iConvertedX, int&e_iConvertedY);
		cTestObjectBase**ppTestObjectArray;
		sGridData();
		~sGridData();
		void	Destroy();
	};
	sGridData		m_GridData;
	//
	virtual	bool	MyParse(TiXmlElement*e_pRoot)override;
	void			ProcessTestObjectData(TiXmlElement*e_pElement);
	void			ProcessGridData(TiXmlElement*e_pElement);
	void			ProcessGeneratingRuleData(TiXmlElement*e_pElement, sIDAndProbability&e_IDAndProbability);
	void			GenerateMap(sIDAndProbability&e_IDAndProbability);
	//
public:
	cTestMap();
	virtual ~cTestMap();
	virtual	NamedTypedObject*	Clone()override;
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render()override;
	virtual	void	DebugRender()override;
};
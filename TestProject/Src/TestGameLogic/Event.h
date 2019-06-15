#pragma once

enum eTest_Event
{
	eT_E_KEY_DOWN,
	eT_E_FIRE,
};


struct seT_E_KEY_DOWNData
{
	bool			bPressed;
	unsigned char	ucKey;
};

struct seT_E_FIRE
{
	int			iTableIndex;
	int			iDamage;
	Vector4		vColor;
	eDirection	Direction;
};
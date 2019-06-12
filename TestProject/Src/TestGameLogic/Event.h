#pragma once

enum eTest_Event
{
	eT_E_KEY_DOWN,
};


struct seT_E_KEY_DOWNData
{
	bool			bPressed;
	unsigned char	ucKey;
};
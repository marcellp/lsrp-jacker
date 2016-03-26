#pragma once

#pragma pack(1)
typedef struct _TEXT_DRAW_TRANSMIT
{
	union
	{
		BYTE byteFlags;
		struct
		{
			BYTE byteBox : 1;
			BYTE byteLeft : 1;
			BYTE byteRight : 1;
			BYTE byteCenter : 1;
			BYTE byteProportional : 1;
			BYTE bytePadding : 3;
		};
	};
	float fLetterWidth;
	float fLetterHeight;
	DWORD dwLetterColor;
	float fLineWidth;
	float fLineHeight;
	DWORD dwBoxColor;
	BYTE byteShadow;
	BYTE byteOutline;
	DWORD dwBackgroundColor;
	BYTE byteStyle;
	BYTE byteSelectable;
	float fX;
	float fY;
	WORD wModelID;
	float fRotX;
	float fRotY;
	float fRotZ;
	float fZoom;
	WORD wColor1;
	WORD wColor2;
} TEXT_DRAW_TRANSMIT;

bool CALLBACK hook_get_tdstring(stRakNetHookParams* params);
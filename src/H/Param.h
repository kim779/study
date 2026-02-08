#pragma once
#pragma pack(1)
//
//	CParam from Param structure
//
class CParam
{
public:
	int		key{};			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point{};			// font point
	int		style{};			// font style
	DWORD		tRGB{};			// text color index
	DWORD		pRGB{};			// paint color index
	CString		options;		// properties
};

#pragma pack()
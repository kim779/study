#pragma once
#include "pch.h"
#pragma	pack(1)

struct	_penR {
	COLORREF clr;				// pen color
	int	width;				// pen width
	int	style;				// pen style
};

struct	_fontR {
	char* name;				// font face name
	int	point;				// font point
	bool	italic;				// italic
	int	bold;				// bold
};

#define DF_HEIGHT_DISPLAYTEXT 20
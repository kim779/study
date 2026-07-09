// split.hxx : split_window definition
//
/////////////////////////////////////////////////////////////////////////////

#ifndef	_SPLIT_HXX
#define	_SPLIT_HXX

struct _spformR	{
	BYTE	kind;
	BYTE	type;
	BYTE	iok;
	int	left;
	int	top;
	int	right;
	int	bottom;
};

#define	MAXSP		15
struct	_spformR	sp[12][MAXSP] = {
	{	// 0
		// kind		type	iok	left	top	right	bottom
		{ FM_OBJECT,	1,	0,	0,	0,	1000,	299 },
		{ FM_OBJECT,	2,	0,	0,	305,	1000,	600 },
		{ FM_SIZE,	1,	0,	0,	300,	1000,	304 },
		{ 0 }
	},
	{	// 1
		// kind		type	iok	left	top	right	bottom
		{ FM_OBJECT,	1,	0,	0,	0,	499,	600 },
		{ FM_OBJECT,	2,	0,	505,	0,	1000,	600 },
		{ FM_SIZE,	1,	1,	500,	0,	504,	600 },
		{ 0 }
	},
	{	// 2
		// kind		type	iok	left	top	right	bottom
		{ FM_OBJECT,	1,	0,	0,	0,	499,	299 },
		{ FM_OBJECT,	2,	0,	505,	0,	1000,	299 },
		{ FM_OBJECT,	3,	0,	0,	305,	1000,	600 },
		{ FM_SIZE,	1,	0,	0,	300,	1000,	304 },
		{ FM_SIZE,	2,	1,	500,	0,	504,	300 },
		{ 0 }
	},
	{	// 3
		// kind		type	iok	left	top	right	bottom
		{ FM_OBJECT,	1,	0,	0,	0,	1000,	299 },
		{ FM_OBJECT,	2,	0,	0,	305,	499,	600 },
		{ FM_OBJECT,	3,	0,	505,	305,	1000,	600 },
		{ FM_SIZE,	1,	0,	0,	300,	1000,	304 },
		{ FM_SIZE,	2,	1,	500,	304,	504,	600 },
		{ 0 }
	},
	{	// 4
		// kind		type	iok	left	top	right	bottom
		{ FM_OBJECT,	1,	0,	0,	0,	499,	299 },
		{ FM_OBJECT,	2,	0,	0,	305,	499,	600 },
		{ FM_OBJECT,	3,	0,	505,	0,	1000,	600 },
		{ FM_SIZE,	1,	1,	500,	0,	504,	600 },
		{ FM_SIZE,	2,	0,	0,	300,	500,	304 },
		{ 0 }
	},
	{	// 5
		// kind		type	iok	left	top	right	bottom
		{ FM_OBJECT,	1,	0,	0,	0,	499,	600 },
		{ FM_OBJECT,	2,	0,	505,	0,	1000,	299 },
		{ FM_OBJECT,	3,	0,	505,	305,	1000,	600 },
		{ FM_SIZE,	1,	1,	500,	0,	504,	600 },
		{ FM_SIZE,	2,	0,	504,	300,	1000,	304 },
		{ 0 }
	},
	{	// 6
		// kind		type	iok	left	top	right	bottom
		{ FM_OBJECT,	1,	0,	0,	0,	1000,	299 },
		{ FM_OBJECT,	2,	0,	0,	305,	329,	600 },
		{ FM_OBJECT,	3,	0,	335,	305,	663,	600 },
		{ FM_OBJECT,	4,	0,	669,	305,	1000,	600 },
		{ FM_SIZE,	1,	0,	0,	300,	1000,	304 },
		{ FM_SIZE,	2,	1,	330,	304,	334,	600 },
		{ FM_SIZE,	3,	1,	664,	304,	668,	600 },
		{ 0 }
	},
	{	// 7
		// kind		type	iok	left	top	right	bottom
		{ FM_OBJECT,	1,	0,	0,	0,	329,	299 },
		{ FM_OBJECT,	2,	0,	335,	0,	663,	299 },
		{ FM_OBJECT,	3,	0,	669,	0,	1000,	299 },
		{ FM_OBJECT,	4,	0,	0,	305,	1000,	600 },
		{ FM_SIZE,	1,	0,	0,	300,	1000,	304 },
		{ FM_SIZE,	2,	1,	330,	0,	334,	300 },
		{ FM_SIZE,	3,	1,	664,	0,	668,	300 },
		{ 0 }
	},
	{	// 8
		// kind		type	iok	left	top	right	bottom
		{ FM_OBJECT,	1,	0,	0,	0,	499,	299 },
		{ FM_OBJECT,	2,	0,	505,	0,	1000,	299 },
		{ FM_OBJECT,	3,	0,	0,	305,	329,	600 },
		{ FM_OBJECT,	4,	0,	335,	305,	663,	600 },
		{ FM_OBJECT,	5,	0,	669,	305,	1000,	600 },
		{ FM_SIZE,	1,	0,	0,	300,	1000,	304 },
		{ FM_SIZE,	2,	1,	500,	0,	504,	300 },
		{ FM_SIZE,	3,	1,	330,	304,	334,	600 },
		{ FM_SIZE,	4,	1,	664,	304,	668,	600 },
		{ 0 }
	},
	{	// 9
		// kind		type	iok	left	top	right	bottom
		{ FM_OBJECT,	1,	0,	0,	0,	329,	299 },
		{ FM_OBJECT,	2,	0,	335,	0,	663,	299 },
		{ FM_OBJECT,	3,	0,	669,	0,	1000,	299 },
		{ FM_OBJECT,	4,	0,	0,	305,	499,	600 },
		{ FM_OBJECT,	5,	0,	505,	305,	1000,	600 },
		{ FM_SIZE,	1,	0,	0,	300,	1000,	304 },
		{ FM_SIZE,	2,	1,	330,	0,	334,	300 },
		{ FM_SIZE,	3,	1,	664,	0,	668,	300 },
		{ FM_SIZE,	4,	1,	500,	304,	504,	600 },
		{ 0 }
	},
	{	// 10
		// kind		type	iok	left	top	right	bottom
		{ FM_OBJECT,	1,	0,	0,	0,	499,	299 },
		{ FM_OBJECT,	2,	0,	505,	0,	1000,	299 },
		{ FM_OBJECT,	3,	0,	0,	305,	499,	600 },
		{ FM_OBJECT,	4,	0,	505,	305,	1000,	600 },
		{ FM_SIZE,	1,	0,	0,	300,	1000,	304 },
		{ FM_SIZE,	2,	1,	500,	0,	504,	300 },
		{ FM_SIZE,	3,	1,	500,	304,	504,	600 }, 
		{ 0 }
	},
	{	// 11
		// kind		type	iok	left	top	right	bottom
		{ FM_OBJECT,	1,	0,	0,	0,	329,	299 },
		{ FM_OBJECT,	2,	0,	335,	0,	663,	299 },
		{ FM_OBJECT,	3,	0,	669,	0,	1000,	299 },
		{ FM_OBJECT,	4,	0,	0,	305,	329,	600 },
		{ FM_OBJECT,	5,	0,	335,	305,	663,	600 },
		{ FM_OBJECT,	6,	0,	669,	305,	1000,	600 },
		{ FM_SIZE,	1,	0,	0,	300,	1000,	304 },
		{ FM_SIZE,	2,	1,	330,	0,	334,	300 },
		{ FM_SIZE,	3,	1,	330,	304,	334,	600 },
		{ FM_SIZE,	4,	1,	664,	0,	668,	300 },
		{ FM_SIZE,	5,	1,	664,	304,	668,	600 },
		{ 0 }
	}
};

#endif
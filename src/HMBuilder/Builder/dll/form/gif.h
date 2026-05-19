#pragma	pack(1)

#define	MaxGIFB		16384
#define	MaxCODES	4096

struct dscgif {
	char	header[6];
	WORD	scrwidth;
	WORD	scrheight;
	char	pflds;
	char	bcindx;
	char	pxasrat;
};

struct	rgb {
	BYTE	r;
	BYTE	g;
	BYTE	b;
};

struct	tabcol {
	WORD	colres;
	WORD	sogct;
	struct	rgb rgbp[256];
};

struct	gifgce {
	BYTE	transpcolflag:1;
	BYTE	userinputflag:1;
	BYTE	dispmeth:3;
	BYTE	res:3;
	BYTE	delaytimehi;
	BYTE	delaytimelo;
	BYTE	transpcolindex;
};

struct	image {
	WORD	l;
	WORD	t;
	WORD	w;
	WORD	h;
	BYTE	pf;
};

#pragma	pack()

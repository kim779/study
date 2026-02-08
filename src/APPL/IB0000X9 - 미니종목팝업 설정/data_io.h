#pragma once


#define MAX_REQCOUNT	96
#define TR_STARTKEY		64

typedef struct _M_GRID_I
{
	char vrow[2];
	char nrow[1];
	char vflg[1];
	char gdir[1];
	char sdir[1];
	char scol[16];
	char ikey[1];
	char page[4];
	char save[80];
} M_GRID_I;

typedef struct _GRID_O
{
	char aflg[1];
	char sdir[1];
	char scol[16];
	char xpos[1];
	char page[4];
	char save[80];
} GRID_O;


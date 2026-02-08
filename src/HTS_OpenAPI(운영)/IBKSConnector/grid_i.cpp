#include "stdafx.h"
#include "grid_i.h"

#define setInt(x, y)	_setInt((x), sizeof(x), (y))

grid_i::grid_i()
{
	setVROW(25);
	setNROW(2000);
	setVFLG(1);
	setGDIR(1);
	setSDIR(0);
	setSCOL("");
	setIKEY(0);
	setPAGE(1);
	setSAVE("");
}

grid_i::grid_i(LPCSTR src)
{
	memcpy(this, src, sizeof(grid_i));
}

void grid_i::setVROW(int val)
{
	setInt(vrow, val);
}
void grid_i::setNROW(int val)
{
	setInt(nrow, val);
}
void grid_i::setVFLG(int val)
{
	setInt(vflg, val);
}
void grid_i::setGDIR(int val)
{
	setInt(gdir, val);
}
void grid_i::setSDIR(int val)
{
	setInt(sdir, val);
}
void grid_i::setSCOL(LPCSTR val)
{
	memset(scol, ' ', sizeof(scol));
	memcpy(scol, val, min(sizeof(scol), strlen(val)));
}
void grid_i::setIKEY(int val)
{
	ikey[0] = (char)val;
}
void grid_i::setPAGE(int val)
{
	setInt(page, val);
}
void grid_i::setSAVE(LPCSTR val)
{
	memset(save, ' ', sizeof(save));
	memcpy(save, val, min(sizeof(save), strlen(val)));
}

void grid_i::_setInt(char *dst, int dlen, int val)
{
	char buff[128];
	int clen = sprintf(buff, "%0*d", dlen, val);
	memcpy(dst, buff, min(clen, dlen));
}

void grid_i::setGRIDO(grid_o &o)
{
	sdir[0] = o.sdir[0];
	memcpy(scol, o.scol, sizeof(scol));
	memcpy(page, o.page, sizeof(page));
	memcpy(save, o.save, sizeof(save));
}

grid_o::grid_o()
{
	memset(this, ' ', sizeof(grid_o));
}

grid_o::grid_o(LPCSTR src)
{
	memcpy(this, src, sizeof(grid_o));
}

BOOL grid_o::IsNext()
{
	return (xpos[0]=='B' || xpos[0]=='C' || xpos[0]=='F' || xpos[0]=='G');
}

CString grid_o::GetNKey()
{
	return CString((LPCSTR)this, sizeof(grid_o));
}
#pragma once

class grid_o;

class grid_i
{
public:
	grid_i();
	grid_i(LPCSTR src);
	void setVROW(int val);
	void setNROW(int val);
	void setVFLG(int val);
	void setGDIR(int val);
	void setSDIR(int val);
	void setSCOL(LPCSTR val);
	void setIKEY(int val);
	void setPAGE(int val);
	void setSAVE(LPCSTR val);
	void setGRIDO(grid_o &o);
	size_t size() { return sizeof(grid_i); }
protected:
	void _setInt(char *dst, int dlen, int val);
public:
	char vrow[ 2];
	char nrow[ 4];
	char vflg[ 1];
	char gdir[ 1];
	char sdir[ 1];
	char scol[16];
	char ikey[ 1];
	char page[ 4];
	char save[80];
};

class grid_o
{
public:
	grid_o();
	grid_o(LPCSTR src);
	BOOL IsNext();
	CString GetNKey();
	size_t size() { return sizeof(grid_o); }
public:
	char aflg[ 1];
	char sdir[ 1];
	char scol[16];
	char xpos[ 1];
	char page[ 4];
	char save[80];
};
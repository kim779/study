#pragma once
#include "StdAfx.h"

#define VBSCRIPT	0
#define LUA		1

#define BUF_SIZE	1024 * 50
#define SEQ_FORMAT	0x08
#define SEQ_SCNLAYOUT	0x05
#define SEQ_DATA	0x0C
#define SEQ_SCRIPT	0x0E
#define SEQ_CELL	0x7F

#define kindLAYOUT	1
#define kindRTS		2
#define SEQ_OUTPUT	0x0E
#define kindScreen	3

class CBinaryMngr
{
public:
	CBinaryMngr();
	~CBinaryMngr();
private:
	BYTE m_buf[BUF_SIZE];
	BYTE* m_pos;
	CFile m_ObjScpFile;
public:
	void InitBuf();
	void AddBinaryData(void* dst, int size, BYTE seq = NULL);
	void AddTRlayoutData(CString dst);
	void AddRTSData(CString dst);
	void AddScreenLayoutData(CString dst);
	void AddMapScript(CString strPath, int kind);
	void AddObjectScript(CString strPath, int& pos, int size, int kind);
	int  getSize();
	BYTE* getBuf();
public:
	CString parse(CString &src, CString sub);
};
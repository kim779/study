#pragma once

#define	SYM_ITEMN	18
#define	PROJECT_INI	"project.ini"

class Csymbol
{
public:
	CString	m_name;		// symbol name
	CString	m_head;		// label or head
	WORD	m_size;		// data length
	BYTE	m_type;		// data type
	CSize	m_headsz;	// head Size(cx, cy)
	CSize	m_datasz;	// data Size(cx, cy)
	BYTE	m_align;	// data alignment
	DWORD	m_hpRGB;	// head paint RGB
	DWORD	m_htRGB;	// head text  RGB
	DWORD	m_pRGB;		// data paint RGB
	DWORD	m_tRGB;		// data text  RGB
	DWORD	m_attr;		// data attributes
	CString	m_edit;		// edit mask
	BYTE	m_float;	// float point
	BYTE	m_kind;		// form kind : edit, combo, check,..
	BYTE	m_iok;		// I/ O kind
	CString	m_nick;		// nickname
};

class CsymbolSET
{
public:
	CString	m_name;
	CString	m_data;
	CArray < Csymbol *, Csymbol * > m_arSYM;
};

class Cgroup
{
public:
	CString	m_name;
	CString	m_data;
	CArray < CsymbolSET *, CsymbolSET * > m_arGROUP;
};

class Croot
{
public:
	CString	m_name;
	CString	m_file;
	CArray < Cgroup *, Cgroup * > m_arROOT;
};

class Cproject
{
public:
	CString	m_name;
	CString	m_section;
	bool	m_default;
	CArray < Croot *, Croot * > m_arPRO;
};

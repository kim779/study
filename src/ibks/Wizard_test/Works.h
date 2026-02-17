#if !defined(AFX_WORKS_H__4EC52CFE_573A_4620_9D0D_CC1B819F67D0__INCLUDED_)
#define AFX_WORKS_H__4EC52CFE_573A_4620_9D0D_CC1B819F67D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OleDrop.h"

#include "../h/axis.h"
#include "../h/axisfire.h"

#define	S_INS			0x00000001
#define	S_HAN			0x00000002
#define	S_FULL			0x00000004
#define	S_CHILD			0x00000008
#define	S_FIND			0x00000010
#define	S_WAIT			0x00000020
#define	S_LOCK			0x00000040
#define	S_FLASH			0x00000080
#define	S_LOAD			0x00000100
#define	S_IGNORE		0x00000200
#define	S_TRIX			0x00000400	// trigger processing
#define	S_BLINK			0x00000800
#define	S_CURSOR		0x00001000
#define	S_SHIFT			0x00002000
#define	S_CONTROL		0x00004000
#define	S_ING			0x00008000
#define	S_DROP			0x00010000
#define	S_DROPME		0x00020000
#define	S_COMBO			0x00040000
#define	S_WORKS			0x00080000
#define	S_XTRI			0x00100000	// trigger disable
#define	S_FIX			0x00200000	// fix font / fix size
#define	S_PASSWD		0x00400000	// passwd edit
#define	S_AVI			0x00800000

class CWorks  
{
public:
	CWorks()		{}
	CWorks(class CGuard* guard, CWnd* view, int key, int type);
	virtual ~CWorks();

public:
	void OnStream(struct _axisH* axisH, char* pBytes, int nBytes);
	virtual bool Attach(CString maps, bool only, bool fix = false);
	virtual void OnAxis(struct _axisH* axisH, char* pBytes, int nBytes);
	virtual void OnAlert(CString code, CString update, int stat);
	virtual void OnTrigger(CString name, CString text, int key);
	virtual void OnProcedure(CString proc, CString param, int key = 0);
	virtual void OnNotice(CString text);
	virtual bool isWorks();
	void OnSave(CString name, CString text);
	virtual BOOL GetAt(CString name, CString& text);
	virtual	BOOL GetData(CString& name, CString& tmps);
	virtual void OnDomino(CString domino);
	virtual int OnInvoke(int wParam, int lParam);
	virtual void OnDrop(CPoint point, CString text, CWnd* pWnd = NULL);
	BOOL Register(HWND hWnd, bool tips = false);
	void Revoke(HWND hWnd);
	virtual void SetPalette();
	virtual void SetFCB(bool blink, int flash);
	virtual	BOOL SetFont(int point, bool resize = false, bool attach = false);
	virtual	void SetGID(int group)	{ m_group = group; }
	virtual bool isTrigger(int key, CString name, CString text);
	virtual	bool isYours(int key);

public:
	CGuard*		m_guard;
	CWnd*		m_view;
	int		m_key;
	int		m_type;
	CRect		m_rect;
	FARPROC		m_callproc;

	UINT		m_status;
	int		m_group;
	int		m_vid;
	CString		m_mapN;

	int		m_font;

protected:

private:
//	class COleDrop*	m_drop;
	class COleDrop	m_drop;
	int		m_axisL;
	char*		m_axisB;

	CMapStringToString	m_saves;
};

#endif // !defined(AFX_WORKS_H__4EC52CFE_573A_4620_9D0D_CC1B819F67D0__INCLUDED_)

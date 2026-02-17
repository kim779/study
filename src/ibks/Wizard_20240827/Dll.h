// Dll.h: interface for the CDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLL_H__6F62A430_AB2E_4109_997E_1D25594DEC7A__INCLUDED_)
#define AFX_DLL_H__6F62A430_AB2E_4109_997E_1D25594DEC7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Works.h"

class CDll : public CWorks  
{
public:
	CDll(CGuard* guard, CWnd* view, int key, int type);
	virtual ~CDll();

	bool	Attach(CString maps, bool only, bool fix = false);
	void	OnAxis(struct _axisH* axisH, char* pBytes, int nBytes);
	void	OnAlert(CString code, CString update, int stat);
	void	OnTrigger(CString name, CString text, int key);
	void	OnProcedure(CString proc, CString param, int key = 0);
	void	OnNotice(CString text);
	void	OnDomino(CString domino);
	int	OnInvoke(int wParam, int lParam);
	void	OnDrop(CPoint point, CString text, CWnd* pWnd);
	BOOL	GetData(CString& name, CString& tmps);
	void	SetPalette();
	void	SetFCB(bool blink, int flash);
	int	GetSize(CString maps);
	int	Attach(CWnd* pWnd, int type = 0);
	void	Detach(int key);
	BOOL	Load(int key, CString maps);
	void	Domino(int key, CString data);
	BOOL	SetFont(int point, bool resize = false, bool attach = false);
	void	SetGID(int group);
	int	GetViewType(CString mapN);
	BOOL	IsWait(int key);
	BOOL	IsWait();
	bool	isTrigger(int key, CString name, CString text);
	bool	isYours(int key);
	void	OnMouse(CString infos, BOOL left);

	void	OnUnit(unsigned char key);
	void	xAlert(int unit = -1);

	void	OnAlert(void* data);

public:
	CWnd*		m_dll;

private:
	HINSTANCE	m_instance;
	CMapWordToPtr	m_screens;
	CMap	<BYTE, BYTE, BYTE, BYTE&>	m_units;
	CMap	<BYTE, BYTE, BYTE, BYTE&>	m_keys;

	CString		m_string;

private:
	void	Free();
};

#endif // !defined(AFX_DLL_H__6F62A430_AB2E_4109_997E_1D25594DEC7A__INCLUDED_)

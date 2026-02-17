// Script.h: interface for the CScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPT_H__B1BDDF50_1DD5_4C2B_A026_1BB34309FDA0__INCLUDED_)
#define AFX_SCRIPT_H__B1BDDF50_1DD5_4C2B_A026_1BB34309FDA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define	evStart		1
#define	evFocus		2
#define	evClose		3
#define	evSend		4
#define	evReceive	5
#define	evKey		6
#define	evService	7
#define	evApprove	8
#define	evDevice	9
#define	evTimer		10
#define	evSelect	11
#define	evFile		12
#define	evAlert		13
#define	evSize		14
#define	evTimerX	15

#define	evClick		100
#define	evChange	101
#define	evDblClk	102
#define	evMouseDn	103
#define	evMouseUp	104
#define	evChar		105
#define	evSetFocus	106
#define	evMouseEnter	107
#define	evMouseLeave	108


class CScript  
{
public:
	CScript(class CGuard* guard = NULL);
	virtual ~CScript();

// Attributes
public:
	bool		m_script;

protected:
	class CGuard*	m_guard;
	class CClient*	m_client;
	class CScreen*	m_screen;

// Operations
public:
	bool	OnStart(class CScreen* screen);
	bool	OnFocus(class CScreen* screen);
	bool	OnClose(class CScreen* screen);
	bool	OnSend(class CScreen* screen);
	bool	OnReceive(class CScreen* screen, CString trs);
	bool	OnKey(class CScreen* screen, class CfmBase* form = NULL);
	bool	OnService(class CScreen* screen, char* data, int datL);
	bool	OnApprove(class CScreen* screen, int key, char* data, int datL);
	bool	OnDevice(class CScreen* screen);
	bool	OnTimer(class CScreen* screen);
	bool	OnTimerX(class CScreen* screen, int id);
	bool	OnSelect(class CScreen* screen, CString maps);
	bool	OnFile(class CScreen* screen, BOOL upload, char* data, int datL);
	bool	OnAlert(class CScreen* screen, CString code, CString data, int stat);
	bool	OnSize(class CScreen* screen);

	bool	OnChange(class CScreen* screen, class CfmBase* form, bool byKey = false);
	bool	OnClick(class CScreen* screen, class CfmBase* form, bool byKey = false);
	bool	OnDblClick(CScreen* screen, class CfmBase* form);
	bool	OnMouseDown(class CScreen* screen, class CfmBase* form);
	bool	OnMouseUp(class CScreen* screen, class CfmBase* form);
	bool	OnCharChange(class CScreen* screen, class CfmBase* form);
	bool	OnSetFocus(class CScreen* screen, class CfmBase* form);
	bool	OnMouseEnter(class CScreen* screen, class CfmBase* form);
	bool	OnMouseLeave(class CScreen* screen, class CfmBase* form);

	bool	OnProcedure(class CScreen* screen, CString procs);

	bool	Procedure(class CScreen* screen, CString procs, CString data, int count = -1);
	bool	IsAvailable(class CScreen* screen, int event, class CfmBase* form = NULL);

protected:
	CString	getProcName(int event, CString symbol = _T(""));
	void	initialize(class CScreen* screen);
};

#endif // !defined(AFX_SCRIPT_H__B1BDDF50_1DD5_4C2B_A026_1BB34309FDA0__INCLUDED_)

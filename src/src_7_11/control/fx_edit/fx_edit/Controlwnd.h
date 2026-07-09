#pragma once

#include "../../../h/axisfire.h"
// CControlwnd
class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};

struct mid
{
	char userid[30];
};

struct mod
{
	char name[30];
};


struct	_ledgerH {
	//char	len[8];
	char	anam[40];		/* application name	*/
	char	snam[40];		/* service name		*/
	char	fnam[32];		/* function name	*/
	char	fncd[2];		/* function code	*/
	char	guid[32];		/* global id		*/
	char	ctyp[2];		/* channel type		*/
	char	eflg[1];		/* environment type	*/
	char	tflg[1];		/* tran flag		*/
	char	tdat[8];		/* tran send date	*/
	char	ttim[20];		/* tran send time	*/
	char	clip[31];		/* client ip		*/
	char	rtyp[1];		/* response type	*/
	char	rcod[8];		/* response code	*/
	char	rlcd[20];		/* response log code	*/
	char	rtit[50];		/* response title	*/
	char	rbmg[100];		/* response message	*/
	char	rdmg[500];		/* response message	*/
	char	usid[12];		/* user id		*/
	char	crst[2];		/* character set	*/
	char	fill[78];		/* filler		*/
	char	tmno[15];		/* terminal no		*/
	char	tmps[15];		/* terminal position	*/
	char	dept[8];		/* user department	*/
	char	brnc[8];		/* user branch		*/
	char	sale[8];		/* business branch	*/
	char	agnc[8];		/* agency		*/
	char	sfill[38];		/* filler		*/
	char	susid[20];		/* user id		*/
	char	menu[20];		/* menu			*/
};

class CControlwnd : public CWnd
{
	DECLARE_DYNAMIC(CControlwnd)

public:
	CControlwnd();
	virtual ~CControlwnd();

	virtual void OnFinalRelease();
	CFont		m_Font;
	CWnd*		m_pWizard;
	CParam		m_Param;
	class CCodeEdit  *m_pEdit;

	CString m_strMsgUser;
	CString m_strMsgName;

	CString m_home;
	CString m_sUserName;
	CString m_sUserID ;
	CString Parser(CString &srcstr, CString substr);
	void  ResizeControls();
	void	SetParam(struct _param *pParam);
	CString Variant(int nComm, CString strData = _T("") );
	bool sendTR(CString trC, char* pBytes, int nBytes);
	void MessangerMemoPop(LPCTSTR userid);
	void GetledgerH(struct _ledgerH* pLedgerH);
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
protected:
	BSTR GetData(void);

	enum 
	{
		dispidGetUserName = 7L,
		dispidSetVisible = 6L,
		dispidSetID = 5L,
		dispidSetName = 4L,
		dispidSetEnable = 3L,
		dispidSetData = 2L,
		dispidGetData = 1L
	};
	void SetData(LPCTSTR strdata);
	void SetEnable(LONG benable);
	void SetName(LPCTSTR name);
	void SetID(LPCTSTR id);
	void SetVisible(LONG bvisible);
	void GetUserName(LPCTSTR id);
};



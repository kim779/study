#pragma once
#include "stdafx.h"

#define WM_SOCK WM_USER + 701

// CMSocketCtrl

class CMSocktCtrl : public CWnd
{
public:
	CMSocktCtrl();
	virtual ~CMSocktCtrl();

public:
	void SetWnd(CWnd *pWnd){ m_pEmule = pWnd; }
	BOOL OpenSock(LPCTSTR home, LPCTSTR ip, long port);
	void CloseSock();
	BOOL WriteStream(LONG_PTR pBytes, long nBytes);
	BOOL OpenDgram(long port);
	void CloseDgram();
	BOOL WriteDgram(LPCTSTR ip, LONG_PTR pBytes, long nBytes, BOOL route);
	BOOL StartAxis(LONG_PTR key, long mode);
	long GetEnum(LONG_PTR pBytes, long nBytes, long mode);	
	void OnStream(long type, LONG_PTR pBytes, long nBytes) 
	{ 
		if (m_pEmule && m_pEmule->GetSafeHwnd())
			m_pEmule->SendMessage(WM_SOCK, MAKEWPARAM(type, nBytes), (LPARAM)pBytes); 
	}
	//{FireEvent(eventidOnStream,EVENT_PARAM(VTS_I4  VTS_I8  VTS_I4), type, pBytes, nBytes);}
// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CSockCtrl)
	dispidOpenSock = 1L,
	dispidCloseSock = 2L,
	dispidWriteStream = 3L,
	dispidOpenDgram = 4L,
	dispidCloseDgram = 5L,
	dispidWriteDgram = 6L,
	dispidStartAxis = 7L,
	dispidGetEnum = 8L,
	eventidOnStream = 1L,
	//}}AFX_DISP_ID
	};

private:
	class CDgram*	m_dgram;
	class CStream*	m_stream;
	class CWsh*	m_rsm;
	class CWsh*	m_wsh;
	
	CWnd* m_pEmule;
protected:
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnFireEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireAlert(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFirePush(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireAxis(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireStat(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireWrite(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireVersion(WPARAM wParam, LPARAM lParam);

};



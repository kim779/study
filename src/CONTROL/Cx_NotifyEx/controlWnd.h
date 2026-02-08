#pragma once

#include "afxtempl.h"
#include "info.h"
#include <afxmt.h>

class CMchg
{
public:
	CMchg() { m_md = m_ms = 0; }
	~CMchg(){ m_md = m_ms = 0; }
	void	SetMchg(CString gubnS, int amt);
	void	SetMchg(CString gubnS, CString stateS, int amt);
	void	GetMchg(int& md, int& ms);
private:
	int	m_md;
	int	m_ms;
};

//================================================================================
class CControlWnd : public CWnd
{
public:
	CControlWnd();
	virtual ~CControlWnd();
	
	//{{AFX_VIRTUAL(CControlWnd)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_DISPATCH(CControlWnd)
	CString m_flag;
	afx_msg void OnFlagChanged();
	CString m_dataList;
	afx_msg void OnDataListChanged();
	afx_msg void SendX(LPCTSTR accnS, LPCTSTR passS, LPCTSTR gubnS);
	afx_msg void Clear();
	//}}AFX_DISPATCH
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	void	SetParam(struct _param *pParam);

private:
	CString	variant(int comm, CString data);
	CString	parser(CString &srcstr, CString substr);
	CString	optionParser(CString sOption, CString sKey);

	void	queryJango(CString accnS = "", CString passS = "");
	void	queryCont(CString ledgerS = "");
	void	queryMingam();
	bool	oubRemain(char* pData, CString msgS);
	bool	oubCont(char* pData, CString msgS);
	void	oubMingam(char *pData, int len);
	void	oubFCurr(char *pData, int len);
	CString	getLedger(CString sTran, CString sSvcn, CString sGubn);
	BOOL	sendTR(CString sTR, CString sData, int nKey, int nStat);

	void	alert(CString rtsS);
	bool	notice(CString noticeS);
	

	void	deleteCodeMap();
	void	deleteMchgMap();
	bool	checkFuPosition(CString codeS, CString gubnS);
	void	sendToMap(int smType = SM_ALL, CString codeS = "");
	CString	getEditData(CString totalS);

	CString	getTime(bool bPID = true);
public:
	CWnd*		m_pParent;
	CParam		m_Param;
private:
	CMap<CString, LPCTSTR, CInfo*, CInfo *>		m_mapCode;
	CMap<CString, LPCTSTR, CMchg*, CMchg *>		m_mapMchg;

	CArray<CString, CString>	m_arCode;

	class COpSensCal	*m_pOpSensCal;		// 변동성 계산(델타,감마)
	CArray <int, int>	m_arField;

	int			m_nPosition;
	CString			m_passS, m_accnS;
	int			m_sendKey;
	struct	_mingamBase	m_mgBase;
	CString			m_oopCode;
	bool			m_bAlert;
	bool			m_bEnableSend;

	CCriticalSection	m_Section;

};
#pragma once

#include <afxtempl.h>
#include <afxmt.h>
#include "../../h/interst.h"
#include "captureWnd.h"

class CMapWnd : public CWnd
{
public:
	CMapWnd();
	virtual ~CMapWnd();
	//{{AFX_VIRTUAL(CMapWnd)
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnCapture(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	BOOL	CreateMap(CWnd *pParent);

private:
	void	initConstruct();
	void	destroy();
	void	deleteAllMapCatch();
	void	deleteAllMapCapture();
	bool	loadCatch(bool bLog);
	bool	saveCatch(CString codeS = _T(""));

	void	querySise();
	BOOL	sendTR(CString name, BYTE type, CString data, int key);
	void	sendTR(CString trC, char* pBytes, int nBytes, int key, BYTE stat);

	void	oubSise(char *buf, int bufL);
	void	oubHisLog(char *buf, int len);

	void	alert(char* buf, int len);
	void 	alertx(gsl::span<DWORD> data, struct _codeinfo* codeinfo);
	void	trigger(CString tmpS);
	
	CString	getString(char* datB, int datL);
	int	openView(int type, CString data);
	void	writeLog(char* format, ...);
	void	writeLog(CString tmpS);
	void	deleteLog();
	CString	SetOrderDominoData(_codeinfo *codeinfo);

	void	writeHistory(_codeinfo *codeinfo, char chk[6], CString currS, CString diffS, CString mdgaS, CString msgaS, CString rateS, CString gvolS);
private:

	CWnd		*m_pParent;
	CCriticalSection m_section;

	CString	m_sRootDir;	CString	m_sTabDir;				// directory
	CString	m_sUser;	CString	m_sFilename;	CString m_envPath;
	CString	m_sLogFile;	CString	m_sHisFile;
	CString	m_sFontName;	int	m_nFontPoint;	
	CFont*	m_pFont;	CFont*	m_pBFont;				// font

	std::vector<std::unique_ptr<_codeinfo>> _vinfo;
	CMap<CString, LPCTSTR, _codeinfo *, _codeinfo *>	m_mapCatch;
	CMap<int, int, CCaptureWnd *, CCaptureWnd *>		m_mapCapture;
	
};

#if !defined(AFX_INFOFILE_H__03ED7047_86BD_42F3_941C_AB58B55B29A5__INCLUDED_)
#define AFX_INFOFILE_H__03ED7047_86BD_42F3_941C_AB58B55B29A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Infofile.h : header file
//
#include "XZip.h"
#include "XUnzip.h"
#include <afxcoll.h>
struct	usrinfo {
       char    gubn[2];                /* 업무 구분                    */
                                       /* MY: 관심 종목                */
                                       /* UL: 사용자 데이타 목록       */
                                       /* UD: 사용자 데이타            */
				       /* UX: Delet Chart		*/
       char    dirt[1];                /* 전송 방향                    */
                                       /* U: PC=>HOST  D: HOST=>PC     */
       char    cont[1];                /* 연속 구분                    */
                                       /* F: First     M: Middle       */
                                       /* L: Last      C: Cancel       */
				       /* D: delete file               */	
       char    name[80];               /* file name (필요시만)         */
       char    nblc[5];                /* 블럭 갯수 or                 */
                                       /* 화일 SIZE (max. 64K)         */
       char    retc[1];                /* O: 정상      E: 에러         */
       char    emsg[40];               /* error message                */
};

struct  d_list {                        /* 사용자 데이타 LIST           */
       char    name[80];               /* file name                    */
       char    size[5];                /* file size                    */
};

#define L_usr	sizeof(struct usrinfo)
#define L_lst	sizeof(struct d_list)

#define APPLY_NONE	0x00
#define APPLY_ENSETUP	0x01
#define APPLY_TOOLBAR	0x02
#define APPLY_TICKER	0x04
#define APPLY_WORKSPACE	0x08
#define APPLY_CODEMEMO	0x10
/////////////////////////////////////////////////////////////////////////////
// CInfofile dialog
#include "useMaindefine.h"
class CInfofile : public CDialog
{
// Construction
public:
	CInfofile(CWnd* wizard, bool upload = true, CWnd* parent = NULL);   // standard constructor
	~CInfofile();

	void	ProcessFMX(WPARAM wParam, LPARAM lParam);
	DWORD	GetApplyBits()	{ return m_applyBits; }

	bool GetUploadMemoYN() { return m_bUploadMemo; }
protected:
	CString Parser(CString &srcstr, CString substr);
	void	Init();
	CWnd*	GetWnd(UINT nRes) { return GetDlgItem(nRes); }
	void	QueryFilelist();
	void	ClearChart();
	bool	Ready();
	void	Uploadfile();
	void	Downloadfile();
	void	Decompressfile();
	void	removeCompressfile();
	CString	Compress(CString target, CString path = _T(""));
	CString CompressFolder(CString target, CString path = _T(""));
	CString	CompressAll(CString path);
	void	CompressChart(CString path);
	void	CompressDRFNChart(CString path);
	void	Decompress(CString target);
	void	SetApplyBits(CString fName);
	void	FindAllFiles(CString path,WIN32_FIND_DATA FindFileData,TCHAR* cp);
	void	CreateFolder(CString strTargetPath);
// Dialog Data
	//{{AFX_DATA(CInfofile)
	enum { IDD = IDD_USEINFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfofile)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD	m_applyBits;
	bool	m_upload;
	bool	m_wait;
	bool	m_clearchart;
	CBrush	m_brush;

	CWnd*	m_wizard;
	CString	m_msg;

	CStringArray	m_filelist;
	CStringArray	m_sfilelist;
	CStringArray	m_errfile;
	CStringArray	m_removelist;
	CStringArray	m_chartlist;

	bool	m_bULFlag;
	CString m_strUL;
	bool m_bUploadMemo{}; //memo
	// Generated message map functions
	//{{AFX_MSG(CInfofile)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAll();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg	LONG OnMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
protected:
#ifdef DF_USE_CPLUS17
	std::unique_ptr <CControlChanger>m_changer;
#else
	CControlChanger* m_changer;
#endif
	
	void OnAction();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOFILE_H__03ED7047_86BD_42F3_941C_AB58B55B29A5__INCLUDED_)

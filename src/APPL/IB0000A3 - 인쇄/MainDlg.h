#pragma once
// MainDlg.h : header file
//
#include "FormParser.h"
#include "Afxmt.h"
/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class CMainDlg : public CDialog
{
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor

	CWnd*				m_pAxMain;
	CString				m_strUserID, m_strDeptCode, m_strPass, m_strRootDir, m_strImgPath;
	CFormParser			m_clParser;
	QueryList			m_stQuery;
	OutFormList			m_stOutput;
	COutDataListArray	m_arGridList;
	OutDataList			m_stSimpleList;
	CStringArray		m_arMultiLines[MAXCOUNT];

	CString				m_strLedger;
	CMapStringToString	m_arDominoList;
	BOOL				m_bNext, m_bListAdd, m_bTestMode, m_bFirstData;
	int					m_iNextCount, m_iQueryCount;
	int					m_iPreviewPage, m_iScreenTotPage;;
	int					m_iDrawX, m_iDrawY;
	bool				m_bExecPrn;
	CString				sSumdata;					//인쇄 추가 데이터(자체 계산)
	CString				sDomino02;					//도미노
	CString				sParseData[100];			// 1401 파싱된 데이타
	CCriticalSection	m_section;
	void				ParseSum(CString sSumdata);
	CBitmap*			getBitmap(CString path);
	void				SetInitValue() ;
	LRESULT				SendAxisMessage(WPARAM wParam, LPARAM lPAram);
	void				sendTR(CString trCode, char* datB, int datL, char key);
	CString				GetLedger(CString svcd, CString strAccount, char cMarket='3', char cOdrf='2', CString sNext=_T("")) ;
	void				sendTransaction(CString sAccount, CString strSVCD, int iInputCnt, CString* sInputType, int* iInputLen, CString* sInputData) ;
	void				ParsingOubData(CString sReceive);
	void				LoadReportType(int itype);

	void				ParsingData(int ilinecnt, int irowcnt, CString sData, int istartidx, int icolcnt, int* iLenlist, int* iFormatList, 
						   CString* sTokenList, COutDataListArray& arDataList);
	void				ParsingMultiData(CString sData);
	CString 			ParsingGridData(CString sData);
	void				ParsingSimpleData(CString sData, int itype=0);
	

	CString				GetInputData(int idx);
	CString				GetOutputData(CString sText);
	int					GetGridTextMargin();
	int					GetTextMargin();
	CString				GetDocName();
	CString				GetHeaderText(CString sText);

	CString				GetCurrentDate(CString stoken);
	CString				GetCurrentTime1(CString stoken);
	CString				GetDominoData(CString sDominoSymbol);

	int					GetMultiLineSze(int icontentIDX);
	int					GetMultiLineSze(int icontentIDX, CStringArray& arMultiLines);



	void				SetDominoData(CString sData);

	void				SetProgressRange(int iMax, int iMin) ;
	void				SetPrinterStatus(CString sMessage) ;
	void				SetProgressPos(int iPos) ;
	void				SetPrinterPage(CString sPage) ;




	void				InsertQueryList(CString sData);
	void				InsertOutputGridList(int icolcnt);
	void				ending();



	void				DrawScreen(CDC* pDC);


	BOOL				m_bOneWay;   //일방향 암호화
	
	CString				GetEncPassword(CString sPswd);
	HINSTANCE			m_hiSha256;









// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAINDIALOG };
	CListCtrl			m_clxOutputList;
	CListCtrl			m_clxQueryList;
	CTextProgressCtrl	m_Progress;
	CComboBox			m_cboReportType;
	CListBox			m_OubList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTest();
	afx_msg void OnPrnTest();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNextPAge();
	afx_msg void OnPrevPage();
	afx_msg void OnClosebutton();
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


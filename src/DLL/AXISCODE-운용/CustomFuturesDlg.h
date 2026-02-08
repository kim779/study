#pragma once
// CustomFuturesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomFuturesDlg dialog

#include "skinlistctrl.h"
#include "memDC.h"
#include "customcheckbox.h"
#include "btnst.h"
#include "resource.h"
#include "tool.h"
#include "myedit.h"
#include "MyBitmap.h"
#include "user.h"
#include "../../h/jmcode.h"

class CCustomFuturesDlg : public CDialog		// 선물 옵션 다이얼로그 
{
// Construction
public:
	CCustomFuturesDlg(int kind, CPoint pt = CPoint(-1, -1), int type = 0, CString szcode = "", CWnd* pParent = nullptr);   // standard constructor

	bool	GetCodeOrderable(CString code, int* type);
	void	SetDlgPos(int x, int y);
	CString	CommaModify(CString string);
	void	SetButton();
	BOOL	FindCode(int kind, CString szCode);
	void	MonthInit();
	void	InitBtn();
	void	SetCode(CString szCode){ m_szEditCode = m_szCode = szCode; }
	void	SetName(CString szName){ m_szEditName = m_szName = szName; }
	CString GetCode(){ return m_szCode; }
	CString GetName(){ return m_szName; }
	CString GetBCode(){ return m_szBCode; }
	CString GetBName(){ return m_szBName; }
	int	GetKind(){ return m_Kind; }	
	int	GetCountMonth(){ return m_nCountMonth; }
	int	GetJongmuk(){ return m_jongmuk; }
	
	CArray<CString, CString>* GetArray(){ return &m_ArrayMonth; };
	
	
	
// Dialog Data
	//{{AFX_DATA(CCustomFuturesDlg)
	enum { IDD = IDD_DIALOG_CUSTOMFUTURES };
	CButtonST	m_ctlButtonSPMonth9;
	CButtonST	m_ctlButtonSPMonth6;
	CButtonST	m_ctlButtonSPMonth3;
	CButtonST	m_ctlButtonKostar;
	CListBox	m_plistbox;
	CMyEdit		m_ctlEditName;
	CMyEdit		m_ctlEditCode;
	CButtonST	m_ctlButtonMonth12;
	CButtonST	m_ctlButtonMonth3;
	CButtonST	m_ctlButtonMonth6;
	CButtonST	m_ctlButtonMonth9;
	CButtonST	m_ctlButtonName;
	CButtonST	m_ctlButtonKospi;
	CString		m_szEditCode;
	CString		m_szEditName;
	int		m_kindList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomFuturesDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	
// Implementation
protected:

	void DisableButton(long lExcept = 0);
	void CountData(CString code, CString codx);
	void FindEnd();
	_DATA FindPut(CString szMonth, CString szPrice);
	_DATA FindCall(CString szMonth, CString szPrice);
	BOOL FindMonth(CString szMon);
	void SearchMonth();
	void AddList(const PCODE& pcode);

	void SearchPrice();
	void Standard(CString szCode, CString codx);
	void ButtonText(CString szName, CString szCode);
	void HideButton(BOOL flag);
	BOOL FindPrice(CString szCompare);

public:
	void SetButton(CButtonST *pButton);
	void SetCodeList(int nMonth);
	void SetFutureCodes(BOOL bKostar = FALSE, CString codx = _T("")) ;
	CString GetString(char *pChar, int nMinLen);
	int		m_type;
	int		m_jongmuk;
protected:

	std::unique_ptr<CCustomCheckBox>	m_pCheck;
	CString		m_szName;
	CString		m_szCode;
	CString		m_szBName;
	CString		m_szBCode;
	int		m_Kind;		// select kind...
	CTool		m_Tool;
	CPoint		m_point;
	CRect		m_rcCheck;
	int		m_nAtm;
	int		m_nCountMonth;
	
	CArray<FCODE, FCODE>		m_arrayFcode;
	CArray<ojcode, ojcode>		m_arrayOcode;
	CArray<PCODE, PCODE>		m_arrayPcode;
	CArray<PCODE, PCODE>		m_arrayput;
	CArray<PCODE, PCODE>		m_arraycall;	
	
	CList<PCODE, PCODE>		m_ListPcode;
	CArray<CString, CString>	m_ArrayPrice;
	CList<CString, CString>		m_ListMonth;
	CArray<CString, CString>	m_ArrayMonth;
	HBRUSH				m_br;

	// Generated message map functions
	//{{AFX_MSG(CCustomFuturesDlg)
	afx_msg void OnButtonClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnButtonKukmin();
	afx_msg void OnButtonKospi200();
	afx_msg void OnButtonSamsung();
	afx_msg void OnButtonSk();
	afx_msg void OnButtonPosco();
	afx_msg void OnButtonKt();
	afx_msg void OnButtonPower();
	afx_msg void OnButtonHyundalcar();
	afx_msg void OnSelchangePcodeList();
	afx_msg void OnButtonKostar();
	afx_msg void OnButtonSpmonth3();
	afx_msg void OnButtonSpmonth6();
	afx_msg void OnButtonSpmonth9();
	afx_msg void OnOk();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonMonth3();
	afx_msg void OnButtonMonth6();
	afx_msg void OnButtonMonth9();
	afx_msg void OnButtonMonth12();
	//}}AFX_MSG
	afx_msg	LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
/////////////////////////////////////////////////////
// 2006.11.14 이인호
/////////////////////////////////////////////////////
public:
	BOOL	loadSfCode();		// 주식선물파일...	
private:
	CArray<SFCODE, SFCODE>		m_arraySfCode;	// 주식선물
	CMapStringToString		m_mapSFCode;
	int				m_nBorder;	
	CString				m_szFindText;
	CArray<int, int>		m_arFindIndex;
	int				m_nSavePos;
	bool				m_bFind;
	
	CString GetShortCode(CString code);
	void	SetSFCodes(int nIdx, CString codx = _T(""));	
	BOOL	GetCurrentSFCODE(CString& code, CString& name, int nIndex);

	void	CreateChild();
	int	CorrectType(int type);
	void	RemoveAllData();
	void	init();
	CString MakeTitle();
	BOOL	loadFjCode(); // 선물파일...
	BOOL	loadOjCode(); // 옵션파일...
	BOOL	loadPjCode(); // 현물파일...
	void	loadfile();
	void	setOption();
	void	UnPushed();
	void	Select(CString code);
	void	Select(WORD kind, CString code = "");
	void	selectFUTURE(CString code);
	void	selectKOSTA(CString code);
	void	selectSFUTURE(CString code);
	void	selectOPTION(CString code);
	void	selectKOPTION(CString code);
	WORD	findKind(const CString& codx);
	WORD	findFirstKind();	
	void	FutureMode();
	void	OptionMode();
	void	SetTitle(CString title);
	void	OrderEnable();
	void	ClickList();
	void	DrawEmptyCheck();
	
	
/////////////////////////////////////////////////////
};


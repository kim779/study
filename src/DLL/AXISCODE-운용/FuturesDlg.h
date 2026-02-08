#pragma once
// FuturesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFuturesDlg dialog

#include "skinlistctrl.h"
#include "memDC.h"
#include "checkbox.h"
#include "stockcheckbox.h"
#include "btnst.h"
#include "resource.h"
#include "tool.h"
#include "myedit.h"
#include "user.h"
#include "../../h/jmcode.h"

template <class T, class PT> class CQArray : public CArray <T, PT>
{
public:
	void QuickSort(BOOL bAscending = TRUE);
};

template <class T, class TP> void CQArray<T,TP>::QuickSort(BOOL bAscending/* = TRUE*/)
{
	if (GetSize() > 1)
	{
		::QuickSort(GetData(), GetSize(), bAscending);
	}
}

template <class T> BOOL QuickSort(T *pArr, int iSize, BOOL bAscending = TRUE)
{
	BOOL rc = TRUE;

	if (iSize > 1) 
	{
		try 
		{
			const int	low = 0,
				high = iSize - 1;

			QuickSortRecursive(pArr, low, high, bAscending);
		} 
		catch (...) 
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}
	} 
	else 
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}

	return rc;
}

template <class T> void QuickSortRecursive(T *pArr, int low, int high, BOOL bAscending)
{
	int ii{}, jj{};
	T str;
	T TTemp;

	ii = high;
	jj = low;

	str = pArr[((int) ((low+high) / 2))];

	do {
		if (bAscending) 
		{
			while (pArr[jj].Compare(str) < 0) jj++;
			while (pArr[ii].Compare(str) > 0) ii--;
		}
		else
		{
			while (pArr[jj].Compare(str) > 0) jj++;
			while (pArr[ii].Compare(str) < 0) ii--;
		}
		if (ii >= jj) 
		{
			if (ii != jj) 
			{
				TTemp = pArr[ii];
				pArr[ii] = pArr[jj];
				pArr[jj] = TTemp;
			}
			ii--;
			jj++;
		}
	} while (jj <= ii);

	if (low < ii) QuickSortRecursive(pArr, low, ii, bAscending);
	if (jj < high) QuickSortRecursive(pArr, jj, high, bAscending);

}

class CSortStringArray : public CStringArray {
public:
	void Sort();
private:
	BOOL CompareAndSwap(int pos);
};

#define NORMAKKOSPI200   0
#define MINIKOSPI200	 1
#define WEEKLYOPTION     2

class CFuturesDlg : public CDialog		// 선물 옵션 다이얼로그 
{
// Construction
public:
	CFuturesDlg(int kind, CPoint pt = CPoint(-1, -1), int type = 0, CWnd* pParent = nullptr);   // standard constructor

	bool	GetCodeOrderable(CString code, int* type);
	void	SetDlgPos(int x, int y);
	CString	CommaModify(CString string);
	void	SetButton();
	void	DisableFuture();
	BOOL	FindCode(int kind, CString szCode);
	void	MonthInit();
	void	InitBtn();
	void	SetCode(CString szCode){ m_szEditCode = m_szCode = szCode; }
	void	SetName(CString szName){ m_szEditName = m_szName = szName; }
	void	SetMgubn(CString szMgubn){ m_szMgubn = szMgubn; }
	CString GetCode(){ return m_szCode; }
	CString GetName(){ return m_szName; }
	CString GetMgubn(){ return m_szMgubn; }	//2016.13.18 KSJ 주식선물 마켓구분
	int	GetKind(){ return m_Kind; }
	int	GetCountMonth(){ return m_nCountMonth; }
	int	GetJongmuk(){ return m_jongmuk; }

	BOOL GetCommCode( CString& strCode );
	
	CArray<CString, CString>* GetArray(){ return &m_ArrayMonth; };
	CMapStringToString m_codemap;
	
	CStringArray m_arrA, m_arrB;	//2016.06.21 KSJ 달러 메뉴에 삽입할 텍스트

	BOOL	loadFjCode(); // 선물파일...
	BOOL	loadOjCode(); // 옵션파일...
	BOOL	loadPjCode(); // 현물파일...
	BOOL	loadCFjCode(); // 상품선물 파일 
	BOOL	loadSfCode(); // 주식선물 파일
	BOOL	loadMoCode(); // 미니 코스피200 옵션파일...
	BOOL	loadMfCode(); // 미니 코스피200 선물파일...
	BOOL	loadWCode();  // 위클리옵션

// Dialog Data
	//{{AFX_DATA(CFuturesDlg)
	enum { IDD = IDD_DIALOG_FUTURES };
	CButton	m_btnSearchPrev;
	CButton	m_btnSearchNext;
	CComboBox	m_cbMonth2;
	CComboBox	m_cbMonth1;
	CButtonST	m_btnSPMonth92;
	CButtonST	m_btnSPMonth62;
	CButtonST	m_btnSPMonth32;
	CButtonST	m_btnSPMonth122;
	CButtonST	m_btnSPMonth12;
	CButtonST	m_btnMonth92;
	CButtonST	m_btnMonth62;
	CButtonST	m_btnMonth32;
	CButtonST	m_btnMonth122;
	CButtonST	m_btnMoney;
	CButtonST	m_btnGumri;
	CButtonST	m_btnComm;
	CSkinListCtrl	m_codeList;
	CButtonST	m_ctlButtonSPMonth9;
	CButtonST	m_ctlButtonSPMonth6;
	CButtonST	m_ctlButtonSPMonth3;
	CButtonST	m_ctlButtonOPMonth4;
	CButtonST	m_ctlButtonOPMonth3;
	CButtonST	m_ctlButtonOPMonth2;
	CButtonST	m_ctlButtonOPMonth1;
	CButtonST	m_ctlButtonKostar;
	CButtonST	m_ctlButtonJOption;
	CListBox	m_plistbox;
	CMyEdit		m_ctlEditName;
	CMyEdit		m_ctlEditCode;
	CButtonST	m_ctlButtonMonth12;
	CButtonST	m_ctlButtonMonth3;
	CButtonST	m_ctlButtonMonth6;
	CButtonST	m_ctlButtonMonth9;
	CButtonST	m_ctlButtonName;
	CButtonST	m_ctlButtonKospi;
	CButtonST	m_ctlButtonName2;		//2015.06.29 KSJ 미니 코스피200 추가
	CButtonST	m_ctlButtonMiniK200;	//2015.06.29 KSJ 미니 코스피200 추가
	CButtonST	m_ctlButtonUSD;		//2016.06.17 KSJ USD
	CButtonST	m_ctlButtonUSD1;	//2016.06.17 KSJ USD ▼
	CButtonST	m_ctlButtonSPUSD;	//2016.06.17 KSJ SP USD
	CButtonST	m_ctlButtonSPUSD1;	//2016.06.17 KSJ SP USD ▼
	CString		m_szEditCode;
	CString		m_szEditName;
	int		m_kindList;
	CButtonST	m_ctlButtonWEEKLYOP;
	CButtonST	m_ctlButtonOPWeek;   //옵션 다이알로그 내부에 추가된 위클리 버튼
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFuturesDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	
// Implementation
protected:

	void DisableButton(long lExcept = 0);
	void CountData(CString code);
	void FindEnd();
	DATA FindPut(CString szMonth, CString szPrice);
	DATA FindCall(CString szMonth, CString szPrice);
	BOOL FindMonth(CString szMon);
	void SearchMonth();
	void AddList(const PCODE& pcode);

	void SearchPrice();
	void Standard(CString szCode);
	void ButtonText(CString szName, CString szCode, BOOL bName2 = FALSE);
	void HideButton(BOOL flag);
	BOOL FindPrice(CString szCompare);

	BOOL IsComm( void );
	CString GetFName( CString strCode );

public:
	void SetAll2Code();
	void SetButton(CButtonST *pButton);
	void HideFutureBtns(BOOL bHide = TRUE);
	void ShowJOption(BOOL bShow = TRUE);
	void SetCodeList(int nMonth);
	void AddListCtlCode(int nItem, CString szCode, CString szName);
	void DisableSpread();
	void AdjustControls();
	void SetFutureCodes();
	void SetCommListBox( BOOL bEnable );
	CString GetString(char *pChar, int nMinLen);
	int		m_type;
	int		m_jongmuk;
protected:
	CString		m_szName;
	CString		m_szCode;
	CString		m_szBName;
	CString		m_szBCode;
	int		m_Kind;		// select kind...
	CTool		m_Tool;
	CPoint		m_point;
	int		m_nAtm;
	int		m_nCountMonth;
	int		m_nStep;
	int		m_nStep2;
	int		m_nMiniK200;	//2015.07.01 KSJ 
	BOOL		m_bKostar;
	std::unique_ptr<CCheckBox> m_pCheck;
	std::unique_ptr<CStockCheckBox> m_pStock;
	
	CArray<FCODE, FCODE>		m_arrayFcode;
	CArray<ojcode, ojcode>		m_arrayOcode;
	CArray<PCODE, PCODE>		m_arrayPcode;
	CArray<PCODE, PCODE>		m_arrayput;
	CArray<PCODE, PCODE>		m_arraycall;
	CArray<ojcode, ojcode>		m_arrayWOcode;  

	CArray<SFCODEBASE, SFCODEBASE> m_sfbase;
	CArray<SFCODEBASE, SFCODEBASE> m_sfview;
	CMap<CString,LPCSTR,SFCODEINFO,SFCODEINFO> m_mapsfinfo;
	CMap<CString,LPCSTR,SFCODE,SFCODE> m_mapsfcode;
	SFCODE	m_sfCode;

	CList<PCODE, PCODE>		m_ListPcode;
	CArray<CString, CString>	m_ArrayPrice;
	CList<CString, CString>		m_ListMonth;
	CArray<CString, CString>	m_ArrayMonth;
	HBRUSH				m_br;

	int m_nStandardNum;		//2014.08.28 KSJ 종목파일이 바뀌기전에 문제가 생기므로 월물이 몇개인지 11개인지 4개인지
	int m_nColCount;		//2014.08.28 KSJ 종목파일이 바뀌기전에 문제가 생기므로 몇개인지 23개인지 9개인지

	HBRUSH	m_hBrush;
	CString m_root, m_user, m_file;
	bool m_bMini;			//2015.09.14 KSJ 코스피미니200인지
	CString m_szMgubn;		//2016.13.18 KSJ 주식선물 마켓구분

	int m_iGubn;

	// Generated message map functions
	//{{AFX_MSG(CFuturesDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnButtonKospi200();
	afx_msg void OnButtonMiniK200();
	afx_msg void OnButtonName2();
	afx_msg void OnButtonName();
	afx_msg void OnDoubleclickedButtonMonth9();
	afx_msg void OnDoubleclickedButtonMonth6();
	afx_msg void OnDoubleclickedButtonMonth3();
	afx_msg void OnDoubleclickedButtonMonth12();
	afx_msg void OnDoubleclickedButtonMonth92();
	afx_msg void OnDoubleclickedButtonMonth62();
	afx_msg void OnDoubleclickedButtonMonth32();
	afx_msg void OnPaint();
	afx_msg void OnDblclkPcodeList();
	afx_msg void OnSelchangePcodeList();
	afx_msg void OnButtonJoption();
	afx_msg void OnClickListCode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonOpmonth1();
	afx_msg void OnButtonOpmonth2();
	afx_msg void OnButtonOpmonth3();
	afx_msg void OnButtonOpmonth4();
	afx_msg void OnPOPTIONRdo();
	afx_msg void OnButtonKostar();
	afx_msg void OnDoubleclickedButtonSpmonth3();
	afx_msg void OnDoubleclickedButtonSpmonth6();
	afx_msg void OnDoubleclickedButtonSpmonth9();
	afx_msg void OnDoubleclickedButtonSpmonth12();
	afx_msg void OnDoubleclickedButtonSpmonth32();
	afx_msg void OnDoubleclickedButtonSpmonth62();
	afx_msg void OnBtnGumri();
	afx_msg void OnBtnMoney();
	afx_msg void OnBtnComm();
	afx_msg void OnButtonMonth3();
	afx_msg void OnButtonMonth6();
	afx_msg void OnButtonMonth9();
	afx_msg void OnButtonMonth12();
	afx_msg void OnButtonSpmonth3();
	afx_msg void OnButtonSpmonth6();
	afx_msg void OnButtonSpmonth9();
	afx_msg void OnButtonMonth32();
	afx_msg void OnButtonMonth62();
	afx_msg void OnButtonMonth92();
	afx_msg void OnButtonMonth122();
	afx_msg void OnButtonSpmonth12();
	afx_msg void OnButtonSpmonth32();
	afx_msg void OnButtonSpmonth62();
	afx_msg void OnButtonSpmonth92();
	afx_msg void OnButtonSpmonth122();
	afx_msg void OnChangeEditSearch();
	afx_msg void OnButtonSearchPrev();
	afx_msg void OnButtonSearchNext();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonMonth123();
	afx_msg void OnButtonMonth124();
	afx_msg void OnButtonSpmonth123();
	afx_msg void OnButtonSpmonth124();
	afx_msg void OnBtnWeekop();
	afx_msg void OnBtnWeek();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


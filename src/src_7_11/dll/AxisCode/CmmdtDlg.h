#pragma once
#include "afxwin.h"
#include "fxButton.h"
#include "afxcmn.h"

#define BTNLEN 8
// CCmmdtDlg 대화 상자입니다.

class CCmmdtDlg : public CDialog
{
	DECLARE_DYNAMIC(CCmmdtDlg)

public:
	CCmmdtDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CCmmdtDlg(CPoint pt, CWnd* pParent = NULL); 
	CCmmdtDlg(int kind, int type = 0, CPoint pt = CPoint(-1, -1), CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmmdtDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CMMDT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CPoint m_Point;
	int		m_Kind;		// select kind...
	BOOL		m_bSort;
	CCheckBox*	m_pCheck;
	CString m_szName;
	CString m_szCode;
	int		m_nAtm;
	int		m_nCountMonth;
public:
	void SetDlgPos(int x, int y);
	void SetCOCodes();
	void SetCOCodeBtn();
	void SetCOCodeBtnInit();
	void GetNameNCode(int iBtn);
	void GetNameNCodeSP(int iBtn);
	void InitBtn();
	void	SetCode(CString szCode){ m_szEditCode = m_szCode = szCode; }
	void	SetName(CString szName){ m_szEditName = m_szName = szName; }
	CString GetName() { return m_szName; }
	CString GetCode() { return m_szCode; }
	int		GetKind(){ return m_Kind; }
	int		GetCountMonth(){ return m_nCountMonth; }
	void	OnButtonCommdt();
	void	Sort();
	CArray<CString, CString>* GetArray();
public:
	CfxButton* m_AryBtn[BTNLEN];
	CfxButton m_BtnOne;
	CfxButton m_BtnTwo;
	CfxButton m_BtnThree;
	CfxButton m_BtnFour;
	CfxButton m_BtnFive;
	CfxButton m_BtnSix;
	CfxButton m_BtnSeven;
	CfxButton m_BtnEight;
	CListBox m_ListCOCode;

	afx_msg void OnLbnSelchangeCocodeList();
	void InitCtrl();
	CSkinListCtrl m_ListCtlCode;
	afx_msg void OnDestroy();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonOne();
	afx_msg void OnBnClickedButtonTwo();
	afx_msg void OnBnClickedButtonThree();
	afx_msg void OnBnClickedButtonFour();
	afx_msg void OnBnClickedButtonFive();
	afx_msg void OnBnClickedButtonSix();
	afx_msg void OnBnClickedButtonSeven();
	afx_msg void OnBnClickedButtonEight();
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	CString m_szEditCode;
	CString m_szEditName;

	CfxButton* m_AryBtnSP[BTNLEN];
	CfxButton m_BtnOneSP;
	CfxButton m_BtnTwoSP;
	CfxButton m_BtnThreeSP;
	CfxButton m_BtnFourSP;
	CfxButton m_BtnFiveSP;
	CfxButton m_BtnSixSP;
	CfxButton m_BtnSevenSP;
	CfxButton m_BtnEightSP;
	afx_msg void OnBnClickedButtonOnesp();
	afx_msg void OnBnClickedButtonTwosp();
	afx_msg void OnBnClickedButtonThreesp();
	afx_msg void OnBnClickedButtonFoursp();
	afx_msg void OnBnClickedButtonFivesp();
	afx_msg void OnBnClickedButtonSixsp();
	afx_msg void OnBnClickedButtonSevensp();
	afx_msg void OnBnClickedButtonEightsp();
};
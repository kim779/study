#if !defined(AFX_SYMBOLDLG_H__480230D2_CFCF_4143_BDA7_4382C238EBA8__INCLUDED_)
#define AFX_SYMBOLDLG_H__480230D2_CFCF_4143_BDA7_4382C238EBA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SymbolDlg.h : header file
//
//bitmapbutton
#include "DynamicBitmapLoad.h"
#include "DynamicBitmapLoadButton.h"

#include "PreView.h"
#include "ColorButton.h"

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSymbolDlg dialog

class CSymbolData;
class CMainSymbolImplementation;
class CSymbolDlg : public CDialog
{
// Construction
public:
	CSymbolDlg(CMainSymbolImplementation* pMain, CSymbolData* pSymbolData, CWnd* pParent = NULL);   // standard constructor

	void SetData(CSymbolData* pSymbolData);
	bool IsSymbolDlgClose();
// Dialog Data
	//{{AFX_DATA(CSymbolDlg)
	enum { IDD = IDD_DLG_SYMBOL };
	CDynamicBitmapLoadButton	m_Char_1_Bt;
	CDynamicBitmapLoadButton	m_Char_2_Bt;
	CDynamicBitmapLoadButton	m_Char_3_Bt;
	CDynamicBitmapLoadButton	m_Char_4_Bt;
	CDynamicBitmapLoadButton	m_Char_5_Bt;
	CDynamicBitmapLoadButton	m_Char_6_Bt;
	CDynamicBitmapLoadButton	m_Text_H_Bt;
	CDynamicBitmapLoadButton	m_Text_G_Bt;
	CDynamicBitmapLoadButton	m_Text_F_Bt;
	CDynamicBitmapLoadButton	m_Text_E_Bt;
	CDynamicBitmapLoadButton	m_Text_D_Bt;
	CDynamicBitmapLoadButton	m_Text_C_Bt;
	CDynamicBitmapLoadButton	m_Text_B_Bt;
	CDynamicBitmapLoadButton	m_Text_A_Bt;
	CDynamicBitmapLoadButton	m_NumTwo_Bt;
	CDynamicBitmapLoadButton	m_NumThree_Bt;
	CDynamicBitmapLoadButton	m_NumOne_Bt;
	CDynamicBitmapLoadButton	m_NumFour_Bt;
	CDynamicBitmapLoadButton	m_NumFive_Bt;
	CDynamicBitmapLoadButton	m_NumSix_Bt;
	CDynamicBitmapLoadButton	m_NumSeven_Bt;
	CDynamicBitmapLoadButton	m_NumEight_Bt;
	CDynamicBitmapLoadButton	m_NumNine_Bt;
	CDynamicBitmapLoadButton	m_NumTen_Bt;
	CDynamicBitmapLoadButton	m_ArrowUp_Bt;
	CDynamicBitmapLoadButton	m_ArrowRightTop_Bt;
	CDynamicBitmapLoadButton	m_ArrowRightBottom_Bt;
	CDynamicBitmapLoadButton	m_ArrowLeftBottom_Bt;
	CDynamicBitmapLoadButton	m_ArrowLeftTop_Bt;
	CDynamicBitmapLoadButton	m_ArrowDown_Bt;
	CPreView	m_PreView_Static;
	CColorButton	m_Color_Bt;
	CDynamicBitmapLoadButton	m_ArrowRight_Bt;
	CDynamicBitmapLoadButton	m_ArrowLeft_Bt;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSymbolDlg)
	afx_msg void OnArrowLeft();
	afx_msg void OnArrowRight();
	afx_msg void OnClose();
	afx_msg void OnArrowLefttop();
	afx_msg void OnArrowDown();
	afx_msg void OnArrowLeftbottom();
	afx_msg void OnArrowRightbottom();
	afx_msg void OnArrowRighttop();
	afx_msg void OnArrowUp();
	afx_msg void OnNumFive();
	afx_msg void OnNumFour();
	afx_msg void OnNumOne();
	afx_msg void OnNumThree();
	afx_msg void OnNumTwo();
	afx_msg void OnTextA();
	afx_msg void OnTextB();
	afx_msg void OnTextC();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSymbolSize();
	afx_msg void OnColor();
	afx_msg void OnFont();
	afx_msg void OnNumSix();
	afx_msg void OnNumTen();
	afx_msg void OnNumSeven();
	afx_msg void OnNumEight();
	afx_msg void OnNumNine();
	afx_msg void OnTextD();
	afx_msg void OnTextE();
	afx_msg void OnTextF();
	afx_msg void OnTextG();
	afx_msg void OnTextH();
	afx_msg void OnTextSpecial1();
	afx_msg void OnTextSpecial2();
	afx_msg void OnTextSpecial3();
	afx_msg void OnTextSpecial4();
	afx_msg void OnTextSpecial5();
	afx_msg void OnTextSpecial6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_bCloseDlg;
	//모든 data의 정보를 관리한다.
	CSymbolData* m_pSymbolData;
	//dlg닫을때..main의 Tool에 Null값을 적용하기 위해서이다.
	CMainSymbolImplementation* m_pMainSymbol;

	void Initial();
	void Initial_View();
	//toggle
	void SetToggle(CWnd* pFocuswnd, CDynamicBitmapLoadButton* pCurrentWnd);
	//선택된 버튼의 type을 Set한다.
	CDynamicBitmapLoadButton* GetButtonType(const CSymbolBaseData::TOOLTYPE bulettype);
	void SetBuletType(const CSymbolBaseData::TOOLTYPE bulettype, CDynamicBitmapLoadButton* pcurrentButton);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLDLG_H__480230D2_CFCF_4143_BDA7_4382C238EBA8__INCLUDED_)

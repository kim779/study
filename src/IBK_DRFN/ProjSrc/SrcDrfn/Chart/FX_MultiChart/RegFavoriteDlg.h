#if !defined(AFX_REGFAVORITEDLG_H__083CA8EE_272A_419D_A0F4_4B4C73408A80__INCLUDED_)
#define AFX_REGFAVORITEDLG_H__083CA8EE_272A_419D_A0F4_4B4C73408A80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegFavoriteDlg.h : header file
//
#include "RscDlg.h"
#include "Resource.h"
#include "./define/ChartMsgDef.h"

/////////////////////////////////////////////////////////////////////////////
// CRegFavoriteDlg dialog
class CFavoriteThingsMng;
class CRegFavoriteDlg : public CRscDlg
{
// Construction
public:
	BOOL m_bChange ; //확인 버튼 클릭시 아무런 메세지 없이 종료할 경우와 
	//지정 폴더가 없다는 메세지를 띄울 경우를 구분하기 위해
	//Flag형태로 사용한다.

	CRegFavoriteDlg(CWnd* pParent = NULL);   // standard constructor
	void SetFavoriteThingsMng(CFavoriteThingsMng *pMng,long pFvrData) { m_pFavoriteMng = pMng; m_pFvrData = (stFvrData*)pFvrData; }
	CPoint *m_pPtTopLeftOfDlg;
// Dialog Data
	//{{AFX_DATA(CRegFavoriteDlg)
	enum { IDD = IDD_DLGREGFAVORITE };
	CListBox	m_listGrp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegFavoriteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	CFavoriteThingsMng *m_pFavoriteMng;
	stFvrData  *m_pFvrData;
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegFavoriteDlg)
	afx_msg void OnBtnewfolder();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtdelfolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGFAVORITEDLG_H__083CA8EE_272A_419D_A0F4_4B4C73408A80__INCLUDED_)

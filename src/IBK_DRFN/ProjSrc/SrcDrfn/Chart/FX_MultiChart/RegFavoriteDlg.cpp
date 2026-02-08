// RegFavoriteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegFavoriteDlg.h"
#include "NewGrpFavoriteDlg.h"
#include "FavoriteThingsMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegFavoriteDlg dialog


CRegFavoriteDlg::CRegFavoriteDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CRegFavoriteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegFavoriteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pFavoriteMng = NULL;
	m_pFvrData = NULL;
	m_pPtTopLeftOfDlg = NULL;
}


void CRegFavoriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegFavoriteDlg)
	DDX_Control(pDX, IDC_LISTGRP, m_listGrp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegFavoriteDlg, CRscDlg)
	//{{AFX_MSG_MAP(CRegFavoriteDlg)
	ON_BN_CLICKED(ID_BTNEWFOLDER, OnBtnewfolder)
	ON_BN_CLICKED(ID_BTDELFOLDER, OnBtdelfolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegFavoriteDlg message handlers

void CRegFavoriteDlg::OnBtnewfolder() 
{
	// TODO: Add your control notification handler code here
	CNewGrpFavoriteDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		if(m_pFavoriteMng->NewGrp(dlg.m_strNewName))
		{
			m_listGrp.AddString(dlg.m_strNewName);
			m_listGrp.SelectString(0,dlg.m_strNewName);
			GetDlgItem(IDOK)->SetFocus();
			m_bChange = TRUE;
		}
	}
}

void CRegFavoriteDlg::OnOK() 
{
	// TODO: Add extra validation here
	int nCurSel = m_listGrp.GetCurSel();
	if(nCurSel!=-1)
	{
		CString strGrpName;
		m_listGrp.GetText(nCurSel,strGrpName);	
		
		if(m_pFavoriteMng)
		{			
			m_pFavoriteMng->SaveItemInfo(m_pFvrData,strGrpName);
		}
	}	
	else
	{
		if(!m_bChange)
		{
			//============================================================================
			// 2005. 08. 11 added by sy, nam 
			// 선택된 폴더가 없으면 경고 메세지를 띄운다.		
			MessageBox("지정된 폴더가 없습니다", "오류", MB_OK | MB_ICONERROR);
			return;		
			//============================================================================
		}
	}
	CRscDlg::OnOK();
}

void CRegFavoriteDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CRscDlg::OnCancel();
}

BOOL CRegFavoriteDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	m_bChange = FALSE;
	
	// TODO: Add extra initialization here
	if(m_pFavoriteMng)
	{
		CString strGrpName;
		m_listGrp.ResetContent();		
		m_pFavoriteMng->LoadGrpInfo();
		long lSize = m_pFavoriteMng->m_strArrayGrp.GetSize();
		for(long lIndex=0;lIndex<lSize;lIndex++)
		{
			strGrpName = m_pFavoriteMng->m_strArrayGrp.GetAt(lIndex);
			m_listGrp.AddString(strGrpName);
		}	
		m_listGrp.SetCurSel(0);
	}
	if(m_pPtTopLeftOfDlg)
	{
		CRect rect;
		GetWindowRect(rect);
		SetWindowPos(&CWnd::wndTop,m_pPtTopLeftOfDlg->x,m_pPtTopLeftOfDlg->y-rect.Height(),0,0,SWP_NOSIZE);	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CRegFavoriteDlg::OnBtdelfolder() 
{	
	int nCurSel = m_listGrp.GetCurSel();
	if(nCurSel!=-1)
	{
		CString strGrpName;
		m_listGrp.GetText(nCurSel,strGrpName);	
		
		if(m_pFavoriteMng)
		{
			CString strMessage;
			strMessage.Format("%s 를 삭제하시겠습니까?",strGrpName);
			int nRetValue = MessageBox(strMessage,DLLTITLENAME,MB_YESNO|MB_ICONSTOP);
			if(nRetValue==IDYES)
			{
				m_bChange = TRUE;
				m_pFavoriteMng->DeleteGrp(strGrpName);
				m_listGrp.DeleteString(nCurSel);
				m_listGrp.Invalidate();
			}	
		}
	}
}

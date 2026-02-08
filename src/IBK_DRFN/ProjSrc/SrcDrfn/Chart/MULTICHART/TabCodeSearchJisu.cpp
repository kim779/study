// TabCodeSearchJisu.cpp : implementation file
//

#include "stdafx.h"
#include "multichart.h"
#include "TabCodeSearchJisu.h"
#include "./define/ChartMsgDef.h"
#include "LeftBarDlg.h"
#include "../CommonTR/Tr33103.h"
#include "StdDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchJisu dialog


CTabCodeSearchJisu::CTabCodeSearchJisu(CWnd* pParent /*=NULL*/)
	: CTabDlg(CTabCodeSearchJisu::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabCodeSearchJisu)
	//}}AFX_DATA_INIT
	m_bInsertingInListCode = FALSE;
	m_bLBtnDown = FALSE;
	m_bDragNDrop = FALSE;
//	m_bCheckError = FALSE;
	m_nPosGroup = -1;
	m_lTitleCode = -1;
}


void CTabCodeSearchJisu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabCodeSearchJisu)
		// NOTE: the ClassWizard will add DDX and DDV calls here
//DX_Control(pDX, IDC_BTEDITGROUP, m_btnEditGrp);
//DX_Control(pDX, IDC_BTAPPLY, m_btnApply);
	DDX_Control(pDX, IDC_LISTCODE, m_listCode);
	DDX_Control(pDX, IDC_CHECK_KOSPI, m_BtnKospi);
	DDX_Control(pDX, IDC_CHECK_K200, m_BtnK200);
	DDX_Control(pDX, IDC_CHECK_KOSDAQ, m_BtnKosdaq);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabCodeSearchJisu, CDialog)
	//{{AFX_MSG_MAP(CTabCodeSearchJisu)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCODE, OnDblclkListcode)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LISTCODE, OnBegindragListcode)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCODE, OnItemchangedListcode)
	ON_NOTIFY(NM_CLICK, IDC_LISTCODE, OnClickListcode)
	ON_NOTIFY(NM_RCLICK, IDC_LISTCODE, OnRclickListcode)
	ON_BN_CLICKED(IDC_CHECK_KOSPI, OnChkKospi)
	ON_BN_CLICKED(IDC_CHECK_KOSDAQ, OnChkKosdaq)
	ON_BN_CLICKED(IDC_CHECK_K200, OnChkK200)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchJisu message handlers
void CTabCodeSearchJisu::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);	
	// TODO: Add your message handler code here
	RecalcLayout();	
}

BOOL CTabCodeSearchJisu::OnInitDialog() 
{
	CTabDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_listCode.Init(m_pParentDlg,0);
	m_listCode.SetColumnWidth(0, 0);
	m_listCode.SetColumnWidth(1, 86);
	m_listCode.SetColumnWidth(2, 55);
	// 업종에 전일비 추가 - ojtaso (20080107)
	m_listCode.SetColumnWidth(3, 47);
	m_JongStatusInTab.Create(CJongStatusInTab::IDD,this);
	m_JongStatusInTab.ShowWindow(SW_SHOW);	

	m_listCode.RemoveAll();
	m_JongStatusInTab.InitStatus();

	m_BtnKospi.FP_vSetImage( m_pIL_W52H19, 0, 1, FALSE, "", TRUE);
	m_BtnK200.FP_vSetImage( m_pIL_W52H19, 2, 3, FALSE, "", TRUE);
	m_BtnKosdaq.FP_vSetImage( m_pIL_W52H19, 4, 5, FALSE, "", TRUE);
	
	m_BtnKospi.SetCheck(TRUE);	
	OnChkKospi();

	SetConfig();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


CRect CTabCodeSearchJisu::GetRectOfCtrl(INT nID,BOOL bInit/*=FALSE*/)
{
	CRect rect;
	GetClientRect(rect);
	int nWidth = rect.Width();
	rect.DeflateRect(GAP_TABDLG,GAP_TABDLG,GAP_TABDLG,GAP_TABDLG);
	switch(nID)
	{
	case IDC_LISTCODE:
		rect.top = rect.top + HEIGHTOFCOMBO + GAP_TABDLG;
		rect.bottom = rect.bottom - HEIGHTOFJONGSTATUS - GAP_TABDLG - GAP_TABDLG;// - HEIGHTOFBUTTON;
		break;		
	case CJongStatusInTab::IDD:
		rect.top = rect.bottom - GAP_TABDLG - HEIGHTOFJONGSTATUS;// - HEIGHTOFBUTTON;
		rect.bottom = rect.bottom - GAP_TABDLG;// - HEIGHTOFBUTTON;
		break;
	default:
		break;
	}
	return rect;
}

void CTabCodeSearchJisu::RecalcLayout()
{	
	RecalcLayoutCtrl(m_listCode);
	RecalcLayoutCtrl(m_JongStatusInTab,CJongStatusInTab::IDD);	
}

void CTabCodeSearchJisu::SetRealPriceData(char *pData, int nSize)
{
//	char szTemp0[16];
//	char szTemp1[16];
//	char szTemp2[16];
//	char szTemp3[16];
//	char szTemp4[16];
//	char chSign;
//	CString strPriceOld;
//	CString strChangeOld;
//	CString strChgrateOld;
//	CString strVolumeOld;
//	char	chSignOld;
//	double dChgRateOld = 0;
//	double dChgRateNew = 0;
//	STPriceData* Op = (STPriceData*) pData;
//	MEMCPYWITHNULL(szTemp0,Op->shcode);		// 종목코드
//	MEMCPYWITHNULL(szTemp1,Op->price);		// 현재가
//	MEMCPYWITHNULL(szTemp2,Op->change);		// 대비
//	MEMCPYWITHNULL(szTemp3,Op->chgrate);	// 대비율
//	MEMCPYWITHNULL(szTemp4,Op->volume);	// 거래량	
//	chSign = Op->sign[0];
//	int nIndex = m_listCode.FindCode(szTemp0);
//	if(nIndex>=0)
//	{
//		m_listCode.GetCodeData(nIndex,strPriceOld,strChangeOld,strChgrateOld,strVolumeOld,chSignOld);
////		m_listCode.UpdateCodeData(nIndex,szTemp0,szTemp1,szTemp2,szTemp3,szTemp4,chSign);		
//		dChgRateOld = atof(strChgrateOld);
//		dChgRateNew = atof(szTemp3);
//		m_JongStatusInTab.UpdateStatus(chSign,chSignOld,dChgRateNew-dChgRateOld);
//	}
	
}


/*
LRESULT CTabCodeSearchJisu::OnIntRequest( UINT wParam, LONG lParam )
{

	if(GetSafeHwnd() == (HWND)wParam)
	{
		CPtrArray *ptrArrayIntGroupSt = (CPtrArray *)((INT_TOTAL*)lParam)->Idata;
		if(!ptrArrayIntGroupSt) 
		{
			m_cbFavorGrp.ResetContent();
			m_listCode.RemoveAll();
			return 1L;
		}
		DWORD wSize = ptrArrayIntGroupSt->GetSize();
		INT_GROUP_ST *pIntGroupSt = NULL;
		INT_ITEM_ST *pIntItemSt = NULL;
		int nIndex= 0;
		long lTitleCode = 0;
		
		char szBuffer[64];
		if(m_nPosGroup==-1)
		{
			m_cbFavorGrp.ResetContent();
			CString strTitle;
			for (DWORD wIndex = 0; wIndex < wSize; wIndex++)
			{
				pIntGroupSt = (INT_GROUP_ST *)ptrArrayIntGroupSt->GetAt(wIndex);
				memcpy(szBuffer,pIntGroupSt->Ino,sizeof(pIntGroupSt->Ino));
				szBuffer[sizeof(pIntGroupSt->Ino)] = NULL;
				lTitleCode = atol(szBuffer);
				memcpy(szBuffer,pIntGroupSt->Ititle,sizeof(pIntGroupSt->Ititle));
				szBuffer[sizeof(pIntGroupSt->Ititle)] = NULL;
				strTitle = szBuffer;
				strTitle.TrimLeft();
				strTitle.TrimRight();				
				nIndex = m_cbFavorGrp.AddString(strTitle);
				m_cbFavorGrp.SetItemData(nIndex,lTitleCode);
			}
			if(wSize)
			{
				//m_nPosGroup = m_cbFavorGrp.FindString(-1,m_strTitle);
				int nCbCnt = m_cbFavorGrp.GetCount();
				for(int i=0;i<nCbCnt;i++)
				{
					lTitleCode = m_cbFavorGrp.GetItemData(i);
					if(lTitleCode==m_lTitleCode)
					{
						m_nPosGroup = i;
						break;
					}					
				}

				if(m_nPosGroup==-1)
				{
					if(m_cbFavorGrp.GetCount()>0)
					{
						m_nPosGroup = 0;
						m_lTitleCode = m_cbFavorGrp.GetItemData(m_nPosGroup);
//						m_cbFavorGrp.GetLBText(m_nPosGroup,m_strTitle);
//						m_strTitle.TrimRight();
					}
					else
					{
						m_listCode.RemoveAll();
						return 0L;
					}						
				}
				m_cbFavorGrp.SetCurSel(m_nPosGroup);
				IntRequest();
			}
		}
		else
		{

			char Buffer[4096];
			char szTemp[16];
			char  jg_code[15];   
			long lOffsetOfBuffer = 0;
			TR_05010_I stTr05010i;
			TR_05010_IS stTr05010is;
			
			//pIntGroupSt = (INT_GROUP_ST *)ptrArrayIntGroupSt->GetAt(m_nPosGroup);
			BOOL bFindGroupSt = FALSE;
			CString strTitleInGroupSt;
			long lSizeOfGroupSt = ptrArrayIntGroupSt->GetSize();
			for(long lPos=0;lPos<lSizeOfGroupSt;lPos++)
			{
				pIntGroupSt = (INT_GROUP_ST *)ptrArrayIntGroupSt->GetAt(lPos);
				memcpy(szBuffer,pIntGroupSt->Ino,sizeof(pIntGroupSt->Ino));
				szBuffer[sizeof(pIntGroupSt->Ino)] = NULL;
				lTitleCode = atol(szBuffer);
				strTitleInGroupSt = pIntGroupSt->Ititle;
				strTitleInGroupSt.TrimRight();
//				if(strTitleInGroupSt==m_strTitle)
				if(lTitleCode==m_lTitleCode)
				{
					bFindGroupSt = TRUE;
					break;
				}
			}
			if(!bFindGroupSt) 
			{
//				CString strMessage;
//				strMessage = m_strTitle + "를 찾을수 없습니다.";
//				MessageBox(strMessage,DLLTITLENAME);
				m_bCheckError = TRUE;
				return 1L;
			}
			CPtrArray *ptrArrayIntItemSt = (CPtrArray *)(pIntGroupSt->Idata);
			DWORD wSizeItems = ptrArrayIntItemSt->GetSize();
			sprintf(szTemp,"%4d",wSizeItems);
			m_listCode.RemoveAll();

			MEMCPYBYTARSIZE(stTr05010i.winid,"0000");
			MEMCPYBYTARSIZE(stTr05010i.jgcnt,szTemp);

			MEMCPYBYSRCSIZE(Buffer,stTr05010i,lOffsetOfBuffer);
			for (DWORD wIndex = 0; wIndex < wSizeItems; wIndex++)
			{
				pIntItemSt = (INT_ITEM_ST *)ptrArrayIntItemSt->GetAt(wIndex);
			
				memset(jg_code,0x20,sizeof(jg_code));
				memcpy(jg_code,pIntItemSt->Icode,sizeof(pIntItemSt->Icode)-1);
				stTr05010is.jg_tp[0] = pIntItemSt->Igubun[0];
				MEMCPYBYTARSIZE(stTr05010is.jg_code,jg_code);
				MEMCPYBYSRCSIZE(Buffer,stTr05010is,lOffsetOfBuffer);
			}
			RequestCurrentPrice(Buffer,lOffsetOfBuffer);
		}
	}

	return 1L;
}
*/

void CTabCodeSearchJisu::RequestCurrentPrice(LPCTSTR lpBuffer,long lLength)
{
	stSend2Server st;
//	st.m_nType = 0;
//	st.m_pWnd = this;
//	st.m_pBuffer = (void*)lpBuffer;
//	st.m_lLength= lLength;
//	st.m_strTRCode = QUERY_STRUPLIST;
//	m_pMultiChartWnd->SendMessage(UMSG_SEND2SERVER,0,(long)&st);	
}


void CTabCodeSearchJisu::ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen)
{
	return;
//	LPTSTR pData = (LPTSTR)aTRData;
//	CString strTRCode = szTR;
//	if(strTRCode==QUERY_STRUPLIST)
//	{
//		CTR_33103_O trOutput;
//		trOutput.Convert((TR_33103_O*)aTRData);
////		TR_33103_O	stTr05010O;
////		TR_33103_G	stTr05010Os;
////		long lOffset = 0;
////		long lCnt = 0;
////		char szTemp[128];
////		char szCode[64];
////		char szName[64];
////		char chSign;
////		CString strPrice;
////		CString strChange;
////		CString strChgRate;
////		CString strVolume;
////		CString strName;
////
//		long	lUpLimit = 0;
//		long	lUp = 0;
//		long	lSteady = 0;
//		long	lDown = 0;
//		long	lDownLimit = 0;
//		double dDaebiRateTotal = 0;
////
////		MEMCPYBYTARSIZE2(stTr05010O,pData,lOffset);
////		MEMCPYWITHNULL(szTemp,stTr05010O.cnt);
////		lCnt = atol(szTemp);
//		m_bInsertingInListCode = TRUE;
//		m_listCode.m_nIndex = 0;
//		for(long lPos=0;lPos<trOutput.m_ngr_cnt;lPos++)
//		{
//			POSITION pos = trOutput.m_DataList.FindIndex(lPos);
//			CTR_33103_G* trItem = trOutput.m_DataList.GetAt(pos);
//			
//			switch(trItem->m_szsign[0])
//			{
//			case UP_LIMIT:	// UpLimit
//				lUpLimit++;
//				break;
//			case UP:	// Up
//				lUp++;
//				break;
//			case EQUAL:	// Steady
//				lSteady++;
//				break;
//			case DOWN:	// Down
//				lDown++;
//				//				strChange = '-' + strChange;
//				//				strChgRate = '-' + strChgRate;
//				break;
//			case DOWN_LIMIT:	// DownLimit
//				lDownLimit++;
//				//				strChange = '-' + strChange;
//				//				strChgRate = '-' + strChgRate;
//				break;	
//				//			case '6':
//				//			case '7':
//				//			case '8':
//				//			case '9':
//				//			case '0':
//				break;
//			}
//			//dDaebiRateTotal += (double)atof(strChgRate);
//			m_listCode.InsertCodeData(
//				trItem->m_szcode,
//				trItem->m_szname,
//				trItem->m_szjisu,
//				// 업종에 전일비 추가 - ojtaso (20080107)
//				trItem->m_szchange,
//				"",
//				"",
//				trItem->m_szsign[0]);
//		}		
//		m_listCode.SetItemCountEx(trOutput.m_ngr_cnt);
//		m_listCode.Invalidate();
//		m_bInsertingInListCode = FALSE;
//		m_JongStatusInTab.SetStatus(lUpLimit,lUp,lSteady,lDown,lDownLimit,trOutput.m_ngr_cnt,dDaebiRateTotal/100);
//
//		CStringList listStrJongmokCodeChecked;
//		CStringList listStrJongmokCodeCheckedFromCodeSrc;
//		CStringList listStrJongmokCodeHighlighted;
//		m_pChartMng->SendMessage(UMSG_GETJCODECHECKEDFROMCHART
//			,(WPARAM)&listStrJongmokCodeChecked
//			,(LPARAM)&listStrJongmokCodeHighlighted);
//		CWnd *pWnd = m_pParentDlg->m_tabctrl.GetWndCodeSearch();
//		if(pWnd)
//		{
//			pWnd->SendMessage(UMSG_GETJCODECHECKEDFROMCODESRC
//				,(WPARAM)&listStrJongmokCodeCheckedFromCodeSrc);
//		}
//		m_listCode.CheckJongmokCode(&listStrJongmokCodeChecked,&listStrJongmokCodeHighlighted);
//		m_listCode.CheckJongmokCode(&listStrJongmokCodeCheckedFromCodeSrc);
//
//		trOutput.ClearList();
//	}
}

LRESULT CTabCodeSearchJisu::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case UMSG_RECEIVEFROMSERVER:
		{
			stReceiveFromServer* pStR = (stReceiveFromServer*)lParam;
			ReceiveData(pStR->m_strTRCode,pStR->m_pBuffer,pStR->m_dwLength);
		}
		break;
	case UMSG_SENDJCODECHECKED:
		{
			m_listCode.CheckJongmokCode(wParam,(LPCTSTR)lParam);
		}
		break;
	case UMSG_GETJCODECHECKEDFROMLEFTBAR:
		{
			BOOL* pbCheck = (BOOL*)wParam;
			*pbCheck = m_listCode.GetCheckByJongmokCode((LPCTSTR)lParam);
		}
		break;
	case UMSG_SENDJCODSYNHIGHLIGHTED:
		{
			m_listCode.UnHighlightByJongmokCode((LPCTSTR)wParam);
		}
		break;
	}
	return CTabDlg::WindowProc(message, wParam, lParam);
}

void CTabCodeSearchJisu::OnBegindragListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_bLBtnDown = TRUE;
	SetCapture();		
	*pResult = 0;
}

void CTabCodeSearchJisu::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bLBtnDown)
	{
		// 드래그임을 표시
		m_bDragNDrop = TRUE;
		CPoint ptDrag(point);
		ClientToScreen(&ptDrag);
//		m_pMultiChartWnd->GetParent()->GetParent()->GetParent()->SendMessage(RMSG_DRAGSTART, (WPARAM)&ptDrag, (LPARAM)m_hWnd);
	}
	
	CTabDlg::OnMouseMove(nFlags, point);
}

void CTabCodeSearchJisu::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bLBtnDown)
	{
		m_bLBtnDown = FALSE;
		ReleaseCapture();

		if(m_bDragNDrop)	// 드래그 중이었으면
		{
			CPoint ptDrag(point);
			ClientToScreen(&ptDrag);
			CRect rectChartMng;
			m_pParentDlg->GetWindowRect(rectChartMng);
			if(!rectChartMng.PtInRect(ptDrag))
			{
				// 데이터 생성
//				CDragNDrop* pDnD = new CDragNDrop;				
//				UINT nCnt = m_listCode.GetSelectedCount();			
//				if(nCnt>1)
//				{
//					pDnD->SetData(&ptDrag, m_hWnd, _T("코드-드래그리스트"), (LPTSTR)(LPCTSTR)m_listCode.GetSelectedCodes());
//				}
//				else
//				{
//					CString strPacketShared;
//					strPacketShared = m_listCode.GetCodeSelected();
//					strPacketShared += ';';
//					strPacketShared += m_listCode.GetTimeSelected();
//					strPacketShared += ';';
//					pDnD->SetData(&ptDrag, m_hWnd, _T("코드-드래그"), (LPTSTR)(LPCTSTR)strPacketShared);
//				}
//				m_pMultiChartWnd->GetParent()->GetParent()->GetParent()->PostMessage(RMSG_DRAGEND, 0, (LPARAM)pDnD);
			}
		}
		m_bDragNDrop = FALSE;
	}
	
	CTabDlg::OnLButtonUp(nFlags, point);
}

void CTabCodeSearchJisu::SetConfig(BOOL bLoad /*= TRUE*/)
{
//	CString strTemp;
//	char szTemp[64];
//	if(bLoad)
//	{
////		::GetPrivateProfileString("TabFavoriteGrp","HeaderTitle","",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
////		m_strTitle = szTemp;
//		::GetPrivateProfileString("TabFavoriteGrp","HeaderTitleCode","0",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
//		m_lTitleCode = atol(szTemp);
//	}
//	else
//	{		
////		::WritePrivateProfileString("TabFavoriteGrp","HeaderTitle",m_strTitle,m_pParentDlg->m_strConfig);
//		strTemp.Format("%d",m_lTitleCode);
//		::WritePrivateProfileString("TabFavoriteGrp","HeaderTitleCode",strTemp,m_pParentDlg->m_strConfig);
//	}
}

void CTabCodeSearchJisu::OnDestroy() 
{
	CTabDlg::OnDestroy();
	
	// TODO: Add your message handler code here
	SetConfig(FALSE);
	
	
}

void CTabCodeSearchJisu::OnDblclkListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iSubItem>0)
	{
		BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
		if(bInsertMode)
		{
			STINPUTCODEDATA stInputCodeData;
			stInputCodeData.m_strCode = m_listCode.GetCodeSelected();
			m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
		}	
	}
	*pResult = 0;
}


void CTabCodeSearchJisu::OnItemchangedListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
//	BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
//	if(!bInsertMode&&!m_bInsertingInListCode&&pNMListView->uNewState==3)
//	{
//		m_pChartMng->SendMessage(UMSG_INPUTCODE,NULL,(LPARAM)(LPCTSTR)m_listCode.GetCodeSelected(pNMListView->iItem));
//	}		
	*pResult = 0;
}

void CTabCodeSearchJisu::OnClickListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iSubItem==0)
	{
		m_listCode.UpdateCheck(pNMListView->iItem,m_pChartMng);
		CWnd *pWnd = m_pParentDlg->m_tabctrl.GetWndCodeSearch();
		if(pWnd)
		{
			m_listCode.UpdateCheck(pNMListView->iItem,pWnd);
		}
	}
	else
	{
		BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
		if(!bInsertMode&&!m_bInsertingInListCode)
		{
			CString strCodeSelected = m_listCode.GetCodeSelected();
			if(!strCodeSelected.IsEmpty())
			{
				STINPUTCODEDATA stInputCodeData;
				stInputCodeData.m_strCode = m_listCode.GetCodeSelected();
				m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
			}
		}		
	}
	*pResult = 0;
}

void CTabCodeSearchJisu::OnRclickListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
//	UINT nCnt = m_listCode.GetSelectedCount();			
//	if(nCnt==1)
//	{
//		STSHAREDDATA stInfo;
//		stInfo.m_szKey = "코드-코코2";
//		stInfo.m_szValue = m_listCode.GetCodeSelected();
//		stInfo.m_szValue.TrimRight(';');
//		((IMainDllObMain*)g_pMainDll)->m_pMainFrame->SendMessage(RMSG_CREATEOPENSHAREDATA,0, (LPARAM)&stInfo);		
//	}
	
	*pResult = 0;
}

void CTabCodeSearchJisu::OnChkKospi() 
{
	// TODO: Add your control notification handler code here
	m_BtnKosdaq.SetCheck(FALSE);	
	m_BtnK200.SetCheck(FALSE);	
	m_listCode.RemoveAll();
	((CStdDialog*)m_pMultiChartWnd)->ChangeUpjongMode(100);	//008	KSPUPJONG

	RequestCurrentPrice("    1", 5);
	
}

void CTabCodeSearchJisu::OnChkKosdaq() 
{
	// TODO: Add your control notification handler code here
	m_BtnKospi.SetCheck(FALSE);	
	m_BtnK200.SetCheck(FALSE);	
	m_listCode.RemoveAll();
	((CStdDialog*)m_pMultiChartWnd)->ChangeUpjongMode(101);	//008	KSQUPJONG

	RequestCurrentPrice("    2", 5);
}

void CTabCodeSearchJisu::OnChkK200() 
{
	// TODO: Add your control notification handler code here
	m_BtnKospi.SetCheck(FALSE);	
	m_BtnKosdaq.SetCheck(FALSE);
	m_listCode.RemoveAll();
	((CStdDialog*)m_pMultiChartWnd)->ChangeUpjongMode(102);	//008	KSPUPJONG

	RequestCurrentPrice("    3", 5);
}

HBRUSH CTabCodeSearchJisu::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CTabDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		{
			pDC->SetBkColor(m_crBk);
			return m_brBkColor;
		}		
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
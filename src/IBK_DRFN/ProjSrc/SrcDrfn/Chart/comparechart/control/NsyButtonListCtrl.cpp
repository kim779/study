// MyButtonListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "NsyButtonListCtrl.h"
#include "../StdDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNsyButtonListCtrl

CNsyButtonListCtrl::CNsyButtonListCtrl()
{	
	m_szImageDir = _T("");
	m_FontOfFont.CreatePointFont(9, "굴림");

	m_bDirectRemove = FALSE;
}

CNsyButtonListCtrl::~CNsyButtonListCtrl()
{
}


BEGIN_MESSAGE_MAP(CNsyButtonListCtrl, CXListCtrl)
	//{{AFX_MSG_MAP(CNsyButtonListCtrl)	
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	//ON_WM_MEASUREITEM()
	//ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNsyButtonListCtrl message handlers
void CNsyButtonListCtrl::SetItemButton(int nItem, CString strJongName, int nJongCode)
{
	CRect subRect;	
//	GetSubItemRect(nItem, 1,LVIR_BOUNDS, subRect);
	CString strImageUp, strImageDown, strImageOver;

// 	if(m_bShowBlank)
// 	{
// 		strImageUp		= m_szImageDir + "\\DEL_BTN_BLANK.bmp";
// 		strImageDown	= strImageUp;
// 		strImageOver	= strImageUp;
// 	}
// 	else
	{
		strImageUp		= m_szImageDir + "\\del_btn_up.bmp";
		strImageDown	= m_szImageDir + "\\del_btn_down.bmp";
		strImageOver	= m_szImageDir + "\\btn_select.bmp";
	}

//	CxSkinButton* pButton = NULL;
//	pButton = new CxSkinButton;
//
//	m_ptrArrayBtnWnd.Add(pButton);
//	pButton->Create(_T(""), WS_VISIBLE | WS_CHILD,  subRect, this, nJongCode + nItem);
//	pButton->SetSkin(strImageUp,strImageDown,strImageOver,"","","",0,0,0);
//	pButton->SetWindowText(strJongName);
	

	GetSubItemRect(nItem, 1,LVIR_BOUNDS, subRect);	

	BTNINFO* pBtnInfo = NULL;
	pBtnInfo = new BTNINFO;
	if(pBtnInfo != NULL)
	{
		pBtnInfo->nIndex = nItem;		
		pBtnInfo->xSkinButton.Create(_T(""), WS_VISIBLE | WS_CHILD,  subRect, this, nJongCode + nItem);
		pBtnInfo->xSkinButton.SetSkin(strImageUp,strImageDown,strImageOver,"","","",0,0,0);
		//pBtnInfo->xSkinButton.SetWindowText(strJongName);
		m_ptrArrayBtnWnd.Add(pBtnInfo);		
	}
}








/*
void CNsyButtonListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{	
	lpMeasureItemStruct->itemHeight = 50; // 요렇게 사이즈 정의하믄 됨
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}*/

void CNsyButtonListCtrl::Init()
{
	{
		CFont* pFont = GetParent()->GetFont();
		SetFont(pFont);
	}

	GetHeaderCtrl()->EnableWindow(FALSE);
	SetExtendedStyle(LVS_EX_FULLROWSELECT);	
	SetHilightColor(RGB(0,0,0),RGB(208,221,240));

	CRect rect;
	LV_COLUMN lvcolumn;
//	HDITEM hditem;	
	
	GetWindowRect(rect);
	int nColWd =  rect.Width()	- 20;	

//	int colwidthsType[3] = {20, nColWd - 20 , 20};
//	for(int i = 0; i < 3; i++)
//	{
//		memset(&lvcolumn, 0, sizeof(lvcolumn));
//		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
//		lvcolumn.fmt =  LVCFMT_CENTER;
//		
//		if(i == 1 )lvcolumn.pszText = _T("종목명");
//		else lvcolumn.pszText = _T("");
//
//		lvcolumn.iSubItem = i;
//		lvcolumn.cx = colwidthsType[i];		
//		InsertColumn(i, &lvcolumn);
//	}	

	int colwidthsType[2] = {nColWd - 20 , 20};
	for(int i = 0; i < 2; i++)
	{
		memset(&lvcolumn, 0, sizeof(lvcolumn));
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt =  LVCFMT_CENTER;
		
		if(i == 0 )lvcolumn.pszText = _T("종목명");
		else lvcolumn.pszText = _T("");

		lvcolumn.iSubItem = i;
		lvcolumn.cx = colwidthsType[i];		
		InsertColumn(i, &lvcolumn);
	}	

	CImageList imageList; 	
	long nheight; 	
	nheight = 13; 	

	imageList.Create(1,nheight,ILC_COLOR4,1,1); 
	SetImageList(&imageList,LVSIL_SMALL); 
}

void CNsyButtonListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CXListCtrl::OnSize(nType, cx, cy);	
	if(GetSafeHwnd())
	{
		SetColumnWidth(0, cx - 20);	
		for(int i = 0; i < m_ptrArrayBtnWnd.GetSize(); i++)
		{
			BTNINFO *pBtnInfo = NULL;
			pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(i);
			if(pBtnInfo != NULL)
			{
				CRect subRect;
				if(i < GetItemCount())
				{
					GetSubItemRect(i, 1,LVIR_BOUNDS, subRect);
					pBtnInfo->xSkinButton.MoveWindow(subRect);
				}				
			}
			/*
			CxSkinButton *pButton = (CxSkinButton*)m_ptrArrayBtnWnd.GetAt(i);
			if(pButton != NULL)
			{
				//BSTR bstrStatus = ::SysAllocString( L"Some text" ); 
				CRect subRect;
				if(i < GetItemCount())
				{
					GetSubItemRect(i, 2,LVIR_BOUNDS, subRect);
					pButton->MoveWindow(subRect);
				}
			}
			*/
		}
	}
}

void CNsyButtonListCtrl::OnDestroy() 
{
	ClearArrayBtnWnd();

	
	CListCtrl::OnDestroy();
}

LRESULT CNsyButtonListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString sTemp;
	switch(message)
	{	
//		case WM_COMMAND:
//			{
//				if( BN_CLICKED == (int)HIWORD(wParam) )				
//				{
//					for(int i = 0; i < m_ptrArrayBtnWnd.GetSize(); i++)
//					{
//						BTNINFO *pBtnInfo = NULL;
//						pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(i);
//						if(pBtnInfo != NULL)
//						{
//							if( pBtnInfo->xSkinButton.GetSafeHwnd() == (HWND)lParam)
//							{
//								DeleteChartData(i);
//								
//								pBtnInfo->xSkinButton.DestroyWindow();
//								delete pBtnInfo;
//								m_ptrArrayBtnWnd.RemoveAt(i);
//								DeleteItem(i);
//								Invalidate();
//
//								// 그 아래에 위치하는 항목들의 Index와 Button들의 위치를 다 옮겨야한다.
//								for(int j = i; j < m_ptrArrayBtnWnd.GetSize(); j++)
//								{
//									BTNINFO *pBtnInfo2 = NULL;
//									pBtnInfo2 = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(j);
//									if(pBtnInfo2 != NULL)
//									{
//										if( pBtnInfo2->xSkinButton.GetSafeHwnd())
//										{											
//											pBtnInfo2->nIndex -= 1;
//											CRect subRect;
//											GetSubItemRect(pBtnInfo2->nIndex, 1,LVIR_BOUNDS, subRect);
//											pBtnInfo2->xSkinButton.MoveWindow(subRect);
//										}
//									}
//								}
//
//								Invalidate();
//								return S_OK;
//							}
//						}
//					}
//				}
//			}
//			break;
		case WM_COMMAND:
			{
				if( BN_CLICKED == (int)HIWORD(wParam) )				
				{
					int nCount = m_ptrArrayBtnWnd.GetSize();
					if(nCount==1)
					{
						Init_CompareChart();
					}
					else
					{
						for(int i = 0; i < nCount; i++)
						{
							BTNINFO *pBtnInfo = NULL;
							pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(i);
							if(pBtnInfo != NULL)
							{
								if( pBtnInfo->xSkinButton.GetSafeHwnd() == (HWND)lParam)
								{
									SendToChartRemoveGraph(i);
									return S_OK;
								}
							}
						}
					}
				}
			}
			break;
	}
	return CListCtrl::WindowProc(message, wParam, lParam);
}

void CNsyButtonListCtrl::DeleteOneItem()
{
	POSITION pos = GetFirstSelectedItemPosition();
	//선택된 항목이 없는 경우 강제로 첫번째 항목으로 선택을 시킨다.
	if(pos == NULL)
	{
		MessageBox("삭제할 항목을 선택하십시오.", "알림", MB_OK | MB_ICONERROR);
		return;
	}

	if(pos)
	{			
		//현재 마우스로 선택되어진 항목의 Zero Base 기반 Index를 가져온다.
		int i = GetNextSelectedItem(pos);
		DeleteChartData(i);


		BTNINFO *pBtnInfo = NULL;
		pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(i);
		if(pBtnInfo != NULL)
		{
			if( pBtnInfo->xSkinButton.GetSafeHwnd())
			{
//				CString sTemp;
//				sTemp.Format("pBtnInfo->nIndex : %d", pBtnInfo->nIndex);
//				AfxMessageBox(sTemp);
				
				
				pBtnInfo->xSkinButton.DestroyWindow();
				delete pBtnInfo;
				m_ptrArrayBtnWnd.RemoveAt(i);
				DeleteItem(i);
				Invalidate();

				// 그 아래에 위치하는 항목들의 Index와 Button들의 위치를 다 옮겨야한다.
				for(int j = i; j < m_ptrArrayBtnWnd.GetSize(); j++)
				{
					BTNINFO *pBtnInfo2 = NULL;
					pBtnInfo2 = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(j);
					if(pBtnInfo2 != NULL)
					{
						if( pBtnInfo2->xSkinButton.GetSafeHwnd())
						{											
							pBtnInfo2->nIndex -= 1;
							CRect subRect;
							GetSubItemRect(pBtnInfo2->nIndex, 1,LVIR_BOUNDS, subRect);
							pBtnInfo2->xSkinButton.MoveWindow(subRect);
						}
					}
				}

				Invalidate();											
			}
		}
	}
}

//리스트에서만 제거.
void CNsyButtonListCtrl::RemoveOnlyList(int nItem)
{
	if(m_bDirectRemove==FALSE)
		DeleteSelectItem(nItem);
}

void CNsyButtonListCtrl::DeleteSelectItem(int nItem)
{
//	DeleteChartData(nItem);

	BTNINFO *pBtnInfo = NULL;
	pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(nItem);
	if(pBtnInfo != NULL)
	{
		if( pBtnInfo->xSkinButton.GetSafeHwnd())
		{
			pBtnInfo->xSkinButton.DestroyWindow();
			delete pBtnInfo;
			m_ptrArrayBtnWnd.RemoveAt(nItem);
			DeleteItem(nItem);
			Invalidate();

			// 그 아래에 위치하는 항목들의 Index와 Button들의 위치를 다 옮겨야한다.
			for(int j = nItem; j < m_ptrArrayBtnWnd.GetSize(); j++)
			{
				BTNINFO *pBtnInfo2 = NULL;
				pBtnInfo2 = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(j);
				if(pBtnInfo2 != NULL)
				{
					if( pBtnInfo2->xSkinButton.GetSafeHwnd())
					{											
						pBtnInfo2->nIndex -= 1;
						CRect subRect;
						GetSubItemRect(pBtnInfo2->nIndex, 1,LVIR_BOUNDS, subRect);
						pBtnInfo2->xSkinButton.MoveWindow(subRect);
					}
				}
			}
			Invalidate();											
		}
	}
}

void CNsyButtonListCtrl::Init_CompareChart()
{
	//2005. 07. 14=====================================================
	int nTot = GetItemCount();
	for(int i=nTot-1; i >= 0; i--)
	{
		CString strCode;
		strCode = GetItemText(i, 1);
		// 1. Real 해제 및 차트 내용 삭제.
		//DeleteChartData(i);		
		SendToChartRemoveGraph(i);
	}
	//2. List 전부 삭제
	DeleteAllItems();

	//3. BtnArray 전부 삭제.
	ClearArrayBtnWnd();
	
	Invalidate();
	//4. 차트 Reset	
	((CStdDialog*)(GetParent()->GetParent()->GetParent()))->SendMessage(UMSG_RESETCHART, 0, 0);
	((CStdDialog*)(GetParent()->GetParent()->GetParent()))->SendMessage(UMSG_INIT_COMPARECHART, 0, 0);

}

void CNsyButtonListCtrl::ClearArrayBtnWnd()
{
	for(int i = 0; i < m_ptrArrayBtnWnd.GetSize(); i++)
	{
		BTNINFO *pBtnInfo = NULL;
		pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(i);
		if(pBtnInfo)
		{
			pBtnInfo->xSkinButton.DestroyWindow();
			delete pBtnInfo;
			pBtnInfo = NULL;
		}		
	}
	m_ptrArrayBtnWnd.RemoveAll();
}

void CNsyButtonListCtrl::DeleteChartData(int nItem)
{
//080604목 RemoveGraph 호출을 하는 SendToChartRemoveGraph(..)루틴을 만들면서 변경함.
// 이함수에서는 아무처리도 
return;

	if(nItem<0 || nItem>=GetItemCount()) return;

	//2005. 07. 14=====================================================
	CString strCode, strName;
	strName = GetItemText(nItem, 0);
	strCode = GetItemText(nItem, 1);
	// 1. Real 해제
	((CStdDialog*)(GetParent()->GetParent()->GetParent()))->OnCodeUnAdvise(strCode, TRUE);

	// 2. 차트에서 삭제.
	// 종목명으로 차트의 블럭이나 데이터를 삭제한다.
	// 업종명이나 선물/옵션의 종목명은 불규착하기 때문에
	// ,나 공백등을 제거하여 추가하므로 
	// CChartItemData의 SetKorName(CString strName)의 이름 추가 방법과 
	// 같은 방법으로 이름을 구성한다.
	strName.TrimLeft(); strName.TrimRight();
	strName.Replace(" ", "");
	strName.Replace(",", "");
	//((CStdDialog*)(GetParent()->GetParent()->GetParent()))->RemoveGraph_Title(-1, -1, strCode, strName, FALSE);
	((CStdDialog*)(GetParent()->GetParent()->GetParent()))->RemoveGraph(0, 0, strCode, strName);
	//=================================================================

// 	// DEFAULT RQ 가 없을때 처리
// 	if(GetItemCount()==0)
// 		Init_CompareChart();
}

void CNsyButtonListCtrl::SendToChartRemoveGraph(int nItem)
{
	if(nItem<0 || nItem>=GetItemCount()) return;
	
	m_bDirectRemove = TRUE;
	CString strCode, strName;
	strName = GetItemText(nItem, 0);
	strCode = GetItemText(nItem, 1);
	
	strName.TrimLeft(); strName.TrimRight();
	strName.Replace(" ", "");
	strName.Replace(",", "");
	((CStdDialog*)(GetParent()->GetParent()->GetParent()))->RemoveGraph(0, 0, strCode, strName);

	// DEFAULT RQ 가 없을때 처리
//	if(GetItemCount()==0)	Init_CompareChart();

	// DEFAULT RQ 지울때를 위한 루틴...
//	if(nItem==0)
	{
		DeleteSelectItem(nItem);
		//DeleteItem(nItem);
	}
	m_bDirectRemove = FALSE;
}

void CNsyButtonListCtrl::SetItemButtonVisible(int nItem, BOOL bVisible)
{
	for(int i = 0; i < m_ptrArrayBtnWnd.GetSize(); i++)
	{
		BTNINFO *pBtnInfo = NULL;
		pBtnInfo = (BTNINFO*)m_ptrArrayBtnWnd.GetAt(i);
		if(pBtnInfo != NULL)
		{
			if(bVisible)
				pBtnInfo->xSkinButton.ShowWindow(SW_SHOW);
			else
				pBtnInfo->xSkinButton.ShowWindow(SW_HIDE);
		}
	}
}
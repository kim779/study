// InterestPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TB205001.h"
#include "InterestPanel.h"
#include "afxdialogex.h"
#include "MapWnd.h"

#define btngroup	1
#define btnsearch	2
#define btngup		3
#define btngdown	4
#define btngremove  5
#define btncup		6
#define btncdown    7
#define btnsave     8
#define btncancel   9
#define btnallload   10
#define btnallsave   11

#pragma warning(disable:4996)
// CInterestPanel 대화 상자입니다.
#define TRIM(s) s.TrimRight();s.TrimLeft();
IMPLEMENT_DYNAMIC(CInterestPanel, CDialogEx)

CInterestPanel::CInterestPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInterestPanel::IDD, pParent)
{
	m_bFirst = TRUE;
	m_iGroupSelect = -1;
	m_arrGroup.RemoveAll();
	m_pPanelleft = NULL;
	m_pPanelright = NULL;
	m_bSearching = false;
	m_strGnameInSave.Empty();
}

CInterestPanel::~CInterestPanel()
{
	delete m_pfont;
	m_pfont = NULL;
	
	delete m_pPanelleft;  
	m_pPanelleft = NULL;

	delete m_pPanelright;  
	m_pPanelright = NULL;

	delete m_pbrush;
	m_pbrush = NULL;
}

void CInterestPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GROUP, m_listGroup);
	DDX_Control(pDX, IDC_LIST_CODE, m_listCode);
	DDX_Control(pDX, IDC_BTN_ADDGROUP, m_btnAddGroup);
	DDX_Control(pDX, IDC_BTN_CDOWN, m_btnCDown);
	DDX_Control(pDX, IDC_BTN_CREMOVE, m_btnCRemove);
	DDX_Control(pDX, IDC_BTN_CUP, m_btnCUp);
	DDX_Control(pDX, IDC_BTN_GDOWN, m_btnGDown);
	DDX_Control(pDX, IDC_BTN_GREMOVE, m_btnGRemove);
	//  DDX_Control(pDX, IDC_BTN_GROUP, m_btnGroup);
	DDX_Control(pDX, IDC_BTN_GROUP, m_btnGroup);
	DDX_Control(pDX, IDC_BTN_GUP, m_btnGUp);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
	//  DDX_Control(pDX, IDC_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_CLOSE, m_btnCancel);
	DDX_Control(pDX, IDC_ALL_LOAD, m_btnAllLoad);
	DDX_Control(pDX, IDC_ALL_SAVE, m_btnAllSave);
	DDX_Control(pDX, IDC_EDIT_GROUP, m_gpedit);
}

BEGIN_MESSAGE_MAP(CInterestPanel, CDialogEx)
	ON_BN_CLICKED(IDC_SAVE, &CInterestPanel::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BTN_ADDGROUP, &CInterestPanel::OnBnClickedBtnAddgroup)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CODE, &CInterestPanel::OnItemchangedListCode)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_GROUP, &CInterestPanel::OnItemchangedListGroup)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CODE, &CInterestPanel::OnDblclkListCode)
	ON_BN_CLICKED(IDC_BTN_GREMOVE, &CInterestPanel::OnBnClickedBtnGremove)
	ON_BN_CLICKED(IDC_BTN_CREMOVE, &CInterestPanel::OnBnClickedBtnCremove)
	ON_BN_CLICKED(IDC_CLOSE, &CInterestPanel::OnBnClickedClose)
	ON_NOTIFY(NM_CLICK, IDC_LIST_GROUP, &CInterestPanel::OnClickListGroup)
	ON_WM_CLOSE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_BTNMSG, OnMessage)
	ON_BN_CLICKED(IDC_BTN_GROUP, &CInterestPanel::OnBnClickedBtnGroup)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GROUP, &CInterestPanel::OnDblclkListGroup)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CInterestPanel::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CInterestPanel::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CInterestPanel::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_CUP, &CInterestPanel::OnBnClickedBtnCup)
	ON_BN_CLICKED(IDC_BTN_CDOWN, &CInterestPanel::OnBnClickedBtnCdown)
	ON_BN_CLICKED(IDC_BUTTON4, &CInterestPanel::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_ALL_SAVE, &CInterestPanel::OnBnClickedAllSave)
END_MESSAGE_MAP()


// CInterestPanel 메시지 처리기입니다.

CString CInterestPanel::GetGroupFromEdit()
{
	CString tmpS;
	((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->GetWindowText(tmpS);
	tmpS.TrimRight();
	tmpS.TrimLeft();
	return tmpS;
}

void CInterestPanel::applyArrToGlist(CListCtrl& list, CStringArray& arr, CStringArray& chkarr)
{
	CString tmpS;
	m_listGroup.DeleteAllItems();
	for(int ii = 0 ; ii < arr.GetSize() ; ii++)
	{
		int iItem = m_listGroup.GetItemCount();
		tmpS.Format(_T("%d"), iItem);

		m_listGroup.InsertItem(iItem, _T(""));
		m_listGroup.SetItemText(iItem,1, tmpS);
		m_listGroup.SetItemText(iItem,2, arr.GetAt(ii));	
	}

	for(int ii = 0 ; ii < m_listGroup.GetItemCount() ; ii++)
	{
		for(int jj = 0 ; jj < chkarr.GetSize() ; jj++)
		{
			if(m_listGroup.GetItemText(ii, 2) == chkarr.GetAt(jj))
				m_listGroup.SetCheck(ii);
		}
	}
	ResetGrouplistNo();
	FillCodelistByGroup(GetGroupFromEdit());
}

void CInterestPanel::applyArrToClist(CListCtrl& list, CStringArray& arr, CStringArray& chkarr)
{
	CString tmpS;
	m_listCode.DeleteAllItems();
	CMap<CString , LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	for(int ii = 0 ; ii < arr.GetSize() ; ii++)
	{
		int iItem = m_listCode.GetItemCount();
		tmpS.Format(_T("%d"), iItem);
		if(m_mapGroupCodemap.Lookup(GetGroupFromEdit(), pmap))
		{
			CCodeData cd;
			if(pmap->Lookup(arr.GetAt(ii),cd))
			{
				m_listCode.InsertItem(iItem, _T(""));
				m_listCode.SetItemText(iItem,1, tmpS);
				m_listCode.SetItemText(iItem,2, cd.strName);	
				m_listCode.SetItemText(iItem,3, cd.strCode);
				m_listCode.SetItemText(iItem,4, cd.strsuik);
				m_listCode.SetItemText(iItem,5, cd.strMangi);
			}
		}
	}

	for(int ii = 0 ; ii < m_listCode.GetItemCount() ; ii++)
	{
		for(int jj = 0 ; jj < chkarr.GetSize() ; jj++)
		{
			if(m_listCode.GetItemText(ii, 3) == chkarr.GetAt(jj))
				m_listCode.SetCheck(ii);
		}
	}

	//FillCodelistByGroup(GetGroupFromEdit());
}

LRESULT CInterestPanel::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case btnallload:
			{
				SearchInterList();
			}
			break;
		case btnallsave:
			{
				OnBnClickedSave();
				InterListOrderSave(0);
			}
			break;
		case btngroup:  //그룹추가
				OnBnClickedBtnAddgroup();
			break;
		case btnsearch:  //관심편집 띄우기
			{
				CWnd *pMain = AfxGetMainWnd();
				CRect rc;
				GetWindowRect(&rc);
				if(pMain->GetSafeHwnd())
				{
					CMapWnd* pMapWnd = (CMapWnd*)GetParent();

					pMain->SendMessage(WM_USER + 500 , axINTERSETON, (LPARAM)&rc);
					pMain->SendMessage(WM_USER + 500 , axINTERSETPT, (LPARAM)GetParent());
					pMain->SendMessage(WM_USER + 500 , axSETINTERKEY, pMapWnd->GetKey());
				}
			}
			break;
		case btngup:     //그룹위로
			{
				CStringArray arr, chkarr;
				CString tmpS, strtemp, strgp;
				bool bcheck = true;
				bool bgroupcheck = true;
				for(int ii = 0 ; ii < m_listGroup.GetItemCount() ; ii++)
				{
					arr.Add(m_listGroup.GetItemText(ii, 2));
				}

				for(int ii = 0 ; ii < m_listGroup.GetItemCount() ; ii++)
				{
					if(m_listGroup.GetCheck(ii))
						chkarr.Add(arr.GetAt(ii));
				}

				for(int ii = 0 ; ii < m_listGroup.GetItemCount() ; ii++)
				{
					if(bgroupcheck && (m_listGroup.GetCheck(ii)))
					{
						bgroupcheck = !CheckGpName(m_listGroup.GetItemText(ii, 2));
						strgp = m_listGroup.GetItemText(ii, 2);
					}
					
					if(m_listGroup.GetCheck(ii) && !CheckGpName(m_listGroup.GetItemText(ii, 2)))
					{		
						if(ii - 1 >= 0 && !m_listGroup.GetCheck(ii - 1) && bcheck && !CheckGpName(m_listGroup.GetItemText(ii - 1, 2)))
						{
							tmpS = arr.GetAt(ii);
							strtemp = arr.GetAt(ii - 1);

							arr.SetAt(ii - 1, tmpS);
							arr.SetAt(ii, strtemp);

							for(int jj = 0 ; jj < arr.GetSize(); jj++)
							{
								CString temp; 
								temp = arr.GetAt(jj);
								if(CheckGpName(arr.GetAt(jj)))
								{
									arr.RemoveAt(jj);
									arr.InsertAt(0, temp);
								}
							}
							
							applyArrToGlist(m_listGroup, arr, chkarr);
						}
						else
							bcheck = false;
					}
				}
				
				if(!bcheck || !bgroupcheck)
				{
					if(!bgroupcheck)
					{
						CString msg;
						msg.Format(_T("기본그룹인 %s은 수정할 수 없습니다."), strgp);
						AfxMessageBox(msg);
					}
					else
						AfxMessageBox("그룹 순서 편집할 수 없습니다.");
				}
			}
			break;
		case btngdown:  //그룹다운
			{
				CStringArray arr, chkarr;
				CString tmpS, strtemp, strgp;
				bool bcheck = true;
				bool bgroupcheck = true;
				for(int ii = 0 ; ii < m_listGroup.GetItemCount() ; ii++)
				{
					arr.Add(m_listGroup.GetItemText(ii, 2));
				}

				for(int ii = 0 ; ii < m_listGroup.GetItemCount() ; ii++)
				{
					if(m_listGroup.GetCheck(ii))
						chkarr.Add(arr.GetAt(ii));
				}

				for(int ii = m_listGroup.GetItemCount() - 1; ii >= 0 ; ii--)
				{			
					if(bgroupcheck && m_listGroup.GetCheck(ii))
					{
						bgroupcheck = !CheckGpName(m_listGroup.GetItemText(ii, 2));
						strgp = m_listGroup.GetItemText(ii, 2);
					}

					if(m_listGroup.GetCheck(ii) && !CheckGpName(m_listGroup.GetItemText(ii, 2)))
					{
						if(ii + 1 <= m_listGroup.GetItemCount() - 1 && !m_listGroup.GetCheck(ii + 1) && bcheck)
						{
							tmpS = arr.GetAt(ii);
							strtemp = arr.GetAt(ii + 1);

							arr.SetAt(ii + 1, tmpS);
							arr.SetAt(ii, strtemp);

							//test
							for(int jj = 0 ; jj < arr.GetSize(); jj++)
							{
								CString temp; 
								temp = arr.GetAt(jj);
								if(CheckGpName(arr.GetAt(jj)))
								{
									arr.RemoveAt(jj);
									arr.InsertAt(0, temp);
								}
							}
							//test

							applyArrToGlist(m_listGroup, arr, chkarr);
						}
						else
							bcheck = false;				
					}
				}

				if(!bcheck || !bgroupcheck)
				{
					if(!bgroupcheck)
					{
						CString msg;
						msg.Format(_T("기본그룹인 %s은 수정할 수 없습니다."), strgp);
						AfxMessageBox(msg);
					}
					else
						AfxMessageBox("그룹 순서 편집할 수 없습니다.");
				}
			}
			break;
		case btngremove:  //그룹삭제
			OnBnClickedBtnGremove();
			break;
		case btncup:     //종목위로
			{
				CStringArray arr, chkarr;
				CString tmpS, strtemp, msg;
				bool bcheck = true;

				((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->GetWindowText(tmpS);
				if(CheckGpName(tmpS))
				{
					msg.Format(_T("기본그룹인 %s 종목의 순서는 변경할 수 없습니다."), tmpS);
					AfxMessageBox(msg);
					return -1;
				}

				for(int ii = 0 ; ii < m_listCode.GetItemCount() ; ii++)
				{
					arr.Add(m_listCode.GetItemText(ii, 3));
				}

				for(int ii = 0 ; ii < m_listCode.GetItemCount() ; ii++)
				{
					if(m_listCode.GetCheck(ii))
						chkarr.Add(arr.GetAt(ii));
				}

				for(int ii = 0 ; ii < m_listCode.GetItemCount() ; ii++)
				{
					if(m_listCode.GetCheck(ii))
					{		
						if(ii - 1 >= 0 && !m_listCode.GetCheck(ii - 1) && bcheck)
						{
							tmpS = arr.GetAt(ii);
							strtemp = arr.GetAt(ii - 1);

							arr.SetAt(ii - 1, tmpS);
							arr.SetAt(ii, strtemp);
							applyArrToClist(m_listCode, arr, chkarr);
						}
						else
							bcheck = false;
					}
				}
			}
			break;
		case btncdown:   //종목아래로
			{
				CStringArray arr, chkarr;
				CString tmpS, strtemp, msg;
				bool bcheck = true;

				((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->GetWindowText(tmpS);
				if(CheckGpName(tmpS))
				{
					msg.Format(_T("기본그룹인 %s 종목의 순서는 변경할 수 없습니다."), tmpS);
					AfxMessageBox(msg);
					return -1;
				}

				for(int ii = 0 ; ii < m_listCode.GetItemCount() ; ii++)
				{
					arr.Add(m_listCode.GetItemText(ii, 3));
				}

				for(int ii = 0 ; ii < m_listCode.GetItemCount() ; ii++)
				{
					if(m_listCode.GetCheck(ii))
						chkarr.Add(arr.GetAt(ii));
				}

				for(int ii = m_listCode.GetItemCount() - 1; ii >= 0 ; ii--)
				{								
					if(m_listCode.GetCheck(ii))
					{
						if(ii + 1 <= m_listCode.GetItemCount() - 1 && !m_listCode.GetCheck(ii + 1) && bcheck)
						{
							tmpS = arr.GetAt(ii);
							strtemp = arr.GetAt(ii + 1);

							arr.SetAt(ii + 1, tmpS);
							arr.SetAt(ii, strtemp);
							applyArrToClist(m_listCode, arr, chkarr);
						}
						else
							bcheck = false;				
					}
				}
			}
			break;
		case btnsave:    //저장
			{
				OnBnClickedSave();
				((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->GetWindowText(m_strGnameInSave);
				InterListOrderSave(0);
			}
			break;
		case btncancel:  //취소
			OnBnClickedClose();
			break;
	}
	return 0;
}

BOOL CInterestPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Init();
	LoadCJCode();
	//LoadGroupOfFile();
	m_listGroup.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_listGroup.SetSelectionMark(0);
	Initbtn();
	SearchInterList();

	char* pdata = new char[sizeof(struct _ledgerH)];
	memset(pdata, ' ', sizeof(struct _ledgerH));
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	int nn = pMapWnd->GetWizard()->SendMessage(WM_USER, MAKEWPARAM(ledgerDLL, NULL), (LPARAM)pdata);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CInterestPanel::Initbtn()
{
	CString	fontName;
	fontName = _T("굴림");
	m_pfont = new CFont;
	m_pfont->CreateFont(	12,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,	           // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				DEFAULT_CHARSET,           // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName);       

	CString imgC, imgO, imgD;
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	CRect rc, tmpRc, rclist;
	GetClientRect(&rc);
	m_listGroup.GetClientRect(&rclist);

	tmpRc.left = rc.left + 3;
	tmpRc.top = rc.top + 3;
	tmpRc.right = tmpRc.left + rclist.Width() + 3;
	tmpRc.bottom = tmpRc.top + 31;

	m_pPanelleft = new CPanel;
	m_pPanelleft->m_pFont = m_pfont;
	if (!m_pPanelleft->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, tmpRc, this, 9898))
		delete m_pPanelleft;

	tmpRc.left = tmpRc.right + 8;
	tmpRc.right = rc.right - 3;
	tmpRc.bottom = tmpRc.top + 31;

	m_pPanelright = new CPanel;
	m_pPanelright->m_bleft = false;
	m_pPanelright->m_pFont = m_pfont;
	if (!m_pPanelright->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, tmpRc, this, 9898))
		delete m_pPanelright;

	m_btnSearch.set_ToolTipText(_T("종목검색"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_Act_50");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_Act_50_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_Act_50_DN");
	m_btnSearch.set_Image(imgC, imgO, imgD);
	m_btnSearch.SetItemFont(m_pfont);
	m_btnSearch.m_fcolor = RGB(255, 255, 255);
	m_btnSearch.m_text = _T("종목검색");
	m_btnSearch.m_id = btnsearch;

	m_btnAllLoad.set_ToolTipText(_T("서버로드"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_50");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTNt_50_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_50_DN");
	m_btnAllLoad.set_Image(imgC, imgO, imgD);
	m_btnAllLoad.SetItemFont(m_pfont);
	m_btnAllLoad.m_fcolor = RGB(0, 0, 0);
	m_btnAllLoad.m_text = _T("서버로드");
	m_btnAllLoad.m_id = btnallload;

	m_btnAllSave.set_ToolTipText(_T("서버저장"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_50");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_50_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_50_DN");
	m_btnAllSave.set_Image(imgC, imgO, imgD);
	m_btnAllSave.SetItemFont(m_pfont);
	m_btnAllSave.m_fcolor = RGB(0, 0, 0);
	m_btnAllSave.m_text = _T("서버저장");
	m_btnAllSave.m_id = btnallsave;

	m_btnSave.set_ToolTipText(_T("저장"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_32");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_32_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_32_DN");
	m_btnSave.set_Image(imgC, imgO, imgD);
	m_btnSave.SetItemFont(m_pfont);
	m_btnSave.m_fcolor = RGB(0, 0, 0);
	m_btnSave.m_text = _T("저장");
	m_btnSave.m_id = btnsave;

	m_btnCancel.set_ToolTipText(_T("취소"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_32");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_32_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_32_DN");
	m_btnCancel.set_Image(imgC, imgO, imgD);
	m_btnCancel.SetItemFont(m_pfont);
	m_btnCancel.m_fcolor = RGB(0, 0, 0);
	m_btnCancel.m_text = _T("취소");
	m_btnCancel.m_id = btncancel;

	m_btnGroup.set_ToolTipText(_T("그룹추가"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-enter");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-enter_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-enter_DN");
	m_btnGroup.set_Image(imgC, imgO, imgD);
	m_btnGroup.SetItemFont(m_pfont);
	m_btnGroup.m_fcolor = RGB(0, 0, 0);
	m_btnGroup.m_text = _T("");
	m_btnGroup.m_id = btngroup;

	m_btnGRemove.set_ToolTipText(_T("삭제"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_32");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_32_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_32_DN");
	m_btnGRemove.set_Image(imgC, imgO, imgD);
	m_btnGRemove.SetItemFont(m_pfont);
	m_btnGRemove.m_fcolor = RGB(0, 0, 0);
	m_btnGRemove.m_text = _T("삭제");
	m_btnGRemove.m_id = btngremove;

	m_btnGDown.set_ToolTipText(_T("아래로"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-down");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-down_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-down_EN");
	m_btnGDown.set_Image(imgC, imgO, imgD);
	m_btnGDown.SetItemFont(m_pfont);
	m_btnGDown.m_fcolor = RGB(0, 0, 0);
	m_btnGDown.m_text = _T("");
	m_btnGDown.m_id = btngdown;

	m_btnGUp.set_ToolTipText(_T("위로"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-up");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-up_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-up_EN");
	m_btnGUp.set_Image(imgC, imgO, imgD);
	m_btnGUp.SetItemFont(m_pfont);
	m_btnGUp.m_fcolor = RGB(0, 0, 0);
	m_btnGUp.m_text = _T("");
	m_btnGUp.m_id = btngup;

	m_btnCDown.set_ToolTipText(_T("아래로"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-down");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-down_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-down_EN");
	m_btnCDown.set_Image(imgC, imgO, imgD);
	m_btnCDown.SetItemFont(m_pfont);
	m_btnCDown.m_fcolor = RGB(0, 0, 0);
	m_btnCDown.m_text = _T("");
	m_btnCDown.m_id = btncdown;

	m_btnCUp.set_ToolTipText(_T("위로"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-up");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-up_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-up_DN");
	m_btnCUp.set_Image(imgC, imgO, imgD);
	m_btnCUp.SetItemFont(m_pfont);
	m_btnCUp.m_fcolor = RGB(0, 0, 0);
	m_btnCUp.m_text = _T("");
	m_btnCUp.m_id = btncup;

	m_pbrush = new CBrush();
	m_pbrush->CreateSolidBrush(RGB(255,255,255));

	m_pbrushbottom = new CBrush();
	m_pbrushbottom->CreateSolidBrush(RGB(238,235,234));

	m_pPenblue = new CPen;
	m_pPenblue->CreatePen(PS_SOLID, 1, RGB(238,235,234));

	m_pPenblack = new CPen;
	m_pPenblack->CreatePen(PS_SOLID, 1, RGB(143,148, 170));
}

void CInterestPanel::Init()
{
	CRect rc;
	//그룹리스트
	
	m_listGroup.GetClientRect(&rc);
	m_listGroup.SetExtendedStyle(m_listGroup.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	m_listGroup.InsertColumn(0,_T(" "), LVCFMT_CENTER, 18);
	m_listGroup.InsertColumn(1, _T("No"), LVCFMT_LEFT, 25);
	m_listGroup.InsertColumn(2, _T("그룹명"), LVCFMT_LEFT, (int)(rc.Width() - 20));

	//코드 리스트 
	m_listCode.SetExtendedStyle(m_listCode.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	m_listCode.GetClientRect(&rc);

	m_listCode.InsertColumn(0,_T(" "), LVCFMT_CENTER, 18);
	m_listCode.InsertColumn(1, _T("No"), LVCFMT_LEFT, 25);
	m_listCode.InsertColumn(2, _T("종목명"), LVCFMT_LEFT, (int)((rc.Width() - 43)*0.37));
	m_listCode.InsertColumn(3, _T("종목코드"), LVCFMT_LEFT, (int)((rc.Width() - 43)*0.26));
	m_listCode.InsertColumn(4, _T("민평"), LVCFMT_CENTER, (int)((rc.Width() - 43)*0.15));
	m_listCode.InsertColumn(5, _T("만기일"), LVCFMT_CENTER, (int)((rc.Width() - 43)*0.22));
}

void CInterestPanel::addlistGroup(CString strgroup)
{
	CString tmpS;
	int iItem = m_listGroup.GetItemCount();
	tmpS.Format(_T("%d"), iItem);

	m_listGroup.InsertItem(iItem, _T(""));
	m_listGroup.SetItemText(iItem,1, tmpS);
	m_listGroup.SetItemText(iItem,2, strgroup);	
}

void CInterestPanel::DeleteItem()
{
	OnBnClickedBtnCremove();
}

void CInterestPanel::addCode(CString strcode)
{	
	CString tmpS, msg;
	((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->GetWindowText(tmpS);
	
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	CStringArray arr;
	while(1)
	{
		arr.Add(pMapWnd->Parser(strcode, _T("\t")));
		if(strcode.IsEmpty())
			break;
	}

	CMap<CString , LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	if(m_mapGroupCodemap.Lookup(tmpS, pmap))
	{
		CCodeData cd;
		for(int ii = 0  ; ii < arr.GetSize(); ii++)
		{
			CString strtemp;
			if(m_mapcode.Lookup(arr.GetAt(ii), strtemp))
			{
				CCodeData cd;
				cd.strCode =  arr.GetAt(ii);
				cd.strName= pMapWnd->Parser(strtemp, _T("\t"));
				cd.strsuik= pMapWnd->Parser(strtemp, _T("\t"));
				cd.strMangi= strtemp;
				pmap->SetAt(arr.GetAt(ii), cd);
			}

		}
	}
	
	FillCodelistByGroup(tmpS);
	ResetCodelistNo();
}

void CInterestPanel::OnBnClickedSave()
{
	SaveFile(0);
		//axRELOADINTER
	CWnd *pMain = AfxGetMainWnd();
	if(pMain->GetSafeHwnd())
		pMain->SendMessage(WM_USER + 500 , axRELOADINTER, 0);
}

void CInterestPanel::SaveFile(int nSavePos /* = 0 */) 
{
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	CString strUserPath(_T(""));

	CString sMsg = _T("");

	if (nSavePos == 0)
		strUserPath.Format(_T("%s\\user\\%s"), pMapWnd->GetRoot(), pMapWnd->GetName());
	else
	{
		strUserPath.Format(_T("%s\\user\\%s\\%s"), pMapWnd->GetRoot(), pMapWnd->GetName(), "save");

		CreateDirectory(strUserPath, NULL);

		sMsg.Format(_T("관심종목의 모든 그룹 및 종목을 저장 하시겠습니까?.\n"));

		if(AfxMessageBox(sMsg, MB_YESNO | MB_ICONINFORMATION) == IDNO)
			return;
	}

	int nGroupCount = m_listGroup.GetItemCount();
	int nCodeCount = 0;

	CString strVal(_T(""));
	CString strGroupInfo(_T(""));
	CString strGIndex(_T(""));
	CString strGName(_T(""));
	CString strData(_T(""));
	CString strPath(_T(""));

	CString strFilePath(_T(""));
	strFilePath.Format("%s\\%s",strUserPath, "userinterest.ini");

	CFile file;
	CFileException ex;
	CArray<CCodeData, CCodeData>* pArr = NULL;
	CString strClr;

	strVal.Format("%d", nGroupCount);

	int nOldCount = GetPrivateProfileInt(_T("GROUPINFO"), _T("COUNT"), 0, (LPCTSTR)strFilePath);
	WritePrivateProfileString(_T("GROUPINFO"), _T("COUNT"), (LPCTSTR)strVal, (LPCTSTR)strFilePath);

	POSITION tmpos;
	CString	 strkey;
	int i = 0;
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;

	for(int ii = 0 ; ii < m_listGroup.GetItemCount(); ii++)
	{
		strGName = m_listGroup.GetItemText(ii, 2);
		
		if(!m_mapGroupCodemap.Lookup(strGName, pmap))
			continue;

		strGIndex.Format(_T("%03d"), i);
		strGroupInfo.Format(_T("%s;%s"), strGIndex, strGName);
		strVal.Format(_T("%d"), i);
	
		WritePrivateProfileString(_T("GROUPINFO"), (LPCTSTR)strVal, (LPCTSTR)strGroupInfo, (LPCTSTR)strFilePath);

		strPath.Format(_T("%s\\%s"), strUserPath, strGName);
		if (!file.Open((LPCTSTR)strPath, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate, &ex))
			continue;

		CString code;
		CCodeData cd;
		for (tmpos = pmap->GetStartPosition(); tmpos;)
		{
			pmap->GetNextAssoc(tmpos, strkey, cd);
			strData += cd.strCode;
			strData += _T("\t");
			strData += cd.strName;
			strData += _T("\t");
			strData += cd.strsuik;
			strData += _T("\t");
			strData += cd.strMangi;
			strData += _T("\n");
		}
	
		file.Write((const void*)(LPCTSTR)strData, strData.GetLength());
		file.Close();
		file.Flush();

		strData.Empty();
		i++;
	}
}

BOOL CInterestPanel::DirectoryExist(LPCTSTR pszDir)
{
	int nAttr = ::GetFileAttributes(pszDir);
	return  nAttr == -1 ? FALSE : (nAttr & FILE_ATTRIBUTE_DIRECTORY ? TRUE : FALSE);
}

void CInterestPanel::LoadGroupOfFile(int nSavePos /* = 0 */)
{
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	CString strUserPath(_T(""));
	CString strFilePath(_T(""));
	CString sMsg = _T("");

	if (nSavePos == 0)
		strUserPath.Format(_T("%s\\user\\%s"), pMapWnd->GetRoot(), pMapWnd->GetName());
	else
	{
		strUserPath.Format(_T("%s\\user\\%s\\%s"), pMapWnd->GetRoot(), pMapWnd->GetName(), "save");

		if (DirectoryExist(strUserPath))
		{
			CreateDirectory(strUserPath, NULL);

			sMsg.Format(_T("현재 관심종목의 모든 그룹 및 종목이 초기화 됩니다. \n새로운 관심종목을 Load 할까요? \n\nPATH : %s"), strUserPath);

			if(AfxMessageBox(sMsg, MB_YESNO | MB_ICONINFORMATION) == IDNO)
				return;
		
		}
		else
		{
			sMsg.Format(_T("PATH : %s \n폴더가 존재하지 않습니다. \n\n저장된 관심종목이 존재하지 않습니다."), strUserPath);
			AfxMessageBox(sMsg, MB_OK | MB_ICONINFORMATION);
			return;
		}

	}

	CFile file;
	CFileException ex;
	CStringArray* pArr = NULL;

	strFilePath.Format(_T("%s\\%s"), strUserPath, _T("userinterest.ini"));

	int nGrpCnt = GetPrivateProfileInt(_T("GROUPINFO"), _T("COUNT"), 0, (LPCTSTR)strFilePath);

	CString sGroup(_T(""));
	CString sGrpData(_T(""));
	CString strGroup(_T(""));

	m_listGroup.DeleteAllItems();
	m_listCode.DeleteAllItems();

	for ( int i = 0 ; i < nGrpCnt ; i ++ )
	{
		sGroup.Format(_T("%d"), i);

		GetPrivateProfileString(_T("GROUPINFO"), _T(sGroup), _T(""), sGrpData.GetBufferSetLength(1024), 1024, (LPCTSTR)strFilePath);
		sGrpData.ReleaseBuffer();

		_TRIMDATA(sGrpData);
		if(sGrpData.IsEmpty())
			continue;

		strGroup = pMapWnd->Parser(sGrpData, _T(";"));
		strGroup = sGrpData;

		AddGroup(strGroup);

		LoadFile(strGroup);
	}

	ReflashSelectList();
}

void CInterestPanel::LoadFile(CString strgroup)
{
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	CString strUserPath(_T(""));
	CString strFilePath(_T(""));

	strUserPath.Format("%s\\user\\%s\\%s", pMapWnd->GetRoot(), pMapWnd->GetName(), strgroup);

	CString strData(_T(""));

	CFile file;

	CFileException ex;

	int nTry = 0;
	
	while(1)
	{
		if (file.Open(strUserPath, CFile::modeRead | CFile::shareDenyWrite, &ex))
		{
			int nFileSize = (int)file.GetLength();

			char* buffer = new char[nFileSize + 1];

			if ( nFileSize > 0 )
			{
				ZeroMemory(buffer, nFileSize + 1);

				FillMemory(buffer, nFileSize + 1, 0L);

				file.Read(buffer, nFileSize + 1);

				strData.Format("%s", buffer);
			}

			delete []buffer;

			file.Close();

			break;
		}
		else
		{
			if (nTry >= 10)
				break;

			Sleep(10);

			nTry ++;
		}
	}

	CString sTmp(_T(""));

	int ncnt = 0;

	ncnt = 0;

	CString strCode(_T(""));

	CString strCodeName(_T(""));

	CString strMangi(_T(""));

	CString strsuik(_T(""));

	CString strName(_T(""));

	CString strCodeData(_T(""));

	CString stmp(_T(""));

	CString strCount(_T(""));

	for ( int i = 0 ; i < 80 ; i ++ )
	{
		if (strData.IsEmpty())
			break;
	
		strCodeData = pMapWnd->Parser(strData, _T("\n"));
		strCode = pMapWnd->Parser(strCodeData, _T("\t"));
		strCode.TrimRight();
		CJCODE* pcjcode;
		for(int ii = 0 ; ii < m_arrayCJCode.GetSize() ; ii++)
		{
				
			pcjcode = m_arrayCJCode.GetAt(ii);
			if(strCode == CString(pcjcode->code, 12).TrimRight())
			{
				strName.Format(_T("%.100s"), pcjcode->name);
				strName.TrimRight();
				strName.TrimLeft();
				strsuik.Format(_T("%.10s"), pcjcode->minthree);
				strsuik.TrimRight();
				strsuik.TrimLeft();
				strMangi.Format(_T("%.10s"), pcjcode->mtday);
				strMangi.TrimRight();
				strMangi.TrimLeft();
				break;
			}
		}
					
		stmp.Format(_T("%s"), strCode);
		strCodeName = _T("");

		AddListOfCodeMap(strgroup, stmp, strName, strsuik, strMangi);

		ncnt++;
	}	

	/*
	if(m_bFirst)
	{
		int nItem = 0;
		m_listGroup.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_listGroup.SetSelectionMark(nItem);
		m_bFirst = FALSE;
	}
	*/
	ResetCodelistNo();
	ResetGrouplistNo();
}

bool CInterestPanel::CheckCodeInMap(CString strgroup, CString strcode)
{
	/*
	CArray<CCodeData, CCodeData>* ptrArr = NULL;
	CString	 strkey, tmpS;
	POSITION pos;
	
	int i = 0;
	bool bfind = false;
	if(m_mapGroupCode.Lookup(strgroup, ptrArr))
	{
		for(int ii = 0 ; ii < ptrArr->GetSize() ; ii++)
		{
			CCodeData cd = ptrArr->GetAt(ii);
			if(cd.strCode == strcode)
			{
					bfind = true;
					return bfind;
			}
		}
	}
	*/
	return false;
}

void CInterestPanel::AddListOfCodeMap(CString strgroup, CString sCode, CString sName, CString strsuik, CString strMangi)
{
	CArray<CCodeData, CCodeData>* ptrArr = NULL;
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	CString	 strkey;
	POSITION pos;

	for (pos = m_mapGroupCodemap.GetStartPosition(); pos; )
	{
		m_mapGroupCodemap.GetNextAssoc(pos, strkey, pmap);
		if(strkey == strgroup)
		{
			CCodeData cd;
			cd.strCode = sCode;
			cd.strName = sName;
			cd.strsuik = strsuik;
			cd.strMangi = strMangi;
			pmap->SetAt(sCode, cd);
		}
	}
}

void CInterestPanel::ReflashSelectList(CString strgroup)
{
	CArray<CCodeData, CCodeData>* ptrArr = NULL;
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	CString	 strkey, tmpS, strtemp;
	POSITION pos;
	CStringArray arr;

	if(strgroup.IsEmpty())
	{
		m_listGroup.DeleteAllItems();
		for(int ii = 0 ; ii < m_arrGroup.GetSize() ; ii++)
			arr.Add(m_arrGroup.GetAt(ii));
	
		for(int ii = 0 ; ii < arr.GetCount() ; ii++)
		{
			strkey = arr.GetAt(ii);
		//	for (pos = m_mapGroupCodemap.GetStartPosition(); pos;)
		//	{
		//	if(arr.GetAt(ii) == strkey)
			{
				int iItem = m_listGroup.GetItemCount();
				tmpS.Format(_T("%d"), iItem);
				m_listGroup.InsertItem(iItem, _T(""));
				m_listGroup.SetItemText(iItem,1, tmpS);
				m_listGroup.SetItemText(iItem,2, strkey);	
			//	break;
			}
		//	}
		}
		
		if(m_listGroup.GetItemCount() > 0)
		{
			((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->SetWindowText(m_listGroup.GetItemText(0,2));
			FillCodelistByGroup(m_listGroup.GetItemText(0,2));
		}
		else
			m_listCode.DeleteAllItems();
	}
	else
	{
		if(m_mapGroupCodemap.Lookup(strgroup, pmap))
		{
			int i = 0;
			int iItem = m_listCode.GetItemCount();
			tmpS.Format(_T("%d"), iItem);
			CCodeData cd;
			for (pos = pmap->GetStartPosition(); pos;)
			{
				pmap->GetNextAssoc(pos, strkey, cd);
				m_listCode.InsertItem(iItem, _T(""));
				m_listCode.SetItemText(iItem,1, tmpS);
				m_listCode.SetItemText(iItem,2, cd.strName);	
				m_listCode.SetItemText(iItem,3, cd.strCode);
				m_listCode.SetItemText(iItem,4, cd.strsuik);
				m_listCode.SetItemText(iItem,5, cd.strMangi);
			}
		}
	}
	ResetGrouplistNo();
}

void CInterestPanel::ReflashSelectList(int nItem)
{

}

void CInterestPanel::RemovePrevName(CString& strName)
{
	if (strName.IsEmpty())
		return;

	char ch = strName.GetAt(0);
	if (ch == _T('!') || ch == _T('#') || ch == _T('%') || ch == _T('&') || ch == _T('-') || ch == _T('@') )
		strName.Delete(0);
}

void CInterestPanel::AddListOfCodeArray(int nIdx, CString sCode, CString sName, CString strsuik, CString strmangi)
{
	
}

void CInterestPanel::ClearGroupList()
{
	
}

void CInterestPanel::AddGroup(CString strGroup)
{
	CString strIndex(_T(""));
	strGroup.TrimRight();
	strIndex.Format(_T("%03d"), m_listGroup.GetItemCount() + 1);

	RemoveSpChar(strGroup);

	addlistGroup(strGroup);
	m_arrGroup.Add(strGroup);

	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	if(!m_mapGroupCodemap.Lookup(strGroup, pmap))
	{
		pmap =  new CMap<CString, LPCTSTR, CCodeData, CCodeData>;
		m_mapGroupCodemap.SetAt(strGroup, pmap);
	}
	
	((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->SetWindowTextA(strGroup);
	FillCodelistByGroup(strGroup);
	ResetGrouplistNo();
}

void CInterestPanel::RemoveSpChar(CString &strValue)
{
	strValue.Replace("~", "");
	strValue.Replace("!", "");
	strValue.Replace("@", "");
	strValue.Replace("#", "");
	strValue.Replace("$", "");
	strValue.Replace("%", "");
	strValue.Replace("^", "");
	strValue.Replace("&", "");
	strValue.Replace("*", "");
	strValue.Replace(";", "");

}

void CInterestPanel::OnBnClickedBtnAddgroup()
{
	//IDC_EDIT_GROUP
	CString tmpS;
	((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->GetWindowText(tmpS);

	_TRIMDATA(tmpS);
	if(tmpS.IsEmpty())
	{
		MessageBox("공백은 그룹명으로 사용하실 수 없습니다.");
		return;
	}

	if(!CheckGroupName(tmpS))
	{
		MessageBox("이미 존재하는 그룹명입니다.");
		return;
	}

	InterListTR(0, tmpS, "");  //그룹추가
	//AddGroup(tmpS);  //test
}

BOOL CInterestPanel::CheckGroupName(CString strGroup)
{
	for(int ii = 0 ; ii < m_listGroup.GetItemCount(); ii++)
	{
		if(m_listGroup.GetItemText(ii,2) == strGroup)
			return FALSE;
	}
	return TRUE;
}

void CInterestPanel::OnItemchangedListCode(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	*pResult = 0;
}

void CInterestPanel::OnItemchangedListGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	*pResult = 0;
}

void CInterestPanel::OnDblclkListCode(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CArray<CCodeData, CCodeData>* ptrArr = NULL;
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	CString	 strkey, tmpS;
	
	((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->GetWindowText(tmpS);

	if(CheckGpName(tmpS.TrimRight()))
	{
		CString msg; 
		msg.Format(_T("기본그룹인 %s의 종목은 삭제할 수 없습니다."), tmpS);
		AfxMessageBox(msg);
		return;
	}

	int i = 0;
	if(m_mapGroupCodemap.Lookup(tmpS, pmap))
	{
		pmap->RemoveKey(m_listCode.GetItemText(pNMItemActivate->iItem, 3));
	}
	
	m_listCode.DeleteItem(pNMItemActivate->iItem);
	ResetGrouplistNo();
	*pResult = 0;
}

void CInterestPanel::FillCodelistByGroup(CString strgroup)
{
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	CString	 strkey, tmpS;
	POSITION pos;

	int i = 0;
	if(m_mapGroupCodemap.Lookup(strgroup, pmap))
	{
		m_listCode.DeleteAllItems();
		int iItem = m_listCode.GetItemCount();
		tmpS.Format(_T("%d"), iItem);
		int ii = 0;
		CCodeData cd;

		for (pos = pmap->GetStartPosition(); pos;)
		{
			pmap->GetNextAssoc(pos, strkey, cd);
			m_listCode.InsertItem(iItem, _T(""));
			m_listCode.SetItemText(iItem,1, tmpS);
			m_listCode.SetItemText(iItem,2, cd.strName);	
			m_listCode.SetItemText(iItem,3, cd.strCode);
			m_listCode.SetItemText(iItem,4, cd.strsuik);
			m_listCode.SetItemText(iItem,5, cd.strMangi);
			ii++;
		}
	}
	ResetCodelistNo();
}

void CInterestPanel::RemoveGroupInArr(CString strgroup)
{
	for(int ii = 0 ; ii < m_arrGroup.GetSize() ; ii++)
	{
		if(m_arrGroup.GetAt(ii) == strgroup)
		{
			m_arrGroup.RemoveAt(ii);
			return;
		}
	}
}

void CInterestPanel::OnBnClickedBtnGremove()
{
	CString tmpS, msg;
	int icheck = 0;
	for(int ii = 0 ; ii < m_listGroup.GetItemCount() ; ii++)
	{
		if(m_listGroup.GetCheck(ii) && CheckGpName(m_listGroup.GetItemText(ii, 2).TrimRight()))
		{
			msg.Format(_T("기본그룹인 %s은 삭제할 수 없습니다."), m_listGroup.GetItemText(ii, 2).TrimRight());
			AfxMessageBox(msg);
				return;
		}
		
		if(m_listGroup.GetCheck(ii))
			icheck++;
	}

	if(icheck == 0)
		return;

	tmpS.Format(_T("선택한 %d개 그룹을 삭제 하시겠습니까?"), icheck);
	if (::MessageBox(this->m_hWnd, tmpS, _T("그룹삭제"), MB_OKCANCEL) == IDOK)
	{
		for(int ii = 0 ; ii < m_listGroup.GetItemCount() ; ii++)
		{
			if(m_listGroup.GetCheck(ii))
			{
				CMapWnd* pMapWnd = (CMapWnd*)GetParent();
				tmpS = m_listGroup.GetItemText(ii, 2);
				
				/*
				m_mapGroupCodemap.RemoveKey(tmpS);
				RemoveGroupInArr(tmpS);
				tmpS.Format(_T("%s\\user\\%s\\%s"), pMapWnd->GetRoot(), pMapWnd->GetName(), m_listGroup.GetItemText(ii, 2));
				DeleteFile(tmpS);		
				*/
			}
		}
		DeleteAllInterList();
	}

	//ReflashSelectList();
}

void CInterestPanel::RemoveCodeInGroup(CString strgroup, CString strcode)
{
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	if(m_mapGroupCodemap.Lookup(strgroup, pmap))
		pmap->RemoveKey(strcode);
}

void CInterestPanel::OnBnClickedBtnCremove()
{
	int icheck = 0;
	CString tmpS, strtemp ,msg;

	((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->GetWindowText(tmpS);

	if(CheckGpName(tmpS.TrimRight()))
	{
		msg.Format(_T("기본그룹인 %s은 종목을 삭제할 수 없습니다."), tmpS);
		AfxMessageBox(msg);
		return;
	}
	
	for(int ii = 0 ; ii < m_listCode.GetItemCount() ; ii++)
	{
		if(m_listCode.GetCheck(ii))
			icheck++;
	}

	//tmpS.Format(_T("선택한 %d개 종목을 삭제 하시겠습니까"), icheck);
	//if (::MessageBox(this->m_hWnd, tmpS, _T("종목삭제"), MB_OKCANCEL) == IDOK)
	//{
		for(int ii = 0 ; ii < m_listCode.GetItemCount() ; ii++)
		{
			if(m_listCode.GetCheck(ii))
			{
				((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->GetWindowText(tmpS);
				RemoveCodeInGroup(tmpS, m_listCode.GetItemText(ii, 3));	
			}
		}
	//}
	FillCodelistByGroup(tmpS);
}

void CInterestPanel::OnBnClickedClose()
{
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	pMapWnd->SendMessage(WM_CLOSE, 0, 0);
}

void CInterestPanel::OnClickListGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CString tmpS;
	tmpS = m_listGroup.GetItemText(pNMItemActivate->iItem, 2);
	((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->SetWindowText(tmpS);
	
	tmpS.TrimLeft();
	tmpS.TrimRight();

	if(tmpS.IsEmpty())
		return;
	
	m_selectgroup = tmpS;
	InterListCodeTR(1, tmpS); //0 조회
	*pResult = 0;
}

void CInterestPanel::ResetGrouplistNo()
{
	CString tmpS;
	for(int ii = 0 ; ii < m_listGroup.GetItemCount(); ii++)
	{
		tmpS.Format(_T("%d"), ii + 1);
		m_listGroup.SetItemText(ii, 1, tmpS);
	}
}

void CInterestPanel::ResetCodelistNo()
{
	CString tmpS;
	for(int ii = 0 ; ii < m_listCode.GetItemCount(); ii++)
	{
		tmpS.Format(_T("%d"), ii + 1);
		m_listCode.SetItemText(ii, 1, tmpS);
	}
}

void CInterestPanel::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnClose();
}

void CInterestPanel::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CInterestPanel::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc, rclist;
	GetClientRect(&rc);
	m_listGroup.GetClientRect(&rclist);

	CBrush* oldBrush = pDC->SelectObject(m_pbrush);

	pDC->PatBlt(0, 0, rc.Width(), rc.Height() - 29, PATCOPY);
	pDC->SelectObject(oldBrush);

	oldBrush = pDC->SelectObject(m_pbrushbottom);
	pDC->PatBlt(0, rc.Height() - 29, rc.Width(), rc.Height(), PATCOPY);
	pDC->SelectObject(oldBrush);

	pDC->SelectObject(m_pPenblack);
	pDC->MoveTo(0, rc.Height() - 28);
	pDC->LineTo(rc.Width(), rc.Height() - 28);

	pDC->SelectObject(m_pPenblue);
	pDC->MoveTo(rclist.right + 10 ,0);
	pDC->LineTo(rclist.right + 10, rc.Height() - 29);

	return TRUE;
}

void CInterestPanel::OnBnClickedBtnGroup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

#include "GroupNameDlg.h"
void CInterestPanel::OnDblclkListGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CGroupNameDlg dlg;
	dlg.SetGroupName(m_listGroup.GetItemText(pNMItemActivate->iItem, 2));

	if (dlg.DoModal() == IDCANCEL)
		return;

	CString msg;
	CString sName = dlg.GetGroupName();
	sName.TrimRight();

	if(CheckGpName(m_listGroup.GetItemText(pNMItemActivate->iItem, 2).TrimRight()))
	{
		msg.Format(_T("기본그룹인 %s은 수정할 수 없습니다."), m_listGroup.GetItemText(pNMItemActivate->iItem, 2));
		AfxMessageBox(msg);
		return;
	}

	if (sName.IsEmpty())
	{
		AfxMessageBox("공백은 그룹명으로 사용하실 수 없습니다.");
		return;
	}

	POSITION pos;
	CString	 strkey;
	int i = 0;
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	for (pos = m_mapGroupCodemap.GetStartPosition(); pos;)
	{
		m_mapGroupCodemap.GetNextAssoc(pos, strkey, pmap);
		if(strkey == sName)
		{
			AfxMessageBox("동일한 그룹명으로 사용하실 수 없습니다.");
			return;
		}
	}

	if(m_mapGroupCodemap.Lookup(m_listGroup.GetItemText(pNMItemActivate->iItem, 2), pmap))
	{
		m_oldGrm = m_listGroup.GetItemText(pNMItemActivate->iItem, 2);
		m_oldGrm.TrimRight();
		this->InterListTR(1, m_oldGrm,sName);
	}
	*pResult = 0;
}

BOOL CInterestPanel::LoadCJCode()
{
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	CString path, tmps, strtemp, tmpS;
	int codeN;
	struct CJcode code;
	CJCODE* cjCode;
	CJCODESTR* cjstrCode = NULL;
	CFile file;
	
	path = pMapWnd->GetRoot() + "\\tab\\" + _T("issbondmast.txt");   
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return false;
	
	int ilen = (int)file.GetLength();
	codeN = (int)file.GetLength() / sizeof(struct CJcode);

	m_arrayCJCode.RemoveAll();
	for (int ii = 0; ii < codeN; ii++)
	{
		cjCode = new CJCODE;
		cjstrCode = new CJCODESTR;

		memset(cjCode, 0, L_CJcode);
		
		file.Read(&code, sizeof(struct CJcode));
		
		memcpy(cjCode->code, code.code, 12);
		tmps = CString(code.name, 100);
		TRIM(tmps);
		memcpy(cjCode->name, tmps, tmps.GetLength());

		tmps = CString(code.kind, 4);
		TRIM(tmps);
		memcpy(cjCode->kind, tmps, tmps.GetLength());

		tmps = CString(code.idct, 6);
		TRIM(tmps);
		memcpy(cjCode->idct, tmps, tmps.GetLength());

		tmps = CString(code.valday, 10);
		TRIM(tmps);
		memcpy(cjCode->valday, tmps, tmps.GetLength());

		tmps = CString(code.mtday, 10);
		TRIM(tmps);
		memcpy(cjCode->mtday, tmps, tmps.GetLength());

		tmps = CString(code.mtry, 6);
		TRIM(tmps);
		memcpy(cjCode->mtry, tmps, tmps.GetLength());

		tmps = CString(code.itrt, 10);
		TRIM(tmps);
		memcpy(cjCode->itrt, tmps, tmps.GetLength());

		tmps = CString(code.intertype, 6);
		TRIM(tmps);
		memcpy(cjCode->intertype, tmps, tmps.GetLength());

		tmps = CString(code.credit, 6);
		TRIM(tmps);
		memcpy(cjCode->credit, tmps, tmps.GetLength());

		tmps = CString(code.minfore, 10);
		TRIM(tmps);
		memcpy(cjCode->minfore, tmps, tmps.GetLength());

		tmps = CString(code.minthree, 10);
		TRIM(tmps);
		memcpy(cjCode->minthree, tmps, tmps.GetLength());

		tmps = CString(code.update, 14);
		TRIM(tmps);
		memcpy(cjCode->update, tmps, tmps.GetLength());
		
		tmps = CString(code.update, 14);
		TRIM(tmps);
		memcpy(cjCode->update, tmps, tmps.GetLength());
		
		m_arrayCJCode.Add(cjCode);
		tmpS.Format(_T("%s\t%s\t%s"), CString(cjCode->name, 100).TrimRight(), CString(cjCode->minfore, 10).TrimRight(),  CString(cjCode->mtday, 10).TrimRight());
		
		m_mapcode.SetAt(CString(cjCode->code, 12).TrimRight(), tmpS);
	}

	file.Close();

	return TRUE;
}

void CInterestPanel::GetledgerH(struct _ledgerH* pLedgerH, int trGubn)
{
	CString strtemp;
	strtemp = "FITS-HTS";
	memcpy(pLedgerH->anam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());  

	switch(trGubn)
	{
		case 0:  //관심그룹 리스트 가져오기 
			{
				strtemp = "FITSComAttGrpSO"; 
				memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

				strtemp = "selectAttGrpList";
				memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
			}
			break;
		case 1:   //관심그룹 종목 조회, 추가 및 변경
			{
				strtemp = "FITSComAttGrpSO"; 
				memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

				strtemp = "doAttGrpItm";
				memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
			}
			break;
		case 2:  //관심순서 저장
			{
				strtemp = "FITSComAttGrpSO"; 
				memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

				strtemp = "saveAttGrpSeq";
				memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
			}
			break;
		case 3:   //관심그룹 추가/변경/삭제
			{
				strtemp = "FITSComAttGrpSO"; 
				memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

				strtemp = "doAttGrp";
				memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
			}
			break;
		case 4:   //관심그룹 추가/변경/삭제
			{
				strtemp = "FITSComAttGrpSO"; 
				memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

				strtemp = "deleteAttGrpMulti";
				memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
			}
			break;
	}
}

bool CInterestPanel::addcodeCheck(CString& bgroup)
{
	CString tmpS;
	((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->GetWindowText(tmpS);
	if(CheckGpName(tmpS))
	{
		bgroup = tmpS;
		return false;
	}
	else
		return true;
}

bool CInterestPanel::CheckGpName(CString strgroup, int gubn) 
{   //디폴트 그룹(C) 이면 true
	CString strkey;
	if(!m_mapGroupck.Lookup(strgroup, strkey))
		return false;

	if(strkey == _T("U")) 
		return false;
	else
		return true; 
}

void CInterestPanel::InterListCodeTR(int gubn, CString strgroup)
{//관심그룹 코드 조회 저장
	typedef struct _listcodemid
	{
		char usrid[30];
		char gbOption;      //S 저장  V 조회
		char attGrpNm[20];   //그룹명
		char attCnt[4];      //종목갯수
	//	int attCnt;      //종목갯수
		//char kRItmCd[12];    //종목코드
	}LISTCODETR;

	if(m_bSearching == true)
		return;

	if(gubn == 0)
	{
		if(CheckGpName(strgroup))  //디폴트 그룹 C이면 저장을 못한다  유저그룹은 U
			return;
	}

	CString tmpS, strkey, strcode, strsend;
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	char* psub = NULL;
	struct _listcodemid  *pmid = new struct _listcodemid;
	memset(pmid, ' ', sizeof(struct _listcodemid) + 1);

	//memset(&pmid[sizeof(struct _listcodemid)], 0, sizeof(struct _listcodemid));

	tmpS.Format(_T("%s"), pMapWnd->GetUser());
	tmpS.TrimRight();
	tmpS.TrimLeft();
	memcpy(pmid->usrid, (char*)tmpS.operator LPCSTR(), tmpS.GetLength());

	if(gubn == 0)  //저장 
		pmid->gbOption = 'S';
	else if(gubn == 1)  //조회
		pmid->gbOption = 'V';
	
	memcpy(pmid->attGrpNm, (char*)strgroup.operator LPCSTR(), strgroup.GetLength());

	POSITION pos;
	if(gubn == 0)
	{
		CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
		if(m_mapGroupCodemap.Lookup(strgroup, pmap))
		{
			CCodeData cd;
			strcode.Empty();
			for (pos = pmap->GetStartPosition(); pos;)
			{
				pmap->GetNextAssoc(pos, strkey, cd);
				strcode += cd.strCode;
			}
			tmpS.Format(_T("%d"), pmap->GetCount());
			tmpS.TrimLeft();
			tmpS.TrimRight();
			memset(pmid->attCnt, 0, 1);
			memcpy(pmid->attCnt, (char*)tmpS.operator LPCSTR(), 1);
		//	pmid->attCnt = pmap->GetCount();
		}
		
		strsend.Format(_T("%s"), (char*)pmid, sizeof(struct _listcodemid));
		strsend = strsend.Left(sizeof(struct _listcodemid));
		strsend += strcode;
	}
	else if(gubn == 1)
	{
		strsend.Format(_T("%s"), (char*)pmid, sizeof(struct _listcodemid));
		strsend = strsend.Left(sizeof(struct _listcodemid));
	}

	char* pbuf = new char[strsend.GetLength() + 1];
	memset(pbuf, ' ', strsend.GetLength() + 1);
	memcpy(pbuf, (char*)strsend.operator LPCSTR(), strsend.GetLength());
	//if(sendTR("PIBOPFRM", (char*)strsend.operator LPCSTR(), strsend.GetLength(), 1))
	if(sendTR("PIBOPFRM", pbuf, strsend.GetLength(), 1) == true)
	{
		m_bSearching = true;
	}
}

void CInterestPanel::oubInterListCode(char* pdata)   //관심그룹 종목 저장 조회
{
	typedef struct _interCodemod
	{
		char filler[30];
		char gubun;
		char usrid[30];
		char gbOption;
		char successYN;
		char attGrpNm[20];
		char attCnt[4];
		char errMsg[200];
		char KRItmCd[12]; 
	}INTCODEMOD;

	m_bSearching = false;

	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	char* pbuf = (char*)pdata;
	struct _interCodemod* pmod =(struct _interCodemod*)&pdata[sizeof(struct  _ledgerH)];

	CString strsuccess, tmpS, strcode, strgroup;
	if(CString(pmod->successYN, 1).TrimRight().MakeUpper() != "Y")
	{
		m_arrTrGrp.RemoveAll();
		return;
	}

	m_listCode.DeleteAllItems();
	//if(CString(pmod->gbOption, 1).TrimRight() == "S")  //저장
	{

	}
	//else if(CString(pmod->gbOption, 1).TrimRight() == "V") //조회
	{
		strgroup = CString(pmod->attGrpNm, 20).TrimRight();

		CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
		if(m_mapGroupCodemap.Lookup(strgroup, pmap))
		{		
			int cnt = atoi(CString(pmod->attCnt, 4).TrimRight());
			
			for(int ii = 0 ; ii < cnt ; ii++)
			{
				//strcode =CString((char*)&pmod[pos + (ii * 12)], 12).TrimRight();
				//strcode = CString(pmod->KRItmCd, 12).TrimRight();
				//pcode = (char*)&pmod[pos];
				char* pcode = new char[13];
				memset(pcode, 0, 13);
				memcpy(pcode, (char*)&pmod->KRItmCd[0 + (ii * 12)], 12);
				strcode = CString((char*)pcode, 12).TrimRight();
				CString strtemp;
				if(m_mapcode.Lookup(strcode, strtemp))
				{
					CCodeData cd;
					cd.strCode =  strcode;
					cd.strName= pMapWnd->Parser(strtemp, _T("\t"));
					cd.strsuik= pMapWnd->Parser(strtemp, _T("\t"));
					cd.strMangi= strtemp;
					pmap->SetAt(strcode, cd);
				}
				delete pcode;
				pcode = NULL;
			}	
			m_mapGroupCodemap.SetAt(strgroup,pmap);
		}

		FillCodelistByGroup(strgroup);
	}

	if(m_arrTrGrp.GetSize() > 0)
	{
		m_arrTrGrp.RemoveAt(0);
		if(m_arrTrGrp.GetSize() > 0)
		{
			EnableBtn(false);
			InterListCodeTR(CString(pmod->gbOption, 1).TrimRight() == "S" ? 0 : 1, m_arrTrGrp.GetAt(0));
		}
		else
		{
			if(m_bFirst)  //최초 화면 오픈시 처리
			{
				m_bFirst = false;
				InterListCodeTR(1, m_listGroup.GetItemText(0, 2).TrimRight());
				//((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->SetWindowText(m_listGroup.GetItemText(0, 2).TrimRight());
				((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->SetWindowText("");
			}
			else
			{
				if(!m_strGnameInSave.IsEmpty())
				{
					((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->SetWindowText(m_strGnameInSave);
					FillCodelistByGroup(m_strGnameInSave);
					m_strGnameInSave.Empty();
				}
			}
			EnableBtn(true);
		}
	}
}

void CInterestPanel::InterListOrderSave(int gubn)
{//관심그룹 순서 저장  전체저장할때만 저장한다 
	typedef struct _listorderemid
	{
		char usrid[30];
		char gbOption;
		char attCnt[4];
	}LISTORDERMID;

	if(m_bSearching == true)
		return;

	CString tmpS, strgroup, strsend, temp;
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	char* psub = NULL;

	struct _listorderemid  *pmid = new struct _listorderemid;
	//char* pbuf = new char[sizeof(struct _listorderemid) + 1];
	memset(pmid, ' ', sizeof(struct _listorderemid) + 1);

	//memset(&pmid[sizeof(struct _listcodemid)], 0, sizeof(struct _listcodemid));

	tmpS.Format(_T("%s"), pMapWnd->GetUser());
	tmpS.TrimRight();
	tmpS.TrimLeft();
	memcpy(pmid->usrid, (char*)tmpS.operator LPCSTR(), tmpS.GetLength());

	if(gubn == 0)  //저장 
	{
		pmid->gbOption = 'S';

		for(int ii = 0 ; ii < m_listGroup.GetItemCount(); ii++)
		{
			tmpS = m_listGroup.GetItemText(ii, 2);
			tmpS.TrimRight();
			tmpS.TrimLeft();
			char* pdata = new char[21];
			memset(pdata, 0, 21);
			memset(pdata, ' ', 20);
			memcpy(pdata, (char*)tmpS.operator LPCSTR(), tmpS.GetLength());
			tmpS.Format(_T("%20s"), pdata);

			if(m_mapGroupck.Lookup(m_listGroup.GetItemText(ii, 2).TrimRight(), temp))
				tmpS += temp;
			else
				tmpS += "U";

			strgroup += tmpS;
		}	
		tmpS.Format(_T("%d"), m_listGroup.GetItemCount());
		tmpS.TrimLeft();
		tmpS.TrimRight();
		memset(pmid->attCnt, 0, 1);
		memcpy(pmid->attCnt, (char*)tmpS.operator LPCSTR(), 1);

		strsend.Format(_T("%s"), (char*)pmid, sizeof(struct _listorderemid));
		strsend = strsend.Left(sizeof(struct _listorderemid));
		strsend += strgroup;
	}
	else if(gubn == 1)  //조회
	{
		pmid->gbOption = 'V';
	}
	
	if(sendTR("PIBOPFRM", (char*)strsend.operator LPCSTR(), strsend.GetLength(), 2) == true)
		m_bSearching = true;
}

void CInterestPanel::oubInterListOrder(char* pdata)   //관심그룹순서
{//관심그룹 순서는 전체저장할때만 일어나며 그후에 그룹종목을 저장한다
	typedef struct _listorderemod
	{
		char filler[30];
		char gubun;
		char usrid[30];
		char gbOption;
		char successYN;
		char attCnt[4];
		char errMsg[200];
		char attGrpNm[20]; 
	}LISTORDERMOD;

	CString msg;
	m_bSearching = false;

	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	char* pbuf = (char*)pdata;
	struct _listorderemod* pmod =(struct _listorderemod*)&pdata[sizeof(struct  _ledgerH)];

	CString strsuccess, tmpS, strcode, strgroup;
	if(CString(pmod->successYN, 1).TrimRight().MakeUpper() != "Y")
	{
		msg.Format(_T("%s"), pmod->errMsg);
		msg.TrimRight();
		AfxMessageBox(msg);
		m_bSearching = false;
		return;
	}

	if(CString(pmod->gbOption, 1).TrimRight() == "S")
	{
		int icnt = atoi(CString(pmod->attCnt, 4).TrimRight());
		int pos = sizeof(_listorderemod);
		if(icnt <= 0)
			return;

		m_listGroup.DeleteAllItems();
		m_listCode.DeleteAllItems();
	//	m_mapGroupCodemap.RemoveAll();
		for(int ii = 0 ; ii < icnt; ii++)
		{
			char* pgroup = new char[21];
			memset(pgroup, ' ', 21);
			memcpy(pgroup, (char*)&pmod->attGrpNm[ii * 21], 21);
			tmpS.Format(_T("%20s"), pgroup);
			tmpS = tmpS.Left(20);
			tmpS.TrimRight();
			tmpS.TrimLeft();
			AddGroup(tmpS);
		}
	}
	else if(CString(pmod->gbOption, 1).TrimRight() == "V")
	{
		int icnt = atoi(CString(pmod->attCnt, 4).TrimRight());
		int pos = sizeof(_listorderemod);
		for(int ii = 0 ; ii < icnt; ii++)
		{
			char* pdata = new char[20];
			memset(pdata,  ' ' , 20);
			memcpy(pdata, (char*)&pmod[pos + (ii * 20)], 20);
		}
	}

	//POSITION pos;
	CString	 strkey;
	int i = 0;
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;

	for(int ii = 0 ; ii < m_listGroup.GetItemCount() ; ii++)
	{
		if(!CheckGpName(m_listGroup.GetItemText(ii, 2).TrimRight()))
			m_arrTrGrp.Add(m_listGroup.GetItemText(ii, 2).TrimRight());
	}

	InterListCodeTR(0, m_arrTrGrp.GetAt(0));
}

void CInterestPanel::InterListTR(int gubn, CString oldname, CString newname)
{//관심 그룹명 추가 0, 변경 1, 삭제 2
	typedef struct _listgrpmid
	{
		char usrid[30];
		char gubn;
		char oldname[20];
		char newname[20];
	}LISTMID;

	if(m_bSearching == true)
		return;

	CString tmpS;
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	char* psub = NULL;
	struct _listgrpmid  *pmid = new struct _listgrpmid;
	memset(pmid, ' ', sizeof(struct _listgrpmid));

	tmpS.Format(_T("%s"), pMapWnd->GetUser());
	tmpS.TrimRight();
	tmpS.TrimLeft();
	memcpy(pmid->usrid, (char*)tmpS.operator LPCSTR(), tmpS.GetLength());

	if(gubn == 0)  //추가 
		pmid->gubn = 'I';
	else if(gubn == 1)  //변경
		pmid->gubn = 'U';
	else if(gubn == 2)  //삭제
		pmid->gubn = 'D';

	memcpy(pmid->oldname, (char*)oldname.operator LPCSTR(), oldname.GetLength());
	memcpy(pmid->newname, (char*)newname.operator LPCSTR(), newname.GetLength());

	psub = (char*)pmid;
				
	if(sendTR("PIBOPFRM", psub, sizeof(struct _listgrpmid), 3) == true)
		m_bSearching = true;
}

void CInterestPanel::oubInterList(char* pdata)   //관심그룹리스트 추가변경삭제
{
	typedef struct _interlistmid
	{
		char gubun;   //서비스구분
		char usrid[30];  //사용자ID
		char gbOption;   //I 추가, U 변경, D:삭제
		char successYN;  //서비스 성공여부
		char attGrpNm[20]; //관심그룹명
		char errMsg[200];   //오류메시지
	}INTLISTMID;
	CString msg;
	m_bSearching = false;
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	CString strsuccess, tmpS, strgroup;
	struct _interlistmid* pmod = (struct _interlistmid*)&pdata[sizeof(struct  _ledgerH)];
	strsuccess = CString(pmod->successYN, 1);    //서비스 성공여부
	if(strsuccess != "Y")
	{
		msg.Format(_T("%s"), pmod->errMsg);
		msg.TrimRight();
		AfxMessageBox(msg);
		m_bSearching = false;
		return;
	}

	if(CString(pmod->gbOption, 1) == _T("D"))  //삭제인경우 
	{
		m_mapGroupCodemap.RemoveKey(CString(pmod->attGrpNm, 20).TrimRight());
		RemoveGroupInArr(CString(pmod->attGrpNm, 20).TrimRight());
		tmpS.Format(_T("%s\\user\\%s\\%s"), pMapWnd->GetRoot(), pMapWnd->GetName(), CString(pmod->attGrpNm, 20).TrimRight());
		for(int ii = 0 ; m_listGroup.GetItemCount() ; ii++)
		{
			if(m_listGroup.GetItemText(ii, 2) == CString(pmod->attGrpNm, 20).TrimRight())
			{
				m_listGroup.DeleteItem(ii);
				break;
			}
		}
	}
	else if(CString(pmod->gbOption, 1) == _T("I"))  //추가인경우 
	{
		AddGroup(CString(pmod->attGrpNm, 20).TrimRight());
	}
	else if(CString(pmod->gbOption, 1) == _T("U"))  //변경인경우 
	{
		strgroup = CString(pmod->attGrpNm, 20).TrimRight();
		CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
		if(m_mapGroupCodemap.Lookup(m_oldGrm, pmap))
		{
			m_mapGroupCodemap.RemoveKey(m_oldGrm);
			m_mapGroupCodemap.SetAt(strgroup,pmap);
			for(int ii = 0 ; m_listGroup.GetItemCount() ; ii++)
			{
				if(m_listGroup.GetItemText(ii, 2) == m_oldGrm)
				{
					m_listGroup.SetItemText(ii,2,strgroup);
					m_listGroup.SetItemState(ii, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
					m_listGroup.SetSelectionMark(ii);
					((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->SetWindowText(strgroup);
					break;
				}
			}
		}
	}
}

void CInterestPanel::DeleteAllInterList()
{//관심그룹 전체 삭제
	typedef struct _removeAllmid
	{
		char usrid[30];
		char gubn;
		char attCnt[4];
		char attGrpNm[20];
	}LISTMID;

	if(m_bSearching == true)
		return;

	CString tmpS, grpNm;

	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	char* psub = NULL;
	struct _removeAllmid  *pmid = new struct _removeAllmid;
	memset(pmid, ' ', sizeof(struct _removeAllmid));
	tmpS.Format(_T("%s"), pMapWnd->GetUser());
	tmpS.TrimRight();
	tmpS.TrimLeft();
	memcpy(pmid->usrid, (char*)tmpS.operator LPCSTR(), tmpS.GetLength());
	pmid->gubn = 'D';
	int cnt = 0;
	for(int ii = 0 ; ii < m_listGroup.GetItemCount(); ii++)
	{
		if(m_listGroup.GetCheck(ii))
		{
			cnt++;
			tmpS = m_listGroup.GetItemText(ii,2).TrimRight();
			char* pbuf = new char[21];
			memset(pbuf, 0, 21);
			memset(pbuf, ' ', 20);
			memcpy(pbuf, (char*)tmpS.operator LPCSTR(), tmpS.GetLength());
			tmpS.Format(_T("%s"), pbuf);
			grpNm += tmpS;
			//디폴트 그룹인지 여부
			grpNm += "N";
		}
	}

	if(cnt == 0)
		return;

	tmpS.Format(_T("%d"), cnt);
	tmpS.TrimLeft();
	tmpS.TrimRight();
	memcpy(pmid->attCnt, (char*)tmpS.operator LPCSTR(), 1);
	memcpy(pmid->attGrpNm, (char*)grpNm.operator LPCSTR(), grpNm.GetLength());

	psub = (char*)pmid;
				
	if(sendTR("PIBOPFRM", psub, sizeof(struct _removeAllmid) + (21 * cnt - 1), 4) == true)
	{
		m_bSearching = true;
	}
}

void CInterestPanel::oubRemoveAllgroup(char* pdata)
{//다수의 관심리스트 삭제 결과
	typedef struct _removeAllmod
	{
		char gubn[1];
		char usrid[30];
		char successYN[1];
		char errMsg[200];
	}LISTMID;

	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	char* pbuf = (char*)pdata;
	struct _removeAllmod* pmod =(struct _removeAllmod*)&pdata[sizeof(struct  _ledgerH)];

	CString strsuccess, tmpS, strcode, strgroup;
	if(CString(pmod->successYN, 1).TrimRight().MakeUpper() == "Y")
	{
		m_bSearching = false;
		m_listGroup.DeleteAllItems();
		m_listCode.DeleteAllItems();
		SearchInterList();
		return;
	}
	else
		m_bSearching = false;
}

void CInterestPanel::SearchInterList()
{
	typedef struct _listmid
	{
		char usrid[30];
	}LISTMID;

	if(m_bSearching == true)
		return;

	CString tmpS;

	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	char* psub = NULL;
	struct _listmid  *pmid = new struct _listmid;
	memset(pmid, ' ', sizeof(struct _listmid));
	tmpS.Format(_T("%s"), pMapWnd->GetUser());
	tmpS.TrimRight();
	tmpS.TrimLeft();
	memcpy(pmid->usrid, (char*)tmpS.operator LPCSTR(), tmpS.GetLength());
	psub = (char*)pmid;
				
	if(sendTR("PIBOPFRM", psub, sizeof(struct _listmid), 0) == true)
	{
		m_bSearching = true;
	}
}

void CInterestPanel::oubSearchList(char* pdata)
{
	//관심그룹 리스트 조회결과
	m_arrGroup.RemoveAll();
	m_bSearching = false;
	typedef struct _listmod
	{
		char fill[30];
		char gubun;     //서비스구분 1
		char usrid[30];  //사용자ID
		char succesYN;  //서비스성공여부 
		char attCnt[4];  //관심그룹갯수
		char errMsg[200]; //오류메시지
		char attGrpNm[20]; //관심그룹명 
		char gubn; //공통인지 사용자용인지 구분   C 공통 U 사용자
	}LISTMID;
	
	CString strsuccess, tmpS, temp, msg;

	strsuccess = CString(&pdata[sizeof(struct  _ledgerH) + 61], 1);    //서비스 성공여부
	if(strsuccess != "Y")
	{
		msg.Format(_T("%s"), CString(&pdata[sizeof(struct  _ledgerH) + 36], 200));
		msg.TrimRight();
		AfxMessageBox(msg);
		m_bSearching = false;
		return;
	}

	int cnt = atoi(CString(&pdata[sizeof(struct  _ledgerH) + 62], 4));   //그룹갯수

	if(cnt <= 0)
		return;

	m_arrGroup.RemoveAll();
	m_arrTrGrp.RemoveAll();
	m_listGroup.DeleteAllItems();
	m_listCode.DeleteAllItems();
	m_mapGroupCodemap.RemoveAll();
	m_mapGroupck.RemoveAll();

	int ipos = sizeof(struct  _ledgerH) + 1 + 60 + 1+ 4 + 200;
	for(int ii = 0 ; ii < cnt ; ii++)
	{
		temp = CString(&pdata[ipos + (ii * 21)], 21);
		tmpS = temp.Left(20);
		tmpS.TrimRight();
		tmpS.TrimLeft();
		m_arrGroup.Add(tmpS);  
		m_mapGroupck.SetAt(tmpS, temp.Right(1));

		AddGroup(tmpS);
		if(!CheckGpName(tmpS))
			m_arrTrGrp.Add(tmpS);
	}

	if(m_arrTrGrp.GetSize() > 0)
	{
		InterListCodeTR(1, m_arrTrGrp.GetAt(0));
	}
}

bool CInterestPanel::sendTR(CString trC, char* pBytes, int nBytes, int gubn)
{
	if(m_bSearching)
		return false;

	CString strTRData, tmpS ;
	int nOutLen = 0;
	struct _userTH user;
	FillMemory(&user, sizeof(struct  _userTH), ' ');
	memcpy(user.trc, (LPSTR)(LPCTSTR)trC, trC.GetLength());
	user.key  = 999;
	user.stat = US_KEY;

	strTRData += CString((char*)&user, sizeof(_userTH));
	nOutLen = strTRData.GetLength();

	struct  _ledgerH ledgerH;	
	FillMemory(&ledgerH, sizeof(struct  _ledgerH), ' ');
	GetledgerH(&ledgerH, gubn);

	tmpS = CString((char *)&ledgerH, sizeof(struct  _ledgerH));
	strTRData += tmpS;

	strncpy((char*)tmpS.operator LPCSTR(), pBytes, nBytes);
	strTRData += CString(pBytes, nBytes);

	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	KillTimer(9999);
	SetTimer(9999, 3000, NULL);

	if(pMapWnd->GetWizard()->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strTRData.GetLength() - nOutLen), (LPARAM)(LPCTSTR)strTRData))
		return true;
	else
		return false;
}

void CInterestPanel::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 9999:
		{
			KillTimer(nIDEvent);
			m_bSearching = false;
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CInterestPanel::EnableBtn(bool bEnable)
{
	m_btnAddGroup.EnableWindow(bEnable);
	m_btnCDown.EnableWindow(bEnable);
	m_btnCRemove.EnableWindow(bEnable);
	m_btnCUp.EnableWindow(bEnable);
	m_btnGDown.EnableWindow(bEnable);
	m_btnGRemove.EnableWindow(bEnable);
	m_btnGroup.EnableWindow(bEnable);
	m_btnGUp.EnableWindow(bEnable);
	m_btnSearch.EnableWindow(bEnable);
	m_btnSave.EnableWindow(bEnable);
	m_btnCancel.EnableWindow(bEnable);
	m_btnAllLoad.EnableWindow(bEnable);
	m_btnAllSave.EnableWindow(bEnable);
	Invalidate();
}

void CInterestPanel::OnBnClickedButton3() //test용버튼
{
	CString strResult, tmpS;

	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	CString	 strkey;
	POSITION pos, mappos;
	int cnt = 1;
	for (pos = m_mapGroupCodemap.GetStartPosition(); pos; )
	{
		m_mapGroupCodemap.GetNextAssoc(pos, strkey, pmap);
		CCodeData cd;
		tmpS.Format(_T("%d="), cnt);
		strResult += tmpS;
		strResult += strkey;
		strResult += _T("\t");
		for (mappos = pmap->GetStartPosition(); mappos;)
		{
			pmap->GetNextAssoc(mappos, strkey, cd);
			strResult += cd.strCode;
			strResult += _T(" ");
		}
		strResult += _T("\n");
		cnt++;
	}
	
	AfxMessageBox(strResult);
}

void CInterestPanel::OnBnClickedButton1()  //test용버튼
{
	CString tmpS;
	((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->GetWindowText(tmpS);

	tmpS.TrimLeft();
	tmpS.TrimRight();
	if(tmpS.IsEmpty())
		return;
	InterListOrderSave(0);
}

void CInterestPanel::OnBnClickedButton2() //test용버튼
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SearchInterList();
}

void CInterestPanel::OnBnClickedBtnCup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CInterestPanel::OnBnClickedBtnCdown()
{
	
}

void CInterestPanel::OnBnClickedButton4()  //test용버튼
{
	m_arrTrGrp.RemoveAll();
	InterListCodeTR(0, GetGroupFromEdit());
}

void CInterestPanel::OnBnClickedAllSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

BOOL CInterestPanel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)	
	{
		if(GetFocus()->m_hWnd == ((CWnd*)GetDlgItem(IDC_EDIT_GROUP))->m_hWnd)
		{
			OnBnClickedBtnAddgroup();
			return true;
		}	
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

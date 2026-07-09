// agencyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AxisCode.h"
#include "agencyDlg.h"
#include "afxdialogex.h"

// agencyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(agencyDlg, CDialogEx)

	
int CALLBACK ListViewCompareFunc5(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{ 
	CString tmpS, tempS;
	_aglistItem *pItem1 = (_aglistItem *) lParam1; 
	_aglistItem *pItem2 = (_aglistItem *) lParam2; 
	
	int	iCmp=0;
	
	switch ((int)lParamSort)
	{
		case 0:
			iCmp = lstrcmpi((char*)pItem1->Name, (char*)pItem2->Name);
			break;
		case 1:
			iCmp = lstrcmpi((char*)pItem1->Code, (char*)pItem2->Code); 			
			break;
		case 2:
			tmpS.Format(_T("%.14s"), (char*)pItem1->balcode);
			tmpS.Replace(_T("-"), _T(""));
			tempS.Format(_T("%s"), (char*)pItem2->balcode);
			tempS.Replace(_T("-"), _T(""));
			//iCmp = lstrcmpi((char*)pItem1->balcode, (char*)pItem2->balcode);
			//iCmp = lstrcmpi((char*)tmpS.operator LPCSTR(), (char*)tempS.operator LPCSTR());
			if(atof(tmpS) > atof(tempS))
				iCmp = -1;
			else
				iCmp = 1;

			break;
	}

	if (m_agsort)
	{
		if (iCmp > 0)
			iCmp = -1;
		else if (iCmp < 0)
			iCmp = 1;
	}
	
	return iCmp;
}

agencyDlg::agencyDlg(CPoint pt, CWnd* pParent /*=NULL*/)
	: CDialogEx(agencyDlg::IDD, pParent)
{
	m_pMain = pParent;
	m_Point = pt;
	m_agsort = false;
	m_pCtlBrush = NULL;
	m_iSelectRow = -1;
}

agencyDlg::agencyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(agencyDlg::IDD, pParent)
{
	
}

agencyDlg::~agencyDlg()
{
}

void agencyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_AGENCY, m_list);
	DDX_Control(pDX, IDC_EDIT_CODE, m_Search);
}

BEGIN_MESSAGE_MAP(agencyDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AGENCY, &agencyDlg::OnDblclkListAgency)
	ON_EN_CHANGE(IDC_EDIT_CODE, &agencyDlg::OnChangeEditCode)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_AGENCY, &agencyDlg::OnCustomdrawListAgency)
	ON_NOTIFY(NM_CLICK, IDC_LIST_AGENCY, &agencyDlg::OnClickListAgency)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// agencyDlg 메시지 처리기입니다.


CString agencyDlg::Parser(CString &strSrc, CString strSub /* = _T("\t") */)
{
	if (strSrc.Find(strSub) == -1)
	{
		CString temp = strSrc;
		strSrc.Empty();
			return temp;
	}
	else
	{
		CString  strTemp = strSrc.Left(strSrc.Find(strSub));
		strSrc = strSrc.Mid(strSrc.Find(strSub) + strSub.GetLength());
		return strTemp;
	}

	return _T("");
}

void agencyDlg::ResetAuthAg()
{
	CString tmpS, temp;
	m_mapAg.RemoveAll();
	
	CString strUserPath(_T(""));
	CString strFilePath(_T(""));

	m_sRoot = ((CAxisCodeApp*)AfxGetApp())->m_RootPath;
	m_sUser = ((CAxisCodeApp*)AfxGetApp())->m_Name;
	
	strUserPath.Format(_T("%s\\user\\%s"), m_sRoot, m_sUser);

	strFilePath.Format(_T("%s\\%s"), strUserPath, _T("authcompany.ini"));

	WritePrivateProfileString(_T("auth"), _T("company"), "", (LPCTSTR)strFilePath);
	WritePrivateProfileString(_T("auth"), _T("chechYN"), "", (LPCTSTR)strFilePath);

	WritePrivateProfileString(_T("auth"), _T("listcompany"), "", (LPCTSTR)strFilePath);
	WritePrivateProfileString(_T("auth"), _T("balchechYN"), "", (LPCTSTR)strFilePath);
	
	tmpS.ReleaseBuffer();
}

void agencyDlg::LoadAuthAg()
{
	CString tmpS, temp;
	m_mapAg.RemoveAll();
	
	CString strUserPath(_T(""));
	CString strFilePath(_T(""));

	m_sRoot = ((CAxisCodeApp*)AfxGetApp())->m_RootPath;
	m_sUser = ((CAxisCodeApp*)AfxGetApp())->m_Name;
	
	strUserPath.Format(_T("%s\\user\\%s"), m_sRoot, m_sUser);

	strFilePath.Format(_T("%s\\%s"), strUserPath, _T("authcompany.ini"));

	//2자리 발행코드
	GetPrivateProfileString(_T("auth"), _T("company"), _T(""), tmpS.GetBufferSetLength(1024), 1024, (LPCTSTR)strFilePath);
	tmpS.ReleaseBuffer();
	tmpS.TrimRight();

	if(!tmpS.IsEmpty())
	{
		while(1)
		{
			temp.Format(_T("%02s"), Parser(tmpS));
			m_mapAg.SetAt(temp, tmpS);
			if(tmpS.IsEmpty())
				break;
		}
	}

	//발행여부에 대한 Y,N
	tmpS.Empty();
	GetPrivateProfileString(_T("auth"), _T("chechYN"), _T(""), tmpS.GetBufferSetLength(1024), 1024, (LPCTSTR)strFilePath);
	tmpS.ReleaseBuffer();
	tmpS.TrimRight();
	if(tmpS.IsEmpty())
		m_btype = false;
	else
	{
		m_btype = tmpS == "Y" ? true : false;
	}

	GetPrivateProfileString(_T("auth"), _T("listcompany"), _T(""), tmpS.GetBufferSetLength(1024), 1024, (LPCTSTR)strFilePath);
	tmpS.ReleaseBuffer();
	tmpS.TrimRight();

	if(!tmpS.IsEmpty())
	{
		while(1)
		{
			temp.Format(_T("%06s"), Parser(tmpS));
			temp.TrimLeft();
			temp.TrimRight();
			m_mapCodeAg.SetAt(temp, temp);
			if(tmpS.IsEmpty())
				break;
		}
	}

	//6자리 회사코드를 다이알로그에 어떻게 표현 할지에 대한  Y <-- 파일에 명시된 회사 코드만 보여라   N <- 파일에 명시된 회사코드만 빼라
	tmpS.Empty();
	GetPrivateProfileString(_T("auth"), _T("balchechYN"), _T(""), tmpS.GetBufferSetLength(1024), 1024, (LPCTSTR)strFilePath);
	tmpS.ReleaseBuffer();
	tmpS.TrimRight();
	if(tmpS.IsEmpty())
		m_bshowtype = false;
	else
	{
		m_bshowtype = tmpS == "Y" ? true : false;
	}
}

BOOL agencyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_pCtlBrush =  new CBrush();
	m_pCtlBrush->CreateSolidBrush(RGB(249,249,249));


	SetDlgPos(m_Point.x , m_Point.y);

	//사용자권한에 따른 회사명
	LoadAuthAg();
	
	InitList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void agencyDlg::SetDlgPos(int x, int y)
{
	if(x < 0 || y < 0)
		return;

	int scWidth, scHeight;
	scWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CRect rc;
	GetClientRect(&rc);

	int gabX = scWidth - (x - rc.Width());
	int	gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(NULL, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}

bool agencyDlg::checkUpcode(AGCODE* agcode) //2자리 회사코드 체크
{
	CString temp, tmpS;
	temp.Format(_T("%.2s"), agcode->up);
	temp.TrimLeft();
	temp.TrimRight();

	if(!m_mapAg.Lookup(temp, tmpS) && m_mapAg.GetSize() > 0)
	  return false;

	if(m_btype == true && agcode->gubn == 'N')  //m_btype 발행사 보여주기 여부 Y 이면 발행여부가 Y인 종목만 보여주고 N 이면 그냥 다 보여준다
		return false;

	return true;
}

bool agencyDlg::checkAgcode(AGCODE* agcode)   //6자리 회사코드 체크
{
	CString temp, tmpS;
	temp.Format(_T("%.6s"), agcode->code);
	temp.TrimRight();
	temp.TrimLeft();

	if(m_bshowtype)   //Y  <-- m_mapCodeAg 에 저장된 회사만 보여야 한다
	{
		if(!m_mapCodeAg.Lookup(temp, tmpS) && m_mapCodeAg.GetSize() > 0)
			return false;
	}
	else   //N  <-- m_mapCodeAg 에 저장된 회사만 빼야 한다
	{
		if(m_mapCodeAg.Lookup(temp, tmpS) && m_mapCodeAg.GetSize() > 0)
			return false;;
	}

	return true;
}

void agencyDlg::InitList()
{	
	if(m_btype == true)
		SetWindowText(_T("발행사검색"));
	else
		SetWindowText(_T("회사검색"));

	CString tmpS, temp;
	CRect rc;
	m_list.GetClientRect(&rc);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() |  LVS_EX_FULLROWSELECT);

	if(m_btype == true)
	{
		m_list.InsertColumn(0,_T("발행사"), LVCFMT_LEFT, 190);
		m_list.InsertColumn(1, _T("발행사코드"), LVCFMT_LEFT,75);
		m_list.InsertColumn(2, _T("법인번호"), LVCFMT_LEFT, 100);
		m_list.InsertColumn(3, _T("업종"), LVCFMT_LEFT, 0);
		m_list.InsertColumn(4, _T("발행여부"), LVCFMT_LEFT, 0);
	}
	else
	{
		m_list.InsertColumn(0,_T("회사명"), LVCFMT_LEFT, 200);
		m_list.InsertColumn(1, _T("회사코드"), LVCFMT_LEFT, 65);
		m_list.InsertColumn(2, _T("법인번호"), LVCFMT_LEFT, 100);
		m_list.InsertColumn(3, _T("업종"), LVCFMT_LEFT, 0);
		m_list.InsertColumn(4, _T("발행여부"), LVCFMT_LEFT, 0);

	}
	//	m_mapCodeAg m_bshowtype
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	AGCODE* agcode = NULL;
	int nCount = (int)pApp->m_arrayAGCode.GetSize();
	m_list.DeleteAllItems();
	int icnt = 0;
	for(int ii = 0 ; ii < nCount ;ii++)
	{
		agcode = pApp->m_arrayAGCode.GetAt(ii);
		
		if(!checkUpcode(agcode))
			continue;

		if(!checkAgcode(agcode))
			continue;

		tmpS.Format(_T("%.50s"), agcode->name);
		tmpS.TrimRight();
		m_list.InsertItem(icnt, tmpS);

		tmpS.Format(_T("%.6s"), agcode->code);
		tmpS.TrimRight();
		m_list.SetItemText(icnt, 1, tmpS);

		tmpS.Format(_T("%.14s"), agcode->balcode);
		tmpS.TrimRight();
		m_list.SetItemText(icnt, 2, tmpS);

		tmpS.Format(_T("%.2s"), agcode->up);
		tmpS.TrimRight();
		m_list.SetItemText(icnt, 3, tmpS);

		tmpS.Format(_T("%c"), agcode->gubn);
		tmpS.TrimRight();
		m_list.SetItemText(icnt, 4, tmpS);

		AGLISTITEM* pitem;
		pitem = new AGLISTITEM;
		strncpy((char*)pitem->Name, agcode->name,50);

		tmpS.Format(_T("%.6s"), agcode->code);
		strncpy((char*)pitem->Code, agcode->code, tmpS.GetLength());

		tmpS.Format(_T("%.14s"), agcode->balcode);
		tmpS.TrimRight();
		strncpy((char*)pitem->balcode,(char*)tmpS.operator LPCSTR(), tmpS.GetLength());
	
		m_list.SetItemData(icnt, (LPARAM)pitem);
		icnt++;
	}
	ResetAuthAg();
}

HBRUSH agencyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_STATIC:
			pDC->SetBkColor(RGB(249,249,249));
			hbr = m_pCtlBrush->operator HBRUSH();	
			break;
	}
	return hbr;
}

BOOL agencyDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(rc, RGB(249,249,249));
	return true;
}

void agencyDlg::OnDblclkListAgency(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int item = m_list.GetSelectionMark();

	CString tmpS;
	m_szCode = m_szName = "";
	m_szCode = m_list.GetItemText(item, 1);
	m_szName = m_list.GetItemText(item, 0);
	OnOK();	

	*pResult = 0;
}

BOOL agencyDlg::IsNumeric(CString strVal)
{
	char* pLast = NULL;
	char* pNext = NULL;

	if (strVal.IsEmpty())
		return FALSE;

	pLast = (LPSTR)(LPCTSTR)strVal + strVal.GetLength();
	strtol((LPCTSTR)strVal, &pNext, 10);

	return (pNext == pLast);
}

int agencyDlg::DeleteSpace(char* pBuff, int nLen)
{
	char szTemp[128];

	int j = 0;

	for (int i = 0; i < nLen; i++)
	{
		if (pBuff[i] != 0x20)
			szTemp[j++] = pBuff[i];
	}

	CopyMemory(pBuff, szTemp, j);
	pBuff[j] = 0x00;

	return j;
}


BOOL agencyDlg::FindCode(CString szCode)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int nCount = (int)pApp->m_arrayAGCode.GetSize();
	AGCODE *code;
	CString tmps;
	
	for (int ii = 0; ii < nCount; ii++)
	{
		code = pApp->m_arrayAGCode.GetAt(ii);

		tmps.Format(_T("%.6s"), code->code);
		tmps.TrimRight();
		tmps.TrimLeft();
		szCode.MakeUpper();
		if (szCode == tmps)
		{
			SetName(code->name);
			return TRUE;
		}
	}

	return FALSE;
}

void agencyDlg::OnChangeEditCode()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CString strSearch(_T(""));
	m_Search.GetWindowText(strSearch);

	strSearch.MakeUpper();			// 2009/08/31 KJH ADD - UPPERCASE

	CString strSrc(_T(""));
	int nIndex = -1;

	BOOL bNumeric = IsNumeric((LPCTSTR)strSearch.Left(3));
	BOOL bFind = FALSE;
	bool isHan = false;

	char* test = (LPSTR)strSearch.operator LPCTSTR();
	CString tmpS = _T("");

	TRACE(tmpS);
	for (int jj = 0; jj < strSearch.GetLength(); jj++)
	{
		if (test[jj] & 0x80)
			isHan = true;
	}

	_TRIMDATA(strSearch);
	int nLen = DeleteSpace((LPSTR)(LPCTSTR)strSearch, strSearch.GetLength());
	
	int nCount = m_list.GetItemCount();
	
	for (int i = 0; i < nCount; i++)
	{
		strSrc = m_list.GetItemText(i, 0);  //0 종목명 1 종목코드
		
		if (strSrc.IsEmpty())
			continue;
	
		strSrc.MakeUpper();
		if(strSrc.Find(strSearch) >= 0)
		{
			nIndex = i;
			break;
		}
	}
	
	if (nIndex == -1)
			return;

	POSITION pos = m_list.GetFirstSelectedItemPosition();
	
	while (pos)
	{
		int nItem = m_list.GetNextSelectedItem(pos);
		m_list.SetItemState(nItem, 0, LVIS_SELECTED);
	}

	m_list.SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_list.SetSelectionMark(nIndex);
	//m_list.SetFocus();
	m_iSelectRow = nIndex;

	CRect rcItem;
	m_list.GetItemRect(0, &rcItem, LVIR_LABEL);
	CSize sz(0, rcItem.Height() * (nIndex - m_list.GetTopIndex()));
				
	m_list.Scroll(sz);

	if(strSearch.GetLength() == 0)
	{
		m_iSelectRow = -1;
		Invalidate();
	}
}


BOOL agencyDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch(((LPNMHDR) lParam)->code)
 	{
 		case LVN_COLUMNCLICK:
 		#define pnm ((LPNMLISTVIEW)lParam)
		switch ((LPARAM)pnm->iSubItem)
		{
 		case 0:
 			m_agsort = !m_agsort;
 			break;
 		case 1:
 			m_agsort = !m_agsort;
			break;
		case 2:
 			m_agsort = !m_agsort;
			break;
 		}
			
		ListView_SortItems((HWND)pnm->hdr.hwndFrom, ListViewCompareFunc5, (LPARAM)pnm->iSubItem);
 			
 		#undef pnm

		break;
 	}
	return CDialogEx::OnNotify(wParam, lParam, pResult);
}


void agencyDlg::OnCustomdrawListAgency(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = (LRESULT)CDRF_NOTIFYITEMDRAW;
		return;
	}

	if(pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
	{
		if(pNMCD->dwItemSpec == m_iSelectRow)
		{
			NMLVCUSTOMDRAW *pDraw = (NMLVCUSTOMDRAW*)(pNMHDR);
			pDraw->clrText = 0xffffff;
			pDraw->clrTextBk = RGB(51,153,255);
			*pResult = (LRESULT)CDRF_NOTIFYITEMDRAW;
			return;
		}
		else
		{
			NMLVCUSTOMDRAW *pDraw = (NMLVCUSTOMDRAW*)(pNMHDR);
			pDraw->clrText = RGB(0,0,0);
			pDraw->clrTextBk = RGB(255,255,255);
			*pResult = (LRESULT)CDRF_NOTIFYITEMDRAW;
			return;
		}
	}
	*pResult = 0;
}


void agencyDlg::OnClickListAgency(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iSelectRow = -1;
	Invalidate();
	*pResult = 0;
}


void agencyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

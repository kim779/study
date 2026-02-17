// columns.cpp : implementation file
//

#include "stdafx.h"
#include "axisdialog.h"
#include "columns.h"
#include "../../h/axisvar.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	cTAB	'\t'
#define	cSEP	'|'

#define INIT		"8BTN.BMP"
#define INIT_DN		"8BTN_DN.BMP"
#define INIT_EN		"8BTN_EN.BMP"
#define TOP		"최상위.BMP"
#define TOP_DN		"최상위_DN.BMP"
#define TOP_EN		"최상위_EN.BMP"
#define UP		"상위.BMP"
#define UP_DN		"상위_DN.BMP"
#define UP_EN		"상위_EN.BMP"
#define DOWN		"하위.BMP"
#define DOWN_DN		"하위_DN.BMP"
#define DOWN_EN		"하위_EN.BMP"
#define BOTTOM		"최하위.BMP"
#define BOTTOM_DN	"최하위_DN.BMP"
#define BOTTOM_EN	"최하위_EN.BMP"
#define OK		"2BTN.BMP"
#define OK_DN		"2BTN_DN.BMP"
#define OK_EN		"2BTN_EN.BMP"
#define CANCEL		"2BTN.BMP"
#define CANCEL_DN	"2BTN_DN.BMP"
#define CANCEL_EN	"2BTN_EN.BMP"
/////////////////////////////////////////////////////////////////////////////
// CColumns dialog


CColumns::CColumns(CWnd* pParent /*=NULL*/)
	: CDialog(CColumns::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColumns)
	//}}AFX_DATA_INIT

	m_columns.RemoveAll();
	m_items.RemoveAll();
	m_defs.RemoveAll();

	m_dragImage	= NULL;
	m_dragWnd	= NULL;
	m_selindex	= -1;
	m_pFont		= NULL;

	m_BtnInit	= NULL;
	m_BtnOk		= NULL;
	m_BtnCancel	= NULL;
	m_BtnTop	= NULL;
	m_BtnUp		= NULL;
	m_BtnDown	= NULL;
	m_BtnBottom	= NULL;
	m_sItemList	= "";
	m_sRoot		= "";
}

CColumns::CColumns(CString sRoot, CString src, CString items, CString defs, CWnd* pParent)
	: CDialog(CColumns::IDD, pParent)
{
	parse(src, items, defs);
	m_dragImage	= NULL;
	m_dragWnd	= NULL;
	m_sRoot		= sRoot;
}

void CColumns::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColumns)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CColumns, CDialog)
	//{{AFX_MSG_MAP(CColumns)
	ON_BN_CLICKED(IDC_TOP, OnTop)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_BOTTOM, OnBottom)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST, OnBeginDragAllList)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_INIT, OnInit)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CColumns::OnClickList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CColumns::OnItemchangedList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CColumns::OnCustomdrawList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColumns message handlers

BOOL CColumns::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_list.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

	addColumn(m_list, _T("전체 항목"), 0, 112);
	addColumn(m_list, _T("symbol"), 1, 0);
	CString sData, sKey;
	CStringArray array;
	int	nPos = 0;
	m_list.DeleteAllItems();
	
	m_sImageDir.Format("%s\\%s", m_sRoot, IMAGEDIR);

	InitControl();

	for (int ii = 0; ii < m_items.GetSize(); ii++)
	{
		CString str = m_items.GetAt(ii);
		if (m_columns.Lookup(m_items.GetAt(ii), sData))
		{
			addItem(sData, m_items.GetAt(ii), nPos);
			m_list.SetCheck(nPos);
			nPos++;
		}
	}

	for (POSITION pos = m_columns.GetStartPosition(); pos; )
	{
		m_columns.GetNextAssoc(pos, sKey, sData);
		if (IsExist(m_items, sKey) == false)
		{
			addItem(sData, sKey, nPos);
			m_list.SetCheck(nPos, FALSE);
			nPos++;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CColumns::IsExist(CStringArray & array, CString sKey)
{
	for (int ii = 0; ii < array.GetSize(); ii++)
	{
		if (sKey == array.GetAt(ii))
			return true;
	}
	
	return false;
}

void CColumns::InitControl()
{
	SetBitmaps();
	CreateFont();

	m_BtnInit = new CfxImgBtn();
	m_BtnInit->Create("기본필드", CRect(10, 50, 70, 72), this, IDC_INIT, false, true);
	m_BtnInit->SetImgBitmap(HbmInit, HbmInit_dn, HbmInit_en);
	m_BtnInit->SetFont(m_pFont);
	m_BtnInit->SetTextColor(RGB(0,0,0));

	m_BtnOk = new CfxImgBtn();
	m_BtnOk->Create("확 인\n확 인", CRect(50, 312, 100, 338), this, IDC_OK, false, true);
	m_BtnOk->SetImgBitmap(HbmOk, HbmOk_dn, HbmOk_en);
	m_BtnOk->SetFont(m_pFont);
	m_BtnOk->SetTextColor(RGB(0,0,0));
	
	m_BtnCancel = new CfxImgBtn();
	m_BtnCancel->Create("취 소\n취 소", CRect(130, 312, 180, 338), this, IDC_CANCEL, false, true);
	m_BtnCancel->SetImgBitmap(HbmCancel, HbmCancel_dn, HbmCancel_en);
	m_BtnCancel->SetFont(m_pFont);
	m_BtnCancel->SetTextColor(RGB(0,0,0));
	
	m_BtnTop = new CfxImgBtn();
	m_BtnTop->Create("맨위로", CRect(185, 90, 245, 115), this, IDC_TOP, false, true);
	//m_BtnTop->SetImgBitmap(HbmTop, HbmTop_dn, HbmTop_en);
	m_BtnTop->SetImgBitmap(HbmInit, HbmInit_dn, HbmInit_en);
	m_BtnTop->SetFont(m_pFont);
	m_BtnTop->SetTextColor(RGB(0,0,0));
	
	m_BtnUp = new CfxImgBtn();
	m_BtnUp->Create("위로", CRect(185, 120, 245, 145), this, IDC_UP, false, true);
	//m_BtnUp->SetImgBitmap(HbmUp, HbmUp_dn, HbmUp_en);
	m_BtnUp->SetImgBitmap(HbmInit, HbmInit_dn, HbmInit_en);
	m_BtnUp->SetFont(m_pFont);
	m_BtnUp->SetTextColor(RGB(0,0,0));
		
	m_BtnDown = new CfxImgBtn();
	m_BtnDown->Create("아래로", CRect(185, 150, 245, 175), this, IDC_DOWN, false, true);
	//m_BtnDown->SetImgBitmap(HbmDown, HbmDown_dn, HbmDown_en);
	m_BtnDown->SetImgBitmap(HbmInit, HbmInit_dn, HbmInit_en);
	m_BtnDown->SetFont(m_pFont);
	m_BtnDown->SetTextColor(RGB(0,0,0));

	m_BtnBottom = new CfxImgBtn();
	m_BtnBottom->Create("맨아래로", CRect(185, 180, 245, 205), this, IDC_BOTTOM, false, true);
	//m_BtnBottom->SetImgBitmap(HbmBottom, HbmBottom_dn, HbmBottom_en);
	m_BtnBottom->SetImgBitmap(HbmInit, HbmInit_dn, HbmInit_en);
	m_BtnBottom->SetFont(m_pFont);
	m_BtnBottom->SetTextColor(RGB(0,0,0));
}

void CColumns::CreateFont()
{
	m_pFont = new CFont;
	m_pFont->CreatePointFont(90,_T("굴림체"));
}

void CColumns::SetBitmaps()
{
	CString sPath;
	sPath.Format("%s\\%s", m_sImageDir, INIT);		bmInit = CreateBmp(sPath);	HbmInit = bmInit->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, INIT_DN);		bmInit_dn = CreateBmp(sPath);	HbmInit_dn = bmInit_dn->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, INIT_EN);		bmInit_en = CreateBmp(sPath);	HbmInit_en = bmInit_en->operator HBITMAP();
	
	sPath.Format("%s\\%s", m_sImageDir, INIT);		bmTop = CreateBmp(sPath);	HbmTop = bmTop->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, INIT_DN);		bmTop_dn = CreateBmp(sPath);	HbmTop_dn = bmTop_dn->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, INIT_EN);		bmTop_en = CreateBmp(sPath);	HbmTop_en = bmTop_en->operator HBITMAP();
	
	sPath.Format("%s\\%s", m_sImageDir, INIT);		bmUp = CreateBmp(sPath);	HbmUp = bmUp->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, INIT_DN);		bmUp_dn = CreateBmp(sPath);	HbmUp_dn = bmUp_dn->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, INIT_EN);		bmUp_en = CreateBmp(sPath);	HbmUp_en = bmUp_en->operator HBITMAP();
	
	sPath.Format("%s\\%s", m_sImageDir, INIT);		bmDown = CreateBmp(sPath);	HbmDown = bmDown->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, INIT_DN);		bmDown_dn = CreateBmp(sPath);	HbmDown_dn = bmDown_dn->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, INIT_EN);		bmDown_en = CreateBmp(sPath);	HbmDown_en = bmDown_en->operator HBITMAP();
	
	sPath.Format("%s\\%s", m_sImageDir, INIT);		bmBottom = CreateBmp(sPath);	HbmBottom = bmBottom->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, INIT_DN);		bmBottom_dn = CreateBmp(sPath);	HbmBottom_dn = bmBottom_dn->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, INIT_EN);		bmBottom_en = CreateBmp(sPath);	HbmBottom_en = bmBottom_en->operator HBITMAP();
	
	sPath.Format("%s\\%s", m_sImageDir, OK);		bmOk = CreateBmp(sPath);	HbmOk = bmOk->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, OK_DN);		bmOk_dn = CreateBmp(sPath);	HbmOk_dn = bmOk_dn->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, OK_EN);		bmOk_en = CreateBmp(sPath);	HbmOk_en = bmOk_en->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, CANCEL);		bmCancel = CreateBmp(sPath);	HbmCancel = bmCancel->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, CANCEL_DN);		bmCancel_dn = CreateBmp(sPath);	HbmCancel_dn = bmCancel_dn->operator HBITMAP();
	sPath.Format("%s\\%s", m_sImageDir, CANCEL_EN);		bmCancel_en = CreateBmp(sPath);	HbmCancel_en = bmCancel_en->operator HBITMAP();
}

CBitmap* CColumns::CreateBmp( LPCTSTR sBMPFile)
{
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead) )
		return NULL;

	BITMAPFILEHEADER bmfHeader;

	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return NULL;

	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return NULL;

	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = file.GetLength() - sizeof(BITMAPFILEHEADER);
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
		return NULL;

	// Read the remainder of the bitmap file.
	//if (file.ReadHuge((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	if (file.Read((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	{
		::GlobalFree(hDIB);
		file.Close();
		return NULL;
	}
	file.Close();

	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed :
						1 << bmiHeader.biBitCount;

	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) +
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	CClientDC dc(NULL);

	HBITMAP hBmp = CreateDIBitmap( dc.m_hDC, &bmiHeader, CBM_INIT, lpDIBBits, &bmInfo, DIB_RGB_COLORS);
	CBitmap *bitmap;
	bitmap = new CBitmap();
	bitmap->Attach( hBmp );

	::GlobalFree(hDIB);
	return bitmap;
}

void CColumns::OnTop() 
{
	int	nItem;
	CString	text, key;
	CString	gname, gno;
	BOOL	bCheck = TRUE;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = m_list.GetNextSelectedItem(pos);
		if (nItem != 0)
		{
			text = m_list.GetItemText(nItem, 0);
			key = m_list.GetItemText(nItem, 1);
			bCheck = m_list.GetCheck(nItem);
			m_list.DeleteItem(nItem);

			if(m_list.GetItemText(0, 0) == "선택")
				nItem = addItem(text, key, 1);
			else
				nItem = addItem(text, key, 0);

			m_list.SetCheck(nItem, bCheck);
						
			KillSelected();
			SetSelected(nItem, FALSE);
		}
	}
}

void CColumns::OnUp() 
{
	int	nItem;
	CString	text, key;
	CString	gname, gno;
	BOOL	bCheck = TRUE;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = m_list.GetNextSelectedItem(pos);

		if (nItem - 1 == 0 && m_list.GetItemText(nItem -1, 0) == "선택")
		{
			m_slog.Format("[axisdialog][%s]<%d> nItem=[%d]  nItem - 1 = 0 return  ]",
				__FUNCTION__, __LINE__, nItem);
			OutputDebugString(m_slog);
			return;
		}

		if (nItem != 0)
		{
			text = m_list.GetItemText(nItem, 0);
			key = m_list.GetItemText(nItem, 1);
			bCheck = m_list.GetCheck(nItem);

			m_slog.Format("[axisdialog][%s]<%d> nItem=[%d] 컬럼헤더=[%s]  컬럼Name=[%s] bCheck=[%d]",
				__FUNCTION__, __LINE__, nItem, text, key, bCheck);
			OutputDebugString(m_slog);

			m_list.DeleteItem(nItem);

			nItem = addItem(text, key, nItem - 1);
			m_list.SetCheck(nItem, bCheck);
			
			KillSelected();
			SetSelected(nItem, FALSE);
		}
	}
}

void CColumns::OnDown() 
{
	int	nItem;//, data;
	CString	text, key;
	CString	gname, gno;
	BOOL	bCheck = TRUE;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = m_list.GetNextSelectedItem(pos);

		if (nItem  == 0 && m_list.GetItemText(nItem, 0) == "선택")
		{
			m_slog.Format("[axisdialog][%s]<%d> nItem=[%d]  nItem  = 0 return  ]",
				__FUNCTION__, __LINE__, nItem);
			OutputDebugString(m_slog);
			return;
		}

		if (nItem != m_list.GetItemCount())
		{
			text = m_list.GetItemText(nItem, 0);
			key = m_list.GetItemText(nItem, 1);
			bCheck = m_list.GetCheck(nItem);
			m_list.DeleteItem(nItem);

			nItem = addItem(text, key, nItem + 1);
			m_list.SetCheck(nItem, bCheck);

			KillSelected();
			SetSelected(nItem, FALSE);
		}
	}
}

void CColumns::OnBottom() 
{
	int	nItem;
	CString	text, key;
	CString	gname, gno;
	BOOL	bCheck = TRUE;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = m_list.GetNextSelectedItem(pos);

		if (nItem == 0 && m_list.GetItemText(nItem, 0) == "선택")
		{
			m_slog.Format("[axisdialog][%s]<%d> nItem=[%d]  nItem  = 0 return  ]",
				__FUNCTION__, __LINE__, nItem);
			OutputDebugString(m_slog);
			return;
		}

		if (nItem != m_list.GetItemCount())
		{
			text = m_list.GetItemText(nItem, 0);
			key = m_list.GetItemText(nItem, 1);
			bCheck = m_list.GetCheck(nItem);
			m_list.DeleteItem(nItem);

			nItem = addItem(text, key, m_list.GetItemCount());
			m_list.SetCheck(nItem, bCheck);

			KillSelected();
			SetSelected(nItem, FALSE);
		}
	}
}

void CColumns::OnInit() 
{
	CString sKey;
	int nPos = 0;
	m_list.DeleteAllItems();
	
	CString sData;
	for (int ii = 0; ii < m_defs.GetSize(); ii++)
	{
		if (m_columns.Lookup(m_defs.GetAt(ii), sData))
		{
			addItem(sData, m_defs.GetAt(ii), nPos);
			m_list.SetCheck(nPos);
			nPos++;
		}
	}
	
	for (POSITION pos = m_columns.GetStartPosition(); pos; )
	{
		m_columns.GetNextAssoc(pos, sKey, sData);
		if (IsExist(m_defs, sKey) == false)
		{
			addItem(sData, sKey, nPos);
			m_list.SetCheck(nPos, FALSE);
			nPos++;
		}
	}
}

void CColumns::KillSelected()
{
	int nItem;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			nItem = m_list.GetNextSelectedItem(pos);
			m_list.SetItemState(nItem, NULL, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

void CColumns::SetSelected(int nItem, BOOL scroll/* = TRUE */)
{
	if (nItem == 0 && m_list.GetItemText(nItem , 0) == "선택")
		return;

	CRect	rc;
	CSize	size;

	m_list.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	if (scroll)
	{
		m_list.GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - m_list.GetTopIndex());
		m_list.Scroll(size);
	}
}

void CColumns::OnOk() 
{
	CString sData;
	char szData[64];
	m_sItemList = "";
	memset(szData, 0x00, sizeof(szData));
	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		if (m_list.GetCheck(ii))
		{
			m_list.GetItemText(ii, 1, szData, sizeof(szData));
			sData.Format("%s", szData);

			if (sData == "선택")
				sData = "HEAD ?";

			m_sItemList += sData;
			m_sItemList += cSEP;
		}
	}

	CDialog::OnOK();
}

void CColumns::OnCancel() 
{
	CDialog::OnCancel();
}

void CColumns::OnBeginDragAllList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nItem = pNMListView->iItem;
	if (nItem == 0 && m_list.GetItemText(nItem, 0) == _T("선택"))
	{
		*pResult = 1;
		return;
	}

	begindrag(&m_list, pNMHDR);
	*pResult = 0;
}

void CColumns::OnBeginDragSelectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nItem = pNMListView->iItem;
	if (nItem == 0 && m_list.GetItemText(nItem, 0) == _T("선택"))
	{
		*pResult = 1;
		return;
	}

	begindrag(&m_list, pNMHDR);
	*pResult = 0;
}

void CColumns::Add() 
{
	CString	keys, sData;

	for (POSITION pos = m_list.GetFirstSelectedItemPosition(); pos; )
	{
		int	item = m_list.GetNextSelectedItem(pos);
		keys = m_list.GetItemText(item, 1);
	
		sData = m_list.GetItemText(item, 0);
		addItem(sData, keys, m_list.GetItemCount());
	}
}

void CColumns::Del() 
{
	for (POSITION pos = m_list.GetFirstSelectedItemPosition(); pos; )
	{
		int	item = m_list.GetNextSelectedItem(pos);
		m_list.DeleteItem(item);
	}
}

bool CColumns::checkDuplicate(CString keys)
{
	CString	data;

	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		data = m_list.GetItemText(ii, 1);
		if (!keys.CompareNoCase(data))
		{
			m_list.DeleteItem(ii);
			return true;
		}
	}
	return false;
}

void CColumns::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_dragImage && m_dragWnd)
	{
		CPoint	dropPt(point);

		ClientToScreen(&dropPt);
		m_dragImage->DragMove(dropPt);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CColumns::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_dragImage && m_dragWnd)
	{
		::ReleaseCapture();
		m_dragImage->DragLeave(GetDesktopWindow());
		m_dragImage->EndDrag();
		
		CPoint	pt(point);
		CWnd*	dropWnd;
		CRect	rect, rectx;

		ClientToScreen(&pt);
		m_list.GetWindowRect(&rect);
		
		if (rect.PtInRect(pt))
			dropWnd = &m_list;
		else
			dropWnd = CWnd::WindowFromPoint(pt);
		
		if (m_dragWnd == &m_list && dropWnd)
		{
			CPoint	newPt;
			int	count, index;

			newPt.x = pt.x - rect.left;
			newPt.y = pt.y - rect.top;
			index = m_list.HitTest(newPt);
			if (index < 0)
				index = min(m_list.GetTopIndex()+m_list.GetCountPerPage(), m_list.GetItemCount());
			
			count = m_list.GetSelectedCount();
			if (count > 0 && index >= 0)
			{
				int	skip, sel = -1;
				CStringArray texts, items, bCheck;

				skip = 0;
				texts.RemoveAll();
				items.RemoveAll();
				bCheck.RemoveAll();

				for (int ii = 0; ii < count; ii++)
				{
					sel = m_list.GetNextItem(sel, LVNI_SELECTED);
					if (sel < index)
						skip++;
					texts.Add(m_list.GetItemText(sel, 0));
					items.Add(m_list.GetItemText(sel, 1));
					if (m_list.GetCheck(sel))
						bCheck.Add("1");
					else
						bCheck.Add("0");
				}

				Del();
				index -= skip;
				for (ii = 0; ii < count; ii++)
				{
					addItem(texts.GetAt(ii), items.GetAt(ii), index);
					if (bCheck.GetAt(ii) == "0")
						m_list.SetCheck(index, FALSE);
					else
						m_list.SetCheck(index, TRUE);
					index++;
				}
			}
		}
					
		m_dragImage->DeleteImageList();
		if (m_dragImage)
			delete m_dragImage;
		m_dragImage = NULL;
		m_dragWnd = NULL;
	}	

	CDialog::OnLButtonUp(nFlags, point);
}

CString CColumns::GetItems()
{
	return m_sItemList;
}

void CColumns::parse(CString src, CString items, CString defs)
{
	int	pos;
	CString	syms;

	m_columns.RemoveAll();
	for (int ii = 0; !src.IsEmpty(); ii++)
	{
		//pos = src.Find(cTAB);  //cTAB cSEP   //test  cTAB->cSEP
		pos = src.Find(cSEP);
		if (pos != -1)
		{
			if (ii % 2)
			{
				if (!syms.IsEmpty())
					m_columns.SetAt(syms, src.Left(pos));
			}
			else
				syms = src.Left(pos);
			src  = src.Mid(pos+1);
		}
		else
		{
			if (ii % 2 && !syms.IsEmpty())
				m_columns.SetAt(syms, src);
			src.Empty();
		}
	}

	m_items.RemoveAll();
	for (ii = 0; !items.IsEmpty(); )
	{
		pos = items.Find(cSEP);
		if (pos != -1)
		{
			m_items.Add(items.Left(pos));
			items = items.Mid(pos+1);
		}
		else
		{
			m_items.Add(items);
			items = _T("");
		}
	}

	m_defs.RemoveAll();
	for (ii = 0; !defs.IsEmpty(); )
	{
		pos = defs.Find(cSEP);
		if (pos != -1)
		{
			m_defs.Add(defs.Left(pos));
			defs = defs.Mid(pos+1);
		}
		else
		{
			m_defs.Add(defs);
			defs = _T("");
		}
	}

	m_slog.Format("[axisdialog][%s]<%d> m_columns size=[%d]  m_items size=[%d] m_defs size=[%d]",
		__FUNCTION__,__LINE__,m_columns.GetCount(), m_items.GetSize(), m_defs.GetSize());
	OutputDebugString(m_slog);
}

BOOL CColumns::addColumn(CListCtrl& list, CString columns, int col, int width)
{
	LV_COLUMN lvc;

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR)columns.operator LPCTSTR();
	if (width > 0)
		lvc.cx = list.GetStringWidth(lvc.pszText) + width;
	else
		lvc.cx = 0;
	lvc.iSubItem = -1;

	return list.InsertColumn(col, &lvc);
}

int CColumns::addItem(CString items, CString keys, int item)
{
	m_slog.Format("[AXISDIALOG][%s]<%d>items=[%s] keys=[%s] item=[%d]",
		__FUNCTION__, __LINE__, items, keys, item);
	OutputDebugString(m_slog);

	if (items.Find("HEAD ?") >= 0)
		items = "선택";


	LV_ITEM lvi;
	int nItem;
	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = 0;
	lvi.pszText  = (LPTSTR) items.operator LPCTSTR();
	nItem = m_list.InsertItem(&lvi);

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = 1;
	lvi.pszText  = (LPTSTR)keys.operator LPCTSTR();
	m_list.SetItem(&lvi);

	return nItem;
}

void CColumns::begindrag(CListCtrl* list, NMHDR* pNMHDR) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (list->GetSelectedCount() <= 0)
		return; 

	CPoint	point = CPoint(0, 0);
 	m_dragImage = createDragImage(list, point);
	if (m_dragImage == NULL)
		return;

	m_dragWnd = list;
	point = CPoint(pNMListView->ptAction) - point;
	m_dragImage->BeginDrag(0, point);
	m_dragImage->DragEnter(GetDesktopWindow(), point);
	SetCapture();
}

CImageList* CColumns::createDragImage(CListCtrl* list, CPoint& point)
{
	if (list->GetSelectedCount() <= 0)
		return NULL;

	int	width, index, bottom;
	CRect	rectSingle;
	CRect	rectComplete(0,0,0,0);

	list->GetClientRect(rectSingle);
	width  = rectSingle.Width();
	index  = list->GetTopIndex()-1;
	bottom = list->GetTopIndex()+list->GetCountPerPage()-1;
	if (bottom > list->GetItemCount()-1)
		bottom = list->GetItemCount() - 1;

	while ((index = list->GetNextItem(index, LVNI_SELECTED)) != -1)
	{
		if (index > bottom)
			break; 

		list->GetItemRect(index, rectSingle, LVIR_BOUNDS);
		if (rectSingle.left < 0) 
			rectSingle.left = 0;

		if (rectSingle.right > width)
			rectSingle.right = width;

		rectComplete.UnionRect(rectComplete, rectSingle);
	}
		
	CClientDC clientDC(this);
	CDC	memDC;
	CBitmap bitmap;

	if (!memDC.CreateCompatibleDC(&clientDC))
		return NULL;

	if (!bitmap.CreateCompatibleBitmap(&clientDC, rectComplete.Width(), rectComplete.Height()))
		return NULL;

	CBitmap *pbitmap = memDC.SelectObject(&bitmap);
 	memDC.FillSolidRect(0, 0, rectComplete.Width(), rectComplete.Height(), RGB(255,255,0));
	index = list->GetTopIndex() - 1;
	while ((index = list->GetNextItem(index, LVNI_SELECTED)) != -1)
	{	
		if (index > bottom)
			return NULL;

		CPoint	pt;
		CImageList* imageList = list->CreateDragImage(index, &pt);

		if (imageList)
		{
			list->GetItemRect(index, rectSingle, LVIR_BOUNDS);
			imageList->Draw(&memDC, 0, CPoint(rectSingle.left - rectComplete.left, rectSingle.top - rectComplete.top), ILD_MASK);
			imageList->DeleteImageList();
			delete imageList;
		}
	}

 	memDC.SelectObject(pbitmap);

	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(rectComplete.Width(), rectComplete.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pCompleteImageList->Add(&bitmap, RGB(0,255,0));
	bitmap.DeleteObject();

	point.x = rectComplete.left;
	point.y = rectComplete.top;

	return pCompleteImageList;
}

void CColumns::OnDestroy()
{
	CDialog::OnDestroy();

	if (bmInit)		{ bmInit->DeleteObject(); delete bmInit; bmInit = NULL; };
	if (bmInit_dn)		{ bmInit_dn->DeleteObject(); delete bmInit_dn; bmInit_dn = NULL; };
	if (bmInit_en)		{ bmInit_en->DeleteObject(); delete bmInit_en; bmInit_en = NULL; };
	if (bmTop)		{ bmTop->DeleteObject(); delete bmTop; bmTop = NULL; };
	if (bmTop_dn)		{ bmTop_dn->DeleteObject(); delete bmTop_dn; bmTop_dn = NULL; };
	if (bmTop_en)		{ bmTop_en->DeleteObject(); delete bmTop_en; bmTop_en = NULL; };
	if (bmUp)		{ bmUp->DeleteObject(); delete bmUp; bmUp = NULL; };
	if (bmUp_dn)		{ bmUp_dn->DeleteObject(); delete bmUp_dn; bmUp_dn = NULL; };
	if (bmUp_en)		{ bmUp_en->DeleteObject(); delete bmUp_en; bmUp_en = NULL; };
	if (bmDown)		{ bmDown->DeleteObject(); delete bmDown; bmDown = NULL; };
	if (bmDown_dn)		{ bmDown_dn->DeleteObject(); delete bmDown_dn; bmDown_dn = NULL; };
	if (bmDown_en)		{ bmDown_en->DeleteObject(); delete bmDown_en; bmDown_en = NULL; };
	if (bmBottom)		{ bmBottom->DeleteObject(); delete bmBottom; bmBottom = NULL; };
	if (bmBottom_dn)	{ bmBottom_dn->DeleteObject(); delete bmBottom_dn; bmBottom_dn = NULL; };
	if (bmBottom_en)	{ bmBottom_en->DeleteObject(); delete bmBottom_en; bmBottom_en = NULL; };
	if (bmOk)		{ bmOk->DeleteObject(); delete bmOk; bmOk = NULL; };
	if (bmOk_dn)		{ bmOk_dn->DeleteObject(); delete bmOk_dn; bmOk_dn = NULL; };
	if (bmOk_en)		{ bmOk_en->DeleteObject(); delete bmOk_en; bmOk_en = NULL; };
	if (bmCancel)		{ bmCancel->DeleteObject(); delete bmCancel; bmCancel = NULL; };
	if (bmCancel_dn)	{ bmCancel_dn->DeleteObject(); delete bmCancel_dn; bmCancel_dn = NULL; };
	if (bmCancel_en)	{ bmCancel_en->DeleteObject(); delete bmCancel_en; bmCancel_en = NULL; };

	if (m_BtnInit)		{m_BtnInit->DestroyWindow(); delete m_BtnInit; m_BtnInit = NULL; };
	if (m_BtnOk)		{m_BtnOk->DestroyWindow() ; delete m_BtnOk; m_BtnOk = NULL; };
	if (m_BtnCancel)	{m_BtnCancel->DestroyWindow(); delete m_BtnCancel; m_BtnCancel = NULL; };
	if (m_BtnTop)		{m_BtnTop->DestroyWindow(); delete m_BtnTop; m_BtnTop = NULL; };
	if (m_BtnUp)		{m_BtnUp->DestroyWindow(); delete m_BtnUp; m_BtnUp = NULL; };
	if (m_BtnDown)		{m_BtnDown->DestroyWindow(); delete m_BtnDown; m_BtnDown = NULL; };
	if (m_BtnBottom)	{m_BtnBottom->DestroyWindow(); delete m_BtnBottom; m_BtnBottom = NULL; };
	if (m_pFont)		{m_pFont->DeleteObject(); delete m_pFont; m_pFont = NULL;};
}



void CColumns::OnClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;
}


void CColumns::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pNMLV = reinterpret_cast<NMLISTVIEW*>(pNMHDR);

	// 체크 상태가 변경되려는 경우인지 확인
	if ((pNMLV->uChanged & LVIF_STATE) &&
		((pNMLV->uNewState ^ pNMLV->uOldState) & LVIS_STATEIMAGEMASK))
	{
		CPoint pt;
		GetCursorPos(&pt);
		m_list.ScreenToClient(&pt);

		LVHITTESTINFO lvhti = { 0 };
		lvhti.pt = pt;
		m_list.HitTest(&lvhti);

		// 체크박스가 아닌 부분 클릭 시 무조건 상태 변경 막기
		//if (!(lvhti.flags & LVHT_ONITEMSTATEICON))
		//{
		//	*pResult = TRUE; // 변경 막기

		//	m_slog.Format("[axisdialog][%s]<%d>  변경막기1",
		//		__FUNCTION__, __LINE__);
		//	OutputDebugString(m_slog);

		//	return;
		//}

		// 추가 조건: 첫 번째 아이템이고 텍스트가 "선택"일 때만 허용
		int iItem = lvhti.iItem;
		if (iItem == 0 && m_list.GetItemText(0, 0) == _T("선택"))
		{
			m_list.SetCheck(iItem, TRUE);
			*pResult = TRUE; // 변경 막기
			return;
		}
	}

	*pResult = FALSE; // 상태 변경 허용
}


void CColumns::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;

	LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	switch (pLVCD->nmcd.dwDrawStage)
	{
		case CDDS_PREPAINT:
			*pResult = CDRF_NOTIFYITEMDRAW;  // 항목별로 그리기 통지 받기
			return;

		case CDDS_ITEMPREPAINT:
		{
			int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);

			if (nItem == 0 && m_list.GetItemText(0, 0) == "선택")
			{
				pLVCD->clrText = RGB(127, 127, 127);  // 회색
			}

			*pResult = CDRF_DODEFAULT;
			return;
		}
	}

	*pResult = CDRF_DODEFAULT;
}

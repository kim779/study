// ToolWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB201600.h"
#include "ToolWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolWnd

CToolWnd::CToolWnd(CWnd* parent, CWnd* mainWnd)
{
	m_pParent = parent;			//부모
	m_pMainWnd = mainWnd;		//최상위
	m_cbIndex = 0;

	m_fontsize = 9;
	EnableAutomation();
}

CToolWnd::~CToolWnd()
{	
}



void CToolWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CToolWnd, CWnd)
	//{{AFX_MSG_MAP(CToolWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CToolWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CToolWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IToolWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {4ED2ADAD-0014-4441-BEF6-6BCEC7BC6C32}
static const IID IID_IToolWnd =
{ 0x4ed2adad, 0x14, 0x4441, { 0xbe, 0xf6, 0x6b, 0xce, 0xc7, 0xbc, 0x6c, 0x32 } };

BEGIN_INTERFACE_MAP(CToolWnd, CWnd)
	INTERFACE_PART(CToolWnd, IID_IToolWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolWnd message handlers

int CToolWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	OperInit();	
	return 0;
}

bool CToolWnd::OperInit()
{
	HBITMAP	hBITMAP{}, hBITMAP_DN{}, hBITMAP_HV{};
	HBITMAP hBITMAP_SAVE{}, hBITMAP_SAVE_DN{}, hBITMAP_SAVE_HV{};

	CRect	rect, clientRC, rectBtn; 
	GetClientRect(clientRC);

	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");
	m_font = getAxFont(_T("굴림체"), 9);
	m_strPal = GetPalette();

	CString	imgPath; 
	imgPath.Format("%s/%s/", m_root, IMAGEDIR);
	
	SetOwner(m_pMainWnd);
	
	rect.SetRect(0, 0, 0, 0);

	rect.left = clientRC.left + 125;
	rect.right = rect.left + 60;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + comboH;

	m_radio10Hoga = std::make_unique<CButton>();
	m_radio10Hoga->Create(_T("10호가"),WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
		WS_GROUP | BS_TOP | BS_BOTTOM, rect, this, IDC_RADIO_10HG);
	m_radio10Hoga->SetFont(m_font, false); 
	
	rect.left = rect.right;
	rect.right = rect.left + 55;
	
	m_radio5Hoga = std::make_unique<CButton>();
	m_radio5Hoga->Create(_T("5호가"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
		BS_TOP | BS_BOTTOM , rect, this, IDC_RADIO_5HG);
	m_radio5Hoga->SetFont(m_font, false); 
	

	m_radio10Hoga->SetCheck(1);

	rect.left = rect.right;// -20;
	rect.bottom = rect.top + 160;
	rect.right = rect.left + 90;

	rectBtn.CopyRect(rect);

	m_cbChangeTab.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST, rect, this, IDC_COMBO_CHANGETAB);
	m_cbChangeTab.SetFont(m_font, false); 
//	m_cbChangeTab.MoveWindow(&rect);
	
	combodataSetting();
	m_cbChangeTab.ShowWindow(SW_HIDE);

	rect.SetRect(0, 0, 0, 0);

	rect.top = clientRC.top + gap5;
	rect.left = clientRC.left + gap3;
	rect.right = rect.left + 20;
	rect.bottom = rect.top + 20;
	
	m_bitmap_nm = getAxBitmap(imgPath + "GTI_NMATRIX.bmp")->operator HBITMAP();
	m_bitmap_dn = getAxBitmap(imgPath + "GTI_NMATRIX_dn.bmp")->operator HBITMAP();
	m_bitmap_hv = getAxBitmap(imgPath + "GTI_NMATRIX_en.bmp")->operator HBITMAP();
	
	m_btnMatrix = std::make_unique<CfxImgButton>();
	m_btnMatrix->Create("", rect, this, IDC_MAXRIX);
	m_btnMatrix->SetFont(m_font, false);  
	m_btnMatrix->SetImgBitmap(m_bitmap_nm, m_bitmap_dn, m_bitmap_hv);
	
	rect.left = rect.right + 2;
	rect.right = rect.left + 200;
	rect.top = clientRC.top + 8;
	rect.bottom = rect.top + 20;

	m_labelMatrix = std::make_unique<CStatic>();
	m_labelMatrix->Create("<< 멀티호가 선택", WS_CHILD | WS_VISIBLE, rect, this, 8050);
	m_labelMatrix->SetFont(m_font);

	hBITMAP = getAxBitmap(imgPath + "1BTN.BMP")->operator HBITMAP();
	hBITMAP_DN = getAxBitmap(imgPath + "1BTN_dn.BMP")->operator HBITMAP();
	hBITMAP_HV = getAxBitmap(imgPath + "1BTN_en.BMP")->operator HBITMAP();

	rect.left = rectBtn.right + 8;
	rect.right = rect.left + 24;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + 20;

	m_btn1 = std::make_unique<CfxImgButton>();
	m_btn1->Create("1",rect, this, IDC_BTN1);
	m_btn1->SetFont(m_font, false);  
	m_btn1->SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	m_btn1->EnableToolTips(TRUE);

	rect.left = rect.right + 4;
	rect.right = rect.left + 24;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + 20;

	m_btn2 = std::make_unique<CfxImgButton>();
	m_btn2->Create("2",rect, this, IDC_BTN2);
	m_btn2->SetFont(m_font, false);  
	m_btn2->SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	m_btn2->EnableToolTips(TRUE);

	rect.left = rect.right + 4;
	rect.right = rect.left + 24;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + 20;

	m_btn3 = std::make_unique<CfxImgButton>();
	m_btn3->Create("3",rect, this, IDC_BTN3);
	m_btn3->SetFont(m_font, false);  
	m_btn3->SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	m_btn3->EnableToolTips(TRUE);

	rect.left = rect.right + 4;
	rect.right = rect.left + 24;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + 20;

	m_btn4 = std::make_unique<CfxImgButton>();
	m_btn4->Create("4",rect, this, IDC_BTN4);
	m_btn4->SetFont(m_font, false);  
	m_btn4->SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	m_btn4->EnableToolTips(TRUE);

	rect.left = rect.right + 4;
	rect.right = rect.left + 24;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + 20;

	m_btn5 = std::make_unique<CfxImgButton>();
	m_btn5->Create("5",rect, this, IDC_BTN5);
	m_btn5->SetFont(m_font, false);  
	m_btn5->SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	m_btn5->EnableToolTips(TRUE);
	
	hBITMAP_SAVE = getAxBitmap(imgPath + "4BTN.BMP")->operator HBITMAP();
	hBITMAP_SAVE_DN = getAxBitmap(imgPath + "4BTN_DN.BMP")->operator HBITMAP();
	hBITMAP_SAVE_HV = getAxBitmap(imgPath + "4BTN_EN.BMP")->operator HBITMAP();

	rect.left = rect.right + 10;
	rect.right = rect.left + 180;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + 20;

	m_chkSelect = std::make_unique <CButton>();
	m_chkSelect->Create(_T("호가,탭설정 그룹별적용"),WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTOCHECKBOX | BS_CHECKBOX | BS_TOP | BS_BOTTOM, rect, this, IDC_CHECK_SELECT);
	m_chkSelect->SetFont(m_font, false); 

	rect.left = rect.right + 5;
	rect.right = rect.left + 132;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + 20;

	m_chkPrice = std::make_unique <CButton>();
	m_chkPrice->Create(_T("등락률알림 일괄적용"),WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTOCHECKBOX | BS_CHECKBOX | BS_TOP | BS_BOTTOM, rect, this, IDC_CHECK_PRICE);
	m_chkPrice->SetFont(m_font, false); 

	rect.left = rect.right + 2;
	rect.right = rect.left + 25;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + 20;

	m_pEditFrom = std::make_unique<CfxEdit>(m_font);
	m_pEditFrom->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_RIGHT, rect, this, IDC_EDIT_PRICEF);
	m_pEditFrom->LimitText(3);

	rect.left = rect.right + 2;
	rect.right = rect.left + 33;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + 20;

	m_pStString = std::make_unique <CfxStatic>(m_font);
	m_pStString->Create("%초과", WS_VISIBLE|WS_CHILD, rect, this);
	m_pStString->SetBkColor(GetColor(66));

// 	rect.left = rect.right;
// 	rect.right = rect.left + 10;
// 	rect.top = clientRC.top + gap5;
// 	rect.bottom = rect.top + 20;
// 	
// 	m_pStString2 = new CfxStatic(m_font);
// 	m_pStString2->Create(" ", WS_VISIBLE|WS_CHILD, rect, this);
// 	m_pStString2->SetBkColor(GetColor(66));
// 	m_pStString2->SetFgColor(GetColor(94));

	rect.left = rect.right + 2;
	rect.right = rect.left + 25;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + 20;

	m_pEditTo = std::make_unique<CfxEdit>(m_font);
	m_pEditTo->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_RIGHT, rect, this, IDC_EDIT_PRICET);
	m_pEditTo->LimitText(3);

	rect.left = rect.right + 2;
	rect.right = rect.left + 33;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + 20;

	m_pStPer = std::make_unique <CfxStatic>(m_font);
	m_pStPer->Create("%미만", WS_VISIBLE|WS_CHILD, rect, this);
	m_pStPer->SetBkColor(GetColor(66));

	rect.left = rect.right + 2;
	rect.right = rect.left + 60;
	rect.top = clientRC.top + gap5;
	rect.bottom = rect.top + 20;

	m_btnSave = std::make_unique<CfxImgButton>();
	m_btnSave->Create("감시시작\n전일종가를 기준으로 계산합니다.",rect, this, IDC_SAVE);
	m_btnSave->SetFont(m_font, false);  
	m_btnSave->SetImgBitmap(hBITMAP_SAVE, hBITMAP_SAVE_DN, hBITMAP_SAVE_HV);
	m_btnSave->CreateTooltip();

	return true;
}

void CToolWnd::combodataSetting()
{
	m_cbChangeTab.ResetContent();

	CString aryType[] = {_T("탭일괄변경"), _T("상세"), _T("체결"), _T("일별"), _T("차트"), _T("회원"), _T("투자"), _T("뉴스")};
	int index =0;

	for(int i=0 ; i< 8 ; i++)
	{
		index = m_cbChangeTab.AddString(aryType[i]);
		m_cbChangeTab.SetItemData(index, i);
	}
	
	m_cbChangeTab.SetCurSel(0);
	UpdateData(FALSE);
}

CBitmap* CToolWnd::getAxBitmap(CString path)
{
	return (CBitmap*)m_pMainWnd->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}


CBrush* CToolWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pMainWnd->SendMessage(WM_USER, getBRUSH, (long)clr);
}


CFont* CToolWnd::getAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;
	
	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)SendAxisMessage( getFONT, (long)&fontR);
}


COLORREF CToolWnd::GetColor(int color)
{
	if (color & 0x02000000)
		return color;
	
	return SendAxisMessage(MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CString	CToolWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	CString strPath = Variant(homeCC, "");
	
	file.Format("%s\\%s\\%s", strPath, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

CString	CToolWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if((long)dta > 1)
		retvalue = CString(dta);
	
	return retvalue;
}

LRESULT CToolWnd::SendAxisMessage(WPARAM wParam, LPARAM lPAram)
{	
	return m_pMainWnd->SendMessage(WM_USER, wParam, lPAram);
}

BOOL CToolWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (LOWORD(wParam))
	{
	case IDC_BTN1:
		if (m_nBtnIndex != 0)
		{
			m_nBtnIndex = 0;
			ClickButton(m_nBtnIndex);
			m_pParent->SendMessage(WM_MANAGE, MK_LOADSNAP, m_nBtnIndex);
		}
		break;
	case IDC_BTN2:
		if (m_nBtnIndex != 1)
		{
			m_nBtnIndex = 1;
			ClickButton(m_nBtnIndex);
			m_pParent->SendMessage(WM_MANAGE, MK_LOADSNAP, m_nBtnIndex);
		}
		break;
	case IDC_BTN3:
		if (m_nBtnIndex != 2)
		{
			m_nBtnIndex = 2;
			ClickButton(m_nBtnIndex);
			m_pParent->SendMessage(WM_MANAGE, MK_LOADSNAP, m_nBtnIndex);
		}
		break;
	case IDC_BTN4:
		if (m_nBtnIndex != 3)
		{
			m_nBtnIndex = 3;
			ClickButton(m_nBtnIndex);
			m_pParent->SendMessage(WM_MANAGE, MK_LOADSNAP, m_nBtnIndex);
		}
		break;
	case IDC_BTN5:
		if (m_nBtnIndex != 4)
		{
			m_nBtnIndex = 4;
			ClickButton(m_nBtnIndex);
			m_pParent->SendMessage(WM_MANAGE, MK_LOADSNAP, m_nBtnIndex);
		}
		break;
	case IDC_SAVE:
		{
			if(m_pEditFrom->GetText() == "" || m_pEditTo->GetText() == "")
				break;

			CString txt;

			m_btnSave->GetWindowText(txt);

			if(txt.Find("감시시작") > -1)
			{
				if(atoi(m_pEditFrom->GetText()) <= atoi(m_pEditTo->GetText()) )
				{
					AfxMessageBox("초과값이 미만값보다 커야 합니다.");
					break;;
				}

				CString str;
				str.Format("%02d,%02d",atoi(m_pEditFrom->GetText()),atoi(m_pEditTo->GetText()));
				m_pParent->SendMessage(WM_MANAGE,MK_RANGEPERCENT,(long)str.operator LPCTSTR());
				m_pEditFrom->EnableWindow(FALSE);
				m_pEditTo->EnableWindow(FALSE);

				m_btnSave->SetWindowText("감시중지");
			}
			else
			{
				CString str;
				str.Format("%02d,%02d",0,0);
				m_pParent->SendMessage(WM_MANAGE,MK_RANGEPERCENT,(long)str.operator LPCTSTR());
				m_pEditFrom->EnableWindow(TRUE);
				m_pEditTo->EnableWindow(TRUE);

				m_btnSave->SetWindowText("감시시작");
			}

// 			CString str;
// 			str.Format("%02d,%02d",atoi(m_pEditFrom->GetText()),atoi(m_pEditTo->GetText()));
// 			m_pParent->SendMessage(WM_MANAGE,MK_RANGEPERCENT,(long)str.operator LPCTSTR());

			CString strPath;
			strPath.Format("%s\\%s\\%s\\IB201600.ini", m_root, USRDIR, m_name);

			if(m_chkPrice->GetCheck())
			{
				//m_btnSave->EnableWindow(TRUE);
				
				WritePrivateProfileString("RANGE", "ALL", "1", (LPCTSTR)strPath);
				WritePrivateProfileString("RANGE", "PERCENTF", m_pEditFrom->GetText(), (LPCTSTR)strPath);
				WritePrivateProfileString("RANGE", "PERCENTT", m_pEditTo->GetText(), (LPCTSTR)strPath);
			}
			else
			{
				//m_btnSave->EnableWindow(FALSE);
				
				WritePrivateProfileString("RANGE", "ALL", "0", (LPCTSTR)strPath);
			}
		}
		break;
	case IDC_MAXRIX:
		m_pParent->SendMessage(WM_MANAGE, MK_SHOWMATRIX);
		break;
	case IDC_RADIO_10HG:
		//saveRadioBtnInfo(HOGATYPE10);
		m_cbChangeTab.ShowWindow(SW_HIDE);
		m_pParent->SendMessage(WM_MANAGE, MK_SHOW10HOGA);
		m_pParent->SendMessage(WM_MANAGE,MK_SNAPSHOT,m_nBtnIndex);
		break;
	case IDC_RADIO_5HG:
		//saveRadioBtnInfo(HOGATYPE5);
		m_cbChangeTab.ShowWindow(SW_SHOW);
		m_cbChangeTab.EnableWindow(FALSE);
		m_pParent->SendMessage(WM_MANAGE, MK_SHOW5HOGA);
		m_pParent->SendMessage(WM_MANAGE,MK_SNAPSHOT,m_nBtnIndex);
		SetTimer(2002,600,nullptr);
		break;
	case IDC_COMBO_CHANGETAB:
		if((HIWORD(wParam) == CBN_SELCHANGE))
		{
			m_cbIndex = m_cbChangeTab.GetCurSel();
			if(m_cbIndex != 0)
			{
				m_pParent->SendMessage(WM_MANAGE, MK_CHANGETAB);
			}

			m_pParent->SendMessage(WM_MANAGE,MK_SNAPSHOT,m_nBtnIndex);
		}
		break;
	case IDC_CHECK_SELECT:
		{
			const int iSelect = m_chkSelect->GetCheck();
			saveCheckBoxInfo(iSelect);
		}
		break;
	case IDC_CHECK_PRICE:
		{
			CString strPath;
			strPath.Format("%s\\%s\\%s\\IB201600.ini", m_root, USRDIR, m_name);

// 			if(m_chkPrice->GetCheck())
// 			{
// 				m_btnSave->EnableWindow(TRUE);
// 
// 				WritePrivateProfileString("RANGE", "ALL", "1", (LPCTSTR)strPath);
// 				WritePrivateProfileString("RANGE", "PERCENTF", m_pEditFrom->GetText(), (LPCTSTR)strPath);
// 				WritePrivateProfileString("RANGE", "PERCENTT", m_pEditTo->GetText(), (LPCTSTR)strPath);
// 			}
// 			else
// 			{
// 				m_btnSave->EnableWindow(FALSE);
// 
// 				WritePrivateProfileString("RANGE", "ALL", "0", (LPCTSTR)strPath);
// 			}
			if(m_chkPrice->GetCheck())
			{
				m_btnSave->EnableWindow(TRUE);
			}
			else
			{
				m_btnSave->EnableWindow(FALSE);
			}

			m_pParent->SendMessage(WM_MANAGE,MK_ALLPERCENT,m_chkPrice->GetCheck());
		}
		break;
	}
	return CWnd::OnCommand(wParam, lParam);
}

void CToolWnd::ClickButton(int nidx)
{
	CFont* boldFont{};
	CFont* font{};

	boldFont = getAxFont("굴림",9,true);
	font = getAxFont("굴림",9,false);


	switch(nidx)
	{
	case 0:
		m_btn1->SetFont(boldFont);
		m_btn1->SetTextColor(255);
		m_btn2->SetFont(font);
		m_btn2->SetTextColor(0);
		m_btn3->SetFont(font);
		m_btn3->SetTextColor(0);
		m_btn4->SetFont(font);
		m_btn4->SetTextColor(0);
		m_btn5->SetFont(font);
		m_btn5->SetTextColor(0);
		break;
	case 1:
		m_btn1->SetFont(font);
		m_btn1->SetTextColor(0);
		m_btn2->SetFont(boldFont);
		m_btn2->SetTextColor(255);
		m_btn3->SetFont(font);
		m_btn3->SetTextColor(0);
		m_btn4->SetFont(font);
		m_btn4->SetTextColor(0);
		m_btn5->SetFont(font);
		m_btn5->SetTextColor(0);
		break;
	case 2:
		m_btn1->SetFont(font);
		m_btn1->SetTextColor(0);
		m_btn2->SetFont(font);
		m_btn2->SetTextColor(0);
		m_btn3->SetFont(boldFont);
		m_btn3->SetTextColor(255);
		m_btn4->SetFont(font);
		m_btn4->SetTextColor(0);
		m_btn5->SetFont(font);
		m_btn5->SetTextColor(0);
		break;
	case 3:
		m_btn1->SetFont(font);
		m_btn1->SetTextColor(0);
		m_btn2->SetFont(font);
		m_btn2->SetTextColor(0);
		m_btn3->SetFont(font);
		m_btn3->SetTextColor(0);
		m_btn4->SetFont(boldFont);
		m_btn4->SetTextColor(255);
		m_btn5->SetFont(font);
		m_btn5->SetTextColor(0);
		break;
	case 4:
		m_btn1->SetFont(font);
		m_btn1->SetTextColor(0);
		m_btn2->SetFont(font);
		m_btn2->SetTextColor(0);
		m_btn3->SetFont(font);
		m_btn3->SetTextColor(0);
		m_btn4->SetFont(font);
		m_btn4->SetTextColor(0);
		m_btn5->SetFont(boldFont);
		m_btn5->SetTextColor(255);
		break;
	}
}

void CToolWnd::saveRadioBtnInfo(int ihoga)
{
	CString strPath(_T(""));
	CString hoga(_T(""));
	
	strPath.Format("%s\\%s\\%s\\saveCode.ini", m_root, USRDIR, m_name);
	
	hoga.Format("%d", ihoga);

	WritePrivateProfileString(szMatrix, "hoga", (LPCTSTR)hoga, (LPCTSTR)strPath);
}

void CToolWnd::saveCheckBoxInfo(int iSelect)
{
	CString strPath(_T(""));
	CString select(_T(""));
	
	strPath.Format("%s\\%s\\%s\\saveInfo.ini", m_root, USRDIR, m_name);
	
	select.Format("%d", iSelect);
	
	WritePrivateProfileString(szMatrix, "select", (LPCTSTR)select, (LPCTSTR)strPath);
}

void CToolWnd::initRadioBtn(int hoga)
{
	if(hoga == HOGATYPE10)
	{
		m_radio10Hoga->SetCheck(true);
		m_radio5Hoga->SetCheck(false);
		m_cbChangeTab.ShowWindow(SW_HIDE);
	}
	else
	{
		m_radio5Hoga->SetCheck(true);
		m_radio10Hoga->SetCheck(false);
		m_cbChangeTab.ShowWindow(SW_SHOW);
	}
}

void CToolWnd::initCheckBox(int iSelect)
{
	m_chkSelect->SetCheck(iSelect);
}

int CToolWnd::GetRadioCheck()
{
	int result{};

	if(m_radio10Hoga->GetCheck())
		result = HOGATYPE10;
	else
		result = HOGATYPE5;

	return result;
}

void CToolWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC	backdc;
	backdc.CreateCompatibleDC(&dc); 
	CBitmap bitmap,  *pOldBitmap;
	CRect rectClient;
	GetClientRect(rectClient);
	
	bitmap.CreateCompatibleBitmap(&dc, rectClient.right, rectClient.bottom);
	pOldBitmap = (CBitmap*)backdc.SelectObject(&bitmap);
	backdc.SetBkMode(OPAQUE);
	
	backdc.FillSolidRect( rectClient, m_clrBgColor); 
	CFont* pOldFont = backdc.SelectObject(m_font);	
	
	Draw(&backdc, rectClient);	
	dc.BitBlt( rectClient.left , rectClient.top , rectClient.right , rectClient.bottom , &backdc, 0,0, SRCCOPY );
	bitmap.DeleteObject(); 
	backdc.SelectObject(pOldBitmap);
	backdc.SelectObject(pOldFont); 
	DeleteObject(pOldFont);
	DeleteObject(pOldBitmap);
	ReleaseDC(&backdc);		
	// Do not call CWnd::OnPaint() for painting messages
}

void CToolWnd::Draw(CDC *pDC, CRect rcClient)
{
	CRect rect,rectb;
	
	GetClientRect(&rect);
	
	//background
	pDC->FillSolidRect(&rect,GetColor(64));//64
	rectb = rect;
	
	//upper panel
	rect.bottom = offsetGH + gap2;
	rect.left = rect.left + gap1;
	rect.top = rect.top + gap1;
	rect.right = rect.right - gap1;
	
	pDC->FillSolidRect(&rect,GetColor(66));
	DrawRoundBitmap(pDC, rect, ROUND_CONTENTS);
}


void CToolWnd::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}


void CToolWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;
	
	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;
	
	memDC.CreateCompatibleDC(pDC);
	
	if (memDC.m_hDC != nullptr)
	{
		memDC.SelectObject(BitMap);
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	}
	DeleteObject(BitMap);
	memDC.DeleteDC();
}

void CToolWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	// TODO: Add your message handler code here

	if(m_radio5Hoga->GetCheck())
		saveRadioBtnInfo(HOGATYPE5);
	else
		saveRadioBtnInfo(HOGATYPE10);
	
	//m_btnMatrix->DestroyWindow();
	//m_radio5Hoga->DestroyWindow();
	//m_radio10Hoga->DestroyWindow();
	//m_labelMatrix->DestroyWindow();

	//delete m_btnMatrix;
	//delete m_radio10Hoga; 
	//delete m_radio5Hoga;
	//delete m_labelMatrix;	
}



HBRUSH CToolWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	const UINT nID = pWnd->GetDlgCtrlID();

	if (nID == IDC_RADIO_10HG || nID == IDC_RADIO_5HG || nID == IDC_CHECK_SELECT || nID == IDC_CHECK_PRICE)
	{
		pDC->SetBkColor(GetColor(66));
		return GetAxBrush(GetColor(66))->operator HBRUSH();  
	}	
	else if(nCtlColor == CTLCOLOR_STATIC)
	{
		HDC hdc = pDC->m_hDC;
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, GetColor(64));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
		return hbr;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CToolWnd::OnClose() 
{
	CWnd::OnClose();
}

void CToolWnd::SetComboIndex(int index)
{
	m_cbChangeTab.SetCurSel(index);

	if(index != 0)
	{
		m_cbIndex = index;

		m_cbChangeTab.EnableWindow(FALSE);
		m_pParent->SendMessage(WM_MANAGE, MK_CHANGETAB);
		m_cbChangeTab.EnableWindow(TRUE);
	}
}

void CToolWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 2002)
	{
		KillTimer(2002);
		m_cbIndex = m_cbChangeTab.GetCurSel();
		
		if(m_cbIndex != 0)
		{
			m_pParent->SendMessage(WM_MANAGE, MK_CHANGETAB);
		}
		m_cbChangeTab.EnableWindow(TRUE);
	}
	CWnd::OnTimer(nIDEvent);
}

void CToolWnd::loadrange()
{
	CString strPath;
	strPath.Format("%s\\%s\\%s\\IB201600.ini", m_root, USRDIR, m_name);
	
	const int nAll = GetPrivateProfileInt("RANGE","ALL",0,strPath);
	
	if(nAll)
	{
		m_btnSave->EnableWindow(TRUE);
		char buff[10];
		CString str;
		
		m_chkPrice->SetCheck(nAll);
		
		GetPrivateProfileString("RANGE", "PERCENTF", "", buff, sizeof(buff), (LPCTSTR)strPath);
		
		str = buff;
		m_pEditFrom->SetText(str);
		
		GetPrivateProfileString("RANGE", "PERCENTT", "", buff, sizeof(buff), (LPCTSTR)strPath);
		
		str = buff;
		m_pEditTo->SetText(str);

		m_pParent->SendMessage(WM_MANAGE,MK_ALLPERCENT,m_chkPrice->GetCheck());
	}
	else
	{
		m_btnSave->EnableWindow(FALSE);
	}
}

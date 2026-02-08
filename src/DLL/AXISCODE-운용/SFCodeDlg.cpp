// SFCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "SFCodeDlg.h"
#include "FuturesDlg.h"
#include "../../h/jmcode.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSFCodeDlg dialog

CSFCodeDlg::CSFCodeDlg(int kind, CPoint pt, int type /*= 0*/, CWnd* pParent /*=nullptr*/)
	: CDialog(CSFCodeDlg::IDD, pParent)
{
	EnableAutomation();

	m_point = pt;
	m_sPreSearch = "";
	//{{AFX_DATA_INIT(CSFCodeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSFCodeDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CSFCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSFCodeDlg)
	DDX_Control(pDX, IDC_NAME_LIST, m_NameList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSFCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CSFCodeDlg)
	ON_WM_CREATE()
	ON_LBN_SELCHANGE(IDC_NAME_LIST, OnSelchangeNameList)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, OnChangeEditSearch)
	ON_BN_CLICKED(IDC_BUTTON_MTH1, OnButtonMth1)
	ON_BN_CLICKED(IDC_BUTTON_MTH2, OnButtonMth2)
	ON_BN_CLICKED(IDC_BUTTON_MTH3, OnButtonMth3)
	ON_BN_CLICKED(IDC_BUTTON_MTH4, OnButtonMth4)
	ON_BN_CLICKED(IDC_BUTTON_MTH5, OnButtonMth5)
	ON_BN_CLICKED(IDC_BUTTON_MTH6, OnButtonMth6)
	ON_BN_CLICKED(IDC_BUTTON_MTH7, OnButtonMth7)
	ON_BN_CLICKED(IDC_BUTTON_MTH8, OnButtonMth8)
	ON_BN_CLICKED(IDC_BUTTON_MTH9, OnButtonMth9)
	ON_BN_CLICKED(IDC_BUTTON_SPR1, OnButtonSpr1)
	ON_BN_CLICKED(IDC_BUTTON_SPR2, OnButtonSpr2)
	ON_BN_CLICKED(IDC_BUTTON_SPR3, OnButtonSpr3)
	ON_BN_CLICKED(IDC_BUTTON_SPR4, OnButtonSpr4)
	ON_BN_CLICKED(IDC_BUTTON_SPR5, OnButtonSpr5)
	ON_BN_CLICKED(IDC_BUTTON_SPR6, OnButtonSpr6)
	ON_BN_CLICKED(IDC_BUTTON_SPR7, OnButtonSpr7)
	ON_BN_CLICKED(IDC_BUTTON_SPR8, OnButtonSpr8)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_NEXT, OnButtonSearchNext)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_PREV, OnButtonSearchPrev)
	//}}AFX_MSG_MAP
	ON_LBN_DBLCLK(IDC_NAME_LIST, &CSFCodeDlg::OnDblclkNameList)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSFCodeDlg, CDialog)
	//{{AFX_DISPATCH_MAP(CSFCodeDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ISFCodeDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {0DF7720B-1F74-424E-8428-9437F94981B3}
static const IID IID_ISFCodeDlg =
{ 0xdf7720b, 0x1f74, 0x424e, { 0x84, 0x28, 0x94, 0x37, 0xf9, 0x49, 0x81, 0xb3 } };

BEGIN_INTERFACE_MAP(CSFCodeDlg, CDialog)
	INTERFACE_PART(CSFCodeDlg, IID_ISFCodeDlg, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSFCodeDlg message handlers

int CSFCodeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CSFCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	LoadMaster();

	m_NameList.SetCurSel(0);

	OnSelchangeNameList();

	((CWnd*)GetDlgItem(IDC_STATIC_SMALL))->ShowWindow(SW_HIDE);
	if (m_iTYPE == 1)
	{
		((CWnd*)GetDlgItem(IDC_STATIC_SMALL))->ShowWindow(SW_SHOW);
		int gap = 0;
		CRect rec, listrec, btnLRec, btnRRec, edRec;
		GetClientRect(rec);

		((CWnd*)GetDlgItem(IDC_NAME_LIST))->GetWindowRect(listrec);
		ScreenToClient(listrec);

		// 현재 폭을 절반으로 줄임
		rec.right = listrec.right + 25;

		// 위치를 유지하면서 폭만 변경
		CPoint pt;
		GetCursorPos(&pt);
		SetWindowPos(NULL, pt.x, pt.y, rec.Width(), rec.Height() + 30, SWP_NOZORDER );


		CWnd* pButton = GetDlgItem(IDC_BUTTON_SEARCH_NEXT);
		pButton->GetWindowRect(btnRRec);
		ScreenToClient(btnRRec);
		btnRRec.left = listrec.right - btnRRec.Width();
		btnRRec.right = listrec.right;
		pButton->MoveWindow(btnRRec.left, btnRRec.top, btnRRec.Width(), btnRRec.Height(), TRUE);

		pButton = GetDlgItem(IDC_BUTTON_SEARCH_PREV);
		pButton->GetWindowRect(btnLRec);
		ScreenToClient(btnLRec);
		btnLRec.left = btnRRec.left - gap - btnLRec.Width();
		btnLRec.right = btnRRec.left - gap;
		pButton->MoveWindow(btnLRec.left, btnLRec.top, btnLRec.Width(), btnLRec.Height(), TRUE);


		((CWnd*)GetDlgItem(IDC_EDIT_SEARCH))->GetWindowRect(edRec);
		ScreenToClient(edRec);
		edRec.right = btnLRec.left - 2;
		((CWnd*)GetDlgItem(IDC_EDIT_SEARCH))->MoveWindow(edRec.left, edRec.top, edRec.Width(), edRec.Height(), TRUE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSFCodeDlg::LoadMaster()
{
	CFile file;
	CString path;
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\sfcode.dat";
	
	// 선물종목코드
	if (file.Open(path, CFile::modeRead))
	{
		m_fjcode.resize( gsl::narrow_cast<const int>(file.GetLength()/sizeof(struct sfcode)) );
		file.Read(&m_fjcode[0], m_fjcode.size() * sizeof(struct sfcode) );
		file.Close();
	}

	m_arName.RemoveAll();
	CSortStringArray sortArray;

	for(size_t i=0;i<m_fjcode.size();i++)
	{
		CString str(m_fjcode[i].gnam,sizeof(m_fjcode[i].gnam));
		CString tmp;

		str.TrimLeft();
		str.TrimRight();

		if(!m_arName.Lookup(str,tmp))
		{
			CString code(m_fjcode[i].gcod,sizeof(m_fjcode[i].gcod));

			m_arName.SetAt(str,code);
			sortArray.Add(str);
			//m_NameList.AddString(str);
		}
	}

	sortArray.Sort();

	if (m_NameList.GetSafeHwnd())
	{
		for (int i = 0; i <= sortArray.GetUpperBound(); i++)
		{
			CString name = sortArray.GetAt(i);
			m_NameList.AddString(name);
		}
	}
}

void CSFCodeDlg::OnSelchangeNameList() 
{
	// TODO: Add your control notification handler code here
	const int nIndex[] = {IDC_BUTTON_MTH1,
		              IDC_BUTTON_MTH2,
		              IDC_BUTTON_MTH3,
		              IDC_BUTTON_MTH4,
		              IDC_BUTTON_MTH5,
		              IDC_BUTTON_MTH6,
		              IDC_BUTTON_MTH7,
		              IDC_BUTTON_MTH8,
		              IDC_BUTTON_MTH9,
			      IDC_BUTTON_SPR1,
		              IDC_BUTTON_SPR2,
		              IDC_BUTTON_SPR3,
		              IDC_BUTTON_SPR4,
		              IDC_BUTTON_SPR5,
		              IDC_BUTTON_SPR6,
		              IDC_BUTTON_SPR7,
		              IDC_BUTTON_SPR8};
	CString text,code;

	m_NameList.GetText(m_NameList.GetCurSel(),text);

	for(int j=0;j<17;j++)
	{
		((CButton*)GetDlgItem(nIndex[j]))->SetWindowText("");
	}

	m_arHNam.RemoveAll();
	m_arHCod.RemoveAll();
	m_mapHCod.RemoveAll();

	if(m_arName.Lookup(text,code))
	{
		int n = 0;
		for(size_t i=0;i<m_fjcode.size();i++)
		{
			CString gcod(m_fjcode[i].gcod,sizeof(m_fjcode[i].gcod));

			if(code == gcod)
			{
				CString cod(m_fjcode[i].codx,sizeof(m_fjcode[i].codx));
				CString nam(m_fjcode[i].hnam,sizeof(m_fjcode[i].hnam));
				CString gnam(m_fjcode[i].gnam, sizeof(m_fjcode[i].gnam));
				
				if(n < 9 && (cod.GetAt(0) == '4' || cod.GetAt(0) == 'D'))  //파생상품 코드개편
					n = 9;
				CString str;

				nam.TrimRight();

				str = nam;

				str.Remove('*');

				if(cod.GetAt(0) == '4' || cod.GetAt(0) == 'D') //파생상품 코드개편
				{
					str = str.Right(16).Mid(0,9);

					((CButton*)GetDlgItem(nIndex[n]))->SetWindowText(str);
				}
				else
				{
					str = str.Right(13).Mid(0,6);

					((CButton*)GetDlgItem(nIndex[n]))->SetWindowText(str);
				}

				m_arHNam.SetAt(str,cod);
				m_arHCod.SetAt(cod,nam);
				m_mapHCod.SetAt(cod, gnam);
				n++;

				if (n >= 17)
					break;
			}
		}
	}
}

void CSFCodeDlg::OnChangeEditSearch() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strName;
	
	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(strName);

	if(strName == m_sPreSearch)
	{
		return;
	}

	strName.TrimRight();

	if(strName == "")
	{
		m_NameList.SetCurSel(0);
		return;
	}

	if(m_NameList.GetCurSel() + 1 >= m_NameList.GetCount())
		m_NameList.SetCurSel(0);
	bool bflag = false;
	int i = 0;
	for(i=0;i< m_NameList.GetCount();i++)
	{
		CString text;

		m_NameList.GetText(i,text);

		if(text.Find(strName) > -1)
		{

			m_NameList.SetCurSel(i);
			bflag = true;
			m_sPreSearch = strName;
			OnSelchangeNameList();
			return;
		}
	}

	if(i >= m_NameList.GetCount())
		m_NameList.SetCurSel(0);

	m_sPreSearch = strName;
}

void CSFCodeDlg::OnButtonMth1() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_MTH1)->GetWindowText(hnam);

	if(m_arHNam.Lookup(hnam,code))
	{
		if (m_iTYPE)
		{
			int pid{}, pid2{};
			int ival{};

			char tmp2[3] = {};
			memcpy(tmp2, code.Mid(1, 2), 2);

			if (tmp2[0] >= 'B')
			{
				pid = 61 + (tmp2[0] - 66) * 36;

				if (tmp2[1] >= 'A' && tmp2[1] <= 'Z')
					pid2 = tmp2[1] - 55;
				else
					pid2 = tmp2[1] - 48;

				ival = pid + pid2 + 100;
			}
			else if (tmp2[0] >= '0' && tmp2[0] <= '9')
			{
				pid = (tmp2[0] - 48) * 36;

				if (tmp2[1] >= 'A' && tmp2[1] <= 'Z')
					pid2 = tmp2[1] - 55;
				else
					pid2 = tmp2[1] - 48;

				ival = pid + pid2 + 400;
			}
			else
				ival = atoi(tmp2) + 100;

			m_szCode.Format("%d", ival);

			if (m_mapHCod.Lookup(code, hnam))
				m_szName = hnam;
		}
		else
		{
			m_szCode = code;

			if (m_arHCod.Lookup(code, hnam))
				m_szName = hnam;
		}
	}
	OnOK();
}

void CSFCodeDlg::OnButtonMth2() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_MTH2)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;

		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonMth3() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_MTH3)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonMth4() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_MTH4)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonMth5() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_MTH5)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonMth6() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_MTH6)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonMth7() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_MTH7)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonMth8() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_MTH8)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonMth9() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_MTH9)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonSpr1() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_SPR1)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonSpr2() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_SPR2)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonSpr3() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_SPR3)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonSpr4() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_SPR4)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonSpr5() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_SPR5)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonSpr6() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_SPR6)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonSpr7() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_SPR7)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}
	OnOK();
}

void CSFCodeDlg::OnButtonSpr8() 
{
	// TODO: Add your control notification handler code here
	CString code,hnam;
	GetDlgItem(IDC_BUTTON_SPR8)->GetWindowText(hnam);
	
	if(m_arHNam.Lookup(hnam,code))
	{
		m_szCode = code;
		
		if(m_arHCod.Lookup(code,hnam))
			m_szName = hnam;
	}

	OnOK();
}

BOOL CSFCodeDlg::FindCode( int kind, CString szCode )
{
	if(kind == sfName)
	{
		szCode.TrimRight();   //파생상품 코드개편
		for(size_t i=0;i<m_fjcode.size();i++)
		{
			CString str(m_fjcode[i].codx,sizeof(m_fjcode[i].codx));
			CString hnam(m_fjcode[i].hnam,sizeof(m_fjcode[i].hnam));
			
			if(str == szCode)
			{
				m_szName = hnam;
				return TRUE;
			}
		}
	}
	else if(kind == getMarketsfuture)
	{
		for(size_t i=0;i<m_fjcode.size();i++)
		{
			CString str(m_fjcode[i].codx,sizeof(m_fjcode[i].codx));
			CString mgub(m_fjcode[i].mgub,sizeof(m_fjcode[i].mgub));
			
			if(str == szCode)
			{
				m_szMgubn = mgub;
				return TRUE;
			}
		}
	}
	else if(kind == getCODEsfuture)
	{
		CString str(m_fjcode[0].codx,sizeof(m_fjcode[0].codx));
		m_szCode = str;

		return TRUE;
	}

	return FALSE;
}

void CSFCodeDlg::OnButtonSearchNext() 
{
	// TODO: Add your control notification handler code here
	CString strName;
	
	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(strName);
	
	strName.TrimRight();
	
	if(strName == "")
	{
		m_NameList.SetCurSel(0);
		return;
	}
	
	if(m_NameList.GetCurSel() + 1 >= m_NameList.GetCount())
		m_NameList.SetCurSel(0);
	
	bool bflag = false;
	
	for(int i=m_NameList.GetCurSel()+1;i<m_NameList.GetCount();i++)
	{
		CString text;
		
		m_NameList.GetText(i,text);
		
		if(text.Find(strName) > -1)
		{
			m_NameList.SetCurSel(i);
			bflag = true;

			OnSelchangeNameList();
			return;
		}
	}
}

void CSFCodeDlg::OnButtonSearchPrev() 
{
	// TODO: Add your control notification handler code here
	CString strName;
	
	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(strName);
	
	strName.TrimRight();
	
	if(strName == "")
	{
		m_NameList.SetCurSel(0);
		return;
	}
	
	if(m_NameList.GetCurSel() - 1 <= 0)
		m_NameList.SetCurSel(m_NameList.GetCount()-1);
	
	bool bflag = false;
	
	for(int i=m_NameList.GetCurSel()-1;i>-1;i--)
	{
		CString text;
		
		m_NameList.GetText(i,text);
		
		if(text.Find(strName) > -1)
		{
			m_NameList.SetCurSel(i);
			bflag = true;

			OnSelchangeNameList();
			return;
		}
	}
}


void CSFCodeDlg::OnDblclkNameList()
{
	if (m_iTYPE)
		OnButtonMth1();
}


BOOL CSFCodeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (m_iTYPE)
		{
			CWnd* pWnd = GetFocus();
			if (pWnd && pWnd->GetDlgCtrlID() == IDC_EDIT_SEARCH)
			{
				int nIndex = m_NameList.GetCurSel();
				if (nIndex != LB_ERR)
				{
					CString strText;
					m_NameList.GetText(nIndex, strText);
					CString strName;
					GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(strName);

					if (strText.Find(strName) >= 0)
						OnButtonMth1();
				}
				return TRUE;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

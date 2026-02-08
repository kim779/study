// InterDomino.cpp : implementation file
//

#include "stdafx.h"
#include "ib202200.h"
#include "InterDomino.h"
#include "resource.h"
#include "sharemsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInterDomino dialog

#define MAXCOUNT	32
#define	SINGLEMODE  12
#define DUALMODE    16
#define BOKSU		1320

CInterDomino::CInterDomino(CWnd* pParent, int rowCount, CString data)
: CDialog(CInterDomino::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(CInterDomino)
	m_staticCount = _T("");
	//}}AFX_DATA_INIT

	m_pParent = pParent;
	m_irowCount = rowCount;
	m_dominoData = data;
	
	for(int i=0 ; i<110 ; i++)
	{
		for(int j=0 ; j<4 ; j++)
		{
			m_strCode[i][j] = "";
		}
	}
	
	m_iTempRow = 0;
	m_maxCount = 0;

}


void CInterDomino::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CInterDomino::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInterDomino)
		DDX_Control(pDX, IDC_LISTJM, m_ListBox);
	DDX_Text(pDX, IDC_STATIC_COUNT, m_staticCount);
	DDV_MaxChars(pDX, m_staticCount, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInterDomino, CDialog)
	//{{AFX_MSG_MAP(CInterDomino)
		ON_WM_SYSCOMMAND()
		ON_WM_PAINT()
		ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BTN_CHKALL, OnBtnChkall)
	ON_BN_CLICKED(ID_BTN_ALLCANCEL, OnBtnAllcancel)
	ON_BN_CLICKED(ID_BTN_CHKKOSPI, OnBtnChkkospi)
	ON_BN_CLICKED(ID_BTN_CHKKOSDAQ, OnBtnChkkosdaq)
	ON_LBN_SELCHANGE(IDC_LISTJM, OnSelchangeListjm)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CInterDomino, CDialog)
	//{{AFX_DISPATCH_MAP(CInterDomino)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IInterDomino to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {2940D37A-BBB2-4B4E-BC8C-AA929BFD52F4}
static const IID IID_IInterDomino =
{ 0x2940d37a, 0xbbb2, 0x4b4e, { 0xbc, 0x8c, 0xaa, 0x92, 0x9b, 0xfd, 0x52, 0xf4 } };

BEGIN_INTERFACE_MAP(CInterDomino, CDialog)
	INTERFACE_PART(CInterDomino, IID_IInterDomino, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInterDomino message handlers
void CInterDomino::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

HCURSOR CInterDomino::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CInterDomino::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
// 		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
// 		
// 		// Center icon in client rectangle
// 		int cxIcon = GetSystemMetrics(SM_CXICON);
// 		int cyIcon = GetSystemMetrics(SM_CYICON);
// 		CRect rect;
// 		GetClientRect(&rect);
// 		int x = (rect.Width() - cxIcon + 1) / 2;
// 		int y = (rect.Height() - cyIcon + 1) / 2;
// 		
// 		// Draw the icon
// 		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}



BOOL CInterDomino::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_pView = (CWnd*)m_pParent->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));

	CString code, name, type;
	int codeType{};

	//일단 sdi 모드인지 mdi 모드인지 판단하고
	//sdi모드일 경우엔 12개만 , mdi모드일 경우엔 사이즈 판단해서 16개 까지 

	CRect rc;
	CWnd* pWnd = nullptr;
	
	pWnd = m_pParent->GetParent()->GetParent()->GetParent();
	
	if(pWnd == nullptr)
	{
		m_maxCount = SINGLEMODE;
	}
	else
	{
		pWnd->GetWindowRect(&rc);

		if(rc.Width() > BOKSU)
		{
			m_maxCount = DUALMODE;
		}
		else
		{
			m_maxCount = SINGLEMODE;
		}
	}
	
	for(int ii=0 ; ii<m_irowCount ; ii++)
	{

		code = IH::Parser(m_dominoData, "|");	
		name = GetCodeName(code);
		codeType = GetCodeType(code);

		type.Format("%d", codeType);

		m_strCode[ii][0] = code;
		m_strCode[ii][1] = name;
		m_strCode[ii][2] = type;

		m_ListBox.AddString(name);
	}
	

	for(int ii=0 ; ii<m_maxCount ; ii++)
	{
		m_ListBox.SetCheck(ii, 1);
		m_strCode[ii][3] = "1";
	}

	m_staticCount.Format("%d", m_maxCount);
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

CString CInterDomino::GetCodeName(CString code)
{
	CString	ret = _T("");
	CString	sztmp;
	sztmp.Format("%s", code);
	ret.Format("%s", (char *)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)sztmp.operator LPCTSTR()));
	ret.TrimLeft(), ret.TrimRight();
	return ret;
}

int CInterDomino::GetCodeType(CString code)
{
	const int codeL = code.GetLength();
	if (codeL <= 0) return 0;
	
	const int result = m_pView->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)code);
	
	switch (result)
	{
	case kospiType:				// kospi code
		return kospiType;
	case kosdaqType:			// kosdaq code
		return kosdaqType;
	case etfType:				// etf code
//		return	kospiCODE;
	case elwType:				// ELW
		return	elwCODE;
	case futureType:
		return	futureCODE;		// 선물
	case sfutureType:
		return sfCODE;		
	case callType:				// call option
	case putType:				// put  option
		return  optionCODE;
	case indexType:				// 업종
		return	indexCODE;
	case thirdType:				// 제3시장
		return	thirdCODE;
	case sinjuType:				// 신주인수권
		return	sinjuCODE;
	case mufundType:			// 뮤츄얼펀드
	case reitsType:				// 리츠
	case hyfundType:			// 하이일드펀드
		return 200;
	}
	
	return 0;
}


void CInterDomino::OnOK() 
{
	// TODO: Add extra validation here
	char rowCount[2]{};
	CString string(_T("")), string_Head(_T("")), string_Data(_T("")), data(_T(""));
	int nScnt = 0;
	constexpr char chTab = P_TAB;
	
	string_Head = "IB202200"; 
	
	for(int ii=0 ; ii<m_irowCount ; ii++)
	{
		if(m_ListBox.GetCheck(ii))
		{
			if(nScnt < m_maxCount)
			{
				string_Data += m_strCode[ii][0];
				string_Data += "|";	
			}
			else
			{
				CString msg;
				msg.Format("최대 %d개까지만 종목 연동이 가능합니다", m_maxCount);	
				
				MessageBox(msg, "IBK투자증권");
				break;
			}
		
			nScnt++;
		}
	}

	if(nScnt>0)
	{
		itoa(nScnt, rowCount, 10);
		
		string = string_Head + chTab + (CString)rowCount + chTab + string_Data;
		
		data.Format("IB201600 /p5 /S/d%s", string);
		m_pParent->SendMessage(WM_MANAGE, MAKEWPARAM(MK_OPENSCR, typeVIEW), (LPARAM)data.operator LPCTSTR());	
	}
	else
	{
		MessageBox("선택된 종목이 없습니다.", "IBK투자증권");
	}

	CDialog::OnOK();
}

void CInterDomino::OnBtnChkall() 
{
	// TODO: Add your control notification handler code here
	for(int ii=0 ; ii<m_irowCount ; ii++)
	{
		m_ListBox.SetCheck(ii, 0);	
		m_strCode[ii][3].Empty();
	}	

	for(int ii=0 ; ii<m_maxCount ; ii++)
	{
		m_ListBox.SetCheck(ii, 1);	
		m_strCode[ii][3] = "1";
	}	

	if(m_irowCount > m_maxCount)
	{
		CString msg;
		msg.Format("최대 %d개까지만 종목 연동이 가능합니다", m_maxCount);	

		MessageBox(msg, "IBK투자증권");
	}
	
	m_staticCount.Format("%d", m_maxCount);
	UpdateData(FALSE);
}

void CInterDomino::OnBtnAllcancel() 
{
	// TODO: Add your control notification handler code here
	for(int ii=0 ; ii<m_irowCount ; ii++)
	{
		m_ListBox.SetCheck(ii, 0);	
		m_strCode[ii][3].Empty();
	}	

	m_staticCount.Format("0");
	UpdateData(FALSE);
}

void CInterDomino::OnBtnChkkospi() 
{
	// TODO: Add your control notification handler code here
	

	for(int ii=0 ; ii<m_irowCount ; ii++)
	{
		m_ListBox.SetCheck(ii, 0);
		m_strCode[ii][3].Empty();
	}	

	int count = 0;

	for(int ii=0 ; ii<m_irowCount ; ii++)
	{
		if(count < m_maxCount)
		{
			if(atoi(m_strCode[ii][2]) == kospiType)
			{
				m_ListBox.SetCheck(ii, 1);
				m_strCode[ii][3] = "1";
				count++;
			}
						
		}
		else
		{
			CString msg;
			msg.Format("최대 %d개까지만 종목 연동이 가능합니다", m_maxCount);	
			
			MessageBox(msg, "IBK투자증권");
			break;
		}
	}

	m_staticCount.Format("%d", count);
	UpdateData(FALSE);

}

void CInterDomino::OnBtnChkkosdaq() 
{
	// TODO: Add your control notification handler code here
	
	
	for(int ii=0 ; ii<m_irowCount ; ii++)
	{
		m_ListBox.SetCheck(ii, 0);	
		m_strCode[ii][3].Empty();
	}	

	int count = 0;

	for(int ii=0 ; ii<m_irowCount ; ii++)
	{
		if(count < m_maxCount)
		{
			if(atoi(m_strCode[ii][2]) == kosdaqType)
			{
				m_ListBox.SetCheck(ii, 1);
				m_strCode[ii][3] = "1";
				count++;
			}
						
		}
		else
		{
			CString msg;
			msg.Format("최대 %d개까지만 종목 연동이 가능합니다", m_maxCount);	
			
			MessageBox(msg, "IBK투자증권");
			break;
		}
	}	

	m_staticCount.Format("%d", count);
	UpdateData(FALSE);
}

void CInterDomino::OnSelchangeListjm() 
{
	// TODO: Add your control notification handler code here
	int count = 0;
	int index = m_ListBox.GetCurSel();

	for(int ii=0 ; ii<m_irowCount ; ii++)
	{
		if(!m_strCode[ii][3].IsEmpty())
		{
			count++;
		}

	}


	if(!m_strCode[index][3].IsEmpty())
	{
		m_ListBox.SetCheck(index, 0);
		m_strCode[index][3].Empty();

		m_staticCount.Format("%d", count-1);
		UpdateData(FALSE);
	}
	else
	{
		if(count >= m_maxCount)
		{
			CString msg;
			msg.Format("최대 %d개까지만 종목 연동이 가능합니다", m_maxCount);	
			
			MessageBox(msg, "IBK투자증권");

			m_ListBox.SetCheck(index, 0);
			m_strCode[index][3].Empty();

			m_staticCount.Format("%d", count);
			UpdateData(FALSE);
		}
		else
		{
			m_ListBox.SetCheck(index, 1);
			m_strCode[index][3] = "1";

			m_staticCount.Format("%d", count+1);
			UpdateData(FALSE);
		}
	}
}

void CInterDomino::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	

	CDialog::OnTimer(nIDEvent);
}

// AccountConfirmWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB300900.h"
#include "AccountConfirmWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccountConfirmWnd dialog


CAccountConfirmWnd::CAccountConfirmWnd(CWnd* pParent /*=NULL*/,AccountMap *pAccount)
	: CDialog(CAccountConfirmWnd::IDD, pParent),CAxisExt(pParent)
{
	//{{AFX_DATA_INIT(CAccountConfirmWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pAccount = pAccount;

	m_mapAccnt.RemoveAll();
	m_accntInfo.RemoveAll();
}


void CAccountConfirmWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccountConfirmWnd)
	DDX_Control(pDX, IDC_LIST_OWN2, m_listNOwn);
	DDX_Control(pDX, IDC_LIST_OWN, m_listOwn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccountConfirmWnd, CDialog)
	//{{AFX_MSG_MAP(CAccountConfirmWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountConfirmWnd message handlers

BOOL CAccountConfirmWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CAccountConfirmWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_listOwn.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	
	
    LV_COLUMN lCol;
    lCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH; 
	
    lCol.fmt = LVCFMT_CENTER;   // 칼럼의 정렬을 왼쪽정렬로 지정(LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_RIGHT중 선택)
    lCol.pszText = "계좌번호";    //  칼럼의 제목을 지정
    lCol.iSubItem = 0;        //  서브아이템의 인덱스를 지정
    lCol.cx = 140;               //  칼럼의 넓이를 지정
    m_listOwn.InsertColumn(0, &lCol);  //  LVCOLUMN구조체로 만들어진 값을 토대로 리스트 컨트롤에 칼럼을 삽입

	m_listNOwn.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	
	
    lCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH; 
	
    lCol.fmt = LVCFMT_CENTER;   // 칼럼의 정렬을 왼쪽정렬로 지정(LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_RIGHT중 선택)
    lCol.pszText = "계좌번호";    //  칼럼의 제목을 지정
    lCol.iSubItem = 0;        //  서브아이템의 인덱스를 지정
    lCol.cx = 140;               //  칼럼의 넓이를 지정
    m_listNOwn.InsertColumn(0, &lCol);  //  LVCOLUMN구조체로 만들어진 값을 토대로 리스트 컨트롤에 칼럼을 삽입

	CString accnInfo = Variant(accountCC,"");
	
	ParseAccntInfo(accnInfo);

	AccountMap::iterator st = m_pAccount->begin();
	AccountMap::iterator ed = m_pAccount->end();

	for(;st!=ed;++st)
	{
		CString acno = st->first.c_str();
		
		CString tmp;

		if(m_mapAccnt.Lookup(acno,tmp))
		{
			CAccountInfo accInfo;

			if(tmp == "1")
			{
				st->second.IsOwn = TRUE;

				int n = m_listOwn.GetItemCount();

				m_listOwn.InsertItem(n,NULL);

				m_listOwn.SetItemText(n,0,acno);
			}
			else
			{
				int n = m_listNOwn.GetItemCount();

				m_listNOwn.InsertItem(n,NULL);
				
				m_listNOwn.SetItemText(n,0,acno);
			}
		}
		else
		{
			int n = m_listNOwn.GetItemCount();
			
			m_listNOwn.InsertItem(n,NULL);
			
			m_listNOwn.SetItemText(n,0,acno);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CAccountConfirmWnd::SplitString(CString &strData, CString strToken)
{
	CString sResult = strData;
	if(strToken == "") return sResult;
	int iFind = strData.Find(strToken); 
	if(iFind > -1 ) 
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
	}
	
	
	return sResult;
}

void CAccountConfirmWnd::ParseAccntInfo(CString data)
{
	CString accnt,optf;
	int posAcct = 0;
	int posFlag = 0;
	
	int pos = data.Find("\n");
	
	CString s;
	s.Format("[ACCOUNT INFO : %s , %d]\n",data,pos);
	//OutputDebugString(s);
	
	while(pos >= 0)
	{
		CString accntLine = SplitString(data,"\n");
		
		accnt = accntLine.Left(11);
		
		posFlag = accntLine.ReverseFind('|');
		
		optf = accntLine.Right(1);
		
		if(optf == "|")
		{
			optf = "0";
		}
		
		m_mapAccnt.SetAt(accnt,optf);

		s.Format("[ACCOUNT : %s , %s]\n",accnt,optf);
		//OutputDebugString(s);
		
		pos = data.Find("\n");
	}
	
	if(data != "")
	{
		CString accntLine = SplitString(data,"\n");

		accnt = accntLine.Left(11);
		
		posFlag = accntLine.ReverseFind('|');
		
		optf = accntLine.Right(1);
		
		if(optf == "|" || optf == "")
		{
			optf = "0";
		}
		
		m_mapAccnt.SetAt(accnt,optf);
	}
}

void CAccountConfirmWnd::OnOK() 
{	
	CDialog::OnOK();
}

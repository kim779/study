// DlgSysVarOption.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSysVarOption.h"
#include ".\dlgsysvaroption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysVarOption dialog


CDlgSysVarOption::CDlgSysVarOption(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysVarOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSysVarOption)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pInputList = NULL;
}


void CDlgSysVarOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysVarOption)
	DDX_Control(pDX, IDC_BT_CLR_BULL, m_btnClrBull);
	DDX_Control(pDX, IDC_BT_CLR_BEAR, m_btnClrBear);
	DDX_Control(pDX, IDC_LIST_STVARIABLES, m_InputList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysVarOption, CDialog)
	//{{AFX_MSG_MAP(CDlgSysVarOption)
	ON_BN_CLICKED(IDC_BT_CLR_BULL, OnBtClrBull)
	ON_BN_CLICKED(IDC_BT_CLR_BEAR, OnBtClrBear)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysVarOption message handlers
BOOL CDlgSysVarOption::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitColumn();
	DoApplyInputParam();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysVarOption::InitColumn()
{
	int colwidthsType0[6] = { 90, 127};	// sixty-fourths

	TCHAR *	lpszHeadersType0[] = {
		_T("변수명"),
			_T("변수값"),
			NULL };
		m_InputList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

		int i;
		int total_cx = 0;
		LV_COLUMN lvcolumn;
		memset(&lvcolumn, 0, sizeof(lvcolumn));
		// add columns
		for (i = 0; ; i++)
		{
			if (lpszHeadersType0[i] == NULL)
				break;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_CENTER;
			lvcolumn.pszText = lpszHeadersType0[i];
			lvcolumn.iSubItem = i;
			lvcolumn.cx = colwidthsType0[i];
			total_cx += lvcolumn.cx;
			m_InputList.InsertColumn(i, &lvcolumn);
		}
		// iterate through header items and attach the image list
		memset(&lvcolumn, 0, sizeof(lvcolumn));
		// set the format again - must do this twice or first column does not get set
		for (i = 0; ; i++)
		{
			if (lpszHeadersType0[i] == NULL)
				break;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
			lvcolumn.fmt = LVCFMT_CENTER;
			lvcolumn.iSubItem = i;
			m_InputList.SetColumn(i, &lvcolumn);
		}
		m_InputList.SetHilightColor(RGB(0,0,0),RGB(208,221,240));
}

void CDlgSysVarOption::SetParam(CStringList* p_dwInputt, LONG dwOutput)
{
	m_pInputList = p_dwInputt;	
}

void CDlgSysVarOption::DoBtnClr(CColorButton &rColorButton)
{
	COLORREF crColor = rColorButton.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		rColorButton.SetColor(color.GetColor());				
		rColorButton.Invalidate();
	}
}

void CDlgSysVarOption::OnBtClrBull() 
{
	DoBtnClr(m_btnClrBull);
}

void CDlgSysVarOption::OnBtClrBear() 
{
	DoBtnClr(m_btnClrBear);
}


void CDlgSysVarOption::DoApplyInputParam()
{
	if(m_pInputList)
	{
		CString szTmp, szKey, szValue;
		int nFind;
		for(POSITION pos=m_pInputList->GetHeadPosition(); pos;)
		{
			szTmp = m_pInputList->GetNext(pos);
			nFind = szTmp.Find('=');
			if(nFind<0) continue;

			szKey = szTmp.Left(nFind);
			szValue = szTmp.Mid(nFind+1);
			if(szKey.CompareNoCase("STNAME")==0)
			{
				if(szValue.Left(2) == "P_")
					szValue = szValue.Mid(2);
				nFind = szValue.Find(gSTGubunKey);
				if(nFind>0) szValue = szValue.Left(nFind);
				SetDlgItemText(IDC_STNAME, szValue);
			}
			else if(szKey.CompareNoCase("SFGPATH")==0)
			{
				nFind = szValue.Find(gSTGubunKey);
				if(nFind>0) szValue = szValue.Left(nFind);
				m_szSFGPath.Format("%s%s", szValue, ".sfg");
				//	DoLoadVarList(m_szSFGPath);
			}
			else if(szKey.CompareNoCase("STPATH")==0)
			{
				m_szSTPath = szValue;
				DoLoadVarList(m_szSTPath);
			}
		}
	}

	char aData[30];
	LPCSTR szSection = "index";

	//DWORD clrBull = RGB(255, 229, 219);	// 14411263
	//DWORD clrBear = RGB(230, 249, 255);	// 16775654
	//sprintf(aData, "%ld", clrBull);
	::GetPrivateProfileString(szSection, "clrBull", "14476026", aData, 30, m_szSFGPath);
	m_btnClrBull.SetColor(atol(aData));

	//sprintf(aData, "%ld", clrBear);
	::GetPrivateProfileString(szSection, "clrBear", "16248787", aData, 30, m_szSFGPath);
	m_btnClrBear.SetColor(atol(aData));
}

void CDlgSysVarOption::DoLoadVarList(LPCSTR szDataFile)
{
	LPCSTR szSection = "Variables";
	int nCnt = ::GetPrivateProfileInt(szSection, "InputCnt", 0, szDataFile);
	if(nCnt<=0) return;

	CString szKey;
	char aTmp[MAX_PATH]={0,};
	int nAddRow;
	CStringArray inArr;

	for(int i=0; i<nCnt; i++)
	{
		szKey.Format("InputName%d", i);
		::GetPrivateProfileString(szSection, szKey, "", aTmp, MAX_PATH, szDataFile);

		nAddRow = m_InputList.InsertItem(m_InputList.GetItemCount(), "");
		m_InputList.SetItemText(nAddRow, 0, aTmp);

		szKey.Format("Input%d", i);
		::GetPrivateProfileString(szSection, szKey, "", aTmp, MAX_PATH, szDataFile);
		m_InputList.SetItemText(nAddRow, 1, aTmp);

		m_InputList.SetEditBox(nAddRow,1);
	}
}

void CDlgSysVarOption::OnBnClickedOk()
{
	int nCnt = m_InputList.GetItemCount();
	LPCSTR szSection = "Variables", szPreKey="Input";
	CString szValue, szKey;

	for(int i=0; i<nCnt; i++)
	{
		szValue = m_InputList.GetItemText(i, 1);
		szKey.Format("%s%d", szPreKey, i);
		::WritePrivateProfileString(szSection, szKey, szValue, m_szSTPath);
	}

	szSection = "index";
	szValue.Format("%ld", m_btnClrBull.GetColor());
	::WritePrivateProfileString(szSection, "clrBull", szValue, m_szSFGPath);

	szValue.Format("%ld", m_btnClrBear.GetColor());
	::WritePrivateProfileString(szSection, "clrBear", szValue, m_szSFGPath);

	OnOK();
}

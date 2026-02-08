// SecCalc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SecCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	SECTION	"CALC"
/////////////////////////////////////////////////////////////////////////////
// CSecCalcApp

BEGIN_MESSAGE_MAP(CSecCalcApp, CWinApp)
	//{{AFX_MSG_MAP(CSecCalcApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecCalcApp construction

CSecCalcApp::CSecCalcApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSecCalcApp object

CSecCalcApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSecCalcApp initialization

BOOL CSecCalcApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	m_pFeeRate[0] = Fee_Stock;
	m_pFeeRate[1] = Fee_Future;
	m_pFeeRate[2] = Fee_Option;
	m_pFeeRate[3] = Fee_Elw;

	LoadInfo();

	CSecCalcDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CSecCalcApp::ExitInstance() 
{
	return CWinApp::ExitInstance();
}

void CSecCalcApp::LoadInfo()
{
	char	pPath[128];
	GetCurrentDirectory(128, pPath);
	m_sPath.Format("%s\\calc.ini", pPath);

	m_sKey[0] = "Stock";
	m_sKey[1] = "Future";
	m_sKey[2] = "Option";
	m_sKey[3] = "Elw";

	char	twb[512];
	for (int ii = 0; ii < 4; ii++)
	{
		GetPrivateProfileString(SECTION, m_sKey[ii], "", twb, 512, m_sPath);
		if (strlen(twb) > 0)	m_pFeeRate[ii] = twb;
	}
	for (ii = 0; ii < 4; ii++)
	{
		GetPrivateProfileString(SECTION, m_sKey[ii]+"_2", "", twb, 512, m_sPath);
		if (strlen(twb) > 0)	m_pCharFee[ii] = twb;
	}
}

void CSecCalcApp::SaveInfo()
{
	for (int ii = 0; ii < 4; ii++)
		WritePrivateProfileString(SECTION, m_sKey[ii], m_pFeeRate[ii], m_sPath);
	for (ii = 0; ii < 4; ii++)
		WritePrivateProfileString(SECTION, m_sKey[ii]+"_2", m_pCharFee[ii], m_sPath);
}

CString CSecCalcApp::editComma(double dVal, bool bInt)
{
	CString tmpS = _T("");
	if (dVal == 0.0)
	{
		tmpS = _T("0");
		return tmpS;
	}

	CString dotRight;

	bool bMinus = (dVal < 0) ? true : false;
	double val = (dVal > 0) ? dVal:-dVal;
	
	CString tmp;
	
	if (bInt)
		tmp.Format("%.0f", val);
	else
		tmp.Format("%f", val);

	int find = tmp.Find('.');
	if (find > 0)  
	{
		dotRight = tmp.Right(tmp.GetLength() - (find));
		tmp = tmp.Left(find);
	}
	else 
	{
		dotRight.Empty();
	}

	int idx = 0, jj = 1;
	for (int ii = tmp.GetLength() - 1; ii >= 0 ;ii--, jj++)
	{
		tmpS.Insert(0, tmp.GetAt(ii));
		if (jj % 3 == 0 && ii > 0)
			tmpS.Insert(0, ",");
	}
	if (bMinus)
		tmpS = "-" + tmpS;

	tmpS += dotRight;
	if (!bInt)
		tmpS.TrimRight('0');

	return tmpS;
}

double CSecCalcApp::CalcBuyCharge(int market, double dCnt, double dPrice, double &dFee, double &dTax)
{
	if (market == mk_Future)
		dPrice *= 500000;
	else if (market == mk_Option)
		dPrice *= 100000;

	double	dTotal = dCnt * dPrice;
	double	dFeeRate = atof(m_pFeeRate[market])*0.01;
	dFee = dTotal * dFeeRate ;
	dFee = int(dFee / 10)*10+ atoi(m_pCharFee[market]);
	dTax = 0;

	return dTotal + dFee;
}

double CSecCalcApp::CalcSellCharge(int market, double dCnt, double dPrice, double &dFee, double &dTax)
{
	if (market == mk_Future)
		dPrice *= 500000;
	else if (market == mk_Option)
		dPrice *= 100000;

	double	dTotal = dCnt * dPrice;
	double	dFeeRate = atof(m_pFeeRate[market])*0.01;
	double	dTaxRate = atof(Fee_Tax);
	dFee = dTotal * dFeeRate;
	dFee = int(dFee / 10)*10+ atoi(m_pCharFee[market]);
	dTax = 0;
	if (market == mk_Stock)
		dTax = dTotal * dTaxRate;
	else
		dTax = 0;

	dTax = floor(dTax);
	return dTotal - dFee - dTax;
}

void CSecCalcApp::CorrectString(CWnd *pWnd, int market)
{
	if (market != mk_Future && market != mk_Option)
	{
		CString	str;
		pWnd->GetWindowText(str);
		int	index = str.Find(".");
		if (index > 0)
		{
			str = str.Left(index);
			pWnd->SetWindowText(str);
		}
	}
}
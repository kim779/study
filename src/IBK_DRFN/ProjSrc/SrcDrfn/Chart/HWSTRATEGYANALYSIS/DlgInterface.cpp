// DlgInterface.cpp : implementation file
//

#include "stdafx.h"

#include "DlgInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInterface dialog
 

CDlgInterface::CDlgInterface(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CEscDialog( nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CDlgInterface)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pInput = NULL;
	m_bIsJusik = TRUE;
}


void CDlgInterface::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInterface)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInterface, CDialog)
	//{{AFX_MSG_MAP(CDlgInterface)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInterface message handlers

CString CDlgInterface::ParsingComma(CString strData)
{
	int nSize;
	nSize = strData.GetLength();

	for(int i = nSize-3;i>0;i-=3)
		strData.Insert(i,",");
	return strData;
}

CString  CDlgInterface::GetNumber( LPCSTR pData )
{
	CString szRet=_T("");
	int nLen = strlen(pData);
	char	ch;

	for(int i=0;  i<nLen ; i++) {
		ch = pData[i];
		if(ch >= '0' && ch <= '9'||ch == '.' || ch == '-' || ch== '+') 
			szRet += ch;
	}
	return szRet;
}

CString CDlgInterface::GetComma(LPCSTR pData)
{
	//KHD : 오류수정 
	double ddata = atof(pData);
	if(ddata ==0)return "0";
	CString szRet=_T("");
	CString szTmp = pData;
	CString szTmp2;
	szTmp.TrimLeft();
	szTmp.TrimRight();

	szTmp = GetNumber(szTmp);
	if(szTmp.IsEmpty())	return "" ;
	TCHAR ch=' ';
	if(szTmp[0]=='+' ||szTmp[0]=='-') 
	{
		ch = szTmp[0];
		szTmp = szTmp.Mid(1);
	}
	BOOL IsFloat;
	int Index =szTmp.Find(".",0); 
	if(Index>0)
		IsFloat = TRUE;
	else
		IsFloat = FALSE;

	int	nLen;
	if(IsFloat)//소숫점 자리이면... 이렇게한다.
	{
		szTmp2 = szTmp.Mid(Index,szTmp.GetLength()-Index);
		szTmp.Delete(Index,szTmp.GetLength()- Index);

	}
	szTmp.MakeReverse();
	nLen= szTmp.GetLength();
	szRet.Empty();
	for(int i=0;   i<nLen;   i++) 
	{
		if(i!=0 && !(i%3))	szRet += ',';
		szRet += szTmp[i];
	}
	if(ch=='-') szRet += ch;
	szRet.MakeReverse();
	if(IsFloat)
		szRet+=szTmp2;
	return szRet;

}



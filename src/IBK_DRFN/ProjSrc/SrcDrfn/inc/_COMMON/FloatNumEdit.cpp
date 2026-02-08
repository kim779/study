// FloatNumEdit.cpp : implementation file
//

#include "stdafx.h"
#include "FloatNumEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		MAX_BUF     1024
/////////////////////////////////////////////////////////////////////////////
// CFloatNumEdit

CFloatNumEdit::CFloatNumEdit()
{
	m_dMin		= 0.0;
	m_dMax		= 0.0;
	m_bFloat	= FALSE;
}

CFloatNumEdit::~CFloatNumEdit()
{
}


BEGIN_MESSAGE_MAP(CFloatNumEdit, CEdit)
	//{{AFX_MSG_MAP(CFloatNumEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloatNumEdit message handlers

void CFloatNumEdit::OnChange() 
{
	CString strText;
	GetWindowText(strText);

	if ( m_strPrvText != strText )
	{
		CheckNumeric ( strText );
	}
}

void CFloatNumEdit::CheckNumeric(CString &strNum)
{
	if ( strNum.GetLength() > MAX_BUF ) return;

	char	cBuff[MAX_BUF + 1];
	int		nCurBuf = 0;
	bool	bChange = FALSE;
	bool    bDot	= FALSE;
	double  dCur;
	CString strNewText;

	memset ( cBuff, ' ', MAX_BUF );

	for ( int i = 0 ; i < strNum.GetLength() ; i++ )
	{
		if ( ( strNum[i] >= 48 && strNum[i] <= 57 ) )
		{
			cBuff[nCurBuf] = strNum[i];
			nCurBuf++;
		}
		else if ( strNum[i] == '.' && m_bFloat && !bDot && i != 0 )
		{
			cBuff[nCurBuf] = strNum[i];
			nCurBuf++;	
			bDot = TRUE;
		}
		else
		{
			bChange = TRUE;
		}
	}

	cBuff[nCurBuf] = '\0';
	strNewText = cBuff;

	dCur = atof (strNewText);

	if ( dCur < m_dMin || dCur > m_dMax )
	{
		SetWindowText ( m_strPrvText );
		SetSel(strNewText.GetLength(),-1); 		
		return;
	}

	if ( bChange )
	{
		SetWindowText ( strNewText );
		SetSel(strNewText.GetLength(),-1); 
	}
	m_strPrvText = strNewText;
}


BOOL CFloatNumEdit::EnableFloat	( BOOL bFloat )
{
	m_bFloat = bFloat;

	return m_bFloat;
}

void CFloatNumEdit::SetMaxMinVal( double dMin, double dMax )
{
	m_dMin = dMin;
	m_dMax = dMax;
}

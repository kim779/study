// MiniCalculator.cpp: implementation of the CMiniCalculator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SecCalc.h"
#include "MiniCalculator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMiniCalculator::CMiniCalculator()
{
	m_pWnd = NULL;
	m_Operator = ck_None;
	m_sLVal.Empty();
	m_sRVal.Empty();
	m_dLVal = 0;
	m_dRVal = 0;
	m_bDotState = false;
	m_bLValState = false;
}

CMiniCalculator::~CMiniCalculator()
{
}

void CMiniCalculator::Init(CWnd *pEditWnd)
{
	m_pWnd = pEditWnd;
	m_pApp = (CSecCalcApp *)AfxGetApp();
}

void CMiniCalculator::Calc_Key(int key, int kind)
{
	CString	str;
	switch (kind)
	{
	case ck_Char:	
		str.Format("%c", key);
		Calc_Char(str);
		break;
	default:
		Calc_Enter();
		Calc_Oper(kind);
		break;
	}
}

void CMiniCalculator::Calc_Char(CString sChar)
{
	if (NULL == m_pWnd)
		return;

	if (m_Operator != ck_None && m_sRVal.IsEmpty())
		m_pWnd->SetWindowText("");

	CString	str;
	m_pWnd->GetWindowText(str);
	if (sChar == _T(".") && (str.Find(".")!=-1)) return;
	/*
	if (sChar == _T("."))			// only one '.' 
	{
		m_bDotState = true;
		//return;
	}

	if (!m_bDotState)
		str.TrimRight('.');
	*/
	str = str + sChar;
	
	//FormatString(str);
	FormatString2(str);


	m_pWnd->SetWindowText(str);
	if (m_Operator == ck_None)
	{
		m_bLValState = true;
		m_pWnd->GetWindowText(m_sLVal);
	}
	else
	{
		m_bLValState = false;
		m_pWnd->GetWindowText(m_sRVal);
	}
}

void CMiniCalculator::Calc_Sign()
{
	if (NULL == m_pWnd)
		return;

	CString	str;
	m_pWnd->GetWindowText(str);
	if (!str.IsEmpty())
	{
		if (str.GetAt(0) == '-')
			str = str.Mid(1);
		else
			str = _T("-") + str;
		m_pWnd->SetWindowText(str);
	}
}
// 모두 초기화
void CMiniCalculator::Calc_C()
{
	m_sLVal = m_sRVal = _T("");
	m_dLVal = m_dRVal = 0;
	m_Operator = ck_None;
	m_bDotState = false;
	m_bLValState = false;

	if (m_pWnd)
		m_pWnd->SetWindowText("");
}
// 이전값 삭제
// [A oper B] -> delete B
// [A]        -> delete All
// [A oper]   -> delete All
void CMiniCalculator::Calc_CE()
{
	if (m_sRVal.IsEmpty())
	{
		m_sLVal.Empty();
		m_dLVal = 0;
		m_Operator = ck_None;
		m_bDotState = false;
		m_bLValState = false;
	}
	else
	{
		m_sRVal.Empty();
	}
}

void CMiniCalculator::Calc_Back()
{
	CString	str;
	m_pWnd->GetWindowText(str);
	if (str.IsEmpty())
		return;

	int	index = str.Find(".");
	if (m_bLValState)
	{
		if (m_sLVal.IsEmpty())
			return;

		if (index == str.GetLength()-1)
		{
			if (m_bDotState)
			{
				m_bDotState = false;
			}
			else
			{
				str.Remove('.');
				str = str.Left(str.GetLength()-1);
				FormatString(str);
			}
		}
		else
		{
			str = str.Left(str.GetLength()-1);
			FormatString(str);
		}
		m_sLVal = str;
		m_pWnd->SetWindowText(str);
	}
	else
	{
		if (m_sRVal.IsEmpty())
			return;

		if (index == str.GetLength()-1)
		{
			if (m_bDotState)
			{
				m_bDotState = false;
			}
			else
			{
				str.Remove('.');
				str = str.Left(str.GetLength()-1);
				FormatString(str);
			}
		}
		else
		{
			str = str.Left(str.GetLength()-1);
			FormatString(str);
		}
		m_sRVal = str;
		m_pWnd->SetWindowText(str);
	}
}

void CMiniCalculator::Calc_Oper(int op)
{
	
	if (NULL == m_pWnd)
		return;
	Calc_Enter();
	m_bDotState = false;
	if (m_Operator == ck_None)
	{
		//m_pWnd->GetWindowText(m_sLVal);
		m_Operator = op;
		return;
	}
	if (m_sRVal.IsEmpty())
		return;

	double dRes;
	CString	sLVal = m_sLVal;
	CString	sRVal = m_sRVal;
	sLVal.Remove(',');
	sRVal.Remove(',');
	switch (m_Operator)
	{
	case ck_Plus:
		dRes = atof(sLVal) + atof(sRVal);
		break;
	case ck_Minus:
		dRes = atof(sLVal) - atof(sRVal);
		break;
	case ck_Mul:
		dRes = atof(sLVal) * atof(sRVal);
		break;
	case ck_Div:
		sRVal.TrimRight('0');
		if (sRVal == _T("0."))
			dRes = 0;
		else
			dRes = atof(sLVal) / atof(sRVal);
		break;
	}

	CString	str;
	str.Format("%f", dRes);
	
	SetResult(str, dRes);
	m_Operator = op;
}

void CMiniCalculator::Calc_Enter()
{
	if (m_sRVal.IsEmpty())
		return;

	double dRes;
	CString	sLVal = m_sLVal;
	CString	sRVal = m_sRVal;
	sLVal.Remove(',');
	sRVal.Remove(',');
	switch (m_Operator)
	{
	case ck_Plus:
		dRes = atof(sLVal) + atof(sRVal);
		break;
	case ck_Minus:
		dRes = atof(sLVal) - atof(sRVal);
		break;
	case ck_Mul:
		dRes = atof(sLVal) * atof(sRVal);
		break;
	case ck_Div:
		//sRVal.TrimRight('0');
		//AfxMessageBox("sLVal: "+sLVal);
		//AfxMessageBox("sRVal: "+sRVal);
		if (sRVal == _T("0."))
			dRes = 0;
		else
			dRes = atof(sLVal) / atof(sRVal);
		break;
	default:
		return;
	}

	CString	str;
	str.Format("%f", dRes);
	
	SetResult(str, dRes);
}

void CMiniCalculator::SetResult(CString str, double dRes)
{
	str.TrimRight('0');			// remove '0' from xxx.xx000
//	int index = str.Find(".");
//	if (index == str.GetLength() - 1)	// remove '.' from xxxx.
//		str.TrimRight('.');
	//FormatString2(str);
	FormatString(str);

	m_pWnd->SetWindowText(str);
	m_sLVal = str;
	m_dLVal = dRes;

	m_sRVal.Empty();
	m_dRVal = 0;
	m_Operator = ck_None;
	m_bLValState = false;
}

void CMiniCalculator::FormatString(CString &str)
{
	str.Remove(',');

	str = m_pApp->editComma(atof(str), false);
	
	if (str.Find('.') < 0)
		str += ".";			// add '.' at last 
	
}

void CMiniCalculator::FormatString2(CString &str)
{
	str.Remove(',');
	CString str1=""; CString str2="";
	int ifind = str.Find(".");
	if (ifind != -1)
	{
		str1 = str.Left(ifind);
		str2 = str.Mid(ifind);
		//if (atoi(str2.Mid(2))==0) str2 = "";
	}
	else
	{
		str1 = str;
	}
	
	str = m_pApp->editComma(atof(str1),true);
	str += str2;

}

// fmEditEx.cpp : implementation file
//

#include "stdafx.h"
#include "fmEditEx.h"
#include "axform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmEditEx

IMPLEMENT_DYNCREATE(CfmEditEx, CfmEdit)

CfmEditEx::CfmEditEx()
{
	EnableAutomation();
}

CfmEditEx::CfmEditEx(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmEdit(axform, formR, iform, false)
{
}

CfmEditEx::~CfmEditEx()
{
}


void CfmEditEx::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmEditEx, CfmEdit)
	//{{AFX_MSG_MAP(CfmEditEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmEditEx, CfmEdit)
	//{{AFX_DISPATCH_MAP(CfmEditEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmEditEx to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {CA3A7032-2513-44B9-85BB-E1006A5FC4BE}
static const IID IID_IfmEditEx =
{ 0xca3a7032, 0x2513, 0x44b9, { 0x85, 0xbb, 0xe1, 0x0, 0x6a, 0x5f, 0xc4, 0xbe } };

BEGIN_INTERFACE_MAP(CfmEditEx, CfmEdit)
	INTERFACE_PART(CfmEditEx, IID_IfmEditEx, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmEditEx message handlers

void CfmEditEx::UpdateData(int key, bool moving, int& result)
{
	result = RC_NOP;
	if (m_form->attr & FA_PROTECT)
		return;

	if (m_state == esReverse)
	{
		m_strR.Empty();
		m_caretPos = 0;
		m_state = esNormal;
		invalidateRect(&m_tRc, false);
	}

	getWhichHangul();
	if (key < 0x80)
		m_method = editUpdMixSingle();
	else
		m_method = editUpdMixDouble();

	processEdit(key, moving, result);
}

void CfmEditEx::InsertData(int key, bool moving, int& result)
{
	UpdateData(key, moving, result);
	return;
}

void CfmEditEx::keyDelete(int& result)
{
	result = RC_NOP;
	if (m_state == esReverse)
	{
		m_strR.Empty();
		m_caretPos = 0;
		m_state = esNormal;

		m_changed = true;
		m_focusing = false;
		invalidateRect(&m_iRc, false);
	}

	getWhichHangul();
	m_method = editDelMixChar();

	processEdit(' ', false, result);
}

void CfmEditEx::keyBackspace(int& result)
{
	result = RC_NOP;
	if (m_state == esReverse)
	{
		m_strR.Empty();
		m_caretPos = 0;
		m_state = esNormal;
	}
	else
	{
		if (m_caretPos == 0)
			result = RC_NODELETE;
		else if (keyArrowLeft(result) == 0)
		{
			getWhichHangul();
			switch (m_chartype[0])
			{
			case WC_SO:
				break;
			case WC_SI:
				if (keyArrowLeft(result) != 0)
					return;
				[[fallthrough]];
			default:
				break;
			}
			keyDelete(result);
			return;
		}
		return;
	}

	m_changed = true;
	m_focusing = false;
	invalidateRect(&m_iRc, false);
}

void CfmEditEx::getWhichHangul()
{
	int	ii, jj;
	bool	hangul = false;

	for (ii = 0; ii < 4; ii++)
		m_chartype[ii] = WC_NC;

	for (ii = m_caretPos, jj = 0; ii < m_form->size && jj < 4; ii++)
	{
		if (ii >= m_strR.GetLength())
		{
			m_chartype[jj++] = WC_AN;
			continue;
		}
		switch (m_strR[ii])
		{
		case SO:
			hangul = false;
			m_chartype[jj++] = WC_SO;
			break;
		case SI:
			hangul = false;
			m_chartype[jj++] = WC_SI;
			break;
		default:
			if (hangul)
			{
				hangul = false;
				m_chartype[jj++] = WC_H2;
				break;
			}
			if (m_strR[ii] & 0x80)
			{
				hangul = true;
				m_chartype[jj++] = WC_H1;
				break;
			}
			m_chartype[jj++] = WC_AN;
		}
	}
}

int CfmEditEx::editInsMixSingle()
{
	switch (m_chartype[0])
	{
	case WC_AN:	return (S_SftR1UpdKey);
	case WC_SO:
		switch (m_chartype[1])
		{
		case WC_SI:	return (S_UpdKeySp);
		case WC_H1:	return (S_SftR1UpdKey);
		default:	return (S_Deny);
		}
	case WC_SI:
		switch (m_chartype[1])
		{
		case WC_AN:	return (S_SkipSftR1UpdKey);
		case WC_SO:	return (S_SkipSftR1UpdKey);
		default:	return (S_Deny);
		}
	case WC_H1:
		switch (m_chartype[2])
		{
		case WC_SI:
			switch (m_chartype[3])
			{
			case WC_AN:	return (S_SftR3UpdSiKeySo);
			case WC_SO:	return (S_SftR3UpdSiKeySo);
			default:	return (S_Deny);
			}
		case WC_H1:	return (S_SftR3UpdSiKeySo);
		default:	return (S_Deny);
		}
	default:	return (S_Deny);
	}
}

int CfmEditEx::editInsMixDouble()
{
	switch (m_chartype[0])
	{
	case WC_AN:
		switch (m_chartype[1])
		{
		case WC_AN:
			switch (m_chartype[2])
			{
			case WC_AN:
				switch (m_chartype[3])
				{
				case WC_H1:	return (S_SftR4UpdSoKeySi);
				case WC_SO:	return (S_SftR4UpdSoKeySi);
				default:	return (S_Deny);
				}
			case WC_SO:	return (S_SftR4UpdSoKeySi);
			default:	return (S_Deny);
			}
		case WC_SO:
			switch (m_chartype[2])
			{
			case WC_SI:	
			case WC_H1:
				switch (m_chartype[3])
				{
				case WC_NC:	return (S_Deny);
				default:	return (S_SftR4UpdSoKeySi);
				}
			default:	return (S_Deny);
			}
		default:	return (S_Deny);
		}
	case WC_SO:
		switch (m_chartype[1])
		{
		case WC_SI:
			switch (m_chartype[2])
			{
			case WC_AN:
			case WC_SO:
				switch (m_chartype[3])
				{
				case WC_NC:	return (S_Deny);
				default:	return (S_SkipSftR2UpdKey);
				}
			default:	return (S_Deny);
			}
		case WC_H1:	return (S_SkipSftR2UpdKey);
		default:	return (S_Deny);
		}
	case WC_SI:
		switch (m_chartype[1])
		{
		case WC_AN:
			switch (m_chartype[2])
			{
			case WC_AN:	return (S_SftR2UpdKey);
			case WC_SO:	return (S_SftR2UpdKey);
			default:	return (S_Deny);
			}
		case WC_SO:	return (S_UpdKey);
		default:	return (S_Deny);
		}
	case WC_H1:	return (S_SftR2UpdKey);
	default:	return (S_Deny);
	}
}

int CfmEditEx::editUpdMixSingle()
{
	switch (m_chartype[0])
	{
	case WC_AN:	return (S_UpdKey);
	case WC_SO:
		switch (m_chartype[1])
		{
		case WC_SI:	return (S_UpdKeySp);
		case WC_H1:	return (S_UpdKeySpSo);
		default:	return (S_UpdKey);
		}
	case WC_SI:
		switch (m_chartype[1])
		{
		case WC_AN:	return (S_UpdSiKey);
		case WC_SO:
			switch (m_chartype[2])
				{
			case WC_SI:	return (S_UpdSiKeySp);
			case WC_H1:	return (S_UpdSiKeySpSo);
			default:	return (S_UpdSiKey);
			}
		default:
			return (S_Deny);
		}
	case WC_H1:
		switch (m_chartype[2])
		{
		case WC_SI:	return (S_UpdSiKeySp);
		case WC_H1:	return (S_UpdSiKeySpSo);
		default:	return (S_UpdSiKey);
		}
	default:
		return (S_Deny);
	}
}

int CfmEditEx::editUpdMixDouble()
{
	switch (m_chartype[0])
	{
	case WC_AN:
		switch (m_chartype[1])
		{
		case WC_AN:
			switch (m_chartype[2])
			{
			case WC_AN:
				switch (m_chartype[3])
				{
				case WC_AN:	return (S_UpdSoKeySi);
				case WC_SO:	return (S_UpdSoKeySftL1);
				default:	return (S_Deny);
				}
			case WC_SO:
				switch (m_chartype[3])
				{
				case WC_SI:	return (S_UpdSoKey);
				case WC_H1:	return (S_UpdSoKey);
				default:	return (S_Deny);
				}
			default:
				return (S_Deny);
			}
		case WC_SO:
			switch (m_chartype[2])
			{
			case WC_SI:
				switch (m_chartype[3])
				{
				case WC_AN:	return (S_UpdSoKeySi);
				case WC_SO:	return (S_UpdSoKeySftL1);
				default:	return (S_Deny);
				}
			case WC_H1:	return (S_UpdSoKeySftL1);
			default:	return (S_Deny);
			}
		default:
			return (S_Deny);
		}
	case WC_SO:
		switch (m_chartype[1])
		{
		case WC_SI:
			switch (m_chartype[2])
			{
			case WC_AN:
				switch (m_chartype[3])
				{
				case WC_AN:	return (S_UpdSoKeySi);
				case WC_SO:	return (S_UpdSoKeySftL1);
				default:	return (S_Deny);
				}
			case WC_SO:	return (S_UpdSoKey);
			default:	return (S_Deny);
			}
		case WC_H1:	return (S_UpdSoKey);
		default:	return (S_Deny);
		}
	case WC_SI:
		switch (m_chartype[1])
		{
		case WC_AN:
			switch (m_chartype[2])
			{
			case WC_AN:	return (S_UpdKeySi);
			case WC_SO:	return (S_UpdKeySftL1);
			default:	return (S_Deny);
			}
		case WC_SO:	return (S_UpdKey);
		default:	return (S_Deny);
		}
	case WC_H1:	return (S_UpdKey);
	default:	return (S_Deny);
	}
}

int CfmEditEx::editDelMixChar()
{
	switch (m_chartype[0])
	{
	case WC_AN:	return (S_SftL1);
	case WC_SO:
		switch (m_chartype[1])
		{
		case WC_SI:	return (S_SftL1UpdSp);
		case WC_H1:	return (S_SkipSftL2);
		default:	return (S_Deny);
		}
	case WC_SI:
		switch (m_chartype[1])
		{
		case WC_AN:	return (S_SkipSftL1);
		case WC_SO:	return (S_SftL1);
		default:	return (S_Deny);
		}
	case WC_H1:		return (S_SftL2);
	default:	return (S_Deny);
	}
}

void CfmEditEx::processEdit(int key, bool moving, int& result)
{
	bool	edit = false;
	eState	pState;

	pState = m_state;
	m_updPos = m_caretPos;

	switch (m_method)
	{
	case S_Deny :
		result = RC_DENY;
		return;
	case S_UpdKey :
		CfmEdit::UpdateData(key, true, result);
		break;
	case S_UpdKeySi :
		CfmEdit::UpdateData(key, true, result);
		CfmEdit::UpdateData(SI,  true, result);
		break;
	case S_UpdKeySp :
		CfmEdit::UpdateData(key, true, result);
		CfmEdit::UpdateData(' ', true, result);
		break;
	case S_UpdKeySpSo :
		CfmEdit::UpdateData(key, true, result);
		CfmEdit::UpdateData(' ', true, result);
		CfmEdit::UpdateData(SO,  true, result);
		break;
	case S_UpdKeySftL1 :
		CfmEdit::UpdateData(key, true, result);
		processShiftLeft(1);
		break;
	case S_UpdSiKey :
		CfmEdit::UpdateData(SI,  true, result);
		CfmEdit::UpdateData(key, true, result);
		break;
	case S_UpdSiKeySp :
		CfmEdit::UpdateData(SI,  true, result);
		CfmEdit::UpdateData(key, true, result);
		CfmEdit::UpdateData(' ', true, result);
		break;
	case S_UpdSiKeySpSo :
		CfmEdit::UpdateData(SI,  true, result);
		CfmEdit::UpdateData(key, true, result);
		CfmEdit::UpdateData(' ', true, result);
		CfmEdit::UpdateData(SO,  true, result);
		break;
	case S_UpdSoKey :
		CfmEdit::UpdateData(SO,  true, result);
		CfmEdit::UpdateData(key, true, result);
		break;
	case S_UpdSoKeySi :
		CfmEdit::UpdateData(SO,  true, result);
		CfmEdit::UpdateData(key, true, result);
		CfmEdit::UpdateData(SI,  true, result);
		break;
	case S_UpdSoKeySftL1 :
		CfmEdit::UpdateData(SO,  true, result);
		CfmEdit::UpdateData(key, true, result);
		processShiftLeft (1);
		break;
	case S_SftR1UpdKey :
		processShiftRight(1);
		CfmEdit::UpdateData(key, true, result);
		break;
	case S_SftR2UpdKey :
		processShiftRight(2);
		CfmEdit::UpdateData(key, true, result);
		break;
	case S_SftR3UpdSiKeySo :
		processShiftRight(3);
		CfmEdit::UpdateData(SI,  true, result);
		CfmEdit::UpdateData(key, true, result);
		CfmEdit::UpdateData(SO,  true, result);
		break;
	case S_SftR4UpdSoKeySi :
		processShiftRight(4);
		CfmEdit::UpdateData(SO,  true, result);
		CfmEdit::UpdateData(key, true, result);
		CfmEdit::UpdateData(SI,  true, result);
		break;
	case S_SkipSftR1UpdKey :
		m_caretPos++;
		processShiftRight(1);
		CfmEdit::UpdateData(key, true, result);
		break;
	case S_SkipSftR2UpdKey :
		m_caretPos++;
		processShiftRight(2);
		CfmEdit::UpdateData(key, true, result);
		break;
	case S_SftL1 :
		edit = true;
		processShiftLeft(1);
		break;
	case S_SftL2 :
		edit = true;
		processShiftLeft(2);
		break;
	case S_SftL1UpdSp :
		processShiftLeft(1);
		CfmEdit::UpdateData(' ', true, result);
		break;
	case S_SkipSftL1 :
		edit = true;
		m_caretPos++;
		processShiftLeft(1);
		break;
	case S_SkipSftL2 :
		edit = true;
		m_caretPos++;
		processShiftLeft(2);
		break;
	default :
		break;
	}

	if (!moving && (pState != esReverse))
		m_caretPos = m_updPos;

	if (edit)
	{
		m_changed = true;
		m_focusing = false;
		invalidateRect(&m_tRc, false);
	}
}

void CfmEditEx::processShiftLeft(int count)
{
	CString	tmpS = _T("");

	tmpS += m_strR.Left(m_caretPos);
	if (m_strR.GetLength() > m_caretPos + count)
		tmpS += m_strR.Mid(m_caretPos + count);

	m_strR = tmpS;
}

void CfmEditEx::processShiftRight(int count)
{
	CString	tmpS = _T("");

	tmpS += m_strR.Left(m_caretPos);
	for (int ii = 0; ii < count; ii++)
		tmpS += CString(" ");
	if (m_strR.GetLength() > m_caretPos)
		tmpS += m_strR.Mid(m_caretPos);

	m_strR = tmpS;
}

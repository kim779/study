#include "stdafx.h"
#include "DispView.h"

/////////////////////////////////////////////////////////////////////////////
// CDispView
OLECHAR *CDispView::m_osInterfaceName[] = {
	// Variable
	
	// Method
	L"ShowDialog",
	L"MsgBoxID",
};


CDispView::CDispView()
{
	m_pdispatch = NULL;
}

void CDispView::InitInterfaceName(IDispatch *pDisp)
{
	int i, nCount = sizeof( m_osInterfaceName) / sizeof( OLECHAR *);
	m_pdID = new DISPID[ nCount];
	for( i = 0; i < nCount; i++)
		pDisp->GetIDsOfNames( IID_NULL, &m_osInterfaceName[ i], 1, LOCALE_USER_DEFAULT, m_pdID + i);
}


/////////////////////////////////////////////////////////////////////////////
// CDispView

short CDispView::ShowDialog(LPCTSTR szKey, short nIndex)
{
	short result;
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_ShowDialog], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		szKey, nIndex);
	return result;
}

short CDispView::MsgBoxID(short nID, LPCTSTR szMessage, LPCTSTR szCaption, short nFlags)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_MsgBoxID], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		nID ,szMessage ,szCaption ,nFlags);
	return result;
}

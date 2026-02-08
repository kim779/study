#include "stdafx.h"
#include "DispFrame.h"

/////////////////////////////////////////////////////////////////////////////
// CDispFrame
OLECHAR *CDispFrame::m_osInterfaceName[] = {
	// Variable
	
	// Method
	L"GetDataSaveType",
	L"GetFrameInfoFile",
	L"GetFontSize",
	L"GetFrameScrNum",
	L"ChangeFrameSize", 
};


CDispFrame::CDispFrame()
{
	m_pdispatch = NULL;
}

void CDispFrame::InitInterfaceName(IDispatch *pDisp)
{
	int i, nCount = sizeof( m_osInterfaceName) / sizeof( OLECHAR *);
	m_pdID = new DISPID[ nCount];
	for( i = 0; i < nCount; i++)
		pDisp->GetIDsOfNames( IID_NULL, &m_osInterfaceName[ i], 1, LOCALE_USER_DEFAULT, m_pdID + i);
}


/////////////////////////////////////////////////////////////////////////////
// CDispFrame

short CDispFrame::GetDataSaveType()
{
	short result;
	InvokeHelper(m_pdID[IIx_GetDataSaveType], DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

CString CDispFrame::GetFrameInfoFile()
{
	CString result;
	InvokeHelper(m_pdID[IIx_GetFrameInfoFile], DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}


short CDispFrame::GetFontSize()
{
	short result;
	InvokeHelper(m_pdID[IIx_GetFontSize], DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

CString CDispFrame::GetFrameScrNum()
{
	CString result;
	InvokeHelper(m_pdID[IIx_GetFrameScrNum], DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CDispFrame::ChangeFrameSize(short nFlag, short nParam1, short nParam2)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(m_pdID[IIx_ChangeFrameSize], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nFlag ,nParam1 ,nParam2);
}

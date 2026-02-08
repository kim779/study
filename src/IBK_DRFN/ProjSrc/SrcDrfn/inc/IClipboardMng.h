#if !defined(AFX_HANARO_ICLIPBOARDMANAGER__HEADER_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_ICLIPBOARDMANAGER__HEADER_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "../../_Include/IClipboardMng.h"
#include "IBaseDefine.h"

//  int nID = 29;
// 	IClipboardManager* pMng = (IClipboardManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IClipboardManager* pMng = (IClipboardManager*)AfxGetPctrInterface(nID);

// 만든이  : 이준옥 2006.01.25(수)
// History : 2006.01.25 인터페이스
//

#define		NCB_BLANKDATA		"!!!!!"
// "!!!!!" 이 값은 플랫폼에서 사용하고 있으므로 클립포드 포맷으로 사용하지 마세요^^


DECLARE_INTERFACE(IClipboardManager)
{
	STDMETHOD_(BOOL ,IsDeskClipboardFormatAvailable) (UINT uFormat) PURE;
	STDMETHOD_(BOOL ,GetData)(UINT uFormat, LPVOID lpszBuffer ,int nBufSize ,HWND &hWnd) PURE;
	STDMETHOD_(BOOL ,GetDataLength)(UINT uFormat, unsigned long *pnSize, HWND &hWnd) PURE;	
	STDMETHOD_(BOOL ,SetData)(UINT uFormat, LPVOID lpszBuffer, unsigned long nSize, HWND &hWnd) PURE;
	STDMETHOD_(BOOL ,Empty)(UINT uFormat, HWND &hWnd) PURE;
};

#endif //AFX_HANARO_ICLIPBOARDMANAGER__HEADER_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

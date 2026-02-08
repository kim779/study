#if !defined(AFX_HEADER_FOR_RMSG_USERINFO_FHDD0D2E__INCLUDED_)
#define AFX_HEADER_FOR_RMSG_USERINFO_FHDD0D2E__INCLUDED_

// #include "../inc/RMsgUserInfo.h"

const UINT RMSG_USERINFO = ::RegisterWindowMessage(_T("RMSG_USERINFO"));

// #define POINTTOPOINTS(pt)      (MAKELONG((short)((pt).x), (short)((pt).y)))
// #define MAKEWPARAM(l, h)      ((WPARAM)(DWORD)MAKELONG(l, h))
// #define MAKELPARAM(l, h)      ((LPARAM)(DWORD)MAKELONG(l, h))
// #define MAKELRESULT(l, h)     ((LRESULT)(DWORD)MAKELONG(l, h))

#define	USERINFO_GET	0x01
#define	USERINFO_SET	0x02


#endif // AFX_HEADER_FOR_RMSG_USERINFO_FHDD0D2E__INCLUDED_

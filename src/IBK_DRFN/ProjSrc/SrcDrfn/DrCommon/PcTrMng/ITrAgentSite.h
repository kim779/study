#if !defined(AFX_INTERFACE_OF_TRAGENT_INCLUDED)
#define AFX_INTERFACE_OF_TRAGENT_INCLUDED

#include "AutoUpdateIO.h"
// 개념 : 
// CTRAgentDll 에서 데이타를 받을때 CTAupBaseDlg를 상속받는 화면 DLL의 함수를 호출하기 위해 정의
// CTAupBaseDlg 의 함수는 virtual 로 선언되어 자식 Class에서 직접 코딩이 가능하다.
//

DECLARE_INTERFACE(ITrAgentSite)
{
	STDMETHOD(ReceiveQueryData) (WORD wQAIndex, LPVOID lpHederInfo, LPVOID lpTRData, long dwTRDateLen) PURE;
	STDMETHOD(ReceiveRealData) (WORD wQAIndex, CAutoToTR * lpAutoToTr) PURE;
};
typedef  ITrAgentSite FAR * LPITrAgentSite;

#endif // AFX_INTERFACE_OF_TRAGENT_INCLUDED
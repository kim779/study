#if !defined(AFX_INTERFACE_OF_DATAMONITORMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_DATAMONITORMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/IAUDataMonitorMng.h"
#include "IBaseDefine.h"

#define		UUID_IAUDataMonitorManager		28

//**************************************************************************************
//데이터변경처리를 위한 인터페이스 사용설명.
//
//#include "../../_Include/IAUDataMonitorMng.h"
//
//가정 1. 공통키값 : "주식_주문설정"
//     2. 
//
//CString szKey = "주식_주문설정";
//CString szSubKey = "A변경";
//long    dwSubKey = (long)(LPCSTR)szSubKey;
//long    dwNotifyData = xx; // 여기에 변경된 Sturct등을 전달할 수 있다.
//
//@@@@@ 데이터 변경처리를 하는 Server에서 처리해줘야할 부분.
//int nID = 28;
//IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);
//pMng->NofityData(szKey, dwSubKey, dwNotifyData);
//
//
//@@@@@ 데이터 변경처리를 해야하는 Client에서 처리해줘야 할 부분.
//Header에서 
//	Class에 멤버변수정의
//	static void CALLBACK DataMonitorCallbackProc(LPCSTR szKey, long dwKey, long dwSubKey, long dwNotifyData);
//
//	long  m_dwDataMonitor;
//
//Cpp에서
//	// OnInitialUpdate등에서
//	int nID = 28;
//	IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);
//	m_dwDataMonitor = pMng->Advise(szKey, dwKey, DataMonitorCallbackProc);
//
//	// Destroy전에 
//	pMng->UnAdvise(m_dwDataMonitor);
//
//	void CMyClass::DataMonitorCallbackProc(LPCSTR szKey, long dwKey, long dwSubKey, long dwNotifyData)
//	{
//		if(szKey=="주식_주문설정")
//		{
//			// 여기서 필요한 처리를 한다.
//		}
//	}
//**************************************************************************************


//  int nID = 28;
// 	IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);

// 개념 : 백그라운드로 실행이 필요한 모듈을 지원하는 매니저
//
// 만든이  : 이준옥 2005.12.03(토)
//           성공의 3단계법칙
//           1단계 : 확실히 믿고
//           2단계 : 이미 이루어진 것을 바라보고
//           3단계 : 말로 선포하라
// History : 2005.12.03 인터페이스 정의
//


#define DataMon_SZGWANSIM		"관심종목"


typedef void (CALLBACK* DataMonitorCallbackProc)(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData);
// static void CALLBACK DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData);

DECLARE_INTERFACE(IAUDataMonitorManager)
{
	// I-PARAM : szKey 데이터키값.
	//           dwKey 콜백받을 때 되돌려 받는 값.. 내부적으로 사용할 수 있도록 제공하는 값임.
	//                 필요없으면 NULL
	//           pCallFunc 변경시 이 함수가 불린다.
	// O-PARAM : Advise키값. 이 키값를 기억했다가 UnAdive등에서 사용.
	STDMETHOD_(long, Advise)(LPCSTR szKey, long pThisPoint, long dwKey, DataMonitorCallbackProc pCallFunc) PURE;

	// I-PARAM : Advise에서 리턴받은 키값.
	// O-PARAM : 
	STDMETHOD_(void, UnAdvise)(long dwRet) PURE;

	// I-PARAM : szKey 데이터키값.
	//           szSubKey 데이터키값외에 서브키값. 필요한 사람은 정의해서 사용..
	//           dwData 데이터에 해당하는 인터페이스
	// O-PARAM : 
	STDMETHOD_(void, NotifyData)(LPCSTR szKey, long dwSubKey, long dwNotifyData) PURE;
};

#endif // AFX_INTERFACE_OF_DATAMONITORMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_

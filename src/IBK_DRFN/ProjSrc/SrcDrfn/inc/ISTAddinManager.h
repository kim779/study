#if !defined(AFX_INTERFACE_OF_ST_ADDINMANAGER_BY_JUNOKLEE_060721A__INCLUDED_)
#define AFX_INTERFACE_OF_ST_ADDINMANAGER_BY_JUNOKLEE_060721A__INCLUDED_

// #include "../../inc/ISTAddinManager.h" // 시스템트레이딩 관련된 컨텐츠 인터페이스

#define		UUSTID_ISTAddinData		0

// 시스템트레이딩 addin에서 데이터를 가져오기 위한 인터페이스 제공
interface ISTAddinData
{
	// 데이터갯수 리턴.
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	STDMETHOD_(int, GetChartData)(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwDataClose,LONG& dwDataVolume, LONG& dwDataExt) PURE;
};


#endif // AFX_INTERFACE_OF_ST_ADDINMANAGER_BY_JUNOKLEE_060721A__INCLUDED_

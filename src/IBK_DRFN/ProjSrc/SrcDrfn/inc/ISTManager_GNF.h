#if !defined(AFX_INTERFACE_OF_STCONTENTS_BY_JUNOKLEE_060721A__INCLUDED_)
#define AFX_INTERFACE_OF_STCONTENTS_BY_JUNOKLEE_060721A__INCLUDED_

// #include "../../inc/ISTManager.h" // 시스템트레이딩 관련된 컨텐츠 인터페이스

#define		UUSTID_ISTAnalysisInput		0

//{{ 20061012_ST관련추가
#include <afxtempl.h>

interface ISTInterface
{
	STDMETHOD_(void*, GetInterface)(int nID) PURE;
	STDMETHOD_(LONG, GetOutBarColor) (COLORREF& clrOLBkgrnd, COLORREF& clrOLOutLine) PURE;
};

typedef struct {
	CString szSTRData;		// ST디렉토리 다음의 상위 폴더이름
	CString szSTName;		// 전략명
	CString szSTPath;		// 해당전략의 디렉토리정보
							// 전체파일경로 = szSTPath + "\\" + szSTName + ".st"
	int		nSTType;		// 전략의 유형, 제공전략(0), 사용자 전략(1)
							// src\Common_ST\LoadSTDir.h 에 정의되어 있음.
	int		nFolderIndx;	// 그룹인덱스, OutlookBar-CListCtrl 스타일일때.
	int		nItemIndx;		// 아이템인덱스, OutlookBar-CListCtrl 스타일일때.
} STMNG_ITEMINFO;

typedef CList<STMNG_ITEMINFO*, STMNG_ITEMINFO*> LIST_STMNG_ITEMINFO;

typedef struct : public STMNG_ITEMINFO{
	int		  nSTLevel;		// 0:초급, 1:고급
							// src\Common_ST\LoadSTDir.h 에 정의되어 있음.
	HTREEITEM hTreeItem;	// OutlookBar-CTreeCtrl 스타일일때.
	HTREEITEM hParentItem;	// 상위 트리의 핸들, 현재가 Item형일때만 세팅됨.
	BOOL	  bIsItem;		// TRUE(Item), FALSE(Folder형)
} STMNG_ITEMINFO_EX1;

// LIST_STMNG_ITEMINFO* a;
// DELETE_PLIST_STMNG_ITEMINFO(a);
#define	DELETE_PLIST_STMNG_ITEMINFO(xxpList) { for(POSITION xpos=xxpList->GetHeadPosition(), xpos; ) delete xxpList->GetNext(xpos); \
				xxpList->RemoveAll(); }

// LIST_STMNG_ITEMINFO a;
// DELETE_LIST_STMNG_ITEMINFO(a);
#define	DELETE_LIST_STMNG_ITEMINFO(xxList) { for(POSITION xpos=xxList.GetHeadPosition(), xpos; ) delete xxList.GetNext(xpos); \
	xxList.RemoveAll(); }


interface ISTUserData
{
	// 데이터갯수 리턴.
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	STDMETHOD_(int, GetChartData)(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwCLose,LONG& dwVolume, int& nBong) PURE;
};
//}} 20061012_ST관련추가

interface ISTAnalysisInput
{
//{{ 20061012_ST관련추가
	//BOOL	m_bUseUserData;					// 구현하는 클래스에서 내부적으로 가짐
	//ISTUserData* m_pInterfaceUserData;	// 구현하는 클래스에서 내부적으로 가짐
	STDMETHOD_(void, SetUserData)(BOOL bUse, ISTUserData* pInterfaceUserData) PURE;	// TRUE 일경우에는 ISTUserData* 값을 세팅해줘야 한다.
	STDMETHOD_(BOOL, IsUseUserData)() PURE;					// 사용자가공 데이터 사용여부
	STDMETHOD_(ISTUserData*, GetUserDataInterface)() PURE;	// 시간최적화일 때는 ISTAnalysisInput*에서 제공하는 GetChartData 대신에

	// ID별 데이터
	STDMETHOD_(COleVariant, GetData)(int nID)  PURE;

	// 0: 종목코드 , GetCode(0)
	STDMETHOD_(CString, GetCode)() PURE;

	// 종목코드명, GetCode(1)
	STDMETHOD_(CString, GetCodeName)() PURE;

	// 종목코드 유형 : 0:주식, 1:선물, GetCode(2)
	// 코드유형은 멀티차트의 InputBarDlg.h에 정의된 값 리턴 : enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, FOREIGN_CHART};
	STDMETHOD_(int, GetCodeType)() PURE;

	// 전략명, GetCode(3)
	//STDMETHOD_(CString, GetSTName)() PURE;
	// 전략정보얻기 성공여부, TRUE/FALSE
	STDMETHOD_(BOOL, GetSTInfo)(STMNG_ITEMINFO* pInfo) PURE;

	// 갯수, GetCode(4)
	STDMETHOD_(int, GetCount)() PURE;

	// 기준일, GetCode(5)
	STDMETHOD_(CString, GetEndDay)() PURE;

	// 분할수, GetCode(6)
	STDMETHOD_(int, GetDivideCnt)() PURE;

	// 분할 상태, GetCode(7)
	// 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)
	STDMETHOD_(int, GetDivideState)() PURE;

	// GetCode(8)
	// 분할 상태일 때의 하위 선택 1, 5, 10, 15 버튼으로 표시된 부분.
	// 리턴값 1(0),5(1),10(2),15(3)
	STDMETHOD_(int, GetUnitPos)() PURE;

	// 중첩율, GetCode(9)
	STDMETHOD_(double, GetDeivideRate)() PURE;

	// 데이터갯수 리턴.
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	STDMETHOD_(int, GetChartData)(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwCLose,LONG& dwVolume, LONG& dwDataExt) PURE;

	// 분할여부
	STDMETHOD_(BOOL, IsDivide)() PURE;

	// 컨트롤을 얻어온다.
	// nID  0 : ISysTabExChartItem*를 넘겨받는다. GetChartItem()를 통해서 ChartOcx 포인터로 변환해서 사용할 수 있다.
	//          증전의 경우 CKoscomChart* 형으로 변환해서 사용할 수 있다.
	//		1 : CListCtrl
//{{ 20061012_ST관련추가
	STDMETHOD_(LONG, GetControl)(int nID) PURE;

	// 20081016 박승룡 - 8진법 변환처리
	STDMETHOD_(CString, GetPacketType)() PURE;

	//>> 20081203 박승룡 - 30초 TimeFormat 처리
	STDMETHOD_(BOOL, Is30SecUnit)() PURE;
	//<< 20081203 박승룡 - 30초 TimeFormat 처리
};


#endif // AFX_INTERFACE_OF_STCONTENTS_BY_JUNOKLEE_060721A__INCLUDED_

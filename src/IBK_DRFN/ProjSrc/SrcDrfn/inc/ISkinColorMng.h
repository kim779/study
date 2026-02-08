#if !defined(AFX_INTERFACE_OF_SKINCOLOR_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_SKINCOLOR_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/ISkinColorMng.h"
#include <AFXTEMPL.H>
#include "IBaseDefine.h"

//	int nID = 6;
//	ISkinColorManager* pSkinColorMng = (ISkinColorManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	ISkinColorManager* pMng = (ISkinColorManager*)AfxGetPctrInterface(nID);

// 개념 : 각각의 데이터에 변경하지 안하을 고유 ID를 부여한다.
// IPCTRSite를 구현할 곳은 데이터를 받는 Client부분이며
// IPCTRManager를 구현할 곳은 데이터를 제공하는 Server부분이다.
//
// 만든이  : 이준옥 2005.11.01(화)
// History : 2005.11.01 인터페이스 정의
//

// #include "../../_Include/ISkinColorMng.h"

#define		ISkinColorSite_SkinInfo_File	"Data\\Config.cfg"
#define		ISkinColor_default_File	"Data\\DefaultColorTable.cfg"
#define		ISkinColorSite_Skin_default		"스킨고전|DefaultColorTable.cfg|Main_bg|116,112,159|Treeicon.bmp|1|default"
// AFX_STATIC_DATA const TCHAR ISkinColorSite_SkinInfo_File[] = _T("Data\\Config.cfg");
// AFX_STATIC_DATA const TCHAR ISkinColorSite_Skin_default[] = _T("스킨고전|DefaultColorTable.cfg|Main_bg|116,112,159|Treeicon.bmp|1|default");


typedef struct {
	CString		m_szName;
	CString		m_szColorTable;
	CString		m_szBKImage;
	CString		m_szBKColor;
	CString		m_szTreeIconImage;
	CString		m_szVScreenImageIndex;
	CString		m_szSubColorTableInfo;
} ST_SKININFO;

typedef CArray<ST_SKININFO, ST_SKININFO> ARRAY_ST_SKININFO;
typedef CList<ST_SKININFO*, ST_SKININFO*>  LIST_ST_SKININFO;


DECLARE_INTERFACE(ISkinColorSite)
{
	// 스킨칼라테이블이 변경되었을 경우 불린다.
	// I-PARAM : -
	// O-PARAM : -
	STDMETHOD_(void, ColorTableChanged)() PURE;
};

DECLARE_INTERFACE(ISkinColorManager)
{
	// 칼라키이름으로 색상구하기
	// I-PARAM : 칼라키
	// O-PARAM : 색상값
	STDMETHOD_(COLORREF, GetColor)(LPCSTR szKeyName) PURE;

	// 칼라키에 해당하는 인덱스구하기
	// I-PARAM : 칼라키
	// O-PARAM : 칼라인덱스
	STDMETHOD_(int, GetIndex)(LPCSTR szKeyName) PURE;

	// 칼라인덱스로 색상구하기
	// I-PARAM : 칼라인덱스
	// O-PARAM : 색상값
	STDMETHOD_(COLORREF, GetColor)(int nIndex) PURE;

	// 컬러테이블 메인 Array 포인터, (ARR_COLOR_TABLE*) 값 리턴
	// I-PARAM : -
	// O-PARAM : (ARR_COLOR_TABLE*) 포인터
	STDMETHOD_(long, GetColorTableArray)() PURE; 

	// 칼라테이블의 변경을 알기위해 ISkinColorSite 등록
	// I-PARAM : ISkinColorSite* 값
	// O-PARAM : ISkinColorSite*가 등록된 키값
	STDMETHOD_(long, AdviseSite)(ISkinColorSite* pSite) PURE;

	// ISkinColorSite 해제.
	// I-PARAM : ISkinColorSite*가 등록된 키값
	// O-PARAM : 성공(TRUE), 실패(FALSE)
	STDMETHOD_(BOOL, UnadviseSite)(long dwKey) PURE;

	// 칼라테이블을 Loading하고, Notify 한다.
	// 기존의 칼라테이블정보대신 새로운 칼라테이블정보로 읽어들인다.
	// 주로 이 기능은 메인에서 한다.
	// I-PARAM : 칼라테이블정보파일
	//           칼라테이블 변경시 ISkinColorSite에 Notify할 것인지 여부, Default는 TRUE
	// O-PARAM : 성공(TRUE), 실패(FALSE)
	STDMETHOD_(BOOL, LoadColorTable)(LPCSTR szTableFile, BOOL bNotify=TRUE) PURE;

	// ISkinColorSite 의 ColorTableChanged()를 호출한다.
	// 주로 이 기능은 메인에서 한다.
	// I-PARAM : -
	// O-PARAM : -
	STDMETHOD_(void, NotifyToSite)() PURE;

	// 칼라테이블의 정보를 가지고 있는 Config.cfg 파일정보를 읽는다.
	// 디폴드로는 DATA/Config.cfg 파일이며, 변경하고자 할 때 사용한다.
	// 파일포맷은 아래와 같다.
	// {{ Config.cfg
	// [SKIN]
	// count=6
	// 01=스킨고전|DefaultColorTable.cfg|Main_bg|116,112,159|Treeicon.bmp|1|default
	// 02=스킨테마1|DefaultColorTable_1.cfg|Main_bg1|169,123,125|Treeicon1.bmp|2|skin1
	// 03=스킨테마2|DefaultColorTable_2.cfg|Main_bg2|114,141,100|Treeicon2.bmp|3|skin2
	// 04=스킨테마3|DefaultColorTable_3.cfg|Main_bg3|157,134,113|Treeicon3.bmp|4|skin3
	// 05=스킨테마4|DefaultColorTable_4.cfg|Main_bg4|115,115,115|Treeicon4.bmp|5|skin4
	// 06=스킨테마5|DefaultColorTable_5.cfg|Main_bg5|121,138,140|Treeicon5.bmp|6|skin5
	// }} Config.cfg
	// 
	// I-PARAM : 설정파일명(FullPath)
	// O-PARAM : CFG파일이 존재하면 TRUE, 아니면 FALSE
	STDMETHOD_(BOOL, SetSkinConfig)(LPCSTR szCfgFileName) PURE;

	// 설정파일에 셋팅된 SKIN갯수를 읽는다.
	// I-PARAM : -
	// O-PARAM : [SKIN]섹션의 count값 리턴.
	STDMETHOD_(int, GetSkinCount)() PURE;

	// 설정파일에 셋팅된 ST_SKININFO 정보
	// example.
	//    ST_SKININFO skInfo;
	//    pSkinMng->GetSkinInfo(nIndex, skInfo);
	//
	// I-PARAM : 인덱스
	// O-PARAM : 인덱스에 해당하는 ST_SKININFO정보, 없으면 NULL
	STDMETHOD_(BOOL, GetSkinInfo)(int nIndex, ST_SKININFO& skInfo) PURE;

	// 기본 칼라키이름으로 색상구하기
	// I-PARAM : 칼라키
	// O-PARAM : 색상값
	STDMETHOD_(COLORREF, GetDefaultColor)(LPCSTR szKeyName) PURE;

	// 기본 칼라키에 해당하는 인덱스구하기
	// I-PARAM : 칼라키
	// O-PARAM : 칼라인덱스
	STDMETHOD_(int, GetDefaultIndex)(LPCSTR szKeyName) PURE;

	// 기본 칼라인덱스로 색상구하기
	// I-PARAM : 칼라인덱스
	// O-PARAM : 색상값
	STDMETHOD_(COLORREF, GetDefaultColor)(int nIndex) PURE;

	// 기본 컬러테이블 메인 Array 포인터, (ARR_COLOR_TABLE*) 값 리턴
	// I-PARAM : -
	// O-PARAM : (ARR_COLOR_TABLE*) 포인터
	STDMETHOD_(long, GetDefaultColorTableArray)() PURE; 

	// 기본 칼라테이블의 정보를 가지고 있는 Config.cfg 파일정보를 읽는다.
	STDMETHOD_(BOOL, LoadDefaultColorTable)(LPCSTR szTableFile) PURE;
};

typedef struct _tabUserColorTable
{
	CString szName;
	COLORREF rgb;
} STUSERCOLORTABLE;
typedef CArray<STUSERCOLORTABLE, STUSERCOLORTABLE>	ARR_COLOR_TABLE;


class CST_SKININFO
{
public:
	static void Convert(ST_SKININFO& stSkinInfo, LPCSTR szInoBuf)
	{
		int nCol = 0;
		AfxExtractSubString(stSkinInfo.m_szName, szInoBuf, nCol++, '|');
		AfxExtractSubString(stSkinInfo.m_szColorTable, szInoBuf, nCol++, '|');
		AfxExtractSubString(stSkinInfo.m_szBKImage, szInoBuf, nCol++, '|');
		AfxExtractSubString(stSkinInfo.m_szBKColor, szInoBuf, nCol++, '|');
		AfxExtractSubString(stSkinInfo.m_szTreeIconImage, szInoBuf, nCol++, '|');
		AfxExtractSubString(stSkinInfo.m_szVScreenImageIndex, szInoBuf, nCol++, '|');
		AfxExtractSubString(stSkinInfo.m_szSubColorTableInfo, szInoBuf, nCol++, '|');

		stSkinInfo.m_szBKImage += ".bmp";
	}
};

#endif //AFX_INTERFACE_OF_SKINCOLOR_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_

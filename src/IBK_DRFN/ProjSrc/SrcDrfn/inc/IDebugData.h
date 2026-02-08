#if !defined(AFX_DEBUGDATA_HEADER_BY_JUNOK_LEE_090805_INCLUDED_)
#define AFX_DEBUGDATA_HEADER_BY_JUNOK_LEE_090805_INCLUDED_

#include <afxtempl.h>

#pragma	pack(1)

// #include "../../inc/IDebugData.h"

enum{
	IDM2_TAG_UM=0,		//User Message
	IDM2_TAG_TI,		//TR Input
	IDM2_TAG_TO,		//TR Output
	IDM2_TAG_RA,		//Real Advise
	IDM2_TAG_RU,		//Real UnAdvise
	IDM2_TAG_RD,		//Real Data
	IDM2_TAG_FI,		//Real FID Input
	IDM2_TAG_FO,		//Real FID Output
	IDM2_TAG_FILLER1,	//아직 사용전.
	IDM2_TAG_FILLER2,	//아직 사용전.
	IDM2_TAG_FILLER3,	//아직 사용전.
	IDM2_TAG_FILLER4,	//아직 사용전.
	IDM2_TAG_FILLER5,	//아직 사용전.
	IDM2_TAG_FILLER6,	//아직 사용전.
	IDM2_TAG_SR,		//System Reques
						//데이터 받기 요청등.
	IDM2_TAG_END,		//End TAG, 사용하지는 않음.

	// UM : User Message
	// TI : TR Input
	// TO : TR Output
	// RA : Real Advise
	// RD : Real Data
	// RU : Real Unadvise
	// SR : System Reques
};
#define	MAX_IDM2_TAG	IDM2_TAG_END

//	StdAfx.cpp에 정의
extern char *gIDM2_TAGSTRING[];// = {"UM", "TI", "TO", "RA", "RU", "RD", "FI", "FD",
				//"F1", "F2", "F3", "F4", "F5", "F6", "SR"};

//※주의사항STIDM2Info_RA 구조체에서 +1로 되어있는 의미는 Null문자열로 값을 채우라는 의미임.
//	내부에서 바로사용하기 위해서 Null문자열로 처리함.
typedef struct 
{
	int  nInfoSize;
	char aKey[20+1];
	char aDataName[20+1];
	char aFieldName[30+1];	//NULL포함.
	int  nKeyNameSize;	//aRealData의 실제 길이.
	char aKeyName[1];
} STIDM2Info_RA;

//STIDM2Info_Data 구조체에서 +1로 되어있는 의미는 Null문자열로 값을 채우라는 의미임.
//	내부에서 바로사용하기 위해서 Null문자열로 처리함.
//  데이타(aData)는 NULL이 포함될 수 있으므로 NULL포함에서 제외함.
typedef struct 
{
	BYTE	aDataKey1[ 1];		// I/O 구분 'I' or 'O'
	char	aDataKey2[30+1];	// 패킷구분값. NULL포함.
	BYTE	pData[1];
} STIDM2Info_TranData;

#define SizeOutOneData(xStr)	(sizeof(xStr)-sizeof(char))

#define	IDebugManager_sizeTranKey	(32)	//aDataKey1+aDataKey2

// RA와 RD는 같은구조.
#define STIDM2Info_RU  STIDM2Info_RA

typedef struct
{
	//	char sDataTime[19];	//YYYY-MM-DD hh:mm:ss
	char sYYYY[4];
	char sMM[2];
	char sDD[2];
	char shh[2];
	char smm[2];
	char sss[2];
	char stt[2];
} StDRTimeFormat;
#define	StDRTimeFormat_LEN	sizeof(StDRTimeFormat)

// IDM2_TAG_SR형태일때의 데이터처리
typedef struct
{
	UINT wParam;
}STIDM2Info_SR_Base;

#define	IDM2_KEY_SR0  0x01		// type : HIWORD(wParam)
								// Data Request

typedef struct : public STIDM2Info_SR_Base
{
	BYTE	aDataKey;		// 'A':Advise, 'U' UnAdvise
	BYTE	aDataKey1[ 1];	// I/O 구분 'I' or 'O'
	char	aDataKey2[30+1];// 패킷구분값.NULL포함.
	char	aXHandle[8];	// WM_COPYDATA로 받을 핸들.
	WORD	dwData;			// WM_COPYDATA에서의 pCopyDataStruct->dwData
}STIDM2Info_SR_A;

typedef CList<STIDM2Info_SR_A*, STIDM2Info_SR_A*> List_STIDM2Info_SR_A;

enum  IDebugM_MSG 
{
	IDebugM_UM	= 0,			// 사용자 메세지
		IDebugM_TR,				// 조회 데이터
};

typedef struct
{
	DWORD	dwTotalSize;	// 가변데이터의 길이를 포함한 전체길이.
	BYTE	cSiteFlag;		// Site Flag.
	BYTE	cIO;			// TR의 'I' or 'O'
	//CString	szTranName;
	char	aTrName[30+1];	// NULL포함 문자열.
	DWORD	dwSize;			// Data의 가변길이.
	BYTE	pData[1];			// Data 버퍼.
}STIDM2CompData;

#define STIDM2CompData_PreSize	(sizeof(STIDM2CompData)-sizeof(BYTE))

#pragma	pack()

#endif //AFX_INTERFACE_OF_DEBUGMANAGER_BY_JUNOK_LEE__INCLUDED_

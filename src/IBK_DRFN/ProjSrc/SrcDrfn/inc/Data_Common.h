#if !defined(AFX_DATA__COMMON__BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_DATA__COMMON__BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "Data_Common.h"
// 작성 : 이준옥
// 날자 : 2005.11.23(수)

#define DEFINE_DRDS_LINE0(xComment, xVal, xChar, xSize)	 char  xVal[xSize]; 

// 변수선언을 위한 루틴들
#define DEFINE_DRDS_STRING(xComment, xVal, xChar, xSize)	 m_s##xVal;


#define DC_ReadPackString(xPacket, xString, xSize)	memcpy(m_aPckTemp, xPacket->xString, xSize); \
		m_aPackTemp[xSize]=0;			\
		m_s##xString = m_aPackTemp;		\
		m_s##xString.TrimRight();


#define DC_ReadPackInt000(xPacket, xString, xSize)  memcpy(m_aPckTemp, xPacket->xString, xSize); \
		m_aPackTemp[xSize]=0;			\
		m_n##xString = atoi(m_aPackTemp);

#define DC_ReadPackLong00(xPacket, xString, xSize)  memcpy(m_aPckTemp, xPacket->xString, xSize); \
		m_aPackTemp[xSize]=0;			\
		m_l##xString = atol(m_aPackTemp);

#define DC_ReadPackFloat0(xPacket, xString, xSize)  memcpy(m_aPckTemp, xPacket->xString, xSize); \
		m_aPackTemp[xSize]=0;			\
		m_f##xString = atof(m_aPackTemp);

#define DC_ReadPackTime00(xPacket, xString, xSize)  memcpy(m_aPckTemp, xPacket->xString, xSize); \
		m_aPackTemp[xSize]=0;			\
		m_s##xString = m_aPackTemp;		\
		m_s##xString.Insert(":", 4);	m_s##xString.Insert(":", 2);


//{{ TR데이터 convert에서 사용
// ex "Tr13110.h"
#define DATA_MEMCOPY(xTmp, xStruct, xKey) memcpy(xTmp, xStruct->xKey, sizeof(xStruct->xKey)); \
			xTmp[sizeof(xStruct->xKey)]=0;  m_sz##xKey=xTmp; \
			m_sz##xKey.TrimLeft(); m_sz##xKey.TrimRight();
#define DATA_MEMCOPY_COMMA(xTmp, xStruct, xKey, xString, xManager)	DATA_MEMCOPY(xTmp, xStruct, xKey);	\
			xString=xManager->GetComma(m_sz##xKey);

#define DATA_CONVERT_int_0(xKey)  m_n##xKey = atoi(m_sz##xKey);
#define DATA_CONVERT_long_0(xKey) m_n##xKey = atol(m_sz##xKey);
#define DATA_CONVERT_float_0(xKey) m_d##xKey = atof(m_sz##xKey);
//}} TR데이터 convert에서 사용


//#define		MAX_CConver_DrdsBase		31

// CConver_DrdsBase
class CDrds_Base
{
public:
	//CConver_DrdsBase() { m_aPackTemp[0]=0; }
	
	// Variable
	//char m_aPackTemp[MAX_CConver_DrdsBase];
};

#endif // AFX_DATA__COMMON__BY_JUNOK_LEE_98B1A__INCLUDED_

// #define AfxGetPctrInterface(nID)		AfxGetMainWnd()->SendMessage(, nID, 0)
// #define AfxGetPctrDataString(nID)	AfxGetMainWnd()->SendMessage(, (WPARAM)'A', nID)
// #define AfxGetPctrDataEx(nID)	AfxGetMainWnd()->SendMessage(, (WPARAM)'E', nID)

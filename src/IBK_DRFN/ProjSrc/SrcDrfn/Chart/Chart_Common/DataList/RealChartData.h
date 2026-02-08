// RealChartData.h: interface for the CRealChartData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALCHARTDATA_H__D3BC27B6_06BD_4E2C_891A_B40A555BAE4E__INCLUDED_)
#define AFX_REALCHARTDATA_H__D3BC27B6_06BD_4E2C_891A_B40A555BAE4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
typedef struct {
	CString DataName;
	CString FieldName;
	CString KeyName;
	UINT	nID;
	BOOL	bAdvise;
} STDRDSINFO;
*/

#define WM_DRDS_DATA				WM_USER + 200
#define WM_DRDS_DATA_PACKET_END		WM_USER + 202 //Real Pakcet End
#define MAX_DRDS_PACKET_LEN		2048

//
//typedef struct  
//{
//	char	DataName[32];
//	char	FieldName[32];
//	char	KeyName[32];
//	UINT	nID;
//	int		Len;
//	char	Data[MAX_DRDS_PACKET_LEN];
//} DrdsData;

class CRealChartData  
{
public:
	CRealChartData();
	virtual ~CRealChartData();

	HWND		m_hOCXHwnd		;//요청한 핸들
//	DrdsData	m_DrdsData		;//
};

#endif // !defined(AFX_REALCHARTDATA_H__D3BC27B6_06BD_4E2C_891A_B40A555BAE4E__INCLUDED_)

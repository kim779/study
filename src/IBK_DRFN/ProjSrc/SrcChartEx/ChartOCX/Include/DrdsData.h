//DrdsData.h
#ifndef __DRDSDATA__
#define __DRDSDATA__

// (2004.05.19, 배승원) 한동혁 과장이 새로 배포한 LoadDrds.h에 있는 내용으로
//		향후 LoadDrds.h만을 계속 유지보수하게 되어, 이곳에서 주석처리한다.
//		주의) LoadDrds.h는 한동혁 과장만이 수정/배포한다.
//#define MAX_DRDS_PACKET_LEN		2048

#ifndef FIELD_INFO_LEN
#define FIELD_INFO_LEN			128
#endif

#define APPLICATION_NAME		"DRDS"
#define INI_FILE_NAME			"Drds.ini"

#define ID_TRAY_MESSAGE			WM_USER + 100

#define WM_DRDS_DATA			WM_USER + 200
#define WM_DRDS_TERMINATE		WM_USER + 201
#define WM_DRDS_DATA_PACKET_END WM_USER + 202

#define WM_DRAG_DRDS_DATA		WM_USER + 210
#define WM_DROP_DRDS_DATA		WM_USER + 211

#define WM_NET_RECEIVE			WM_USER + 300
#define WM_NET_CONNECT			WM_USER + 301
#define WM_NET_CLOSE			WM_USER + 302

typedef struct  {
	char	DDEPath[128];
	char	Name[32];
	char	Field[32];
	int		Attr;
	int		RecCnt;
	char	FieldList;
} DrdsDataInfo;

// (2004.03.26, 배승원) 한동혁 과장이 새로 배포한 LoadDrds.h에 있는 내용으로
//		향후 LoadDrds.h만을 계속 유지보수하게 되어, 이곳에서 주석처리한다.
//		주의) LoadDrds.h는 한동혁 과장만이 수정/배포한다.
/*
typedef struct  {
	char	Name[32];
	char	Field[32];
	char	Key[32];
	UINT	Id;
	int		Len;
	char	Data[MAX_DRDS_PACKET_LEN];
} DrdsData;
*/

//////////////////////
typedef struct {
	char OutputString[64];
	char ServerName[3];
	char DataName[3];
	char FieldName[16];
//	int  Attr;
} DrdsDragData;


#ifndef uchar
#define uchar unsigned char
#endif

#ifdef __cplusplus__
extern "C"
{
#endif
void *	WINAPI DrdsInit(HWND hWnd);
BOOL	WINAPI DrdsTerm(void *pDrds, HWND hWnd);
BOOL	WINAPI DrdsConnect(void *pDrds, HWND hWnd, LPSTR DataName);
BOOL	WINAPI DrdsClose(void *pDrds, HWND hWnd, LPSTR DataName);
BOOL	WINAPI DrdsAdvise(void *pDrds, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName);
BOOL	WINAPI DrdsUnadvise(void *pDrds, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName);
BOOL	WINAPI DrdsRequest(void *pDrds, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName);
void	WINAPI DrdsPropertySet();
void	WINAPI DrdsTraceOn();
void	WINAPI DrdsTraceOff();

#ifdef __cplusplus__
}
#endif


#endif
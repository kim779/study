#ifndef __PACKET__HEADER__KLDK_FJJHD9383_KKDDFH
#define __PACKET__HEADER__KLDK_FJJHD9383_KKDDFH

//@유진삭제
//// 통신패킷 구조 
//typedef struct {
//	UCHAR	Stx;				// 0x02
//	UCHAR	Packet_Len[2];		// 패킷 데이타 사이즈
//	UCHAR	Packet_Attri;		// 팻킷 속성 : 압축, 암호화, 연속
//	UCHAR	Packet_Seq;			// AP Packet별 일련번호 
//	UCHAR	PacketData[1017];   // PACKET DATA
//	UCHAR	Etx;				// 0x03
//	UCHAR	Bcc;				// BCC CHECK
//} HtsPacket, *LPHTSPACKET;

// STTRHEADER의 사이즈 108
typedef struct {
	long	WindowID;			// 4 byte
	UCHAR	TRCode[5];
	UCHAR	MsgCode[6];
	UCHAR	Message[82];
	UCHAR	CursorPos[2];
	UCHAR	Message2Len[3];		// 보조 메시지 길이
	UCHAR	APDataLen[3];		// AP에서 사용하는 데이타
	char	TRData[1];			// 3 byte
} STTRHEADER, *LPSTTRHEADER;
#define		SZSTTRHEADER		sizeof(STTRHEADER)
#define		STTRHEADER_TRDATAHEAD		SZSTTRHEADER-sizeof(char)

typedef struct {
	UCHAR	Command;
	STTRHEADER  pTrData;
} STPACKETDATA, *LPSTPACKETDATA;

// SOCKET으로 보내는 WM_COPYDATA 구분자
#define		TYPE300			300
#define		TYPE301			301
#define		TYPE400			400
#define		TYPE401			401
#define		TYPE410			410
#define		TYPE500			500
#define		TYPE900			-900			// -900번대는 에러번호영역
			// 900			Cert 취소 및 비밀번호 에러

//- 맵(화면)에서 데이타를 보낼경우에는 암호화여부가 포함된다.
typedef struct {
	BOOL	bEnc;
	int		nLen;
	STTRHEADER trHeader;
} STTYPE400, *LPSTTYPE400;

typedef struct {
	BOOL	bEnc;
	int		nLen;
	char	pData[2];		// Data
} STTYPE300, *LPSTTYPE300;


typedef struct {
	BOOL	bEnc;
	int		nLen;
	char	pData[20];		// Data
} STTYPE301, *LPSTTYPE301;




#define		SOCKET_CONNECT_FLAG		1	//	(LPARAM :성공시 : 0이 아님 
#define		SOCKET_ERROR_PLAG		2	//	(LPARAM : 에러메세지)
#define		SOCKET_PACKET_RECV		3	//	(LPARAM : 에러메세지)
#define		SOCKET_CLOSE_RECV		4	//	(LPARAM : 에러메세지)


#endif // __PACKET__HEADER__KLDK_FJJHD9383_KKDDFH

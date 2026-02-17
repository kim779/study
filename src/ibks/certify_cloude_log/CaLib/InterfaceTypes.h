#ifndef _INTERFACETYPES_H
#define _INTERFACETYPES_H

#include <windows.h>

#pragma pack(push,8)

/* 전자서명시 사용하는 정보로서 입력되는 정보가 무엇인지에 따라 처리및 컨텍스타입이 달라짐 */
/* API_Version0 : 현재 일반 인증모듈에 사용,  API_Version1 : 상호연동을 위해 표준을 지킬 경우 */
#define CMS_API_Version0	0
#define CMS_API_Version1	1	/* standard CMS output */

/* 암호화시 사용하는 정보로서 입력되는 정보가 무엇인지에 따라 컨텍스타입이 달라짐 */
#define CMS_API_DATA_PLAIN	0
#define CMS_API_DATA_SIGNED	1	/* standard CMS envelop를 위한 input/open시 data type */

typedef char  int1;
typedef short int2;

typedef struct _SIGN_FILE_INFO {
	char *pOriginalFileName ;
	unsigned int nFileLength ;
	unsigned char *pFileInfo ;
	int   nSignContentFlag ;
} SIGN_FILE_INFO ;

typedef struct _CardMapList {
	char **ppValue ;
} CardMapList, CardMapFidList, CardMapNameList ;

/* 2000.2.29 signature 513->1024, DN관련 모두 256 */
typedef struct _ExtCertInfo {
	char szVersion[15] ;
	char szSerial[10] ;		/* "extended" (Serial_EXTENDED)가 있다면 szExtSerial을 참조 */
#define  Serial_EXTENDED		"extended"
	char szSignatureAlgId[30];
	char szIssuer[256] ;
	char szSubject[256] ;
	char szSubjectPublicKeyAlgId[30] ;
	char szFrom[30] ;
	char szTo[30] ;
	char signature[1024] ;	   /* support 8 KBytes */
	char subjectAltName[300] ;
	char keyUsage[300] ;
	char extKeyUsage[300] ;
	char policy[300] ;
	char policyMapping[30] ;
	char basicConstraint[100] ;
	char policyConstraint[100] ;
	char distributionPoints[256] ;
	char authorityKeyId[100] ;
	char subjectKeyId[100] ;
/* 2002/04/03 */
/* char publicKey[1024] ; 프로그램 호환성을 위해 해당 필드를 아래 2개와 같이 분할 사용 */
/* 단 아래의 경우는 인증서가 확장 Serial을 가진 경우에 한하며 (행자부 인증서),
   이 필드의 변경으로 인해 현재의 프로그램에는 어떤 영향도 주지 않음 */
/* 일반 응용프로그램의 경우 publicKey의 크기를 기존 1024로 처리해도 전혀 관계없음 */
	char publicKey[680] ;
	char szExtOCSPserver[300] ;
	char szExtSerial[44] ;
} ExtCertInfo ;


/* The following values can be returned by DecryptData() with CMS Version */
#define CONTENT_IS_DATA				0x00000000
#define CONTENT_IS_SIGNED			0x00000001
#define CONTENT_IS_ENVELOPED		0x00000002
#define CONTENT_IS_ENCRYPTED		0x00000004

typedef struct _UString {
	int	length;
	unsigned char *value;
} UString;

typedef struct _UStringList {
	UString value ;
	struct _UStringList *next ;
} UStringList;

typedef struct _CString128 {
	int	length;
	unsigned char value[128];
} CString128;

typedef struct _CString256 {
	int	length;
	unsigned char value[256];
} CString256;

typedef struct _CString512 {
	int	length;
	unsigned char value[512];
} CString512;

typedef struct _CString1024 {
	int	length;
	unsigned char value[1024];
} CString1024;

typedef struct _CString2048 {
	int	length;
	unsigned char value[2048];
} CString2048;

typedef struct _CString4096 {
	int	length;
	unsigned char value[4096];
} CString4096;

typedef struct _CString5120 {
	int	length;
	unsigned char value[5120];
} CString5120;


/* 인증서의 목적 정의 */
#define	E_SN_CERT				0x00000000
#define	E_SN					0x00000000
#define	E_KM_CERT				0x00000001
#define	E_KM					0x00000001
#define	E_TSA_CERT				0x00000002

#define	E_SN_AND_KM				0x00000008

#define E_BUFLEN						512


#define	R_RANDOM_LENGTH					20

typedef struct _R_INFO {
	int choice ;
#define R_empty								0x00
#define R_randomNumber_chosen_octet			0x40	/* invalid old format */
#define R_randomNumber_chosen				0x80
#define R_randomNumber_empty				0xFF /* fatal worning : for smartcard operation */
// HSM DEVEL [2007]  DEFINE
#define R_randomNumber_HSM			    	0x81

	unsigned int length ;
	unsigned char value[128] ;			/* max 128 bytes random : 변경 불가 */
} R_INFO ;

typedef struct _VID_INFO {
	int bit_mask ;
#define VID_empty								0x00
#define VID_present								0x80
	int hash_algorithm ;
	unsigned int length ;
	unsigned char value[128] ;			/* max 128 bytes VID */
} VID_INFO ;

typedef struct _SESSION_MANAGER {
	void *pSession ;		/* created & established session manager handler */
	char name[520+1] ;		/* name to put/get from current session (maximum 520 bytes) */

	int  err ;				/* each api-call result */
} SESSION_MANAGER ;


/* only used system */
typedef struct _CertSetList CertSetList ;

struct _CertSetList {
	BYTE	bStorage ;
	char	*DN ;
	char	*pszName ;

	char	serial[10] ;
	char	exDate[10+1] ;
	char	policy[20+0] ;

	CertSetList *prior ;
	CertSetList *next  ;

/* 2001/11/16 : 상호연동 관련 */
	int		ca ;
	int		position ;
	int		km_request ;	/* km operation request for read/write */

} ;


/*
	password minimum-length is 8.
	so, 1-7 represent special informations. below...
*/
#define		PASSWD_ENCRYPTED_NOAPPLY	0x01 /* == implicit */
#define		PASSWD_ENCRYPTED_16BYTES	0x02
#define		PASSWD_ENCRYPTED_32BYTES	0x03
#define		PASSWD_BINARY_VALUE_LENG	0x04 /* not used currently */
#define     PASSWD_ENCRYPTED_32DKEY		0x06	
#define		PASSWD_MAXIMUM_FLAG			0x08


/* new~, saved~ variable is set/used during update operation */
typedef struct _SD_API_CONTEXT {

	char szRefNo[32+1] ;			/* cmp reference */
	char szAuthCode[32+1] ;			/* cmp auth code */

	unsigned char	bOldStorage ;	/* current cert storage */
	unsigned char	bNewStorage ;	/* cert storage to save for current operation */

	char szOldPasswd[32+1] ;	/* current private key password */
	char szNewPasswd[32+1] ;	/* signprovatekey password to change */

/* common reference */
	char szCertSetName[E_BUFLEN+1] ;		/* saved relative directory from pseroot (not full path) */
	char szSavedCertSetName[E_BUFLEN+1] ;	/* new changed relative directory (not full path) hsm dll path */
	char szDN[300+1] ;					/* user dn (must be set, or returned on Issue) */

/* IC CARD Interface (only reserved, developer can't use it) */
	char szPinPasswd[32+1] ;			/* pin */

	union _RESERVED_AREA {
		char reserved[32+1] ;				/* api-reserved : do not use this field */
		struct _REDEFINED_AREA {
			unsigned char bUsePasswordOnSelectDLG ;
			unsigned char bForceAllCaSearchModeOnSelectDLG ;
			//HSM set current Status 다시 라이브러리 물어보지 않기 위해 사용하는 경우에만 사용
			unsigned char bCurrnetHsmMediaSetOK ;
			//Trusted HSM 을 검증한 경우에 설정...
			unsigned char bCurrnetHsmTrustedOK ;
			//[8.2.9.0][2010/09/27] #101 지문보안토큰 정보추출(수자원공사)   
			unsigned char bCurrnetBioHsmOK ;
			//[9.2.9.0] [2010/10/11] #102  고도화 인증서 확인 : SHA256 여부 
			unsigned char bSha256Alg;
			//[9.2.9.0][2010/10/13] #104 IC카드 2048 지원여부 비트  
			unsigned char bICCard2048MapSupport;
			//[9.8.0.0]][2011/07/20] #125 EPKI 인증서 연동개발 [GPKI 인증서 검색]  SUPERSK
			unsigned char bGPKISerachOK;
			//[2014/02/18] #170 USIM 서비스 연동
			unsigned char bUSIMSet;
			unsigned char bOnlyUSIMUse;
		} rd ;
	} ra ;
#define	bUseSDPasswordDLG		ra.rd.bUsePasswordOnSelectDLG
#define	bForceAllCaSearchMode	ra.rd.bForceAllCaSearchModeOnSelectDLG
#define	bHsmLibMediaSet			ra.rd.bCurrnetHsmMediaSetOK
#define	bHsmLibTrustedSet		ra.rd.bCurrnetHsmTrustedOK
//[8.2.9.0][2010/09/27] #101 지문보안토큰 정보추출(수자원공사)
#define	bBioHsmSet      		ra.rd.bCurrnetBioHsmOK
//[9.2.9.0] [2010/10/11] #102 인증서가 SHA256 즉 고도화용 인증서인지 확인 
#define	bSha256Cert		        ra.rd.bSha256Alg
//[9.2.9.0][2010/10/13] #104 IC카드 2048 지원여부 비트  
#define	bICCard2048Map		        ra.rd.bICCard2048MapSupport
//[9.8.0.0]][2011/07/20] #125 EPKI 인증서 연동개발 [GPKI 인증서 검색]  SUPERSK
#define	bGPKISerachMode			ra.rd.bGPKISerachOK
	//[2014/02/18] #170 USIM 서비스 연동
#define	bUsimMediaSet         ra.rd.bUSIMSet
#define	bOnlyUSIMUseSet       ra.rd.bOnlyUSIMUse


//HSM 정보로 이용
/* for UserID-based implementation */
	char szUserId[E_BUFLEN+1] ;			/* user id --> cn=real_name(userid).... */

/* must be cleaed or matched value by given dn, certsetname */
	unsigned char EncPrivateKey[2048] ;	/* encrypted privatekey */
	int	 nEncPrivateKeyLen ;			/* length */
	unsigned char DerCertificate[2048] ;/* der certificate */
	int	 nDerCertificateLen ;			/* length */

} SD_API_CONTEXT ;


/* 2001/12/24 상호연동으로 인한 타입추가 : 이전 프로그램과의 호환성 보장 */

typedef struct _SD_EXTENDED_INFO {
/* VERI VERY IMPORTANT */
	int km_request_flag ;		/* km operation requested then, is not zero */
#define KM_NOT_REQUESTED					0
#define KM_REQUESTED_ONLY					1
#define KM_REQUESTED_WITH_SIGN				2

#define KM_REQUESTED_WITH_SIGN_POSSIBLE		3

	int	ca ;
	int position ;

	SD_API_CONTEXT km ;	/* 지정된 암호화용 인증서/서명키 구조체로의 포인터 */
						/* 편이를 위해서 동적할당은 하지 않음 */
} SD_EXTENDED_INFO ;

typedef struct _SD_API_CONTEXT_NEW {

	SD_API_CONTEXT sd ;
	SD_EXTENDED_INFO ext ;

} SD_API_CONTEXT_NEW ;


/* CONTEXT type */
/* 인증서 선택을 위해 구성하는 정보의 종류를 설정
   SD_API_CONTEXT의 bOldStorage가 setting되어 있지 않다면 mode-1 우선적용
   SD_API_CONTEXT의 bOldStorage가 setting되어 있다면      mode-1 우선적용 후 filtering

   - type이 추가가될 경우 SelectCertSet의
     인증서 만료검사 부분을 skip처리하는지에 대한 setting을 해야 함

   - 2002/05/15 : MODE/COMMAND를 분리하여 재정리
*/
typedef unsigned long		CONTEXT_TYPE ;

/* mode0 - dialog-mode */
#define		CONTEXT_MODE_SELTDLG	0x00010000	/* dialog-mode */
#define		CONTEXT_MODE_MUSTPWD	0x00020000	/* dialog+passwd mode */

/* mode1 - media scan */
#define		CONTEXT_MODE_ALLSCAN	0x10000000
#define		CONTEXT_MODE_ORDSCAN	0x20000000	/* scan by media-order */
#define		CONTEXT_MODE_ONLY_SK	0x40000000	/* only signkorea cert search mode */
// HSM DEVEL VER.7.9.0.1[2007/09]  DEFINE
#define		CONTEXT_MODE_NOT_HSM	0x80000000	/* cert search mode except HSM*/
//[VER 8.2.2.1] [2010/02/24] DEFINE NOT CONTEXT_MODE_ORDSCAN BUT FILTERING CERT  
#define		CONTEXT_MODE_NOTBANK	0x01000000	/* cert listing with filtering option*/
#define		CONTEXT_MODE_ONLY_BK	0x02000000	/* cert listing with filtering option*/
#define		CONTEXT_MODE_EPOLICY	0x04000000	/* cert listing setpolicy with filtering option*/ 
//[VER 9.9.7.0 [2016/10/20] #210 유효기간내 인증서만 표시 (주의 PC시간 체크)  
#define		CONTEXT_MODE_NOEXPIRED	0x08000000	/* cert scan by media-order pc filtering option*/ 



//#define		CONTEXT_MODE_NOT_	0x01000000	/* cert listing with filtering option*/


/* filtering */
#define		CONTEXT_COMM_FILTER		0x0000FFFF /* mode clear */
#define		CONTEXT_MODE_FILTER		0xFFFF0000 /* command clear */
#define		CONTEXT_MODE0_FILTER	0x00FF0000 /* command & mode-1 clear */
#define		CONTEXT_MODE1_FILTER	0xFF000000 /* command & mode-0 clear */

/* command : exclusive with each other */
#define		CONTEXT_COMM_NORMAL		0x00000000 /* <High-Priority> */	/* ActiveX <0> */
#define		CONTEXT_COMM_ISSUE		0x00000001							/* ActiveX <1> */
#define		CONTEXT_COMM_KEYNEW		0x00000002							/* ActiveX <2> */
#define		CONTEXT_COMM_CERTNEW	0x00000004							/* ActiveX <3> */
#define		CONTEXT_COMM_SUSPEND	0x00000008							/* ActiveX <4> */
#define		CONTEXT_COMM_REVOKE		0x00000010							/* ActiveX <5> */
#define		CONTEXT_COMM_BACKUP		0x00000020							/* ActiveX <6> */
#define		CONTEXT_COMM_RECOVER	0x00000040							/* ActiveX <7> */
#define		CONTEXT_COMM_CHNGPWD	0x00000080 /* <Low-Priority> */		/* ActiveX <8> */


/* context : CONTEXT MUST BE ONE OF THE BELOWs */
#define		CONTEXT_ISSUE			(CONTEXT_COMM_ISSUE+CONTEXT_MODE_ONLY_SK)
#define		CONTEXT_KEYNEW			(CONTEXT_COMM_KEYNEW+CONTEXT_MODE_SELTDLG+CONTEXT_MODE_ONLY_SK+CONTEXT_MODE_EPOLICY) /*[8.2.3.0] 정책 Filtering 추가 */
#define		CONTEXT_CERTNEW			(CONTEXT_COMM_CERTNEW+CONTEXT_MODE_SELTDLG+CONTEXT_MODE_ONLY_SK+CONTEXT_MODE_EPOLICY) /*[8.2.3.0] 정책 Filtering 추가 */
#define		CONTEXT_SUSPEND			(CONTEXT_COMM_SUSPEND+CONTEXT_MODE_SELTDLG+CONTEXT_MODE_ONLY_SK+CONTEXT_MODE_MUSTPWD)
#define		CONTEXT_REVOKE			(CONTEXT_COMM_REVOKE+CONTEXT_MODE_SELTDLG+CONTEXT_MODE_ONLY_SK+CONTEXT_MODE_MUSTPWD)
#define		CONTEXT_BACKUP			(CONTEXT_COMM_BACKUP+CONTEXT_MODE_SELTDLG+CONTEXT_MODE_ONLY_SK+CONTEXT_MODE_MUSTPWD)
#define		CONTEXT_RECOVER			(CONTEXT_COMM_RECOVER+CONTEXT_MODE_SELTDLG+CONTEXT_MODE_ONLY_SK+CONTEXT_MODE_MUSTPWD)
#define		CONTEXT_CHNGPWD			(CONTEXT_COMM_CHNGPWD+CONTEXT_MODE_SELTDLG+CONTEXT_MODE_ONLY_SK)

#define		CONTEXT_NORMAL			(CONTEXT_COMM_NORMAL+CONTEXT_MODE_ALLSCAN)
#define		CONTEXT_SELECT			(CONTEXT_COMM_NORMAL+CONTEXT_MODE_ORDSCAN)
#define		CONTEXT_LIST			(CONTEXT_COMM_NORMAL+CONTEXT_MODE_ALLSCAN+CONTEXT_MODE_SELTDLG)
#define		CONTEXT_QUERY			(CONTEXT_COMM_NORMAL+CONTEXT_MODE_ALLSCAN)	/* not dialog-mode & all scan */
#define		CONTEXT_SELECT2			(CONTEXT_COMM_NORMAL+CONTEXT_MODE_ORDSCAN+CONTEXT_MODE_MUSTPWD)
#define		CONTEXT_SELECTDLG		(CONTEXT_COMM_NORMAL+CONTEXT_MODE_ORDSCAN+CONTEXT_MODE_SELTDLG)
#define		CONTEXT_LIST2			(CONTEXT_COMM_NORMAL+CONTEXT_MODE_ALLSCAN+CONTEXT_MODE_SELTDLG+CONTEXT_MODE_MUSTPWD)
// HSM DEVEL VER.7.9.0.1[2007/09]  DEFINE
#define		CONTEXT_CHNGMEDIA		CONTEXT_LIST3
#define		CONTEXT_LIST3   		(CONTEXT_COMM_NORMAL+CONTEXT_MODE_ALLSCAN+CONTEXT_MODE_SELTDLG+CONTEXT_MODE_MUSTPWD+CONTEXT_MODE_NOT_HSM)


/* SEARCH type */
/* 인증서 선택을 위해 검색되는 대상을 상세하게 분류하기 위해 사용
   - 현재 내부용도로만 사용
*/
typedef unsigned long		SEARCH_TYPE  ;


/* search-mode0 : media mode */
#define SEARCH_MODE_NONE			0x00000000	// none (no select == issue)
#define SEARCH_MODE_SMARTCARD		0x00010000	//
#define SEARCH_MODE_DISKETTE		0x00020000	//
#define SEARCH_MODE_HARDDISK		0x00040000	//
#define SEARCH_MODE_USBKEY			0x00080000	//
// HSM DEVEL [2007]  DEFINE
#define SEARCH_MODE_P11				0x00080000	//
//[2014/02/18] #170 USIM 서비스 연동 호환성 개발 
/* search-mode0-: mobile mode */
#define SEARCH_MODE_M_USIM 				0x00180000	//  $$$

#define SEARCH_MODE_MEDIA_ALL		(SEARCH_MODE_SMARTCARD+SEARCH_MODE_DISKETTE+SEARCH_MODE_HARDDISK+SEARCH_MODE_USBKEY)
#define SEARCH_MODE_MEDIA_FILTER	0x000F0000	//

/* search-mode1 : ca mode */
#define SEARCH_MODE_CA_SIGNKOREA	0x10000000	// SignKorea 인증서 검색
#define SEARCH_MODE_CA_ALL			0x20000000	// 모든 인증기관 인증서 검색
#define SEARCH_MODE_CA_GPKI			0x40000000  //[9.8.0.0]GPKI 인증서 검색

#define SEARCH_MODE_CA_FILTER		0xF0000000	//
#define SEARCH_MODE_CA_MASK			0x0FFFFFFF	//

/* search command */
#define SEARCH_COMM_MATCH_ONE		0x00000001
#define SEARCH_COMM_MATCH_ALL		0x00000002
#define SEARCH_COMM_MATCH_FILTER	0x0000000F
#define SEARCH_COMM_MATCH_MASK		0xFFFFFFF0


/* filtering & masking */
#define SEARCH_MODE_MASK			0x0000FFFF
#define SEARCH_COMM_MASK			0xFFFF0000
#define SEARCH_MODE_FILTER			SEARCH_COMM_MASK
#define SEARCH_COMM_FILTER			SEARCH_MODE_MASK


/* search context */
#define SEARCH_SELECT			(SEARCH_COMM_MATCH_ONE+SEARCH_MODE_CA_SIGNKOREA+SEARCH_MODE_MEDIA_ALL)	// match one
#define SEARCH_ALLMEDIA			(SEARCH_COMM_MATCH_ALL+SEARCH_MODE_CA_SIGNKOREA+SEARCH_MODE_MEDIA_ALL)  // all scan
//#define SEARCH_BACKUP			(SEARCH_COMM_MATCH_ALL+SEARCH_MODE_CA_SIGNKOREA+SEARCH_MODE_MEDIA_ALL-SEARCH_MODE_DISKETTE)  // exclude floppy
// HSM 과 DISK 제외 // HSM DEVEL VER.7.9.0.1[2007/09]  DEFINE
#define SEARCH_BACKUP			(SEARCH_COMM_MATCH_ALL+SEARCH_MODE_CA_SIGNKOREA+SEARCH_MODE_MEDIA_ALL-SEARCH_MODE_DISKETTE-SEARCH_MODE_P11)
#define SEARCH_HARDDISK			(SEARCH_COMM_MATCH_ONE+SEARCH_MODE_CA_SIGNKOREA+SEARCH_MODE_HARDDISK)	// match one
#define SEARCH_DISKETTE			(SEARCH_COMM_MATCH_ONE+SEARCH_MODE_CA_SIGNKOREA+SEARCH_MODE_DISKETTE)	// match one
#define SEARCH_USBMEDIA			(SEARCH_COMM_MATCH_ONE+SEARCH_MODE_CA_SIGNKOREA+SEARCH_MODE_USBKEY)	// match one
#define SEARCH_SMARTCARD		(SEARCH_COMM_MATCH_ONE+SEARCH_MODE_CA_SIGNKOREA+SEARCH_MODE_SMARTCARD)	// match one


typedef struct _APP_CONTEXT {
	int SignatureAlg;
	int EncryptionAlg;
	int UsedStorage ;
	int CertPathInc;
	int PathValidationOp;		/* 0:do not, 1:try, 2:do, 3:do_but_exclude_crl_check */
	char RecipientDN[E_BUFLEN+1] ;
	unsigned char SessionKey[128] ;
	int	 nSessionKeyLen ;

	/* 2000.09.27 */
	char SignerDN[E_BUFLEN+1];				/* 서명검증시 서명자의 식별명 */
	char DefaultDN[E_BUFLEN+1];

#define USE_SD_CONTEXT_EMPTY				0x00
#define USE_SD_CONTEXT_GLOBAL				0x01
#define USE_SD_CONTEXT_PRESET				0x02
	int bUseGlobalContext ;			/* bUseGlobalContext != USE_SD_CONTEXT_EMPTY,
					then pInterfaceContext is set to global/pre-set context */
	/* alert !!! : do not refer */
	SD_API_CONTEXT *pInterfaceContext ;
} APP_CONTEXT;

/* !!!!!!!!!!! Important !!!!!!!!!!!
// int bUseGlobalContext 로서 변경 사용됨. 다음 api와 함께 복합적으로 사용됨.
// static context를 free(refresh)하기 위해서는 별도의 api
// sk_if_cert_static_context_release(...) 를 먼저 사용해야 함
// bUseGlobalContext는 단순히 static context를 사용할 것인지 안 할 것인지만을 결정
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

typedef struct _PKEY_CONTEXT {
	unsigned char value[1024] ;
	int		 length ;
	int		 keytype;
#define PKEY_KEY	0x00
#define PKEY_CERT	0x01	/* do not use anymore... use only PKEY_KEY */
							/* it may occur fatal error, because cert exceed 1024 */
} PKEY_CONTEXT ;


typedef struct _CERTKEY_CONTEXT {
	unsigned char value[2048] ;
	int		 length ;

	/* CERTKEY_CERT == 0x00, CERTKEY_PKEY == 0x01 */
	int		 req_type;
#define CERTKEY_CERT			0x00
#define CERTKEY_PKEY			0x01
} CERTKEY_CONTEXT ;

typedef struct _SMARTCARD_PARAMS {
	const char *pszPin ;
	const char *pszMap ;
	const char **ppszFidList ; /* combination list of "dn", "signkey", "signcert" */
} SMARTCARD_PARAMS ;


/*---------------- Path Include Flag Value Start ----------------------*/
/* internal value == direct handling */
#define IN_CERT_PATH					0x00000020	/* 경로상의 모든 인증서 첨부 */
#define IN_CERT_ONLY					0x00000021	/* 2000.5.19 ADD 사용자 인증서만을 첨부 */
#define EX_CERT_PATH					0x00000022	/* 사용자 DN만 첨부 */

/*	external (api level) value
	case 0 :	= EX_CERT_PATH ;
	case 1 :	= IN_CERT_ONLY ;
	case 2 :	= IN_CERT_PATH ;
*/

/*---------------- Path Include Flag Value end    ----------------------*/

typedef struct _SIGNEDMSG_INFO {
	CString256	SignInfo ;		/* 2048 RSA Supported */
	ExtCertInfo	CertInfo ;
} SIGNEDMSG_INFO ;

typedef struct _SIGNEDMSG_INFO_V2 {
	CString256	SignInfo ;		/* 2048 RSA Supported */
	CString2048	Certificate ;
} SIGNEDMSG_INFO_V2 ;

/*---------------- Path Validation Flag Value Start ----------------------*/
#define	 FLAG_READ_FROM_REGISTRY	0xffffffff
				/* Registry value 아님 !!!!! */
				/* global setting을 이용 (default) */

/* internal value == Registry value == direct handling */

/* priority : FLAG_VALIDATE_TRY > FLAG_VALIDATE_NO > FLAG_VALIDATE_YES */
#define	 FLAG_VALIDATE_NO				0x00000000		/* 인증서를 검증 하지 않음 */
#define	 FLAG_VALIDATE_TRY				0x00000001		/* 일단 시도, 결과는 무시 == 무조건 성공 */
#define	 FLAG_VALIDATE_YES				0x00000002		/* 인증서를 검증함 */
#define	 FLAG_VALIDATE_YES_NO_CRL		0x00000004		/* 인증서를 검증함, CRL 검사는 하지 않음 */
#define	 FLAG_VALIDATE_YES_ROOT			0x00000008		/* 최상위 인증서 신뢰성 확인 */
#define	 FLAG_VALIDATE_CRL_MASK		    0x0000000F		/*[9.8.0.0] Flag Optimize */

#define	 FLAG_VALIDATE_YES_OCSP			0x00000010		/* OCSP */
#define	 FLAG_VALIDATE_YES_TIMESTAMP	0x00000020		/* TIMESTAMP */
//[9.8.0.0]][2011/07/15] #125 EPKI 인증서 연동개발 [GPKI 검증]  SUPERSK
#define	 FLAG_VALIDATE_GPKI         	0x00000100		/*[9.8.0.0] GPKI Certificate */

#define	 FLAG_VALIDATE_KM				0x80000000		/* 위와 3값에 bit-OR로 사용됨 */
#define	 FLAG_VALIDATE_MASK				0x0000FFFF		/* for high-word masking */

/*---------------- Path Validation Flag Value end   ----------------------*/


typedef struct _VERIFY_EXTENTION_INFO {
	int nOperationFlag ;
#define EXT_NO_VERIFY				0x00000000
#define EXT_DO_VERIFY				0x00000001

	SIGNEDMSG_INFO SignInfo ;
} VERIFY_EXTENTION_INFO ;


typedef struct _VERIFY_EXTENTION_INFO_V2 {
	int nOperationFlag ;
#define EXT_NO_VERIFY				0x00000000
#define EXT_DO_VERIFY				0x00000001

	SIGNEDMSG_INFO_V2 SignInfo ;
} VERIFY_EXTENTION_INFO_V2 ;

//[2008/01/31] 암호 패키지 통합
typedef struct _SessionInfo {
	unsigned char *SessID;
	int nSessID;
	unsigned char *SessKey;
	int nSessKey;
	unsigned char *SvrCert;
	int nSvrCert;
} SESSIONINFO_CONTEXT ;

//[2021/10/22] 통합인증앱 연동 기능 추가 - 요청 메시지 구조체 추가
typedef struct _RequestDataBox {
	int  opCode;             //기능 opCode
	char companyID[10];      //회사 아이디
	char App_AndroidID[80];  //안드로이드 appid
	char App_iosID[80];      //아이폰 appid
	char UserDn[200];        //선택할 인증서 Dn 
	unsigned char *pInputData; //원문
	int pInputDatalen;       //원문 길이
	int ui;                  //ui 선택
	int link;                //기능 옵션
	int authSelect;          //인증수단 선택
	int policyMode;          //정책 필터 모드
	char * policyList;	     //정책 리스트
	int useTestServer;       //테스트서버 사용유무
} REQUESTDATABOX ;

//[2021/10/22] 통합인증앱 연동 기능 추가 - 결과 메시지 구조체 정의
typedef struct _ResultDataBox {
	unsigned char * pOutSignData; //서명 결과값
	int  pOutSignDatalen;
	char selectCertDn[301];       //사용한 인증서 DN
	char certPeriodFrom[51];      //인증서 유효기간
	char certPeriodTo[51];
	int regAuthList;              //인증서에 등록된 인증수단 리스트
	int selectedAuthType;         //서명에 사용한 인증수단
	char companyID[11];           //회사 아이디
} RESULTDATABOX ;


//Cloud 관련

//Cloud 인증서 선택 기능 관련 옵션
#define SELECT_CLOUDCERT_OPTION_NOSEARCHLOCAL	1
#define SELECT_CLOUDCERT_OPTION_INPUTPIN		2
#define SELECT_CLOUDCERT_OPTION_NOCERT_OUT		4
#define SELECT_CLOUDCERT_OPTION_NOCERT_DISCONNECT	8
#define SELECT_CLOUDCERT_OPTION_CERTNEW_ALARM		16
#define SELECT_CLOUDCERT_OPTION_CERTLOCK_RETURN		32

//cloud config 값 넘겨주는 구조체 추가
//[10.0.2.4] [2022/11/23] #289 클라우드 config 구조체 수정
typedef struct _CloudConfig {	
	char * SITE_CODE[10];
	char * CUSTOMER_ID;
	char * SERVER_HOST;
	char * AGREEMENT_URL;
	char * VERSION;
	int SERVER_PORT;
	unsigned int TIMEOUT_MSEC; /* ms */
}CloudConfig;

#pragma pack(pop)

#endif /* _INTERFACETYPES_H */

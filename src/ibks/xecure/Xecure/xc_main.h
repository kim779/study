#ifndef	__XC_MAIN_H__
#define	__XC_MAIN_H__

#ifdef WIN32
#ifndef  WINAPI
#define WINAPI __stdcall
#endif
#else
#define WINAPI
#endif

#include "xc_protocol.h"

/* Subject Mode */
#define XC_SMODE_CLIENT		1
#define XC_SMODE_SERVER		2

/* Transaction Mode : function input value */
#define XC_TMODE_KEY		10
#define XC_TMODE_MSG		20
#define XC_TMODE_RESUME		30

#define XC_TMODE_KEYEX		40
#define XC_TMODE_MSG_V1		50

#define XC_TMODE_KEYEX_V3	60
#define XC_TMODE_MSG_V3		70

/* Message Type : function return value */
#define XC_MTYPE_HANDSHAKE	100
#define XC_MTYPE_MESSAGE	200
#define XC_MTYPE_NEWPROFILE	300

/* Padding mode for xc2 */
#define XC_V2PAD_MODE_SCP2	0
#define XC_V2PAD_MODE_PKCS5	1

/* Cert Info Mode : function input */
#define XC_CERTINFOFIELD_SERIAL						111
#define XC_CERTINFOFIELD_ISSUER						112
#define XC_CERTINFOFIELD_SUBJECT					113
#define XC_CERTINFOFIELD_SUBJECT_ALT_NAME				114
#define XC_CERTINFOFIELD_SIGNATUREALGORITHM				115
#define XC_CERTINFOFIELD_USAGE						116
#define XC_CERTINFOFIELD_POLICYID					117
#define XC_CERTINFOFIELD_CRLDP						118
#define XC_CERTINFOFIELD_AUTHORITY_INFO_ACCESS				119
#define XC_CERTINFOFIELD_VALIDITY_FROM					120
#define XC_CERTINFOFIELD_VALIDITY_TO					121
#define XC_CERTINFOFIELD_UNIQUEID					199

/* Cert Object type */
#define XC_OTYPE_A							0x00		/* all */
#define XC_OTYPE_P							0x01		/* personal */
#define XC_OTYPE_B							0x02		/* cop */
#define XC_OTYPE_L							0x04		/* limited */

/* CN */
#define XC_CN_KISA			"CertRSA01"
#define XC_CN_KISA2			"KISA RootCA 1"
#define XC_CN_CROSSCERT			"CrossCertCA"
#define XC_CN_CROSSCERT2048		"CrossCertCA2"
#define XC_CN_CROSSCERT3		"CrossCertCA3"
#define XC_CN_CROSSCERT2		"CrossCert Certificate Authority"
#define XC_CN_SIGNGATE			"signGATE CA"
#define XC_CN_SIGNGATE2			"signGATE CA2"
#define XC_CN_SIGNGATE4			"signGATE CA4"
#define XC_CN_SIGNGATE5			"signGATE CA5"
#define XC_CN_SIGNGATE_FTCA		"signGATE FTCA02"
#define XC_CN_SIGNKOREA			"SignKorea CA"
#define XC_CN_SIGNKOREA2		"SignKorea CA2"
#define XC_CN_SIGNKOREA3		"SignKorea CA3"
#define XC_CN_SIGNKOREA_TEST		"SignKorea Test CA"
#define XC_CN_TRADESIGN			"TradeSignCA"
#define XC_CN_TRADESIGN2		"TradeSignCA2"
#define XC_CN_TRADESIGN3		"TradeSignCA3"
#define XC_CN_TRADESIGN_TEST		"TestTradeSignCA"
#define XC_CN_YESSIGN			"yessignCA"
#define XC_CN_YESSIGN_TEST	  	"yessignCA-TEST"
#define XC_CN_YESSIGNCLASS1		"yessignCA Class 1"
#define XC_CN_YESSIGNCLASS2		"yessignCA Class 2"
#define XC_CN_YESSIGN_TEST_CLASS1	"yessignCA-Test Class 1"
#define XC_CN_YESSIGN_TEST_CLASS2	"yessignCA-Test Class 2"
#define XC_CN_NCASIGN			"NCASign CA"
#define XC_CN_NCASIGN_TEST		"NCATESTSign3280CA"
#define XC_CN_GPKI_CA01			"CA131000001"
#define XC_CN_GPKI_CA02			"CA131000002"
#define XC_CN_GPKI_CA31T		"CA131000031T"
#define XC_CN_MPKI			"MND CA"
#define XC_CN_MPKI_TEST			"mpki mnd ca"

/* CN constant */
#define XC_CNCONST_KISA			0
#define XC_CNCONST_KISA2		1
#define XC_CNCONST_CROSSCERT		10
#define XC_CNCONST_CROSSCERT2		11
#define XC_CNCONST_CROSSCERT2048	12
#define XC_CNCONST_CROSSCERT3		13
#define XC_CNCONST_SIGNGATE		20
#define XC_CNCONST_SIGNGATE2		21
#define XC_CNCONST_SIGNGATE4		22
#define XC_CNCONST_SIGNGATE5		25
#define XC_CNCONST_SIGNKOREA		30
#define XC_CNCONST_SIGNKOREA2		31
#define XC_CNCONST_SIGNKOREA3		32
#define XC_CNCONST_TRADESIGN		40
#define XC_CNCONST_TRADESIGN2		41
#define XC_CNCONST_TRADESIGN3		42
#define XC_CNCONST_YESSIGN		50
#define XC_CNCONST_YESSIGNCLASS1	51
#define XC_CNCONST_YESSIGNCLASS2	52
#define XC_CNCONST_NCASIGN		60
#define XC_CNCONST_GPKI_CA01		70
#define XC_CNCONST_GPKI_CA02		71
#define XC_CNCONST_MPKI			80

#define XC_CNCONST_SIGNGATE_FTCA	120
#define XC_CNCONST_SIGNKOREA_TEST	130
#define XC_CNCONST_TRADESIGN_TEST	140
#define XC_CNCONST_YESSIGN_TEST		150
#define XC_CNCONST_YESSIGN_TEST_CLASS1	151
#define XC_CNCONST_YESSIGN_TEST_CLASS2	152
#define XC_CNCONST_NCASIGN_TEST		160
#define XC_CNCONST_GPKI_CA31T		170
#define XC_CNCONST_MPKI_TEST		180

typedef struct {
	unsigned char   pre_keyblock_sha256[128];
	unsigned char   pre_keyblock[104];
	unsigned char	pre_csecret[48];
	unsigned int	lseq;				/* local sequence number */
	unsigned int	pseq;				/* peer sequence number */
	unsigned char	pid[32];
	unsigned char	pid_len;
	unsigned char	sid[32];
	unsigned char	sid_len;
	unsigned char	mode;
	unsigned char   mode_len;

	unsigned char   keyinit_hash[20];
	unsigned char   keyinit_hash256[32];
	int		client_mversion;
	int		server_mversion;

	/* For shared secret mode */
	unsigned char	client_id[32];
	unsigned char	client_id_len;
	unsigned char	server_id[32];
	unsigned char	server_id_len;
	unsigned char	client_random[48];
	unsigned char	pre_shared_key[48];

	/* For session resume */
	unsigned char	pre_master_ex_sha256[80];
	unsigned char	pre_master_ex[68];

	int		xc2pad_mode;
	int		longmsg_flag;
} XC_CTX;

typedef struct {
	unsigned char	plain[10240];
	int		plain_len;
	unsigned char	cert[10240];
	int		cert_len;
} XC_SIGNVERIFY_CTX;

extern unsigned char NULL_VALUE[];

#ifdef  __cplusplus
extern "C" {
#endif

/* Security API : Main function */
int WINAPI XC_INIT			(char *passwd, char *conf_file, int smode);
int WINAPI XC_ENCODE			(XC_CTX *ctx, unsigned char *out, int *out_len, unsigned char *in, int in_len, unsigned char *id, int id_len, int tmode);
int WINAPI XC_DECODE			(XC_CTX *ctx, unsigned char *out, int *out_len, unsigned char *in, int in_len, unsigned char *id, int id_len);

/* Digital sign and envelope */
void WINAPI XC_SIGNBRIEFON		(void);
void WINAPI XC_SIGNBRIEFOFF		(void);
int WINAPI XC_SIGN			(unsigned char *out, int *out_len, unsigned char *in, int in_len);
int WINAPI XC_SIGN_W			(uint8 *out, int *out_len, uint8 *in, int in_len, char * acceptableList);
int WINAPI XC_SIGNGETCERT	 	(unsigned char *cert, int *cert_len, uint8 *in, int in_len);
int WINAPI XC_SIGNGEN			(unsigned char *out, int *out_len, unsigned char *in, int in_len, unsigned char *cert, int cert_len, unsigned char *key, int key_len, char *passwd);
int WINAPI XC_SIGNVERIFY		(unsigned char *out, int *out_len, unsigned char *in, int in_len);
int WINAPI XC_SIGNVERIFYEX		(XC_SIGNVERIFY_CTX *out, uint8 *in, int in_len);
int WINAPI XC_SIGNCHECK			(unsigned char *out, int *out_len, unsigned char *in, int in_len);
int WINAPI XC_ENVENC			(unsigned char *out, int *out_len, unsigned char *in, int in_len, unsigned char *cert, int cert_len);
int WINAPI XC_ENVENC_EX			(uint8 *out, unsigned int *out_len, uint8 *in, unsigned int in_len, uint8 *cert, int cert_len, int algID);
int WINAPI XC_ENVDEC			(unsigned char *out, int *out_len, unsigned char *in, int in_len, unsigned char *cert, int cert_len, unsigned char *key, int key_len, char *passwd);
int WINAPI XC_ENVDEC2			(XC_CTX *ctx, unsigned char *out, int *out_len, unsigned char *in, int in_len, unsigned char *cert, int cert_len, unsigned char *key, int key_len, char *passwd);
int WINAPI XC_ENVDEC3 			(XC_CTX *ctx, unsigned char *out, int *out_len, unsigned char *in, int in_len, unsigned char *cert, int cert_len, unsigned char *key, int key_len, char *passwd);
int WINAPI XC_ENCODE3	 		(XC_CTX *ctx, uint8 *out, int *out_len, uint8 *in, int in_len);

int WINAPI XC_SIGNIDV			(unsigned char *out, int *out_len, unsigned char *in, int in_len, char *idn, int idn_len);
int WINAPI XC_SIGNIDV_EX		(unsigned char *out, int *out_len, unsigned char *in, int in_len, char *idn, int idn_len);
int WINAPI XC_SIGNIDV_W			(uint8 *out, int *out_len, uint8 *in, int in_len, char *idn, int idn_len, char * acceptableList);
int WINAPI XC_SIGNIDV_VERIFY		(unsigned char *out, int *out_len, unsigned char *in, int in_len, char *idn, int idn_len);
int WINAPI XC_SIGNIDV_VERIFY_EX		(unsigned char *out, int *out_len, unsigned char *in, int in_len, char *idn, int idn_len);
int WINAPI XC_GET_CERTINFO		(char *sz_out, uint8 *cert, int cert_len, int field);
int WINAPI XC_GET_CERTINFO1		(char *sz_out, uint8 *cert, int cert_len);

int WINAPI XC_CERT_VALIDATE		(uint8 *cert, int cert_len, int cert_type);
/* Verification ID */
int WINAPI XC_SETIDV			(unsigned char *out, int *out_len, char *idn, int idn_len);
int WINAPI XC_IDVERIFY			(unsigned char *in,	int in_len, char *idn, int idn_len);
int WINAPI XC_VID_VERIFY		(unsigned char *rvalue, int rvalue_len, unsigned char *vid, int vid_len, char *idn, int idn_len);
int WINAPI XC_VID_VERIFY_EX		(unsigned char *signed_msg, int signed_msg_len, unsigned char *env_idn_r, int env_idn_r_len);
int WINAPI XC_VID_VERIFY_EX_NO_IDN	(unsigned char *signed_msg, int signed_msg_len, unsigned char *env_r, int env_r_len, unsigned char *idn, int idn_len);
int WINAPI XC_GET_IDN_FROM_ENV		(unsigned char *idn, int *idn_len, unsigned char *env_idn_r, int env_idn_r_len);

/* Policy */
int WINAPI XC_ADDCN			(char *cn);
int WINAPI XC_ADDCN_CONST		(int cn_const);
int WINAPI XC_ADDOID			(char *cn, char *oid);
int WINAPI XC_ADDOID_CONST		(int cn_const, char *oid);
int WINAPI XC_DELCN			(char *cn);
int WINAPI XC_DELCN_CONST		(int cn_const);
int WINAPI XC_DELOID			(char *cn, char *oid);
int WINAPI XC_DELOID_CONST		(int cn_const, char *oid);
int WINAPI XC_DELCA			(char *cn);

/* Text Codec */
int WINAPI XC_BINTOTEXT			(char *out, int *out_len, unsigned char *in, int in_len);
int WINAPI XC_TEXTTOBIN			(unsigned char *out, int *out_len, char *in, int in_len);
int WINAPI XC_BINTOHEX			(char *out, int *out_len, unsigned char *in, int in_len);
int WINAPI XC_HEXTOBIN			(unsigned char *out, int *out_len, char *in, int in_len);

/* Error debugging and logging */
void WINAPI XC_TRACEIN			(char *file);
void WINAPI XC_TRACEOUT			(void);
void WINAPI XC_ERROR			(char *file);
char * WINAPI XC_GETERR			(void);
void WINAPI XC_VERSION			(char *file);
void WINAPI XC_STATUS			(XC_CTX *ctx, char *file);
void WINAPI XC_BINOUT			(char *title, unsigned char *data, int len);
void WINAPI XC_FBINOUT			(char *fname, char *title, unsigned char *data, int len);
void WINAPI XC_SBINOUT			(char *msg, char *title, unsigned char *data, int len);
void WINAPI XC_ERROR_MSG		(char *out, int *out_len, int error_code);

/* TCP Socket */
int WINAPI XC_SRVSOCK_INIT		(int port);
int WINAPI XC_CLNTSOCK_INIT		(void);
int WINAPI XC_CONNECT			(char *IP, int port, int timeout);
int WINAPI XC_ACCEPT			(int master, int timeout);
int WINAPI XC_RECV			(int s, unsigned char *vbuf, int len);
int WINAPI XC_SEND			(int s, unsigned char *vbuf, int len);
int WINAPI XC_GETSOCKERR		(void);
void WINAPI XC_CLOSESOCK		(int sock);

/* Additional */
void WINAPI XC_CTXCOPY			(XC_CTX *out, XC_CTX *in);	/* For session resume */
void WINAPI XC_CTXFINALIZE		(XC_CTX *in);
void WINAPI XC_SEQCHKON			(void);				/* Sequence number check */
void WINAPI XC_SEQCHKOFF		(void);				/*                       */
void WINAPI XC_MACALG			(int alg);			/* Change mac algorithm */
void WINAPI XC_CIPHERALG		(int alg);			/* Change cipher algorithm */
void WINAPI XC_MSGLONG			(void);				/* When data size is over 63K */
void WINAPI XC_MSGSHORT			(void);				/* Set default data size flag (under 63K) */
void WINAPI XC_SMANON			(void);				/* Use session manager */
void WINAPI XC_SMANOFF			(void);				/* Do not use session manager */
int WINAPI XC_GETMODE			(void);				/* Get current cert mode */
void WINAPI XC_V2PAD_MODE		(int mode);

/* EXPORT CLIENT API FOR COM */
int WINAPI XC_ENCODE_TEXT		(XC_CTX *ctx, unsigned char *out, int *out_len, unsigned char *in, int in_len, int tmode);
int WINAPI XC_DECODE_TEXT		(XC_CTX *ctx, unsigned char *out, int *out_len, unsigned char *in, int in_len );

int WINAPI XC_VERIFYVID_TEXT		(unsigned char *out, int *out_len, char *idn, int idn_len );
int WINAPI XC_SIGNGEN_TEXT		(unsigned char *out, int *out_len, unsigned char *in, int in_len, unsigned char *cert, int cert_len, unsigned char *key, int key_len, char *passwd);
int WINAPI XC_SIGN_TEXT			(unsigned char *out, int *out_len, unsigned char *in, int in_len);
int WINAPI XC_SIGNIDV_TEXT		(unsigned char *out, int *out_len, unsigned char *in, int in_len, char *idn, int idn_len);
int WINAPI XC_GETSID			(XC_CTX *ctx, uint8 * out , int * out_len);
int WINAPI XC_SETSID			(XC_CTX *ctx, uint8 * in , int in_len);
int WINAPI XC_CHAINCERT_VALIDATE	(uint8 *cert, int cert_len, int cert_type);

/* For PEM Encode and Decode  */
int WINAPI XC_PEM_ENCODE		(char *out, uint8 *in, int in_len, char *pem_header);
int WINAPI XC_PEM_DECODE		(uint8 *out, int *out_len, char *pem);
int WINAPI XC_FILE_PEM_ENCODE		(char *out, char *filename, char *pem_header);
int WINAPI XC_FILE_PEM_DECODE		(char *pem, char *filename);
int WINAPI XC_GET_SRVCERT_PEM		(char *out);
int WINAPI XC_GET_SRVCERT_PEM_FOR_WEB	(char *out);
int WINAPI XC_GET_SRVCERT_PEM_FOR_WEB_EX(char *out, int opt);

int WINAPI XC_ENVENC_FILE		(uint8 *out, int *out_len, uint8 *in, int in_len, char *certpath);
int WINAPI XC_ENVDEC_FILE		(uint8 *out, int *out_len, uint8 *in, int in_len, char *certpath, char *keypath, char *passwd);

int WINAPI XC_ENVENC_E2E		(XC_CTX *ctx, uint8 *out, int *out_len, uint8 *in, int in_len, char *certpath);
int WINAPI XC_ENVDEC_E2E		(XC_CTX *ctx, unsigned char *out, int *out_len, unsigned char *in, int in_len, char *certpath, char *keypath, char *passwd);
int WINAPI XC_ENCODE_E2E		(XC_CTX *ctx, uint8 *out, int *out_len, uint8 *in, int in_len);
int WINAPI XC_DECODE_E2E		(XC_CTX *ctx, uint8 *out, int *out_len, uint8 *in, int in_len);
int WINAPI XC_ENCODE_E2E_KS		(XC_CTX *ctx, uint8 *out, int *out_len, uint8 *in, int in_len);
int WINAPI XC_ENVDEC_E2E_KS		(XC_CTX *ctx, unsigned char *out, int *out_len, unsigned char *in, int in_len, char *certpath, char *keypath, char *passwd);

#ifdef  __cplusplus
}
#endif

#endif

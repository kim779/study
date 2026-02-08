#ifndef _INTERFACEDLL_H
#define _INTERFACEDLL_H

#include "InterfaceTypes.h"

#ifdef _SKEURE 
#include "session_api.h"
#endif

#define IN
#define OUT
#define IO

#ifdef __cplusplus
extern "C" {
#endif

	/* 일반기능 : SD_API_CONTEXT(_NEW)를 저장하기위해서 사용하면 절대 안됨 */
	/* SESSION_MANAGER pSm은 sk_if_SessionCreate() 호출이전에 초기화되어 있어야 함, 반드시 !!! */
	__declspec(dllimport) int sk_if_SessionCreate(SESSION_MANAGER *pSm, LPCTSTR server, LPCTSTR service) ;
	__declspec(dllimport) int sk_if_SessionDelete(SESSION_MANAGER *pSm) ;
	__declspec(dllimport) int sk_if_SessionPutData(SESSION_MANAGER *pSm, UString *pData) ;
	__declspec(dllimport) int sk_if_SessionGetData(SESSION_MANAGER *pSm, UString *pData) ;

	__declspec(dllimport) BOOL sk_if_SetSessionServicePort(int port) ;
	//	[2005/06/21] SESSION MANAGER DELETE
	__declspec(dllimport) BOOL sk_if_ClearSessionService(LPCTSTR server, LPCTSTR service) ;

	__declspec(dllimport) void sk_if_CleanUp(int Device = 0) ;
	__declspec(dllimport) void sk_if_SetGlobalCardPin(const char *pin) ;

	__declspec(dllimport) BOOL sk_if_PEM2BIN(const char *pPEM, unsigned char **ppBIN, int *pnLen) ;
	__declspec(dllimport) BOOL sk_if_BIN2PEM(const unsigned char *pBin, int nLen, char **ppPEM) ;
	__declspec(dllimport) BOOL sk_if_DnExtract(const unsigned char *pCert, int nLen, char *pDn300Bytes) ;
	__declspec(dllimport) BOOL sk_if_PublicKeyExtract(const unsigned char *pCert, int nLen, PKEY_CONTEXT *pKeyContext) ;

	/* not supported start */
	__declspec(dllimport) BOOL sk_if_EncryptPrivateKey(
						const unsigned char *pDecryptedKey, int nDecryptedKeyLen,
						char *pszPasswd,
						      unsigned char *pEncryptedKey, int *pnEncryptedKeyLen) ;
	__declspec(dllimport) BOOL sk_if_DecryptPrivateKey(
						const unsigned char *pEncryptedKey, int nEncryptedKeyLen,
						char *pszPasswd,
						      unsigned char *pDecryptedKey, int *pnDecryptedKeyLen) ;
	/* not supported end */
	__declspec(dllimport) BOOL sk_if_EncryptPrivateKey_Ext(
						const unsigned char *pDecryptedKey, int nDecryptedKeyLen,
						R_INFO *pR,
						char *pszPasswd,
						unsigned char *pEncryptedKey, int *pnEncryptedKeyLen) ;
	__declspec(dllimport) BOOL sk_if_DecryptPrivateKey_Ext(
						const unsigned char *pEncryptedKey, int nEncryptedKeyLen,
						char *pszPasswd,
						unsigned char *pDecryptedKey, int *pnDecryptedKeyLen,
						R_INFO *pR) ;

/* 2000.08.30 new added */
	__declspec(dllimport) BOOL sk_if_DownloadCRLs(const char *pSignCertBase64, char **ppCRLBase64, char **ppARLBase64) ;
	__declspec(dllimport) BOOL sk_if_DownloadSignCert(const char *pDN, char **ppSignCertBase64) ;

	/* general function for certificate downloading */
	__declspec(dllimport) int  sk_if_cert_DownloadCertificate(const char *pDN, int nType, UString *puCert) ;
													/* nType = E_KM_CERT, E_SN_CERT */

	__declspec(dllimport) BOOL sk_if_VerifySignCert(const char *pSignCertBase64) ;
	__declspec(dllimport) BOOL sk_if_VerifySignCertV2(const char *pSignCertBase64, int nType) ;	/* 0,1,2 */
	__declspec(dllimport) BOOL sk_if_GetSignCert(SD_API_CONTEXT *pContext, char **ppSignCertBase64) ;
	__declspec(dllimport) BOOL sk_if_GetSignKey(SD_API_CONTEXT *pContext, char **ppSignKeyBase64) ;

	__declspec(dllimport) BOOL sk_if_SearchRenewedCertificate(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_RestoreCertificateInfo(SD_API_CONTEXT *pContext) ;

	/* primitive storage api */
	__declspec(dllimport) int sk_if_SelectStorage(LPCTSTR pszTitleMsg, BYTE nDisableCurrentMedia, BYTE *pnStorage) ;
	
	/* api for CertManager */
	/* CAUTION !!!!!!!!
	   You must initialize variable pointed by "pContext", OR
				set "pContext" to NULL pointer OR exact values
	   If you don't, then the result can be fatal */
	__declspec(dllimport) LPCTSTR sk_if_GetLastErrorMsg() ;

	/* old certman api - Recommend : do not use */
	__declspec(dllimport) BOOL sk_if_CertManager_Backup(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_Recover(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_Issue(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_Suspend(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_Revoke(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_KeyNew(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_CertNew(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_ChangePWD(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_VerifyPassword(SD_API_CONTEXT *pContext) ;

	/* old certman api - Recommend : use this */
	__declspec(dllimport) BOOL sk_if_CertManager(SD_API_CONTEXT_NEW *pContext, CONTEXT_TYPE nContextType) ;


	/* select old function : only returned signkey */
	__declspec(dllimport) BOOL sk_if_CertSetSelect(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertSetSelectV2(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertSetListing(SD_API_CONTEXT *pContext) ; // view all (Query)
	__declspec(dllimport) BOOL sk_if_CertSetListingV2(SD_API_CONTEXT *pContext) ;

	/* select new function */
	__declspec(dllimport) BOOL sk_if_CertSetSelectExt(SD_API_CONTEXT_NEW *pContext, CONTEXT_TYPE ctype, SEARCH_TYPE stype) ;


	// find matched context
	__declspec(dllimport) int sk_if_SelectMatchedContext(SD_API_CONTEXT *pContext, const char *pIDofDN, const char *pSuffixofDN) ;
	__declspec(dllimport) int sk_if_SelectMatchedContextExt(SD_API_CONTEXT_NEW *pContext, const char *pIDofDN, const char *pSuffixofDN) ;
	__declspec(dllimport) int sk_if_SelectFirstMatchedContext(SD_API_CONTEXT *pContext, const char *pIDofDN, const char *pSuffixofDN) ;


	/* 호출전에 인증서정보(SD_API_CONTEXT or SD_API_CONTEXT_NEW)가 setting 되어야하는 함수 */
	__declspec(dllimport) BOOL sk_if_RemoveFromMedia(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_ChangeStorageMedia(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_RemoveFromMediaExt(SD_API_CONTEXT_NEW *pContext) ;
	__declspec(dllimport) BOOL sk_if_ChangeStorageMediaExt(SD_API_CONTEXT_NEW *pContext) ;


	__declspec(dllimport) void sk_if_MemFree(void *pBuf) ;
	__declspec(dllimport) int  sk_if_Hash(int nAlg, UString *pIn, CString128 *pOut) ;
	__declspec(dllimport) int  sk_if_cert_Hash_const(APP_CONTEXT *pApp_Context, UString *pIn, CString128 *pOut) ;

	__declspec(dllimport) int sk_if_cert_download_crl(LPCTSTR dn, UString *pCrl, int writeFlag) ;
	// recommended/must-be before-call for other _cert_ APIs
	__declspec(dllimport) int sk_if_cert_InitContextApp(APP_CONTEXT *pApp_Context, char *pTargetDN, int crl_update) ;

	// preset 
	// this function is recommended, fully support all ?cert? apis
	__declspec(dllimport) int sk_if_cert_preset_context(APP_CONTEXT *pApp_Context, const SD_API_CONTEXT *pContext) ;

	// ?static? function is not recommended !!!
	__declspec(dllimport) int sk_if_cert_set_static_context(APP_CONTEXT *pApp_Context, const SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) int sk_if_cert_static_context(APP_CONTEXT *pApp_Context, const char *pContextID) ;
	__declspec(dllimport) int sk_if_cert_static_contextV2(APP_CONTEXT *pApp_Context,
												const char *pContextID, const char *pPasswd) ;
	__declspec(dllimport) int sk_if_cert_static_context_release() ;


	__declspec(dllimport) int sk_if_cert_SignData(APP_CONTEXT *pApp_Context, const char *pPassword, UString *puIn, UString *puOut) ;
	__declspec(dllimport) int sk_if_cert_SignDataWithR(APP_CONTEXT *pApp_Context, const char *pPassword, UString *puIn, UString *puOut, UString *puR) ;


	__declspec(dllimport) int
				sk_if_cert_CMSSignDataInfo(APP_CONTEXT *pApp_Context,
						const char *pszPassword,
						int	bMakeContentInfoOrNot,
						UString *pIn, UString *pOut, UString *pR) ;

	__declspec(dllimport) int
				sk_if_cert_CMSVerifySignedDataInfo(APP_CONTEXT *pApp_Context,
						int	bMakedContentInfoOrNot,
						UString *pIn, UString *pOut,
						PKEY_CONTEXT *pKeyContext,
						VERIFY_EXTENTION_INFO_V2 *pExtention) ;

	/* 아래 api는 권장되지 않음 : 서버에서 누구인지 알수가 없다 */
	__declspec(dllimport) int sk_if_cert_SignDataWith1HashedVID
										(APP_CONTEXT *pApp_Context,
										 const char *pPassword, const char *idn,
										 UString *puIn, UString *puOut, UString *p1Hashed) ;

	__declspec(dllimport) int sk_if_cert_SignFile(APP_CONTEXT *pApp_Context, const char *pPassword, int nOutToFile, const char *pFilePath, UString *pSignInfo) ;
	__declspec(dllimport) int sk_if_cert_SignData_notEncode(APP_CONTEXT *pApp_Context, const char *pPassword, UString *puIn, UString *puOut, CERTKEY_CONTEXT *pCertKeyContext) ;

	__declspec(dllimport) int sk_if_cert_VerifySignedData(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut) ;
	__declspec(dllimport) int sk_if_cert_VerifySignedFile(APP_CONTEXT *pApp_Context, const char *pSignFilePath, const char *pOriginFilePath, UString *pVerifiedInfo) ;
	__declspec(dllimport) int sk_if_cert_VerifySignedData_notDecode(APP_CONTEXT *pApp_Context, UString *puInText, UString *puInSign, CERTKEY_CONTEXT *pCertKeyContext) ;

	__declspec(dllimport) int sk_if_cert_VerifySignedFileWithSignature(APP_CONTEXT *pApp_Context, UString *pSignInfo, const char *pOriginFilePath, SIGN_FILE_INFO *pOriginalFileInfo) ;

	/* 되도록이면, sk_if_cert_VerifySignedData_ExtV2를 사용하도록 할 것*/
	/* sk_if_cert_VerifySignedData_ExtV2를 사용할 경우
	   sk_if_cert_VerifySignedData_Ext 기능을 그대로 구현하기 위해서는
	   VERIFY_EXTENTION_INFO_V2 내에 들어있는 인증서를
	   sk_if_CertExtract를 이용해서 정보를 추출하는 기능을 구현하기를 권장함 */
	__declspec(dllimport) int sk_if_cert_VerifySignedData_Ext(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut, VERIFY_EXTENTION_INFO *pExt) ;
	__declspec(dllimport) int sk_if_cert_VerifySignedData_ExtV2(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut, VERIFY_EXTENTION_INFO_V2 *pExt) ;
	__declspec(dllimport) int sk_if_cert_VerifySignedData_ExtV2_Integrate(APP_CONTEXT *pApp_Context,UString *puIn, UString *puOut,VERIFY_EXTENTION_INFO_V2 *pExt);

	__declspec(dllimport) int sk_if_cert_EncryptData(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut) ;
	__declspec(dllimport) int sk_if_cert_DecryptData(APP_CONTEXT *pApp_Context, const char *pPassword, UString *puIn, UString *puOut) ;

	__declspec(dllimport) int sk_if_cert_GetSessionKey(APP_CONTEXT *pApp_Context) ;

	__declspec(dllimport) int sk_if_cert_Encrypt(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut) ;
	__declspec(dllimport) int sk_if_cert_Decrypt(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut) ;

	/* 2015.06.01  #450 패스워드 기반에 파일 Encrypt, Decrypt 추가 */
	__declspec(dllexport) int sk_if_cert_EncryptFile(APP_CONTEXT *pApp_Context, const char* pszPassword, const char* pszPlainFilePath, const char* pszEncryptFilePath);
	__declspec(dllexport) int sk_if_cert_DecryptFile(APP_CONTEXT *pApp_Context, const char* pszPassword, const char* pszEncryptFilePath, const char* pszDecryptFilePath);

	/* 2001.09.21 */
	__declspec(dllimport) int sk_if_cert_EncryptDataByCert(APP_CONTEXT *pApp_Context, const UString *pIn, UString *pOut, const UString *pECert) ;
	__declspec(dllimport) int sk_if_cert_DecryptDataByKey(APP_CONTEXT *pApp_Context, const UString *pIn, UString *pOut, const UString *pEKey, const char *pszPasswd) ;


	__declspec(dllimport) void sk_if_cert_MemFree(void *pBuf) ;

	__declspec(dllimport) int sk_if_cert_VerifySignedDataAndGetPubKey(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut, PKEY_CONTEXT *pKeyContext) ;
	__declspec(dllimport) int sk_if_cert_VerifySignedDataUsingPubKey(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut, PKEY_CONTEXT *pKeyContext) ;

	__declspec(dllimport) int sk_if_cert_GetCaCertififcates(const UString *pCert, UString *pCa, UString *pRootCa) ;

	__declspec(dllimport) int sk_if_cert_PathValidationOnOff(int nFlag) ;
	/* Global Path Validation Flag

		case 0 :	= FLAG_VALIDATE_NO  ;
		case 1 :	= FLAG_VALIDATE_TRY ;
		case 2 :	= FLAG_VALIDATE_YES ;

		:: api내에서만 영향 줄때에는 관련구조체 직접 변경 (APP_CONTEXT)
	*/
	/* 기타 */
	__declspec(dllimport) int sk_if_CountCertSet() ;
	__declspec(dllimport) int sk_if_GetLastErrorCode() ;

	__declspec(dllimport) int sk_if_GetPkcs12Ptr
					(const char *pszPasswd, unsigned char **pp12str, int *pLen ) ;

	__declspec(dllimport) int sk_if_GetPkcs12PtrWithR
					(const char *dn, char *pszPasswd, UString *pP12Out, UString *pROut, int rtype) ;

	__declspec(dllimport) int sk_if_GetPkcs12PtrWithR_ebid
					(const char *dn, char *pszPasswd, unsigned char **pp12str, int *pLen,
													  unsigned char **ppR, int *pRLen, int rtype) ;

	__declspec(dllimport) BOOL sk_if_ChangePasswd
					( const char *pszOldPasswd, const char *pszNewPasswd ) ;
	__declspec(dllimport) BOOL sk_if_ChangePasswdV2
					(SD_API_CONTEXT_NEW *pContext, const char *pszOldPasswd, const char *pszNewPasswd) ;

	__declspec(dllimport) BOOL sk_if_ValidateCert(const char *pSignCertBase64) ;

	__declspec(dllimport) int sk_if_CertificateStatus(SD_API_CONTEXT *pContext) ;

	__declspec(dllimport) int sk_if_StoreContextP12(SD_API_CONTEXT_NEW *pContext) ;

	/* 2001.04.03 : Must be called after sk_if_CertSetSelect or sk_if_CertSetListing */
	__declspec(dllimport) int sk_if_UpdateSignCert(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) int sk_if_SetSmartCardPort(int nPort) ;

	__declspec(dllimport) void sk_if_ConfigMenu() ;
	__declspec(dllimport) void sk_if_ConfigMenuX(LPCTSTR cmdline) ;

	__declspec(dllimport) int sk_if_SetOldCertUpdateMode(int nMode) ;
	__declspec(dllimport) int sk_if_SetSelectDialogMode(int nMode) ;
	__declspec(dllimport) int sk_if_SetDisketteDrive(int nDrive) ;
	__declspec(dllimport) int sk_if_SetMiscMedia(int nMedia) ;

	/* 2001.08.09 */
	__declspec(dllimport) char *sk_if_cert_EncodeUTF8(LPCTSTR pStr) ;
	__declspec(dllimport) char *sk_if_cert_DecodeUTF8(LPCTSTR pUtf8) ;

	/* 2001.08.16 */
	__declspec(dllimport) BOOL sk_if_CertExtract(const unsigned char *pCert, int nLen, ExtCertInfo *pCertInfo) ;

	/* 2001.08.17 */
	__declspec(dllimport) void sk_if_ViewCertificate(SD_API_CONTEXT_NEW *pContext) ;



	/* for coregate : 2001/12/05 */
	__declspec(dllimport) BOOL sk_if_SetCardApi(const char *pszCardApiName) ;
	__declspec(dllimport) BOOL sk_if_SetCmpPort80() ;


	__declspec(dllimport) int sk_if_StoreContext(SD_API_CONTEXT_NEW *pContext) ;

	__declspec(dllimport) int sk_if_cert_exportp12
					(const char *pszPasswd,
					 const UString *pKey,
					 const UString *pCert,
					 UString *pPFX) ;

	__declspec(dllimport) int sk_if_cert_importp12
					(const char *pszPasswd,
					 const UString *pPFX,
					 UString *pKey,
					 UString *pCert, int *pnCertType
					 ) ;

	__declspec(dllimport) void sk_if_About() ; // program information dialog box

	/* 1 : old-compatible mode, 0 : new-mode */
	__declspec(dllimport) void sk_if_SetApiCompatibleMode(int mode) ;
	/* if 1 then pbes,pbkdf == v1.5, use old directory, donot validate rootcert */

	__declspec(dllimport) int  sk_if_cert_Get_VID_Information(UString *pECert, VID_INFO *pVID) ;
	__declspec(dllimport) int  sk_if_cert_Get_R_Information(UString *pECert, UString *pEKey,
											char *pszPassword, R_INFO *pR, /* can be NULL */VID_INFO *pVID ) ;

	__declspec(dllimport) int  sk_if_cert_Hash(int nAlg, UString *pIn, UString *pOut) ;
	__declspec(dllimport) int  sk_if_SetWrongPasswordLimit(int count) ;

	__declspec(dllimport) int  sk_if_VerifyRootCertUserConfirm(const UString *rCert, int mode) ; /* do not use */
	__declspec(dllimport) int  sk_if_cert_VerifyCertWithR(const UString *pR, char *idn, const UString *pCert, int certType) ;
	__declspec(dllimport) int  sk_if_cert_IdentifyCertificate(const UString *pCert, const char *idn, const UString *pR, int nEncFlag) ;
	__declspec(dllimport) int  sk_if_cert_IdentifyCertificate1Hash(const UString *pCert, const UString *p1Hash) ;
	__declspec(dllimport) int  sk_if_cert_GetCaNameFromDn(const char *pDn, char *ca_name_max50) ;

	__declspec(dllimport) int  sk_if_VerifySignCert_ebid(int certType, unsigned char *pCert, int nLen) ;
	 #define sk_if_cert_CheckSignCertValidation(a) sk_if_VerifySignCert_ebid(0, (a)->value, (a)->length)

	__declspec(dllimport) BOOL sk_if_Identify(SD_API_CONTEXT *pSignContext, const char *idn) ;


	__declspec(dllimport) int  sk_if_CertSetSearch(CertSetList **ppSearchResult,
								CONTEXT_TYPE ctype, SEARCH_TYPE stype) ;
	__declspec(dllimport) int  sk_if_CurrentCertSetRead(CertSetList *pToReadCertSet,
								SD_API_CONTEXT_NEW *pContext) ;
	__declspec(dllimport) int  sk_if_CertSetResultFree(CertSetList **ppSearchResult) ;

	__declspec(dllimport) void sk_if_SetLanguageMode(int english_mode) ;
	__declspec(dllimport) int  sk_if_GetLanguageMode() ;

	__declspec(dllimport) int  sk_if_SetDeviceOrder(int *new_map) ;
	//-------------------------------------------------------------
	// 인증서 선책창의 저장매체 이미지 순서 설정 기능 구현 /*[8.2.1.1]*/
	// 디폴트 순서는 [REMOVABLE, PKCS11_KEY, SMART_CARD ,HARD_DISK] 
	// [cf] sk_if_SetDeviceOrder 는 인증서 검색의 우선순위이고 이 함수는 이미지 위치만 변경함 
	__declspec(dllimport) int  sk_if_SetDeviceViewOrder(int *new_map) ; 
	//--------------------------------------------------------------
	__declspec(dllimport) int  sk_if_SetCPSFileLocation(const char *filepath) ;

	__declspec(dllimport) void sk_if_DialogModalMode(HWND hParentWnd) ;
	__declspec(dllimport) int  sk_if_SetPasswordEncMode(int boolFlag) ;


	__declspec(dllimport) int  sk_if_SetLogoFilePath(LPCTSTR logofile_408x64) ;
	__declspec(dllimport) int  sk_if_SetCertNewUrlInfo(LPCTSTR url) ;
	__declspec(dllimport) int  sk_if_SetExipreCheckSkip(int boolFlag) ;

	__declspec(dllimport) BOOL sk_if_CMP_SetCmpConfig(const char *ip, int port) ;	
	
	//-----------------------------------------------------------------------------------------
	//	- 인증서 선택시 저장매체별 수동선택 
	//	: sk_if_SetScanByDialogChoiceMode(int mode )
	//	   mode 0  기존처럼 자동으로 모든 저장매체의 인증서를 검색
	//	   mode 1  인증서 선택창에서 하드디스크를 default로 보여주고 저장매체 선택아이콘을
	//		   	   누를때마다 해당매체를 검색

	__declspec(dllimport) BOOL sk_if_SetScanByDialogChoiceMode(int mode) ;

	// VER [5.12.0.1]   if Storage Media is available , return TRUE   
	__declspec(dllimport) BOOL sk_if_SelectStorageCheck( BYTE CheckForMedia , int MsgFlag ) ;

	// VER [6.4.0.1]	 IC card pin check function
	__declspec(dllimport)  int sk_if_CardPinCheck(const char *inPin, char **ppoutPin , int Flag);

	//------------------------------------------------------------------------------------------
	// VER [6.7.0.1]
	__declspec(dllimport) int  sk_if_SetKeySaferMode(int boolFlag);
	
	/*  boolFlag
	       1 킹스정보통신
	       2 잉카인터넷
	       3 안철수 연구소
	       4 테커스

	       보안업체가 고유값 입력시  boolFlag = boolFlag + (고유값 * 16)
	*/
	//------------------------------------------------------------------------------------------
	// VER [8.2.0.5] 

	// parameter 
	//      mode =>	0  없음   1   범용 개인  16 범용 법인  256 증권전용  4096 신용카드 
	//      pOid => 정책 OID 직접 입력   (oid end 표시 ';')
	
	// 호출 예 ) 범용인증서만 선택창에서 보여주고 싶은 경우 
	//           sk_if_SetPolicyFilter(1+16, "");
	//
	//           증권전용 인증서와 OID가 1.2.410.200004.5.1.1.4 또는  1.2.410.200004.5.1.1.5 인 인증서 선택 
	//			 sk_if_SetPolicyFilter(256, "1.2.410.200004.5.1.1.4;1.2.410.200004.5.1.1.5;");
	//         
	//           정책 필터링 클리어 
	//			 sk_if_SetPolicyFilter(0,NULL)
	//  

	__declspec(dllimport) void  sk_if_SetPolicyFilter(int mode, const char *pOid = "" );

	//-------------------------------------------------------------------------------------------
	//인증서 저장매체 선택창에서 콤보박스의 최상위 인덱스 값 설정 API 
	__declspec(dllexport) int sk_if_SetTopSaveDevice(BYTE nDevice);
	
	/*-------------------------------------------------------------------------------------------
	  : 인증서 선택창에서 사용할 이미지 로고를 URL로 부터 다운받아 사용하는 기능 
	  
	   ● 파라미터 
		 BannerURL  : 다운로드 대상 URL (해당 파일은 SignKorea 에서 관리하는 키로 서명되어야함.)
		 LocalPath  : PC에 저장되는 파일명 ( 필요시 사용)

	   ● 결과 
		  성공 : TRUE
		  실패 : FALSE      
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) BOOL sk_if_SetLogoURL(const char *pBannerURL,const char *pLocalPath) ;
	

	
	//------------------------------------------------------------------------------------------
	// VER [9.9.1.7]
	__declspec(dllimport) int sk_if_SetLibraryPath(int mode, LPCTSTR strPath);
	/*
	 : 인증서 모듈에서 이용하는 외부업체의 라이브러리 참조위치를 지정하는 기능
	  
	   ● 파라미터 
		 mode     : 1)키보드 보안모듈
		 strPath  : 참조 파일이 설치되어 있는 PATH

	   ● 결과 
		  성공 : TRUE
		  실패 : FALSE      
	--------------------------------------------------------------------------------------------*/
	
	/*-------------------------------------------------------------------------------------------
		BOOL sk_if_GetMacAddress(char * macAddr , int mode)
   		
	     : 호출하는 시스템의 macAddr 와 ip 정보를 획득하는 기능 
		  
		  ● 파라미터 
			 macAddr  : 모드설정에 따른  MAC과 IP 정보 문자열
			 maxLen   : (상위에서 메모리 설정한)문자열 최대 길이
			 mode     : 필요정보 내용   하위 4bit 0 - MAC만   1 - MAC;IP;     2 - IP만
			                            상위 4bit 0 - first Device, 1(16) - default Device, 2(32) - All Device
			            
　		 ● 결과 
	　　　	성공 : TRUE     (WEB)  Mac;IP　스트링   
			실패 : FALSE    (WEB)  NULL

	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) BOOL sk_if_GetMacAddress(char * macAddr , int maxLen, int mode) ;

	/*-------------------------------------------------------------------------------------------
	   int sk_if_GetEncryptedPassword(const char *password, char *enc_password33Bytes)

		: 패스워드 암호화 함수 
        : 복호화는 인증툴킷 내부에서 자동으로 수행 
  
	  ● 파라미터 
	     password : 패스워드 원문 
		 enc_password33Bytes : 암호화 결과 (33 byte )
	   	  
	  ● 결과 
	     return : encrypted size  플래그 + 16 = 17
		                          플래그 + 32 = 33
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_GetEncryptedPassword(const char *password, char *enc_password33Bytes );	
	/*-------------------------------------------------------------------------------------------
	   int sk_if_GetEncryptedPasswordB64(const char *pPassword, char *pOutBuf, int *nOutlen );

		: 키보드 보안업체용 인증서 패스워드 암호화 함수 
		: 암호화된 인증서비밀번호가 BASE64로 인코딩되어 출력됨  
        : ACTIVE-X를 이용하여 전자서명하는 WTS에서 이용가능  
		  (HTS의 경우는 반드시 BASE64를 디코딩 한 후, 전달해야 이용가능)     

  
	  ● 파라미터 
	     pPassword : 인증서 패스워드 원문 
		 pOutBuf :  BASE64 인코딩된 암호화 결과 저장용 변수  (?? byte )
		 nOutlen : (in)  alloc된 pOutBuf Size,  0 설정후, 호출하면 반환될 buffer size를 nOutLen에 출력함.
		           (out) pOutBuf 의 유효 결과길이   
	   	           
	  ● 결과 
	     return 0  [성공] : pPassword는 초기화 
	           -1  [실패] :	패스워드 자리수 초과(30),  pOutBuf의 Size가 적은 경우 
	--------------------------------------------------------------------------------------------*/
	__declspec(dllexport) int sk_if_GetEncryptedPasswordB64(const char *pPassword, char *pOutBuf, int *nOutlen );

	/*-------------------------------------------------------------------------------------------
	 int sk_if_SetWebAccMode(int mode);
		: 장애인 웹 접근성 기능 적용 옵션  
  
	  ● 파라미터 
	    boolFlag  :    [1] 설정    [0] 해제   [-1] 현재 설정 옵션
	  ● 결과 
	　	    설정된 옵션  
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_SetWebAccMode(int mode);


	/*-------------------------------------------------------------------------------------------
	 //[9.9.2.0] 이후 적용
	 int sk_if_SetSiteInform(int mode, int nCode, const char *pEncValue );
		: 이용기관 정보 및 무결성 검증 함수. 
  
	  ● 파라미터 
	    mode  :    [1] SIGNKOREA 회원사   [2] USIM 회원사 코드   [-1] 현재 인증 이용기관 조회
		nCode :   사이트 코드
		pEncValue : 인증정보
	  ● 결과 
	　	  정상 여부  
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_SetSiteInform(int mode, int nCode, const char *pEncValue );

	/*-------------------------------------------------------------------------------------------
	 int sk_if_cert_exportp12_New
	[8.2.2.0] [2010/01/11] #3  암호화용 인증서 통합 PKCS#12 API 
  
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_cert_exportp12_New (IN const char *pszPasswd, IN const UString *pKey, IN const UString *pCert, IN const UString *pKmKey, IN const UString *pKmCert, OUT UString *pPFX);
	__declspec(dllimport) int sk_if_cert_importp12_New (IN const char *pszPasswd, IN const UString *pPFX, OUT UString *pKey, OUT UString *pCert, OUT UString *pKmKey, OUT UString *pKmCert, OUT int *pnCertType);

	
	/*-------------------------------------------------------------------------------------------
	[9.9.7.2] [2017/06/15] #212 PFX 파일 read & context 셋팅 기능
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_cert_SetContextbyReadP12 (IN const UString *pPFX, IN/*OUT*/SD_API_CONTEXT_NEW *pContext,IN int mode);
	/*-------------------------------------------------------------------------------------------

	[9.9.7.0 ] [2016/8/24] 인증서 스마트폰 상호 이동 기능
  
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_cert_export_mobile (IN const char *pszPasswd, IN const UString *pKey, IN const UString *pCert, IN const UString *pKmKey, IN const UString *pKmCert);
	__declspec(dllimport) int sk_if_cert_import_mobile (IN const char *pszPasswd, OUT UString *pKey, OUT UString *pCert, OUT UString *pKmKey, OUT UString *pKmCert);

	/*-------------------------------------------------------------------------------------------

	[9.9.9.3] [2021/10/21] 인증서 스마트폰 상호 이동 기능 서버ip, 포트 변경 가능 추가
  
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_Set_TradeSerVerIPNPort (char * IP, int PORT);
	__declspec(dllimport) int sk_if_cert_export_ToMobile ();
	__declspec(dllimport) int sk_if_cert_import_FromMobile ();

	/*-------------------------------------------------------------------------------------------

	[9.7.0.0]][2011/05/09] #122 인증서 관리기능 트레이 기능  */
  
	__declspec(dllexport) void sk_if_show_tray(void);
    __declspec(dllexport) void sk_if_hide_tray(void);

	//[9.9.9.0] [2021/03/03] 폴더 경로를 지정 해줄 수 있는 api 추가
	__declspec(dllimport) int sk_if_SetEnvPath(char *pfolderPath, int nOption);

	/*-------------------------------------------------------------------------------------------
	[9.9.9.5] [2021/10/22] #255 통합인증앱 연동 기능 추가
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_Set_AppSignServiceSerVerIPNPort(int nTestServer, char *IP, int PORT);
	__declspec(dllimport) void sk_if_MsgGetToken(const char * oridata, char *tok, char **outdata);
	__declspec(dllimport) int sk_if_MyPassApp_SignService
		(HWND hwnd, REQUESTDATABOX reqdata, int tempKeyOption, SD_API_CONTEXT_NEW * Context, RESULTDATABOX * resultbox);
	
	/*==============================================================================================
	ONLY FOR USED IN SKEURE 
    암호패키지(SKEURE) 에서만 사용하는 API
  
   :  crypto Session API FOR Active-X 
   :  인증모듈에서는 사용할 수없습니다.
	//*-----------------------------------------------------------------------------------------*/
#ifdef _SKEURE 	
	__declspec(dllimport) void sk_if_Free_SessionInformation(SESSIONINFO_CONTEXT *pSess);
	__declspec(dllimport) int  sk_if_Get_SessionInformation(const char *svrInfo, SESSIONINFO_CONTEXT *pSess);
	__declspec(dllimport) void sk_if_Close_SessionInformation(const char *svrInfo, unsigned char *SessionID, int nSessionID);

	__declspec(dllimport) void sk_if_crypto_SessionClose(APP_CONTEXT_SESSION *pApp_Context);
	__declspec(dllimport) int  sk_if_crypto_SessionInit(APP_CONTEXT_SESSION *pApp_Context);

//	[2008/10/15]
	__declspec(dllimport) int sk_if_cert_RSAEncrypt( const UString *pIn, UString *pOut, const UString *pECert, int nEncFlag = 0);
	__declspec(dllimport) int sk_if_cert_RSADecrypt( const UString *pIn, UString *pOut,	const UString *pEKey, const char *pszPasswd);

	/*=============================================================================================*/
#endif

	/*-------------------------------------------------------------------------------------------
	[10.0.0.1] [2021/11/29] #260 클라우드 인증 기능 추가
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_Set_CloudConfig(CloudConfig cloud_config);		
	__declspec(dllimport) int sk_if_Set_Show_OnlyValidateCloudCert_flag(int flag);
	__declspec(dllimport) int sk_if_Cloud_KeyPadUse(int flag);
	__declspec(dllimport) int sk_if_CloudCertSetSelectExt(SD_API_CONTEXT_NEW *pContext, int nOptionValue);
	__declspec(dllimport) int sk_if_IssueCert_toCloud(SD_API_CONTEXT_NEW *pContext, int nOptionValue);
	__declspec(dllimport) int sk_if_CertNew_toCloud(SD_API_CONTEXT_NEW *pContext, int nOptionValue);
	__declspec(dllimport) int sk_if_DeleteCert_inCloud(int nOptionValue);
	__declspec(dllimport) int sk_if_CertChangePin_inCloud(SD_API_CONTEXT_NEW *pContext, int nOptionValue);
	__declspec(dllimport) int sk_if_UploadPCtoCloud(SD_API_CONTEXT_NEW *pContext, int nOptionValue);
	__declspec(dllimport) int sk_if_DownloadCloudtoPC(int nOptionValue);
	__declspec(dllimport) int sk_if_Connected_CloudUser_Confirm(int nOptionValue);
	__declspec(dllimport) int sk_if_Cloud_AutoConnected_Device(int nOptionValue);
	__declspec(dllimport) int sk_if_CloudUser_DeleteAccount(int nOptionValue);

#ifdef __cplusplus
}
#endif

#endif // _INTERFACEDLL_H

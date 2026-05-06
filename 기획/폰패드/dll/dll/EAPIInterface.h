#ifndef _EAPI_INTERFACE_H_
#define _EAPI_INTERFACE_H_

#include "EAPIInterfaceDefine.h"

DLL_EAPI BOOL EAPIIsLogin();
DLL_EAPI BOOL EAPIIsConnect();
DLL_EAPI BOOL EAPIInitialize(RESP_CALLBACK fnRespCallBack, EVENT_CALLBACK fnEvtCallBack, UUI_CALLBACK fnUUICallBack, HWND hWnd, HANDLE hHnd);
DLL_EAPI BOOL EAPIFinalize();
DLL_EAPI BOOL EAPIConnect(const char* pszIP, const int nPort);
DLL_EAPI BOOL EAPIConnect2(const char* pszIP, const int nPort, const char* pszIP2, const int nPort2);
DLL_EAPI BOOL EAPIDisconnect(void);
DLL_EAPI BOOL EAPILogin(const char* pszID, const char* pszDeviceID, const char* pszUniqueNO, const char* pszLoginOption);
DLL_EAPI BOOL EAPIAddLogin(const char* pszID, const char* pszDeviceID, const char* pszUniqueNO);
DLL_EAPI BOOL EAPILogout(const char* pszID, const char* pszDeviceID, const char* pszUniqueNO);
DLL_EAPI BOOL EAPIMakeCall(const char* pszDialNumber, const char* pszUserToUser = NULL);
DLL_EAPI BOOL EAPIAnswerCall(const char* pszCallID = NULL);
DLL_EAPI BOOL EAPIReleaseCall(const char* pszCallID = NULL);
DLL_EAPI BOOL EAPIHoldCall(const char* pszCallID = NULL);
DLL_EAPI BOOL EAPIRetrieveCall(const char* pszCallID = NULL);
DLL_EAPI BOOL EAPISingleStepTransfer(const char* pszCalledNumber, const char* pszCallID = NULL, const int nRedirect  = 0);
DLL_EAPI BOOL EAPITransferCall(const char* pszCallID = NULL);
DLL_EAPI BOOL EAPIConferenceCall(const char* pszCallID = NULL);
DLL_EAPI BOOL EAPIIVRConsultationCall(const char* pszCallID, const char* pszCustData, const char* pszDialNumber);
DLL_EAPI BOOL EAPIConsultationCall(const char* pszCalledNumber, const int nConsultOption = 1, const char* pszCallID = NULL);
DLL_EAPI BOOL EAPIGetCallForward();
DLL_EAPI BOOL EAPISetCallForward(const char* pszForwardNumber);
DLL_EAPI BOOL EAPIReleaseCallForward(void);
DLL_EAPI BOOL EAPIGenerateDigits(const char* pszDigits, const char* pszCallID = NULL);
DLL_EAPI BOOL EAPIReconnect(const char* pszCallID = NULL);
DLL_EAPI BOOL EAPISetUUI(const char* pszCallID,  const char* pszUUIData);
DLL_EAPI BOOL EAPISetUUIKey(const char* pszCallID, const char* pszUUIKey, const char* pszUUIValue);
DLL_EAPI BOOL EAPIGetUUI(const char* pszCallID, const char* pszKey, const char* pszInternalOpt = NULL);
DLL_EAPI char* EAPIGetUUIKey(const char* pszCallID, const char* pszUUIKey);
DLL_EAPI BOOL EAPIGetUCData(const char* pszKey, const char* pszUUIKey);
DLL_EAPI BOOL EAPISetUCData(const char* pszKey, const char* pszUUIKey,const char* pszUUIValue);
DLL_EAPI BOOL EAPIPhonePad(const char* pszCallID, const char* pszSvcCode, const char* pszCustData, const char* pszDialNumber);
DLL_EAPI BOOL EAPIRemoveParty(const char* pszDialNumber, const char* pszCallID = NULL);
DLL_EAPI BOOL EAPIGetIsCall(const char* pszCallID);
DLL_EAPI char* EAPIDecrypt(const char* pszData, const char* pszKey = NULL);
DLL_EAPI char* EAPIEncrypt(const char* pszData, const char* pszKey = NULL);

DLL_EAPI char* EAPIServerInfo(void);		/** ���� Active�� ���� ����(xxx.xxx.xxx.xxx:5511)	**/
DLL_EAPI char* EAPILibraryInfo(void);		/** ���� ���̺귯�� ����							**/
DLL_EAPI char* EAPIGetForwardingNO(void);	/** ������ ��ȣ										**/
DLL_EAPI int EAPIIsForwarding(void);		/** ������ ����										**/
DLL_EAPI char* EAPIGetAccount(void);		/** Account											**/
DLL_EAPI char* EAPIGetDeviceID(void);		/** DeviceID										**/
DLL_EAPI char* EAPIGetUniqueNo(void);	/** ���� ����͸� ���� �ܸ� MAC(SEPxxxxxxxxxxxx)	**/

#endif
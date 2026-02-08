#if !defined(AFX_HANARO_IENCRYPTIONMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_IENCRYPTIONMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "../../_Include/IEncryptionMng.h"
#include "IBaseDefine.h"

//  int nID = 23;
// 	IEncryptionManager* pMng = (IEncryptionManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IEncryptionManager* pMng = (IEncryptionManager*)AfxGetPctrInterface(nID);

// 만든이  : 이준옥 2005.11.16(수)
// History : 2005.11.16 인터페이스
//

DECLARE_INTERFACE(IEncryptionManager)
{
	STDMETHOD_(void, Encryption)(int key, LPCSTR buff, LPSTR encdata, int len) PURE;
	STDMETHOD_(void, Decryption)(int key, LPCSTR buff, LPSTR encdata, int len) PURE;
	STDMETHOD_(void, EncryptCharacterz)(int nKey, char cCharacter, LPSTR szDestination) PURE;
	STDMETHOD_(void, DecryptCharacterz)(LPSTR szEncCharacter, LPSTR szDestination) PURE;
	STDMETHOD_(BOOL, EX_EncryptWord)(int nKey, LPSTR szSource, LPSTR szDestination) PURE;
	STDMETHOD_(BOOL, EX_DecryptWord)(LPSTR szSource, LPSTR szDestination) PURE;
};

#endif //AFX_HANARO_IENCRYPTIONMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

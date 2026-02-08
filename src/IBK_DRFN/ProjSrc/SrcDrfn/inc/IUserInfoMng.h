#if !defined(AFX_INTERFACE_OF_USERINFO_1464_45BB_ACC8_93B923698B1A__INCLUDED_)
#define AFX_INTERFACE_OF_USERINFO_1464_45BB_ACC8_93B923698B1A__INCLUDED_

// #include "../../_include/IUserInfoMng.h"
#include "IBaseDefine.h"

DECLARE_INTERFACE_(IUserInfoManager, IBaseDataString)
{
	// '사용자아이디' 리턴
	// I-PARAM : -
	// O-PARAM : 유저아이디
	STDMETHOD_(CString, GetUserID)() PURE;

	// '암호화된 사용자아이디' 리턴
	// I-PARAM : -
	// O-PARAM : 암호화된 유저아이디
	STDMETHOD_(CString, GetEncUserID)() PURE;
};

#define		USERINFOMNG_USERID			"A10000"
#define		USERINFOMNG_USERENCID		"A10001"

#endif //AFX_INTERFACE_OF_USERINFO_1464_45BB_ACC8_93B923698B1A__INCLUDED_

#if !defined(AFX_INTERFACE_OF_MULTISTMANAGER_1464_45BB_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_MULTISTMANAGER_1464_45BB_BY_JUNOK_LEE__INCLUDED_

// 용도 : 전략에 관련된 정보를 제공한다.

// #include "../../_include/ISTMng.h"
#include "IBaseDefine.h"

#define	UUID_ISTManager				41

DECLARE_INTERFACE(ISTManager)
{
	// 전략실행여부를 세팅한다.
	// I-PARAM : szSTPath 전략FullPath, bAdd:TRUE이면 +, FALSE이면 -
	// O-PARAM : 
	STDMETHOD_(int, AddSTRunINfo)(LPCSTR szSTPath, BOOL bAdd) PURE;

	/* src\Include_ST\IStrategyItem.h 참조.
	enum OPERATTIONSTATUS {
		STRATEGY_NONE = 0,
		STRATEGY_RUN,
		STRATEGY_PAUSE,
		STRATEGY_STOP,
		STRATEGY_STANDBY
	*/

	// 전략실행 정보를 알아온다.
	// I-PARAM : szSTPath 전략FullPath
	// O-PARAM : 실행되고 있는 카운트, 실행되고 있으면 0이상
	STDMETHOD_(int, GetSTRunINfo)(LPCSTR szSTPath) PURE;
};


#endif //AFX_INTERFACE_OF_MULTISTMANAGER_1464_45BB_BY_JUNOK_LEE__INCLUDED_

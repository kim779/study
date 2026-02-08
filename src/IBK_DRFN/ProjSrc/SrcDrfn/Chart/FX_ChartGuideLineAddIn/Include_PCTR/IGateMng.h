#if !defined(AFX_HANARO_IGATENANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_IGATENANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "../../_Include/IGateMng.h"
#include "IBaseDefine.h"

//  int nID = 0;
// 	IGateManager* pGateMng = (IGateManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IGateManager* pMng = (IGateManager*)AfxGetPctrInterface(nID);

// 만든이  : 이준옥 2005.11.14(월)
// History : 2005.11.14 인터페이스
//

DECLARE_INTERFACE(IGateManager)
{
	// Interface포인터 구하기
	// I-PARAM : nID : 인터페이스 고유ID
	//           dwUnUse : 사용하지 않음.
	// O-PARAM : 해당 Interface* 포인터
	STDMETHOD_(LONG, GetInterface)(int nID, long dwUnUse=0) PURE;
};

#endif //AFX_HANARO_INTERFACE_H__320E_1464_BY_JUNOK_LEE_98B1A__INCLUDED_

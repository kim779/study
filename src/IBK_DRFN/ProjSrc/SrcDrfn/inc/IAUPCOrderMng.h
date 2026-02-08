#if !defined(AFX_INTERFACE_OF_ORDER_BY_PC_MANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_ORDER_BY_PC_MANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/IAUPCOrderMng.h"
#include "IBaseDefine.h"

//  int nID = 25;
// 	IAUPCOrderManager* pMng = (IAUPCOrderManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUPCOrderManager* pMng = (IAUPCOrderManager*)AfxGetPctrInterface(nID);

// 개념 : PC주문(예약주문/자동주문) 매니저
//
// 만든이  : 믿음을 주는 사람 이준옥 2005.12.03(토)
// 믿음의 말 : 꿈은 항상 내 앞에 있다. 나는 내 꿈을 이루기 위해 오늘도 계획을 세워 행동한다.
// History : 2005.12.03 인터페이스 정의
//

DECLARE_INTERFACE(IAUPCOrderManager)
{
	// 설   명 : 
	// I-PARAM : szKey 고유키
	// O-PARAM : TRUE/FALSE
	// STDMETHOD_(BOOL, IsBackGround)(LPCSTR szKey) PURE;
};


#endif // AFX_INTERFACE_OF_ORDER_BY_PC_MANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_

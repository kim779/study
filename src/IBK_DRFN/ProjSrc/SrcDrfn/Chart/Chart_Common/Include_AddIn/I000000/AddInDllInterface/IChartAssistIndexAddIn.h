// IChartAssistIndexAddIn.h: interface for the IChartAssistIndexAddIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTASSISTINDEXADDIN_H__0ADE69EC_94E7_430E_ADCD_7128CBDF784C__INCLUDED_)
#define AFX_ICHARTASSISTINDEXADDIN_H__0ADE69EC_94E7_430E_ADCD_7128CBDF784C__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의! Chart OCX가 이용하는 이 DLL만의 Special Interface이다.
//		따라서 수정시 Chart OCX에게 Header를 전달하여야하며,
//		 특히 AddIn DLL마다 이 Interface Header를 같이 이용할 수 없다.
//			(다른 이름으로 재정의할 것)
//		Chart OCX에게 별도의 Interface를 제공할 필요가 없는 경우,
//		 이 Header를 삭제하며, CSiteCustomAddin::GetAddInCustomInterface()를 정의하지 않아
//		 CSiteCustomAddin::GetAddInCustomInterface()가 재정의되지 않도록 한다.
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IPacket050531.h"

interface IChartAssistIndexAddIn : public IUnknown
{
protected:
	virtual ~IChartAssistIndexAddIn()	{}

public:
	// 지표를 계산한다.(지표추가, 지표변경)
	virtual BOOL CalculateIndicator(const char* pcszIndicatorName, CList<IPacket050531*, IPacket050531*>* listIPacket) = 0;
	// 지표를 삭제한다.
	virtual BOOL DeleteIndicator(const char* pcszIndicatorName) = 0;
	// 현재추가되어 있는 모든 지표를 다시 계산한다.(TR 수신할때 / Real 수신할때)
	virtual BOOL CalculateAll(const int nDataOption) = 0;
};

#endif // !defined(AFX_ICHARTASSISTINDEXADDIN_H__0ADE69EC_94E7_430E_ADCD_7128CBDF784C__INCLUDED_)

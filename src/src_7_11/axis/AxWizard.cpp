// AxWizard.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axis.h"
#include "AxWizard.h"
#include "../h/axisvar.h"


#define PLATFORMNAME _T("AxisWizard.WizardCtrl.AX2010")

// CAxWizard

IMPLEMENT_DYNAMIC(CAxWizard, CWnd)

CAxWizard::CAxWizard()
{

}

CAxWizard::~CAxWizard()
{
}


BEGIN_MESSAGE_MAP(CAxWizard, CWnd)
END_MESSAGE_MAP()



// CAxWizard 메시지 처리기입니다.
BOOL CAxWizard::CreateWizard(CWnd* pParent)
{
	ASSERT(this); 
	return CreateControl(PLATFORMNAME, NULL, WS_CHILD, CRect(0, 0, 0, 0), pParent, -1);
}

BOOL CAxWizard::LoginAXIS(CString ip, int port)
{
	ASSERT(this);
	BOOL rc = FALSE;
	
	InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
	 //   (BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXIS, ip == "" ? 0 : (LONG_PTR)ip.GetString(), port);
		(BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, ip == "" ? 0 : (LONG_PTR)ip.GetString(), port);

	return rc;
}

BOOL CAxWizard::LogOff()
{
	ASSERT(this);
	return LoginAXIS("");
}


BOOL CAxWizard::set_optSOCK(BOOL bflag)
{
	// set socket parsing mode, variant : true / false
	ASSERT(this);
	BOOL rc = FALSE;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(optSOCK, 0), bflag);

	return rc;
}

void CAxWizard::DetachForm(int key)
{
	ASSERT(this);
	InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY,
		(void *)NULL, (BYTE *)(VTS_I4), (short)key);
}

void CAxWizard::setGroupTrigger(int key, int nGroup)
{
	ASSERT(this);
	LONG_PTR value = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&value, (BYTE *)(VTS_I4 VTS_I4),
		    setGID, (LONG_PTR)MAKELONG((WORD)key, nGroup));

	// key : LOWORD(variant), ## : HIWORD(variant)
}

BOOL CAxWizard::sendDeviceEvent(int key)
{
	ASSERT(this);
	BOOL rc = FALSE;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(devEV, key), 0);

	return rc;
}

CString CAxWizard::encryptPASS(CString sPass, int kind)
{
	// HIWORD(kind) = 0 : account, 1 : userID
	LONG_PTR rc = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(encPASS, kind), (LONG_PTR)sPass.GetString());

	return CString((TCHAR*)rc);
}



BOOL CAxWizard::applyAccount()
{
	BOOL rc = FALSE;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_BOOL, (void *)&rc, (BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(applyACC, 0), 0);
	
	return rc;
}


BOOL CAxWizard::setSignUser(char* wb, byte nBytes)
{
	BOOL rc = FALSE;
	InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4 VTS_I4), signUSER, (LONG_PTR)wb, nBytes);

	return rc;
}

void CAxWizard::setScreenNumber(int key, CString mapN)
{
	LONG_PTR rc = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(setSCN, key), (LPARAM)mapN.operator LPCTSTR());

}


int CAxWizard::getFormInfo(CString mapname, int& size)
{
	int vtype;
	InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&vtype,
		(BYTE *)(VTS_BSTR VTS_I4), mapname.Left(L_MAPN), &size);

	return vtype;
}

int CAxWizard::AttachForm(CView *pView, int type, int key)
{
	InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
		(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (LONG_PTR)pView, type, key);

	return key;
}

BOOL CAxWizard::loadMap(CString mapN, int key, int size, bool bflag)
{
	BOOL rc = FALSE;
	InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, bflag);


	return rc;
}


int CAxWizard::setFontSize(int key, int fontSize)
{
	LONG_PTR rcc = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rcc,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), (LONG_PTR)MAKELONG(0, fontSize));

	return rcc;
}


void CAxWizard::setTrigger(int key, CString symbol)
{
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)symbol.operator LPCTSTR());
}


CString CAxWizard::getTrigger(int key, CString symbol)
{
	LONG_PTR	rc = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(getFDC, key), (LONG_PTR)symbol.operator LPCTSTR());

	return rc ? (char *)rc : _T("");
}


void CAxWizard::setTrace(HWND hwnd)
{
	LONG_PTR rc = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(setTRACE, WM_USER), (LONG_PTR)hwnd);

}

CString CAxWizard::loadAcc()
{
	LONG_PTR	rc = 0;
	CString	data = _T("");

	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), loadACC, 0);

	data = (rc ? (char *)rc : _T(""));

	return data;
}

void CAxWizard::saveAcc(CString sData)
{
	LONG_PTR	rc = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(saveACC, 0), (LONG_PTR)sData.operator LPCTSTR());

}


BOOL CAxWizard::setUDP(CString ip, int port)
{
	BOOL rc = 0;
	InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4 VTS_I4), listenUDP, (LONG_PTR)ip.GetString(), port);

	return rc;
}

BOOL CAxWizard::setUDD(CString ports)
{
	BOOL rc = 0;
	InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4 VTS_I4), listenCAST, (LONG_PTR)ports.operator LPCTSTR(), 0);

	return rc;
}

BOOL CAxWizard::cleanUDP()
{
	BOOL	rc;
	InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4 VTS_I4), listenUDP, 0, -1);

	return rc;
}

BOOL CAxWizard::cleanUDD()
{
	BOOL	rc;
	InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4 VTS_I4), listenCAST, 0, -1);

	return rc;
}

void CAxWizard::detachForm(int key)
{
	InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY,
		(void *)NULL, (BYTE *)(VTS_I4), (short)key);

}

void CAxWizard::changeSkin()
{
	LONG_PTR	rc = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4,
		(void *)&rc, (BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(setPAL, 0), 0);
}


void CAxWizard::setvirtualScreen(int index)
{
	LONG_PTR	rc = 0;

	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setVID, 0), (LONG_PTR)MAKELONG(0, index));

}


bool CAxWizard::sendTR(char* datB, int datL)
{
	BOOL	rc = FALSE;
	InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), (LONG_PTR)datB, datL);
	
	return (rc ? true : false);
}


bool CAxWizard::sendTR(CString trN, char* datB, int datL, BYTE stat, int key)
{
	if (datL <= 0)	return false;
	char*	wb;
	BOOL	rc = FALSE;
	struct	_userTH* userTH;

	wb = new char[L_userTH + datL + 1];
	ZeroMemory(wb, L_userTH + datL + 1);
	userTH = (struct _userTH *)wb;
	CopyMemory(userTH->trc, (char *)trN.operator LPCTSTR(), trN.GetLength());

	userTH->stat = stat;	userTH->key = key;
	CopyMemory(&wb[L_userTH], datB, datL);
	InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), (LONG_PTR)wb, datL);
	delete[] wb;
	return (rc ? true : false);
}


void CAxWizard::enableBlink(int value)
{
	LONG_PTR rc = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFCB, 0), (LONG_PTR)value);
}

void CAxWizard::enableHogaFilter(CString sHogaFilter)
{
	LONG_PTR	rc = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(filterRTM, 0), (LONG_PTR)sHogaFilter.operator LPCTSTR());

}


void CAxWizard::setInteval(int time)
{
	LONG_PTR rc = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setINTERVAL, 0), (LONG_PTR)time);
	//** 이때 value = 0 이면 호가 필터링을 하지 않는다.
}


void CAxWizard::setOption(int key, CString sText)
{
	LONG_PTR rc = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE*)(VTS_I4 VTS_I4), MAKELONG(setGOPTION, key), (LONG_PTR)(char *)sText.operator LPCTSTR());

}

void CAxWizard::ReloadMaster()
{
	LONG_PTR rc = 0;
	InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
		(BYTE*)(VTS_I4 VTS_I4), MAKELONG(reloadMASTER, 0),0);
}
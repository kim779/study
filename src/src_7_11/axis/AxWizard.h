#pragma once
// CAxWizard

#include "../h//axisfire.h"

class CAxWizard : public CWnd
{
	DECLARE_DYNAMIC(CAxWizard)

public:
	CAxWizard();
	virtual ~CAxWizard();

	BOOL	CreateWizard(CWnd* pParent);
	BOOL	LoginAXIS(CString ip, int port = -1);
	BOOL	LogOff();

	BOOL	set_optSOCK(BOOL bflag = TRUE);

	void	DetachForm(int key);
	void	setGroupTrigger(int key, int nGroup);

	BOOL	sendDeviceEvent(int key);
	CString encryptPASS(CString sPass, int kind = 0);
	BOOL	applyAccount();
	BOOL	setSignUser(char* wb, byte nByte);
	void	setScreenNumber(int key, CString mapN);
	int	getFormInfo(CString mapname, int& size);
	int	AttachForm(CView *pView, int type, int key = 0);
	BOOL    loadMap(CString mapN, int key, int size, bool bflag = false);
	int	setFontSize(int key, int fontSize);
	void	setTrigger(int key, CString symbol);
	void	setTrace(HWND hwnd);
	
	CString	loadAcc();
	void     saveAcc(CString sData);

	BOOL	setUDP(CString ip, int port);
	BOOL	setUDD(CString ports);
	BOOL	cleanUDP(); 
	BOOL	cleanUDD();
	void    changeSkin();
	void	detachForm(int key);
	void	setvirtualScreen(int index);
	CString getTrigger(int key, CString symbol);

	bool	sendTR(CString trN, char* datB, int datL, BYTE stat, int key = 0);
	bool	sendTR(char* datB, int datL);
	void	enableBlink(int value);
	void	enableHogaFilter(CString sHogaFilter);
	void	setInteval(int time);
	void	setOption(int key, CString sText);
	void	ReloadMaster();
protected:

	DECLARE_MESSAGE_MAP()
};



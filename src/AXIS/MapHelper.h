#ifndef __MAPHELPER_H__
#define __MAPHELPER_H__

class CMainFrme;
#include "useMaindefine.h"
class CMapHelper
{
private:
	CMainFrame* m_main;
#ifdef DF_USE_CPLUS17
	std::map<CString, int> m_counter{};
#else
	CMapStringToPtr m_counter;
#endif
	CArray	<CWnd*, CWnd*>	m_arInterWnd;
	std::map <CString, int> m_mapcnt;
public:
	CMapHelper(CMainFrame* main);
	~CMapHelper();
	CString GetCountData();
	void SetCountData(LPCSTR data, int dlen);
	void SendInterSignal();

protected:
	void CountMap(LPCSTR mapN);
	void FreeCountMap();
	BOOL GetMapInfo(const char* mapName, int& type, int& size);

public:
	//** create or copy children map
	int	CreateTabView(CString mapN, int trigger, WINDOWPLACEMENT wndpl, BOOL showChild = true, int vsN = -1);
	int	CreateTabView(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0, CPoint point = CPoint(-1, -1));

	int	ChangeTabView(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0, CPoint point = CPoint(-1, -1));

	int	ChangeSDITabView(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0, CPoint point = CPoint(-1, -1));

	int	CreateChild(CString mapN, int trigger, WINDOWPLACEMENT wndpl, BOOL showChild = true, int vsN = -1,CString strTag = "");
	int	CreateChild(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0, CPoint point = CPoint(-1, -1));

	int	CreateSDI(CString mapN, int group = 1, int key = 0, int position = 0, CPoint point = CPoint(-1, -1));
	int	CreateSDI(CString mapN, int group, WINDOWPLACEMENT wndpl, BOOL showChild = true, int vsN = -1);

	int	CreateSDI_Tabview(CString mapN, int group = 1, int key = 0, int position = 0, CPoint point = CPoint(-1, -1));
	int	CreateSDI_Tabview(CString mapN, int group, WINDOWPLACEMENT wndpl, BOOL showChild = true, int vsN = -1);

	int	CreateSDIbyMDI(CString mapN, int group, int fontN, CRect mdiRc, CRect sdiRc, int vsN = -1);
	int	CreateSDIbyMDI_Tabview(CString mapN, int group, int fontN, CRect mdiRc, CRect sdiRc, int vsN = -1);

	int	CreateMDIbySDI(CString mapN, int group, int fontN, CRect sdiRc, CRect mdiRc, int vsN = -1);
	int	CreateMDIbySDI_Tabview(CString mapN, int group, int fontN, CRect sdiRc, CRect mdiRc, int vsN = -1);

	int	CopyScreen(CString mapN, int group, int fontsize, CPoint sp);
	int	CopySScreen(CString mapN, int group, int fontsize, CPoint sp);

	int	CopyTabScreen(CString mapN, int group, int fontsize, CPoint sp);

	int	ChangeChild(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0);
	int	ChangeSDIChild(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0);
	int	ChangeChild(int index);

	int	CreateBkNotice(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0, CPoint point = CPoint(-1, -1));
	int	ChangeBkNotice(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0);

	int	CreatePopup(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0, int actkey = 0, CPoint point = CPoint(-1, -1), BOOL closex = false, BOOL NoCaption = false);
	int	ChangePopup(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0, int actkey = 0);

	int	CreateModal(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0, int actkey = 0, CPoint point = CPoint(-1, -1));


	//** close map
	void	DirectClose();
	BOOL	closeChild(int key);
	BOOL	closeAllWithoutThis();
	
	BOOL	closeX();
	BOOL	closeAll(BOOL all = TRUE);	
	BOOL	closeChild();
	
//	BOOL	closeChild(int key);
	BOOL	closePopup();
	BOOL	closePopup(int key);

	//** map info
	BOOL	IsValidMap(const char* mapName);
	BOOL	IsDLL(const char* mapName);
	BOOL	IsDRFN(const char* mapName);
	BOOL	CloseInterest();

public:
	BOOL minimizeAll(BOOL all = TRUE);
	void	TransformMDIChildToSingleWindow(int key, int vs);
	void	TransformSingleWindowToMDIChild(int key, int vs);
};


#endif // __MAPHELPER_H__
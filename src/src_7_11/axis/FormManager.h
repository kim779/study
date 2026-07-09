#pragma once
class CMainFrame;
class CFormManager
{
public:
	CFormManager();
	~CFormManager();

	void	Init(CWnd* pMain, void* pfunc);
	
	int	ChangeChild(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0);
	CView*	CreateBaseForm(CString mapN, int& key, bool bSdi, int vsN = -1);
	
	int	CreateChild(CString mapN, int trigger, WINDOWPLACEMENT wndpl, bool showChild = true, int vsN = -1);
	int	CreateChild(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0, CPoint point = CPoint(-1, -1));
	int	CreateSDIbyMDI(CString mapN, int group, int fontN, CRect mdiRc, CRect sdiRc, int vsN = -1);
	int	CreateMDIbySDI(CString mapN, int group, int fontN, CRect sdiRc, CRect mdiRc, int vsN = -1);
	int	CreatePopup(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0, int actkey = 0, CPoint point = CPoint(-1, -1), bool closex = false);
	int	ChangePopup(CString mapN = _T(""), int trigger = 1, int key = 0, int position = 0, int actkey = 0);
	bool	OnCloseForm(int key);
	bool	closeChild(int key);
	bool	closeChild(CString strmap);
	bool	closeChild();
	void	addScreenList(CString mapN, int key, int vsN);
	void	EnableChild(BOOL bflag = TRUE);
	void	Setfocus_Child(int key, bool restore = true);
	void	displayChild(int vsN, bool show = true, bool bSDI = false);
	void	change_VirtualScreen(int index);
	void	set_Trigger(int key, int triggerN, int vsN = -1);

	void	convertSDI(int key, int vsN);
	void	convertMDI(int key, int vsN);
	void	childAll(CString dat);
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

	bool	closeAll(bool all = true);
	bool	hideAll(bool bAll = true);
	void	hidemap(int mapkey);
	void	refreshMap(int key);

	long	doFormKey(WPARAM wParam, LPARAM lParam);
	long	DoPopupKey(WPARAM wParam, LPARAM lParam);
	bool	CheckSDIMap(int key);
	void	LinkOPEN(int dat, CString mapN);
	bool	minimizeAll(bool all);
	void	TransformMDIChildToSingleWindow(int key, int vs);
	int		CopyScreen(CString mapN, int group, int fontsize, bool bboldfont, CPoint sp);
	BOOL	GetMapInfo(const char* mapName, int& type, int& size);
	template <typename T> void setChild(T, CString mapname, int key, int vsN, int vtype, int size, CSize frameGap);
	template <typename T> void refreshMap(T, int key);
	template <typename T> long runHooked(T t, WPARAM wParam, LPARAM lParam);

private:
	CMainFrame* m_pFrame;

};



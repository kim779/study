#pragma once

class CiForm;
class CfmObject;
class CChildView;
class CformProp;

class AFX_EXT_CLASS CObjectLoad : public CObject
{
public:
	CObjectLoad();
	CObjectLoad(CformProp* fmProp, CiForm* iForm, CChildView* view);
	virtual ~CObjectLoad();
	CChildView*		m_view;
protected:
	HINSTANCE		m_hInstDll;

	CformProp*		m_fmProp;
	CiForm*			m_piForm;
	CfmObject*		m_fmObj;
public:
	bool LoadLib(char kind);
	CString GetLibName(char kind);
	void SetLastPoint(CPoint point);
	void SetHooking(bool bHook);
	bool GetHooking();
	CRect GetRect();
	void LoadProc();
	CfmObject* getObject();
	CWnd* getWnd();
	CString   GetProperties();
	CString   GetMethod();
	CString	  GetEvent();
	CformProp* getProp(){ return m_fmProp; };
	CString GetLibName() { return m_strLibName; };
public:
	virtual void	LoadProcEnd() {};
	virtual CWnd*	getButtonWnd() { return NULL; };

public:
	FARPROC	m_callproc;
public:
	CWnd* m_wndObj;
private:
	CString m_strLibName;
	bool	m_bHook;
};
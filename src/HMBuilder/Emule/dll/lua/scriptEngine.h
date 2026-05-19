#if !defined(AFX_SCRIPTENGINE_H__817F6AD4_EE82_4306_BCE7_1D37D2CE01CD__INCLUDED_)
#define AFX_SCRIPTENGINE_H__817F6AD4_EE82_4306_BCE7_1D37D2CE01CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// scriptEngine.h : header file
//

#include "StdAfx.h"
#include "scriptsite.h"

// Form
#define LUA_SEND		0
#define	LUA_GETFORMNAME		1
// System
#define LUA_PUSH		5
#define LUA_POP			6

#define LUA_VERIFY		20



/////////////////////////////////////////////////////////////////////////////
// CScriptEngine command target

class AFX_EXT_CLASS CScriptEngine : public COleDispatchDriver
{
public:
	CScriptEngine(CWnd* parent);           // protected constructor used by dynamic creation
	virtual ~CScriptEngine();

// Attributes
public:
	bool	m_bParent;
	bool	m_invoke;
	bool	m_bSend;
	int	m_clientKey;
	int	m_screenKey;
	int	m_subScreenKey;
	lua_State* m_pLua;
	lua_State* m_pLua2;	
	luabind::object m_pObj;

	CScriptEngine* m_pParent;

	CString m_pushData;
	CString m_formName;

	bool	m_bGetObj;

protected:
	CWnd*	m_parent;
	CScriptSite	m_ssite;
	IDispatch*	m_dispatch;
	IActiveScript*	m_as;
	IActiveScriptParse* m_asp;


// Operations
public:
	void	Initialize();
	void	AddObject(CString names, CCmdTarget* object, DWORD flag = 0);
	BOOL	Debug_Lua(int nType, int nerror);
	void	NotSupportMSG(CString strMethod);
	
	void	LuaSend(int target, const char* strTR);
	void	LuaSend2(int target, const char* strTR, int session);

	//bool	IsAvailable(CString procs);
	bool	DoProcedure(CString procs);
	CString ScriptVerify(CString procs);
	CMap<CString, LPCTSTR, class CScreen*, class CScreen*>	m_mapObj;
	CMapStringToOb	m_objects;

// Implementation
private:
	
public:
	void		LuaClose();
	void		LuaClear();
	void		changeForm(const char* pchTR);
	int		getAbsCoord(const char* pcName, int pos);
	void		setAnimation(int type, int fromDelta, int toDelta);
	void		commitAnimation(luabind::object ob, long duration, int effect, const char* pcMethod);
	void		triggerAndRun(int type, const char* name, const char* data, const char* procs, const char* args);
	void		messageBoxAndRun(int msgid, int type, const char* msg, const char* title, const char* okStr, const char* cancelStr);
	void		setRtsOn(bool on);
	int		getStartX();
	int		getStartY();
	int		getEndX();
	int		getEndY();
	bool		getVIsible();
	void		setVisible(bool visible);
	void		setLayout(const char* name);
	void		setDefaultLayout();

	// FORM
	LPCTSTR		getFormName(long target);
	CScriptEngine*	getMain();
	CScriptEngine*	LuaGetParent();
	luabind::object	LuaGetObject(const char* strName);	
	void		setOnSend(bool onsend);
	bool		getOnSend();


	// SYSTEM
	void		Push(LPCTSTR sKey, LPCTSTR data);
	LPCTSTR		Pop(LPCTSTR sKey);
	void		LuaMessageBox(LPCTSTR msg, LPCTSTR title);
	void		toast(LPCTSTR msg);
	void		createWindow(LPCTSTR fmName, long kind, long pos, long type, long x, long y, long width, long height);
	void		Trigger(LPCTSTR fmName, LPCTSTR procs, LPCTSTR arg);
	int		getMetrics(int index);
	LPCTSTR		getAccount(LPCTSTR name);
	void		setValue(LPCTSTR fileName, LPCTSTR groupName, LPCTSTR key, LPCTSTR value);
	LPCTSTR		getValue(LPCTSTR fileName, LPCTSTR groupName, LPCTSTR key);
	void		deleteValue(LPCTSTR fileName, LPCTSTR groupName, LPCTSTR key);
	void		deleteGroup(LPCTSTR fileName, LPCTSTR groupName);
	void		deleteFile(LPCTSTR fileName);
	void		deleteHistory(int type, LPCTSTR code);			
	void		clearHistory(int type);
	LPCTSTR		getHistory(int type, bool last);
	void		addHistory(LPCTSTR code);
	void		addHistoryWithType(int type, LPCTSTR code, int index);

	int		getCodeType(LPCTSTR code);
	LPCTSTR		getCodeName(LPCTSTR code);
	void		goPreviousForm();

	void		saveCloudString(LPCTSTR key, LPCTSTR data);
	void		saveCloudData(LPCTSTR key, LPCTSTR path, LPCTSTR data);	
	void		saveCloudFloat(LPCTSTR key, float fData);
	void		saveCloudBool(LPCTSTR key, bool bData);
	void		saveCloudInt(LPCTSTR key, int nData);
	LPCTSTR		loadCloudString(LPCTSTR key, LPCTSTR defstr);
	LPCTSTR		loadCloudData(LPCTSTR key);
	float		loadCloudFloat(LPCTSTR key, float defval);
	bool		loadCloudBool(LPCTSTR key, bool defval);
	int		loadCloudInt(LPCTSTR key, int defval);
	void		setOrientation(int orientation);

	//class CAmButton*	LuaGetObject(const char* strName);
	
	//luabind::object	setParentObj(CString strName);
	void		setParentObj(lua_State* L);
	//void LuaGetParent();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTENGINE_H__817F6AD4_EE82_4306_BCE7_1D37D2CE01CD__INCLUDED_)

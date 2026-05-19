// scriptEngine.cpp : implementation file
//

#include "stdafx.h"
#include "scriptEngine.h"

#include "../form/amform.h"

#include "../form/amBrowser.h"
#include "../form/amButton.h"
#include "../form/amRadio.h"
#include "../form/amEdit.h"
#include "../form/amOut.h"
#include "../form/amLabel.h"
#include "../form/amCheck.h"
#include "../form/amPanel.h"
#include "../form/amBox.h"
#include "../form/amCombo.h"
#include "../form/amUserTab.h"
#include "../form/amTable.h"
#include "../form/amGrid.h"
#include "../form/amObject.h"

#include "login.h"
#include "ledgerx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace luabind;

#define LUA_LOADFILE		0
#define LUA_PCALL		1

#define WM_LUASCRIPT		WM_USER+3
#define WM_LUAOBJECT		WM_USER+4
#define WM_LUAGETNAME		WM_USER+5
#define WM_LUAGETMAIN		WM_USER+6


/////////////////////////////////////////////////////////////////////////////
// CScriptEngine

CScriptEngine::CScriptEngine(CWnd* parent)
{
	m_parent   = parent;

	m_pParent  = NULL;	// Lua GetParent()
	m_as       = NULL;
	m_asp      = NULL;
	m_dispatch = NULL;
	m_invoke   = false;
	m_bParent  = false;
	m_bGetObj  = false;
	m_bSend	   = true;
	m_subScreenKey = 0;

	m_mapObj.RemoveAll();
}

CScriptEngine::~CScriptEngine()
{
	m_mapObj.RemoveAll();
	//lua_close(m_pLua);
}

/////////////////////////////////////////////////////////////////////////////
// CScriptEngine message handlers

///////////////////////////////////////////////////////////////////////////////////////////////
/*	nType : 0 - Loadfile, 1 - pcall		*/
BOOL CScriptEngine::Debug_Lua(int nType, int nerror)
{
	CString strError = _T("");
	if (nType == LUA_LOADFILE)
	{
		switch (nerror)
		{
		case LUA_ERRSYNTAX:
			TRACE("[%d] Syntax : %s\n", nerror, lua_tostring(m_pLua, -1)); // ERROR STRING
			strError.Format("%s", lua_tostring(m_pLua, -1));
			m_parent->SendMessage(WM_LUASCRIPT, MAKEWPARAM(m_clientKey, LUA_VERIFY), (LPARAM)(LPCTSTR)strError);
			lua_pop(m_pLua, 1);
			return FALSE;
		case LUA_ERRMEM:
			TRACE("[%d] Memory : %s\n", nerror, lua_tostring(m_pLua, -1)); // ERROR STRING
			strError.Format("%s", lua_tostring(m_pLua, -1));
			m_parent->SendMessage(WM_LUASCRIPT, MAKEWPARAM(m_clientKey, LUA_VERIFY), (LPARAM)(LPCTSTR)strError);
			lua_pop(m_pLua, 1);
			return FALSE;
		}
	}
	else if (nType == LUA_PCALL)
	{
		switch (nerror)
		{
		case LUA_ERRRUN:
			TRACE("[%d] Runtime : %s\n",nerror, lua_tostring(m_pLua, -1)); // ERROR STRING
			strError.Format("%s", lua_tostring(m_pLua, -1));
			m_parent->SendMessage(WM_LUASCRIPT, MAKEWPARAM(m_clientKey, LUA_VERIFY), (LPARAM)(LPCTSTR)strError);
			lua_pop(m_pLua, 1);
			return FALSE;
		case LUA_ERRMEM:
			TRACE("[%d] Runtime Memory : %s\n",nerror, lua_tostring(m_pLua, -1)); // ERROR STRING
			strError.Format("%s", lua_tostring(m_pLua, -1));
			m_parent->SendMessage(WM_LUASCRIPT, MAKEWPARAM(m_clientKey, LUA_VERIFY), (LPARAM)(LPCTSTR)strError);
			lua_pop(m_pLua, 1);
			return FALSE;
		case LUA_ERRERR:
			TRACE("[%d] Exception : %s\n",nerror, lua_tostring(m_pLua, -1)); // ERROR STRING
			strError.Format("%s", lua_tostring(m_pLua, -1));
			m_parent->SendMessage(WM_LUASCRIPT, MAKEWPARAM(m_clientKey, LUA_VERIFY), (LPARAM)(LPCTSTR)strError);
			lua_pop(m_pLua, 1);
			return FALSE;
		}
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////


void CScriptEngine::Initialize()
{
	// Lua open
	m_pLua = lua_open();


	luaL_openlibs(m_pLua);
	luabind::open(m_pLua);

	//int dd = luaL_loadfile(m_pLua, "init.lua");
	//Debug_Lua(LUA_LOADFILE, luaL_dofile(m_pLua, "init.lua"));
 	
	module(m_pLua)
		[
			//class_<class CGuard>("CGuard")
			//	.def(constructor<CWnd*>())
			//	,
			//class_<CScreen>("CScreen")
			//	.def(constructor<class CClient*, CRect>())
			//,

			// Lua Send
			class_<CScriptEngine>("CScriptEngine")
				.def(constructor<CWnd*>())
				.def("close", &CScriptEngine::LuaClose)
				.def("clear", &CScriptEngine::LuaClear)
				.def("changeForm", &CScriptEngine::changeForm)				
				.def("send", &CScriptEngine::LuaSend)
				.def("send2", &CScriptEngine::LuaSend2)
				.def("getFormName", &CScriptEngine::getFormName)				
				.def("getParent", &CScriptEngine::LuaGetParent)
				.def("getMain", &CScriptEngine::getMain)
				.def("getObject", &CScriptEngine::LuaGetObject)
				.def("setOnSend", &CScriptEngine::setOnSend)
				.def("getOnSend", &CScriptEngine::getOnSend)
				// FORM -- Emul Doesn't Support
				.def("getAbsCoord", &CScriptEngine::getAbsCoord)
				.def("setAnimation", &CScriptEngine::setAnimation)
				.def("commitAnimation", &CScriptEngine::commitAnimation)
				.def("triggerAndRun", &CScriptEngine::triggerAndRun)
				.def("messageBoxAndRun", &CScriptEngine::messageBoxAndRun)
				.def("setRtsOn", &CScriptEngine::setRtsOn)
				.def("setLayout", &CScriptEngine::setLayout)
				.def("setDefaultLayout", &CScriptEngine::setDefaultLayout)
				.property("meStartX", &CScriptEngine::getStartX)
				.property("meStartY", &CScriptEngine::getStartY)
				.property("meEndX", &CScriptEngine::getEndX)
				.property("meEndY", &CScriptEngine::getEndY)
				.property("visible", &CScriptEngine::getVIsible, &CScriptEngine::setVisible)
				// SYSTEM
				.def("push", &CScriptEngine::Push)
				.def("pop", &CScriptEngine::Pop)
				.def("messageBox", &CScriptEngine::LuaMessageBox)
				.def("toast", &CScriptEngine::toast)
				// SYSTEM -- Emul Doesn't Support
				.def("createWindow", &CScriptEngine::createWindow)
				.def("trigger", &CScriptEngine::Trigger)
				.def("getMetrics", &CScriptEngine::getMetrics)
				.def("getAccount", &CScriptEngine::getAccount)
				.def("setValue", &CScriptEngine::setValue)
				.def("getValue", &CScriptEngine::getValue)
				.def("deleteValue", &CScriptEngine::deleteValue)
				.def("deleteGroup", &CScriptEngine::deleteGroup)
				.def("deleteFile", &CScriptEngine::deleteFile)
				.def("deleteHistory", &CScriptEngine::deleteHistory)
				.def("clearHistory", &CScriptEngine::clearHistory)
				.def("getHistory", &CScriptEngine::getHistory)
				.def("addHistory", &CScriptEngine::addHistory)
				.def("addHistoryWithType", &CScriptEngine::addHistoryWithType)
				.def("getCodeType", &CScriptEngine::getCodeType)
				.def("getCodeName", &CScriptEngine::getCodeName)
				.def("goPreviousForm", &CScriptEngine::goPreviousForm)
				.def("saveCloudString", &CScriptEngine::saveCloudString)
				.def("saveCloudData", &CScriptEngine::saveCloudData)
				.def("saveCloudFloat", &CScriptEngine::saveCloudFloat)
				.def("saveCloudBool", &CScriptEngine::saveCloudBool)
				.def("saveCloudInt", &CScriptEngine::saveCloudInt)
				.def("loadCloudString", &CScriptEngine::loadCloudString)
				.def("loadCloudData", &CScriptEngine::loadCloudData)
				.def("loadCloudFloat", &CScriptEngine::loadCloudFloat)
				.def("loadCloudBool", &CScriptEngine::loadCloudBool)
				.def("loadCloudInt", &CScriptEngine::loadCloudInt)
				.def("setOrientation", &CScriptEngine::setOrientation)
				,
			class_<CLogin>("CLogin")
				.def(constructor<>())
				.def("getUser", &CLogin::GetUsid)
				.def("getName", &CLogin::GetUserName)
				.def("getTerm", &CLogin::GetTerm)
				.property("commKind", &CLogin::GetCommKind)
				.property("media", &CLogin::GetMedia)
				.property("dept", &CLogin::GetDept)
				.property("deptName", &CLogin::GetDeptName)
				.property("svrNo", &CLogin::GetSvrNo)
				.property("method", &CLogin::GetMethod)
				.property("idNo", &CLogin::GetIdNo)
				.property("grade", &CLogin::GetGrade)
				.property("date", &CLogin::GetDate)
				.property("today", &CLogin::GetToday)
				.property("lmedia", &CLogin::GetLmedia)
				.property("ldate", &CLogin::GetLdate)
				.property("ltime", &CLogin::GetLtime)
				.property("check", &CLogin::GetCheck)
				.property("popup", &CLogin::GetPopup)
				,
			class_<CLedger>("CLLedger")
				.def(constructor<>())
				.property("time", &CLedger::GetTime, &CLedger::SetTime)
				.property("flag", &CLedger::GetFlag, &CLedger::SetFlag)
				.property("svcn", &CLedger::GetSvcn, &CLedger::SetSvcn)
				.property("svid", &CLedger::GetSvid, &CLedger::SetSvid)
				.property("pgid", &CLedger::GetPgid, &CLedger::SetPgid)
				.property("usid", &CLedger::GetUsid)
				.property("dept", &CLedger::GetDept)
				.property("term", &CLedger::GetTerm)
				.property("usnm", &CLedger::GetUsnm)
				.property("trcd", &CLedger::GetTrcd, &CLedger::SetTrcd)
				.property("scrn", &CLedger::GetScrn, &CLedger::SetScrn)
				.property("kend", &CLedger::GetKEnd, &CLedger::SetKEnd)
				.property("fxcp", &CLedger::GetFxcp, &CLedger::SetFxcp)
				.property("ikey", &CLedger::GetIkey, &CLedger::SetIkey)
				.property("eofd", &CLedger::GetEofd, &CLedger::SetEofd)
				.property("reqn", &CLedger::GetReqn, &CLedger::SetReqn)
				.property("ecod", &CLedger::GetECod, &CLedger::SetECod)
				.property("emsg", &CLedger::GetEMsg, &CLedger::SetEMsg)
				.property("accn", &CLedger::GetAccn, &CLedger::SetAccn)
				,
			// Amform Define			
			class_<CamBase>("CamBase")
				.def(constructor<CAmForm*, struct _formR*>())
				,
			class_<CamLabel>("CamLabel")
				.def(constructor<CAmForm*, struct _formR*>())
				.property("text", &CamLabel::GetText, &CamLabel::SetText)
				.property("backgroundColor", &CamLabel::GetPRgb, &CamLabel::SetPRgb)
				.property("textColor", &CamLabel::GetTRgb, &CamLabel::SetTRgb)
				.property("visible", &CamLabel::GetVisible, &CamLabel::SetVisible)
				.def("refresh", &CamLabel::_Refresh)
				,
			class_<CamBox>("CamBox")
				.def(constructor<CAmForm*, struct _formR*>())
				.property("backgroundColor", &CamBox::GetPRgb, &CamBox::SetPRgb)
				.property("visible", &CamBox::GetVisible, &CamBox::SetVisible)
				.def("Refresh", &CamBox::_Refresh)
				,
			class_<CamPanel>("CamPanel")
				.def(constructor<CAmForm*, struct _formR*>())
				//.property("text", &CamPanel::GetText, &CamPanel::SetText)
				.property("backgroundColor", &CamPanel::GetPRgb, &CamPanel::SetPRgb)
				//.property("trgb", &CamPanel::GetTRgb, &CamPanel::SetTRgb)
				.property("visible", &CamPanel::GetVisible, &CamPanel::SetVisible)
				.def("Refresh", &CamPanel::_Refresh)
				,
			class_<CAmButton>("CAmButton")
				.def(constructor<CAmForm*, struct _formR*>())
				.property("enable", &CAmButton::GetEnable, &CAmButton::SetEnable)
				.property("visible", &CAmButton::GetVisible, &CAmButton::SetVisible)
				.property("text", &CAmButton::GetText, &CAmButton::SetText)
				.property("backgroundColor", &CAmButton::GetPRgb, &CAmButton::SetPRgb)
				.property("textColor", &CAmButton::GetTRgb, &CAmButton::SetTRgb)
				.def("refresh", &CAmButton::Refresh)
				// luabind overriding
				//.def("visible", (void(CAmButton::*)(BOOL))&CAmButton::SetVisible)
				,
			class_<CamRadio>("CamRadio")
				.def(constructor<CAmForm*, struct _formR*>())
				.property("enable", &CamRadio::GetEnable, &CamRadio::SetEnable)
				.property("visible", &CamRadio::GetVisible, &CamRadio::SetVisible)
				.property("text", &CamRadio::GetText, &CamRadio::SetText)				
				.property("backgroundColor", &CamRadio::GetPRgb, &CamRadio::SetPRgb)
				.property("textColor", &CamRadio::GetTRgb, &CamRadio::SetTRgb)
				.property("checked", &CamRadio::_GetChecked, &CamRadio::_SetChecked)
				.def("refresh", &CamRadio::Refresh)
				.def("getGroupName", &CamRadio::_GetGruopName)
				,
			class_<CamCheck>("CamCheck")
				.def(constructor<CAmForm*, struct _formR*>())
				.property("enable", &CamCheck::GetEnable, &CamCheck::SetEnable)
				.property("visible", &CamCheck::GetVisible, &CamCheck::SetVisible)
				.property("text", &CamCheck::GetText, &CamCheck::SetText)				
				.property("backgroundColor", &CamCheck::GetPRgb, &CamCheck::SetPRgb)
				.property("textColor", &CamCheck::GetTRgb, &CamCheck::SetTRgb)
				.property("checked", &CamCheck::_GetChecked ,&CamCheck::_SetChecked)
				.def("refresh", &CamCheck::Refresh)
				,
			class_<CamEdit>("CamEdit")
				.def(constructor<CAmForm*, struct _formR*, bool>())
				.property("enable", &CamEdit::GetEnable, &CamEdit::SetEnable)
				.property("visible", &CamEdit::GetVisible, &CamEdit::SetVisible)
				.property("data", &CamEdit::GetData, &CamEdit::SetData)
				.property("text", &CamEdit::GetText)//, &CamEdit::SetText)				
				.property("backgroundColor", &CamEdit::GetPRgb, &CamEdit::SetPRgb)
				.property("textColor", &CamEdit::GetTRgb, &CamEdit::SetTRgb)					
				.def("clear", &CamEdit::_Clear)
				.def("blink", &CamEdit::_Blink)
				.def("refresh", &CamEdit::_Refresh)
				,
			class_<CamOut>("CamOut")
				.def(constructor<CAmForm*, struct _formR*>())
				.property("enable", &CamOut::GetEnable, &CamOut::SetEnable)
				.property("visible", &CamOut::GetVisible, &CamOut::SetVisible)
				.property("data", &CamOut::GetData, &CamOut::SetData)
				.property("text", &CamOut::GetText)//, &CamOut::SetText)				
				.property("backgroundColor", &CamOut::GetPRgb, &CamOut::SetPRgb)
				.property("textColor", &CamOut::GetTRgb, &CamOut::SetTRgb)
				.def("clear", &CamOut::_Clear)	
				.def("blink", &CamOut::_Blink)
				.def("refresh", &CamOut::_Refresh)
				,
			class_<CamCombo>("CamCombo")
				.def(constructor<CAmForm*, struct _formR*>())				
				.property("enable", &CamCombo::GetEnable, &CamCombo::SetEnable)
				.property("visible", &CamCombo::GetVisible, &CamCombo::SetVisible)
				.property("data", &CamCombo::GetData, &CamCombo::SetData)
				.property("text", &CamCombo::GetText)				
				.property("backgroundColor", &CamCombo::GetPRgb, &CamCombo::SetPRgb)
				.property("textColor", &CamCombo::GetTRgb, &CamCombo::SetTRgb)
				.property("index", &CamCombo::GetIndex, &CamCombo::SetIndex)
				.def("refresh", &CamCombo::_Refresh)
				,
			class_<CamGrid>("CamGrid")
				.def(constructor<CAmForm*, struct _formR*>())
				.property("visible", &CamGrid::GetVisible, &CamGrid::SetVisible)
				.property("data", &CamGrid::GetData, &CamGrid::SetData)
				.property("text", &CamGrid::GetText)
				// bkColor
				.property("backgroundColor", &CamGrid::GetBG, &CamGrid::SetBG)
				// textColor
				.property("textColor", &CamGrid::GetFG, &CamGrid::SetFG)
				.property("row", &CamGrid::GetRow, &CamGrid::SetRow)
				.property("head", &CamGrid::GetHead, &CamGrid::SetHead)
				.property("hitPos", &CamGrid::GetHitPos)
				.property("column", &CamGrid::GetColumn, &CamGrid::SetColumn)
				.property("rows", &CamGrid::GetRows)
				.property("columns", &CamGrid::GetColumns)
				.property("action", &CamGrid::GetAction)
				.property("realtm", &CamGrid::GetRealTm)
				.property("headerDirection", &CamGrid::GetHeaderDirection)
				.property("headerSortDirection", &CamGrid::GetHeaderSortDirection)
				.property("headerSortColumn", &CamGrid::GetHeaderSortColumn)
				.property("headerSave", &CamGrid::GetHeaderSave)
				.property("headerPage", &CamGrid::GetHeaderPage)
				.property("scrollX", &CamGrid::GetScrollX, &CamGrid::SetSctollX)
				.property("scrollY", &CamGrid::GetScrollY, &CamGrid::SetScrollY)
				//.property("index", &CamGrid::GetIndex, &CamGrid::SetIndex)				
				//.property("pos", &CamGrid::GetPos, &CamGrid::SetPos)
				//.property("page", &CamGrid::GetPage, &CamGrid::SetPage)
				//.property("direction", &CamGrid::GetDirection, &CamGrid::SetDirection)
				//.property("sortcolumn", &CamGrid::GetSortColumn, &CamGrid::SetSortColumn)				
				//.property("info", &CamGrid::GetInfo, &CamGrid::SetInfo)
				.def("clear", &CamGrid::_Clear)
				.def("refresh", &CamGrid::_Refresh)
				.def("getHead", &CamGrid::_GetHead)
				.def("setHead", &CamGrid::_SetHead)
				.def("getBackColor", &CamGrid::_GetPRgb)
				.def("setBackColor", &CamGrid::_SetPRgb)
				.def("getGridData", &CamGrid::_GetCell)
				.def("setGridData", &CamGrid::_SetCell)
				.def("isColumnVisible", &CamGrid::_IsColumnVisible)
				.def("setColumnVisible", &CamGrid::_SetColumnVisible)
				.def("isCellEnable", &CamGrid::_IsCellEnable)
				.def("setCellEnable", &CamGrid::_SetCellEnable)
				.def("getColumnName", &CamGrid::_GetColumnName)
				.def("insertRow", &CamGrid::_InsertRow)
				.def("removeRow", &CamGrid::_RemoveRow)
				.def("appendView", &CamGrid::_appendView)
				.def("setDataPosition", &CamGrid::_SetDataPosition)
				.def("setOrderInfo", &CamGrid::_setOrderInfo)
				.def("getOrderInfo", &CamGrid::_getOrderInfo)
				.def("scrollToRow", &CamGrid::_scrollToRow)
				.def("isCellChecked", &CamGrid::_isCellChecked)
				.def("setCellChecked", &CamGrid::_setCellChecked)
				,
			class_<CamTable>("CamTable")
				.def(constructor<CAmForm*, struct _formR*>())
				.property("enable", &CamTable::GetEnable, &CamTable::SetEnable)
				.property("visible", &CamTable::GetVisible, &CamTable::SetVisible)
				.property("data", &CamTable::GetData, &CamTable::SetData)
				.property("text", &CamTable::GetText)
				//.property("backgroundColor", &CamTable::GetPRgb, &CamTable::SetPRgb)
				//.property("textColor", &CamTable::GetTRgb, &CamTable::SetTRgb)
				.property("row", &CamTable::GetRow, &CamTable::SetRow)
				.property("head", &CamTable::GetHead, &CamTable::SetHead)
				.property("hitPos", &CamTable::GetHitPos)
				.property("rows", &CamTable::GetSize)
				//.property("cell", &CamTable::GetCell, &CamTable::SetCell)
				.def("clear", &CamTable::_Clear)
				.def("refresh", &CamTable::_Refresh)
				.def("getText", &CamTable::_GetText)
				.def("getData", &CamTable::_GetCell)
				.def("setData", &CamTable::_SetCell)
				.def("getHead", &CamTable::_GetHead)		
				.def("setHead", &CamTable::_SetHead)
				.def("getBackColor", &CamTable::_GetPRgb)		
				.def("setBackColor", &CamTable::_SetPRgb)
				.def("getTextColor", &CamTable::_GetTRgb)
				.def("setTextColor", &CamTable::_SetTRgb)
				,
			class_<CamObject>("CamObject")
				.def(constructor<CAmForm*, struct _formR*>())
				.def("clear", &CamObject::_Clear)
				.def("changeForm", &CamObject::_SetObject)
				.def("send", &CamObject::_Send)
				.def("getFormName", &CamObject::_GetObjectName)
				.def("getObject", &CamObject::_GetObject)
				,
			class_<CamUserTab>("CamUserTab")
				.def(constructor<CAmForm*, struct _formR*>())				
				.property("enable", &CamUserTab::GetEnable, &CamUserTab::SetEnable)
				.property("visible", &CamUserTab::GetVisible, &CamUserTab::SetVisible)
				.property("data", &CamUserTab::GetData, &CamUserTab::SetData)
				.property("text", &CamUserTab::GetText, &CamUserTab::SetText)				
				.property("backgroundColor", &CamUserTab::GetPRgb, &CamUserTab::SetPRgb)
				.property("textColor", &CamUserTab::GetTRgb, &CamUserTab::SetTRgb)
				.property("id", &CamUserTab::GetID, &CamUserTab::SetID)
				.property("hitPos", &CamUserTab::GetHitpos)
				//.property("index", &CamUserTab::GetIndex, &CamUserTab::SetIndex)				
				.def("refresh", &CamUserTab::Refresh)
				.def("getText", &CamUserTab::_GetText)
				.def("setText", &CamUserTab::_SetText)
				.def("isTabVisible", &CamUserTab::_IsTabVisible)
				.def("setTabVisible", &CamUserTab::_SetTabVisible)
				.def("getOrderInfo", &CamUserTab::_GetOrderInfo)
				.def("setOrderInfo", &CamUserTab::_SetOrderInfo)
				//.def("change", &CamUserTab::_Change)
				//.def("reload", &CamUserTab::_Reload)
				,
			class_<CamBrowser>("CamBrowser")
				.def(constructor<CAmForm*, struct _formR*, bool>())
				.property("visible", &CamBrowser::GetVisible, &CamBrowser::SetVisible)
				.def("navigate", &CamBrowser::_Navigate)				
		];
	bind_class_info(m_pLua);

	// On_관련 스크립트를 위한 객체

	luabind::globals(m_pLua)[(LPCSTR)"FORM"] = this;
	luabind::globals(m_pLua)[(LPCSTR)"SYSTEM"] = this;

}


void CScriptEngine::AddObject(CString names, CCmdTarget* object, DWORD flag)
{	
	CString strCurClass = object->GetRuntimeClass()->m_lpszClassName;
	
	////////////////////////////////////////////////////////////////////////
	//			 Lua 문법				      //
	// -------------------------------------------------------------------//
	// Method Call :  Obj_Name:Method_Name(Param)  * : 콜론으로 호출      //
	// Property Call : Obj_Name.Property = value   * . (dot)닷으로 호출   //
	////////////////////////////////////////////////////////////////////////
		
	if (strcmp(strCurClass, "CAmButton") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CAmButton*&)object;
	else if (strcmp(strCurClass, "CamRadio") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamRadio*&)object;
	else if (strcmp(strCurClass, "CamEdit") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamEdit*&)object;
	else if (strcmp(strCurClass, "CamLabel") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamLabel*&)object;
	else if (strcmp(strCurClass, "CamOut") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamOut*&)object;
	else if (strcmp(strCurClass, "CamCheck") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamCheck*&)object;
	else if (strcmp(strCurClass, "CamBrowser") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamBrowser*&)object;
	else if (strcmp(strCurClass, "CamPanel") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamPanel*&)object;
	else if (strcmp(strCurClass, "CamBox") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamBox*&)object;
	else if (strcmp(strCurClass, "CamCombo") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamCombo*&)object;
	else if (strcmp(strCurClass, "CamUserTab") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamUserTab*&)object;
	else if (strcmp(strCurClass, "CamTable") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamTable*&)object;
	else if (strcmp(strCurClass, "CamGrid") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamGrid*&)object;	
	else if (strcmp(strCurClass, "CamObject") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CamObject*&)object;	
	else if (strcmp(strCurClass, "CLogin") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CLogin*&)object;
	else if (strcmp(strCurClass, "CLedger") == 0)
		luabind::globals(m_pLua)[(LPCSTR)names] = (CLedger*&)object;	
		
	m_objects.SetAt(names, (CObject*&)object);
}


bool CScriptEngine::DoProcedure(CString procs)
{
	if (m_pLua)
	{
		/*
		if (procs.Find("true") > 0)
			procs.Replace("true", "1");
		else if (procs.Find("false") > 0)
			procs.Replace("false", "0");
		*/

		if (Debug_Lua( 0, luaL_loadstring(m_pLua, (LPCSTR)procs)))
			Debug_Lua(LUA_PCALL, lua_pcall(m_pLua, 0,0,0));

		return true;
	}	
	return false;
}



////////////////////////////////////////////////////////////////////////////
// FORM:
void CScriptEngine::LuaSend(int target, const char* strTR)
{
	if (!m_bSend)
	{
		CString strMsg;
		strMsg.Format("(SYSTEM)OnSend(boolean) Value is \"false\".\n");
		m_parent->SendMessage(WM_LUASCRIPT, MAKEWPARAM(m_clientKey, LUA_VERIFY), (LPARAM)(LPCTSTR)strMsg);
		return;
	}

	CString strTmp, strKey;
	strTmp.Format("%d=%d/%s", m_screenKey, target, strTR);
		
	m_parent->SendMessage(WM_LUASCRIPT, MAKEWPARAM(m_clientKey, LUA_SEND), (LPARAM)(LPCSTR)strTmp);
}

void CScriptEngine::LuaSend2(int target, const char* strTR, int session)
{
	LuaSend(target, strTR);
}

// 0: 자신, 1: main
LPCTSTR CScriptEngine::getFormName(long target)
{
	m_parent->SendMessage(WM_LUAGETNAME, MAKEWPARAM(m_clientKey, m_screenKey), MAKELPARAM(0, target));
	return m_formName;
}

void CScriptEngine::setOnSend(bool onsend)
{
	m_bSend = onsend;
}

bool CScriptEngine::getOnSend()
{
	return m_bSend;
}

CScriptEngine* CScriptEngine::getMain()
{
	if (!m_screenKey)	// screen Key가 0이면 자신이 Main Form이다.
	{
		CString strMsg;
		strMsg.Format("(SYSTEM)This Form is main form.\n");
		m_parent->SendMessage(WM_LUASCRIPT, MAKEWPARAM(m_clientKey, LUA_VERIFY), (LPARAM)(LPCTSTR)strMsg);
		return NULL;	
	}
	m_parent->SendMessage(WM_LUAGETMAIN, MAKEWPARAM(m_clientKey, m_screenKey), 0);

	if (m_pParent)
	{
		m_pParent->m_bParent = true;
		return m_pParent;
	}
	return NULL;
}

CScriptEngine* CScriptEngine::LuaGetParent()
{
	if (!m_screenKey)	// screen Key가 0이면 자신이 Main Form이다.
	{
		CString strMsg;
		strMsg.Format("(SYSTEM)This Form is main form.\n");
		m_parent->SendMessage(WM_LUASCRIPT, MAKEWPARAM(m_clientKey, LUA_VERIFY), (LPARAM)(LPCTSTR)strMsg);
		return NULL;	
	}
	// m_screenKey(현재) - 1 부모의 Screen을 가져온다.
	m_parent->SendMessage(WM_LUAGETMAIN, MAKEWPARAM(m_clientKey, m_screenKey), (LPARAM)(m_screenKey - 1));

	if (m_pParent)
	{
		m_pParent->m_bParent = true;
		return m_pParent;
	}
	return NULL;
}
//FORM:getParent():getObject("I1301").text
object CScriptEngine::LuaGetObject(const char* strName)
{
	object obj;
	int subKey = 0;
	
	CString formName;
	if (m_bParent)
	{		
		obj = luabind::globals(m_pLua)[strName];
		luabind::detail::object_rep *rep = touserdata<luabind::detail::object_rep>(obj);
		CString objName = rep->crep()->name();

		formName.Format("p%s", strName);

		if (strcmp(objName, "CAmButton") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CAmButton*>(obj);
		else if (strcmp(objName, "CamRadio") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CamRadio*>(obj);
		else if (strcmp(objName, "CamEdit") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CamEdit*>(obj);
		else if (strcmp(objName, "CamLabel") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CamLabel*>(obj);
		else if (strcmp(objName, "CamOut") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CamOut*>(obj);
		else if (strcmp(objName, "CamCheck") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CamCheck*>(obj);
		else if (strcmp(objName, "CamBrowser") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CamBrowser*>(obj);
		else if (strcmp(objName, "CamBox") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CamBox*>(obj);
		else if (strcmp(objName, "CamCombo") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CamCombo*>(obj);
		else if (strcmp(objName, "CamUserTab") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CamUserTab*>(obj);
		else if (strcmp(objName, "CamTable") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CamTable*>(obj);
		else if (strcmp(objName, "CamGrid") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CamGrid*>(obj);
		else if (strcmp(objName, "CamObject") == 0)
			luabind::globals(m_pLua2)[(LPCSTR)formName] = object_cast<CamObject*>(obj);

		m_bParent = false;
		strName = formName;
		obj = luabind::globals(m_pLua2)[strName];
		m_pObj = luabind::globals(m_pLua2)[(LPCSTR)strName];
		return obj;
	}
	
	obj = luabind::globals(m_pLua)[strName];
	m_pObj = luabind::globals(m_pLua)[(LPCSTR)strName];
	return obj;
}
/*object CScriptEngine::LuaGetObject(const char* strName)
{
	m_parent->SendMessage(WM_LUAOBJECT, MAKEWPARAM(m_clientKey, m_screenKey), MAKELPARAM(m_subScreenKey, m_bParent));

	object obj;

	if (!m_bGetObj)
		return obj;

	obj = luabind::globals(m_pLua2)[strName];
	luabind::detail::object_rep *rep = touserdata<luabind::detail::object_rep>(obj);
	CString objName = rep->crep()->name();

	CString formName;
	if (m_bParent)
		formName.Format("p%s", strName);
	else
		formName.Format("s%s", strName);

	if (strcmp(objName, "CAmButton") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CAmButton*>(obj);
	else if (strcmp(objName, "CamRadio") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CamRadio*>(obj);
	else if (strcmp(objName, "CamEdit") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CamEdit*>(obj);
	else if (strcmp(objName, "CamLabel") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CamLabel*>(obj);
	else if (strcmp(objName, "CamOut") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CamOut*>(obj);
	else if (strcmp(objName, "CamCheck") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CamCheck*>(obj);
	else if (strcmp(objName, "CamBrowser") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CamBrowser*>(obj);
	else if (strcmp(objName, "CamBox") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CamBox*>(obj);
	else if (strcmp(objName, "CamCombo") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CamCombo*>(obj);
	else if (strcmp(objName, "CamUserTab") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CamUserTab*>(obj);
	else if (strcmp(objName, "CamTable") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CamTable*>(obj);
	else if (strcmp(objName, "CamGrid") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CamGrid*>(obj);
	else if (strcmp(objName, "CamObject") == 0)
		luabind::globals(m_pLua)[(LPCSTR)formName] = object_cast<CamObject*>(obj);	

	//m_bGetObj = false;
	m_bParent = false;
	obj = luabind::globals(m_pLua)[(LPCSTR)formName];
	m_pObj = luabind::globals(m_pLua)[(LPCSTR)formName];
	return obj;
}*/

void CScriptEngine::setParentObj(lua_State* L)
{
	m_bGetObj = true;
	m_pLua2 = L;
}

// -- FORM : Does not Support
void CScriptEngine::LuaClose()
{
	NotSupportMSG("close");
}
void CScriptEngine::LuaClear()
{
	NotSupportMSG("clear");
}
void CScriptEngine::changeForm(const char* pchFormname)
{
	NotSupportMSG("changeForm");
}
int CScriptEngine::getAbsCoord(const char* pcName, int pos)
{
	NotSupportMSG("getAbsCoord");
	return 0;
}
void CScriptEngine::setAnimation(int type, int fromDelta, int toDelta)
{
	NotSupportMSG("setAnimation");
}
void CScriptEngine::commitAnimation(object ob, long duration, int effect, const char* pcMethod)
{
	NotSupportMSG("commitAnimation");
}
void CScriptEngine::triggerAndRun(int type, const char* name, const char* data, const char* procs, const char* args)
{
	NotSupportMSG("triggerAndRun");
}
void CScriptEngine::messageBoxAndRun(int msgid, int type, const char* msg, const char* title, const char* okStr, const char* cancelStr)
{
	NotSupportMSG("messageBoxAndRun");
}
void CScriptEngine::setRtsOn(bool on)
{
	NotSupportMSG("setRtsOn");
}
int CScriptEngine::getStartX()
{
	NotSupportMSG("getStartX");
	return 0;
}
int CScriptEngine::getStartY()
{
	NotSupportMSG("getStartY");
	return 0;
}
int CScriptEngine::getEndX()
{
	NotSupportMSG("getEndX");
	return 0;
}
int CScriptEngine::getEndY()
{
	NotSupportMSG("getEndY");
	return 0;
}
bool CScriptEngine::getVIsible()
{
	NotSupportMSG("getVIsible");
	return true;
}
void CScriptEngine::setVisible(bool visible)
{
	NotSupportMSG("setVIsible");
}

void CScriptEngine::setLayout(const char* name)
{
	NotSupportMSG("setLayout");
}
void CScriptEngine::setDefaultLayout()
{
	NotSupportMSG("setDefaultLayout");
}



///////////////////////////////////////////////////////////////////////////////
// SYSTEM:
void CScriptEngine::Push(LPCTSTR sKey, LPCTSTR data)
{
	CString strData;
	strData = CString(sKey) + "/" + CString(data);
	m_parent->SendMessage(WM_LUASCRIPT, MAKEWPARAM(m_clientKey, LUA_PUSH), (LPARAM)(LPCSTR)(LPCTSTR)strData);
}

LPCTSTR CScriptEngine::Pop(LPCTSTR sKey)
{
	CString strKey;
	strKey.Format("%d/%s", m_screenKey, sKey);
	m_parent->SendMessage(WM_LUASCRIPT, MAKEWPARAM(m_clientKey, LUA_POP), (LPARAM)(LPCSTR)(LPCTSTR)strKey);
	
	if (m_pushData.IsEmpty())
		return _T("Data Empty.");
	
	return (LPCTSTR)m_pushData;
}

void CScriptEngine::LuaMessageBox(LPCTSTR msg, LPCTSTR title)
{
	::MessageBox(NULL, CString(msg), CString(title), MB_OK);
}

void CScriptEngine::toast(LPCTSTR msg)
{
	::MessageBox(NULL, CString(msg), _T("Toast"), MB_OK);}

// -- SYSTEM : Does not Support
void CScriptEngine::createWindow(LPCTSTR fmName, long kind, long pos, long type, long x, long y, long width, long height)
{	
	NotSupportMSG("createWindow");
}
void CScriptEngine::Trigger(LPCTSTR fmName, LPCTSTR procs, LPCTSTR arg)
{	
	NotSupportMSG("Trigger");
}
int CScriptEngine::getMetrics(int index)
{	
	NotSupportMSG("getMetrics");
	return 0;
}
LPCTSTR CScriptEngine::getAccount(LPCTSTR name)
{	
	NotSupportMSG("getAccount");
	return "";
}
void CScriptEngine::setValue(LPCTSTR fileName, LPCTSTR groupName, LPCTSTR key, LPCTSTR value)
{	
	NotSupportMSG("setValue");
}
LPCTSTR CScriptEngine::getValue(LPCTSTR fileName, LPCTSTR groupName, LPCTSTR key)
{	
	NotSupportMSG("getValue");
	return "";
}
void CScriptEngine::deleteValue(LPCTSTR fileName, LPCTSTR groupName, LPCTSTR key)
{	
	NotSupportMSG("deleteValue");
}
void CScriptEngine::deleteGroup(LPCTSTR fileName, LPCTSTR groupName)
{	
	NotSupportMSG("deleteGroup");
}
void CScriptEngine::deleteFile(LPCTSTR fileName)
{	
	NotSupportMSG("deleteFile");
}
void CScriptEngine::deleteHistory(int type, LPCTSTR code)
{	
	NotSupportMSG("deleteHistory");
}								
void CScriptEngine::clearHistory(int type)
{	
	NotSupportMSG("clearHistory");
}
LPCTSTR CScriptEngine::getHistory(int type, bool last)
{	
	NotSupportMSG("getHistory");
	return "";
}
void CScriptEngine::addHistory(LPCTSTR code)
{
	NotSupportMSG("addHistoryWithType");
}
void CScriptEngine::addHistoryWithType(int type, LPCTSTR code, int index)
{	
	NotSupportMSG("addHistoryWithType");
}
int CScriptEngine::getCodeType(LPCTSTR code)
{	
	NotSupportMSG("getCodeType");
	return 0;
}
LPCTSTR CScriptEngine::getCodeName(LPCTSTR code)
{	
	NotSupportMSG("getCodeName");
	return "";
}
void CScriptEngine::goPreviousForm()
{	
	NotSupportMSG("goPreviousForm");
}
void CScriptEngine::saveCloudString(LPCTSTR key, LPCTSTR data)
{
	NotSupportMSG("saveCloudString");
}
void CScriptEngine::saveCloudData(LPCTSTR key, LPCTSTR path, LPCTSTR data)
{
	NotSupportMSG("saveCloudData");
}
void CScriptEngine::saveCloudFloat(LPCTSTR key, float fData)
{
	NotSupportMSG("saveCloudFloat");
}
void CScriptEngine::saveCloudBool(LPCTSTR key, bool bData)
{
	NotSupportMSG("saveCloudBool");
}
void CScriptEngine::saveCloudInt(LPCTSTR key, int nData)
{
	NotSupportMSG("saveCloudInt");
}
LPCTSTR CScriptEngine::loadCloudString(LPCTSTR key, LPCTSTR defstr)
{
	NotSupportMSG("loadCloudString");
	return defstr;
}
LPCTSTR CScriptEngine::loadCloudData(LPCTSTR key)
{
	NotSupportMSG("loadCloudData");
	return "";
}
float CScriptEngine::loadCloudFloat(LPCTSTR key, float defval)
{
	NotSupportMSG("loadCloudFloat");
	return defval;
}
bool CScriptEngine::loadCloudBool(LPCTSTR key, bool defval)
{
	NotSupportMSG("loadCloudBool");
	return defval;
}
int CScriptEngine::loadCloudInt(LPCTSTR key, int defval)
{
	NotSupportMSG("loadCloudInt");
	return defval;
}
void CScriptEngine::setOrientation(int orientation)
{
	NotSupportMSG("setOrientation");
}
/////////////////////////////////////////////////////////////////////////////////////

// Emulator 지원 안함 Tracex 메시지
void CScriptEngine::NotSupportMSG(CString strMethod)
{
	CString strMsg;
	strMsg.Format("(SYSTEM)This Method(%s) is not supported by Emulator.\n", strMethod);
	m_parent->SendMessage(WM_LUASCRIPT, MAKEWPARAM(m_clientKey, LUA_VERIFY), (LPARAM)(LPCTSTR)strMsg);
}
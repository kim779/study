/////////////////////////////////////////////////////////////////
//	ctinterface.h	for CSTool.dll
/////////////////////////////////////////////////////////////////
#pragma once
typedef CWnd* (__stdcall *_axNTool)(CWnd*, UINT, CString, CString);
//_axNTool	axNTool;

#define WM_TOOLEVENT		WM_USER + 111		// receive message for mainframe by tool window
#define WM_INTERFACE		WM_USER + 222		// receive message for tool base window
							// wParam : HIWORD - interface id, LOWORD - etc data, lParam : datas

//	interface
#define INTERFACE_INVENTORY	0x01			


//	tool event
#define EVENT_MAPDISPN		1
#define EVENT_MAPDESC		2
#define EVENT_MAPSNAME		3
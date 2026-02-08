// Smcall.cpp: implementation of the CSmcall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Smcall.h"
#include "smsheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSmcall::CSmcall()
{
}

CSmcall::~CSmcall()
{

}

void CSmcall::Set_Infomation(bool (*axiscall)(int, WPARAM, LPARAM), class CTMenu* menu)
{
	m_axiscall = axiscall;
	m_menu = menu;
}

void CSmcall::Show_Environment(int idx)
{
/*	CString		text;
	text.LoadString(ST_TEXT_ENVIRONMENT_SETUP);
	CSmSheet	dlg(text);

	CESetup		sheet1(m_home, m_menu);
	CTSetup		sheet2(m_menu, m_home, m_userN);
	CHotkey		sheet3(m_menu, m_home, m_userN);

	dlg.AddPage(&sheet1);
	dlg.AddPage(&sheet2);
	dlg.AddPage(&sheet3);

	if (dlg.GetPageCount() <= idx)
		return;

	dlg.SetActivePage(idx);
	dlg.DoModal();*/
}

void CSmcall::Show_Calc()
{
/*	CSmSheet	dlg("±ÝÀ¶°è»ê±â");

	CCalcA		sheet1;
	CCalcB		sheet2;
	CCalcC		sheet3;
	CCalcD		sheet4;

	dlg.AddPage(&sheet1);
	sheet2.SetHome(m_home);
	dlg.AddPage(&sheet2);
	dlg.AddPage(&sheet3);
	dlg.AddPage(&sheet4);
	dlg.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	dlg.DoModal();//*/
}
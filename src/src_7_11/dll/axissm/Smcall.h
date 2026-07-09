// Smcall.h: interface for the CSmcall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMCALL_H__D650ACA7_90AD_426D_851F_2F6B735928F0__INCLUDED_)
#define AFX_SMCALL_H__D650ACA7_90AD_426D_851F_2F6B735928F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSmcall  
{
public:
	CSmcall();
	virtual ~CSmcall();

	void	Set_Infomation(bool (*axiscall)(int, WPARAM, LPARAM), class CTMenu* menu, CString home, CString userN);
	void	Show_Environment(int idx);
	void	Show_Calc();
protected:
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

	class CTMenu*	m_menu;
	CString		m_home;
	CString		m_userN;
};

#endif // !defined(AFX_SMCALL_H__D650ACA7_90AD_426D_851F_2F6B735928F0__INCLUDED_)

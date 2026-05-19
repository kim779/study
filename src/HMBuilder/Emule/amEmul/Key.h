// Key.h: interface for the CKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEY_H__A672DC2B_B21E_4B71_BB0D_8BC02B07BB4E__INCLUDED_)
#define AFX_KEY_H__A672DC2B_B21E_4B71_BB0D_8BC02B07BB4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Screen.h"

class CKey  
{
public:
	CKey(class CClient* cleint);
	virtual ~CKey();


// Attributes
public:

protected:
	class CGuard*	m_guard;
	class CClient*	m_client;

// Operations
public:
	void	OnKey(WPARAM wParam, LPARAM lParam);
	void	OnIME(WPARAM wParam, LPARAM lParam);
	void	OnChar(WPARAM wParam, LPARAM lParam);
	void	GoNextForm(bool byKey = true);
	void	GoPrevForm();
	bool	OnChange(CScreen* screen, int ev, bool byKey);
	void	OnHome(bool start);

// Implementation
protected:
	bool	OnKeyCell(CScreen* screen, class CamBase* form, WPARAM wParam);
	void	OnChar(class CScreen* screen, class CamBase* form, int key, bool moving);
	bool	SetOnKeys(class CScreen*& screen, class CamBase*& form, bool reset);
	bool	OnKey(class CScreen* screen, WPARAM wParam, CamBase* form = NULL);
	bool	isKoscom(WPARAM wParam);
	int	NextForm(CScreen* screen, int& tab, bool back);
	int	NextForm(CScreen* screen, CamBase* form, bool back);
	bool	inFocus(CScreen* screen, CamBase* form);

	void	Copy$Paste(WPARAM wParam, CamBase* form);
};

#endif // !defined(AFX_KEY_H__A672DC2B_B21E_4B71_BB0D_8BC02B07BB4E__INCLUDED_)

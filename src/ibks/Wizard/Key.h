// Key.h: interface for the CKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEY_H__A672DC2B_B21E_4B71_BB0D_8BC02B07BB4E__INCLUDED_)
#define AFX_KEY_H__A672DC2B_B21E_4B71_BB0D_8BC02B07BB4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
	bool	OnKeyCell(CScreen* screen, class CfmBase* form, WPARAM wParam);
	void	OnChar(class CScreen* screen, class CfmBase* form, int key, bool moving);
	bool	SetOnKeys(class CScreen*& screen, class CfmBase*& form, bool reset);
	bool	OnKey(class CScreen* screen, WPARAM wParam, CfmBase* form = NULL);
	bool	isKoscom(WPARAM wParam);
	int	NextForm(CScreen* screen, int& tab, bool back);
	int	NextForm(CScreen* screen, CfmBase* form, bool back);
	bool	inFocus(CScreen* screen, CfmBase* form);

	void	Copy$Paste(WPARAM wParam, CScreen* screen, CfmBase* form);
};

#endif // !defined(AFX_KEY_H__A672DC2B_B21E_4B71_BB0D_8BC02B07BB4E__INCLUDED_)

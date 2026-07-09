// Mouse.h: interface for the CMouse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUSE_H__69BBFD5D_AA28_4ABE_9D18_FB09A540CC4F__INCLUDED_)
#define AFX_MOUSE_H__69BBFD5D_AA28_4ABE_9D18_FB09A540CC4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Misc.h"
enum class mouse_t {mouseNO, mouseIN, mouseOUT, mouseERR};

class CMouse  
{
public:
	CMouse(class CClient* client);
	virtual ~CMouse();

// Attributes
public:
	CCaret		m_caret;		// current client caret
	int		m_idx;

	int		m_repeat;
	CPoint		m_point;
	bool		m_capture;
	CCaret		m_mouse;		// mouse action caret
	CCaret		m_hover;		// mouse hover caret
	CCaret		m_move;			// mouse move caret
	class CScreen*	m_screen;		// current screen

protected:
	class CGuard*	m_guard;
	class CClient*	m_client;

	CCaret		m_object;		// object key
	CRect		m_rect;	

private:
	int		m_type;

// Operations
public:
	void	SaveMouse(CPoint &point, bool click = false);
	void	RestoreMouse(bool click = false);
	void	OnDown(CPoint point, WPARAM wParam);
	void	OnUp(CPoint point, WPARAM wParam);
	bool	OnRepeat(CPoint point);
	void	OnDblClick(CPoint point, WPARAM wParam);
	bool	OnWheel(CPoint point, WPARAM wParam);
	void	OnMove(CPoint point, WPARAM wParam);
	CScreen* GetScreen()	{ return m_screen; }
	bool	IsEnable(class CfmBase* form);
	void	OnCursor(bool reset = false, CPoint point = CPoint(-1, -1));

// Implementation
protected:
	mouse_t L_Down(CPoint& point, int shift);
	mouse_t	X_Down(CPoint& point);
	mouse_t	L_Up(CPoint& point);
	mouse_t	X_Up(CPoint& point);
	bool	OnClick();

	void	ComboBox(CPoint point, bool down, bool click = true);
	int	WhichForm(CScreen* screen, CPoint pt);
	int	WhichForm(CPoint pt);
	bool	IsSelf(CfmBase* form);
};

#endif // !defined(AFX_MOUSE_H__69BBFD5D_AA28_4ABE_9D18_FB09A540CC4F__INCLUDED_)

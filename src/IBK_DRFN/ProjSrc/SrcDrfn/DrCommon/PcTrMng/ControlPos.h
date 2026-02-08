//------------------------------------------------------------------------------
// ControlPos.h
// 
//	CControlPos 
//	Position controls on a form's resize 
// 
//		Copyright (c) 2000 Paul Wendt
// 
//		VERSION#	DATE			NAME	DESCRIPTION OF CHANGE
//		--------	----------	----	---------------------
//		1.01  	07/11/2000	PRW	Original creation.
// 
#ifndef CONTROLPOS_H_
#define CONTROLPOS_H_

//----------------------------------------------
// these #define's specify HOW the control
// will move. they can be combined with the
// bitwise or operator
//

#ifndef _CONTROLPOS__AA
#define _CONTROLPOS__AA
	#define CP_MOVE_HORIZONTAL					1
	#define CP_MOVE_VERTICAL					2
	#define CP_RESIZE_HORIZONTAL				4
	#define CP_RESIZE_VERTICAL					8
#endif //_CONTROLPOS__AA

typedef struct tagCONTROLDATA
{
	HWND  hControl;		// HWND's never change; some MFC functions return temporary CWnd *'s
	DWORD dwStyle;		// check the #define's above
	CRect m_ChRect;
	char  pCtrlName[20];
	//	BOOL  bControl;
} CONTROLDATA, *LPCONTROLDATA;


class CControlPos
{
public:
	CControlPos(CWnd* pParent = NULL);
	virtual ~CControlPos();

public:
	void SetParent(CWnd* pParent, CSize* pSize);

	BOOL AddControl(CWnd* pControl, const DWORD& dwStyle = CP_MOVE_HORIZONTAL);
	BOOL AddControl(const UINT& unId, const DWORD& dwStyle = CP_MOVE_HORIZONTAL);
	BOOL AddControl(CWnd* pControl, const DWORD& dwStyle, CString csVBName);
	BOOL AddControl(const UINT& unId, const DWORD& dwStyle, CString csVBName);

	// * 추가 2001. 03 .08
	// * 컨트롤이 아닌 영역도 추가 ex)Shape
//	BOOL AddControl2(LPRECT pRect, const DWORD& dwStyle = CP_MOVE_HORIZONTAL);

	BOOL RemoveControl(CWnd* pControl);
	BOOL RemoveControl(const UINT& unId);
	void ResetControls(void);
	virtual void MoveControls(void);
	void ResizeControls(int oldwidth, int oldheight, int newwidth, int newheight);

	//---------------------------------------------------
	// most of the time, you don't want to move controls
	// if the user reduces window size [controls can
	// overlap and cause "issues"]
	// negative moves won't move controls when the parent
	// window is getting smaller than its original size
	//
	void SetNegativeMoves(const BOOL& fNegativeMoves = TRUE);
	BOOL GetNegativeMoves(void) const;

protected:
	virtual void UpdateParentSize(void);

private:
	CWnd*  m_pParent;		
	int    m_nOldParentWidth;			
	int    m_nOldParentHeight;
	int    m_nOriginalParentWidth;
	int    m_nOriginalParentHeight;
	BOOL   m_fNegativeMoves;

public:
	CRect	m_PaRect;
	CRect	m_PaWRect;
	CRect	m_ChRect;
		
	CObArray m_awndControls;

	int									mp_nSavedWidth;
	int									mp_nSavedHeight;
};


#endif

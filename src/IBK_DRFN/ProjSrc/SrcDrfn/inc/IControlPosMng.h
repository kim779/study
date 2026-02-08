#ifndef _INTERFACE_OF_CONTROLPOS_H__BY_JUNOKLEE__HEADER_FILES___
#define _INTERFACE_OF_CONTROLPOS_H__BY_JUNOKLEE__HEADER_FILES___

// #include "../../Inc/IControlPosMng.h"
#include "IBaseDefine.h"
//#define		UUID_IControlPosManager				39 //'IBaseDefine.h'에서 정의됨.

#ifndef _CONTROLPOS__AA
#define _CONTROLPOS__AA
	#define CP_MOVE_HORIZONTAL					1
	#define CP_MOVE_VERTICAL					2
	#define CP_RESIZE_HORIZONTAL				4
	#define CP_RESIZE_VERTICAL					8
#endif //_CONTROLPOS__AA

interface IControlPosManager
{
	//------------------------------
	// 예약영역
	//------------------------------
	void*	m_pReservedForSelp;
	void*	m_pReserved1;
	void*	m_pReserved2;

	//------------------------------
	CWnd*	m_pParent;		
	int		m_nOldParentWidth;			
	int		m_nOldParentHeight;
	int		m_nOriginalParentWidth;
	int		m_nOriginalParentHeight;
	BOOL	m_fNegativeMoves;
	CRect	m_PaRect;
	CRect	m_PaWRect;
	CRect	m_ChRect;		
	CObArray m_awndControls;
	int		mp_nSavedWidth;
	int		mp_nSavedHeight;

	STDMETHOD_(BOOL, Advise)(CWnd* pParent = NULL) PURE;
	STDMETHOD_(BOOL, UnAdvise)() PURE;
	STDMETHOD_(void,  SetParent)(CWnd* pParent, CSize* pSize) PURE;
	STDMETHOD_(BOOL, AddControl)(CWnd* pControl, const DWORD& dwStyle = CP_MOVE_HORIZONTAL) PURE;
	STDMETHOD_(BOOL, AddControl)(const UINT& unId, const DWORD& dwStyle = CP_MOVE_HORIZONTAL) PURE;
	STDMETHOD_(BOOL, AddControl)(CWnd* pControl, const DWORD& dwStyle, CString csVBName) PURE;
	STDMETHOD_(BOOL, AddControl)(const UINT& unId, const DWORD& dwStyle, CString csVBName) PURE;

	STDMETHOD_(BOOL, RemoveControl)(CWnd* pControl) PURE;
	STDMETHOD_(BOOL, RemoveControl)(const UINT& unId) PURE;
	STDMETHOD_(void, ResetControls)() PURE;
//	virtual void MoveControls(void) PURE;
	STDMETHOD_(void, ResizeControls)(int oldwidth, int oldheight, int newwidth, int newheight) PURE;
	STDMETHOD_(void, SetNegativeMoves)(const BOOL& fNegativeMoves = TRUE) PURE;
	STDMETHOD_(BOOL, GetNegativeMoves)() const PURE;

//	virtual void UpdateParentSize(void) PURE;
};

//----------------------------------------------------
// internal structure used to hold all information
// about a CWnd* control
//


#endif // _INTERFACE_OF_CONTROLPOS_H__BY_JUNOKLEE__HEADER_FILES___


#if !defined(AFX_DRAWPATTENCTRL_H__538CA15F_D84C_4EF3_8A64_4052344059F7__INCLUDED_)
#define AFX_DRAWPATTENCTRL_H__538CA15F_D84C_4EF3_8A64_4052344059F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawPattenCtrl.h : header file
//
/************************************************************************/
/*                                                                      */
/*                          Patten Draw Control                         */
/*                                                                      */
/*                                                                      */
/*                            累己老 : 2007. 12~                        */
/*                       累己磊 : 快悼快 (DRfn co.)                     */
/*                                                                      */
/************************************************************************/

typedef struct tagPosition
{
	float x;
	float y;
} Position;

#define FILE_VERSION_DRAWDATA		3

/////////////////////////////////////////////////////////////////////////////
// CDrawPattenCtrl window
#define MAXCNT_XPOSITION		120
class CDrawPattenCtrl : public CWnd
{
// Construction
public:
	CDrawPattenCtrl();

private:
	//# System Val
	CWnd *m_pwndMsgTarget;
	int m_nOffset_CanvasGap;
	BOOL m_bEnable;
			
	CBrush m_brushCanvas;
	HPEN m_hPen_Move, m_hPen_Guide, m_hPen_Draw, m_hPen_Grid, m_hPen_Canvas;
	
	int m_nState_LMouse;	//# 0 : None, 1 : Draw Start, Draw During
		
	int m_nXGridCnt;
	CRect m_reCanvas;
	int m_nInterval_X, m_nInterval_Y;

	int m_nDrawXPos_Min, m_nDrawXPos_Max;
	int m_nDrawYPos_Min, m_nDrawYPos_Max;
	
	int m_nDrawPosition[MAXCNT_XPOSITION];
	
	int m_nPosX_Start;
	CPtrArray *m_paDrawData;
	CPtrArray *m_paDrawData_ReDraw;
	CFont m_fontDefault, m_fontSmall;

public:
	BOOL Create(CWnd* pParentWnd, UINT nID);
	void SetInit();

	void SetData_XGridCnt(int nData);
	int GetData_XGridCnt() { return m_nXGridCnt; };

	BOOL SetSave_DrawData(CString strFile = "");
	BOOL GetLoad_DrawData(CString strFile = "");
	BOOL LoadPatternData4Thumbnail(CString strFile, CRect& rcCanvas, CPtrArray *paDrawData);

	int GetData_DrawPacket(long *pDrawData);
	BOOL GetCheck_Sidle();
	
	void SetEnable(BOOL bEnable);
	BOOL HaveSaveData(){ return m_paDrawData->GetSize() > 0; }

private:
	void SetData_Clear();

	void FillSolidRect(HDC hDC, int x, int y, int cx, int cy, COLORREF clr);
	void Draw3dRect(HDC hDC, int x, int y, int cx, int cy, int nLH,
									COLORREF clrTopLeft, COLORREF clrBottomRight);

	void SetDraw_All(HDC hDC);

	void SetDraw_Canvas(HDC hDC, CRect reClient, COLORREF crColor);
	void SetDraw_Grid(HDC hDC);

	void SetDraw_Line(HDC hDC, CPtrArray *paData, int nModeDraw = 0);
	void SetDraw_GuideLine(HDC hDC);

	void SetDraw_Data(BOOL bDraw = FALSE);
	BOOL GetCheck_CanvasArea(Position ptPos);

	void SetAddData(CPtrArray *paData, Position stPosition, BOOL bCheck = TRUE);
	void AdjustJumpPoint(CPtrArray *paData);
	void SetDelData(CPtrArray *paData);
	void SetCalCulationData(CPtrArray *paData);

	void SetData_ReSize(CPtrArray *paData);
	CRect GetSize_Canvas(CRect reClient);
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawPattenCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawPattenCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDrawPattenCtrl)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWPATTENCTRL_H__538CA15F_D84C_4EF3_8A64_4052344059F7__INCLUDED_)

#pragma once
// AccCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAccCombo window

class CAccountCtrl;

#define WM_DELETE_COMBODATE WM_USER + 100
#define WM_MOUSEMOVE_COMBO  WM_USER + 200
#define STR_ALL_DELECT      "       전체 삭제"

#define ACC_NUMBER_LEN 13

////////////////////////// 
// 클래스명	    : CAccCombo
// 설 명	    : ComboBox 컨트롤의 삭제 이미지를 넣기 위한 클래스
// 작성일시	    : 2009/06/04
// 작성자       : kbkim
// 관련요건	    : 000010(조원형:2009/05/12)
//////////////////////////
class CAccCombo : public CComboBox
{
// Construction
public:
	CAccCombo(CAccountCtrl* pAccountCtrl);

// Attributes
public:
	inline BOOL	GetGroup() { return m_bGroup; };
	inline BOOL	GetGroupList() { return m_bGroupList; };
	inline int	GetGroupKey() { return m_nGroupKey; };
	inline void	SetGroup(BOOL bGroup) { m_bGroup = bGroup; };
	inline void	SetGroupList(BOOL bGroupList) { m_bGroupList = bGroupList; };
	inline void	SetGroupKey(int nGroupKey) { m_nGroupKey = nGroupKey; };

	void	AddAccString(CString strAcc);
	void	DeleteAllAcc();
	void	DeleteAllAccArray();
	CString	GetListCtrlText(int nIndex);
	void	SetSearchType(BOOL bType) { m_bSearchType = bType;	};	//2012.02.27 KSJ 검색된 리스트를 보여줄때는 'X'버튼 없앤다.
	BOOL	GetSearchType() { return m_bSearchType;	};	//2012.02.27 KSJ 

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccCombo)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAccCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAccCombo)
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LRESULT OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteMouseButton(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	HWND m_hListBox;
	CAccountCtrl* m_pParentAccountCtrl;
	CStringArray  m_strAccArray;

	CRect rComboImageRect;
	BOOL m_bGroup;
	BOOL m_bGroupList;
	int	m_nGroupKey;
	BOOL m_bSearchType;	//2012.02.27 KSJ 검색된 리스트를 보여줄때는 'X'버튼 없앤다.
	int m_nOldCount;	//2012.02.28 KSJ 이전갯수와 지금갯수가 같으면 리사이징 안해준다.
};

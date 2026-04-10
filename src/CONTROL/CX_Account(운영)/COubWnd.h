#pragma once


// COubWnd

class COubWnd : public CWnd
{
	DECLARE_DYNAMIC(COubWnd)

public:
	COubWnd();
	virtual ~COubWnd();

	CString m_sMsg{};

	CRect  m_recParent{};
	CRect  m_rect{};
	bool		m_bDrag{};
	bool     m_bShow{};
	CWnd* m_pWizard{};
	CWnd* m_pParent{};

	// -----------------------------------------------
	// 대리인 팝업 / 7일 억제 관련 추가 멤버
	// -----------------------------------------------
	BOOL		m_bIsAgent{};		// TRUE = 대리인 팝업 (체크박스 표시)
	BOOL		m_bDontShow{};		// 체크박스 상태 (7일 억제 여부)
	CString		m_sAccNo{};			// 계좌번호 (해시 저장용)
	CString		m_sIniPath{};		// ini 파일 전체 경로

private:
	CRect		m_rcCheckBox{};		// 체크박스 클릭 판정 영역 (내부용)

	// -----------------------------------------------
	// 해시 / 암복호화
	// -----------------------------------------------
	CString		HashAccount(const CString& sAccNo);
	CString		EncryptAccount(const CString& sAccNo);
	CString		DecryptAccount(const CString& sEncrypted);

	// -----------------------------------------------
	// ini 유틸
	// -----------------------------------------------
	void		SaveHideDate();		// 체크 시 날짜 저장
public:
	bool		IsHiddenToday();	// 7일 억제 중인지 확인 (AccountCtrl에서 호출)

	CFont* getAxFont(CString fName, int point, int style);

	void drawData(CDC* pDC);
	void drawCaption(CDC* pDC);
	CPen* getAxPen(COLORREF clr, int width, int style);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);



	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};



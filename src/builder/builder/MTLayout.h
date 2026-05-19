#pragma once
#include "afxwin.h"
#include "FormPreviewStatic.h"

#define PREVIEW_CNT 3

// CMTLayout 대화 상자입니다.

class CMTLayout : public CDialogEx
{
	DECLARE_DYNAMIC(CMTLayout)

public:
	CMTLayout(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMTLayout();
public:
	CArray<CWnd*, CWnd*> m_aryWnd;
	int GetSel() { return m_iSel + m_iStart; }
	void SetMaxCnt(int iMax);
	void InitPreview();
	void SetBkClr(int idx, COLORREF clr);
	void SetBkImg(int idx, CString strImg);
private:
	int m_iSel;
	int m_iStart;
	int m_iMax;
	void Init();
	CRect m_rc;
	COLORREF m_clr[PREVIEW_CNT];
	CString	 m_img[PREVIEW_CNT];
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MTLAYOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPre();
	afx_msg LRESULT OnObjMsg(WPARAM wParam, LPARAM lParam);
	CFormPreviewStatic m_picPreview1;
	CFormPreviewStatic m_picPreview2;
	CFormPreviewStatic m_picPreview3;
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void PreviewDelete();
	afx_msg void OnBnClickedButtonDel();
};

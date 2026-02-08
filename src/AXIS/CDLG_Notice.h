#pragma once

#include "ImgBtn.h"
// CDLG_Notice 대화 상자

class CDLG_Notice : public CDialog
{
	DECLARE_DYNAMIC(CDLG_Notice)

public:
	CDLG_Notice(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDLG_Notice();

	CString m_strurl, m_strbitmap, m_slog;;
	CBitmap* m_bmpBg;
	CBitmap* LoadFileBitmap(const char* bmpName);
	CImgBtn m_btnOk{};
	CImgBtn m_btnConfirm{};

	BOOL m_bDev{};
	BOOL m_file1pop{};
	BOOL m_file2pop{};

	CString m_sRoot{};
	CString m_sUser{};

	CRect m_rec1PFD{};   //매매 PDF
	CRect m_rec2PFD{};   //최선집행 PDF
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_NOTICEPOPUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChk1Confirm();
	afx_msg void OnBnClickedChk2Confirm();
	afx_msg void OnBnClickedBtnDevclose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

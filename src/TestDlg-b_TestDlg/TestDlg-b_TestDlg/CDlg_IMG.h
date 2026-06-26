#pragma once


// CDlg_IMG 대화 상자
#include <gdiplus.h>
using namespace Gdiplus;

class CDlg_IMG : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_IMG)

public:
	CDlg_IMG(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_IMG();
	CBitmap* m_pBltBmp{};
	CBitmap* m_pStrechBmp{};
	HBITMAP m_hBmp{};

	CBitmap* m_pBitTrans{};

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_IMG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:

	using MBITMAP = std::tuple<std::shared_ptr<CBitmap>, BOOL, BOOL>;
	//std::tuple<std::shared_ptr<CBitmap>, BOOL, BOOL> MBITMAP; //이렇게 하면 변수를 선언한거라  위처럼 해야한다
	std::map<CString, MBITMAP> _mbitmaps;
};

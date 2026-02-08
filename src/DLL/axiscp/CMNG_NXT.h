#pragma once

#include "SetupDialog.h"
#include "resource.h"
#include "../../axis/BmpButton.h"
#include <afxtempl.h>

class AFX_EXT_CLASS CMNG_NXT : public CSetupDialog
{
	//DECLARE_DYNAMIC(CMNG_NXT)

public:
	CMNG_NXT(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMNG_NXT();

	CMapStringToString	m_mapMng;

	void loadInfo();
	void setControlValue(CString keys, CString value);
	void saveInfo();
	virtual void		ApplySetup();
	virtual void		DefaultSetup();
	virtual void		Loadsetup();

	CFont m_font;
	void draw_groupBox(CPaintDC* pdc, int ids);

	int		m_pos;		// 0:좌상단, 1:우상단, 2:좌하단, 3:우하단
	int		m_sound;

	CBitmap m_bitmapTooltip{};
	std::unique_ptr<CBmpButton> m_bitmapBtn;  //IDC_STATIC_TOOL
// 대화 상자 데이터입니다.
	enum { IDD = IDD_MNG_NXT };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

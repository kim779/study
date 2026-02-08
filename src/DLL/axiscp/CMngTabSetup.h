#pragma once

#include "SetupDialog.h"
#include "resource.h"
// CMngTabSetup 대화 상자
#include <afxtempl.h>

class AFX_EXT_CLASS CMngTabSetup : public CSetupDialog
{


public:
	CMngTabSetup(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMngTabSetup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MNGTABSETUP };
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MNGTABSETUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	DECLARE_MESSAGE_MAP()
};

// CBrowserWnd.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CBrowserWnd.h"


// CBrowserWnd

IMPLEMENT_DYNAMIC(CBrowserWnd, CWnd)

CBrowserWnd::CBrowserWnd()
{

}

CBrowserWnd::~CBrowserWnd()
{
}


BEGIN_MESSAGE_MAP(CBrowserWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CBrowserWnd 메시지 처리기

#include "ControlComponent.h"


int CBrowserWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//NewComponent<ControlComponent>(this);
	ControlComponent* pcom{};
	pcom = new ControlComponent(this);
	return 0;
}


void CBrowserWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

}

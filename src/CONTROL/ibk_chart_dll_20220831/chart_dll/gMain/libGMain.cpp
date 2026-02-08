#include "stdafx.h"
#include "axisGMain.h"

#include "PnChart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

__declspec(dllexport) CWnd* WINAPI axCreateCtrl(int iCtrlKind, CWnd* pwndView, CWnd* pwndGView, char* info, CFont* pFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (iCtrlKind)
	{
	case GEV_CHART:
		{
			CPnChart* pChart = new CPnChart();
			return pChart->CreateCtrl(pwndView, pwndGView, info, pFont);
		}
	default:
		break;
	}
	return (CWnd *)NULL;
}


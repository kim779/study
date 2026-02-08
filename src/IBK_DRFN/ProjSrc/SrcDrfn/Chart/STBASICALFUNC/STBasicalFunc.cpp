// STBasicalFunc.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "../Include_ST/Commonlib.h"

#include "AbsValue.h"
#include "ACos.h"
#include "ASin.h"
#include "ATan.h"
#include "Avg.h"
#include "Buy.h"
#include "Ceiling.h"
#include "Cos.h"
#include "CosH.h"
#include "DateToJulian.h"
#include "DayClose.h"
#include "DayHigh.h"
#include "DayLow.h"
#include "DayOfWeek.h"
#include "DayOpen.h"
#include "ExitLong.h"
#include "ExitShort.h"
#include "Floor.h"
#include "FracPortion.h"
#include "IntPortion.h"
#include "JulianToDate.h"
#include "LastCalMMTime.h"
#include "Log.h"
#include "Log10.h"
#include "Max.h"
#include "Min.h"
#include "MinutesToTime.h"
#include "Pie.h"
#include "Plot1.h"
#include "Plot2.h"
#include "Plot3.h"
#include "Plot4.h"
#include "Plot5.h"
#include "Plot6.h"
#include "Plot7.h"
#include "Plot8.h"
#include "Plot9.h"
#include "Plot10.h"
#include "Plot11.h"
#include "Plot12.h"
#include "Plot13.h"
#include "Plot14.h"
#include "Plot15.h"
#include "Plot16.h"
#include "Plot17.h"
#include "Plot18.h"
#include "Plot19.h"
#include "Plot20.h"
#include "PlotBaseLine1.h"
#include "PlotBaseLine2.h"
#include "PlotBaseLine3.h"
#include "PlotBull1.h"
#include "PlotBear1.h"
#include "Pow.h"
#include "Random.h"
#include "Round.h"
#include "Sell.h"
#include "Sin.h"
#include "SinH.h"
#include "Sqrt.h"
#include "tan.h"
#include "TimeToMinutes.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CSTBasicalFuncApp

BEGIN_MESSAGE_MAP(CSTBasicalFuncApp, CWinApp)
	//{{AFX_MSG_MAP(CSTBasicalFuncApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTBasicalFuncApp construction

CSTBasicalFuncApp::CSTBasicalFuncApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

BOOL CSTBasicalFuncApp::GetInputs(long lType, long lIndex, CString &rStrFuncName, CString &rStrInputs)
{
	CInnerFunction* pInnerFunction = LoadInnerFunction(lIndex);
	if(pInnerFunction)
	{
		rStrFuncName = pInnerFunction->GetName(lType);
		rStrInputs = pInnerFunction->GetInputs(lType);
		delete pInnerFunction;
		return TRUE;
	}
	return FALSE;
}


CInnerFunction* CSTBasicalFuncApp::LoadInnerFunction(long lIndex)
{
	switch(lIndex)
	{
	case 0:		return new CAbsValue;		break;
	case 1:		return new CACos;			break;
	case 2:		return new CASin;			break;
	case 3:		return new CATan;			break;
	case 4:		return new CAvg;			break;
	case 5:		return new CBuy;			break;
	case 6:		return new CCeiling;		break;
	case 7:		return new CCos;			break;
	case 8:		return new CCosH;			break;
	case 9:		return new CDateToJulian;	break;
	case 10:	return new CDayClose;		break;
	case 11:	return new CDayHigh;		break;
	case 12:	return new CDayLow;			break;
	case 13:	return new CDayOfWeek;		break;
	case 14:	return new CDayOpen;		break;
	case 15:	return new CExitLong;		break;
	case 16:	return new CExitShort;		break;
	case 17:	return new CFloor;			break;
	case 18:	return new CFracPortion;	break;
	case 19:	return new CIntPortion;		break;
	case 20:	return new CJulianToDate;	break;
	case 21:	return new CLastCalMMTime;	break;
	case 22:	return new CLog;			break;
	case 23:	return new CLog10;			break;
	case 24:	return new CMax;			break;
	case 25:	return new CMin;			break;
	case 26:	return new CMinutesToTime;	break;
	case 27:	return new CPie;			break;	
	case 28:	return new CPlot1;			break;
	case 29:	return new CPlot2;			break;
	case 30:	return new CPlot3;			break;
	case 31:	return new CPlot4;			break;
	case 32:	return new CPlot5;			break;
	case 33:	return new CPlot6;			break;
	case 34:	return new CPlot7;			break;
	case 35:	return new CPlot8;			break;
	case 36:	return new CPlot9;			break;
	case 37:	return new CPlot10;			break;
	case 38:	return new CPlotBaseLine1;	break;
	case 39:	return new CPlotBaseLine2;	break;
	case 40:	return new CPlotBaseLine3;	break;
	case 41:	return new CPlotBull1;		break;
	case 42:	return new CPlotBear1;		break;
	case 43:	return new CPow;			break;
	case 44:	return new CRandom;			break;
	case 45:	return new CRound;			break;
	case 46:	return new CSell;			break;
	case 47:	return new CSin;			break;
	case 48:	return new CSinH;			break;
	case 49:	return new CSqrt;			break;
	case 50:	return new CTan;			break;
	case 51:	return new CTimeToMinutes;	break;

	// Added 2006.11.23
	case 52:	return new CPlot11;			break;
	case 53:	return new CPlot12;			break;
	case 54:	return new CPlot13;			break;
	case 55:	return new CPlot14;			break;
	case 56:	return new CPlot15;			break;
	case 57:	return new CPlot16;			break;
	case 58:	return new CPlot17;			break;
	case 59:	return new CPlot18;			break;
	case 60:	return new CPlot19;			break;
	case 61:	return new CPlot20;			break;
	default:
		return NULL;
	}
	return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CSTBasicalFuncApp object
CSTBasicalFuncApp theApp;

long WINAPI fnLoadInnerFunction(long lIndex)
{
	return (long)theApp.LoadInnerFunction(lIndex);
}

BOOL WINAPI fnGetInputs(long lType, long lIndex, CString &rStrFuncName, CString &rStrInputs)
{
	return theApp.GetInputs(lType, lIndex,rStrFuncName,rStrInputs);
}

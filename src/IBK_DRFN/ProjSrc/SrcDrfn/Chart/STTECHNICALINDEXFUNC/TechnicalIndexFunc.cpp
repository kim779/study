// TechnicalIndexFunc.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TechnicalIndexFunc.h"
#include "AccDist.h"
#include "Accum.h"
#include "AccumN.h"
#include "ADX.h"
#include "ATR.h"
#include "BollBandDown.h"
#include "BollBandUp.h"
#include "BW.h"
#include "CCI.h"
#include "CO.h"
#include "Correlation.h"
#include "CrossDown.h"
#include "CrossUp.h"
#include "CSar.h"
#include "CV.h"
#include "DiMinus.h"
#include "DiPlus.h"
#include "Disparity.h"
#include "EMA.h"
#include "EnvelopeDown.h"
#include "EnvelopeUp.h"
#include "EOM.h"
#include "Highest.h"
#include "Lowest.h"
#include "LRL.h"
#include "LRS.h"
#include "MA.h"
#include "MACD.h"
#include "MACD_OSC.h"
#include "MFI.h"
#include "MI.h"
#include "Momentum.h"
#include "MRO.h"
#include "NthHighest.h"
#include "NthHighestBar.h"
#include "NthLowest.h"
#include "NthLowestBar.h"
#include "NVI.h"
#include "OBV.h"
#include "OSCP.h"
#include "OSCV.h"
#include "ROC.h"		/*	#include "PROC.h"*/
#include "PVI.h"
#include "PVT.h"
#include "RSI.h"
#include "Sar.h"
#include "Simrido.h"
#include "SONAR.h"
#include "SPrice.h"
#include "Std.h"
#include "StochasticsD.h"
#include "StochasticsK.h"
#include "SwingHigh.h"
#include "SwingHighBar.h"
#include "SwingLow.h"
#include "SwingLowBar.h"
#include "TRIX.h"
#include "VHF.h"
#include "VR.h"
#include "VROC.h"
#include "WC.h"
#include "WILLA.h"
#include "WILLR.h"
#include "WMA.h"


/*-지표추가작업 (2005.7.4 ~ ---------------------------------------------------*/
#include "ARatio.h"
#include "BRatio.h"
#include "AroonUp.h"
#include "AroonDown.h"
#include "SROC.h"
#include "ForceIndex.h"
#include "DEMA.h"
#include "DEMA2.h"
#include "TEMA.h"
#include "TEMA2.h"
#include "RMI.h"
#include "Disparity_SMA.h"
#include "Disparity_EMA.h"
#include "BinaryWave.h"
#include "PriceChUpper.h"
#include "PriceChLower.h"
#include "StarcCenter.h"
#include "StarcUpper.h"
#include "StarcLower.h"
#include "Formula.h"
#include "DRF.h"
#include "NCO.h"
#include "CMF.h"
#include "ElderRayBear.h"
#include "ElderRayBull.h"
#include "PsyhologicalLine.h"
#include "StoFastK.h"
#include "StoFastD.h"
#include "TD.h"
#include "TDI.h"
#include "MarketPosition.h"
#include "BandB.h"
#include "BandWidth.h"
#include "ADXR.h"
#include "PDI.h"
#include "MDI.h"
#include "NDI.h"
#include "Qstick.h"
#include "DPO.h"
#include "StandardError.h"
#include "7BinaryWave.h"
#include "SterrBnCenter.h"
#include "SterrBnUpper.h"
#include "SterrBnLower.h"
/*------------------------------------------------------------------------------*/

#include "SMA.h"
#include "AMA.h"
#include "StochasticsD_Raw.h"
#include "StochasticsK_Raw.h"
/**************2006년 05월08일 추가*****************************/
#include "MACD_SMA.h"
#include "MACD_EMA.h"
#include "Sonar_SMA.h"
#include "Sonar_EMA.h"
#include "STD_SMA.h"
#include "STD_EMA.h"
/*********************************************************************************/
#include "MAO.h"
#include "SIGMA.h"



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
// CTechnicalIndexFuncApp

BEGIN_MESSAGE_MAP(CTechnicalIndexFuncApp, CWinApp)
	//{{AFX_MSG_MAP(CTechnicalIndexFuncApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTechnicalIndexFuncApp construction

CTechnicalIndexFuncApp::CTechnicalIndexFuncApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

BOOL CTechnicalIndexFuncApp::GetInputs(long lType, long lIndex, CString &rStrFuncName, CString &rStrInputs)
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

CInnerFunction* CTechnicalIndexFuncApp::LoadInnerFunction(long lIndex)
{
	switch(lIndex)
	{
	case 0:		return new CAccDist;		break;
	case 1:		return new CAccum;			break;
	case 2:		return new CAccumN;			break;
	case 3:		return new CADX;			break;
	case 4:		return new CATR;			break;
	case 5:		return new CBollBandDown;	break;
	case 6:		return new CBollBandUp;		break;
	case 7:		return new CBW;				break;
	case 8:		return new CCCI;			break;
	case 9:		return new CCO;				break;
	case 10:	return new CCorrelation;	break;
	case 11:	return new CCrossDown;		break;
	case 12:	return new CCrossUp;		break;
	case 13:	return new CCSar;			break;
	case 14:	return new CCV;				break;
	case 15:	return new CDiMinus;		break;
	case 16:	return new CDiPlus;			break;
	case 17:	return new CDisparity;		break;	
	case 18:	return new CEMA;			break;
	case 19:	return new CEnvelopeDown;	break;
	case 20:	return new CEnvelopeUp;		break;
	case 21:	return new CEOM;			break;
	case 22:	return new CHighest;		break;
	case 23:	return new CLowest;			break;
	case 24:	return new CLRL;			break;
	case 25:	return new CLRS;			break;
	case 26:	return new CMA;				break;
	case 27:	return new CMACD;			break;
	case 28:	return new CMACD_OSC;		break;
	case 29:	return new CMFI;			break;
	case 30:	return new CMI;				break;
	case 31:	return new CMomentum;		break;
	case 32:	return new CMRO;			break;
	case 33:	return new CNthHighest;		break;
	case 34:	return new CNthHighestBar;	break;
	case 35:	return new CNthLowest;		break;
	case 36:	return new CNthLowestBar;	break;
	case 37:	return new CNVI;			break;
	case 38:	return new COBV;			break;
	case 39:	return new COSCP;			break;
	case 40:	return new COSCV;			break;
/*	case 41:	return new CPROC;			break;		*/
	case 41:	return new CROC;			break;

	case 42:	return new CPVI;			break;
	case 43:	return new CPVT;			break;
	case 44:	return new CRSI;			break;
	case 45:	return new CSar;			break;
	case 46:	return new CSimrido;		break;
	case 47:	return new CSONAR;			break;
	case 48:	return new CSPrice;			break;
	case 49:	return new CStd;			break;
	case 50:	return new CStochasticsD;	break;		
	case 51:	return new CStochasticsK;	break;		
	case 52:	return new CSwingHigh;		break;
	case 53:	return new CSwingHighBar;	break;
	case 54:	return new CSwingLow;		break;
	case 55:	return new CSwingLowBar;	break;
	case 56:	return new CTRIX;			break;
	case 57:	return new CVHF;			break;
	case 58:	return new CVR;				break;
	case 59:	return new CVROC;			break;
	case 60:	return new CWC;				break;
	case 61:	return new CWILLA;			break;
	case 62:	return new CWILLR;			break;
	case 63:	return new CWMA;			break;

/*-지표추가작업 (2005.7.4 ~ ---------------------------------------------------*/
	case 64:	return new CARatio;			break;
	case 65:	return new CBRatio;			break;
	case 66:	return new CAroonUp;		break;
	case 67:	return new CAroonDown;		break;
	case 68:	return new CSROC;			break;
	case 69:	return new CForceIndex;		break;
	case 70:	return new CDEMA;			break;
//	case 71:	return new CDEMA2;			break;
	case 71:	return new CTEMA;			break;
//	case 73:	return new CTEMA2;			break;
	case 72:	return new CRMI;			break;
	case 73:	return new CDisparity_SMA;	break;
	case 74:	return new CDisparity_EMA;	break;
	case 75:	return new CBinaryWave;		break;
	case 76:	return new CPriceChUpper;	break;
	case 77:	return new CPriceChLower;	break;
	case 78:	return new CStarcCenter;	break;
	case 79:	return new CStarcUpper;		break;
	case 80:	return new CStarcLower;		break;
	case 81:	return new CFormula;		break;
	case 82:	return new CDRF;			break;
	case 83:	return new CNCO;			break;
	case 84:	return new CCMF;			break;
	case 85:	return new CElderRayBear;	break;
	case 86:	return new CElderRayBull;	break;
	case 87:	return new CPsyhologicalLine;	break;
	case 88:	return new CStoFastK;		break;
	case 89:	return new CStoFastD;		break;
	case 90:	return new CTD;				break;
	case 91:	return new CTDI;			break;
	case 92:	return new CMarketPosition;	break;
	case 93:	return new CBandB;			break;
	case 94:	return new CBandWidth;		break;
	case 95:	return new CADXR;			break;
	case 96:	return new CPDI;			break;
	case 97:	return new CMDI;			break;
	case 98:	return new CNDI;			break;
	case 99:	return new CQstick;			break;
	case 100:	return new CDPO;			break;
	case 101:	return new CStandardError;	break;
	case 102:	return new C7BinaryWave;	break;
	case 103:	return new CSterrBnCenter;	break;
	case 104:	return new CSterrBnUpper;	break;
	case 105:	return new CSterrBnLower;	break;
/*----------------------------------------------------------------------------*/
	case 106:	return new CSMA;			break;	
	case 107:	return new CAMA;			break;	
	case 108:	return new CStochasticsD_Raw;	break;		
	case 109:	return new CStochasticsK_Raw;	break;	
/*******************2006년 05월08일 추가******************************/
	case 110:	return new CMACD_SMA;			break;	
	case 111:	return new CMACD_EMA;			break;	
	case 112:	return new CSonar_SMA;			break;	
	case 113:	return new CSonar_EMA;			break;	
	case 114:	return new CSTD_SMA;			break;	
	case 115:	return new CSTD_EMA;			break;	
/*******************2006년 05월08일 추가******************************/
	case 116:	return new CMAO;				break;	
	case 117:	return new CSIGMA;				break;	
	default:	return NULL;	
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTechnicalIndexFuncApp object

CTechnicalIndexFuncApp theApp;

long WINAPI fnLoadInnerFunction(long lIndex)
{
	return (long)theApp.LoadInnerFunction(lIndex);
}

BOOL WINAPI fnGetInputs(long lType, long lIndex, CString &rStrFuncName, CString &rStrInputs)
{
	return theApp.GetInputs(lType,lIndex,rStrFuncName,rStrInputs);
}

BOOL CTechnicalIndexFuncApp::InitInstance() 
{
	return CWinApp::InitInstance();
}

int CTechnicalIndexFuncApp::ExitInstance() 
{
	return CWinApp::ExitInstance();
}


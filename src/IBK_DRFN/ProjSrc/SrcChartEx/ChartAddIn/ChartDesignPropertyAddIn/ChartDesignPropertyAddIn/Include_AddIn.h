// Include_AddIn.h: interface for the CInclude_AddIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INCLUDE_ADDIN_H__C490BAB2_66E9_4A10_AB59_5942AD67AC00__INCLUDED_)
#define AFX_INCLUDE_ADDIN_H__C490BAB2_66E9_4A10_AB59_5942AD67AC00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 해당 site관련 Addin Define 삭제 - ojtaso (20090907)
//#if defined _ADDIN_KC
#include "../Include_AddIn_134661/_resource.h"					// for ID_CAA_...
#include "../Include_AddIn_134661/_IAddInManager.h"				// for IAddInManager
#include "../Include_AddIn_134661/I134661/_IChartAddIn.h"		// for IChartAddIn
#include "../Include_AddIn_134661/I134661/_ChartAddInBase.h"	// for CChartAddInBase
#include "../Include_AddIn_134661/I134661/_IAddInToolMgr.h"		// for IAddInToolMgr
#include "../Include_AddIn_134661/I134661/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn_134661/I134661/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn_134661/I134661/_IBlock.h"			// for IBlock
#include "../Include_AddIn_134661/I134661/_IPacketManager.h"	// for IPacketManager
#include "../Include_AddIn_134661/I134661/_IPacket.h"			// for IPacket
#include "../Include_AddIn_134661/I134661/_IGraphInfoManager.h"	// for IGraphInfoManager
#include "../Include_AddIn_134661/I134661/_IGraphInfo.h"		// for IGraphInfo
#include "../Include_AddIn_134661/I134661/_AddInCommand.h"		// for DECLARE_ADDIN_EVENT_MAP

//#elif defined _ADDIN_SS
//#include "../Include_AddIn_994661/_resource.h"					// for ID_CAA_...
//#include "../Include_AddIn_994661/_IAddInManager.h"				// for IAddInManager
//#include "../Include_AddIn_994661/I994661/_IChartAddIn.h"		// for IChartAddIn
//#include "../Include_AddIn_994661/I994661/_ChartAddInBase.h"	// for CChartAddInBase
//#include "../Include_AddIn_994661/I994661/_IAddInToolMgr.h"		// for IAddInToolMgr
//#include "../Include_AddIn_994661/I994661/_IChartOCX.h"			// for IChartOCX
//#include "../Include_AddIn_994661/I994661/_IChartManager.h"		// for IChartManager
//#include "../Include_AddIn_994661/I994661/_IBlock.h"			// for IBlock
//#include "../Include_AddIn_994661/I994661/_IPacketManager.h"	// for IPacketManager
//#include "../Include_AddIn_994661/I994661/_IPacket.h"			// for IPacket
//#include "../Include_AddIn_994661/I994661/_IGraphInfoManager.h"	// for IGraphInfoManager
//#include "../Include_AddIn_994661/I994661/_IGraphInfo.h"		// for IGraphInfo
//#include "../Include_AddIn_994661/I994661/_AddInCommand.h"		// for DECLARE_ADDIN_EVENT_MAP
//
//#endif

#endif // !defined(AFX_INCLUDE_ADDIN_H__C490BAB2_66E9_4A10_AB59_5942AD67AC00__INCLUDED_)

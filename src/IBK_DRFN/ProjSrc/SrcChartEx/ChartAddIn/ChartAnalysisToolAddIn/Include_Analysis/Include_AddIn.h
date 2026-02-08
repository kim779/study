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
#include "../Include_AddIn_133101/_resource.h"					// for ID_CAA_...
#include "../Include_AddIn_133101/_IAddInManager.h"				// for IAddInManager
#include "../Include_AddIn_133101/I133101/_IChartAddIn.h"		// for IChartAddIn
#include "../Include_AddIn_133101/I133101/_ChartAddInBase.h"	// for CChartAddInBase
#include "../Include_AddIn_133101/I133101/_IAddInToolMgr.h"		// for IAddInToolMgr
#include "../Include_AddIn_133101/I133101/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn_133101/I133101/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn_133101/I133101/_IBlock.h"			// for IBlock
#include "../Include_AddIn_133101/I133101/_IPacketManager.h"	// for IPacketManager
#include "../Include_AddIn_133101/I133101/_IPacket.h"			// for IPacket
#include "../Include_AddIn_133101/I133101/_AddInCommand.h"		// for DECLARE_ADDIN_EVENT_MAP
//
//#elif defined _ADDIN_SS
//#include "../Include_AddIn_993101/_resource.h"					// for ID_CAA_...
//#include "../Include_AddIn_993101/_IAddInManager.h"				// for IAddInManager
//#include "../Include_AddIn_993101/I993101/_IChartAddIn.h"		// for IChartAddIn
//#include "../Include_AddIn_993101/I993101/_ChartAddInBase.h"	// for CChartAddInBase
//#include "../Include_AddIn_993101/I993101/_IAddInToolMgr.h"		// for IAddInToolMgr
//#include "../Include_AddIn_993101/I993101/_IChartOCX.h"			// for IChartOCX
//#include "../Include_AddIn_993101/I993101/_IChartManager.h"		// for IChartManager
//#include "../Include_AddIn_993101/I993101/_IBlock.h"			// for IBlock
//#include "../Include_AddIn_993101/I993101/_IPacketManager.h"	// for IPacketManager
//#include "../Include_AddIn_993101/I993101/_IPacket.h"			// for IPacket
//#include "../Include_AddIn_993101/I993101/_AddInCommand.h"		// for DECLARE_ADDIN_EVENT_MAP
//
//#else
//#include "D:/User/BaeSeungWon/~$Chart Working Folder/Project5/AddInManager/Include_Addin/_resource.h"					// for ID_CAA_...
//#include "D:/User/BaeSeungWon/~$Chart Working Folder/Project5/AddInManager/Include_Addin/_IAddInManager.h"				// for IAddInManager
//#include "D:/User/BaeSeungWon/~$Chart Working Folder/Project5/AddInManager/Include_Addin/I000000/_IChartAddIn.h"		// for IChartAddIn
//#include "D:/User/BaeSeungWon/~$Chart Working Folder/Project5/AddInManager/Include_Addin/I000000/_ChartAddInBase.h"		// for CChartAddInBase
//#include "D:/User/BaeSeungWon/~$Chart Working Folder/Project5/AddInManager/Include_Addin/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr
//#include "D:/User/BaeSeungWon/~$Chart Working Folder/Project5/AddInManager/Include_Addin/I000000/_IChartOCX.h"			// for IChartOCX
//#include "D:/User/BaeSeungWon/~$Chart Working Folder/Project5/AddInManager/Include_Addin/I000000/_IChartManager.h"		// for IChartManager
//#include "D:/User/BaeSeungWon/~$Chart Working Folder/Project5/AddInManager/Include_Addin/I000000/_IBlock.h"				// for IBlock
//#include "D:/User/BaeSeungWon/~$Chart Working Folder/Project5/AddInManager/Include_Addin/I000000/_IPacketManager.h"		// for IPacketManager
//#include "D:/User/BaeSeungWon/~$Chart Working Folder/Project5/AddInManager/Include_Addin/I000000/_IPacket.h"			// for IPacket
//#include "D:/User/BaeSeungWon/~$Chart Working Folder/Project5/AddInManager/Include_Addin/AddInCommand.h"				// for DECLARE_ADDIN_EVENT_MAP
//
//#endif

#endif // !defined(AFX_INCLUDE_ADDIN_H__C490BAB2_66E9_4A10_AB59_5942AD67AC00__INCLUDED_)

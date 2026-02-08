#ifndef __AXMCTRL_H__
#define __AXMCTRL_H__


//** include AXM header files.
#include "common.h"
#include "debug.h"

#include "AxMDib.h"
#include "AxMShade.h"
#include "AxButton.h"

#include "AxMBarCtrl.h"
#include "AxMBorder.h"
#include "AxMButtonCtrl.h"
#include "AxMEdit.h"
#include "AxMListCtrl.h"
#include "AxMRadioButton.h"
#include "AxMSpeedButton.h"
#include "AxMSpinCtrl.h"
#include "AxMStaticCtrl.h"
#include "AxMTabCtrl.h"
#include "AxMTreeCtrl.h"
#include "AxMTableCtrl.h"
#include "AxMCellMergeCtrl.h"
#include "AxMTableMergeCtrl.h"


//** define AXM Messages.
#ifndef AXM_MESSAGE

#define AXM_MESSAGE			(WM_USER + 30000)
#define AXM_SETMSGHANDLER	(AXM_MESSAGE + 1)
#define AXM_GETMSGHANDLER	(AXM_MESSAGE + 2)

#define WM_AXUSER		WM_USER+10000
#define	WM_AXDROPITEM	WM_AXUSER+1
#define	WM_AXSETCTRL	WM_AXUSER+2
#define	WM_AXGETCTRL	WM_AXUSER+3
#define	WM_AXDELKIND	WM_AXUSER+4
#define	WM_AXRELOAD		WM_AXUSER+5

#define	WM_AXRMOUSE		WM_AXUSER+6
#define	WM_AXADDUSER	WM_AXUSER+7
#define	WM_AXDELUSER	WM_AXUSER+8
#define	WM_AXGETUGRP	WM_AXUSER+9
#define	WM_AXADDGRP		WM_AXUSER+10

#define WM_AXITEMCLICK  WM_AXUSER+11

#endif //AXM_MESSAGE

#define AXM_TABLE_NOTIFY (AXM_MESSAGE + 10) // table making information 
			// LOWORD(wParam): column count, HIWORD(wParam): row count
			// lParam: unused
#define AXM_CELL_NOTIFY	 (AXM_MESSAGE + 11) // change cell merge information notify message
			// wParam: unused
			// lParam: unused


#define BEGIN_AXM_MESSAGE_MAP(param, target, msgFunc) \
		msg_map_wnd = param; \
		if (msg_map_wnd == target) return msgFunc(wParam, lParam); 
#define AXM_MESSAGE_MAP(target, msgFunc) \
		else if (msg_map_wnd == target) return msgFunc(wParam, lParam);
#define END_AXM_MESSAGE_MAP return 0L;


#endif // __AXMCTRL_H__
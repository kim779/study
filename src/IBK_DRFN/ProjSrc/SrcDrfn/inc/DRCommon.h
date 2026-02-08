#if !defined(AFX_DRFN_COMMON__HEADER_BY_JUNOKLEE_8C9176A965AA__INCLUDED_)
#define AFX_DRFN_COMMON__HEADER_BY_JUNOKLEE_8C9176A965AA__INCLUDED_

//#include "../../inc/DRCommon.h"

// RMSG_DRUSER -->
#ifndef message_RMSG_DRUSER
	#define message_RMSG_DRUSER
	const UINT RMSG_DRUSER = ::RegisterWindowMessage(_T("RMSG_DRUSER"));
#endif //message_RMSG_DRUSER

//BYTE bt = LOBYTE(LOWORD(wParam));
// bt List.
#define drbt_CODE				0x01
			//BYTE ht = HIBYTE(LOWORD(wParam));
			//[ht]	1 : EditFull
			//		2 : EditEnter
			//		3 : SelChanged
#define drbt_OPENSCREEN			0x02
#define	drbt_COLORSKIN			0x03
			//BYTE ht = HIBYTE(LOWORD(wParam));
			//[ht]	1 : Get
			//
			//		2 : Set
			//

#define	drbt_TRCOMM				0x04
// RMSG_DRUSER <--

#define	drbt_REGWINDOW			0x05
			//lPram	: 이름\t핸들(hex)

#define	drbt_GETINPUTBARMARKET	0x06

#endif // AFX_DRFN_COMMON__HEADER_BY_JUNOKLEE_8C9176A965AA__INCLUDED_


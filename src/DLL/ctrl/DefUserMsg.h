#ifndef	_DEF_USERMSG_H
#define _DEF_USERMSG_H

#define WM_USER_SET_DATA			(WM_USER + 1)
#define WM_USER_ON_SELECTION_CANCEL		(WM_USER + 2)
#define WM_USER_ON_SELECTION_OK			(WM_USER + 3)
#define WM_USER_ON_NEW_SELECTION		(WM_USER + 4)
#define WM_USER_ON_ERROR			(WM_USER + 5)

#define WM_COMBO_ON_ERROR			(WM_USER + 6)	
#define WM_LIST_ON_ERROR			(WM_USER + 7)
#define WM_EDIT_ON_ERROR			(WM_USER + 8)
#define	WM_TITLE_CHANGE				(WM_USER + 9)

// Error Msg : Search.cpp에서 error메세지 만듬
const	int	error_il = 0;			// 일자범위(1-300)
const	int	error_si = 1;			// 시간범위(0-60)
const	int	error_pr = 2;			// % (0.5-100)
const	int	error_pe = 3;			// % (0.5-1000)
const	int	error_pa = 4;			// % (0-100)

#endif
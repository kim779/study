/*
#define IDC_STATIC_ACCOUNT	100
#define IDC_STATIC_NAME		101
#define IDC_STATIC_PASSWORD	102
#define IDC_STATIC_QTY		103
#define IDC_STATIC_ORDER	104
*/
#define IDC_STATIC_WINDOW	100
#define IDC_ACCOUNT		0x00
#define IDC_EDIT_PASSWORD	0x01
#define IDC_SPIN_QTY		0x03
#define IDC_BUTTON_QTY		0x04
#define IDC_EDIT_ORDER		0x05
#define IDC_MATRIX		0x06
#define IDC_STATIC_GUIDE	0x07
#define IDC_CHECK_ORDER		0x08
#define IDC_EDIT_ORDERNUMBER	0x09
#define IDC_ORDERWINDOW		0x10
#define IDC_AMOUNTBUTTON	0x11

#define DLL_WIDTH		402
#define DLL_HEIGHT		206

#define SIZE_MARGIN		2
#define SIZE_MARGIN_1		1
#define SIZE_MARGIN_4		4
#define SIZE_CONTROL_HEIGHT	20
#define SIZE_STATIC_WIDTH	30
#define SIZE_STATIC_WIDTH_4	53
#define SIZE_ACC_CTRL_WIDTH	150		// 계좌
#define SIZE_ACC_NAME_WIDTH	60		// 계좌명
#define SIZE_EDIT_PASS_WIDTH	58		// 비밀번호
#define SIZE_CHECK_ORDER_WIDTH	100		// 체크박스
#define SIZE_MATRIX_BUTTON	20
#define SIZE_SPIN_WIDTH		74		// 수량
#define SIZE_ORDERNUMBER_WIDTH	60		// 주문번호
#define SIZE_MAP_STARTPOINT	46
#define SIZE_MAP_WIDTH		191
#define SIZE_MAP_HEIGHT		180

#define	MATRIX_MAX_COLUMN	5
#define MATRIX_MAX_ROW		5

#define TRKEY_ACCCTRL1		0x11
#define TRKEY_ACCCTRL2		0x12
#define TRKEY_PASS		0x13

#define TRNAME_PASS		"pibopswd"

#define CHANGE_ORDER_NUMBER	"OrderNumber"	// 주문번호 받아오는거
#define CHANGE_GUIDELINE	"GuideLine"
#define SEND_MAP_KEY		"SendMapKey"							 
#define GET_ACCOUNT			"GetAccount"
#define GET_QUANTITY		"GetQuantity"
#define GET_CODE			"80301"

#define TRIGGER_ACC_FOCUS		"FocusAccNum"
#define TRIGGER_PASSWORD_FOCUS	"FocusPasswd"
#define TRIGGER_VOLUME_FOCUS	"FocusVolume"






#define TIMER_GUIDE		1
#define TIMER_TIME		3000

//// INI파일
#define INI_FILE			"SI906400.INI"
#define INI_APP_MATRIX		"matrix"
#define INI_KEY_ROW			"row"
#define INI_KEY_COL			"col"
#define INI_KEY_CHECK		"check"

#define INI_APP_ITEM		"item"
#define INI_KEY_CODE		"name"

//계좌 수신		( Winix의 include_def.h 에서 가져옴 )
#define			ACC_REV_KEY1		125
#define			ACC_REV_KEY2		126

// 사용자 정의
#define _TRIMDATA(data)		data.TrimLeft(); data.TrimRight();
#define DELETE_WND(p)		if(p) { p->DestroyWindow(); delete p; p = NULL;	}
//#define SAFE_DELETE(p)		if(p) {	delete p; p = NULL;}

enum { STATIC_ACCOUNT = 0, STATIC_NAME, STATIC_PASSWORD, STATIC_QTY, STATIC_ORDER };

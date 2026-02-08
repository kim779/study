#pragma once

#include "../IhUtils.h"
#include "../../axis/axMsg.hxx"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/memDc.h"
#define WM_MANAGE	(WM_USER + 1000)
#include "Resource.h"

/////////////////////////////////////////////////////////////////
//////  WM_MAINWND	Msg Kind
/////////////////////////////////////////////////////////////////
// LOWORD(wParam)
#define IDC_MAXRIX					5000				//매트릭스 버튼
#define	IDC_MAXRIXMAP				5001
#define IDC_RADIO_10HG				5002
#define IDC_RADIO_5HG				5003
#define IDC_SEARCHJM				5004
#define IDC_BTN_MAESU				5005
#define IDC_BTN_MAEDO				5006
#define IDC_COMBO_CHANGETAB			5007
#define IDC_CHECK_SELECT			5008
#define IDC_RANGE_PRICE				5009
#define IDC_CHECK_PRICE				5010
#define IDC_EDIT_PRICEF				5011
#define IDC_EDIT_PRICET				5012

#define IDC_TOOL					7000				
#define IDC_VIEW					7001
#define	MK_SHOWMATRIX				7002
#define MK_HIDEMATRIX				7003
#define	MK_SETROWCOL				7004
#define MK_SHOW10HOGA				7005
#define MK_SHOW5HOGA				7006
#define MK_INITROWCOL				7007
#define MK_DOMINOSETROWCOL			7008
#define	MK_CHANGETAB				7009
#define MK_SNAPSHOT					7010
#define MK_LOADSNAP					7011
#define MK_RANGEPERCENT				7012
#define MK_ALLPERCENT				7013
#define MK_MARKETSNAPSHOT	    7014

#define	IDC_HOGAWND					7100

#define	currMAP						"IB2016T0"			// kospi  현재가
#define	currMAP2					"IB2016T1"			
#define IMAGEDIR					"image"


#define		P_DELI					0x7f
#define		P_TAB					0x09
#define		P_NEW					0x0a
#define		P_PER					0x25

#define		MAP_WIDTH					330
#define		MAP_HEIGHT					400
#define		HOGA_WIDTH					318
#define		HOGA_HEIGHT					365

#define		DUAL_MONITOR				10001
#define		SINGLE_MONITOR				10002

#define		HOGATYPE10				10
#define		HOGATYPE5				5

#define		MATRIX_MAXROW			8
#define		MATRIX_MAXCOL			4
#define		MAX_GROUP				40

#define		MK_OPERSIZE				8000
#define		MK_KILLFOCUS			8001

#define		C_BACKIDX				181
#define		C_LBLBACKIDX			77
#define		C_MEDOBACKIDX			82
#define		C_MESUBACKIDX			85
#define		C_LBLBACKIDX2			78
#define		C_LINEIDX				43

#define		C_DATAFGIDX				69
#define		C_LBLFGIDX				130

#define	    ROUND_PANEL				1
#define		ROUND_CONTENTS			2
#define		MK_SETPAL				3	// set color

#define		WM_COMBO_ENTERED		9000
#define		IDC_COMBO_SEARCH		9001
#define		IDC_BTN_SEARCH			9002
#define		IDC_CODECTRL			9003

#define		IDC_BTN1				4001
#define		IDC_BTN2				4002
#define		IDC_BTN3				4003
#define		IDC_BTN4				4004
#define		IDC_BTN5				4005
#define		IDC_SAVE				4006

// send key
// char 한자리에대한 치환 스트럭쳐
struct _trkey
{
	UINT	kind:5;
	UINT	group:3;
};

#define	L_trkey	sizeof(struct _trkey)

struct _jobObj {
	CString msgBuf;
	int key;
	int msg;
	int len;
};

const CString szMatrix = _T("IB201600");

const int gap1		= 1;
const int gap2		= 2;
const int gap3		= 3;
const int gap5		= 5;
const int comboH	= 20;
const int panelH    = 25;
const int offsetGH	= gap5 + comboH + gap3;
const int gapHoga   = 28;

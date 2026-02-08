
#pragma once


#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/ledger.h"
#include "../../h/grid.h"
#include "../../control/fx_grid/fxgrid.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxstatic.h"

#include "MxFlatEdit.h"

#include "resource_def.h"
#include "common_ml.h"
#include "grid_inc.h"
#include "struct.h"
#include "calc_com.h"

#define		CONFIGFILE				"stopmit_fo.ini"
#define		LOGFILE					"axis_stopfolog.dat"


#define		KEY_ACCOUNT				255//계좌컨트롤 메세지
#define		MAP_WIDTH				966
#define		MAP_HEIGHT				374	//487

//colorINDEX
#define		BACKCOLOR_INDEX			64
#define		TOOLCOLOR_INDEX			66
#define		BODYCOLOR_INDEX			181
#define		GRIDLINECOLOR_INDEX		75
#define		GRIDHDRCOLOR_INDEX		74
#define		GRIDHDRTXTCOLOR_INDEX	76
#define		GRIDROWCOLOR1_INDEX		68
#define		GRIDROWCOLOR2_INDEX		77
#define		GRIDFOCUSCOLOR_INDEX	78
#define		GUIDCOLOR_INDEX			34
#define		GUIDTXTCOLOR_INDEX		130
#define		BUTTONTXTCOLOR_INDEX	97

#define		PANELBACK_INDEX			66
#define		PANELBORDER_INDEX		131

#define		LABELBACK_INDEX			160
#define		LABELBORDER_INDEX		89


#define		UPCOLOR_INDEX			94
#define		DOWNCOLOR_INDEX			95
#define		TXTCOLOR_INDEX			130

#define		MAESU_BACKINDEX			162
#define		MAESU_LINEINDEX			163
#define		MAESU_BOXBGINDEX1		174
#define		MAESU_BOXBGINDEX2		170

#define		MAEDO_BACKINDEX			164
#define		MAEDO_LINEINDEX			165
#define		MAEDO_BOXBGINDEX1		175
#define		MAEDO_BOXBGINDEX2		171

#define		DANGA_BGINDEX			104
#define		DANGA_TXTINDEX			103

#define		REALBACK_INDEX			5

//////SIZE
#define		LINEHEIGHT				20
#define		HDRLINEHEIGHT			12
#define		GUIDE_HEIGHT			18
#define		TXTGAP_S				10
#define		GAP_WIDTH				3
#define		GAP_WIDTH_MIN			2
#define		GAP_WIDTH_L				6
#define		TOOL_HEIGHT				25
#define		CONFIGWINDOW_W			640
#define		ACCOUNT_WIDTH			160
#define		ACCNAME_WIDTH			85
#define		PASS_WIDTH				120//lbl+edit
#define		GRIDHEADER_HEIGHT		45
#define		VERT_GAP				3
#define		HORI_GAP				3
#define		BOT_GAP					6
#define		CONTROL_HEIGHT			20
#define		BUTTON_WIDTH			56
#define		TOOLGAP					1


#define		LINEH					14
#define		COLW					68

//////DEF STRING
#define		TABDIR					"tab"
#define		IMAGEDIR				"image"
//#define		tabCHAR					0x09
#define		tabCHAR					'\t'
//#define		newlineCHAR				0x0a
#define		newlineCHAR				'\r'

//////DEF STRING-LOG
//state : S-접수, C-접수 수신, 
#define		SENDJUMUN				"S"
#define		RECVJUMUN				"C"
#define		CONTJUMUN				"X"
#define		MAPNAME					"IB300800"
#define		MAPTITLE				"선물옵션스탑로스"


/////QUERY KEY
#define		SJGOQUERY				"PIBOSJGO"
#define		SJGOSERVICE				"SONAQ052"
#define		POOPQUERY				"POOPPOOP"
#define		ORDERSERVICE			"PIBOFODR"
#define		LOGQUERY				"pidologm"

#define		B_SETUP					"*"
#define		S_SETUP					"■"

#define		SJGOLIST				0
#define		SJGOROW					1
#define		GOOPKEY					2
#define		HOGALIST				3
#define		POOPKEY					4
#define		ORDERKEY				5
#define		LOGKEY					6
#define		PASSKEY					7



#define		SJGOROWLEN				194
#define		SENDTR					1


#define		MAXROW					100

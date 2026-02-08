#ifndef GRID_TYPEDEF
#define GRID_TYPEDEF

#define GETFONT(logFont, nHeight, nWeight, fontData) \
	logFont.lfHeight = nHeight;                            \
	logFont.lfWidth = 0;                                    \
	logFont.lfEscapement = 0;                               \
	logFont.lfOrientation = 0;                              \
	logFont.lfWeight = nWeight;                             \
	logFont.lfItalic = 0;                                   \
	logFont.lfUnderline	=	0;                                \
	logFont.lfStrikeOut	=	0;                                \
	logFont.lfCharSet	=	129;                                \
	logFont.lfOutPrecision	=	3;                            \
	logFont.lfClipPrecision	=	2;                            \
	logFont.lfQuality	=	1;                                  \
	logFont.lfPitchAndFamily=	49;                           \
	strcpy(logFont.lfFaceName, (const char*)_T("굴림체"));  \
  fontData.CreateFontIndirect(&logFont);

	
#define COLOR_SPLITTER			RGB(171, 168, 153)

//# Head Color
#define COLOR_HEADERTEXT				RGB(0, 0, 1)
#define COLOR_HEADERTEXT_FOCUS			COLOR_HEADERTEXT

#define COLOR_HEADERFACE				RGB(227, 231, 237)
#define COLOR_HEADERFACE_FOCUS			RGB(210, 222, 238)

//# Face Color
#define COLOR_SEL_ROW					RGB(255, 243, 198)
#define COLOR_EVEN_ROW					RGB(255, 255, 255)
#define COLOR_ODD_ROW					RGB(242, 245, 250)
#define COLOR_LINE						RGB(167, 167, 167)

#define COLOR_SEL_ROW_FOCUS				RGB(206, 221, 239)
#define COLOR_EVEN_ROW_FOCUS			RGB(239, 243, 246)
#define COLOR_ODD_ROW_FOCUS				RGB(255, 255, 255)

//# Button Color
#define COLOR_BUTTON_TEXT				RGB(0, 0, 1)
#define COLOR_BUTTON					RGB(218, 218, 218)
#define COLOR_BUTTON_SEARCH				RGB(221, 224, 233)


const int FOOTER_DEF_HEIGHT	=	28;

#define COLOR_SELECT_GROUP				RGB(255, 255, 225)
#define COLOR_BLACK						RGB(0, 0, 1)
#define COLOR_WHITE						RGB(255, 255, 255)

//#define COLOR_FOOTER					RGB(226, 227, 232)
#define COLOR_FOOTER					RGB(232, 226, 210)		// KB투자증권

//#define COLOR_FOOTER_FOCUS			RGB(206, 217, 221)
#define COLOR_FOOTER_FOCUS				RGB(220, 205, 172)		// KB투자증권	

//#define COLOR_LABEL					RGB(226, 227, 232)
#define COLOR_LABEL						RGB(232, 226, 210)		// KB투자증권

//#define COLOR_LABEL_FOCUS				RGB(206, 217, 221)
#define	COLOR_LABEL_FOCUS				RGB(220, 205, 172)		// KB투자증권	



//# Grid Event Message
//# Grid => Wnd or View
#define WM_GRID_EVENT													(WM_USER + 10)

#define NM_GRID_SETFOCUS											1
#define NM_GRID_KILLSETFOCUS									2		//#<= Not User

#define NM_ROW_SELECT													3
#define NM_ROW_MAXMIN													4

#define NM_ROW_DRAGDROP												5		//# (0) : Old Row, (1) : New Row

#define NM_HEAD_VALVIEWALL_CHECK							10	//# Val
#define NM_HEAD_DELETEALL											11	//# All Delete

#define NM_ROW_CONDITION_CHECK								20	//# (0) : Element, (1) : Row, (2) : Val
#define NM_ROW_TARGET_LIST										21	//# (0) : Element, (1) : Row, (2) : Target Type, (3) : Target
#define NM_ROW_TARGET_LIST_START							25	//# Only Order Grid => Window UpDate
#define NM_ROW_VALVIEW_CHECK									22	//# (0) : Element, (1) : Row, (2) : Val
#define NM_ROW_MODIFY_BTN											23	//# (0) : Element, (1) : Row, (2) : Display Row, (3) : Display Row => Top Screen Pos
#define NM_ROW_DELETE_BTN											24	//# (0) : Element, (1) : Row(After Select) 

#define NM_ROW_UP_BTN										26
#define NM_ROW_DOWN_BTN										27	
#define NM_ROW_MAXUP_BTN									28
#define NM_ROW_MINDOWN_BTN									29

#define NM_ALIGN_ELEMENT											30

#define NM_GROUP_ADD													40	//# Group	No
#define NM_GROUP_INSERTUP											41	//# Group	No
#define NM_GROUP_INSERTDOWN										42	//# Group	No
#define NM_GROUP_DELETE												43	//# Group	No
#define NM_UPDATE											44


//# Event => Send Wnd Message
//# Order Wnd => Order View
#define WM_WND_EVENT													(WM_USER + 20)

#define NM_SELECT_GROUP												1		//# Group
#define NM_SELECT_CONDITION										2		//# (0) : Group, (1) : Row, (2) : Add Falg

#define NM_CONDITION_VIEW											4		//# (0) : Group, (1) : TotHeight

#define NM_STARTBONG_VAL											8		//# (0) : Group, (1) : Val
#define NM_ENDBONG_VAL												9		//# (0) : Group, (1) : Val

#define NM_CONDITION_DRAGDROP									5		//# (0) : Group, (1) : Old Row, (2) : New Row

#define NM_VALVIEWALL_CHECK										10		//# (0) : Group, (1) : Val
#define NM_DELETEALL													11		//# Group
	
#define NM_CONDITION_CHECK										20		//# (0) : Group, (1) : Row, (2) : Val
#define NM_TARGET_LIST												21		//# (0) : Group, (1) : Row, (2) : Target Type, (3) : Target
#define NM_VALVIEW_CHECK											22		//# (0) : Group, (1) : Row, (2) : Val
#define NM_MODIFY_BTN													23		//# (0) : Group, (1) : Row, (2) : Display Row, (3) : Display Row => Top Screen Pos
#define NM_DELETE_BTN													24		//# (0) : Group, (1) : Row(After Select), (2) : TotHeight, (3) : Element Index

#define NM_SETBTNIMG												(WM_USER + 50)
#define NM_CHANGE_SUBJECT											51
#define NM_CHANGE_SUBJECT_INIT										52

/*
#define NM_ALIGN_ELEMENT											30

#define NM_GROUP_ADD													40	//# Group	
#define NM_GROUP_INSERTUP											41	//# Group
#define NM_GROUP_INSERTDOWN										42	//# Group
#define NM_GROUP_DELETE												43	//# Group
*/



#define WM_MAIN_EVENT													(WM_USER + 30)

/*
#define NM_SELECT_GROUP												1		//#=> GroupNo
#define NM_SELECT_CONDITION										2		//# (0) : Group, (1) : Row, (2) : Add Falg

#define NM_ALIGN_ELEMENT											30	//# (0) : Type, (1) : TotElementIndex	

#define NM_VALVIEWALL_CHECK										10		//# (0) : Group, (1) : Val
#define NM_DELETEALL													11		//# Group

#define NM_CONDITION_CHECK										20		//# (0) : Group, (1) : Row, (2) : Val
#define NM_TARGET_LIST												21		//# (0) : Group, (1) : Row, (2) : Target Type, (3) : Target
#define NM_VALVIEW_CHECK											22		//# (0) : Group, (1) : Row, (2) : Val
#define NM_MODIFY_BTN													23		//# (0) : Group, (1) : Row, (2) : Display Row, (3) : Display Row => Top Screen Pos
#define NM_DELETE_BTN													24		//# (0) : Group, (1) : Row(After Select), (2) : Element Index


#define NM_GROUP_INSERTUP											41	//# (0) : Group Index (1) : Group
#define NM_GROUP_INSERTDOWN										42	//# (0) : Group Index (1) : Group
#define NM_GROUP_DELETE												43	//# (0) : Group Index (1) : Focus Group
*/




//# WM_NOTIFY_CMGRIDEVENT
#define WM_CALLPARENT													(WM_USER + 100)

/*
#define NM_SELECT_GROUP												1		//# Group
#define NM_SELECT_CONDITION										2		//# (0) : Group, (1) : Row

#define NM_CONDITION_DRAGDROP									5		//# (0) : Group, (1) : Old Row, (2) : New Row

#define NM_STARTBONG_VAL											8		//# (0) : Group, (1) : Val
#define NM_ENDBONG_VAL												9		//# (0) : Group, (1) : Val

#define NM_ALIGN_ELEMENT											30	//#=> Last Element Index


#define NM_VALVIEWALL_CHECK										10		//# (0) : Group, (1) : Val
#define NM_DELETEALL													11		//# Group

#define NM_CONDITION_CHECK										20		//# (0) : Group, (1) : Row, (2) : Val
#define NM_TARGET_LIST												21		//# (0) : Group, (1) : Row, (2) : Target Type, (3) : Target
#define NM_VALVIEW_CHECK											22		//# (0) : Group, (1) : Row, (2) : Val
#define NM_MODIFY_BTN													23		//# (0) : Group, (1) : Row, (2) : Display Row, (3) : Display Row => Top Screen Pos
#define NM_DELETE_BTN													24		//# (0) : Group, (1) : Row(After Select), (2) : Element Index


#define NM_GROUP_ADD													40	//# Group	
#define NM_GROUP_INSERTUP											41	//# Group
#define NM_GROUP_INSERTDOWN										42	//# Group
#define NM_GROUP_DELETE												43	//# Group
*/

typedef struct  
{
	COLORREF clrGridHeader;
	COLORREF clrGridHText;  
	COLORREF clrGridHilight;
	COLORREF clrGridSBk;
	COLORREF clrGridSText;  
	COLORREF clrGridUpText;  
	COLORREF clrGridDownText;
	COLORREF clrGridVolText; 
} ST_GRIDCOLOR;

#endif // GRID_TYPEDEF
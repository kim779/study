#ifndef MSG_TYPEDEF
#define MSG_TYPEDEF

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

#define NM_ALIGN_ELEMENT											30

#define NM_GROUP_ADD													40	//# Group	No
#define NM_GROUP_INSERTUP											41	//# Group	No
#define NM_GROUP_INSERTDOWN										42	//# Group	No
#define NM_GROUP_DELETE												43	//# Group	No


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


#endif // MSG_TYPEDEF
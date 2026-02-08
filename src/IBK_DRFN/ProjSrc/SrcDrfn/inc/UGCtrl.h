/***********************************************
	Ultimate Grid 97
	Copyright 1994 - 1997 Dundas Software Ltd.

	Class 
		CUGCtrl
	Purpose
		This is the main grid class. It contains
		and/or controls all of the other grid clases.
		CUGCtrl - stands for C++ Ultimate Grid Control.
		This is the class that other grids are usually
		derived from, since all of the grid's notifications
		are contained in this class.
		For simple and many complex grid applications this
		is the only class that needs to be used and worked
		with directly.
	Details
		-Generaly a derived class is created from this
		 class and used in other applications. A derived
		 class is neccessary if any of the grid's 
		 notifications are to be used
		-Being derived from CWnd allows the grid to be
		 used anywhere a CWnd can be used, plus all CWnd
		 functions are available as well.
		-This class automatically creates the grids
		  child components (grid, top heading, side heading,
		  scrollbars, tabs, and corner button)
		-This class also creates a default memory manager
		 datasource and registers the grids standard
		 cell types
		-When an instance of this class (or a class
		 derived from this) is created, a windows still
		 needs to be created (this is the same for any
		 CWnd derived class).
		 Call the classes CreateGrid to create the window.
		 Or call AttachGrid to attach the grid to a control
		 in a dialog box.
		-When this class creates or attaches to a window
		 it's OnSetup member is automatically called. 
		 This is a good spot for initial setup of the grid.
************************************************/

#ifndef CUGCTRLINC
#define CUGCTRLINC


// grid feature enable defines - rem out one or more of these defines 
// to remove one or more features
#define UG_ENABLE_MOUSEWHEEL
#define UG_ENABLE_PRINTING
#define UG_ENABLE_FINDDIALOG
#define UG_ENABLE_SCROLLHINTS


#ifdef __AFXOLE_H__  //OLE must be included
	#define UG_ENABLE_DRAGDROP
#endif

//standard grid classes
class CUGGridInfo;
class CUGGrid;
class CUGTopHdg;
class CUGSideHdg;
class CUGCnrBtn;
class CUGVScroll;
class CUGHScroll;
class CUGCtrl;
class CUGCell;
class CUGCellType;
class CUGEdit;
class CUGCellFormat;

//standard grid includes
#include "../DrCommon/UGridCtrl/Include/UGDefine.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGStruct.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGPtrLst.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGDrwHnt.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGCell.h"		//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGCnrBtn.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGGrid.h"		//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGHScrol.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGSideHd.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGTopHdg.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGVScrol.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGDtaSrc.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGMemMan.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGCelTyp.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGMultiS.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGEdit.h"		//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGTab.h"		//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGGdInfo.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/UGFormat.h"	//@Solomon090819
#ifdef UG_ENABLE_SCROLLHINTS
	#include "../DrCommon/UGridCtrl/Include/ughint.h"	//@Solomon090819
#endif 
#ifdef UG_ENABLE_PRINTING
	#include "../DrCommon/UGridCtrl/Include/UGPrint.h"	//@Solomon090819
#endif

//standard cell types
#include "../DrCommon/UGridCtrl/Include/ugLstBox.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/ugdltype.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/ugcbtype.h"	//@Solomon090819
#include "../DrCommon/UGridCtrl/Include/ugctarrw.h"	//@Solomon090819

#ifdef UG_ENABLE_DRAGDROP
	#include "../DrCommon/UGridCtrl/Include/UGDrgDrp.h"	//@Solomon090819
#endif


#undef  AFX_DATA   
#ifdef __UGRID_IMPL__
	#define AFX_DATA AFX_DATA_EXPORT
#else
	#define AFX_DATA AFX_DATA_IMPORT
#endif

/////////////////////////////////////////////////
class CUGCtrl : public CWnd
//class AFX_EXT_CLASS CUGCtrl : public CWnd
{

// Construction
public:
AFX_EXT_API		CUGCtrl();


// Attributes
public:

// Operations
public:
	AFX_EXT_API			virtual void OnSetup();
	AFX_EXT_API			virtual void OnSheetSetup(int sheetNumber);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGCtrl)
	protected:
AFX_EXT_API			virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
AFX_EXT_API			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
AFX_EXT_API			virtual ~CUGCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGCtrl)
AFX_EXT_API			afx_msg void OnSize(UINT nType, int cx, int cy);
AFX_EXT_API			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
AFX_EXT_API			afx_msg void OnPaint();
AFX_EXT_API			afx_msg void OnSetFocus(CWnd* pOldWnd);
AFX_EXT_API			afx_msg void OnSysColorChange();
AFX_EXT_API			afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
AFX_EXT_API			afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
AFX_EXT_API			afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	int m_contructorResults;

protected:
	//***** internal classes *****
	
	//data source list
	CUGDataSource		**m_dataSrcList;
	int					m_dataSrcListLength;
	int					m_dataSrcListMaxLength;

	CUGPtrList			*m_fontList;
	CUGPtrList			*m_bitmapList;
	CUGPtrList			*m_cellTypeList;
	CUGPtrList			*m_cellStylesList;
	CUGPtrList			*m_validateList;
	CUGPtrList			*m_displayFormatList;

	//standard cell types
	CUGCellType			m_normalCellType;
	CUGDropListType		m_dropListType;
	CUGCheckBoxType		m_checkBoxType;
	CUGArrowType		m_arrowType;


	#ifdef UG_ENABLE_PRINTING
	CUGPrint*			m_CUGPrint;
	#endif

	//popup menu
	CMenu				* m_menu;			
	int					m_menuCol;
	long				m_menuRow;
	int					m_menuSection;

	CUGCell m_cell;


	//grid info list / sheet variables
	CUGGridInfo ** m_GIList;
	int m_currentSheet;
	int m_numberSheets;


	//update enable/disable flag
	BOOL m_enableUpdate;


public:
	//current sheet 
	CUGGridInfo * m_GI;

	//child window classes
	CUGGrid				*m_CUGGrid;
	CUGTopHdg			*m_CUGTopHdg;
	CUGSideHdg			*m_CUGSideHdg;
	CUGCnrBtn			*m_CUGCnrBtn;

	CUGVScroll			*m_CUGVScroll;
	CUGHScroll			*m_CUGHScroll;

	//scroll hint window
	#ifdef UG_ENABLE_SCROLLHINTS
	CUGHint				*m_CUGHint;
	#endif
	
	//tabs
	CUGTab				*m_CUGTab;
	//tab sizing flag
	BOOL m_tabSizing;

	//tracking topmost window
	CWnd				*m_trackingWnd;

	//default edit control
	CUGEdit m_defEditCtrl;

	//editing
	int		m_editInProgress;		//TRUE or FALSE
	long	m_editRow;
	int		m_editCol;
	CWnd *	m_editCtrl;				//edit control currently being used
	CUGCell m_editCell;
	CWnd *	m_editParent;

	//***** other variables *****
	CPen m_threeDLightPen;
	CPen m_threeDDarkPen;


protected:
	//***** internal functions *****


AFX_EXT_API			void CalcTopRow();			//Calcs the max top row then adjusts the top row if needed
AFX_EXT_API			void CalcLeftCol();			//Calcs the max left col then adjusts the left col if needed
AFX_EXT_API			void AdjustTopRow();		//moves top row so that the current row is in view
AFX_EXT_API			void AdjustLeftCol();		//move the left col so that the current col is in view
AFX_EXT_API			void Update();				//updates all windows - also performs recalculations
AFX_EXT_API			void Moved();				//this function is called whenever a grid movement is made
								//this function then notifies the child windows
	
AFX_EXT_API			BOOL CreateChildWindows();	//creates the child windows

AFX_EXT_API			void SetLockRowHeight();
AFX_EXT_API			void SetLockColWidth();

AFX_EXT_API			void MoveTrackingWindow();

public:

	//check to see if the new position is valid
AFX_EXT_API			int VerifyTopRow(long* newRow);
AFX_EXT_API			int VerifyCurrentRow(long* newRow);
AFX_EXT_API			int VerifyLeftCol(int* newCol);
AFX_EXT_API			int VerifyCurrentCol(int* newCol);

	//*************** creation/setup *****************
	//window creation
AFX_EXT_API			BOOL CreateGrid(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	//dialog resource functions
AFX_EXT_API			AttachGrid(CWnd * wnd,UINT ID);

AFX_EXT_API			void AdjustComponentSizes();	//adjusts and positions the child windows

	//*************** editing *****************
AFX_EXT_API			int		StartEdit();
AFX_EXT_API			int		StartEdit(int key);
AFX_EXT_API			int		StartEdit(int col,long row,int key);
AFX_EXT_API			int		ContinueEdit(int adjustCol,long adjustRow);

	//*************** row and column *****************
AFX_EXT_API			int		SetNumberRows(long rows,BOOL redraw = TRUE);
AFX_EXT_API			long	GetNumberRows();
	
AFX_EXT_API			int		SetNumberCols(int cols,BOOL redraw = TRUE);
AFX_EXT_API			int		GetNumberCols();

AFX_EXT_API			int		SetDefColWidth(int width);
AFX_EXT_API			int		SetColWidth(int col,int width);
AFX_EXT_API			int		GetColWidth(int col,int *width);
AFX_EXT_API			int		GetColWidth(int col);

AFX_EXT_API			int		SetDefRowHeight(int height);
AFX_EXT_API			int		SetUniformRowHeight(int flag);
AFX_EXT_API			int		SetRowHeight(long row,int height);
AFX_EXT_API			int		GetRowHeight(long row,int *height);
AFX_EXT_API			int		GetRowHeight(long row);

AFX_EXT_API			int		GetCurrentCol();
AFX_EXT_API			long	GetCurrentRow();
AFX_EXT_API			int		GetLeftCol();
AFX_EXT_API			long	GetTopRow();
AFX_EXT_API			int		GetRightCol();
AFX_EXT_API			long	GetBottomRow();

AFX_EXT_API			int		InsertCol(int col);
AFX_EXT_API			int		AppendCol();
AFX_EXT_API			int		DeleteCol(int col);
AFX_EXT_API			int		InsertRow(long row);
AFX_EXT_API			int		AppendRow();
AFX_EXT_API			int		DeleteRow(long row);

	//************ heading functions *************
AFX_EXT_API			int		SetTH_NumberRows(int rows);
AFX_EXT_API			int		SetTH_RowHeight(int row,int height);
AFX_EXT_API			int		SetSH_NumberCols(int cols);
AFX_EXT_API			int		SetSH_ColWidth(int col,int width);


	//************* find/replace/sort *********************
AFX_EXT_API			int FindFirst(CString *string,int *col,long *row,long flags);
AFX_EXT_API			int FindNext(CString *string,int *col,long *row,int flags);

AFX_EXT_API			int FindDialog();
AFX_EXT_API			int ReplaceDialog();
AFX_EXT_API			long ProcessFindDialog(UINT,long);
AFX_EXT_API			int FindInAllCols(BOOL state);
	BOOL m_findDialogRunning;
	BOOL m_findDialogStarted;
	BOOL m_findInAllCols;
	CFindReplaceDialog *m_findReplaceDialog;


AFX_EXT_API			int SortBy(int col,int flag = UG_SORT_ASCENDING);
AFX_EXT_API			int SortBy(int *cols,int num,int flag = UG_SORT_ASCENDING);


	//************* child windows *********************
AFX_EXT_API			int		SetTH_Height(int height);
AFX_EXT_API			int		GetTH_Height();
AFX_EXT_API			int		SetSH_Width(int width);
AFX_EXT_API			int		GetSH_Width();
AFX_EXT_API			int		SetVS_Width(int width);
AFX_EXT_API			int		GetVS_Width();
AFX_EXT_API			int		SetHS_Height(int height);
AFX_EXT_API			int		GetHS_Height();

	//************* default/cell information *********************
AFX_EXT_API			int		GetCell(int col,long row,CUGCell *cell);
AFX_EXT_API			int		GetCellIndirect(int col,long row,CUGCell *cell);
AFX_EXT_API			int		SetCell(int col,long row,CUGCell *cell);
AFX_EXT_API			int		DeleteCell(int col,long row);

AFX_EXT_API			int		SetColDefault(int col,CUGCell *cell);
AFX_EXT_API			int		GetColDefault(int col,CUGCell *cell);
AFX_EXT_API			int		SetGridDefault(CUGCell *cell);
AFX_EXT_API			int		GetGridDefault(CUGCell *cell);
AFX_EXT_API			int		SetHeadingDefault(CUGCell *cell);
AFX_EXT_API			int		GetHeadingDefault(CUGCell *cell);

	//column information and translation
AFX_EXT_API			int		GetColTranslation(int col);
AFX_EXT_API			int		SetColTranslation(int col,int transCol);

	//cell joining
AFX_EXT_API			int		JoinCells(int startcol,long startrow,int endcol,long endrow);
AFX_EXT_API			int		UnJoinCells(int col,long row);
AFX_EXT_API			int		EnableJoins(BOOL state);

AFX_EXT_API			int		QuickSetText(int col,long row,LPCTSTR string);
AFX_EXT_API			int		QuickSetNumber(int col,long row,double number);
AFX_EXT_API			int		QuickSetMask(int col,long row,LPCTSTR string);
AFX_EXT_API			int		QuickSetLabelText(int col,long row,LPCTSTR string);
AFX_EXT_API			int		QuickSetTextColor(int col,long row,COLORREF color);
AFX_EXT_API			int		QuickSetBackColor(int col,long row,COLORREF color);
AFX_EXT_API			int		QuickSetAlignment(int col,long row,int align);
AFX_EXT_API			int		QuickSetBorder(int col,long row,int border);
AFX_EXT_API			int		QuickSetBorderColor(int col,long row,CPen *pen);
AFX_EXT_API			int		QuickSetFont(int col,long row,CFont * font);
AFX_EXT_API			int		QuickSetFont(int col,long row,int index);
AFX_EXT_API			int		QuickSetBitmap(int col,long row,CBitmap * bitmap);
AFX_EXT_API			int		QuickSetBitmap(int col,long row,int index);
AFX_EXT_API			int		QuickSetCellType(int col,long row,long type);
AFX_EXT_API			int		QuickSetCellTypeEx(int col,long row,long typeEx);
AFX_EXT_API			int		QuickSetHTextColor(int col,long row,COLORREF color);
AFX_EXT_API			int		QuickSetHBackColor(int col,long row,COLORREF color);

AFX_EXT_API			int		QuickSetRange(int startCol,long startRow,int endCol,long endRow,
				CUGCell* cell);

AFX_EXT_API			int			QuickGetText(int col,long row,CString *string);
AFX_EXT_API			LPCTSTR		QuickGetText(int col,long row);


	//************* general modes and settings *********************
AFX_EXT_API			int	SetCurrentCellMode(int mode);	//focus and highlighting
											//bit 1:focus 2:hightlight
AFX_EXT_API			int	SetHighlightRow(int mode);		//on,off

//# danny : 2005.09.23
AFX_EXT_API			int	SetHighlightRow(int mode, BOOL bExtend);

AFX_EXT_API			int	SetMultiSelectMode(int mode);	//on,off
AFX_EXT_API			int	Set3DHeight(int height);		//in pixels
AFX_EXT_API			int	SetPaintMode(int mode);			//on,off
AFX_EXT_API			int	GetPaintMode();
AFX_EXT_API			int	SetVScrollMode(int mode);		// 0-normal 2- tracking 3-joystick
AFX_EXT_API			int	SetHScrollMode(int mode);		// 0-normal 2- tracking
	
AFX_EXT_API			int HideCurrentCell();

AFX_EXT_API			int SetBallisticMode(int mode);	// 0:off  1:on  2:on-squared 3:on-cubed
AFX_EXT_API			int SetBallisticDelay(int milisec);

AFX_EXT_API			int SetBallisticKeyMode(int mode); //0-off n-number of key repeats before increase
AFX_EXT_API			int SetBallisticKeyDelay(int milisec);
	
AFX_EXT_API			int SetDoubleBufferMode(int mode); // 0:off 1:on

AFX_EXT_API			int LockColumns(int numCols);		
AFX_EXT_API			int	LockRows(int numRows);

AFX_EXT_API			int	EnableCellOverLap(BOOL state);

AFX_EXT_API			int EnableColSwapping(BOOL state);

AFX_EXT_API			int EnableExcelBorders(BOOL state);


	//************* movement *********************
AFX_EXT_API			int MoveTopRow(int flag);	//0-lineup 1-linedown 2-pageup 3-pagedown 4-top 5-bottom
AFX_EXT_API			int AdjustTopRow(long adjust);
AFX_EXT_API			int MoveCurrentRow(int flag);
AFX_EXT_API			int AdjustCurrentRow(long adjust);
AFX_EXT_API			int GotoRow(long row);
AFX_EXT_API			int	SetTopRow(long row);

AFX_EXT_API			int MoveLeftCol(int flag);
AFX_EXT_API			int AdjustLeftCol(int adjust);
AFX_EXT_API			int MoveCurrentCol(int flag);
AFX_EXT_API			int AdjustCurrentCol(int adjust);
AFX_EXT_API			int GotoCol(int col);
AFX_EXT_API			int	SetLeftCol(int col);

AFX_EXT_API		int GotoCell(int col,long row);

	//************* finding cells *********************
AFX_EXT_API			int GetCellFromPoint(int x,int y,int *col,long *row);
AFX_EXT_API			int GetCellFromPoint(int x,int y,int *ptcol,long *ptrow,RECT *rect);
AFX_EXT_API			int GetAbsoluteCellFromPoint(int x,int y,int *ptcol,long *ptrow);

AFX_EXT_API			int GetCellRect(int col,long row,RECT *rect);
AFX_EXT_API			int GetCellRect(int *col,long *row,RECT *rect);
AFX_EXT_API			int GetRangeRect(int startCol,long startRow,int endCol,long endRow,RECT *rect);
	
AFX_EXT_API			int GetJoinStartCell(int *col,long *row);
AFX_EXT_API			int GetJoinStartCell(int *col,long *row,CUGCell *cell);
AFX_EXT_API			int GetJoinRange(int *col,long *row,int *col2,long *row2);


	//************* cell types *********************
AFX_EXT_API			long AddCellType(CUGCellType *);   //returns the cell type ID
AFX_EXT_API			int RemoveCellType(int ID);
AFX_EXT_API			CUGCellType * GetCellType(int type);	//returns the pointer to a cell type
AFX_EXT_API			int GetCellType(CUGCellType * type);

	//************* data sources *********************
AFX_EXT_API			int AddDataSource(CUGDataSource * ds);
AFX_EXT_API			CUGDataSource * GetDataSource(int index);
AFX_EXT_API			int RemoveDataSource(int index);
AFX_EXT_API			int SetDefDataSource(int index);
AFX_EXT_API			int GetDefDataSource();
AFX_EXT_API			int SetGridUsingDataSource(int index);

	//************* fonts *********************
AFX_EXT_API			int AddFont(LPCTSTR fontName,int height,int weight);
AFX_EXT_API			int AddFont(int height,int width,int escapement,int orientation, 
			int weight,BYTE italic,BYTE underline,BYTE strikeOut, 
			BYTE charSet,BYTE outputPrecision,BYTE clipPrecision, 
			BYTE quality,BYTE pitchAndFamily,LPCTSTR fontName);
AFX_EXT_API			int RemoveFont(int index);
AFX_EXT_API			int ClearAllFonts();
AFX_EXT_API			CFont * GetFont(int index);
AFX_EXT_API			int SetDefFont(CFont *font);
AFX_EXT_API			int SetDefFont(int index);

	//************* bitmaps *********************
AFX_EXT_API			int AddBitmap( UINT resourceID,LPCTSTR resourceName= NULL);
AFX_EXT_API			int AddBitmap( LPCTSTR fileName);
AFX_EXT_API			int RemoveBitmap(int index);
AFX_EXT_API			int ClearAllBitmaps();
AFX_EXT_API			CBitmap* GetBitmap(int index);

	//************* redrawing *********************
AFX_EXT_API			int RedrawAll();
AFX_EXT_API			int RedrawCell(int col,long row);
AFX_EXT_API			int RedrawRow(long row);
AFX_EXT_API			int RedrawCol(int col);
AFX_EXT_API			int RedrawRange(int startCol,long startRow,int endCol,long endRow);

	//************* multi-select *********************
AFX_EXT_API			int ClearSelections();
AFX_EXT_API			int Select(int col,long row);
AFX_EXT_API			int SelectRange(int startCol,long startRow,int endCol,long endRow);
AFX_EXT_API			int EnumFirstSelected(int *col,long *row);
AFX_EXT_API			int EnumNextSelected(int *col,long *row);
AFX_EXT_API			int EnumFirstBlock(int *startCol,long *startRow,int *endCol,long *endRow);
AFX_EXT_API			int EnumNextBlock(int *startCol,long *startRow,int *endCol,long *endRow);
AFX_EXT_API			BOOL IsSelected(int col,long row,int *blockNum = NULL);

	//************* clipboard ********************
AFX_EXT_API			int CopySelected();
AFX_EXT_API			int CutSelected();
AFX_EXT_API			int CopySelected(int cutflag);  //TRUE,FALSE
AFX_EXT_API			int Paste();
AFX_EXT_API			int Paste(CString &string);
AFX_EXT_API			int CopyToClipBoard(CString* string);
AFX_EXT_API			int CopyFromClipBoard(CString* string);
AFX_EXT_API			void CreateSelectedString(CString& string,int cutFlag);

	//************* column sizing ********************
AFX_EXT_API			int FitToWindow(int startCol,int endCol);
AFX_EXT_API			int BestFit(int startCol,int endCol,int CalcRange,int flag);

	//************* printing ********************
	#ifdef UG_ENABLE_PRINTING
AFX_EXT_API			int PrintInit(CDC * pDC, CPrintDialog* pPD, int startCol,long startRow,
					int endCol,long endRow);
AFX_EXT_API			int PrintPage(CDC * pDC, int pageNum);
AFX_EXT_API			int PrintSetMargin(int whichMargin,int size);
AFX_EXT_API			int PrintSetScale(double scale);
AFX_EXT_API			int PrintSetOption(int option,long param);
AFX_EXT_API			int PrintGetOption(int option,long *param);
	#endif


	//************* hints ********************
AFX_EXT_API			int UseHints(BOOL state);
AFX_EXT_API			int UseVScrollHints(BOOL state);
AFX_EXT_API			int UseHScrollHints(BOOL state);


	//************* pop-up menu ********************
AFX_EXT_API			CMenu * GetPopupMenu();
AFX_EXT_API			int EmptyMenu();
AFX_EXT_API			int AddMenuItem(int ID,LPCTSTR string);
AFX_EXT_API			int RemoveMenuItem(int ID);
AFX_EXT_API			int EnableMenu(BOOL state);


	//************* drag and drop ********************
	#ifdef UG_ENABLE_DRAGDROP
		COleDataSource	m_dataSource;
		CUGDropTarget	m_dropTarget;

AFX_EXT_API				int StartDragDrop();
AFX_EXT_API				int DragDropTarget(BOOL state);
	#endif


	//************* tabs ********************
AFX_EXT_API			int AddTab(LPCTSTR label,long ID);
AFX_EXT_API			int InsertTab(int pos,LPCTSTR label,long ID);
AFX_EXT_API			int DeleteTab(long ID);
AFX_EXT_API			int SetTabBackColor(long ID,COLORREF color);
AFX_EXT_API			int SetTabTextColor(long ID,COLORREF color);
AFX_EXT_API			int SetTabWidth(int width);
AFX_EXT_API			int SetCurrentTab(long ID);
AFX_EXT_API			int GetCurrentTab();


	//************* sheets ********************
AFX_EXT_API			int SetNumberSheets(int numSheets);
AFX_EXT_API			int GetNumberSheets();
AFX_EXT_API			int SetSheetNumber(int index,BOOL update = TRUE);
AFX_EXT_API			int GetSheetNumber();

	//****************************************************
	//*********** Over-ridable Notify Functions **********
	//****************************************************
//virtual void OnSetup();
//AFX_EXT_API			virtual void OnSheetSetup(int sheetNumber);

	//movement and sizing
AFX_EXT_API			virtual int  OnCanMove(int oldcol,long oldrow,int newcol,long newrow);
AFX_EXT_API			virtual int  OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow);
AFX_EXT_API			virtual void OnHitBottom(long numrows,long rowspast,long rowsfound);
AFX_EXT_API			virtual void OnHitTop(long numrows,long rowspast);
	
AFX_EXT_API			virtual int  OnCanSizeCol(int col);
AFX_EXT_API			virtual void OnColSizing(int col,int *width);
AFX_EXT_API			virtual void OnColSized(int col,int *width);
AFX_EXT_API			virtual int  OnCanSizeRow(long row);
AFX_EXT_API			virtual void OnRowSizing(long row,int *height);
AFX_EXT_API			virtual void OnRowSized(long row,int *height);

AFX_EXT_API			virtual int  OnCanSizeTopHdg();
AFX_EXT_API			virtual int  OnCanSizeSideHdg();
AFX_EXT_API			virtual int  OnTopHdgSizing(int *height);
AFX_EXT_API			virtual int  OnSideHdgSizing(int *width);
AFX_EXT_API			virtual int  OnTopHdgSized(int *height);
AFX_EXT_API			virtual int  OnSideHdgSized(int *width);
	
AFX_EXT_API			virtual void OnColChange(int oldcol,int newcol);
AFX_EXT_API			virtual void OnRowChange(long oldrow,long newrow);
AFX_EXT_API			virtual void OnCellChange(int oldcol,int newcol,long oldrow,long newrow);
AFX_EXT_API			virtual void OnLeftColChange(int oldcol,int newcol);
AFX_EXT_API			virtual void OnTopRowChange(long oldrow,long newrow);

	//mouse and key strokes
AFX_EXT_API			virtual void OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);
AFX_EXT_API			virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);
AFX_EXT_API			virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
AFX_EXT_API			virtual void OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed=0);
AFX_EXT_API			virtual void OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
AFX_EXT_API			virtual void OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
AFX_EXT_API			virtual void OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed=0);
AFX_EXT_API			virtual void OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
AFX_EXT_API			virtual void OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed=0);
AFX_EXT_API			virtual void OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed=0);
AFX_EXT_API			virtual void OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed=0);
AFX_EXT_API			virtual void OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed=0);
AFX_EXT_API			virtual void OnCB_DClicked(RECT *rect,POINT *point,BOOL processed=0);
	
AFX_EXT_API			virtual void OnKeyDown(UINT *vcKey,BOOL processed);
AFX_EXT_API			virtual void OnCharDown(UINT *vcKey,BOOL processed);
	
	//GetCellIndirect notification
AFX_EXT_API			virtual void OnGetCell(int col,long row,CUGCell *cell);

	//SetCell notification
AFX_EXT_API			virtual void OnSetCell(int col,long row,CUGCell *cell);
	
	//data source notifications
AFX_EXT_API			virtual void OnDataSourceNotify(int ID,long msg,long param);

	//cell type notifications
AFX_EXT_API			virtual int OnCellTypeNotify(long ID,int col,long row,long msg,long param);

	//editing
AFX_EXT_API			virtual int OnEditStart(int col, long row,CWnd **edit);
AFX_EXT_API			virtual int OnEditVerify(int col,long row,CWnd *edit,UINT *vcKey);
AFX_EXT_API			virtual int OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
AFX_EXT_API			virtual int OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow);

	//menu notifications
AFX_EXT_API			virtual void OnMenuCommand(int col,long row,int section,int item);
AFX_EXT_API			virtual int  OnMenuStart(int col,long row,int section);

	//hints
AFX_EXT_API			virtual int OnHint(int col,long row,int section,CString *string);
AFX_EXT_API			virtual int OnVScrollHint(long row,CString *string);
AFX_EXT_API			virtual int OnHScrollHint(int col,CString *string);

	//drag and drop
	#ifdef UG_ENABLE_DRAGDROP
AFX_EXT_API				virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject);
AFX_EXT_API				virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject,int col,long row);
AFX_EXT_API				virtual DROPEFFECT OnDragDrop(COleDataObject* pDataObject,int col,long row);
	#endif

	//sorting
AFX_EXT_API			virtual int  OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);
	
	//DC setup
AFX_EXT_API			virtual void OnScreenDCSetup(CDC *dc,CDC *db_dc,int section);
	
AFX_EXT_API			virtual void OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
		RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs);

AFX_EXT_API			virtual void OnTabSelected(int ID);

AFX_EXT_API			virtual COLORREF OnGetDefBackColor(int section);

	//focus rect setup
AFX_EXT_API			virtual void OnDrawFocusRect(CDC *dc,RECT *rect);

AFX_EXT_API			virtual void OnSetFocus(int section);
AFX_EXT_API			virtual void OnKillFocus(int section);
	
AFX_EXT_API			void DrawExcelFocusRect(CDC *dc,RECT *rect);
		
AFX_EXT_API		int StartMenu(int col,long row,POINT *point,int section);

AFX_EXT_API			int SetArrowCursor(HCURSOR cursor);
AFX_EXT_API			int SetWESizingCursor(HCURSOR cursor);
AFX_EXT_API			int SetNSSizingCursor(HCURSOR cursor);

AFX_EXT_API			int SetMargin(int pixels);

	//functions to be called by grid edit controls
AFX_EXT_API			int EditCtrlFinished(LPCTSTR string,BOOL cancelFlag,
		BOOL continueFlag,int continueCol,long continueRow);

AFX_EXT_API			int EditCancel();

AFX_EXT_API			int SetCancelMode( BOOL bCancel );
AFX_EXT_API			BOOL GetCancelMode();

AFX_EXT_API			CUGCellType * GetCellType(int col,long row);

AFX_EXT_API			virtual BOOL OnColSwapStart(int col);
AFX_EXT_API			virtual BOOL OnCanColSwap(int fromCol,int toCol);

AFX_EXT_API			int MoveColPosition(int fromCol,int toCol,BOOL insertBefore);

AFX_EXT_API			int SetNewTopHeadingClass(CUGTopHdg * topHeading);
AFX_EXT_API			int SetNewSideHeadingClass(CUGSideHdg * sideHeading);
AFX_EXT_API			int SetNewGridClass(CUGGrid * grid);
AFX_EXT_API			int SetNewMultiSelectClass(CUGMultiSelect * multiSelect);
AFX_EXT_API			int SetNewTabClass(CUGTab * tab);
AFX_EXT_API			int SetNewVScrollClass(CUGVScroll * scroll);
AFX_EXT_API			int SetNewHScrollClass(CUGHScroll * scroll);


AFX_EXT_API			int SetNewEditClass(CWnd * edit);
AFX_EXT_API			CWnd * GetEditClass();


	//overlay objects
AFX_EXT_API			int AddOverlayObject(CWnd *wnd,int startCol,int startRow,int endCol,int endRow);
AFX_EXT_API			int SetOverlayObjectPos(int index,int startCol,int startRow,int endCol,int endRow,
		int leftOffset=0,int topOffset=0,int rightOffset=0,int bottomOffset=0);
AFX_EXT_API			int GetOverlayObjectPos(int index,int* startCol,int* startRow,int* endCol,int* endRow,
		int* leftOffset,int* topOffset,int* rightOffset,int* bottomOffset);
AFX_EXT_API			int GetOverlayObjectPos(int index,int* startCol,int* startRow,int* endCol,int* endRow);
AFX_EXT_API			int RemoveOverlayObject(int index);
AFX_EXT_API			CWnd * GetOverlayObject(int index);

	//tracking window
AFX_EXT_API			int SetTrackingWindow(CWnd *wnd);
AFX_EXT_API			int SetTrackingWindowMode(int mode);
AFX_EXT_API			virtual void OnTrackingWindowMoved(RECT *origRect,RECT *newRect);

AFX_EXT_API			int EnableUpdate(BOOL state);

AFX_EXT_API			int SetUserSizingMode(BOOL state);

AFX_EXT_API			int SetColDataSource(int col,int dataSrcIndex);
AFX_EXT_API			int SetColDataSource(int col,CUGDataSource * dataSrc);

AFX_EXT_API			static BOOL CALLBACK ModifyDlgItemText(HWND hWnd, LPARAM lParam);
};


#undef  AFX_DATA   
#define AFX_DATA
/////////////////////////////////////////////////////////////////////////////

#endif

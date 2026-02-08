
#ifndef __PRINT_4_GRID__
#define	__PRINT_4_GRID__
////////////////////////////////////////////////
//	20070703 이문수
//	그리드 프린트 작업을 위해 새로 파일 추가
////////////////////////////////////////////////
#include "Grid_Result.h"

class CPrint4Grid{

public:

	typedef struct CUGPrintListTag{	
		int		page;
		int		startCol;
		int		endCol;
		long	startRow;
		long	endRow;
		CUGPrintListTag * next;
	}CUGPrintList;

	CUGPrintList*	m_printList;

	int				m_pagesWide;
	int				m_pagesHigh;

	float			m_printVScale;
	float			m_printHScale;
	
	int				m_printLeftMarginPX;
	int				m_printTopMarginPX;
	int				m_printRightMarginPX;
	int				m_printBottomMarginPX;
	
	int				m_paperWidthPX;
	int				m_paperHeightPX;


	//print options
	int	m_printLeftMargin;	//margin in mm
	int	m_printTopMargin;
	int	m_printRightMargin;
	int	m_printBottomMargin;
	
	BOOL m_printFrame;		//print a frame(border) around the grid
	BOOL m_printCenter;		//center the grid within the margins
	BOOL m_fitToPage;		//fit the range of cells to one page
	BOOL m_printTopHeading;	//print the top heading
	BOOL m_printSideHeading;//print the side heading

	
	double m_scaleMultiplier;


	int ClearPrintList();
	int	AddPageToList(int page,int startCol,int endCol,long startRow,long endRow);
	int	GetPageFromList(int page,int *startCol,int *endCol,long *startRow,long *endRow);

	void InternPrintCell(CDC *dc,RECT * rect,int col,long row);

public:
	
	CGrid_Result *	m_ctrl;
	CUGGridInfo *	m_GI;

	CPrint4Grid();
	~CPrint4Grid();

	int PrintInit(CDC * pDC, CPrintDialog* pPD, int startCol,long startRow,
				int endCol,long endRow);
	int PrintPage(CDC * pDC, int pageNum);
	int PrintSetMargin(int whichMargin,int size);
	int PrintSetScale(double scale);
	int PrintSetOption(int option,long param);
	int PrintGetOption(int option,long *param);
};

#endif
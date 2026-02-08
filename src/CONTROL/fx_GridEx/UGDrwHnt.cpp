/*************************************************************************
				Class Implementation : CUGDrawHint
**************************************************************************
	Source file : UGDrwHnt.cpp
	Copyright ? Dundas Software Ltd. 1994 - 2002, All Rights Reserved
*************************************************************************/
#include "stdafx.h"
#include "UGCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***************************************************
	Standard construction/desrtuction
***************************************************/
CUGDrawHint::CUGDrawHint()
{
	m_List					= nullptr;
	m_VList					= nullptr;

}

CUGDrawHint::~CUGDrawHint()
{
	ClearHints();
}

/***************************************************
AddHint
	function is called to add a single cell to the 
	redraw list.
Params:
	col, row	- coordinates of the cell to redraw
Returns:
	<none>
*****************************************************/
void CUGDrawHint::AddHint(int col,long row)
{
	AddHint(col,row,col,row);	
}

/***************************************************
AddHint
	function is called to add a block of cells to the 
	redraw list.
Params:
	startCol,	- parameters passed in indicate the 
	startRow,	  range of cells that need to be added
	endCol,		  to the redraw list.
	endRow
Returns:
	<none>
*****************************************************/
void CUGDrawHint::AddHint(int startCol,long startRow,int endCol,long endRow)
{
	
	UGDrwHintList	*next = m_List;
	UGDrwHintList	*newhint = new UGDrwHintList;
		
	if(m_List != nullptr)
	{
		while(next->next != nullptr)
		{
			next = next->next;
		}
		next->next = newhint;
	}
	else
	{
		m_List = newhint;
		m_minCol = startCol;
		m_minRow = startRow;
		m_maxCol = endCol;
		m_maxRow = endRow;
	}
	
	newhint->next = nullptr;
	newhint->startCol = startCol;
	newhint->startRow = startRow;
	newhint->endCol = endCol;
	newhint->endRow = endRow;

	if(startCol < m_minCol)
		m_minCol = startCol;
	if(endCol > m_maxCol)
		m_maxCol = endCol;
	if(startRow < m_minRow)
		m_minRow = startRow;
	if(endRow > m_maxRow)
		m_maxRow = endRow;
}

/***************************************************
AddToHint
	function is almost identical to the AddHint with only one addition, 
	it updates the member UGDrwHintList (m_List) object to contain
	total redraw range in the head of the list.
Params:
	col, row	- coordinates of the cell to redraw
Returns:
	<none>
*****************************************************/
void CUGDrawHint::AddToHint(int col,long row)
{
		AddToHint(col,row,col,row);	
}

/***************************************************
AddToHint
	function is almost identical to the AddHint with only one addition, 
	it updates the member UGDrwHintList (m_List) object to contain
	total redraw range in the head of the list.
Params:
	startCol,	- parameters passed in indicate the 
	startRow,	  range of cells that need to be added
	endCol,		  to the redraw list.
	endRow
Returns:
	<none>
*****************************************************/
void CUGDrawHint::AddToHint(int startCol,long startRow,int endCol,long endRow)
{
	if(m_List == nullptr)
		AddHint(startCol,startRow,endCol,endRow);	

	if(m_List->startCol > startCol)
		m_List->startCol = startCol;
	if(m_List->endCol < endCol)
		m_List->endCol = endCol;
	if(m_List->startRow > startRow)
		m_List->startRow = startRow;
	if(m_List->endRow < endRow)
		m_List->endRow = endRow;
}

/***************************************************
ClearHints
	function is called to remove all entries
	in the draw hint list.
Params:
	<none>
Returns:
	<none>
*****************************************************/
void CUGDrawHint::ClearHints()
{
	
	//clear the Invalid list
	UGDrwHintList	*current = m_List;
	UGDrwHintList	*next;
		
	while(current != nullptr)
	{
		next = current->next;
		delete current;
		current = next;
	}
	m_List = nullptr;

	//clear the valid list
	UGDrwHintVList	*currentV = m_VList;
	UGDrwHintVList	*nextV;
		
	while(currentV != nullptr)
	{
		nextV = currentV->next;
		delete currentV;
		currentV = nextV;
	}
	m_VList = nullptr;
}

/***************************************************
IsInvalid
	function is called to determine if a given cell
	is invalid, and has to be redrawn.
Params:
	col, row	- coordinates of a cell in question
Returns:
	TRUE		- if the cell is invalid
	FALSE		- if failed to locate the cell
*****************************************************/
BOOL CUGDrawHint::IsInvalid(int col,long row)
{
	if(m_List == nullptr)
		return FALSE;

	if(col < m_minCol || col > m_maxCol)
		return FALSE;
	if(row < m_minRow || row > m_maxRow)
		return FALSE;

	UGDrwHintList	*current = m_List;

	//check the invalid list
	while(current != nullptr)
	{
		//check to see if the item in the list covers a greater range
		if(col >= current->startCol && col <= current->endCol)
		{
			if(row >= current->startRow && row <= current->endRow)
			{
				return TRUE;
			}
		}		
		current = current->next;
	}	

	//if the item is not in the invalid list assume it is valid
	return FALSE;
}

/***************************************************
IsValid
	function is called to determine if a given cell
	is valid, and does not have to be redrawn.
Params:
	col, row	- coordinates of a cell in question
Returns:
	TRUE		- if the cell is valid
	FALSE		- if failed to locate the cell
*****************************************************/
BOOL CUGDrawHint::IsValid(int col,long row)
{
	if(m_VList == nullptr)
		return FALSE;

	UGDrwHintVList	*currentV = m_VList;
	UGDrwHintVList	*nextV;
		
	//check the valid list
	while(currentV != nullptr)
	{
		nextV = currentV->next;
		if(col == currentV->Col && row == currentV->Row)
			return TRUE;
		currentV = nextV;
	}
	return FALSE;
}

/***************************************************
GetTotalRange
	function is called to retrieve the total redraw
	range.
Params:
	startCol,	- parameters passed in are used to
	startRow,	  return the entire range of cells to
	endCol,		  redraw.
	endRow
Returns:
	FALSE		- if the list is empty
	TRUE		- on success
*****************************************************/
int CUGDrawHint::GetTotalRange(int *startCol,long *startRow,int *endCol,long *endRow)
{
	if(m_List == nullptr)
	{
		*startCol = 0;	//put in def values just in case the return
		*startRow = 0;	//value is not checked
		*endCol = 0;
		*endRow = 0;
		return FALSE;
	}

	*startCol = m_List->startCol;
	*startRow = m_List->startRow;
	*endCol = m_List->endCol;
	*endRow = m_List->endRow;

	UGDrwHintList	*current = m_List;
	UGDrwHintList	*next;
		
	while(current != nullptr)
	{
		next = current->next;
	
		//check to see if the item in the list covers a greater range
		if(*startCol > current->startCol)
			*startCol = current->startCol;
		if(*startRow > current->startRow)
			*startRow = current->startRow;
		if(*endCol < current->endCol)
			*endCol = current->endCol;
		if(*endRow < current->endRow)
			*endRow = current->endRow;
		
		current = next;
	}
	
	return TRUE;
}

/***************************************************
SetAsValid
	function is called to validate a cell.
Params:
	col, row	- coordinates of a cell to validate
Returns:
	<none>
*****************************************************/
void CUGDrawHint::SetAsValid(int col,long row)
{
	UGDrwHintVList	*nextV = m_VList;
	UGDrwHintVList	*newhintV = new UGDrwHintVList;
		
	if(m_VList != nullptr)
	{
		while(nextV->next != nullptr)
		{
			nextV = nextV->next;
		}
		nextV->next = newhintV;
	}
	else
		m_VList = newhintV;
	
	newhintV->next = nullptr;
	newhintV->Col = col;
	newhintV->Row = row;
}
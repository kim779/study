
#include "stdafx.h"
#include "SortStringArray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//CSortStringArray::CSortStringArray()
//{
//}
//CSortStringArray::~CSortStringArray()
//{
//}

void CSortStringArray::Sort() 
{ 
    BOOL bNotDone = TRUE; 
    
    while (bNotDone) 
    { 
        bNotDone = FALSE; 
        for(int pos = 0;pos < GetUpperBound();pos++) 
            bNotDone |= CompareAndCStringSwap(pos); 
    } 
} 

BOOL CSortStringArray::CompareAndCStringSwap(int pos) 
{ 
    CString vc_b; 
    int posFirst = pos; 
    int posNext = pos + 1; 
    
    if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0) 
    { 
        vc_b = GetAt(posFirst); 
        SetAt(posFirst, GetAt(posNext)); 
        SetAt(posNext, vc_b); 
        return TRUE; 
    } 
    return FALSE;     
} 

#if !defined(_SORT_STRING_ARRAY_H)
#define _SORT_STRING_ARRAY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSortStringArray : public CStringArray 
{ 
public: 
	void Sort(); 
private: 
    BOOL CompareAndCStringSwap(int pos);     
}; 

#endif

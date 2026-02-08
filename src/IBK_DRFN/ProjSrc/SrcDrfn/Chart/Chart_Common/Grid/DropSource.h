#ifndef __DROPSOURCE
#define __DROPSOURCE

#include "afxole.h" 

class CDropSource : protected COleDataSource
{
public:
	CDropSource();
	virtual ~CDropSource();
	virtual DROPEFFECT StartDragging( DWORD Data, RECT* rClient, CPoint* MousePos);
protected:
	virtual void CompleteMove() {};
private:
};

#endif

// #include "LineObj.h"
#include "PeriodObj.h"

class CLineList : public CObList
{
public:
	int GetLineCount();				
	void DeleteAll();
	void AddHeadAgent(CLineObj* pLineObject/*IN*/)
				{ AddHead(pLineObject); }
	void SetAtLine(POSITION pos/*IN*/, CLineObj* pLineObject/*IN*/)
				{ SetAt(pos, pLineObject);}
	CLineObj* GetHeadAgent()
				{ return (CLineObj*)GetHead(); }
	CLineObj* GetNextLine(POSITION& pos/*IN*/)
				{ return (CLineObj*)GetNext(pos); }
	CLineObj* GetPrevLine(POSITION& pos/*IN*/)
				{ return (CLineObj*)GetPrev(pos); }

	int GetLineCount2();				
	void DeleteAll2();
	void AddHeadAgent2(CPeriodObj* pPeriodObj/*IN*/)
				{ AddHead(pPeriodObj); }
	void SetAtLine(POSITION pos/*IN*/, CPeriodObj* pPeriodObj/*IN*/)
				{ SetAt(pos, pPeriodObj);}
	CPeriodObj* GetHeadAgent2()
				{ return (CPeriodObj*)GetHead(); }
	CPeriodObj* GetNextLine2(POSITION& pos/*IN*/)
				{ return (CPeriodObj*)GetNext(pos); }
	CPeriodObj* GetPrevLine2(POSITION& pos/*IN*/)
				{ return (CPeriodObj*)GetPrev(pos); }


};
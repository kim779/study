// LogicItem.h: interface for the CLogicItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICITEM_H__C4A5E091_1CA4_4ED5_A52C_3F770009D295__INCLUDED_)
#define AFX_LOGICITEM_H__C4A5E091_1CA4_4ED5_A52C_3F770009D295__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
enum {
	DISPIID_ADD = 0,
	DISPIID_SUB,
	DISPIID_MUL,
	DISPIID_DIV,
	DISPIID_AND,
	DISPIID_OR,
	DISPIID_NEXT
};

class CLogicOperation;
class CLogicItem  
{
public:
	virtual ~CLogicItem();
	virtual double GetValue() {return 0;}
	virtual void SetIndex(long lIndex) {}

	CLogicOperation* GetParent() {return m_pLogicItemParent;}
protected:
	friend class CLogicOperation;
	void SetParent(CLogicOperation* pParent) { m_pLogicItemParent = pParent;}
	CLogicItem();
private:
	CLogicOperation* m_pLogicItemParent;
};

#endif // !defined(AFX_LOGICITEM_H__C4A5E091_1CA4_4ED5_A52C_3F770009D295__INCLUDED_)

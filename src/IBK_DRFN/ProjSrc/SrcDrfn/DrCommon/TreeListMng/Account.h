#ifndef _MY_ACCOUNT_H_
#define _MY_ACCOUNT_H_

#include <afxtempl.h>

typedef struct _scode {
	CString strAcc;
	CString strCode;
	CString strName;
} scode;

class ACCOUNT {
public:
	ACCOUNT();
	~ACCOUNT();

	void load_acc( CString str );
	CString loadPass(CString dat, CString acct);

	CStringArray* get_account() { return &m_arrAccCode; }

protected:

	CStringArray				m_arrAccCode; // 내계좌 리스트 
	CList<scode,scode>			m_listCode;
};



#endif
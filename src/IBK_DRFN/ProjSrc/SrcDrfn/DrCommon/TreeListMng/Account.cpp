// Account.cpp: implementation of the CAccount class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Account.h"
#include "stdafx.h"

#include "account.h"

ACCOUNT::ACCOUNT()
{
}

ACCOUNT::~ACCOUNT()
{
}

void ACCOUNT::load_acc(CString str)
{
	m_arrAccCode.RemoveAll();
	
	int idx, cur = 0;
	while( TRUE )
	{
		idx = str.Find( "\n", cur );
		if( idx < 0 )
			break;
		
		CString strTemp;
		
		strTemp = str.Mid( cur, idx-cur );
		m_arrAccCode.Add( strTemp.Left(11) );
		cur = idx + 1;
	}
}

CString ACCOUNT::loadPass(CString dat, CString acct)
{
	CString	accS, pswdS, tmps;
	CString result = "";

	while (!dat.IsEmpty())
	{
		int	pos = dat.Find('\n');
		if (pos == -1)	break;
		tmps = dat.Left(pos++);
		dat = dat.Mid(pos);
		
		pos = tmps.Find('\t');
		if (pos == -1)	break;
		accS = tmps.Left(pos++);
		tmps = tmps.Mid(pos);
		
		pos = tmps.Find('\t');
		if (pos == -1)	break;
		pswdS = tmps.Left(pos++);
		tmps = tmps.Mid(pos);
		


		if(acct == accS)
		{
			result = pswdS;
		}
	}

	return result;
}
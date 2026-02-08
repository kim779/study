// Account.h: interface for the CAccount class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CAccount  
{
public:
	CString m_strPriAcc;		//대표계좌구분
	CString m_strSortNum;		//계좌일련번호
	CString m_strAccntNum;		//계좌번호
	CString m_strAccntName;		//계좌이름
	CString	m_strPassword;		//계좌비번
	CString m_strAllocRate;		//배정비율
	CString	m_strAllocMulti;	//승수
	CString m_strAccntNick;		//계좌별칭
	BOOL	m_bDelegation;		//위임계좌여부
	BOOL	m_bWrap;			//Wrap계좌여부
	BOOL	m_bAgent5;			//2013.03.12 KSJ 금융상품매매대리 설정여부
	BOOL	m_bAgent6;			//2013.03.12 KSJ 종합계좌간자금대체대리인 설정여부
public:
	CAccount()
	{
		m_strPriAcc = _T("");
		m_strSortNum = _T("");
		m_strAccntNum = _T("");
		m_strAccntName = _T("");
		m_strPassword = _T("");
		m_strAllocRate = _T("");
		m_strAllocMulti = _T("");
		m_strAccntNick = _T("");
		m_bDelegation = FALSE;
		m_bWrap = FALSE;
		m_bAgent5 = FALSE;
		m_bAgent6 = FALSE;
	};
	
	CAccount(CAccount const &other)
	{
		Copy(other);
	};
	
	~CAccount() {}
	
	inline void Copy(CAccount const &other)
	{
		m_strPriAcc = other.m_strPriAcc;
		m_strSortNum = other.m_strSortNum;
		m_strAccntNum = other.m_strAccntNum;
		m_strAccntName = other.m_strAccntName;
		m_strPassword = other.m_strPassword;
		m_strAllocRate = other.m_strAllocRate;
		m_strAllocMulti = other.m_strAllocMulti;
		m_strAccntNick = other.m_strAccntNick;
		m_bDelegation = other.m_bDelegation;
		m_bWrap = other.m_bWrap;
		m_bAgent5 = other.m_bAgent5;
		m_bAgent6 = other.m_bAgent6;
	};
	
	inline void TrimData()
	{
		m_strPriAcc.TrimRight();
		m_strSortNum.TrimRight();
		m_strAccntNum.TrimRight();
		m_strAccntName.TrimRight();
		m_strPassword.TrimRight();
		m_strAllocRate.TrimRight();
		m_strAllocMulti.TrimRight();
		m_strAccntNick.TrimRight();
	}
};
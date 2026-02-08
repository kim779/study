#if !defined(AFX_CHARTREALDEFINE_H__A82B8802_19BA_49a7_AA4C_94E1AA76C9D5__INCLUDED_)
#define AFX_CHARTREALDEFINE_H__A82B8802_19BA_49a7_AA4C_94E1AA76C9D5__INCLUDED_

class CRealData_Common
{
public:
	CString m_strCode;		// [종목코드]
	CString m_strTime;		// [체결시간]
//@solomon	현지시간(확인되면 다시 셋팅..)
//	CString m_strTime2;		// [현지체결시간]
	CString m_strSign;		// [1:상한]
	CString m_strPrice;		// [현재가,     원]
	CString m_strChange;	// [전일대비,   원]
	CString m_strChrate;	// [등락율]
	CString m_strVolume;	// [누적거래량, 주]

	CString m_strOpen;		// [시가,       원]
	CString m_strHigh;		// [고가,       원]
	CString m_strLow;		// [저가,       원]

	CString m_strValue;		// [거래대금,       원]

	int		m_nMarketType;
	BOOL	m_bIsRealJang;	// 장시작 여부 (TRUE : 장시간, FALSE : 시간외, 장전등..)

	CString	m_strOpenyak;
};

class CRealData_Common_FX
{
public:
	CString m_strCode;						// [종목코드]
	CString m_strTime;						// [체결시간]

	CRealData_Common	m_RealData_S;		// 매도
	CRealData_Common	m_RealData_B;		// 매수
};
#endif //AFX_CHARTREALDEFINE_H__A82B8802_19BA_49a7_AA4C_94E1AA76C9D5__INCLUDED_

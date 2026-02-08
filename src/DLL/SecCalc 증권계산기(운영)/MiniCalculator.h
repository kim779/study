// MiniCalculator.h: interface for the CMiniCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINICALCULATOR_H__9D4D0DE2_1A9A_4DC6_A673_CB3322709795__INCLUDED_)
#define AFX_MINICALCULATOR_H__9D4D0DE2_1A9A_4DC6_A673_CB3322709795__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMiniCalculator  
{
public:
	CMiniCalculator();
	virtual ~CMiniCalculator();

	void Init(CWnd *pEditWnd);
	void Calc_Key(int key, int kind);
	void Calc_Char(CString sChar);
	void Calc_Sign();
	void Calc_C();
	void Calc_CE();
	void Calc_Back();
	void Calc_Oper(int op);
	void Calc_Enter();

private:
	void FormatString2(CString &str);
	CWnd		*m_pWnd;
	CSecCalcApp	*m_pApp;
	CString		m_sLVal;
	CString		m_sRVal;
	double		m_dLVal;
	double		m_dRVal;
	int		m_Operator;
	bool		m_bDotState;
	bool		m_bLValState;

	void	SetResult(CString str, double dRes);
	void	FormatString(CString &str);
};

#endif // !defined(AFX_MINICALCULATOR_H__9D4D0DE2_1A9A_4DC6_A673_CB3322709795__INCLUDED_)

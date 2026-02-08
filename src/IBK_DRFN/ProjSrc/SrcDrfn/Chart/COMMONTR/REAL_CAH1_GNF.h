#if !defined(AFX_REAL_CAH1__H_1A69CAB4_990E_4c28_9C85_63E7A39B8341_INCLUDED_)
#define AFX_REAL_CAH1__H_1A69CAB4_990E_4c28_9C85_63E7A39B8341_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     스타
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];		/* Data 구분 */
	char		kjmcode[10];		/* 종목코드 */
	char		hotime[6];		/* 호가시간 */
	char		mdhoga1[10];		/* 매도호가 1 */
	char		mshoga1[10];		/* 매수호가 1 */
	char		mdhorem1[8];		/* 매도호가 잔량 1 */
	char		mshorem1[8];		/* 매수호가 잔량 1 */
	char		mdhocnt1[6];		/* 매도호가 건수 1 */
	char		mshocnt1[6];		/* 매수호가 건수 1 */
	char		premdhocha1[6];		/* 직전매도대비수량 1 */
	char		premshocha1[6];		/* 직전매수대비수량 1 */
	char		mdhoga2[10];		/* 매도호가 2 */
	char		mshoga2[10];		/* 매수호가 2 */
	char		mdhorem2[8];		/* 매도호가 잔량 2 */
	char		mshorem2[8];		/* 매수호가 잔량 2 */
	char		mdhocnt2[6];		/* 매도호가 건수 2 */
	char		mshocnt2[6];		/* 매수호가 건수 2 */
	char		premdhocha2[6];		/* 직전매도대비수량 2 */
	char		premshocha2[6];		/* 직전매수대비수량 2 */
	char		mdhoga3[10];		/* 매도호가 3 */
	char		mshoga3[10];		/* 매수호가 3 */
	char		mdhorem3[8];		/* 매도호가 잔량 3 */
	char		mshorem3[8];		/* 매수호가 잔량 3 */
	char		mdhocnt3[6];		/* 매도호가 건수 3 */
	char		mshocnt3[6];		/* 매수호가 건수 3 */
	char		premdhocha3[6];		/* 직전매도대비수량 3 */
	char		premshocha3[6];		/* 직전매수대비수량 3 */
	char		mdhoga4[10];		/* 매도호가 4 */
	char		mshoga4[10];		/* 매수호가 4 */
	char		mdhorem4[8];		/* 매도호가 잔량 4 */
	char		mshorem4[8];		/* 매수호가 잔량 4 */
	char		mdhocnt4[6];		/* 매도호가 건수 4 */
	char		mshocnt4[6];		/* 매수호가 건수 4 */
	char		premdhocha4[6];		/* 직전매도대비수량 4 */
	char		premshocha4[6];		/* 직전매수대비수량 4 */
	char		mdhoga5[10];		/* 매도호가 5 */
	char		mshoga5[10];		/* 매수호가 5 */
	char		mdhorem5[8];		/* 매도호가 잔량 5 */
	char		mshorem5[8];		/* 매수호가 잔량 5 */
	char		mdhocnt5[6];		/* 매도호가 건수 5 */
	char		mshocnt5[6];		/* 매수호가 건수 5 */
	char		premdhocha5[6];		/* 직전매도대비수량 5 */
	char		premshocha5[6];		/* 직전매수대비수량 5 */
	char		totmdhorem[10];		/* 총 매도호가 잔량 */
	char		totmshorem[10];		/* 총 매수호가 잔량 */
	char		totmdhocnt[8];		/* 총 매도호가 건수 */
	char		totmshocnt[8];		/* 총 매수호가 건수 */
	char		totpremdhocha[8];		/* 총 직전매도대비수량 */
	char		totpremshocha[8];		/* 총 직전매수대비수량 */
	char		jmcode[10];		/* 종목코드 */

} stEUREAL_CAH1;	//67

class CREAL_CAH1
{
public: //Variable
	CString     m_szdgubun;				/* Data 구분 */
	CString     m_szkjmcode;			/* 종목코드 */
	CString     m_szhotime;				/* 호가시간 */
	CString     m_szmdhoga1;			/* 매도호가 1 */
	CString     m_szmshoga1;			/* 매수호가 1 */
	CString     m_szmdhorem1;			/* 매도호가 잔량 1 */
	CString     m_szmshorem1;			/* 매수호가 잔량 1 */
	CString     m_szmdhocnt1;			/* 매도호가 건수 1 */
	CString     m_szmshocnt1;			/* 매수호가 건수 1 */
	CString     m_szpremdhocha1;		/* 직전매도대비수량 1 */
	CString     m_szpremshocha1;		/* 직전매수대비수량 1 */
	CString     m_szmdhoga2;			/* 매도호가 2 */
	CString     m_szmshoga2;			/* 매수호가 2 */
	CString     m_szmdhorem2;			/* 매도호가 잔량 2 */
	CString     m_szmshorem2;			/* 매수호가 잔량 2 */
	CString     m_szmdhocnt2;			/* 매도호가 건수 2 */
	CString     m_szmshocnt2;			/* 매수호가 건수 2 */
	CString     m_szpremdhocha2;		/* 직전매도대비수량 2 */
	CString     m_szpremshocha2;		/* 직전매수대비수량 2 */
	CString     m_szmdhoga3;			/* 매도호가 3 */
	CString     m_szmshoga3;			/* 매수호가 3 */
	CString     m_szmdhorem3;			/* 매도호가 잔량 3 */
	CString     m_szmshorem3;			/* 매수호가 잔량 3 */
	CString     m_szmdhocnt3;			/* 매도호가 건수 3 */
	CString     m_szmshocnt3;			/* 매수호가 건수 3 */
	CString     m_szpremdhocha3;		/* 직전매도대비수량 3 */
	CString     m_szpremshocha3;		/* 직전매수대비수량 3 */
	CString     m_szmdhoga4;			/* 매도호가 4 */
	CString     m_szmshoga4;			/* 매수호가 4 */
	CString     m_szmdhorem4;			/* 매도호가 잔량 4 */
	CString     m_szmshorem4;			/* 매수호가 잔량 4 */
	CString     m_szmdhocnt4;			/* 매도호가 건수 4 */
	CString     m_szmshocnt4;			/* 매수호가 건수 4 */
	CString     m_szpremdhocha4;		/* 직전매도대비수량 4 */
	CString     m_szpremshocha4;		/* 직전매수대비수량 4 */
	CString     m_szmdhoga5;			/* 매도호가 5 */
	CString     m_szmshoga5;			/* 매수호가 5 */
	CString     m_szmdhorem5;			/* 매도호가 잔량 5 */
	CString     m_szmshorem5;			/* 매수호가 잔량 5 */
	CString     m_szmdhocnt5;			/* 매도호가 건수 5 */
	CString     m_szmshocnt5;			/* 매수호가 건수 5 */
	CString     m_szpremdhocha5;		/* 직전매도대비수량 5 */
	CString     m_szpremshocha5;		/* 직전매수대비수량 5 */
	CString     m_sztotmdhorem;			/* 총 매도호가 잔량 */
	CString     m_sztotmshorem;			/* 총 매수호가 잔량 */
	CString     m_sztotmdhocnt;			/* 총 매도호가 건수 */
	CString     m_sztotmshocnt;			/* 총 매수호가 건수 */
	CString     m_sztotpremdhocha;		/* 총 직전매도대비수량 */
	CString     m_sztotpremshocha;		/* 총 직전매수대비수량 */
	CString     m_szjmcode;				/* 종목코드 */


public:
	CREAL_CAH1()
	{
	}

	virtual void  Convert(stEUREAL_CAH1* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun		);/* Data 구분 */
		DATA_MEMCOPY(sTmp, pInput, kjmcode		);/* 종목코드 */
		DATA_MEMCOPY(sTmp, pInput, hotime		);/* 호가시간 */
		DATA_MEMCOPY(sTmp, pInput, mdhoga1		);/* 매도호가 1 */
		DATA_MEMCOPY(sTmp, pInput, mshoga1		);/* 매수호가 1 */
		DATA_MEMCOPY(sTmp, pInput, mdhorem1		);/* 매도호가 잔량 1 */
		DATA_MEMCOPY(sTmp, pInput, mshorem1		);/* 매수호가 잔량 1 */
		DATA_MEMCOPY(sTmp, pInput, mdhocnt1		);/* 매도호가 건수 1 */
		DATA_MEMCOPY(sTmp, pInput, mshocnt1		);/* 매수호가 건수 1 */
		DATA_MEMCOPY(sTmp, pInput, premdhocha1	);/* 직전매도대비수량 1 */
		DATA_MEMCOPY(sTmp, pInput, premshocha1	);/* 직전매수대비수량 1 */
		DATA_MEMCOPY(sTmp, pInput, mdhoga2		);/* 매도호가 2 */
		DATA_MEMCOPY(sTmp, pInput, mshoga2		);/* 매수호가 2 */
		DATA_MEMCOPY(sTmp, pInput, mdhorem2		);/* 매도호가 잔량 2 */
		DATA_MEMCOPY(sTmp, pInput, mshorem2		);/* 매수호가 잔량 2 */
		DATA_MEMCOPY(sTmp, pInput, mdhocnt2		);/* 매도호가 건수 2 */
		DATA_MEMCOPY(sTmp, pInput, mshocnt2		);/* 매수호가 건수 2 */
		DATA_MEMCOPY(sTmp, pInput, premdhocha2	);/* 직전매도대비수량 2 */
		DATA_MEMCOPY(sTmp, pInput, premshocha2	);/* 직전매수대비수량 2 */
		DATA_MEMCOPY(sTmp, pInput, mdhoga3		);/* 매도호가 3 */
		DATA_MEMCOPY(sTmp, pInput, mshoga3		);/* 매수호가 3 */
		DATA_MEMCOPY(sTmp, pInput, mdhorem3		);/* 매도호가 잔량 3 */
		DATA_MEMCOPY(sTmp, pInput, mshorem3		);/* 매수호가 잔량 3 */
		DATA_MEMCOPY(sTmp, pInput, mdhocnt3		);/* 매도호가 건수 3 */
		DATA_MEMCOPY(sTmp, pInput, mshocnt3		);/* 매수호가 건수 3 */
		DATA_MEMCOPY(sTmp, pInput, premdhocha3	);/* 직전매도대비수량 3 */
		DATA_MEMCOPY(sTmp, pInput, premshocha3	);/* 직전매수대비수량 3 */
		DATA_MEMCOPY(sTmp, pInput, mdhoga4		);/* 매도호가 4 */
		DATA_MEMCOPY(sTmp, pInput, mshoga4		);/* 매수호가 4 */
		DATA_MEMCOPY(sTmp, pInput, mdhorem4		);/* 매도호가 잔량 4 */
		DATA_MEMCOPY(sTmp, pInput, mshorem4		);/* 매수호가 잔량 4 */
		DATA_MEMCOPY(sTmp, pInput, mdhocnt4		);/* 매도호가 건수 4 */
		DATA_MEMCOPY(sTmp, pInput, mshocnt4		);/* 매수호가 건수 4 */
		DATA_MEMCOPY(sTmp, pInput, premdhocha4	);/* 직전매도대비수량 4 */
		DATA_MEMCOPY(sTmp, pInput, premshocha4	);/* 직전매수대비수량 4 */
		DATA_MEMCOPY(sTmp, pInput, mdhoga5		);/* 매도호가 5 */
		DATA_MEMCOPY(sTmp, pInput, mshoga5		);/* 매수호가 5 */
		DATA_MEMCOPY(sTmp, pInput, mdhorem5		);/* 매도호가 잔량 5 */
		DATA_MEMCOPY(sTmp, pInput, mshorem5		);/* 매수호가 잔량 5 */
		DATA_MEMCOPY(sTmp, pInput, mdhocnt5		);/* 매도호가 건수 5 */
		DATA_MEMCOPY(sTmp, pInput, mshocnt5		);/* 매수호가 건수 5 */
		DATA_MEMCOPY(sTmp, pInput, premdhocha5	);/* 직전매도대비수량 5 */
		DATA_MEMCOPY(sTmp, pInput, premshocha5	);/* 직전매수대비수량 5 */
		DATA_MEMCOPY(sTmp, pInput, totmdhorem	);/* 총 매도호가 잔량 */
		DATA_MEMCOPY(sTmp, pInput, totmshorem	);/* 총 매수호가 잔량 */
		DATA_MEMCOPY(sTmp, pInput, totmdhocnt	);/* 총 매도호가 건수 */
		DATA_MEMCOPY(sTmp, pInput, totmshocnt	);/* 총 매수호가 건수 */
		DATA_MEMCOPY(sTmp, pInput, totpremdhocha);/* 총 직전매도대비수량 */
		DATA_MEMCOPY(sTmp, pInput, totpremshocha);/* 총 직전매수대비수량 */
		DATA_MEMCOPY(sTmp, pInput, jmcode		);/* 종목코드 */
	}
};

#endif // AFX_REAL_CAH1__H_1A69CAB4_990E_4c28_9C85_63E7A39B8341_INCLUDED_

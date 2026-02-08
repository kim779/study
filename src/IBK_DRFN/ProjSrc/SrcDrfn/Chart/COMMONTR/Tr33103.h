#if !defined(AFX_TR_33103_HEADER_)
#define AFX_TR_33103_HEADER_

#include "./Data_Common.h"

#define  TR_33103	"33103"
#define MAX33103 200

//-------------------------------------------------------------------------------//
// 실시간미체결            사용TR번호:33103 
//--------------------------------[INPUT ]---------------------------------------//
typedef struct _TR_33103_I
{
    char    winid   [4];
    char    mkgb    [1];    /* 1:KOSPI, 2:KOSDAQ */
} TR_33103_I;
#define  TR_33103_I_SIZE sizeof(TR_33103_I)

// #define tr_33103MAX		400
//--------------------------------[OUTPUT반복 ]-----------------------------------//
typedef struct _TR_33103_G
{
    char    code    [ 5];
    char    name    [20];
    char    sign    [ 1];
    char    jisu    [ 8]; // 7->8
	// 업종에 전일비 추가 - ojtaso (20080107)
	char	change  [ 8];
} TR_33103_G;
#define  TR_33103_G_SIZE sizeof(_TR_33103_G)

//--------------------------------[OUTPUT ]---------------------------------------//
typedef struct _TR_33103_O
{
     char        winid   [4];
    char        cnt     [4];
    TR_33103_G  data    [MAX33103];
} TR_33103_O;

class CTR_33103_G
{
public: //Variable
	CString m_szcode		;  		/* 업종코드     */ 
	CString m_szname		;  		/* 업종명	 	*/
	CString m_szsign		; 		/* sign			*/
	CString m_szjisu		;   	/* 현재가		*/
	// 업종에 전일비 추가 - ojtaso (20080107)
	CString m_szchange;
public:
	CTR_33103_G()
	{
	}

	virtual void  Convert(TR_33103_G* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, code		);    	/* 업종코드 	*/
		DATA_MEMCOPY(sTmp, pInput, name		);  	/* 업종명     */ 
		DATA_MEMCOPY(sTmp, pInput, sign		);  	/* sign	 	*/
		DATA_MEMCOPY(sTmp, pInput, jisu		); 		/* 현재가 */
		// 업종에 전일비 추가 - ojtaso (20080107)
		DATA_MEMCOPY(sTmp, pInput, change	); 		/* 전일비 */
	}
};

typedef CList<CTR_33103_G*, CTR_33103_G*> LIST_TR_33103_G;

class CTR_33103_O
{
public:
    CString m_szgr_cnt		;			/* Grid count           */
    CString m_szgr_bjlen	;           /* Grid Bojo Data Length*/
    CString m_szgr_bjdata	;           /* Grid Bojo Data       */
	// char		gr_DATA[1];		// 사이즈만큼 알아서..

    int		 m_ngr_cnt;				/* Grid count           */
    int		m_ngr_bjlen;			/* Grid Bojo Data Length*/    
	int		m_ngr_bjdata	;		/* Grid Bojo Data       */

	LIST_TR_33103_G m_DataList;

public:
	CTR_33103_O()
	{
		ResetAll();
	}

	~CTR_33103_O()
	{
		ResetAll();
	}

	virtual void  Convert(TR_33103_O* pInput)
	{
		char tmp[4+1]={0,};

		memcpy(tmp, pInput->cnt, sizeof(pInput->cnt)); 
		m_ngr_cnt = atoi(tmp);

		CTR_33103_G *pNewItem=NULL;
		TR_33103_G *pData=NULL;
		CString sTemp1, sTemp2;
		for(int i=0; i < m_ngr_cnt; i++)
		{
			pData = (TR_33103_G*)&pInput->data[i];//*sizeof(TR_33103_G)];
			pNewItem = new CTR_33103_G;
			pNewItem->Convert(pData);
			m_DataList.AddTail(pNewItem);
		}
	}

	virtual void  ClearList()
	{
		for(POSITION pos=m_DataList.GetHeadPosition(); pos; )
		{
			CTR_33103_G* pItem = m_DataList.GetNext(pos);
			if(pItem) delete pItem;
		}
		m_DataList.RemoveAll();
	}

	void ResetAll()
	{
		m_ngr_cnt = 0;

		ClearList();
	}

};

#endif //AFX_TR_33103_HEADER_

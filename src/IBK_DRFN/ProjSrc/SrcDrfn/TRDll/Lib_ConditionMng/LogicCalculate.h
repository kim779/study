#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <afxcoll.h>

/********************************************************************/
/*																	                                */
/*										각 지표별 논리 연산 결과 산출 모듈						*/
/*																	                                */
/*										2006. 09.  		  Doori		                      */
/*                    작성자 : 우동우																*/
/********************************************************************/

typedef struct tagCodeList
{
	char sElement;
  int nCnt;
  char *psData;
} CodeList;

class CLogicCalculate {
typedef struct tagToken
{
	bool	  bOpt;
  char    sValue;
  BYTE    nValue;
} Token;

struct Node
{
	int		nSize;
  void  *pToken;
	Node	*pLink;
};

private:
  CStringArray *m_psaMSTCode;

  void GetTokenData(char *psData, void *pQueue);
  int GetTokenData(char ***psTokenList, char *psTarget, char *psDefine);

  bool SetPush(void *pStack, void *pToken, int nSize);
  bool SetPop(void *pStack, void *pToken, int *nSize);

  bool SetAdd(void *pQueue, void *pToken, int nSize);
  void SetView_All(void *pQueue);
  bool SetDelete_Rear(void *pQueue);
  void SetDelete_All(void *pQueue);

	int GetOptPriority(void *pStack);
	int	GetOptPriority(char sOpt);


  BOOL SetMakeBitSet(CStringArray *psaCode, UCHAR *pBitSet);
  BOOL SetConvertBitSet(CPtrArray *paData, CPtrArray *paBitSet);
  BOOL SetMakeBitSet(int nCnt, char *psCode, char *pBitSet);
  int SetBitSetToChar(char *pBitSet, char *psResult);

  char* GetElementToBitSet(char sElement, CPtrArray *paData);

  void Debug_ViewToken(CString strMode, Token *pToken);
  
public:
	CLogicCalculate();
  ~CLogicCalculate();

  BOOL SetInit(CStringArray *psaMSTCode);
  void SetFree_PtrArray(CPtrArray *paData);
  	
  BOOL GetLogicCalculate(char *psData, CPtrArray *paElementData, char **psResult);
};

#endif CALC_H
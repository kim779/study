#include "stdafx.h"
#include "LogicCalculate.h"

/********************************************************************/
/*																	                                */
/*										각 지표별 논리 연산 결과 산출 모듈						*/
/*																	                                */
/*										2006. 09.  		  Doori		                      */
/*                   															*/
/********************************************************************/

//#define SZ_BITSET   300	//250 -> 300
#define SZ_BITSET   400	//300 -> 400    3000 -> 4000

CLogicCalculate::CLogicCalculate()
{
	m_psaMSTCode = NULL;
	
}

CLogicCalculate::~CLogicCalculate()
{
	if (m_psaMSTCode != NULL)
	{
		m_psaMSTCode->RemoveAll();
		delete m_psaMSTCode;
		m_psaMSTCode = NULL;
	}
}

BOOL CLogicCalculate::SetInit(CStringArray *psaMSTCodeList)
{
	if (psaMSTCodeList == NULL)
		return FALSE;
	
	if (m_psaMSTCode == NULL)
		m_psaMSTCode = new CStringArray;
	else
		m_psaMSTCode->RemoveAll();
	
	m_psaMSTCode->Copy(*psaMSTCodeList);
	return TRUE;
}

int CLogicCalculate::GetOptPriority(void *pStack)
{
	int nResult;
	
	int nSize;
	Token *pToken = (Token *)malloc(sizeof(Token));
	memset(pToken, 0, sizeof(Token));
	
	if (SetPop(pStack, pToken, &nSize))
		SetPush(pStack, pToken, sizeof(Token*));
	else
		pToken->sValue = '!';
	
	switch (pToken->sValue)
	{
	case '(':
		nResult = 0;
		break;
    case ')':
		nResult = 19;
		break;
    case '|':
		nResult = 12;
		break;
    case '&':
		nResult = 13;
		break;
    case '!':case '\0':default:
		nResult = 0;
		break;
	}
	
	free(pToken);
	return nResult;
}

int CLogicCalculate::GetOptPriority(char sOpt)
{
	switch (sOpt)
	{
	case '(':
		return 20;
    case ')':
		return 19;
    case '|':
		return 12;
    case '&':
		return 13;
    case '!':case '\0':default:
		return 0;
	}
}

BOOL CLogicCalculate::SetMakeBitSet(int nCnt, char *psCode, char *pBitSet)
{
	if (m_psaMSTCode == NULL)
		return FALSE;

	if(m_psaMSTCode->GetSize() < 0)
		return FALSE;
	
	BOOL bAdd = FALSE;
	char sCode[7];

	int nIndex, nIndex_Code = 0;
	for (nIndex = 0; nIndex < m_psaMSTCode->GetSize(); nIndex++)
	{
		bAdd = FALSE;
		for (nIndex_Code = 0; nIndex_Code < nCnt; nIndex_Code++)
		{
			memset(&sCode, 0x00, sizeof(sCode));
			memcpy(&sCode, psCode + nIndex_Code * sizeof(sCode), sizeof(sCode));
			
			CString strTemp;
			strTemp = m_psaMSTCode->GetAt(nIndex);
						
			if (m_psaMSTCode->GetAt(nIndex) == (CString)sCode)
			{
				bAdd = TRUE;
				break;
			}
		}
		
		if (bAdd == TRUE)
		{
			int nBytePos = nIndex / 8;
			int nBitPos = nIndex % 8;
			
			byte byteTemp = 0x80;
			byte byteOld;
			
			byteTemp >>= nBitPos;
			
			byteOld = *(pBitSet + nBytePos);
			byteTemp = byteTemp | byteOld;
			
			*(pBitSet + nBytePos) = byteTemp;
			
			nIndex_Code++;
		}
	}
	
	return TRUE;
}

BOOL CLogicCalculate::SetConvertBitSet(CPtrArray *paData, CPtrArray *paBitSet)
{
	BOOL bResult = TRUE;
	
	CodeList *pstData, *pstResult;
	for (int nIndex = 0; nIndex < paData->GetSize(); nIndex++)
	{
		pstData = (CodeList *)paData->GetAt(nIndex);
		
		pstResult = (CodeList*)malloc(sizeof(CodeList));
		pstResult->sElement = pstData->sElement;
		pstResult->nCnt = (int)SZ_BITSET;
		pstResult->psData = (char*)malloc(pstResult->nCnt);
		
		if (pstData->nCnt == 0)
		{
			memset(pstResult->psData, 0xFF, pstResult->nCnt);
			paBitSet->Add(pstResult);
		}
		else
		{
			memset(pstResult->psData, 0x00, pstResult->nCnt);
			if (SetMakeBitSet(pstData->nCnt, pstData->psData, pstResult->psData) == TRUE)
				paBitSet->Add(pstResult);
			else
			{
				bResult = FALSE;
				SetFree_PtrArray(paBitSet);
				break;
			}
		}
	}
	
	return bResult;
}

char* CLogicCalculate::GetElementToBitSet(char sElement, CPtrArray *paData)
{
	char *psResult = NULL;
	
	CodeList *pstCodeList;
	for (int nIndex = 0; nIndex < paData->GetSize(); nIndex++)
	{
		pstCodeList = (CodeList *)paData->GetAt(nIndex);
		if (sElement == pstCodeList->sElement)
		{
			psResult = pstCodeList->psData;
			break;
		}
	}
	
	return psResult;
}

void CLogicCalculate::SetFree_PtrArray(CPtrArray *paData)
{
	CodeList *pstCodeList;
	while (paData->GetSize() > 0)
	{
		pstCodeList = (CodeList *)paData->GetAt(0);
		if (pstCodeList->psData != NULL)
		{
			free(pstCodeList->psData);
			pstCodeList->psData = NULL;
		}
		
		free(pstCodeList);
		pstCodeList = NULL;
		
		paData->RemoveAt(0);
	}
}

BOOL CLogicCalculate::GetLogicCalculate(char *psData, CPtrArray *paElementData, char **psResult)
{
	BOOL bResult = FALSE;
	
	CPtrArray *paBitSet = new CPtrArray;
	if (SetConvertBitSet(paElementData, paBitSet) == FALSE)
	{
		delete paBitSet;
		return bResult;
	}
	
	int nElementCnt = 0;
	
	int nSize;
	Node *pNode_Stack;
	Node *pNode_InQ, *pNode_PostQ;
	
	pNode_Stack = (Node *)malloc(sizeof(Node));
	memset(pNode_Stack, 0x00, sizeof(Node));
	
	pNode_InQ = (Node *)malloc(sizeof(Node));
	memset(pNode_InQ, 0x00, sizeof(Node));
	
	pNode_PostQ = (Node *)malloc(sizeof(Node));
	memset(pNode_PostQ, 0x00, sizeof(Node));
	
	GetTokenData(psData, pNode_InQ);
	
	Token *pToken;
	Node *pNode = pNode_InQ->pLink->pLink;
	while (pNode != pNode_InQ->pLink)
	{
		pToken = (Token *)malloc(sizeof(Token));
		memset(pToken, 0, sizeof(Token));
		memcpy(pToken, pNode->pToken, sizeof(Token));
		
		if (pToken->bOpt)
		{
			SetAdd(pNode_PostQ, (Token*)pToken, sizeof(Token));
			nElementCnt++;
		}
		else if (pToken->sValue == ')')
		{
			SetPop(pNode_Stack, pToken, &nSize);
			while (pToken->sValue != '(')
			{
				SetAdd(pNode_PostQ, (Token*)pToken, sizeof(Token));
				SetPop(pNode_Stack, pToken, &nSize);
			}
		}
		else
		{
			Token *pToken_In;
			while (GetOptPriority(pNode_Stack) >= GetOptPriority(pToken->sValue))
			{
				pToken_In = (Token *)malloc(sizeof(Token));
				memset(pToken_In, 0, sizeof(Token));
				
				SetPop(pNode_Stack, pToken_In, &nSize);
				SetAdd(pNode_PostQ, pToken_In, sizeof(Token));
				
				free(pToken_In);
			}
			SetPush(pNode_Stack, pToken, sizeof(pToken));
		}
		
		pNode = pNode->pLink;
		free(pToken);
	}
	
	SetDelete_All(pNode_InQ);
	
	pToken = (Token *)malloc(sizeof(Token));
	while (SetPop(pNode_Stack, pToken, &nSize))
	{
		SetAdd(pNode_PostQ, (Token*)pToken, sizeof(Token));
		free(pToken);
		
		pToken = (Token *)malloc(sizeof(Token));
	}
	free(pToken);
	
	SetView_All(pNode_PostQ);
	
	pToken = (Token *)malloc(sizeof(Token));
	memset(pToken, 0, sizeof(Token));
	pToken->bOpt = false;
	pToken->sValue = '!';
	pToken->nValue = 0;
	SetAdd(pNode_PostQ, pToken, sizeof(pToken));
	free(pToken);
	
	char *pBitSet_right, *pBitSet_left;
	pNode = pNode_PostQ->pLink->pLink;
	while (pNode != pNode_PostQ->pLink)
	{
		pToken = (Token*)pNode->pToken;
		if (pToken->bOpt)
		{
			char *psData;
			psData = GetElementToBitSet(pToken->sValue, paBitSet);
			
			if (psData != NULL)
				SetPush(pNode_Stack, psData, (int)SZ_BITSET);
			
			
			
				/*
				CodeList *pstCodeList;
				pstCodeList = (CodeList *)paBitSet->GetAt(0);
				
				  char *psData;
				  
					/*
					CString strData_Index;
					strData_Index.Format("%c", pToken->sValue);
					psData = (char*)paBitSet->GetAt(atoi(strData_Index));
			*/
			/*
			psData = (char*)paBitSet->GetAt(pToken->nValue);
			SetPush(pNode_Stack, psData, (int)SZ_BITSET);
			*/
			
			
			/*
			pBitSet_right = (char *)malloc(256);
			memset(pBitSet_right, 0, 256);
			SetPop(pNode_Stack, pBitSet_right, &nSize);
			
			  for (int nIndex = 0; nIndex < 256; nIndex++)
			  {
			  strData_Index.Format("\n %d : %d", nIndex, pBitSet_right[nIndex]);
			  TRACE(strData_Index);
			  }
			*/
		}
		else
		{
			pBitSet_right = (char *)malloc((int)SZ_BITSET);
			memset(pBitSet_right, 0x00, (int)SZ_BITSET);
			SetPop(pNode_Stack, pBitSet_right, &nSize);
			
			pBitSet_left = (char *)malloc((int)SZ_BITSET);
			memset(pBitSet_left, 0x00, (int)SZ_BITSET);
			SetPop(pNode_Stack, pBitSet_left, &nSize);
			
			if (pToken->sValue == '|')
			{
				for (int  nIndex = 0; nIndex < (int)SZ_BITSET; nIndex++)
					pBitSet_left[nIndex] = pBitSet_left[nIndex] | pBitSet_right[nIndex];
			}
			else if (pToken->sValue == '&')
			{
				for (int  nIndex = 0; nIndex < (int)SZ_BITSET; nIndex++)
					pBitSet_left[nIndex] = pBitSet_left[nIndex] & pBitSet_right[nIndex];
			}
			SetPush(pNode_Stack, pBitSet_left, (int)SZ_BITSET);
			
			free(pBitSet_right);
			free(pBitSet_left);
		}
		pNode = pNode->pLink;
	}
	
	pBitSet_left = (char *)malloc((int)SZ_BITSET);
	memset(pBitSet_left, 0x00, (int)SZ_BITSET);
	SetPop(pNode_Stack, pBitSet_left, &nSize);
	
	int nTrueCnt = 0;
	if (SetBitSetToChar(pBitSet_left, *psResult) > 0)
		bResult = TRUE;
	
	free(pBitSet_left);
	
	
	SetDelete_All(pNode_PostQ);
	
	memset(pNode_Stack, 0x00, sizeof(Node));
	free(pNode_Stack);
	
	memset(pNode_InQ, 0x00, sizeof(Node));
	free(pNode_InQ);
	
	memset(pNode_PostQ, 0x00, sizeof(Node));
	free(pNode_PostQ);
	
	
	SetFree_PtrArray(paBitSet);
	delete paBitSet;
	
	return bResult;
}


int CLogicCalculate::SetBitSetToChar(char *pBitSet, char *psResult)
{
	int nResult = 0;
	
	memset(psResult, 0x30, (int)SZ_BITSET * 10);  //20091005 SJ_KIM 2000 -> 2500....   *8 -> *10
	
	for (int nBytePos = 0; nBytePos < (int)SZ_BITSET; nBytePos++)
	{
		for (int nBitPos = 0; nBitPos < 8; nBitPos++)
		{
			byte byteData = 0x80;
			byte byteOld;
			
			byteData >>= nBitPos;
			byteOld = *(pBitSet + nBytePos);
			byteData = byteData & byteOld;
			
			if ((int)byteData > 0)
			{
				psResult[nBytePos * 8 + nBitPos] = '1';
				nResult++;
			}
			
			/*
			if (nBytePos * 8 + nBitPos < 20)
			{
			CString strTemp;
			strTemp.Format("\n%d : %d", nBytePos * 8 + nBitPos, byteData);
			TRACE(strTemp);
			}
			*/
		}
	}
	
	return nResult;
}


void CLogicCalculate::GetTokenData(char *psData, void *pQueue)
{
	int nIndex;
	int nTokenCnt;
	char **psTokenList;
	
	nTokenCnt = GetTokenData(&psTokenList, psData, "|&()");
	
	Token *pToken;
	for (nIndex = 0; nIndex < nTokenCnt; nIndex++)
	{
		pToken = (Token *)malloc(sizeof(Token));
		memset(pToken, 0x30, sizeof(Token));
		
		switch (psTokenList[nIndex][0])
		{
		case '|':case '&':
		case '(':case ')':
			pToken->bOpt = false;
			pToken->sValue = psTokenList[nIndex][0];
			pToken->nValue = 0;
			break;
		default :
			pToken->bOpt = true;
			pToken->sValue = psTokenList[nIndex][0];
			pToken->nValue = atoi(psTokenList[nIndex]);
			break;
		}
		
		SetAdd(pQueue, pToken, sizeof(Token));
		free(pToken);
	}
	
	pToken = (Token *)malloc(sizeof(Token));
	memset(pToken, 0x30, sizeof(Token));
	pToken->bOpt = false;
	pToken->sValue = '!';
	pToken->nValue = 0;
	SetAdd(pQueue, pToken, sizeof(Token));
	free(pToken);
	
	SetView_All(pQueue);
	
	for (nIndex = 0; nIndex < nTokenCnt; nIndex++)
		free(psTokenList[nIndex]);
	free(psTokenList);
}

int CLogicCalculate::GetTokenData(char ***psTokenList, char *psTarget, char *psDefine)
{
	int nPos = 0, nPos_Step = 0;
	int nDefineCnt = 0;
	int nPos_Before = 0;
	int nCnt = 0;
	char *psDefine_Token, **psResult_Token, *psData;
	bool bSame = false;
	
	psDefine_Token = (char *)malloc(strlen(psDefine) + 1);
	memset(psDefine_Token, 0, strlen(psDefine) + 1);
	
	nDefineCnt = strlen(psDefine);
	int nPos_Result = 0;
	for (nPos = 0; nPos < nDefineCnt; nPos++)
	{
		bSame = false;
		for (nPos_Step = nPos + 1; nPos_Step <= nDefineCnt; nPos_Step++)
		{
			if (psDefine[nPos] == psDefine[nPos_Step])
			{
				bSame = true;
				break;
			}
		}
		
		if (!bSame)
			psDefine_Token[nPos_Result++] = psDefine[nPos];
	}
	
	nDefineCnt = strlen(psDefine_Token);
	if (strlen(psTarget) == 0)
	{
		free(psDefine_Token);
		return 0;
	}
	
	if (nDefineCnt == 0)
	{
		psData = (char *)malloc(strlen(psTarget));
		memset(psData, 0, strlen(psTarget));
		strcpy(psData, psTarget);
		psResult_Token = (char **)malloc((int)(sizeof(char *)));
		*psResult_Token = psData;
		nCnt = 1;
		*psTokenList = psResult_Token;
		
		free(psDefine_Token);
		return nCnt;
	}
	
	nPos = nPos_Before = nCnt = 0;
	while (*(psTarget + nPos))
	{
		for (nPos_Step = 0; nPos_Step < nDefineCnt; nPos_Step++)
		{
			if (*(psTarget + nPos) == psDefine_Token[nPos_Step])
			{
				if (nPos == nPos_Before)
				{
					nPos_Before++;
					nCnt++;
					break;
				}
				nCnt++;
				nPos_Before = nPos + 1;
				nCnt++;
				break;
			}
		}
		if (!*(psTarget + nPos + 1))
		{
			nCnt++;
			nPos++;
			break;
		}
		nPos++;
	}
	
	psResult_Token = (char **)malloc((int)(sizeof(char *))*nCnt);
	
	nPos = nPos_Before = nCnt = 0;
	while (*(psTarget + nPos))
	{
		for (nPos_Step = 0; nPos_Step < nDefineCnt; nPos_Step++)
		{
			if (*(psTarget + nPos) == psDefine_Token[nPos_Step])
			{
				if (nPos == nPos_Before)
				{
					psData = (char *)malloc((size_t)(nPos - nPos_Before + 2));
					memset(psData, 0, (nPos - nPos_Before + 2));
					strncpy(psData, (psTarget + nPos_Before), nPos - nPos_Before + 1);
					*(psResult_Token + nCnt) = psData;
					nCnt++;
					nPos_Before++;
					break;
				}
				psData = (char *)malloc((size_t)(nPos - nPos_Before + 1));
				memset(psData, 0, (nPos-nPos_Before+1));
				strncpy(psData, (psTarget+nPos_Before), nPos-nPos_Before);
				*(psResult_Token + nCnt) = psData;
				nCnt++;
				nPos_Before = nPos;
				
				psData = (char *)malloc((size_t)(nPos - nPos_Before + 2));
				memset(psData, 0, (nPos - nPos_Before + 2));
				strncpy(psData, (psTarget + nPos_Before), nPos - nPos_Before + 1);
				*(psResult_Token + nCnt) = psData;
				nCnt++;
				nPos_Before++;
				
				break;
			}
		}
		nPos++;
		if (!*(psTarget + nPos))
		{
			if (nPos != nPos_Before)
			{
				psData = (char *)malloc((size_t)(nPos - nPos_Before + 2));
				memset(psData, 0, (nPos - nPos_Before + 2));
				strncpy(psData, (psTarget + nPos_Before), nPos - nPos_Before + 1);
				*(psResult_Token + nCnt) = psData;
				nCnt++;
				nPos++;
				
				break;
			}
		}
	}
	
	free(psDefine_Token);
	
	*psTokenList = psResult_Token;
	return nCnt;
}

bool CLogicCalculate::SetPush(void *pStack, void *pToken, int nSize)
{
	Node *pNodeTop = (Node *)pStack;
	Node *pNode = (Node *)malloc(sizeof(Node));
	
	pNode->nSize = nSize;
	pNode->pToken = (void *)malloc(nSize);
	if (pNode->pToken == NULL)
	{
		free(pNode);
		return false;
	}
	memcpy(pNode->pToken, pToken, nSize);
	
	if (pNodeTop->pLink == NULL)
	{
		pNode->pLink = NULL;
		pNodeTop->pLink	= pNode;
	}
	else
	{
		pNode->pLink = pNodeTop->pLink;
		pNodeTop->pLink	= pNode;
	}
	
	//m_nCnt++;
	
	return true;
}

bool CLogicCalculate::SetPop(void *pStack, void *pToken, int *nSize)
{
	Node *pNodeTop = (Node *)pStack;
	
	CString strTemp;
	CString strMode = "\n[Stack POP] : ";
	
	if (pNodeTop->pLink == NULL)
		return false;
	
	Node *pNode = pNodeTop->pLink; 
	memset(pToken, 0, pNode->nSize);
	memcpy(pToken, pNode->pToken, pNode->nSize);
	*nSize = pNode->nSize;
	free(pNode->pToken);
	
	pNodeTop->pLink = pNode->pLink;
	free(pNode);
	
	//m_nCnt--;
	
	return true;
}

bool CLogicCalculate::SetAdd(void *pQueue, void *pToken, int nSize)
{
	Node *pNodeRear = (Node *)pQueue;
	
	Node *pNode = (Node *)malloc(sizeof(Node));
	if (pNode == NULL)
		return false;
	memset(pNode, 0, sizeof(Node));
	
	pNode->nSize = nSize;
	pNode->pToken = (void *)malloc(nSize);
	if (pNode->pToken == NULL)
	{
		free(pNode);
		return false;
	}
	memcpy(pNode->pToken, pToken, nSize);
	
	if (pNodeRear->pLink == NULL)
	{
		pNodeRear->pLink = pNode;
		pNode->pLink = pNode;
	}
	else
	{
		pNode->pLink = pNodeRear->pLink->pLink;
		pNodeRear->pLink->pLink = pNode;
	}
	pNodeRear->pLink = pNode;
    
	return true;
}

void CLogicCalculate::SetView_All(void *pQueue)
{
	Node *pNodeRear = (Node *)pQueue;
	
	CString strMode = "\n[Q View] : ";
	Token *pToken;
	CString strTemp;
	
	Node *pNext;
	Node *pNode_rear = pNodeRear->pLink;
	Node *pNode = pNodeRear->pLink->pLink;
	while (pNode != pNode_rear)
	{
		pToken = (Token*)pNode->pToken;
		Debug_ViewToken(strMode, pToken);
        
		pNext = pNode->pLink;
		pNode = pNext;
	}
	
	pToken = (Token*)pNode_rear->pToken;
	Debug_ViewToken(strMode, pToken);
}

void CLogicCalculate::Debug_ViewToken(CString strMode, Token *pToken)
{
	CString strData;
	strData.Format("%d, %c, %d", pToken->bOpt, pToken->sValue, pToken->nValue);
	TRACE(strMode + strData);
}

bool CLogicCalculate::SetDelete_Rear(void *pQueue)
{
	Node *pNodeRear = (Node *)pQueue;
	
	if (pNodeRear->pLink == NULL) 
		return false;
	
	CString strMode = "\n[Rear Delete] : ";
	
	Token *pToken;
	CString strTemp;
	
	Node *pNode_rear = pNodeRear->pLink;
	pToken = (Token*)pNode_rear->pToken;
	
	if (pToken != NULL)
	{
		Debug_ViewToken(strMode, pToken);
		free(pToken);
	}
	pNodeRear->pLink = pNodeRear->pLink->pLink;
	
	free(pNode_rear);
	return true;
}

void CLogicCalculate::SetDelete_All(void *pQueue)
{
	Node *pNodeRear = (Node *)pQueue;
	
	CString strMode = "\n[Q Delete] : ";
	Token *pToken;
	CString strTemp;
	
	Node *pNext;
	Node *pNode_rear = pNodeRear->pLink;
	Node *pNode = pNodeRear->pLink->pLink;
	while (pNode != pNode_rear)
	{
		pToken = (Token*)pNode->pToken;
		Debug_ViewToken(strMode, pToken);
		free(pToken);
		
		pNext = pNode->pLink;
		free(pNode);
		
		pNode = pNext;
	}
	
	pToken = (Token*)pNode_rear->pToken;
	Debug_ViewToken(strMode, pToken);
	
	free(pToken);
	free(pNode_rear);
}
#pragma once
#include "axisgenv.h"
#pragma pack(1)

// 멀티 차트를 위한 포맷 정의(PruWin 7530~7533)

#define	MAX_MMAPY		4					// Map의 최대 Y Size
#define	MAX_MMAPX		6					// Map의 최대 X Size
#define	MAX_MMAPCELL	MAX_MMAPY*MAX_MMAPX	// Map의 최대 Cell Size

#define	VERSION_MC		1					// Multi Chart 버젼
#define MCI				".mc0"				// Multi Chart 정보

#define	MT_SINGLE	1	// 단일종목 멀티차트
#define	MT_MULTI	2	// 복수종목 멀티차트
#define	MT_MONITOR	3	// 시장감시 멀티차트
#define	MT_FOREIGN	4	// 해외증시 멀티차트

struct _MultiChart
{
	WORD	wVersion{};						// file version - reserved
	char	acObjName[16]{};					// ObjName
	BYTE	btUnit{};							// 초기 업종 선택
	BYTE	btShowCFG{};						// ConfigBar Show
	BYTE	btOption{};						// Option
	BYTE	btType{};							// 1~4
	BYTE	btToday{};						// 당일체크 
	WORD	btMainKind{};						// Main Graph Kind
    char    acCntX{};							// 맵의 수평사이즈(1~6)
    char    acCntY{};							// 맵의 수직사이즈(1~4)
	int		acGigan[5]{};						// 일,주,월,분,틱(300,200,100,400,400)
	char	acUlist[MAX_UGRP]{};				// 즐겨찾기
	char    acSelect[MAX_MMAPCELL]{};			// map CheckBox
    char    acMatrix[300]{};					// map matrix
    char    acCode[MAX_MMAPCELL][12]{};		// 코드 저장
	char	acDummy[64]{};					// reserve
};
#define	SZ_MULTICHART	sizeof(struct _MultiChart)

struct	_ActiveChartInfo
{
	char	acCode[12]{};						// 코드
	int		iUnit{};							// 종목,코드..
	int		iIndex{};							// 일주월..
	int		iMGap{};							// 분갭
	int		iTGap{};							// 틱갭
	int		iRCnt{};							// 요청 개수
	int		iToday{};							// 당일 체크
};

#pragma pack()


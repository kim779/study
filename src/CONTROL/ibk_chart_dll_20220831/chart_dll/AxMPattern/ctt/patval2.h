/*  Candle Pattern for Prudential securities	*/

/* 상승반전형	*/
#define V2_InvertedHammer		101
#define V2_Hammer			102
#define V2_BullishBeltHold		103
#define V2_BullishEngulfing		104
#define V2_Harami			105
#define V2_HaramiCross			106
#define V2_BullishCounterAttckLine	107
#define V2_PiercingLine			108
#define V2_MorningStar			109
#define V2_MorningDojiStar		110
#define V2_AbandonedBaby		111
#define V2_ThreeInsideUp		112
#define V2_ThreeOutsideUp		113
#define V2_UpsideGap2CrowsDown		114
#define V2_BullishDojiStar		115
#define V2_TweezersBottom		116

/* 상승지속형	*/
#define V2_ThreeWhiteSoldier		151
#define V2_AdvancedBlock		152
#define V2_StalledPattern		153
#define V2_UpsideGapTasuki		154
#define V2_UpsideGap3Method		155

/* 하락반전형	*/
#define V2_HangingMan			201
#define V2_BearishBeltHold		202
#define V2_BearishEngulfing		203
#define V2_HaramiBlack			204
#define V2_HaramiCrossBlack		205
#define V2_BearishCounterAttckLine	206
#define V2_DarkCrowsCover		207
#define V2_EveningStar			208
#define V2_EveningDojiStar		209
#define V2_AbandonedBabyBlack		210
#define V2_ThreeInsideDown		211
#define V2_ThreeOutsideDown		212
#define V2_UpsideGap2Crows		213
#define V2_BearishDojiStar		214
#define V2_TweezersTop			215

/* 하락지속형	*/
#define V2_ThreeBlackCrows		251
#define V2_IdenticalThreeCrows		252
#define V2_DownsideGapTasuki		253
#define V2_DownsideGap3Method		254
#define V2_ShootingStar			255

/* 상승반전형	*/
char	*to_up[20] = {
	"", "역망치형 ", "양봉망치형 ", "불리쉬벨트홀드 ", "상승장악형 ",
	"상승잉태형 ", "하락십자잉태형 ", "상승접선형 ", "관통형 ",
	"샛별형 ", "샛별십자형 ", "어밴던드베이비 ", "쓰리 인사이드업 ",
	"쓰리 아웃사이드업 ", "하락까마귀형 ", "베어리쉬도지스타 ",
	"바닥형집게 ",
	"END",
};
/* 상승지속형	*/
char	*con_up[20] = {
	"", "쓰리화이트솔져 ", "어드밴스불락 ", "스톨드패턴 ", "업사이드태스키 ",
	"업사이드갭쓰리 ",
	"END",
};
/* 하락반전형	*/
char	*to_down[20] = {
	"", "음봉망치형 ", "베어리쉬벨트홀드 ", "하락장악형 ", "하락잉태형 ",
	"상승십자잉태형 ", "하락접선형 ", "먹구름형 ", "석별형 ", "석별십자형 ",
	"어밴던드베이비 ", "쓰리인사이드다운 ", "쓰리아웃사이드다운 ", "상승까마귀형 ",
	"베어리쉬도지스타 ", "천정형집게 ",
	"END",
};
/* 하락지속형	*/
char	*con_down[20] = {
	"", "쓰리블랙크로우즈 ", "아이덴티컬쓰리크로즈 ", "하락돌파갭형 ",
	"다운사이드갭쓰리 ", "음봉역망치형 ",
	"END",
};

#define	V_AbandonedBabyBottom	1	/* 상승기아형		*/
#define	V_AbandonedBabyTop	2	/* 하락기아형		*/
#define	V_AdvanceBlock		3	/* 상승블럭형		*/
#define	V_CounterAttackBear	4	/* 하락반격형		*/
#define	V_CounterAttackBull	5	/* 상승반격형		*/
#define	V_DarkCloudCover	6	/* 먹구름형		*/
#define	V_EngulfingLineBear	7	/* 하락장악형		*/
#define	V_EngulfingLineBull	8	/* 상승장악형		*/
#define	V_EveningDojiStar	9	/* 십자석별형		*/
#define	V_EveningStar		10	/* 석별형		*/
#define	V_Hammer		11	/* 망치형		*/
#define	V_Hangingman		12	/* 교수형		*/
#define	V_HaramiBlack		13	/* 잉태형		*/
#define	V_HaramiCrossBlack	14	/* 하락십자잉태형	*/
#define	V_HaramiCrossWhite	15	/* 상승십자잉태형	*/
#define	V_HaramiWhite		16	/* 상승잉태형		*/
#define	V_InvertedHammer	17	/* 역망치형		*/
#define	V_MorningDojiStar	18	/* 십자샛별형		*/
#define	V_MorningStar		19	/* 샛별형		*/
#define	V_PiercingLine		20	/* 관통형		*/
#define	V_SeperatingLineBear	21	/* 하락격리형		*/
#define	V_SeperatingLineBull	22	/* 상승격리형		*/
#define	V_ShootingStar		23	/* 유성형		*/
#define	V_SideBySideWhiteGapDn	24	/* 하락나란히형		*/
#define	V_SideBySideWhiteGapUp	25	/* 상승나란히형		*/
#define	V_TasukiDownsideGap	26	/* 하락타스키갭형	*/
#define	V_TasukiUpsideGap	27	/* 상승타스키갭형	*/
#define	V_ThreeWhiteSoldiers	28	/* 적삼병		*/
#define	V_ThrustingLine		29	/* 상승관통형		*/
#define	V_UpsideGap2Crows	30	/* 까마귀형		*/

char	*pattern_name[] = {
	"",
	"상승기아형 ", "하락기아형 ", "상승블럭형 ", "하락반격형 ",
	"상승반격형 ", "먹구름형 ", "하락장악형 ", "상승장악형 ",
	"십자석별형 ", "석별형 ", "망치형 ", "교수형 ",
	"잉태형 ", "하락십자잉태형 ", "상승십자잉태형 ", "상승잉태형 ",
	"역망치형 ", "십자샛별형 ", "샛별형 ", "관통형",
	"하락격리형 ", "상승격리형 ", "유성형 ", "하락나란히형 ",
	"상승나란히형 ", "하락타스키갭형 ", "상승타스키갭형 ", "적삼병 ",
	"상승관통형 ", "까마귀형 ",
};

#define	N_PATTERN	10

struct	bunbong	{
	YMD	date;		/* 일자				*/
	HMS	time;		/* 시분				*/
	int	siga;		/* 시가				*/
	int	koga;		/* 고가				*/
	int	jega;		/* 저가				*/
	int	jgga;		/* 종가				*/
	char	giho;		/* 기호				*/
	int	diff;		/* 대비				*/
	int	gvol;		/* 거래량			*/
	int	xvol;		/* 누적 거래량			*/
};

struct  bonginf {
	int	nrec;
	int	type;		/* 1:date,2:date+time,3:time */
	char	cod2[16];
	struct	bunbong bong[400];
};


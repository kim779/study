#if !defined(AFX_CANDLEPATTERN_H__4ACBF377_47B2_48C0_9664_3FE8338C5894__INCLUDED_)
#define AFX_CANDLEPATTERN_H__4ACBF377_47B2_48C0_9664_3FE8338C5894__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CandlePattern.h : header file
//

// Minimum Period for Candle Pattern Analysis
#define	NEED_PERIOD_3			3
#define	NEED_PERIOD_4			4
#define	NEED_PERIOD_5			5
#define	NEED_PERIOD_10			10
#define NEED_PERIOD_20			20

// Period of Accumulation rise and fall rate(Base Close Price, Next 5 Days Close Price)
#define ACCUMULATION_PERIOD		6


// This structure is basic data of a candle
typedef struct _STCANDLE_DATA
{
	int		nDataCnt;

	double *dTime;
	double *dOpen;
	double *dHigh;
	double *dLow;
	double *dClose;

} STCANDLE_DATA, *LPSTCANDLE_DATA;



/////////////////////////////////////////////////////////////////////////////
// CCandlePattern

class CCandlePattern : public CObject
{
// Construction
public:
	CCandlePattern();
	virtual ~CCandlePattern();


// ***************************************************************************************
//	Get Basic Candle Data 
// ***************************************************************************************
public:
	// Get Height of a Candle
	double		GetCandleHeight(			LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Get Average Height of a Candle
	double		GetCandleAvgHeight(			LPSTCANDLE_DATA pstCandleData, int nDataPos);
	
	// Get Height of a Candle Body 
	double		GetCandleBodyHeight(		LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Get High Value of a Candle Body
	double		GetCandleBodyHighValue(		LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Get Low Value of a Candle Body
	double		GetCandleBodyLowValue(		LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Get Middle Value of a Candle Body
	double		GetCandleBodyMidValue(		LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Get Shadow-Up of a Candle
	double		GetCandleShadowUp(			LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Get Shadow-Down of a Candle
	double		GetCandleShadowDown(		LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Check White Candle
	BOOL		IsCandleWhite(				LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Check Black Candle
	BOOL		IsCandleBlack(				LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Check Long Candle
	BOOL		IsCandleLong(				LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Check Short Candle
	BOOL		IsCandleShort(				LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Check Doji Candle
	BOOL		IsCandleDoji(				LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Check Marubozu Candle
	BOOL		IsCandleMarubozu(			LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Check Trend-Up Candle
	BOOL		IsCandleTrendUp(			LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Check Trend-Down Candle
	BOOL		IsCandleTrendDown(			LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Check Gap-Up Candle
	BOOL		IsCandleGapUp(				LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Check Gap-Down Candle
	BOOL		IsCandleGapDown(			LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Check Harami Candle
	BOOL		IsCandleHarami(				LPSTCANDLE_DATA pstCandleData, int nDataPos);

	// Check Engulfing Candle
	BOOL		IsCandleEngulfing(			LPSTCANDLE_DATA pstCandleData, int nDataPos);



// ***************************************************************************************
//	Find Candle Pattern
// ***************************************************************************************
public:
	// ## 01. Hammer							(망치형)           														
	int			CDFind_Hammer(						LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);							
	// ## 02. Hanging Man						(교수형)           			                            		
	int			CDFind_HangingMan(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);						
	// ## 03. Inverted Hammer					(역망치형)         			                            		
	int			CDFind_InvertedHammer(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 04. Shooting Star						(유성형)           			                            		
	int			CDFind_ShootingStar(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 05. Bullish Belt Hold Line			(상승샅바형)       			
	int			CDFind_Bullish_BeltHoldLine(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);			
	// ## 06. Bearish Belt Hold Line			(하락샅바형)       			                                	
	int			CDFind_Bearish_BeltHoldLine(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);			
	// ## 07. Long White Body					(장대양봉)         			
	int			CDFind_Long_WhiteBody(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 08. Long Black Body					(장대음봉)         			                            		
	int			CDFind_Long_BlackBody(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 09. Bullish Harami					(상승잉태형)       			                            		
	int			CDFind_Bullish_Harami(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 10. Bearish Harami					(하락잉태형)       			                            		
	int			CDFind_Bearish_Harami(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 11. Bullish Engulfing					(상승장악형)       			
	int			CDFind_Bullish_Engulfing(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 12. Bearish Engulfing					(하락장악형)       			                                	
	int			CDFind_Bearish_Engulfing(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 13. Bullish Counterattack Line		(상승반격형)       			
	int			CDFind_Bullish_CounterattackLine(	LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);		
	// ## 14. Bearish Counterattack Line		(하락반격형)       			
	int			CDFind_Bearish_CounterattackLine(	LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);		
	// ## 15. Piercing Pattern					(관통형)           			
	int			CDFind_PiercingPattern(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 16. Dark Cloud Cover					(먹구름형)         			                            		
	int			CDFind_DarkCloudCover(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 17. Morning Star						(샛별형)           			                            		
	int			CDFind_MorningStar(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);						
	// ## 18. Evening Star						(석별형)           			                        			
	int			CDFind_EveningStar(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);						
	// ## 19. Morning Doji Star					(십자샛별형)       			                            		
	int			CDFind_MorningDojiStar(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 20. Evening Doji Star					(십자석별형)       			                            		
	int			CDFind_EveningDojiStar(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 21. Abandoned Baby In Downtrend		(상승기아형)       			
	int			CDFind_AbandonedBabyInDowntrend(	LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);		
	// ## 22. Abandoned Baby In Uptrend			(하락기아형)       			
	int			CDFind_AbandonedBabyInUptrend(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);		
	// ## 23. Three Indise Up					(상승잉태확인형)   			
	int			CDFind_ThreeIndiseUp(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 24. Three Indise Down					(하락잉태확인형)   			                            		
	int			CDFind_ThreeIndiseDown(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 25. Three Outside Up					(상승장악확인형)   			                            		
	int			CDFind_ThreeOutsideUp(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 26. Three Outside Down				(하락장악확인형)   			                                	
	int			CDFind_ThreeOutsideDown(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 27. Upside Gap Two Crows In Uptrend	(까마귀형)         			
	int			CDFind_UpsideGapTwoCrowsInUptrend(	LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);	
	// ## 28. Three White Soldier				(적삼병)           			
	int			CDFind_ThreeWhiteSoldier(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 29. Three Black Corws					(흑삼병)           			                                	
	int			CDFind_ThreeBlackCorws(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 30. Stalled Pattern					(지연형)           			                            		
	int			CDFind_StalledPattern(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 31. Advance Block						(블락형)           			                            		
	int			CDFind_AdvanceBlock(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 32. Upside Gap Tasuki					(상승타스키갭형)   			                            		
	int			CDFind_UpsideGapTasuki(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 33. Downside Gap Tasuki				(하락타스키갭형)   			                                	
	int			CDFind_DownsideGapTasuki(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 34. Mat-Hold							(매트형)           			
	int			CDFind_MatHold(						LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);							
	// ## 35. Bullish Tri Star					(상승십자별형)     			
	int			CDFind_Bullish_TriStar(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 36. Bearish Tri Star					(하락십자별형)     			                            		
	int			CDFind_Bearish_TriStar(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 37. Unique Three River Bottom			(희귀삼천바닥형)   			                            		
	int			CDFind_UniqueThreeRiverBottom(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);		
	// ## 38. Identical Three Crows				(동일흑삼병형)     			
	int			CDFind_IdenticalThreeCrows(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);			
	// ## 39. Bullish Breakaway					(상승탈출형)       			                                	
	int			CDFind_Bullish_Breakaway(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 40. Bearish Breakaway					(하락탈출형)       			                                	
	int			CDFind_Bearish_Breakaway(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 41. Two Crows							(약세까마귀형)     			                                	
	int			CDFind_TwoCrows(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);						
	// ## 42. Three Stars In The South			(남삼성형)         			
	int			CDFind_ThreeStarsInTheSouth(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);			
	// ## 43. Concealing Baby Swallow			(애기제비감추기형) 			
	int			CDFind_ConcealingBabySwallow(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);			
	// ## 44. Stick Sandwich					(막대샌드위치형)   			
	int			CDFind_StickSandwich(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 45. Bullish Kicking					(상승박차형)	   			                            		
	int			CDFind_Bullish_Kicking(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 46. Bearish Kicking					(하락박차형)       			                            		
	int			CDFind_Bearish_Kicking(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 47. Homing Pigeon						(전서구형)         			                            		
	int			CDFind_HomingPigeon(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 48. Ladder Bottom						(사다리바닥형)     			                            		
	int			CDFind_LadderBottom(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);					
	// ## 49. Matching Low						(동일저점형)       			                            		
	int			CDFind_MatchingLow(					LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);						
	// ## 50. Bullish Side-By-Side White Lines	(상승나란히형)     			
	int			CDFind_Bullish_SideBySideWhiteLines(LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);	
	// ## 51. Bearish Side-By-Side White Lines	(하락나란히형)     			
	int			CDFind_Bearish_SideBySideWhiteLines(LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);	
	// ## 52. Rising Three Methods				(상승삼법형)       			
	int			CDFind_RisingThreeMethods(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);				
	// ## 53. Falling Three Methods				(하락삼법형)       			                                	
	int			CDFind_FallingThreeMethods(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);			
	// ## 54. Bullish Separating Lines			(상승갈림길형)     			
	int			CDFind_Bullish_SeparatingLines(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);			
	// ## 55. Bearish Separating Lines			(하락갈림길형)     			
	int			CDFind_Bearish_SeparatingLines(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);			
	// ## 56. Bullish Three-Line Strike			(강세삼선반격형)   			
	int			CDFind_Bullish_ThreeLineStrike(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);		
	// ## 57. Bearish Three-Line Strike			(약세삼선반격형)   			
	int			CDFind_Bearish_ThreeLineStrike(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);		
	// ## 58. Upside Gap Three Methods			(상승갭삼법형)     			
	int			CDFind_UpsideGapThreeMethods(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);			
	// ## 59. Downside Gap Three Methods		(하락갭삼법형)     			
	int			CDFind_DownsideGapThreeMethods(		LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);		
	// ## 60. On Neck							(걸침형)           			
	int			CDFind_OnNeck(						LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);							
	// ## 61. In Neck							(진입형)           			                    				
	int			CDFind_InNeck(						LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);							
	// ## 62. Thrusting Line					(공격형)           			
	int			CDFind_ThrustingLine(				LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);		
	// ## 63. Bullish GraveStone				(강세 비석형)
	int			CDFind_Bullish_GraveStone(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);
	// ## 64. Bullish DragonFly					(강세 잠자리형)
	int			CDFind_Bullish_DragonFly(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);


	void		SetCandlePatternData(	CObject* pObjCandlePatternData, LPSTCANDLE_DATA pstCandleData, 
										int nCandlePtnType, int nStPos, int nEndPos);

public:
	// ## Find All Candle Pattern
	void		CDFind_All_CandlePattern(			LPSTCANDLE_DATA pstCandleData, CObject *pObjCandlePatternData);


public:
	CString		GetCandlePatternName(		int nCandlePatternType);
	int			GetCandlePatternTrend(		int nCandlePatternType);
	CString		GetCandlePatternTrendName(	int nCandlePatternType);
	int			GetCandlePatternTrust(		int nCandlePatternType);

	CString		GetCandlePatternHelpName(int nCandlePatternType);

// 2008.02.20 by LYH >>
protected:
	HWND m_hOcxWnd;
// 2008.02.20 by LYH <<
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANDLEPATTERN_H__4ACBF377_47B2_48C0_9664_3FE8338C5894__INCLUDED_)

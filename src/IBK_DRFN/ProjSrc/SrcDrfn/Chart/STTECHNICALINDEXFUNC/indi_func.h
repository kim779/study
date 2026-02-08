#ifndef _INDI_FUNC_H_
#define _INDI_FUNC_H_

#define MAX_TEMP_ARRAY_SIZE		10	/* 임시변수 배열수 */

// Chart OCX와 관련된 사항.
// 차트OCX는 틱등의 데이터가 많아질경우 과거 데이터를 지운다. 따라서 지표계산등에서도 이와 맞춰서
// 인덱스 작업을 해줘야 한다.
//[[
#define OCX_MAX_INDEX			2047		// 2046다음에 2047이어야 하지만 1998로 변한다.
#define OCX_DELETE_CNT			50			// 지워지는 데이터 카운트 수
//]]

#include "indi_define.h"

double yesStd(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesAccum(long lIsUpdateData, long lIndex, UNKNOWN_DATA_TYPE SRC_VAR, 
		TEMP_DATA_TYPE TEMP_VAR);

double yesAccumN(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesEMA(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesHighest(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesLowest(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesSMA(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesWMA(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR, 
		TEMP_DATA_TYPE TEMP_VAR);

double yesAMA(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR, 
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesCrossUp(long lIsUpdateData, long lIndex,
		UNKNOWN_DATA_TYPE SRC_VAR1,
		UNKNOWN_DATA_TYPE SRC_VAR2,
		TEMP_DATA_TYPE TEMP_VAR);

double yesCrossDown(long lIsUpdateData, long lIndex,
		UNKNOWN_DATA_TYPE SRC_VAR1,
		UNKNOWN_DATA_TYPE SRC_VAR2,
		TEMP_DATA_TYPE TEMP_VAR);

double yesNthHighestBar(long lIsUpdateData, long lIndex, long lNth, 
		long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR);

double yesNthHighest(long lIsUpdateData, long lIndex, long lNth, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR);

double yesNthLowestBar(long lIsUpdateData, long lIndex, long lNth, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR);

double yesNthLowest(long lIsUpdateData, long lIndex, long lNth, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR);

double yesMRO(long lIsUpdateData, long lIndex, long lPeriod, long lOccur,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR);

double yesSwingHigh(long lIsUpdateData, long lIndex, long lOccur, long lPeriod,
		long lLeftStrength, long lRightStrength,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR);

double yesSwingHighBar(long lIsUpdateData, long lIndex, long lOccur, 
		long lPeriod,
		long lLeftStrength, long lRightStrength,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR);

double yesSwingLow(long lIsUpdateData, long lIndex, long lOccur, long lPeriod,
		long lLeftStrength, long lRightStrength,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR);

double yesSwingLowBar(long lIsUpdateData, long lIndex, long lOccur, 
		long lPeriod,
		long lLeftStrength, long lRightStrength,
		UNKNOWN_DATA_TYPE SRC_VAR, TEMP_DATA_TYPE TEMP_VAR);

double yesCorrelation(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR1, UNKNOWN_DATA_TYPE SRC_VAR2,
		TEMP_DATA_TYPE TEMP_VAR);

/******************************************************************************/
/********************************* 지표함수 ***********************************/
/******************************************************************************/
double yesAccDist(long lIsUpdateData, long lIndex,
		UNKNOWN_DATA_TYPE OPEN_VAR,		  
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesADX(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR, UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		UNKNOWN_DATA_TYPE OPT_VAR5,
		UNKNOWN_DATA_TYPE OPT_VAR6,
		UNKNOWN_DATA_TYPE OPT_VAR7,
		UNKNOWN_DATA_TYPE OPT_VAR8,
		UNKNOWN_DATA_TYPE OPT_VAR9,
		UNKNOWN_DATA_TYPE OPT_VAR10,
		TEMP_DATA_TYPE TEMP_VAR);

double yesATR(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesBollBandDown(long lIsUpdateData, long lIndex, long lPeriod, 
		double dStdDev,
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesBollBandUp(long lIsUpdateData, long lIndex, long lPeriod, 
		double dStdDev,
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesBW(long lIsUpdateData, long lIndex,
		long lMaPeriod, long lRocPeriod, long lStoPeriod1, long lStoPeriod2,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3, 
		UNKNOWN_DATA_TYPE OPT_VAR4, 
		UNKNOWN_DATA_TYPE OPT_VAR5, 
		UNKNOWN_DATA_TYPE OPT_VAR6, 
		TEMP_DATA_TYPE TEMP_VAR);

double yesCCI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		TEMP_DATA_TYPE TEMP_VAR);

double yesCO(long lIsUpdateData, long lIndex,
			 long lShortPeriod, long lLongPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR);

double yesCSar(long lIsUpdateData, long lIndex, double dAF, double dMaxAF,
        UNKNOWN_DATA_TYPE HIGH_VAR,
        UNKNOWN_DATA_TYPE LOW_VAR,
        UNKNOWN_DATA_TYPE CLOSE_VAR,
        TEMP_DATA_TYPE TEMP_VAR);

double yesCV(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		TEMP_DATA_TYPE TEMP_VAR);

double yesDiMinus(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR);

double yesDiPlus(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR);

double yesDisparity(long lIsUpdateData, long lIndex, long lPeriod, 
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesEOM(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR);

double yesEnvelopeDown(long lIsUpdateData, long lIndex, long lPeriod, 
		double dPercent,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesEnvelopeUp(long lIsUpdateData, long lIndex, long lPeriod, 
		double dPercent,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesLRL(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR);

double yesLRS(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR);

double yesMACD(long lIsUpdateData, long lIndex, long lShortPeriod, 
		long lLongPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR);

double yesMACD_OSC(long lIsUpdateData, long lIndex, long lShortPeriod, 
		long lLongPeriod, long lPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR);

double yesMFI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR);

double yesMI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		UNKNOWN_DATA_TYPE OPT_VAR5,
		TEMP_DATA_TYPE TEMP_VAR);

double yesMomentum(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesNVI(long lIsUpdateData, long lIndex,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesOBV(long lIsUpdateData, long lIndex,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesOSCP(long lIsUpdateData, long lIndex, long lShortPeriod, 
		long lLongPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR);

double yesOSCV(long lIsUpdateData, long lIndex, long lShortPeriod, 
		long lLongPeriod,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

/*double yesPROC(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR);
*/
double yesROC(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesPVI(long lIsUpdateData, long lIndex,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesPVT(long lIsUpdateData, long lIndex,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesRSI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR);

double yesSar(long lIsUpdateData, long lIndex, double dAF, double dMaxAF,
        UNKNOWN_DATA_TYPE HIGH_VAR,
        UNKNOWN_DATA_TYPE LOW_VAR,
        UNKNOWN_DATA_TYPE CLOSE_VAR,
        TEMP_DATA_TYPE TEMP_VAR);

double yesSONAR(long lIsUpdateData, long lIndex, long lPeriod, long mPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR);

double yesSPrice(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE OPEN_VAR,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesSimrido(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

/*	yesStochasticsD -> yesStoSlowD	*/
double yesStochasticsD(long lIsUpdateData, long lIndex, long lPeriod1, 
		long lPeriod2, long lPeriod3,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		UNKNOWN_DATA_TYPE OPT_VAR5,
		UNKNOWN_DATA_TYPE OPT_VAR6,
		TEMP_DATA_TYPE TEMP_VAR);

/*	yesStochasticsK -> yesStoSlowK	*/
double yesStochasticsK(long lIsUpdateData, long lIndex, long lPeriod1, 
		long lPeriod2,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR);

double yesTRIX(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		UNKNOWN_DATA_TYPE OPT_VAR5,
		TEMP_DATA_TYPE TEMP_VAR);

double yesVHF(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesVR(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		TEMP_DATA_TYPE TEMP_VAR);

double yesVROC(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE VOLUME_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesWC(long lIsUpdateData, long lIndex,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesWILLA(long lIsUpdateData, long lIndex,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesWILLR(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

/*-----------------------------------------------------------------------------*/
/* 지표추가작업 (2005.7.4 ~													   */

double yesARatio(long lIsUpdateData, long lIndex, long lPeriod, 
				 UNKNOWN_DATA_TYPE HIGH_VAR,
				 UNKNOWN_DATA_TYPE LOW_VAR,
				 UNKNOWN_DATA_TYPE OPEN_VAR,
				 UNKNOWN_DATA_TYPE SRC_VAR, 
				 UNKNOWN_DATA_TYPE SRC_VAR1,
				 TEMP_DATA_TYPE TEMP_VAR);

double yesBRatio(long lIsUpdateData, long lIndex, long lPeriod, 
				 UNKNOWN_DATA_TYPE HIGH_VAR,
				 UNKNOWN_DATA_TYPE LOW_VAR,
				 UNKNOWN_DATA_TYPE CLOSE_VAR,
				 UNKNOWN_DATA_TYPE SRC_VAR, 
				 UNKNOWN_DATA_TYPE SRC_VAR1,
				 TEMP_DATA_TYPE TEMP_VAR);

double yesAroonUp(long lIsUpdateData, long lIndex, long lPeriod, 
				 UNKNOWN_DATA_TYPE HIGH_VAR,
				 TEMP_DATA_TYPE TEMP_VAR);

double yesAroonDown(long lIsUpdateData, long lIndex, long lPeriod, 
				 UNKNOWN_DATA_TYPE LOW_VAR,
				 TEMP_DATA_TYPE TEMP_VAR);

double yesSROC(long lIsUpdateData, long lIndex, 
			   long lPeriod1, long lPeriod2,
			   UNKNOWN_DATA_TYPE SRC_VAR1,
			   UNKNOWN_DATA_TYPE OPT_VAR1, 
			   UNKNOWN_DATA_TYPE OPT_VAR2,
			   TEMP_DATA_TYPE TEMP_VAR);

double yesForceIndex(long lIsUpdateData, long lIndex, long lPeriod, 
				UNKNOWN_DATA_TYPE VOLUME_VAR,
				UNKNOWN_DATA_TYPE SRC_VAR1,
				UNKNOWN_DATA_TYPE SRC_VAR2, 
				UNKNOWN_DATA_TYPE OPT_VAR1,				
				TEMP_DATA_TYPE TEMP_VAR);

double yesDEMA(long lIsUpdateData, long lIndex, long lPeriod, 
				UNKNOWN_DATA_TYPE SRC_VAR1,
				UNKNOWN_DATA_TYPE SRC_VAR2,
				UNKNOWN_DATA_TYPE OPT_VAR1,
				UNKNOWN_DATA_TYPE OPT_VAR2,				
				TEMP_DATA_TYPE TEMP_VAR);

double yesDEMA2(long lIsUpdateData, long lIndex, long lPeriod, 
				UNKNOWN_DATA_TYPE SRC_VAR1,
				UNKNOWN_DATA_TYPE SRC_VAR2,
				UNKNOWN_DATA_TYPE OPT_VAR1,
				UNKNOWN_DATA_TYPE OPT_VAR2,				
				TEMP_DATA_TYPE TEMP_VAR);

double yesTEMA(long lIsUpdateData, long lIndex, long lPeriod, 
				UNKNOWN_DATA_TYPE SRC_VAR1,
				UNKNOWN_DATA_TYPE SRC_VAR2,
				UNKNOWN_DATA_TYPE SRC_VAR3,
				UNKNOWN_DATA_TYPE OPT_VAR1,
				UNKNOWN_DATA_TYPE OPT_VAR2,
				UNKNOWN_DATA_TYPE OPT_VAR3,				
				TEMP_DATA_TYPE TEMP_VAR);

double yesTEMA2(long lIsUpdateData, long lIndex, long lPeriod, 
				UNKNOWN_DATA_TYPE SRC_VAR1,
				UNKNOWN_DATA_TYPE SRC_VAR2,
				UNKNOWN_DATA_TYPE SRC_VAR3,
				UNKNOWN_DATA_TYPE OPT_VAR1,
				UNKNOWN_DATA_TYPE OPT_VAR2,
				UNKNOWN_DATA_TYPE OPT_VAR3,				
				TEMP_DATA_TYPE TEMP_VAR);

double yesRMI(long lIsUpdateData, long lIndex, 
			  long lPeriod, long lKPeriod,
				UNKNOWN_DATA_TYPE SRC_VAR,
				UNKNOWN_DATA_TYPE RISE_VAR,
				UNKNOWN_DATA_TYPE FALL_VAR,
				TEMP_DATA_TYPE TEMP_VAR);

double yesDisparity_EMA(long lIsUpdateData, long lIndex, long lPeriod, 
						UNKNOWN_DATA_TYPE SRC_VAR,
						UNKNOWN_DATA_TYPE OPT_VAR1,		
						TEMP_DATA_TYPE TEMP_VAR);

double yesBinaryWave(long lIsUpdateData, long lIndex,
					 long lShortPeriod, long lLongPeriod, long lSignalPeriod, 
					 long lDisparityPeriod, long lROCPeriod, 
					 long lStoKPeriod, long lStoDPeriod,
					 UNKNOWN_DATA_TYPE HIGH_VAR,
					 UNKNOWN_DATA_TYPE LOW_VAR,
					 UNKNOWN_DATA_TYPE CLOSE_VAR,
					 UNKNOWN_DATA_TYPE MACD_VAL1,
					 UNKNOWN_DATA_TYPE MACD_VAL2,
					 UNKNOWN_DATA_TYPE MACD_VAL3,
					 UNKNOWN_DATA_TYPE MACD_VAL4,
					 UNKNOWN_DATA_TYPE STO_VAL1,
					 UNKNOWN_DATA_TYPE STO_VAL2,
					 UNKNOWN_DATA_TYPE STO_VAL3,
					 UNKNOWN_DATA_TYPE STO_VAL4,
					 TEMP_DATA_TYPE TEMP_VAR);

double yes7BinaryWave(long lIsUpdateData, long lIndex,
					 UNKNOWN_DATA_TYPE HIGH_VAR,
					 UNKNOWN_DATA_TYPE LOW_VAR,
					 UNKNOWN_DATA_TYPE CLOSE_VAR,
					 UNKNOWN_DATA_TYPE VOLUME_VAR,
					 UNKNOWN_DATA_TYPE MACD_VAL1,
					 UNKNOWN_DATA_TYPE MACD_VAL2,
					 UNKNOWN_DATA_TYPE MACD_VAL3,
					 UNKNOWN_DATA_TYPE MACD_VAL4,
					 UNKNOWN_DATA_TYPE STO_VAL1,
					 UNKNOWN_DATA_TYPE STO_VAL2,
					 UNKNOWN_DATA_TYPE STO_VAL3,
					 UNKNOWN_DATA_TYPE STO_VAL4,
					 UNKNOWN_DATA_TYPE CO_VAL1,
					 UNKNOWN_DATA_TYPE CO_VAL2,
					 UNKNOWN_DATA_TYPE CO_VAL3,
					 UNKNOWN_DATA_TYPE CO_VAL4,
					 UNKNOWN_DATA_TYPE CCI_VAL1,
					 UNKNOWN_DATA_TYPE CCI_VAL2,
					 UNKNOWN_DATA_TYPE CCI_VAL3,
					 UNKNOWN_DATA_TYPE SRC_VAL,
					 UNKNOWN_DATA_TYPE OPT_VAR1,	
					 TEMP_DATA_TYPE TEMP_VAR);

double yesPriceChUpper(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE HIGH_VAR,
					TEMP_DATA_TYPE TEMP_VAR);

double yesPriceChLower(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE LOW_VAR,
					TEMP_DATA_TYPE TEMP_VAR);

double yesStarcCenter(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE SRC_VAR1,
					TEMP_DATA_TYPE TEMP_VAR);

double yesStarcUpper(long lIsUpdateData, long lIndex, long lPeriod, long lPeriod1,
					UNKNOWN_DATA_TYPE HIGH_VAR,
					UNKNOWN_DATA_TYPE LOW_VAR,
					UNKNOWN_DATA_TYPE SRC_VAR1,
					UNKNOWN_DATA_TYPE SRC_VAR2,
					TEMP_DATA_TYPE TEMP_VAR);

double yesStarcLower(long lIsUpdateData, long lIndex, long lPeriod, long lPeriod1,
					UNKNOWN_DATA_TYPE HIGH_VAR,
					UNKNOWN_DATA_TYPE LOW_VAR,
					UNKNOWN_DATA_TYPE SRC_VAR1,
					UNKNOWN_DATA_TYPE SRC_VAR2,
					TEMP_DATA_TYPE TEMP_VAR);

double yesFormula(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE SRC_VAR1,
					UNKNOWN_DATA_TYPE SRC_VAR2,
					TEMP_DATA_TYPE TEMP_VAR);

double yesDRF(long lIsUpdateData, long lIndex,
					UNKNOWN_DATA_TYPE HIGH_VAR,
					UNKNOWN_DATA_TYPE LOW_VAR,
					UNKNOWN_DATA_TYPE CLOSE_VAR,
					TEMP_DATA_TYPE TEMP_VAR);

double yesNCO(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE CLOSE_VAR,
					TEMP_DATA_TYPE TEMP_VAR);

double yesCMF(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE HIGH_VAR,
					UNKNOWN_DATA_TYPE LOW_VAR,
					UNKNOWN_DATA_TYPE CLOSE_VAR,
					UNKNOWN_DATA_TYPE VOLUME_VAR,
					UNKNOWN_DATA_TYPE SRC_VAR,
					TEMP_DATA_TYPE TEMP_VAR);

double yesElderRayBear(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE LOW_VAR,
					UNKNOWN_DATA_TYPE SRC_VAR,
					UNKNOWN_DATA_TYPE OPT_VAR1,					
					TEMP_DATA_TYPE TEMP_VAR);

double yesElderRayBull(long lIsUpdateData, long lIndex, long lPeriod,
					UNKNOWN_DATA_TYPE HIGH_VAR,
					UNKNOWN_DATA_TYPE SRC_VAR,
					UNKNOWN_DATA_TYPE OPT_VAR1,					
					TEMP_DATA_TYPE TEMP_VAR);

double yesPsyhologicalLine(long lIsUpdateData, long lIndex, long lPeriod,
							UNKNOWN_DATA_TYPE SRC_VAR1,
							UNKNOWN_DATA_TYPE SRC_VAR2,
							TEMP_DATA_TYPE TEMP_VAR);

double yesStoFastK(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		TEMP_DATA_TYPE TEMP_VAR);

double yesStoFastD(long lIsUpdateData, long lIndex, long lPeriod1, 
		long lPeriod2,
		UNKNOWN_DATA_TYPE HIGH_VAR,
		UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR);

double yesTD(long lIsUpdateData, long lIndex, long lPeriod, 
			UNKNOWN_DATA_TYPE SRC_VAR1,
			UNKNOWN_DATA_TYPE SRC_VAR2,
			TEMP_DATA_TYPE TEMP_VAR);

double yesTDI(long lIsUpdateData, long lIndex, long lPeriod, 
			UNKNOWN_DATA_TYPE SRC_VAR,
			UNKNOWN_DATA_TYPE OPT_VAR1,
			UNKNOWN_DATA_TYPE OPT_VAR2,
			TEMP_DATA_TYPE TEMP_VAR);

double yesMarketPosition(long lIsUpdateData, long lIndex, long lPeriod, 
			UNKNOWN_DATA_TYPE SRC_VAR,
			UNKNOWN_DATA_TYPE OPT_VAR1,
			UNKNOWN_DATA_TYPE OPT_VAR2,
			UNKNOWN_DATA_TYPE OPT_VAR3,
			TEMP_DATA_TYPE TEMP_VAR);

double yesBandB(long lIsUpdateData, long lIndex, 
				long lPeriod, double dStdDev,
				UNKNOWN_DATA_TYPE CLOSE_VAR,
				UNKNOWN_DATA_TYPE SRC_VAR,
				TEMP_DATA_TYPE TEMP_VAR);

double yesBandWidth(long lIsUpdateData, long lIndex, 
					long lPeriod, double dStdDev,
					UNKNOWN_DATA_TYPE SRC_VAR,
					TEMP_DATA_TYPE TEMP_VAR);

double yesADXR(long lIsUpdateData, long lIndex, long lPeriod1, long lPeriod2,
		UNKNOWN_DATA_TYPE HIGH_VAR, UNKNOWN_DATA_TYPE LOW_VAR, UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE ADX_OPT1,
		UNKNOWN_DATA_TYPE ADX_OPT2,
		UNKNOWN_DATA_TYPE ADX_OPT3,
		UNKNOWN_DATA_TYPE ADX_OPT4,
		UNKNOWN_DATA_TYPE ADX_OPT5,
		UNKNOWN_DATA_TYPE ADX_OPT6,
		UNKNOWN_DATA_TYPE ADX_OPT7,
		UNKNOWN_DATA_TYPE ADX_OPT8,
		UNKNOWN_DATA_TYPE ADX_OPT9,
		UNKNOWN_DATA_TYPE ADX_OPT10,
		UNKNOWN_DATA_TYPE EMA_OPT1,
		UNKNOWN_DATA_TYPE EMA_OPT2,
		TEMP_DATA_TYPE TEMP_VAR);

double yesPDI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR, UNKNOWN_DATA_TYPE LOW_VAR, 
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR);

double yesMDI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR, UNKNOWN_DATA_TYPE LOW_VAR, 
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR1,
		UNKNOWN_DATA_TYPE OPT_VAR2,
		UNKNOWN_DATA_TYPE OPT_VAR3,
		UNKNOWN_DATA_TYPE OPT_VAR4,
		TEMP_DATA_TYPE TEMP_VAR);

double yesNDI(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE HIGH_VAR, UNKNOWN_DATA_TYPE LOW_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE PDI_OPT1,
		UNKNOWN_DATA_TYPE PDI_OPT2,
		UNKNOWN_DATA_TYPE PDI_OPT3,
		UNKNOWN_DATA_TYPE PDI_OPT4,
		UNKNOWN_DATA_TYPE MDI_OPT1,
		UNKNOWN_DATA_TYPE MDI_OPT2,
		UNKNOWN_DATA_TYPE MDI_OPT3,
		UNKNOWN_DATA_TYPE MDI_OPT4,
		TEMP_DATA_TYPE TEMP_VAR);

double yesQstic(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE OPEN_VAR,
		UNKNOWN_DATA_TYPE CLOSE_VAR,
		UNKNOWN_DATA_TYPE SRC_VAR1,
		TEMP_DATA_TYPE TEMP_VAR);

double yesDPO(long lIsUpdateData, long lIndex, long lPeriod,
		UNKNOWN_DATA_TYPE SRC_VAR,
		UNKNOWN_DATA_TYPE OPT_VAR,
		TEMP_DATA_TYPE TEMP_VAR);

double yesStandardError(long lIsUpdateData, long lIndex, long lPeriod,
						UNKNOWN_DATA_TYPE SRC_VAR,
						UNKNOWN_DATA_TYPE OPT_VAR1,
						UNKNOWN_DATA_TYPE OPT_VAR2,
						UNKNOWN_DATA_TYPE OPT_VAR3,
						UNKNOWN_DATA_TYPE OPT_VAR4,
						UNKNOWN_DATA_TYPE OPT_VAR5,
						TEMP_DATA_TYPE TEMP_VAR);

double yesSterrBnCenter(long lIsUpdateData, long lIndex, long lPeriod,
						UNKNOWN_DATA_TYPE SRC_VAR1,
						UNKNOWN_DATA_TYPE OPT_VAR1,
						UNKNOWN_DATA_TYPE OPT_VAR2,
						UNKNOWN_DATA_TYPE OPT_VAR3,
						UNKNOWN_DATA_TYPE OPT_VAR4,
						TEMP_DATA_TYPE TEMP_VAR);

double yesSterrBnUpper(long lIsUpdateData, long lIndex, long lPeriod, double dStdDev,
						UNKNOWN_DATA_TYPE SRC_VAR1,
						UNKNOWN_DATA_TYPE OPT_VAR1,
						UNKNOWN_DATA_TYPE OPT_VAR2,
						UNKNOWN_DATA_TYPE OPT_VAR3,
						UNKNOWN_DATA_TYPE OPT_VAR4,
						UNKNOWN_DATA_TYPE OPT_VAR5,
						UNKNOWN_DATA_TYPE OPT_VAR6,
						UNKNOWN_DATA_TYPE OPT_VAR7,
						UNKNOWN_DATA_TYPE OPT_VAR8,
						UNKNOWN_DATA_TYPE OPT_VAR9,
						TEMP_DATA_TYPE TEMP_VAR);

double yesSterrBnLower(long lIsUpdateData, long lIndex, long lPeriod, double dStdDev,
						UNKNOWN_DATA_TYPE SRC_VAR1,
						UNKNOWN_DATA_TYPE OPT_VAR1,
						UNKNOWN_DATA_TYPE OPT_VAR2,
						UNKNOWN_DATA_TYPE OPT_VAR3,
						UNKNOWN_DATA_TYPE OPT_VAR4,
						UNKNOWN_DATA_TYPE OPT_VAR5,
						UNKNOWN_DATA_TYPE OPT_VAR6,
						UNKNOWN_DATA_TYPE OPT_VAR7,
						UNKNOWN_DATA_TYPE OPT_VAR8,
						UNKNOWN_DATA_TYPE OPT_VAR9,
						TEMP_DATA_TYPE TEMP_VAR);

double yesMAO(long lIsUpdateData, long lIndex, long lShortPeriod, 
			  long lLongPeriod,
			  UNKNOWN_DATA_TYPE SRC_VAR,
			  UNKNOWN_DATA_TYPE OPT_VAR1,
			  UNKNOWN_DATA_TYPE OPT_VAR2,
			  TEMP_DATA_TYPE TEMP_VAR);

double yesSIGMA(long lIsUpdateData, long lIndex, long lPeriod, 
				UNKNOWN_DATA_TYPE SRC_VAR, 
				TEMP_DATA_TYPE TEMP_VAR);

/*-----------------------------------------------------------------------------*/
#endif



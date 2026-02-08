#ifndef _SCRIPT_DEF_H_
#define _SCRIPT_DEF_H_

#include "../Common_ST/RawDataDef.h"
#define NumericSeries	0
#define NumericSimple	1

#define INPUTS(Index,InputName,InputType,Value)				pSTActor->SetInputs(Index,#InputName,InputType,Value)
#define VARIABLES(HolderIndex,VarName,VarType,Value)		pSTActor->SetVariables(HolderIndex,#VarName,VarType,Value)
#define ARRAY(HolderIndex,ArrayName,ArrayType,Value,Size)	pSTActor->SetArray(HolderIndex, #ArrayName, ArrayType, Value, Size)
#define OPENDATA(Index)									pSTActor->GetRawData(RAWOPENDATA,Index)
#define HIGHDATA(Index)									pSTActor->GetRawData(RAWHIGHDATA,Index)
#define LOWDATA(Index)									pSTActor->GetRawData(RAWLOWDATA,Index)
#define CLOSEDATA(Index)								pSTActor->GetRawData(RAWCLOSEDATA,Index)
#define VOLUMEDATA(Index)								pSTActor->GetRawData(RAWVOLUMEDATA,Index)
#define AMOUNTDATA(Index)								pSTActor->GetRawData(RAWAMOUNTDATA,Index)
#define DAYOPENDATA(Index)								pSTActor->GetRawData(RAWDAYOPENDATA,Index)
#define DAYHIGHDATA(Index)								pSTActor->GetRawData(RAWDAYHIGHDATA,Index)
#define DAYLOWDATA(Index)								pSTActor->GetRawData(RAWDAYLOWDATA,Index)
#define DAYCLOSEDATA(Index)								pSTActor->GetRawData(RAWDAYCLOSEDATA,Index)
#define INDEX											pSTActor->GetRawData(RAWINDEX,0)
#define CURRENTBAR										pSTActor->GetRawData(RAWCURRENTBAR,0)



#define SUBINPUT(Index,Value)							pSTActor->SetSubInputsFrom(Index,Value)


#define SETVARHOLDERDATA(lHolderIndex, dIndex, dValue)		pSTActor->SetVarHolderData(lHolderIndex,dIndex, dValue)
#define GETVARHOLDERDATA(lHolderIndex, dIndex)				pSTActor->GetVarHolderData(lHolderIndex, dIndex)

#define SETINPUTHOLDERDATA(lHolderIndex, dIndex, dValue)	pSTActor->SetInputHolderData(lHolderIndex,dIndex, dValue)
#define GETINPUTHOLDERDATA(lHolderIndex, dIndex)			pSTActor->GetInputHolderData(lHolderIndex, dIndex)

#define SETFUNCDATA(dIndex,dValue)							pSTActor->SetFuncData(dIndex, dValue)
#define GETFUNCDATA(dIndex)									pSTActor->GetFuncData(dIndex)

#define SETARRAYHOLDERDATA(lHolderIndex, dIndex, dValue)		pSTActor->SetArrayHolderData(lHolderIndex,dIndex, dValue)
#define GETARRAYHOLDERDATA(lHolderIndex, dIndex)				pSTActor->GetArrayHolderData(lHolderIndex, dIndex)


#define DECLAREFUNCTION_(FuncName) double FuncName(CSTActor* pSTActor,long lIndex, BOOL bTemp0 = FALSE, BOOL bTemp1 = FALSE,BOOL bTemp2 = FALSE, BOOL bTemp3 = FALSE,BOOL bTemp4 = FALSE, BOOL bTemp5 = FALSE,BOOL bTemp6 = FALSE, BOOL bTemp7 = FALSE,BOOL bTemp8 = FALSE, BOOL bTemp9 = FALSE);
#define FUNCTIONBEGIN_(FuncName) double FuncName(CSTActor* pSTActor,long lIndex, BOOL bTemp0, BOOL bTemp1,BOOL bTemp2, BOOL bTemp3,BOOL bTemp4, BOOL bTemp5,BOOL bTemp6, BOOL bTemp7,BOOL bTemp8, BOOL bTemp9) { pSTActor->SubIn(lIndex);
#define FUNCTIONEND_(FuncName)	double dRetValue = pSTActor->GetFuncData(0);	pSTActor->SubOut();	return dRetValue;}
#define IFBEGIN_(Condition)	{ BOOL bCond = (Condition); pSTActor->IfIn();if(pSTActor->IfCondition(bCond)){pSTActor->BracketIn();
//#define ELSEIF_(Condition)	pSTActor->BracketOut();}else if(pSTActor->IfCondition(Condition)){pSTActor->BracketIn();
#define ELSE_				pSTActor->BracketOut();}else{pSTActor->BracketIn();
#define IFEND_				pSTActor->BracketOut();}pSTActor->IfOut(); }

#define FORBEGIN_(InitExpr, CondExpr, LoopExpr)	pSTActor->ForIn();for(InitExpr;CondExpr;LoopExpr){pSTActor->BracketIn();
#define FOREND_			pSTActor->BracketOut();}pSTActor->ForOut();



#endif // _SCRIPT_DEF_H_
// DrawCalculator.cpp: implementation of the CDrawCalculator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawCalculator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "PacketBase.h"
#include "DataMath.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawCalculator::CDrawCalculator()
{

}

CDrawCalculator::~CDrawCalculator()
{

}

int CDrawCalculator::GetDataDrawCount(CPacket* dataPacket, int dataCount)
{
	if(dataPacket == NULL || dataPacket->GetnumericDataList() == NULL)
		return 0;

	int dataDrawCount = 0;
	bool upCheck = true;
	bool downCheck = true;
	double data[4];
	POSITION startPos = dataPacket->GetnumericDataList()->GetHeadPosition();
	if(startPos == NULL)
		return 0;

	data[0] = data[1] = data[2] = data[3] = dataPacket->GetnumericDataList()->GetNext(startPos);

	for(int i = 1 ; i < dataCount ; i++){
		if(startPos == NULL)
			break;

		double checkData = dataPacket->GetnumericDataList()->GetNext(startPos);
		if(data[0] < checkData){
			dataDrawCount++;
			if(upCheck == true){                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
				data[3] = data[2];
				data[2] = data[1];
				data[1] = data[0];
				data[0] = checkData;
			}
			else{
				data[0] = checkData;
				upCheck = true;
			}
			downCheck = false;
		}
		else if(data[3] > checkData){
			dataDrawCount++;
			if(downCheck == true){
				data[0] = data[1];
				data[1] = data[2];
				data[2] = data[3];
				data[3] = checkData;			
			}
			else{
				data[3] = checkData;
				downCheck = true;			
			}
			upCheck = false;
		}		
	}
	return dataDrawCount;
}

bool CDrawCalculator::GetInitDataUpCheck(CPacket* dataPacket, int nUnitSize, POSITION startPos)
{
	if(dataPacket == NULL || dataPacket->GetnumericDataList() == NULL || startPos == NULL)
		return false;

	bool UPCheck = true;
	double checkData = dataPacket->GetnumericDataList()->GetNext(startPos);
	for(int i = 1; i < dataPacket->GetCount(); i++){
		if(startPos == NULL)
			break;
		double data = dataPacket->GetnumericDataList()->GetNext(startPos);
		if(data >= checkData + nUnitSize){
			UPCheck = true;
			break;
		}
		else if(data <= checkData - nUnitSize){
			UPCheck = false;
			break;
		}
	}
	return UPCheck;
}

int CDrawCalculator::PFdataDrawingCount(int dataCount, CPacket* dataPacket, POSITION startPos, bool UPCheck, int nUnitSize, int checkData, int checkCount, int checkConvert)
{
	if(dataPacket == NULL || dataPacket->GetnumericDataList() == NULL || startPos == NULL)
		return 1;

	int widthCount =1;
	for(int i = 1; i < dataCount; i++){
		if(startPos == NULL)
			break;

		int data = (int)dataPacket->GetnumericDataList()->GetNext(startPos);

		if(data >= checkData + nUnitSize && UPCheck == true){
			checkCount = (data - checkData) / nUnitSize;
			checkData = data;
		}
		else if(data >= checkData + nUnitSize && UPCheck == false){
			checkCount = (data - checkData) / nUnitSize;
			if(checkCount >= checkConvert){
				UPCheck = true;
				widthCount++;
				checkData = data;
			}
			else
				checkCount = 0;
		}
		else if(data <= checkData - nUnitSize && UPCheck == false){
			checkCount = (checkData - data) / nUnitSize;
			checkData = data;
		}
		else if(data <= checkData - nUnitSize && UPCheck == true){
			checkCount = (checkData - data) / nUnitSize;
			if(checkCount >= checkConvert){
				UPCheck = false;
				widthCount++;
				checkData = data;
			}
			else
				checkCount = 0;
		}
		else
			checkCount = 0;

	}
	
	return widthCount;
}

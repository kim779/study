#include "stdafx.h"
#include "../../Inc/ExOCXDLL.h"

int CExOCXDLL::CImplPackIn::GetData(char* buf, int bufLen)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, ImplPackIn)

	return pThis->Packet_GetData(buf, bufLen);
}


/////////////////////////////////////////////////////////
CExOCXDLL::CImplPackOut::CImplPackOut()
{
//	char *tmp = "------";
//	memcpy(m_aData, tmp, 6);
//	m_nLines = m_nBozoSize = 0;
}

// * 별표로 세팅했을 경우 데스크에서 컨트롤에게 크기를 확인한다.
// * 여기선 * 세팅이 없으므로 처리하지 않는다. 
// * 혹 필요하게 되면 구현하기 바람.
int CExOCXDLL::CImplPackOut::GetPackDataSize(char* pIn)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, ImplPackOut)
	return pThis->Packet_GetPackDataSize(pIn);
}

// * 컨트롤에게 넘겨주는 데이타가 사이즈가 정해진 경우의 데이타
int CExOCXDLL::CImplPackOut::SetData(char* buf, int bufLen)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, ImplPackOut)
	return pThis->Packet_SetData(buf, bufLen);
}

// * 컨트롤에게 넘겨주는 데이타가 *인경우일때의 데이타
// * 여기선 * 처리가 없으므로 처리하지 않는다. 
// * 혹 필요하게 되면 구현하기 바람.
int CExOCXDLL::CImplPackOut::SetData2(char* buf, int bufLen)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, ImplPackOut)
	return pThis->Packet_SetData2(buf, bufLen);
}

// 컨트롤에게 넘겨주는 데이타가 !인경우일때의 데이타
int CExOCXDLL::CImplPackOut::SetData3(char* buf, int bufLen)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, ImplPackOut) 
	return pThis->Packet_SetData3(buf, bufLen);
}

// * 여러타입지정
// * 위에서 언급한 타입도 가능
// * nType  : 1 -> SetData 스타일
// *          2 -> SetData2 스타일
// *          3 -> SetData3 스타일
// * 혹 어떤 스타일이 더 필요하게 될지 모르기 때문에 이 루틴을 추가함.
int CExOCXDLL::CImplPackOut::SetData999(int nType, char* buf, int bufLen)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, ImplPackOut)
	return pThis->Packet_SetData999(nType, buf, bufLen);
}

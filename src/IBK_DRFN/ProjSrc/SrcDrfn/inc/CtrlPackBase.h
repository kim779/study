#if !defined(AFX_0D1DA4A3_849F_11D4_B318_0048543A0D2E__INCLUDED_)
#define AFX_0D1DA4A3_849F_11D4_B318_0048543A0D2E__INCLUDED_

// #include "../include/CtrlPackBase.h"
#define		BCP_OK					0	// 성공
#define		BCP_ERROR				-1	// UnKnown Error

// Input Error	-100번대
#define		BCP_INPUT				-100

// Output Error	-200 번대
#define		BCP_OUTPUT				-200

class CBaseCtrlPackIn
{
public:
	// bufLen 크기를 갖는 buf 에 데이터를 받는다.
	// 컨트롤에서는 NULL 값이 들어가면 안되고, 앞에서부터 데이터를 채운다. 
	// 남은 공간은 스페이스를 넣는다.
	// 입력이 이루어지지 않았으면 에러 값을 넘겨준다.
	// 성공이면 0, 에러이면 에러값
	// 해당길이 외의 버퍼를 임의로 손대면 안된다.
	// NULL값을 넣으면 절대 안된다.
	virtual int GetData(char* buf, int bufLen){ASSERT(FALSE); return 0;}
};

class CBaseCtrlPackOut
{
public:
	// 가변크기를 갖는 컨트롤일 경우에 호출된다.
	// 컨트롤에서 사용할 데이타의 크기를 계산해서 넘겨준다.
	virtual int	GetPackDataSize(char* pIn){ASSERT(FALSE); return 0;}

	// 컨트롤에게 넘겨줄 패킷데이터를 길이와 함께 넘겨준다.
	// 데이타의 길이가 정해진 경우일때의 데이타
	virtual int SetData(char* buf, int bufLen){ASSERT(FALSE); return 0;}

	// 컨트롤에게 넘겨주는 데이타가 *인경우일때의 데이타
	virtual int SetData2(char* buf, int bufLen){ASSERT(FALSE); return 0;}

	// 컨트롤에게 넘겨주는 데이타가 !인경우일때의 데이타
	virtual int SetData3(char* buf, int bufLen){ASSERT(FALSE); return 0;}

	// * 여러타입지정
	// * 위에서 언급한 타입도 가능
	// * nType  : 1 -> SetData 스타일
	// *          2 -> SetData2 스타일
	// *          3 -> SetData3 스타일
	// * 혹 어떤 스타일이 더 필요하게 될지 모르기 때문에 이 루틴을 추가함.
	virtual int SetData999(int nType, char* buf, int bufLen) {ASSERT(FALSE); return 0;}
};



#endif // AFX_0D1DA4A3_849F_11D4_B318_0048543A0D2E__INCLUDED_
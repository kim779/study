#ifndef TRS_5000_LIB_H
#define TRS_5000_LIB_H

#include <windows.h>


////////////////////////////////////////////////////////
//  TRS Library로 부터 수신되는 메세지 헨들러를 정의합니다.
//  자세한 내용은 DEMO프로그램을 참조하십시오.
#define WM_TRS5000MSG              WM_USER+656

#define MINORID_PASSWD                       1
#define MINORID_TRSENABLE                    2
#define MINORID_TRSDISABLE                   3
#define MINORID_SESSIONCLOSE                 4


#define RCPHONE_SUCCESS                      1
#define RCPHONE_FAIL                         0
////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////
//   공통사항 : 만약 펑션콜 이후 FALSE가 리턴되는 경우는
//              포트를 클로즈후 다시 열어주십시오

/////////////////////////////////////////////////////////////
// TRS Initialize (Reserved)
// 아직 특별한 기능은 없습니다.
extern "C" EXPORT int  PASCAL PHONEPAD_InitDevice(void);



/////////////////////////////////////////////////////////////
// TRS의 입력 비밀번호의 길이를 셋합니다.
//       입력 대기시간을 셋합니다. 
//parameter    : len     - 입력 비밀번호의 길이(0~16)
//                       - 1~16은 입력받는자리수 fixed
//                       - 0은 입력받은 비밀번호 자리수 끝에 * or # (최대 16자)
//               timeout - 입력 대기시간을 셋합니다.(최대 60, 단위는 초단위)
//return Value : 1  성공
//               0 실패
extern "C" EXPORT int  PASCAL PHONEPAD_SetConfig(int len,int timeout);



/////////////////////////////////////////////////////////////
// TRS의 입력 받은 비밀 번호를 받아옵니다. 
//parameter    : data : String Buffer
//
//return Value : 1  성공
//               0 실패
extern "C" EXPORT int  PASCAL PHONEPAD_GetPassWD(char * data);


/////////////////////////////////////////////////////////////
// TRS의 입력 받은 비밀 번호를 받아옵니다. 
//parameter    : 
//
//return Value : String Data  성공
//               NULL         실패
extern "C" EXPORT char *  PASCAL PHONEPAD_GetPassWDSTR(void);


/////////////////////////////////////////////////////////////
// TRS의 상태를 확인합니다.
//parameter    : 
//
//return Value : 0         normal status
//               1         password
//               2         timeout or cancel
extern "C" EXPORT int  PASCAL PHONEPAD_Status(void);


/////////////////////////////////////////////////////////////
// TRS의 포트를 오픈합니다.
// parameter   : no - 매개변수 셋팅을 0으로 하면 자동으로 포트를 찾습니다.
//                    이외 1~255는 해당포트를 오픈합니다.
//return Value : 1  성공
//               0 실패
extern "C" EXPORT int  PASCAL PHONEPAD_PortOpen(int no);



/////////////////////////////////////////////////////////////
// TRS의 포트를 클로즈 합니다.
extern "C" EXPORT void  PASCAL PHONEPAD_PortClose(void);



///////////////// 1.8.0.1 just Koscom Surpported
// Koscom에서만 사용합니다.(동부에선 사용하지 마십시오)
// 파라미터로는 윈도우 메인헨들을 넘겨주시면됩니다.
// 메세지형식 WM_TRS5000MSG : 메세지 구분자
//              세부항목 WPARAM : MINORID_TRSENABLE  : 입력버튼
//                              : MINORID_TRSDISABLE : 취소버튼
extern "C" EXPORT int  PASCAL PHONEPAD_GetPWSignalWindow(HWND hWnd);
///////////////// 1.8.0.1 just Koscom Surpported

#endif

#pragma once

#define DF_TOTALWIDTH 350             //전체 스티커 리스트까지 포함 폭
#define DF_NOTEWIDTH 250              //스티커 폭
#define DF_NOTEHEIGHT 130             //스티커 높이
#define DF_NOTEHMARGIN 10          //스티커간 위아래 간격
#define DF_FUNCHEIGHT  80              //스티커리스트 상단의 검색 및 기능영역
#define DF_PANE_RIGHTMARGIN 10

#define DF_FUNCWND_EDITWIDTH    200  //기능역역의 검색 폭
#define DF_FUNCWND_EDITHEIGHT   20   //기능역역의 검색에디트 높이




#define DF_NOSCROLLRATE 10

#define MSG_FUNCWND_LOG  0x01

#define COLOR_FUNC_BACK_COL  RGB(238, 238, 238)
#define COLOR_COMLINE				RGB(176, 176, 176)

 
#define	COLOR_XCOMBX		RGB(255, 255, 255)
#define	COLOR_BARTEXT		RGB(  0,   0,   0)
#define	COLOR_BARBACKGROUND	RGB(255, 255, 255)//RGB(238, 247, 255)





static void _LOG(int scnt, ...)
{
	va_list argList;
	va_start(argList, scnt);

	CString sTmp, sResult;

	CTime time;
	time = CTime::GetCurrentTime();
	sResult.Format("%02d:%02d:%02d  ", time.GetHour(), time.GetMinute(), time.GetSecond());

	for (int i = 0; i < scnt; i++)
	{
		if (i == 0)
			sTmp.Format("<%s>", va_arg(argList, LPCTSTR));
		else
			sTmp.Format("[%s]", va_arg(argList, LPCTSTR));

		sResult += sTmp;
		if (i == 0)
			sResult += "   ";
		else
			sResult += " ";
	}
	va_end(argList);

	OutputDebugString("\r\n" + sResult);
}
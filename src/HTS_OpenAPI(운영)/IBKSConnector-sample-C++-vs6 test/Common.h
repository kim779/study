#ifndef __COMMON_H__
#define __COMMON_H__

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Transaction Key
#define TK_TR1001		(1)			
#define TK_TR1002		(2)			
#define TK_TR1003		(3)		
#define TK_TR1201		(4)			
#define TK_TR1201_01	(5)			
#define TK_TR1202		(6)			
#define TK_TR1211		(7)			
#define TK_TR1221		(8)			
#define TK_TR1231		(9)			
#define TK_TR1801		(10)
#define TK_TR1802		(11)
#define TK_TR1803		(12)
#define TK_TR1804		(13)
#define TK_TR3001		(14)	
#define TK_TR3002		(15)
#define TK_TR3003		(16)
#define TK_TR3201		(17)
#define TK_TR3202		(18)
#define TK_TR3211		(19)
#define TK_TR3221		(20)
#define TK_TR3231		(21)
#define TK_TR4003		(22)
#define TK_TR4004		(23)
#define TK_TR4101		(24)
#define TK_TR4201		(25)
#define TK_TR4202		(26)
#define TK_TR4221		(27)
#define TK_TR4223		(28)
#define TK_TR4224		(29)
#define TK_TR4303		(30)
#define TK_TR4401		(31)
#define TK_GETCODE		(32)
#define TK_TR4501		(33)
#define TK_TR4502		(34)
#define TK_TR4503		(35)
#define TK_TR4511		(36)
#define TK_TR4512		(37)
#define TK_TR4521		(38)
#define TK_TR4522		(39)
#define TK_TR4523		(40)
#define TK_TR4524		(41)
#define TK_TR4525		(42)
#define TK_TR4526		(43)
#define TK_TR4527		(44)
#define TK_TR1222		(45)
#define TK_TR3222		(46)
#define TK_TR3232		(47)
#define TK_TR3411		(48)
#define TK_TR8001		(100)
#define TK_GREEKS1		(150)
#define TK_GREEKS2		(151)

class StringProxy : public pair<LPCSTR,LPCSTR> {
public:
	StringProxy() : pair<LPCSTR,LPCSTR>() {}
	StringProxy(LPCSTR st, LPCSTR ed) : pair<LPCSTR, LPCSTR>(st, ed) {}
	string ToStr() { return string(first, second); }
	CString ToCStr() { return CString(first, second-first); }
};

class op_notice_spliter {
public:
	bool operator() (char ch)
	{
		return (ch=='\r')||(ch=='\n')||(ch=='\t');
	}
};



#endif
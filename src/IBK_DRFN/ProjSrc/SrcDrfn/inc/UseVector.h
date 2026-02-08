#ifndef __VECTOR__USE__HEADER__
#define	__VECTOR__USE__HEADER__

//#include "../../UseVector.h"

#include <vector>
#include <algorithm>
// FIDLIST는 벡터 테이블을 이용한다.
#if !defined(_WIN32_WCE)
	typedef std::vector<char*> CHARVECTOR;
	typedef std::vector<std::vector<char*> > CHARVECTOR2;
	typedef CHARVECTOR::iterator CHARVECTORIT;
	typedef CHARVECTOR2::iterator CHARVECTOR2IT;
#else
	typedef std::vector<TCHAR*> CHARVECTOR;
	typedef std::vector<std::vector<TCHAR*> > CHARVECTOR2;
	typedef CHARVECTOR::iterator CHARVECTORIT;
	typedef CHARVECTOR2::iterator CHARVECTOR2IT;
#endif

#endif //__VECTOR__USE__HEADER__

#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

/************************************************************************/
/* CEnvironment                                                         */
/************************************************************************/

class CEnvironment
{
private:
	CWnd*	m_axis;
public:
	CEnvironment(CWnd* m_wizard);
	~CEnvironment();
protected:
	virtual void LoadEnvironment() = 0;
	virtual void SaveEnvironment() = 0;
public:
	virtual void Refresh();
	virtual void Default() {}
};


/************************************************************************/
/* CMainEnvironment                                                     */
/************************************************************************/
class CMainEnvironment: public CEnvironment
{
public:
	int		m_maxChild;		// 최대 차일드윈도우 개수
	int		m_childPos;		// 기본 차일드 포지션
	int		m_startMap;		// 시작시 윈도우 오픈 옵션  1: 최종 2: 지정 other: 없음
	int		m_fontSize;		// 디폴트 폰트 사이즈
	bool	m_unflash;		// 반짝거림 없음
	bool	m_vsReflect;	// 가상화면간 트리거 여부
	int		m_msgPos;		// 메시지 포지션
	int		m_idleTime;		// Idle time
protected:
	virtual void LoadEnvironment();
	virtual void SaveEnvironment();
public:
	virtual void Default();
};


/************************************************************************/
/* CMarketEnvironment                                                   */
/************************************************************************/
enum { 
	open30min = 1,			// 장시작 30분전
	open10min = 21,			// 장시작 10분전
	open5min = 26,			// 장시작 5분전
	open1min = 30,			// 장시작 1분전
	open10sec = 31,			// 장시작 10초전
	open30sec = 33,			// 장시작 30초전
	close10min = 121,		// 장종료 10분전
	close5min = 126,		// 장종료 5분전
	close1min = 130,		// 장종료 1분전
	close10sec = 131,		// 장종료 10초전
	close30sec = 133		// 장종료 30초전
};

class CMarketEnvironment: public CEnvironment
{
public:
	int		m_msgPos;		// 장운영 메시지 포지션
protected:
	virtual void LoadEnvironment();
	virtual void SaveEnvironment();
public:
	virtual void Default();
};


#endif // __ENVIRONMENT_H__
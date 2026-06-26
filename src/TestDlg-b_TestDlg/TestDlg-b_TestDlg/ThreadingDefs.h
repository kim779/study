#pragma once
#ifndef DF_THREAD
#define DF_THREAD
#include <queue>
#include <mutex>
// CDlgSTD 대화 상자

class CMQue : public CObject
{
public:
	CMQue();
	virtual ~CMQue();

public:
	int	m_ssm;

	int	m_nBytes;
	char* m_pBytes;
};

template<typename T>
class ThreadSafeQueue
{
public:
	void push(const T& item)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push(item);
	}

	bool pop(T& item)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_queue.empty())
			return false;

		item = m_queue.front();
		m_queue.pop();
		return true;
	}

	bool empty() {
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.empty();
	}

private:
	std::queue<T> m_queue;
	std::mutex m_mutex;

};




template <typename T>
class ThreadRunner
{
public:
	static UINT AFX_CDECL Run(LPVOID lpvoid)
	{
		T* pWsh = reinterpret_cast<T*>(lpvoid);
		// 작업
		if (!pWsh) return 0;

		while (pWsh->m_running)
		{
			CMQue* pMQ = nullptr;

			if (pWsh->m_queRTM.pop(pMQ)) // 안전하게 pop
			{
				if (pMQ)
				{
					//pWsh->SendRTM(pMQ);
					delete pMQ;
				}
			}
			else
			{
				Sleep(1); // 큐가 비었으면 CPU 점유 낮춤
			}
		}
		return 0;
	}
};


#endif

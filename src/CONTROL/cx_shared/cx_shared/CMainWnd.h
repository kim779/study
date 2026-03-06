#pragma once


// CMainWnd
#include "ShMemory.h"
#include "../H/TickStore.h"

class CMainWnd : public CWnd
{
	DECLARE_DYNAMIC(CMainWnd)

public:
	CMainWnd(CWnd* pParent, void* pParam);
	virtual ~CMainWnd();

	std::unique_ptr<char[]> m_hkey;
	std::unique_ptr<char[]> m_mkey;

	std::unique_ptr<class CShMemory> m_ShMemory;
	std::unique_ptr<class CShMemory> m_SmMemory;

	CWnd* m_pWizard{};
	CString m_slog{};
	CString m_sRoot{};
	CString m_sName{};
	void   FileMove();
	CString Variant(int comm, CString data);
	CString Parser(CString& srcstr, CString substr);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);


public:
	struct SafeAlertItem {
		char  code[CODE_LEN];
		DWORD ts_ms;
		char  values[MAX_RTS_INDEX][SYMBOL_STR_LEN];  // 값 자체를 복사
		bool  hasValue[MAX_RTS_INDEX];
	};

	// 워커스레드 관련
	std::queue<SafeAlertItem*> m_alertQueue;
	std::mutex            m_alertMutex;
	std::thread           m_workerThread;
	std::atomic<bool>     m_workerRunning{ false };

	void WorkerThreadFunc();
	void StartWorkerThread();
	void StopWorkerThread();
	std::unordered_map<std::string, int>   m_tickCount;
	std::unordered_map<std::string, DWORD> m_tickFirst; // 첫 수신 시각
	DWORD m_tickCountStart{ 0 };                        // 카운팅 시작 시각
	std::queue<int> g_poolKeys;
	std::mutex g_poolMtx;

	//bool ReadTick(const char* code, TickSnapshot* out);

	std::unordered_map<HWND, RtsSubscription> g_subByHwnd;
	mutable std::shared_mutex g_subMtx; // read/write lock
	//mutable std::mutex g_subMtx; // read/write lock

	int EnsureSlotIndexForCode(const char* code);
	void DumpAllSlots();

	// 안전 문자열 복사 함수
	inline void CopyZ(char* dst, size_t cap, const char* src)
	{
		if (!dst || cap == 0) return;
		if (!src) { dst[0] = '\0'; return; }
		strncpy_s(dst, cap, src, _TRUNCATE);
	}

	// _alertR → TickSnapshot 변환 함수 (핵심)
	using PTR_T = DWORD; // 32bit라 DWORD OK (64bit면 uintptr_t)


	//update_ticker(int kind, struct _alertR* alertR)  에서 호출
	//
	void UpdateSnapshotFromAlert(TickSnapshot& s, const SafeAlertItem* item)
	{
		// seqlock begin
		int v = s.seq.load(std::memory_order_relaxed);
		s.seq.store(v + 1, std::memory_order_release);

		s.ts_ms = item->ts_ms;
		CopyZ(s.code, sizeof(s.code), item->code);

		// valid.reset() 없이 누적 저장
		for (int jj = 0; jj < MAX_RTS_INDEX; jj++)
		{
			if (item->hasValue[jj])
			{
				CopyZ(s.values[jj], SYMBOL_STR_LEN, item->values[jj]);
				s.valid.set(jj);
			}
		}

		// seqlock end
		s.seq.store(v + 2, std::memory_order_release);
	}
};



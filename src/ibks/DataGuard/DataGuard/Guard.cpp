#include "pch.h"
#define AXLOG_MODULE_TAG "DATAGUARD"
#include <bcrypt.h>
#include <string>
#include <vector>
#include <mutex>
#include "../../h/axlog.h"
#include "../../h/DataGuardApi.h"
#pragma comment(lib, "bcrypt.lib")   //SHA-256 and random number 

#define DG_EXPORT extern "C" __declspec(dllexport)

namespace {
	/*
	struct	_ledgerH {
		char	size[6]{};	// 0	전문길이		I	S	X	전문전체(Size-6)
		char	type[1]{};	// 6	전문구분		I	S		B:계정계, I:정보계, F:파일송수신, N:Notice 자동갱신(R:정보계, D:계정계)
		char	encf[2]{};	// 7	압축구분		I/O	S	X	전문압축/암호화구분(00:압축,암호화 안함)
		char	dptr[3]{};	// 9	데이터 옵셋		I/O	S		입/출력 데이타가 시작되는 위치(공통헤더를 포함)
		char	svcd[8]{};	// 12	서비스코드		I	Cf	●	서비스코드               ♥ TxHead(3) of Map
		char	cono[3]{};	// 20	회사번호		I	S		회사번호 "991"
		char	usid[16]{};	// 23	사용자ID		I	C		트랜잭션을 발생시킨 사용자 고유정보 (예:사용자 접속ID)
		char	cogb[2]{};	// 39	접속분류코드		I	S		접속 접속매체 (HT : 자체HTS)
		char	fep1[12]{};	// 41	접속채널번호		I		X	접속 순차번호(3)+일련번호(9)
		char	pbid[16]{};	// 53	트랜잭션추적ID		I	Cf	X	전문 추적ID ('@'로시작)  ♥ 로그인 성공시 받은값 유지
		char	puip[12]{};	// 69	공인IP			I	S		공인IP (168.126.63.1의 경우 "168126063001" 형태)
		char	pcip[12]{};	// 81	사설IP			I	C		사설IP (168.126.63.1의 경우 "168126063001" 형태)
		char	dept[3]{};	// 93	처리지점 번호		I			처리PC가 위치한 지점
		char	brno[3]{};	// 96	지점번호		I			계좌가 속한 지점
		char	term[8]{};	// 99	단말번호		I	Cm	●	매체코드 (직원 : 001 고객 : 002)
		char	lang[1]{};	// 107	언어구분		I	S		K:한글, E:영문, C:중국, J:일본
		char	time[9]{};	// 108	AP처리시각		O			요청 처리시각 (HHMMSSmmm)
		char	ecod[4]{};	// 117	메시지코드		O			오류코드 :(정상코드: 0000 ~ 0999, 비정상코드 : 1000 ~ 9999, 요청시 Space로 채움)
		char	eknd[1]{};	// 121	출력메시지구분		O			
		char	fep2[1]{};	// 122	압축요청구분		I		X	
		char	fkey[4]{};	// 123	기능키목록		I/O	Cm	●	입력: 기능키(1바이트) ♥ TxHead(2) of Map (5:확인 7:연속 C:전송)
		char	rcnt[4]{};	// 127	요청 레코드 개수	I	Cm	●	일반적으로 "0000"을 입력
		char	fil1[6]{};	// 131	Filler			-		X	
		char	next[1]{};	// 137	연속여부		I/O	C		연속 데이터 유무 (Y:연속있음, 이외의 값:연속없음)
		char	nkey[18]{};	// 138	연속키			I/O	C	●	연속처리를 위한 키값
		char	usiz[2]{};	// 156	가변시스템정보길이	I		X	IBK 고정	
		char	hsiz[2]{};	// 158	가변헤더정보길이	I/O		X	사용 : epwd와 함께 사용
		char	msiz[2]{};	// 160	가변메시지길이		O		X	성공 및 오류메시지 (요청시 "00"입력)
		char	fep3[10]{};	// 162	Reserved for  FEP		S	X	
		char	mkty[1]{};	// 172	시장구분			Cfm	●	1:현물 2:선옵 3:기타    ♥TxHead(0) of MAP
		char	odrf[1]{};	// 173	주문구분			Cfm	●	1:주문 2:조회 3:이체    ♥TxHead(1) of MAP
		char	evid[4]{};	// 174	Reserved for  I/F			X	
		char	svrm[12]{};	// 178	서버IP				S		서버IP (168.126.63.1의 경우 "168126063001" 형태)
		char	skip[1]{};	// 190	Flag for I/F
		char	fil2[49]{};	// 191	Filler					X	
		char	eact[1]{};	// 240	에러구분					에러구분(0:상태바 1:메세지박스)
		char	emsg[99]{};	// 241	오류메세지					코드(4)+메시지(94)(9999 오류입니다 !)
		char	epwd[44]{};	// 340	sha256암호화 된 계좌비밀번호*/
	//--rule table (built-in, edit and redeploy DLL to chagne) -- 
	//off: offset from data start (ledger include), len : field length
	//data offset 0                 384                                    384 + body
	//	| <----------ledger 384B----------> | <----body(map IO fields in IOORDER)----> |

	struct Field { int off; 
						 int len; 
						 int kind; };

	struct Rule { char svc[9]; 
						int mode; 
						int n; 
						Field f[4]; };

	const Rule g_rules[] = {
		{ "SACMT289", DG_LOG, 1 , {{390,24, DG_JUNO },}},
	};

	// applied to every ledger send regardless of svc
	const Field g_common[] = {
		{ 23, 16, DG_USERID },		// ledger usid = login sign id
	};
	const int g_commonMode = DG_LOG;
	
	//---session store--
	struct Entry { int kind; std::string hash; };

	std::mutex g_lock;
	std::vector<Entry> g_session;
	BYTE g_salt[16];
	bool g_saltOK = false;

	std::string Normalize(const char* v, int len)
	{
		std::string s;
		for (int ii = 0; ii < len && v[ii]; ii++)
		{
			if (isalnum((unsigned char)v[ii]))
				s += v[ii];
		}
		return s;
	}

	std::string Hash(const std::string& norm)
	{
		if (norm.empty())
			return "";
		if (!g_saltOK)
		{
			if (!BCRYPT_SUCCESS(BCryptGenRandom(NULL, g_salt, sizeof(g_salt), BCRYPT_USE_SYSTEM_PREFERRED_RNG)))
				return "";
			g_saltOK = true;
		}

		BYTE digest[32]{};
		BCRYPT_ALG_HANDLE  hAlg = NULL;
		BCRYPT_HASH_HANDLE hHash = NULL;
		bool ok = BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, NULL, 0))
			&& BCRYPT_SUCCESS(BCryptCreateHash(hAlg, &hHash, NULL, 0, NULL, 0, 0))
			&& BCRYPT_SUCCESS(BCryptHashData(hHash, g_salt, sizeof(g_salt), 0))
			&& BCRYPT_SUCCESS(BCryptHashData(hHash, (PUCHAR)norm.data(), (ULONG)norm.size(), 0))
			&& BCRYPT_SUCCESS(BCryptFinishHash(hHash, digest, sizeof(digest), 0));
		if (hHash)	BCryptDestroyHash(hHash);
		if (hAlg)	BCryptCloseAlgorithmProvider(hAlg, 0);
		if (!ok)
			return "";

		char hex[65];
		for (int ii = 0; ii < 32; ii++)
			sprintf_s(&hex[ii * 2], 3, "%02x", digest[ii]);
		return std::string(hex, 64);
	}

	bool Has(int kind, const std::string& h)
	{
		for (const Entry& e : g_session)
		{
			if (e.kind == kind && e.hash == h)
				return true;
		}
		return false;
	}

	bool HasKind(int kind)
	{
		for (const Entry& e : g_session)
		{
			if (e.kind == kind)
				return true;
		}
		return false;
	}

	const Rule* FindRule(const char* svc)
	{
		for (const Rule& r : g_rules)
		{
			if (!memcmp(r.svc, svc, 8))
				return &r;
		}
		return NULL;
	}

	//--API--
	void SessionClear(void)
	{
		std::lock_guard<std::mutex> lk(g_lock);
		g_session.clear();
		axlog(LOG_LOGIN, "[DG] session clear");
	}

	void SessionAdd(int kind, const char* value)
	{
		if (!value)
			return;

		std::lock_guard<std::mutex> lk(g_lock);
		std::string h = Hash(Normalize(value, (int)strlen(value)));
		if (h.empty() || Has(kind, h))
			return;
		g_session.push_back({ kind, h });
	//	axlog(LOG_LOGIN, "[DG] session add kind=%d hash=%.8s count=%d", kind, h.c_str(), (int)g_session.size());
	}

	void SessionRemove(int kind, const char* value)
	{
		if (!value)
			return;
		std::lock_guard<std::mutex> lk(g_lock);
		std::string h = Hash(Normalize(value, (int)strlen(value)));

		for (size_t ii = 0; ii < g_session.size(); ii++)
		{
			if (g_session[ii].kind == kind && g_session[ii].hash == h)
			{
				g_session.erase(g_session.begin() + ii);
				break;
			}
		}
		axlog(LOG_LOGIN, "[DG] session remove kind=%d count=%d", kind, (int)g_session.size());
	}

	void SessionClearKind(int kind)
	{
		std::lock_guard<std::mutex> lk(g_lock);
		for (size_t ii = g_session.size(); ii-- > 0; )
		{
			if (g_session[ii].kind == kind)
				g_session.erase(g_session.begin() + ii);
		}
		axlog(LOG_LOGIN, "[DG] session clear kind=%d count=%d", kind, (int)g_session.size());
	}
#define DG_SHAPE_LOG1
#ifdef DG_SHAPE_LOG		// test PC only: layout without values
#pragma message("WARNING: DG_SHAPE_LOG enabled - plaintext send data is logged. Do not deploy.")
	std::string Shape(const char* d, int len)
	{
		std::string s;
		s.reserve(len);
		for (int ii = 0; ii < len; ii++)
		{
			//unsigned char c = (unsigned char)d[ii];
			//s += isdigit(c) ? '9' : isalpha(c) ? 'A' : (c == ' ') ? '_' : (c == 0) ? '0' : '.';
			s += (d[ii] == 0) ? ' ' : d[ii];
		}
		return s;
	}
#endif

	int Check(const char* svc, const char* data, int len, int dir)
	{
		if (!svc || !data)
			return DG_PASS;

		std::lock_guard<std::mutex> lk(g_lock);
		int result = DG_PASS;

		// common fields: every send, log only on mismatch
		if (svc[0] != ' ' && svc[0] != '\0')	// blank svcd = info TR, ledger not filled
		{
			for (const Field& f : g_common)
			{
				if (!HasKind(f.kind) || f.off + f.len > len)
					continue;		// kind not registered (before login) -> skip
				std::string v = Normalize(data + f.off, f.len);
				if (!v.empty() && Has(f.kind, Hash(v)))
					continue;
				axlog(LOG_DATA, "[DG][Common] TR=%.8s dir=%d off=%d kind=%d vlen=%d own=0 mode=%d",
					svc, dir, f.off, f.kind, (int)v.size(), g_commonMode);
				if (g_commonMode > result)
					result = g_commonMode;
			}
		}

		const Rule* r = FindRule(svc);
		if (!r)
			return result;

#ifdef DG_SHAPE_LOG
		axlog(LOG_DATA, "[DG] check svc=%.8s dir=%d len=%d", svc, dir, len);
		axlog(LOG_DATA, "[DG] shape svc=%.8s len=%d [%s]", svc, len, Shape(data, len).c_str());
#endif
		for (int ii = 0; ii < r->n; ii++)
		{
			const Field& f = r->f[ii];
			int own = 0;
			std::string v;
			if (f.off + f.len <= len)
			{
				v = Normalize(data + f.off, f.len);
				own = (!v.empty() && Has(f.kind, Hash(v))) ? 1 : 0;	// empty field = not own
			}
			//dir 0 송신, 1 수신
			//kind 1 계좌, 2 아이디, 3 주민번호
			//own 0 불일치, 1 일치 
			//mode  불일치 처리 방식  1로그 2막기
			axlog(LOG_DATA, "[DG][Check]  TR=%.8s data direction=%d off=%d chk val Kind=%d vlen=%d val=%s chk res=%d res mode=%d",
				svc, dir, f.off, f.kind, (int)v.size(), (f.kind == DG_ACCOUNT) ? v.c_str() : "-", own, r->mode);
			if (!own && r->mode > result)
				result = r->mode;
		}
		return result;
	}

	const DG_API g_api = {
			sizeof(DG_API), DG_VERSION,
			SessionClear, SessionAdd, SessionRemove, Check,
			SessionClearKind,
	};
}  //namespace

DG_EXPORT const DG_API* DG_GetApi(int version)
{
	if (version > DG_VERSION)
		return NULL;		// caller is newer than this DLL
	return &g_api;
}
#include "pch.h"
#define AXLOG_MODULE_TAG "DATAGUARD"
#include <bcrypt.h>
#include <string>
#include <vector>
#include <mutex>
#include "../../h/axlog.h"
#include "../../h/DataGuardApi.h"
#pragma comment(lib, "bcrypt.lib")

#define DG_EXPORT extern "C" __declspec(dllexport)

namespace {
	//--rule table (built-in, edit and redeploy DLL to chagne) -- 
	//off: offset from data start (ledger include), len : field length
	struct Field { int off; int len; int kind; };
	struct Rule { char svc[9]; int mode; int n; Field f[4]; };

	const Rule g_rules[] = {
		//test rule: map puts own account into ledger fil2 
		{ "SAMFQ014", DG_LOG, 1 , {{228,12, DG_ACCOUNT }}},
	};
	
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
		axlog(LOG_LOGIN, "[DG] session add kind=%d hash=%.8s count=%d", kind, h.c_str(), (int)g_session.size());
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

	int Check(const char* svc, const char* data, int len, int dir)
	{
		if (!svc || !data)
			return DG_PASS;
		const Rule* r = FindRule(svc);
		if (!r)
			return DG_PASS;

		std::lock_guard<std::mutex> lk(g_lock);
		int result = DG_PASS;
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
			axlog(LOG_DATA, "[DG] check svc=%.8s dir=%d off=%d kind=%d val=%s own=%d mode=%d",
				svc, dir, f.off, f.kind, v.c_str(), own, r->mode);
			if (!own)
				result = r->mode;
		}
		return result;
	}

	const DG_API g_api = {
		sizeof(DG_API), DG_VERSION,
		SessionClear, SessionAdd, SessionRemove, Check,
	};
}  //namespace

DG_EXPORT const DG_API* DG_GetApi(int version)
{
	if (version > DG_VERSION)
		return NULL;		// caller is newer than this DLL
	return &g_api;
}
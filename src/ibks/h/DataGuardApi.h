// DataGuardApi.h - send/recv data guard (session ownership check).
// Shared by DataGuard.dll and axWizard. C ABI only.
// DG_API is append-only: never insert or reorder members.
#pragma once

#define DG_VERSION	1

// session data kinds (append-only)
#define DG_ACCOUNT	1
#define DG_USERID	2

// Check() result
#define DG_PASS		0		// ok, or service not protected
#define DG_LOG		1		// mismatch detected, log only
#define DG_BLOCK	2		// mismatch detected, caller must stop

// Check() direction
#define DG_SEND		0		// plaintext, before encryption
#define DG_RECV		1		// plaintext, after decryption

struct DG_API {
	int	size;				// sizeof(DG_API)
	int	version;			// DG_VERSION
	void	(*SessionClear)(void);
	void	(*SessionAdd)(int kind, const char* value);
	void	(*SessionRemove)(int kind, const char* value);
	// svc : 8 bytes, space padded, not null terminated (ledger svcd or trxC)
	// data: body including ledger, len: body length
	int	(*Check)(const char* svc, const char* data, int len, int dir);
	// append new functions here only
};

typedef const DG_API* (*DG_GETAPI)(int version);
#define DG_GETAPI_NAME	"DG_GetApi"

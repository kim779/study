#pragma once

#define DEFAULT_RSA_KEY_SIZE        (2048)


#ifndef ATPASSRSA_LIBRARY_STATIC
	#ifdef ATPASSRSA_LIBRARY_EXPORTS
		#define ATPASSRSA_LIBRARY_API __declspec(dllexport)
	#else
		#define ATPASSRSA_LIBRARY_API __declspec(dllimport)
	#endif // #ifdef ATPASSRSA_LIBRARY_EXPORTS
#else
	#define ATPASSRSA_LIBRARY_API
#endif // #ifndef ATPASSRSA_LIBRARY_STATIC


/**
 * CONTEXT 구조체 정의
 */
typedef struct tagATPASS_CONTEXT
{
	LPVOID	   _rsa_key   ;		/* RSA 키쌍      */
	BOOL       _key_loaded;		/* 키쌍 생성 여부 */
} ATPASS_CONTEXT, *LPATPASS_CONTEXT;


/**
 * CONTEXT 구조체를 초기화한다.
 *
 * @param {input}   ctx CONTEXT 구조체 포인터
 *
 * @return HRESULT
 */
extern "C" ATPASSRSA_LIBRARY_API HRESULT ATPASS_InitContext(LPATPASS_CONTEXT ctx);

/**
 * 초기화 시에 키쌍을 생성한다.
 *
 * @param {input}   ctx CONTEXT 구조체 포인터
 * @param {input}   bGenNewKeyPair 새로운 RSA 키쌍 생성 여부
 *
 * @return HRESULT
 */
extern "C" ATPASSRSA_LIBRARY_API HRESULT ATPASS_Initialize(LPATPASS_CONTEXT ctx, BOOL bGenNewKeyPair);

/**
 * 기존 생성한 키쌍을 삭제한다.
 *
 * @param {input}   ctx CONTEXT 구조체 포인터
 *
 * @return void
 */
extern "C" ATPASSRSA_LIBRARY_API void ATPASS_CleanUp(LPATPASS_CONTEXT ctx);

/**
 * 초기화 시 생성된 키쌍을 삭제하고 새로 생성한다.
 *
 * @param {input}   ctx CONTEXT 구조체 포인터
 *
 * @return HRESULT
 */
extern "C" ATPASSRSA_LIBRARY_API HRESULT ATPASS_GenerationKeyPair(LPATPASS_CONTEXT ctx);

/**
 * RSA키 BLOB의 사이즈를 가져온다.
 *
 * @param {input}   ctx CONTEXT 구조체 포인터
 *
 * @return 키BLOB의 사이즈
 *         -1: rsakey is null
 *         -2: public key is null
 *		   -3: CONTEXT is null
 */
extern "C" ATPASSRSA_LIBRARY_API int ATPASS_GetPublicKeyBlobSize(LPATPASS_CONTEXT ctx);

/**
 * RSA키의 모듈러스를 가져온다.
 *
 * @param {input}   ctx CONTEXT 구조체 포인터
 * @param {output}  modulus 모듈러스
 * @param {input}   blobSize modulus의 버퍼사이즈 (키BLOB의 길이)
 * @param {output}  mosulusSize 모듈러스의 실제 사이즈
 *
 * @return HRESULT
 */
extern "C" ATPASSRSA_LIBRARY_API HRESULT ATPASS_GetModulus(LPATPASS_CONTEXT ctx, PUCHAR modulus, ULONG blobSize, ULONG * modulusSize);

/**
 * ASN.1 Primitive 타입으로 공개키를 가져온다.
 *
 * @param {input}   ctx CONTEXT 구조체 포인터
 * @param {output}  asnPublicKey ASN.1 Primitive 타입의 공개키
 * @param {input}   blobSize asnPublicKey의 버퍼사이즈 (키BLOB의 길이)
 * @param {output}  asnPublicKeySize ASN.1 Primitive 타입의 실제 사이즈
 *
 * @return HRESULT
 */
extern "C" ATPASSRSA_LIBRARY_API HRESULT ATPASS_GetPublicAsn1Primitive(LPATPASS_CONTEXT ctx, PUCHAR asnPublicKey, ULONG blobSize, ULONG * asnPublicKeySize);

/**
 * 축약서명을 한다.
 *
 * @param {input}   ctx CONTEXT 구조체 포인터
 * @param {input}   plainText 서명원문
 * @param {input}   plainTextLength 서명원문의 길이
 * @param {output}  signHash 서명결과
 * @param {input}   signHashSize 서명결과의 버퍼사이즈
 * @param {output}  signLength 실제 서명결과의 길이
 *
 * @return HRESULT
 */
extern "C" ATPASSRSA_LIBRARY_API HRESULT ATPASS_GetSignHash(LPATPASS_CONTEXT ctx, PUCHAR plainText, ULONG plainTextSize, PUCHAR signHash, ULONG signHashSize, ULONG * signLength);


/**
 * 축약서명을 검증한다.
 *
 * @param {input}   ctx CONTEXT 구조체 포인터
 * @param {input}   public_key 축약서명 공개키
 * @param {input}   public_key_size 축약서명 공개키 길이
 * @param {input}   message 서명원문
 * @param {input}   message_size 서명원문 버퍼사이즈
 * @param {input}   signature 서명메시지
 * @param {input}   signature_size 서명메시지 버퍼사이즈
 *
 * @return HRESULT
 */
extern "C" ATPASSRSA_LIBRARY_API int ATPASS_VerifySignHash(LPATPASS_CONTEXT ctx, PUCHAR public_key, ULONG public_key_size, PUCHAR message, ULONG message_size, PUCHAR signature, ULONG signature_size);


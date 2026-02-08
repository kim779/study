

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_1201Print.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_CX_1201Print,0x7B493B1C,0xA83F,0x4998,0x9A,0x57,0x82,0xE2,0xCF,0x9A,0x03,0xB5);


MIDL_DEFINE_GUID(IID, DIID_IMainDlg,0xB55F180D,0x37A0,0x4B94,0xB8,0x7D,0x14,0xAE,0xFA,0x5A,0x60,0x7D);


MIDL_DEFINE_GUID(CLSID, CLSID_MainDlg,0x184D8FBC,0x0220,0x40D5,0x96,0x36,0x7E,0xAE,0x69,0x2C,0x49,0x02);


MIDL_DEFINE_GUID(IID, DIID_IMainWnd,0x22BD57D7,0x2027,0x474D,0xAC,0x86,0x32,0xDD,0x07,0x0C,0x98,0x9D);


MIDL_DEFINE_GUID(CLSID, CLSID_MainWnd,0xF8A8B87A,0xD133,0x410F,0x9C,0x25,0x02,0xF2,0x24,0xAA,0x9A,0xDB);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif




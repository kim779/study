

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Mar 23 15:55:28 2017
 */
/* Compiler settings for Wizard.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


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
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_WIZARDLib,0x95840AF6,0xB82B,0x42DE,0x94,0xC9,0x20,0x39,0x36,0x76,0x89,0xCB);


MIDL_DEFINE_GUID(IID, DIID__DWizard,0x540246C5,0x8AA5,0x46DE,0xB4,0xCB,0xE3,0xF2,0x91,0xD6,0x95,0xC3);


MIDL_DEFINE_GUID(IID, DIID__DWizardEvents,0xA2DD4943,0xF0C3,0x4AB0,0x85,0xC6,0xC5,0xD4,0xE5,0x33,0x0A,0x01);


MIDL_DEFINE_GUID(CLSID, CLSID_Wizard,0xB2816A99,0x87C0,0x4EC2,0xB9,0x7F,0x38,0x75,0xB8,0x6A,0xF2,0xC7);


MIDL_DEFINE_GUID(IID, DIID_IxScreen,0x892925C0,0x9BED,0x4662,0x99,0x32,0xDB,0x3E,0x6E,0xCB,0x32,0x9B);


MIDL_DEFINE_GUID(CLSID, CLSID_xScreen,0x63ED2000,0x869B,0x44C9,0xB4,0x3E,0x2D,0x37,0x67,0x58,0xC5,0x8F);


MIDL_DEFINE_GUID(IID, DIID_IxSystem,0x55F722FF,0x9F53,0x411D,0xB7,0xBA,0x86,0x33,0xBD,0x30,0x08,0x85);


MIDL_DEFINE_GUID(CLSID, CLSID_xSystem,0x9B4CF2DB,0x3D25,0x4DFF,0xB8,0xFA,0xF6,0x34,0x09,0x26,0x84,0xA3);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif




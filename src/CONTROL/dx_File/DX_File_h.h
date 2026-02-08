

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for DX_File.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __DX_File_h_h__
#define __DX_File_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFilex_FWD_DEFINED__
#define __IFilex_FWD_DEFINED__
typedef interface IFilex IFilex;

#endif 	/* __IFilex_FWD_DEFINED__ */


#ifndef __Filex_FWD_DEFINED__
#define __Filex_FWD_DEFINED__

#ifdef __cplusplus
typedef class Filex Filex;
#else
typedef struct Filex Filex;
#endif /* __cplusplus */

#endif 	/* __Filex_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __DX_File_LIBRARY_DEFINED__
#define __DX_File_LIBRARY_DEFINED__

/* library DX_File */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_DX_File,0x73EB261C,0xF502,0x4E85,0x9B,0x75,0xC3,0xD3,0x9E,0x94,0xA2,0x05);

#ifndef __IFilex_DISPINTERFACE_DEFINED__
#define __IFilex_DISPINTERFACE_DEFINED__

/* dispinterface IFilex */
/* [uuid] */ 


DEFINE_GUID(DIID_IFilex,0x5B749E8C,0xDF77,0x448D,0x94,0xDA,0xA4,0x12,0xD4,0x7B,0x27,0x9F);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5B749E8C-DF77-448D-94DA-A412D47B279F")
    IFilex : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IFilexVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFilex * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFilex * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFilex * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFilex * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFilex * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFilex * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFilex * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IFilexVtbl;

    interface IFilex
    {
        CONST_VTBL struct IFilexVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFilex_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFilex_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFilex_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFilex_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IFilex_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IFilex_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IFilex_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IFilex_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Filex,0x6E25AAFD,0x9E86,0x42FE,0xA3,0x2C,0x26,0x37,0x83,0xEB,0xCB,0x7C);

#ifdef __cplusplus

class DECLSPEC_UUID("6E25AAFD-9E86-42FE-A32C-263783EBCB7C")
Filex;
#endif
#endif /* __DX_File_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



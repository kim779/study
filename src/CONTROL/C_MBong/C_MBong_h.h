

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for C_MBong.odl:
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


#ifndef __C_MBong_h_h__
#define __C_MBong_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IGraphWnd_FWD_DEFINED__
#define __IGraphWnd_FWD_DEFINED__
typedef interface IGraphWnd IGraphWnd;

#endif 	/* __IGraphWnd_FWD_DEFINED__ */


#ifndef __GraphWnd_FWD_DEFINED__
#define __GraphWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class GraphWnd GraphWnd;
#else
typedef struct GraphWnd GraphWnd;
#endif /* __cplusplus */

#endif 	/* __GraphWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __C_MBong_LIBRARY_DEFINED__
#define __C_MBong_LIBRARY_DEFINED__

/* library C_MBong */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_C_MBong,0x4392FDEE,0x5488,0x4E0A,0xBD,0x78,0x95,0x3F,0x6A,0x4C,0x94,0x10);

#ifndef __IGraphWnd_DISPINTERFACE_DEFINED__
#define __IGraphWnd_DISPINTERFACE_DEFINED__

/* dispinterface IGraphWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IGraphWnd,0x0403AEEA,0xA549,0x471F,0x90,0x25,0xED,0x57,0xC1,0x33,0x92,0xDA);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("0403AEEA-A549-471F-9025-ED57C13392DA")
    IGraphWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IGraphWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGraphWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGraphWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGraphWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGraphWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGraphWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGraphWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGraphWnd * This,
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
    } IGraphWndVtbl;

    interface IGraphWnd
    {
        CONST_VTBL struct IGraphWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGraphWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGraphWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGraphWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGraphWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IGraphWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IGraphWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IGraphWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IGraphWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_GraphWnd,0x8C98EACA,0x711C,0x429D,0x9A,0xBC,0x47,0x2F,0x6F,0x8E,0x36,0xEC);

#ifdef __cplusplus

class DECLSPEC_UUID("8C98EACA-711C-429D-9ABC-472F6F8E36EC")
GraphWnd;
#endif
#endif /* __C_MBong_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



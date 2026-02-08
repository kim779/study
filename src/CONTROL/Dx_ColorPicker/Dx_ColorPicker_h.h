

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for Dx_ColorPicker.odl:
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


#ifndef __Dx_ColorPicker_h_h__
#define __Dx_ColorPicker_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IColorPickerWnd_FWD_DEFINED__
#define __IColorPickerWnd_FWD_DEFINED__
typedef interface IColorPickerWnd IColorPickerWnd;

#endif 	/* __IColorPickerWnd_FWD_DEFINED__ */


#ifndef __ColorPickerWnd_FWD_DEFINED__
#define __ColorPickerWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class ColorPickerWnd ColorPickerWnd;
#else
typedef struct ColorPickerWnd ColorPickerWnd;
#endif /* __cplusplus */

#endif 	/* __ColorPickerWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Dx_ColorPicker_LIBRARY_DEFINED__
#define __Dx_ColorPicker_LIBRARY_DEFINED__

/* library Dx_ColorPicker */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Dx_ColorPicker,0xC89314C3,0x5853,0x48BD,0x97,0xC4,0xDC,0xE4,0xD0,0x8F,0xDE,0x79);

#ifndef __IColorPickerWnd_DISPINTERFACE_DEFINED__
#define __IColorPickerWnd_DISPINTERFACE_DEFINED__

/* dispinterface IColorPickerWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IColorPickerWnd,0xA60C3483,0x0472,0x461D,0x8F,0x29,0x5F,0x7B,0x7C,0xE8,0xBC,0x6B);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A60C3483-0472-461D-8F29-5F7B7CE8BC6B")
    IColorPickerWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IColorPickerWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IColorPickerWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IColorPickerWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IColorPickerWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IColorPickerWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IColorPickerWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IColorPickerWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IColorPickerWnd * This,
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
    } IColorPickerWndVtbl;

    interface IColorPickerWnd
    {
        CONST_VTBL struct IColorPickerWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IColorPickerWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IColorPickerWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IColorPickerWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IColorPickerWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IColorPickerWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IColorPickerWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IColorPickerWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IColorPickerWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_ColorPickerWnd,0xCBB54D9A,0x60D5,0x4906,0x8D,0x1B,0xD9,0xB0,0x12,0x96,0x59,0x31);

#ifdef __cplusplus

class DECLSPEC_UUID("CBB54D9A-60D5-4906-8D1B-D9B012965931")
ColorPickerWnd;
#endif
#endif /* __Dx_ColorPicker_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



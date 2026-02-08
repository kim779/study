

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_FundPrinter.odl:
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


#ifndef __CX_FundPrinter_h_h__
#define __CX_FundPrinter_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPrinter_FWD_DEFINED__
#define __IPrinter_FWD_DEFINED__
typedef interface IPrinter IPrinter;

#endif 	/* __IPrinter_FWD_DEFINED__ */


#ifndef __Printer_FWD_DEFINED__
#define __Printer_FWD_DEFINED__

#ifdef __cplusplus
typedef class Printer Printer;
#else
typedef struct Printer Printer;
#endif /* __cplusplus */

#endif 	/* __Printer_FWD_DEFINED__ */


#ifndef __IFPrinter_FWD_DEFINED__
#define __IFPrinter_FWD_DEFINED__
typedef interface IFPrinter IFPrinter;

#endif 	/* __IFPrinter_FWD_DEFINED__ */


#ifndef __FPrinter_FWD_DEFINED__
#define __FPrinter_FWD_DEFINED__

#ifdef __cplusplus
typedef class FPrinter FPrinter;
#else
typedef struct FPrinter FPrinter;
#endif /* __cplusplus */

#endif 	/* __FPrinter_FWD_DEFINED__ */


#ifndef __IPrintDlg_FWD_DEFINED__
#define __IPrintDlg_FWD_DEFINED__
typedef interface IPrintDlg IPrintDlg;

#endif 	/* __IPrintDlg_FWD_DEFINED__ */


#ifndef __PrintDlg_FWD_DEFINED__
#define __PrintDlg_FWD_DEFINED__

#ifdef __cplusplus
typedef class PrintDlg PrintDlg;
#else
typedef struct PrintDlg PrintDlg;
#endif /* __cplusplus */

#endif 	/* __PrintDlg_FWD_DEFINED__ */


#ifndef __IMainWnd_FWD_DEFINED__
#define __IMainWnd_FWD_DEFINED__
typedef interface IMainWnd IMainWnd;

#endif 	/* __IMainWnd_FWD_DEFINED__ */


#ifndef __MainWnd_FWD_DEFINED__
#define __MainWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class MainWnd MainWnd;
#else
typedef struct MainWnd MainWnd;
#endif /* __cplusplus */

#endif 	/* __MainWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_FundPrinter_LIBRARY_DEFINED__
#define __CX_FundPrinter_LIBRARY_DEFINED__

/* library CX_FundPrinter */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_FundPrinter,0x35CD300E,0x1E79,0x488B,0x99,0xD0,0xA3,0x83,0xB8,0xBD,0x0E,0xEE);

#ifndef __IPrinter_DISPINTERFACE_DEFINED__
#define __IPrinter_DISPINTERFACE_DEFINED__

/* dispinterface IPrinter */
/* [uuid] */ 


DEFINE_GUID(DIID_IPrinter,0x522CFD24,0x117A,0x4BEE,0xAB,0x78,0x3C,0xEC,0x2B,0x5C,0x0B,0xFB);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("522CFD24-117A-4BEE-AB78-3CEC2B5C0BFB")
    IPrinter : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IPrinterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPrinter * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPrinter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPrinter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPrinter * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPrinter * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPrinter * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPrinter * This,
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
    } IPrinterVtbl;

    interface IPrinter
    {
        CONST_VTBL struct IPrinterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPrinter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPrinter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPrinter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPrinter_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPrinter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPrinter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPrinter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IPrinter_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Printer,0xD9FF8DC6,0xE3E8,0x4CDC,0xAA,0x76,0x50,0x09,0x94,0xC2,0x54,0xDD);

#ifdef __cplusplus

class DECLSPEC_UUID("D9FF8DC6-E3E8-4CDC-AA76-500994C254DD")
Printer;
#endif

#ifndef __IFPrinter_DISPINTERFACE_DEFINED__
#define __IFPrinter_DISPINTERFACE_DEFINED__

/* dispinterface IFPrinter */
/* [uuid] */ 


DEFINE_GUID(DIID_IFPrinter,0x5EBD1A3F,0xD1A8,0x4B36,0x99,0xBC,0xF0,0x76,0x4B,0xA7,0xE3,0x10);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5EBD1A3F-D1A8-4B36-99BC-F0764BA7E310")
    IFPrinter : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IFPrinterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFPrinter * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFPrinter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFPrinter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFPrinter * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFPrinter * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFPrinter * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFPrinter * This,
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
    } IFPrinterVtbl;

    interface IFPrinter
    {
        CONST_VTBL struct IFPrinterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFPrinter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFPrinter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFPrinter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFPrinter_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IFPrinter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IFPrinter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IFPrinter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IFPrinter_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_FPrinter,0x66E84B79,0x2B5C,0x422E,0x9D,0xEB,0xD1,0x5A,0x31,0x35,0x03,0xAF);

#ifdef __cplusplus

class DECLSPEC_UUID("66E84B79-2B5C-422E-9DEB-D15A313503AF")
FPrinter;
#endif

#ifndef __IPrintDlg_DISPINTERFACE_DEFINED__
#define __IPrintDlg_DISPINTERFACE_DEFINED__

/* dispinterface IPrintDlg */
/* [uuid] */ 


DEFINE_GUID(DIID_IPrintDlg,0x31F284BA,0x8B08,0x4B11,0xBB,0xB3,0x8A,0xE8,0x6F,0x6A,0x06,0x72);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("31F284BA-8B08-4B11-BBB3-8AE86F6A0672")
    IPrintDlg : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IPrintDlgVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPrintDlg * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPrintDlg * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPrintDlg * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPrintDlg * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPrintDlg * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPrintDlg * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPrintDlg * This,
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
    } IPrintDlgVtbl;

    interface IPrintDlg
    {
        CONST_VTBL struct IPrintDlgVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPrintDlg_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPrintDlg_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPrintDlg_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPrintDlg_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPrintDlg_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPrintDlg_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPrintDlg_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IPrintDlg_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_PrintDlg,0x71A36926,0xB3C8,0x4ED5,0xBD,0x63,0xA7,0x26,0x77,0x4D,0x04,0xF2);

#ifdef __cplusplus

class DECLSPEC_UUID("71A36926-B3C8-4ED5-BD63-A726774D04F2")
PrintDlg;
#endif

#ifndef __IMainWnd_DISPINTERFACE_DEFINED__
#define __IMainWnd_DISPINTERFACE_DEFINED__

/* dispinterface IMainWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IMainWnd,0x059274D8,0xE747,0x48DB,0xBA,0x58,0xCC,0xF0,0x5A,0xD9,0x51,0xD0);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("059274D8-E747-48DB-BA58-CCF05AD951D0")
    IMainWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IMainWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMainWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMainWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMainWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMainWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMainWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMainWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMainWnd * This,
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
    } IMainWndVtbl;

    interface IMainWnd
    {
        CONST_VTBL struct IMainWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMainWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMainWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMainWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMainWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMainWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMainWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMainWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IMainWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MainWnd,0x68078129,0x659E,0x4E90,0x81,0x41,0xBF,0x95,0x0A,0x2B,0x51,0xFE);

#ifdef __cplusplus

class DECLSPEC_UUID("68078129-659E-4E90-8141-BF950A2B51FE")
MainWnd;
#endif
#endif /* __CX_FundPrinter_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



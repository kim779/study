#if !defined(AFX_AZIO_UTIL_CONTROL_BY_JUNOK_LEE__INCLUDED_)
#define AFX_AZIO_UTIL_CONTROL_BY_JUNOK_LEE__INCLUDED_

// #include "../../inc/AZUtil_Ctrl.h"

class CAZUtilCtrl
{
public:
	static HTREEITEM SearchFolderItem(CTreeCtrl* pTreeCtrl, LPCSTR szText, BOOL bSub=TRUE)
	{
		HTREEITEM hCurItem = pTreeCtrl->GetRootItem();
		HTREEITEM hNextItem;
		CString szTitle;
		while (hCurItem != NULL)
		{
			szTitle = pTreeCtrl->GetItemText(hCurItem);
			if(bSub)
				if(szTitle.Compare(szText)==0) return hCurItem;
			else
			if(szTitle.Find(szText)>=0) return hCurItem;

			hNextItem = pTreeCtrl->GetNextItem(hCurItem, TVGN_NEXT);
			hCurItem = hNextItem;
		}
		return NULL;
	}

	static HTREEITEM SearchItem(CTreeCtrl* pTreeCtrl, HTREEITEM hStart, LPCSTR szText, BOOL bFullCheck, BOOL bSub=TRUE)
	{
		// Delete all of the children of hmyItem.
		if(!hStart) return NULL;

		HTREEITEM hCurItem = hStart;
		HTREEITEM hNextItem;
		CString szTitle;
		while (hCurItem != NULL)
		{
			if (bSub && pTreeCtrl->ItemHasChildren(hCurItem)) 
			{
				hNextItem = SearchItem(pTreeCtrl, pTreeCtrl->GetChildItem(hCurItem), szText, bFullCheck, bSub);
				if(hNextItem) return hNextItem;
			}
			else
			{
				szTitle = pTreeCtrl->GetItemText(hCurItem);
				if(bFullCheck)
				{
					if(szTitle.Compare(szText)==0) return hCurItem;
				}
				else
				{
					if(szTitle.Find(szText)>=0) return hCurItem;
				}
			}
			hNextItem = pTreeCtrl->GetNextItem(hCurItem, TVGN_NEXT);
			hCurItem = hNextItem;
		}
		return NULL;
	}

	static int WaitResult(HWND hWnd, DWORD p_nTimeOut)
	{
		DWORD dwStartTime = GetCurrentTime();	
		DWORD dwTimeOut = p_nTimeOut;
		MSG		msg;

		while( (GetCurrentTime() - dwStartTime) < dwTimeOut )
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE )) 
			{
				if (!hWnd || msg.message == WM_QUIT || msg.message == WM_CLOSE) 
				{
    				PostQuitMessage(msg.wParam);
					return 0;
       			}

   				TranslateMessage( &msg );
   				DispatchMessage( &msg );
			}
		}
		return 1;
	}

//	Microsoft Visual Studio\VC98\Include\WINBASE.H
//	#define VER_PLATFORM_WIN32s             0
//	#define VER_PLATFORM_WIN32_WINDOWS      1
//	#define VER_PLATFORM_WIN32_NT           2

	static int GetOSPlatformID()
	{
		OSVERSIONINFO OSversion;	
		OSversion.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
		::GetVersionEx(&OSversion);
		return OSversion.dwPlatformId;
		/*
		switch(OSversion.dwPlatformId)
		{
		   case VER_PLATFORM_WIN32s: 
				   m_sStr.Format("Windows %d.%d",OSversion.dwMajorVersion,OSversion.dwMinorVersion);
			   break;
		   case VER_PLATFORM_WIN32_WINDOWS:
  			  if(OSversion.dwMinorVersion==0)
				  m_sStr="Windows 95";  
			  else
				  if(OSversion.dwMinorVersion==10)  
				  m_sStr="Windows 98";
       			  else
				  if(OSversion.dwMinorVersion==90)  
				  m_sStr="Windows Me";
				  break;
		   case VER_PLATFORM_WIN32_NT:
  			 if(OSversion.dwMajorVersion==5 && OSversion.dwMinorVersion==0)
					 m_sStr.Format("Windows 2000 With %s", OSversion.szCSDVersion);
				 else	
				 if(OSversion.dwMajorVersion==5 &&   OSversion.dwMinorVersion==1)
					 m_sStr.Format("Windows XP %s",OSversion.szCSDVersion);
				 else	
			 if(OSversion.dwMajorVersion<=4)
				m_sStr.Format("Windows NT %d.%d with %s",
								   OSversion.dwMajorVersion,
								   OSversion.dwMinorVersion,
								   OSversion.szCSDVersion);			
				 else	
					 //for unknown windows/newest windows version
				m_sStr.Format("Windows %d.%d ",
								   OSversion.dwMajorVersion,
								   OSversion.dwMinorVersion);
				 break;
		}
		*/
	}

	static BOOL IsWindowsNT()
	{
		if (GetVersion()<0x80000000) return TRUE;
		else	return FALSE;
	}

};

#endif //AFX_AZIO_UTIL_CONTROL_BY_JUNOK_LEE__INCLUDED_

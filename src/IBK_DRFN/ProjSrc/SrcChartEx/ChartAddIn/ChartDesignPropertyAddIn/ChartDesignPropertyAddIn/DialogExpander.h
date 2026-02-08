
// Expanding and collapsing dialogs
//
// Michael Walz (walz@epsitec.ch)
// Dec. 99
//

#ifndef _INC_EXPANDDLG_H_
#define _INC_EXPANDDLG_H_

class CExpandDialog
{
  BOOL m_bIsInitialized ;
  BOOL m_bIsExpanded ;
  int m_IdCollapsedMark ;
  CWnd *m_pDialog ;
  CString m_sTextMore ;
  CRect m_dialogrect ;

public:
  CExpandDialog();

// call this function from the OnInitDialog() function
  void Initialize(CWnd *pDialog,			// pointer to dialog
				  BOOL bInitiallyExpanded,	// TRUE => dialog is initially expanded
				  int IdCollapsedMark);		// id of static that indicates position of 
											// the bottom of the collapsed dialog

  void OnExpandButton(bool m_bIsExpanded);					// call this in the handler of the
											// "More"/"Less" button
} ;


#endif /* _INC_EXPANDDLG_H_ */
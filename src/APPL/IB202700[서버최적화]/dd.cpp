/*			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			CMapStringToString	mapGroup;
			pWnd->SendMessage(WM_MANAGE, MK_GETGROUP, (LPARAM)&mapGroup);
			CGridData	sdata;
			int	kind = 0;
			int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
			int	max = MAX_LINE;
			CString	sztmp = _T(""), sztmp2 = _T("");

// 			if (max == MO_VISIBLE)
// 				max = GetRowCount();

			pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->FieldSetup(true);
				kind = m_GridWnd[ii]->GetKind();
				sdata.Reset();

				if (CAST_TREEID(kind)->kind == xINTEREST)
				{
					sztmp.Format("%02d", CAST_TREEID(kind)->value);
					if (mapGroup.Lookup(sztmp, sztmp2))
					{
						sdata.SetKind(kind);
						pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, max), (LPARAM)&sdata);				
					}
					else
						sdata.SetKind(0);

					m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)&sdata);	
				}

				m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);

			}
*/

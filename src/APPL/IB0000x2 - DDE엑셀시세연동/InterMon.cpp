#include "stdafx.h"
#include "InterMon.h"

void InterMon::Request_GroupList()
{
	int	sendL = 0;
	std::string sendB;
	sendB.resize(16 * 1024);
	const int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;

	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';
	sendTR(trUPDOWN, sendB.data(), sendL, TRKEY_GROUP);
}

void InterMon::Request_GroupCode(int iseq, int vidx)
{
	const int index = iseq;
	int sendL = 0;
	CString stmp;
	std::string sendB;
	sendB.resize(16 * 1024);

	const int isz_updn = sz_uinfo + 2;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'j';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';
	stmp.Format("%02d", index);
	memcpy((char*)&sendB[sz_uinfo], stmp, 2);
	sendTR(trUPDOWN, sendB.data(), sendL, TRKEY_INTER + vidx);
}

void InterMon::sendTR(CString trCode, char* datB, int datL, int key)
{
	std::unique_ptr<char[]> sendB = std::make_unique<char[]>(L_userTH + datL + 1);

	struct	_userTH* uTH;
	uTH = (struct _userTH*)sendB.get();

	strcpy(uTH->trc, trCode);
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB.get()[L_userTH], datB, datL);
	m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM)sendB.get());

	sendB = nullptr;
}

void InterMon::receiveOub(int key, CString data, int vidx)
{
	struct  _glistex {
		char    ngrs[2];           /* New Group Seqn       */
		char    ogrs[2];           /* Old Group Seqn       */
		char	gnam[30];
	};

	switch (key)
	{
	case TRKEY_GROUP:
	{
		const int cnt = atoi(data.Left(4));
		m_group_count = cnt;
		data = data.Mid(4);
		data.Trim();
		if (!data.IsEmpty() && cnt > 0)
		{
			m_groupname.clear();
			//std::vector<std::pair<CString, CString>> vGroupName;
			const gsl::span<_glistex> groupList((_glistex*)data.GetString(), cnt);

			std::sort(groupList.begin(), groupList.end(), [](auto item, auto item1) {
				return CString(item.ngrs, 2).Trim().Compare((CString(item1.ngrs, 2).Trim())) < 0;
			});

			for_each(groupList.begin(), groupList.end(), [&](auto item) {
				const CString groupKey = CString(item.ngrs, 2).Trim();
				const CString groupName = CString(item.gnam, 30).Trim();
				m_groupname.emplace_back(std::make_pair(groupName, groupKey));
			});


			//for (int vidx = 0; vidx < maxVIEW; vidx++)
			//	m_internm[vidx].ResetContent();

			//for (int i = 0; i < cnt; i++)
			//{
			//	for (int vidx = 0; vidx < maxVIEW; vidx++)
			//	{
			//		const int index = m_internm[vidx].AddString(vGroupName[i].first);
			//		m_internm[vidx].SetItemData(index, atoi(vGroupName[i].second));
			//	}
			//}

			///const int count = m_internm[0].GetCount();

			//if (m_pTreeWnd)
			//	m_pTreeWnd->makeInterGroup(vGroupName);

			//if (m_pToolWnd)
			//	m_pToolWnd->addGroup(vGroupName);

			//loadGroupInfo();
		}
	}
	break;
	case TRKEY_INTER:
	{
		const CString skey = data.Left(2);
		const CString sGroupName = data.Mid(2, 20);
		const CString sCount = data.Mid(22, 4);
		//
		data = data.Mid(26);

		const gsl::span<_jinfo> codelist((_jinfo*)data.GetString(), atoi(sCount));


		m_inters.RemoveAll();

		for_each(codelist.begin(), codelist.end(), [&](auto item) {
			//			auto pinters = _v_inters.emplace_back(std::make_shared<_inters>());
			std::shared_ptr<_inters> pinters = std::make_shared<_inters>();
			m_inters.Add(pinters);

			memcpy(&pinters.get()->code[0], &item.code[0], 12);

			CString	name, code;

			memcpy(code.GetBufferSetLength(12), &item.code[0], 12);


			code.TrimRight();

			if (code.GetLength() == 6 || code.GetLength() == 8)
			{
				char wB[64]; strcpy(wB, code);
				char* xxP = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)wB);
				name = xxP;
				name.TrimLeft();
				name.TrimRight();
			}

			if (!name.IsEmpty()) {
				memcpy(&pinters.get()->name[0], name, 32);
			}
		});

		//loadInterest2(0, 0);
		//sendTransaction(vidx);
	}
	break;
	default:
		break;
	}
}
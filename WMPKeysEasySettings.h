//WMPKeysEasySettings.h : Declaration of the CWMPKeysEasySettings

#pragma once

#include "resource.h"	//main symbols
#include <atlhost.h>
#include "WMPKeysEasy.h"

//CWMPKeysEasySettings

class CWMPKeysEasySettings :
	public CDialogImpl<CWMPKeysEasySettings> {
public:
	CWMPKeysEasySettings()
		: m_WMPKeysEasy(NULL) {}

	~CWMPKeysEasySettings() {}

	enum { IDD = IDD_WMPKeysEasySETTINGS };

	BEGIN_MSG_MAP(CWMPKeysEasySettings)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		GetDlgItem(IDC_VOLUP).SendMessage(HKM_SETHOTKEY, m_WMPKeysEasy->m_volup, 0);
		GetDlgItem(IDC_VOLDOWN).SendMessage(HKM_SETHOTKEY, m_WMPKeysEasy->m_voldown, 0);
		GetDlgItem(IDC_FASTBACKWARD).SendMessage(HKM_SETHOTKEY, m_WMPKeysEasy->m_fastbackward, 0);
		GetDlgItem(IDC_FASTFORWARD).SendMessage(HKM_SETHOTKEY, m_WMPKeysEasy->m_fastforward, 0);
		GetDlgItem(IDC_PREV).SendMessage(HKM_SETHOTKEY, m_WMPKeysEasy->m_prev, 0);
		GetDlgItem(IDC_NEXT).SendMessage(HKM_SETHOTKEY, m_WMPKeysEasy->m_next, 0);
		GetDlgItem(IDC_PREVFRAME).SendMessage(HKM_SETHOTKEY, m_WMPKeysEasy->m_prevframe, 0);
		GetDlgItem(IDC_NEXTFRAME).SendMessage(HKM_SETHOTKEY, m_WMPKeysEasy->m_nextframe, 0);
		bHandled = TRUE;
		return 1;	//Let the system set the focus
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
		m_WMPKeysEasy->m_volup = GetDlgItem(IDC_VOLUP).SendMessage(HKM_GETHOTKEY);
		m_WMPKeysEasy->m_voldown = GetDlgItem(IDC_VOLDOWN).SendMessage(HKM_GETHOTKEY);
		m_WMPKeysEasy->m_fastbackward = GetDlgItem(IDC_FASTBACKWARD).SendMessage(HKM_GETHOTKEY);
		m_WMPKeysEasy->m_fastforward = GetDlgItem(IDC_FASTFORWARD).SendMessage(HKM_GETHOTKEY);
		m_WMPKeysEasy->m_prev = GetDlgItem(IDC_PREV).SendMessage(HKM_GETHOTKEY);
		m_WMPKeysEasy->m_next = GetDlgItem(IDC_NEXT).SendMessage(HKM_GETHOTKEY);
		m_WMPKeysEasy->m_prevframe = GetDlgItem(IDC_PREVFRAME).SendMessage(HKM_GETHOTKEY);
		m_WMPKeysEasy->m_nextframe = GetDlgItem(IDC_NEXTFRAME).SendMessage(HKM_GETHOTKEY);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
		EndDialog(wID);
		return 0;
	}

public:
	void SetWMPKeysEasy(CWMPKeysEasy* pWMPKeysEasy) {
		m_WMPKeysEasy = pWMPKeysEasy;
	}
	CWMPKeysEasy* m_WMPKeysEasy;
};

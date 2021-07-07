//WMPKeysEasy.cpp : Implementation of CWMPKeysEasy
//Copyright (c) Microsoft Corporation. All rights reserved.

#include "stdafx.h"
#include "WMPKeysEasy.h"
#include "WMPKeysEasySettings.h"

CComPtr<IWMPCore> g_core;
DWORD CWMPKeysEasy::m_volup;
DWORD CWMPKeysEasy::m_voldown;
DWORD CWMPKeysEasy::m_fastbackward;
DWORD CWMPKeysEasy::m_fastforward;
DWORD CWMPKeysEasy::m_prev;
DWORD CWMPKeysEasy::m_next;
DWORD CWMPKeysEasy::m_prevframe;
DWORD CWMPKeysEasy::m_nextframe;
HHOOK g_hHook;

static LRESULT CALLBACK keyHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		CComPtr<IWMPControls> controls;
		CComPtr<IWMPControls2> controls2;
		CComPtr<IWMPSettings> settings;
		CComPtr<IWMPMedia> media;
		WMPPlayState state;
		long volume;
		double position;
		g_core->get_controls(&controls);
		g_core->get_settings(&settings);
		g_core->get_playState(&state);
		g_core->get_currentMedia(&media);
		controls->QueryInterface((__uuidof(IWMPControls2)), (void**)&controls2);
		if (lParam & (1 << 31)) {
			UINT modifiers = 0;
			if (GetAsyncKeyState(VK_MENU))
				modifiers |= MOD_ALT;
			if (GetAsyncKeyState(VK_CONTROL))
				modifiers |= MOD_CONTROL;
			if (GetAsyncKeyState(VK_SHIFT))
				modifiers |= MOD_SHIFT;
			if (wParam == VK_SPACE) {
				if (state == wmppsPlaying)
					controls->pause();
				else if ((state == wmppsPaused) || (state == wmppsStopped) || (state == wmppsReady) || (state == wmppsUndefined))
					controls->play();
			}
			else if (modifiers == CWMPKeysEasy::GetModifiers(CWMPKeysEasy::m_volup) && wParam == LOBYTE(LOWORD(CWMPKeysEasy::m_volup))) {
				settings->get_volume(&volume);
				if (volume >= 95)
					settings->put_volume(100);
				else
					settings->put_volume(volume + 5);
			}
			else if (modifiers == CWMPKeysEasy::GetModifiers(CWMPKeysEasy::m_voldown) && wParam == LOBYTE(LOWORD(CWMPKeysEasy::m_voldown))) {
				settings->get_volume(&volume);
				if (volume <= 5)
					settings->put_volume(0);
				else
					settings->put_volume(volume - 5);
			}
			else if (modifiers == CWMPKeysEasy::GetModifiers(CWMPKeysEasy::m_fastbackward) && wParam == LOBYTE(LOWORD(CWMPKeysEasy::m_fastbackward))) {
				controls->get_currentPosition(&position);
				controls->put_currentPosition(position - 5);
			}
			else if (modifiers == CWMPKeysEasy::GetModifiers(CWMPKeysEasy::m_fastforward) && wParam == LOBYTE(LOWORD(CWMPKeysEasy::m_fastforward))) {
				controls->get_currentPosition(&position);
				controls->put_currentPosition(position + 5);
			}
			else if (modifiers == CWMPKeysEasy::GetModifiers(CWMPKeysEasy::m_prev) && wParam == LOBYTE(LOWORD(CWMPKeysEasy::m_prev)))
				controls->previous();
			else if (modifiers == CWMPKeysEasy::GetModifiers(CWMPKeysEasy::m_next) && wParam == LOBYTE(LOWORD(CWMPKeysEasy::m_next)))
				controls->next();
			else if (modifiers == CWMPKeysEasy::GetModifiers(CWMPKeysEasy::m_prevframe) && wParam == LOBYTE(LOWORD(CWMPKeysEasy::m_prevframe)))
				controls2->step(-1);
			else if (modifiers == CWMPKeysEasy::GetModifiers(CWMPKeysEasy::m_nextframe) && wParam == LOBYTE(LOWORD(CWMPKeysEasy::m_nextframe)))
				controls2->step(1);
		}
	}
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

//CWMPKeysEasy::CWMPKeysEasy
//Constructor

CWMPKeysEasy::CWMPKeysEasy() {}

//CWMPKeysEasy::~CWMPKeysEasy
//Destructor

CWMPKeysEasy::~CWMPKeysEasy() {}

//CWMPKeysEasy:::FinalConstruct
//Called when an plugin is first loaded. Use this function to do one-time
//intializations that could fail instead of doing this in the constructor,
//which cannot return an error.

HRESULT CWMPKeysEasy::FinalConstruct() {
	LoadSettings();
	g_hHook = SetWindowsHookEx(WH_KEYBOARD, keyHookProc, NULL, GetCurrentThreadId());
	return S_OK;
}

//CWMPKeysEasy:::FinalRelease
//Called when a plugin is unloaded. Use this function to free any
//resources allocated in FinalConstruct.

void CWMPKeysEasy::FinalRelease() {
	if (g_hHook != NULL) {
		UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
	}
	ReleaseCore();
}

//CWMPKeysEasy::SetCore
//Set WMP core interface

HRESULT CWMPKeysEasy::SetCore(IWMPCore* pCore) {
	HRESULT hr = S_OK;
	//release any existing WMP core interfaces
	ReleaseCore();
	//If we get passed a NULL core, this  means
	//that the plugin is being shutdown.
	if (pCore == NULL)
		return S_OK;
	m_spCore = pCore;
	g_core = pCore;
	return hr;
}

//CWMPKeysEasy::ReleaseCore
//Release WMP core interfaces

void CWMPKeysEasy::ReleaseCore() {
	if (m_spCore) {
		m_spCore = NULL;
		g_core = NULL;
	}
}

//CWMPKeysEasy::GetProperty
//Get plugin property

HRESULT CWMPKeysEasy::GetProperty(const WCHAR* pwszName, VARIANT* pvarProperty) {
	if (NULL == pvarProperty)
		return E_POINTER;
	return E_NOTIMPL;
}

//CWMPKeysEasy::SetProperty
//Set plugin property

HRESULT CWMPKeysEasy::SetProperty(const WCHAR* pwszName, const VARIANT* pvarProperty) {
	return E_NOTIMPL;
}

HRESULT CWMPKeysEasy::DisplayPropertyPage(HWND hwndParent) {
	CWMPKeysEasySettings settingsDialog;
	settingsDialog.SetWMPKeysEasy(this);
	if (settingsDialog.DoModal(hwndParent) == IDOK)
		SaveSettings();
	return S_OK;
}

HRESULT CWMPKeysEasy::LoadSettings(void) {
	CRegKey regSoftware;
	CRegKey regWMPKeysEasy;
	if (regSoftware.Open(HKEY_CURRENT_USER, _T("Software"), KEY_READ) != ERROR_SUCCESS)
		return E_FAIL;
	if (regWMPKeysEasy.Create((HKEY)regSoftware, _T("WMPKeysEasy")) != ERROR_SUCCESS)
		return E_FAIL;
	if (regWMPKeysEasy.QueryDWORDValue(_T("volup"), m_volup) != ERROR_SUCCESS)
		m_volup = (DWORD)MAKEWORD(VK_UP, HOTKEYF_EXT);
	if (regWMPKeysEasy.QueryDWORDValue(_T("voldown"), m_voldown) != ERROR_SUCCESS)
		m_voldown = (DWORD)MAKEWORD(VK_DOWN, HOTKEYF_EXT);
	if (regWMPKeysEasy.QueryDWORDValue(_T("fastbackward"), m_fastbackward) != ERROR_SUCCESS)
		m_fastbackward = (DWORD)MAKEWORD(VK_LEFT, HOTKEYF_EXT);
	if (regWMPKeysEasy.QueryDWORDValue(_T("fastforward"), m_fastforward) != ERROR_SUCCESS)
		m_fastforward = (DWORD)MAKEWORD(VK_RIGHT, HOTKEYF_EXT);
	if (regWMPKeysEasy.QueryDWORDValue(_T("prev"), m_prev) != ERROR_SUCCESS)
		m_prev = (DWORD)MAKEWORD(VK_LEFT, HOTKEYF_CONTROL | HOTKEYF_EXT);
	if (regWMPKeysEasy.QueryDWORDValue(_T("next"), m_next) != ERROR_SUCCESS)
		m_next = (DWORD)MAKEWORD(VK_RIGHT, HOTKEYF_CONTROL | HOTKEYF_EXT);
	if (regWMPKeysEasy.QueryDWORDValue(_T("prevframe"), m_prevframe) != ERROR_SUCCESS)
		m_prevframe = (DWORD)MAKEWORD(VK_LEFT, HOTKEYF_ALT | HOTKEYF_EXT);
	if (regWMPKeysEasy.QueryDWORDValue(_T("nextframe"), m_nextframe) != ERROR_SUCCESS)
		m_nextframe = (DWORD)MAKEWORD(VK_RIGHT, HOTKEYF_ALT | HOTKEYF_EXT);
	return S_OK;
}

void CWMPKeysEasy::SaveSettings(void) {
	CRegKey regSoftware;
	CRegKey regWMPKeysEasy;
	if (regSoftware.Open(HKEY_CURRENT_USER, _T("Software"), KEY_READ) != ERROR_SUCCESS)
		return;
	if (regWMPKeysEasy.Create((HKEY)regSoftware, _T("WMPKeysEasy")) != ERROR_SUCCESS)
		return;
	regWMPKeysEasy.SetDWORDValue(_T("volup"), m_volup);
	regWMPKeysEasy.SetDWORDValue(_T("voldown"), m_voldown);
	regWMPKeysEasy.SetDWORDValue(_T("fastbackward"), m_fastbackward);
	regWMPKeysEasy.SetDWORDValue(_T("fastforward"), m_fastforward);
	regWMPKeysEasy.SetDWORDValue(_T("prev"), m_prev);
	regWMPKeysEasy.SetDWORDValue(_T("next"), m_next);
	regWMPKeysEasy.SetDWORDValue(_T("prevframe"), m_prevframe);
	regWMPKeysEasy.SetDWORDValue(_T("nextframe"), m_nextframe);
}

//translate modifiers from Hot Key Control form to Virtual-Key code
UINT CWMPKeysEasy::GetModifiers(DWORD dwHotKeyCode) {
	BYTE wHKMod = HIBYTE(LOWORD(dwHotKeyCode));
	UINT uiRes = 0;
	if (wHKMod & HOTKEYF_ALT)
		uiRes |= MOD_ALT;
	if (wHKMod & HOTKEYF_CONTROL)
		uiRes |= MOD_CONTROL;
	if (wHKMod & HOTKEYF_SHIFT)
		uiRes |= MOD_SHIFT;
	return uiRes;
}

//WMPKeysEasy.h : Declaration of the CWMPKeysEasy
//Copyright (c) Microsoft Corporation. All rights reserved.

#ifndef __WMPKeysEasy_H_
#define __WMPKeysEasy_H_

#pragma once

#include "resource.h"
#include "wmpplug.h"

//{9B61F641-7794-4322-BF6A-E45EFD6C8D7C}
DEFINE_GUID(CLSID_WMPKeysEasy, 0x9b61f641, 0x7794, 0x4322, 0xbf, 0x6a, 0xe4, 0x5e, 0xfd, 0x6c, 0x8d, 0x7c);

//CWMPKeysEasy
class ATL_NO_VTABLE CWMPKeysEasy :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWMPKeysEasy, &CLSID_WMPKeysEasy>,
	public IWMPPluginUI {
public:
	CWMPKeysEasy();
	~CWMPKeysEasy();

	DECLARE_REGISTRY_RESOURCEID(IDR_WMPKeysEasy)

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CWMPKeysEasy)
		COM_INTERFACE_ENTRY(IWMPPluginUI)
	END_COM_MAP()

	//CComCoClass methods
	HRESULT FinalConstruct();
	void    FinalRelease();

	//IWMPPluginUI methods
	STDMETHODIMP SetCore(IWMPCore *pCore);
	STDMETHODIMP Create(HWND hwndParent, HWND *phwndWindow) { return E_NOTIMPL; }
	STDMETHODIMP Destroy() { return E_NOTIMPL; }
	STDMETHODIMP TranslateAccelerator(LPMSG lpmsg) { return E_NOTIMPL; }
	STDMETHODIMP DisplayPropertyPage(HWND hwndParent);
	STDMETHODIMP GetProperty(const WCHAR *pwszName, VARIANT *pvarProperty);
	STDMETHODIMP SetProperty(const WCHAR *pwszName, const VARIANT *pvarProperty);

private:
	void         ReleaseCore();
	CComPtr<IWMPCore>           m_spCore;
	HRESULT LoadSettings(void);

public:
	static DWORD m_volup;
	static DWORD m_voldown;
	static DWORD m_fastforward;
	static DWORD m_fastbackward;
	static DWORD m_next;
	static DWORD m_prev;
	static DWORD m_nextframe;
	static DWORD m_prevframe;
	void SaveSettings(void);
	//translate modifiers from Hot Key Control form to Virtual-Key code
	static UINT GetModifiers(DWORD dwHotKeyCode);
};

#endif  //__WMPKeysEasy_H_

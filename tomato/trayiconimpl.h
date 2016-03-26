// Implementation of the CNotifyIconData class and the CTrayIconImpl template.
#pragma once

#include <atlmisc.h>

// Wrapper class for the Win32 NOTIFYICONDATA structure
class CNotifyIconData : public NOTIFYICONDATA
{
public:	
	CNotifyIconData()
	{
		memset(this, 0, sizeof(NOTIFYICONDATA));
		cbSize = sizeof(NOTIFYICONDATA);
	}
};

// Template used to support adding an icon to the taskbar.
// This class will maintain a taskbar icon and associated context menu.
template <class T>
class CTrayIconImpl
{
private:
	UINT WM_TRAYICON;
    UINT WM_TASKBARCREATE;
	CNotifyIconData m_nid;
	bool m_bInstalled;
	UINT m_nDefault;
public:	
	CTrayIconImpl() : m_bInstalled(false), m_nDefault(0)
	{
		WM_TRAYICON = ::RegisterWindowMessage(_T("WM_TRAYICON"));
        WM_TASKBARCREATE = RegisterWindowMessage(L"TaskbarCreated");;
	}
	
	~CTrayIconImpl()
	{
		// Remove the icon
		RemoveIcon();
	}

	// Install a taskbar icon
	// 	lpszToolTip 	- The tooltip to display
	//	hIcon 		- The icon to display
	// 	nID		- The resource ID of the context menu
	/// returns true on success
	bool InstallIcon(LPCTSTR lpszToolTip, HICON hIcon, UINT nID, bool bModify = false)
	{
		T* pT = static_cast<T*>(this);
		// Fill in the data		
		m_nid.hWnd = pT->m_hWnd;
		m_nid.uID = nID;
		m_nid.hIcon = hIcon;
		m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		m_nid.uCallbackMessage = WM_TRAYICON;
		_tcscpy(m_nid.szTip, lpszToolTip);
		// Install
        DWORD dwMessage = NIM_ADD;
        if (bModify)
        {
            dwMessage = NIM_MODIFY;
        }
		m_bInstalled = Shell_NotifyIcon(dwMessage, &m_nid) ? true : false;
		// Done
		return m_bInstalled;
	}

    BOOL ShowBalloon(LPCTSTR szText,
        LPCTSTR szTitle  = NULL,
        DWORD   dwIcon   = NIIF_NONE,
        UINT    uTimeout = 10 )
    {

        m_nid.uFlags |=  NIF_INFO;
        _tcsncpy(m_nid.szInfo, szText, 256);
        if (szTitle)
            _tcsncpy(m_nid.szInfoTitle, szTitle, 64);
        else
            m_nid.szInfoTitle[0] = _T('\0');
        m_nid.dwInfoFlags = dwIcon;
        m_nid.uTimeout = uTimeout * 1000;   // convert time to ms

        BOOL bSuccess = Shell_NotifyIcon (NIM_MODIFY, &m_nid);

        // Zero out the balloon text string so that later operations won't redisplay
        // the balloon.
        m_nid.szInfo[0] = _T('\0');

        return bSuccess;
    }

	// Remove taskbar icon
	// returns true on success
	bool RemoveIcon()
	{
		if (!m_bInstalled)
			return false;
		// Remove
		m_nid.uFlags = 0;
		return Shell_NotifyIcon(NIM_DELETE, &m_nid) ? true : false;
	}

	// Set the icon tooltip text
	// returns true on success
	bool SetTooltipText(LPCTSTR pszTooltipText)
	{
		if (pszTooltipText == NULL)
			return FALSE;
		// Fill the structure
		m_nid.uFlags = NIF_TIP;
		_tcscpy(m_nid.szTip, pszTooltipText);
		// Set
		return Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? true : false;
	}

	// Set the default menu item ID
	inline void SetDefaultItem(UINT nID) { m_nDefault = nID; }

	BEGIN_MSG_MAP(CTrayIcon)
		MESSAGE_HANDLER(WM_TRAYICON, OnTrayIcon)
        MESSAGE_HANDLER(WM_TASKBARCREATE, OnTaskbarCreate)
	END_MSG_MAP()

    LRESULT OnTaskbarCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
    {
        if (m_bInstalled)
        {
            m_bInstalled = Shell_NotifyIcon(NIM_ADD, &m_nid);
        }
        return 0;
    }

	LRESULT OnTrayIcon(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		// Is this the ID we want?
		if (wParam != m_nid.uID)
			return 0;
		T* pT = static_cast<T*>(this);
		// Was the right-button clicked?
		if (LOWORD(lParam) == WM_RBUTTONUP)
		{
			// Load the menu
			CMenu oMenu;
			if (!oMenu.LoadMenu(m_nid.uID))
				return 0;
			// Get the sub-menu
			CMenuHandle oPopup(oMenu.GetSubMenu(0));
			// Prepare
			pT->PrepareMenu(oPopup);
			// Get the menu position
			CPoint pos;
			GetCursorPos(&pos);
			// Make app the foreground
			SetForegroundWindow(pT->m_hWnd);
			// Set the default menu item
			if (m_nDefault == 0)
				oPopup.SetMenuDefaultItem(0, TRUE);
			else
				oPopup.SetMenuDefaultItem(m_nDefault);
			// Track
			oPopup.TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, pT->m_hWnd);
			// BUGFIX: See "PRB: Menus for Notification Icons Don't Work Correctly"
			pT->PostMessage(WM_NULL);
			// Done
			oMenu.DestroyMenu();
		}
		else if (LOWORD(lParam) == WM_LBUTTONDBLCLK || LOWORD(lParam) == WM_LBUTTONDOWN)
		{
            pT->OnLButton(LOWORD(lParam));
		}
		return 0;
	}

	// Allow the menu items to be enabled/checked/etc.
	virtual void PrepareMenu(HMENU hMenu)
	{
		// Stub
	}
};

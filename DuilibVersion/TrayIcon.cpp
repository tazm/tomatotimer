#include "TrayIcon.h"
#include <tchar.h>

TrayIcon::TrayIcon()
:installed_(FALSE)
{
    memset(&notify_icon_data_, 0, sizeof(notify_icon_data_));
}

TrayIcon::~TrayIcon()
{
    RemoveIcon();
}

BOOL TrayIcon::InstallIcon( HWND hWnd, UINT uCallbackMessage, const wchar_t* tool_tip, HICON hIcon, UINT id)
{
    notify_icon_data_.hWnd = hWnd;
    notify_icon_data_.uID = id;
    notify_icon_data_.hIcon = hIcon;
    notify_icon_data_.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    notify_icon_data_.uCallbackMessage = uCallbackMessage;
    _tcscpy(notify_icon_data_.szTip, tool_tip);
    DWORD dwMessage = NIM_ADD;

    installed_ = Shell_NotifyIcon(dwMessage, &notify_icon_data_);

    return installed_;
}

BOOL TrayIcon::ShowBalloon( const wchar_t* text, const wchar_t* title /*= NULL*/, DWORD dwIcon /*= NIIF_NONE*/, UINT time_out /*= 10 */ )
{
    notify_icon_data_.uFlags |=  NIF_INFO;
    _tcsncpy(notify_icon_data_.szInfo, text, 256);
    if (title)
        _tcsncpy(notify_icon_data_.szInfoTitle, title, 64);
    else
        notify_icon_data_.szInfoTitle[0] = L'\0';
    notify_icon_data_.dwInfoFlags = dwIcon;
    notify_icon_data_.uTimeout = time_out * 1000;   // convert time to ms

    BOOL bSuccess = Shell_NotifyIcon (NIM_MODIFY, &notify_icon_data_);

    // Zero out the balloon text string so that later operations won't redisplay
    // the balloon.
    notify_icon_data_.szInfo[0] = L'\0';

    return bSuccess;
}

BOOL TrayIcon::RemoveIcon()
{
    if (!installed_)
    {
        return TRUE;
    }

    notify_icon_data_.uFlags = 0;
    return Shell_NotifyIcon(NIM_DELETE, &notify_icon_data_);
}

BOOL TrayIcon::SetTooltipText( const wchar_t* tool_tip_text )
{
    notify_icon_data_.uFlags = NIF_TIP;
    _tcscpy(notify_icon_data_.szTip, tool_tip_text);
    return Shell_NotifyIcon(NIM_MODIFY, &notify_icon_data_);
}

BOOL TrayIcon::ModifyIcon( const wchar_t* tool_tip, HICON hIcon, UINT id )
{
    notify_icon_data_.hIcon = hIcon;
    notify_icon_data_.uID = id;
    notify_icon_data_.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    _tcscpy(notify_icon_data_.szTip, tool_tip);
    DWORD dwMessage = NIM_MODIFY;

    return Shell_NotifyIcon(dwMessage, &notify_icon_data_);
}

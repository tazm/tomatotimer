#ifndef _TRAY_ICON_H_
#define _TRAY_ICON_H_

#include <Windows.h>
#include <shellapi.h>

class TrayIcon
{
public:
    TrayIcon();
    ~TrayIcon();

    BOOL InstallIcon(
        HWND hWnd, 
        UINT uCallbackMessage, 
        const wchar_t* tool_tip, 
        HICON hIcon, 
        UINT id
        );

    BOOL ModifyIcon(
        const wchar_t* tool_tip, 
        HICON hIcon, 
        UINT id
        );

    BOOL ShowBalloon(
        const wchar_t* text, 
        const wchar_t* title = NULL, 
        DWORD dwIcon = NIIF_NONE, 
        UINT time_out = 10
        );
    BOOL RemoveIcon();

    BOOL SetTooltipText(const wchar_t* tool_tip_text);

private:
    NOTIFYICONDATAW notify_icon_data_;
    BOOL installed_;
};

#endif
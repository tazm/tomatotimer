#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>

#include "MainWnd.h"
#include "resource.h"

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "comctl32.lib")


int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
//     CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

    CPaintManagerUI::SetResourceZip(_T("skin.zip"));

    CMainWndDlg* pMainDlg = new CMainWndDlg();
    pMainDlg->Create(NULL, _T("番茄时钟"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
//     pMainDlg->Create(NULL, NULL, WS_POPUP, WS_EX_TOOLWINDOW);

    pMainDlg->CenterWindow();

    HICON hIconSmall = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    ::SendMessage(pMainDlg->GetHWND(), WM_SETICON, 0, (LPARAM)hIconSmall);

    pMainDlg->ShowModal();

    CPaintManagerUI::MessageLoop();	//进入消息循环

    return 0;
}
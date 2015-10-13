// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

#include <string>
using namespace std;
const int kTimeId = 1;

const int kWork = 1;
const int kBreak = 2;

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	hIconSmall_ = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall_, FALSE);

    hIconSmall2_ = AtlLoadIconImage(IDI_ICON1, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));


    InstallIcon(L"番茄时钟", hIconSmall_, IDR_POP);

    LOGFONT logFont = { 0 };
    logFont.lfCharSet = DEFAULT_CHARSET;
    logFont.lfHeight = 280;
    SecureHelper::strncpy_x(logFont.lfFaceName, _countof(logFont.lfFaceName), L"宋体", _TRUNCATE);

    font_.CreatePointFontIndirect(&logFont);

    CStatic static2;
    static2.Attach(GetDlgItem(IDC_STATIC_TIME));
    static2.SetFont(font_);

	return TRUE;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CMainDlg::OnExit( UINT uCode, int nID, HWND hwndCtrl )
{
    EndDialog(0);
    return 0;
}

LRESULT CMainDlg::OnShowMain( UINT uCode, int nID, HWND hwndCtrl )
{
    if (!::IsWindowVisible(m_hWnd))
        ::ShowWindow(m_hWnd, SW_SHOW);

    if (::IsIconic(m_hWnd))
        ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE | HTCAPTION, 0);

    ::BringWindowToTop(m_hWnd);
    ::SetForegroundWindow(m_hWnd);

    return 0;
}

LRESULT CMainDlg::OnWork( UINT uCode, int nID, HWND hwndCtrl )
{
    InstallIcon(L"番茄时钟", hIconSmall_, IDR_POP, true);

    wchar_t text[32]  = {0};
    GetDlgItemText(IDC_BTN_WORK, text, 31);
    if (0 == wcscmp(text, L"工作"))
    {
        time_type_ = kWork;
        SetTimer(kTimeId, 1000, NULL);

        SetDlgItemText(IDC_BTN_WORK, L"停止");
        SetDlgItemText(IDC_BTN_BREAK, L"休息");
    }
    else
    {
        KillTimer(kTimeId);
        SetDlgItemText(IDC_BTN_WORK, L"工作");
    }

    time_current_ = 25 * 60;
    SetDlgItemText(IDC_STATIC_TIME, L"25:00");

    return 0;
}

LRESULT CMainDlg::OnBreak( UINT uCode, int nID, HWND hwndCtrl )
{
    InstallIcon(L"番茄时钟", hIconSmall_, IDR_POP, true);

    wchar_t text[32]  = {0};
    GetDlgItemText(IDC_BTN_BREAK, text, 31);
    if (0 == wcscmp(text, L"休息"))
    {
        time_type_ = kBreak;
        SetTimer(kTimeId, 1000, NULL);

        SetDlgItemText(IDC_BTN_WORK, L"工作");
        SetDlgItemText(IDC_BTN_BREAK, L"停止");
    }
    else
    {
        KillTimer(kTimeId);
        SetDlgItemText(IDC_BTN_BREAK, L"休息");
    }

    time_current_ = 5 * 60;
    SetDlgItemText(IDC_STATIC_TIME, L"05:00");

    return 0;
}

void CMainDlg::OnTimer( UINT_PTR id )
{
    if (0 == time_current_)
    {
        KillTimer(id);

        SetDlgItemText(IDC_BTN_WORK, L"工作");
        SetDlgItemText(IDC_BTN_BREAK, L"休息");

        wstring str;

        if (kWork == time_type_)
        {
            str = L"时间到了, 休息一下吧!";
            SetDlgItemText(IDC_STATIC_TIME, L"05:00");
        }
        else
        {
            str = L"时间到了, 开始工作吧!";
            SetDlgItemText(IDC_STATIC_TIME, L"25:00");
        }

        InstallIcon(L"番茄时钟", hIconSmall2_, IDR_POP, true);
        ShowBalloon(str.c_str(), L"番茄时钟");

        SetTooltipText(str.c_str());
        return;
    }

    time_current_ -= 1;
    tm* tm_now = localtime(&time_current_);
    char time_str[32] = {0};
    std::strftime(time_str, sizeof(time_str), "%M:%S", tm_now);

    wstring time_text = CA2W(time_str);
    SetDlgItemText(IDC_STATIC_TIME, time_text.c_str());

    wstring tip;
    if (kWork == time_type_)
    {
        tip = L"工作剩余时间\r\n";
    }
    else
    {
        tip = L"休息剩余时间\r\n";
    }
    tip += time_text;

    SetTooltipText(tip.c_str());
}

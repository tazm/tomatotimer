#include "MainWnd.h"
#include "resource.h"

#include <atlbase.h>

#include <string>
using namespace std;

#include "duilib/UIMenu.h"

#define UWM_TRAYNOTIFY (WM_APP + 1)

const int kWork = 1;
const int kBreak = 2;
const int kTimeId = 1;


CMainWndDlg::CMainWndDlg(void)
:time_type_(1),
btnRest_(NULL),
btnWork_(NULL)
{
}

CMainWndDlg::~CMainWndDlg(void)
{
}

LPCTSTR CMainWndDlg::GetWindowClassName() const
{
	return _T("TomatoWnd");
}

LRESULT CMainWndDlg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	switch( uMsg ) 
	{
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled);	break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled);	break;
	case WM_NCCALCSIZE:	   lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);	break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
    case WM_NCHITTEST:	   lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
    case WM_TIMER:	       lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
    case UWM_TRAYNOTIFY:   lRes = OnTray(uMsg, wParam, lParam, bHandled); break;
    case WM_MENUCLICK:     lRes = OnMenuClick(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
        break;
	}

	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainWndDlg::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_PaintManager.Init(m_hWnd);

	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("main.xml"), (UINT)0, NULL, &m_PaintManager);

	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);  

    hIconMain_ = LoadIconW(CPaintManagerUI::GetInstance(), MAKEINTRESOURCE(IDI_ICON1));
    hIconTomato_ = LoadIconW(CPaintManagerUI::GetInstance(), MAKEINTRESOURCE(IDI_ICON2));

    tray_icon_.InstallIcon(m_hWnd, UWM_TRAYNOTIFY, L"番茄时钟", hIconMain_, IDR_POP);

    btnWork_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnWork")));
    btnRest_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnRest")));
    labelTime_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labelTime")));

	return 0;
}

LRESULT CMainWndDlg::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;

	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CMainWndDlg::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainWndDlg::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainWndDlg::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    POINT pt; 
    pt.x = GET_X_LPARAM(lParam); 
    pt.y = GET_Y_LPARAM(lParam);
    ScreenToClient(*this, &pt);

    RECT rcClient;
    GetClientRect(*this, &rcClient);

    RECT rcCaption = m_PaintManager.GetCaptionRect();
    if (pt.x >= rcClient.left + rcCaption.left &&
        pt.x < rcClient.right - rcCaption.right &&
        pt.y >= rcCaption.top && pt.y < rcCaption.bottom
        )
    {
        CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
        if (pControl &&
            _tcscmp(pControl->GetClass(), _T("ButtonUI")) &&
            _tcscmp(pControl->GetClass(), _T("OptionUI")) &&
            _tcscmp(pControl->GetClass(), _T("TextUI"))
            )
        {
            return HTCAPTION;
        }
    }

    return HTCLIENT;
}

void CMainWndDlg::Notify( TNotifyUI& msg )
{
    if( msg.sType == _T("click") )
    {
		if(0 == _tcscmp(msg.pSender->GetName(), L"btnClose")) 
        {
            ShowWindow(SW_HIDE);
			return; 
		}
        else if(0 == _tcscmp(msg.pSender->GetName(), L"btnWork")) 
        {
            OnWork();
            return; 
        }
        else if(0 == _tcscmp(msg.pSender->GetName(), L"btnRest")) 
        {
            OnRest();
            return; 
        }
	}
}

void CMainWndDlg::OnWork()
{
    tray_icon_.ModifyIcon(L"番茄时钟", hIconMain_, IDR_POP);

    wchar_t text[32]  = {0};
    if (0 == wcscmp(btnWork_->GetText(), L"工作"))
    {
        time_type_ = kWork;
        SetTimer(m_hWnd, kTimeId, 1000, NULL);

        btnWork_->SetText(L"停止");
        btnRest_->SetText(L"休息");
    }
    else
    {
        KillTimer(m_hWnd, kTimeId);
        btnWork_->SetText(L"工作");
    }

    time_current_ = 25 * 60;
    labelTime_->SetText(L"25:00");
}

void CMainWndDlg::OnRest()
{
    tray_icon_.ModifyIcon(L"番茄时钟", hIconMain_, IDR_POP);

    if (0 == wcscmp(btnRest_->GetText(), L"休息"))
    {
        time_type_ = kBreak;
        SetTimer(m_hWnd, kTimeId, 1000, NULL);

        btnWork_->SetText(L"工作");
        btnRest_->SetText(L"停止");
    }
    else
    {
        KillTimer(m_hWnd, kTimeId);
        btnRest_->SetText(L"休息");
    }

    time_current_ = 5 * 60;
    labelTime_->SetText(L"05:00");
}

LRESULT CMainWndDlg::OnTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    if (0 == time_current_)
    {
        KillTimer(m_hWnd, kTimeId);

        btnWork_->SetText(L"工作");
        btnRest_->SetText(L"休息");

        wstring str;

        if (kWork == time_type_)
        {
            str = L"时间到了, 休息一下吧!";
            labelTime_->SetText(L"05:00");
        }
        else
        {
            str = L"时间到了, 开始工作吧!";
            labelTime_->SetText(L"25:00");
        }

        tray_icon_.ModifyIcon(L"番茄时钟", hIconTomato_, IDR_POP);
        tray_icon_.ShowBalloon(str.c_str(), L"番茄时钟");

        tray_icon_.SetTooltipText(str.c_str());
        return 0;
    }

    time_current_ -= 1;
    tm* tm_now = localtime(&time_current_);
    char time_str[32] = {0};
    std::strftime(time_str, sizeof(time_str), "%M:%S", tm_now);

    wstring time_text = CA2W(time_str);
    labelTime_->SetText(time_text.c_str());

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

    tray_icon_.SetTooltipText(tip.c_str());
    return 0;
}

LRESULT CMainWndDlg::OnTray( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    if (LOWORD(lParam) == WM_RBUTTONUP)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point(0,0);
        GetCursorPos(&point);
        point.y -= 35;

        pMenu->Init(NULL, _T("start_menu.xml"), point, &m_PaintManager, NULL); 
    }
    else if (LOWORD(lParam) == WM_LBUTTONDBLCLK || LOWORD(lParam) == WM_LBUTTONDOWN)
    {
        if (!::IsWindowVisible(m_hWnd))
            ::ShowWindow(m_hWnd, SW_SHOW);

        if (::IsIconic(m_hWnd))
            ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE | HTCAPTION, 0);

        ::BringWindowToTop(m_hWnd);
        ::SetForegroundWindow(m_hWnd);
    }
    return 0;
}

LRESULT CMainWndDlg::OnMenuClick( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    CStdString *strMenuName = (CStdString*)wParam; 

    wstring strTmp(*strMenuName);
    delete strMenuName; 

    if (L"exit" == strTmp)   
    {      
        tray_icon_.RemoveIcon();
        ShowWindow(SW_HIDE);
        PostQuitMessage(0);
    }  
    return 0;
}


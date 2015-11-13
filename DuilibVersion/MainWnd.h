#pragma once
#include "duilib/UIlib.h"
#include "TrayIcon.h"

#include <ctime>
using namespace std;

using namespace DuiLib;

class CMainWndDlg : public CWindowWnd, public INotifyUI
{
public:
	CMainWndDlg(void);
	~CMainWndDlg(void);

public:

	LPCTSTR GetWindowClassName() const;
	
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void Notify(TNotifyUI& msg); 

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnTray(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnMenuClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    void OnWork();
    void OnRest();

protected:
	CPaintManagerUI m_PaintManager;

    TrayIcon tray_icon_;

    HICON hIconMain_;
    HICON hIconTomato_;

    time_t time_current_;
    int time_type_;

    CButtonUI* btnWork_;
    CButtonUI* btnRest_;
    CLabelUI*  labelTime_;
};

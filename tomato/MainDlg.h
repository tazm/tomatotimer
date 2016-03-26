// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <ctime>
using namespace std;
#include "trayiconimpl.h"

class CMainDlg : public CDialogImpl<CMainDlg>,
    public CTrayIconImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP_EX(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER_EX(ID_MENU_EXIT, OnExit)
        COMMAND_ID_HANDLER_EX(IDC_BTN_WORK, OnWork)
        COMMAND_ID_HANDLER_EX(IDC_BTN_BREAK, OnBreak)
        MSG_WM_TIMER(OnTimer)
        CHAIN_MSG_MAP(CTrayIconImpl<CMainDlg>)
        REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    LRESULT OnExit(UINT uCode, int nID, HWND hwndCtrl);
    LRESULT OnWork(UINT uCode, int nID, HWND hwndCtrl);
    LRESULT OnBreak(UINT uCode, int nID, HWND hwndCtrl);

    int OnLButton(int type);

    void OnTimer(UINT_PTR id);
private:

    CFont font_;

    time_t time_current_;

    int time_type_;

    HICON hIconSmall_;
    HICON hIconSmall2_;

    UINT m_nMsgTaskbarCreated;
};

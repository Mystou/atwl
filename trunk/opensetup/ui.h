// -----------------------------------------------------------------
// RagnarokOnline OpenSetup
// (c) 2010-2013 Ai4rei/AN
// See doc/license.txt for details.
//
// -----------------------------------------------------------------

#ifndef _UI_H_
#define _UI_H_

class UI
{
public:
    typedef struct BATCHLIST
    {
        int nIDDlgItem;
        UINT uMsg;
        WPARAM wParam;
        LPARAM lParam;
    }
    BATCHLIST,* LPBATCHLIST;
    typedef const BATCHLIST* LPCBATCHLIST;

protected:
    static BOOL CALLBACK LoadMUIForEach(HWND hWnd, LPARAM lParam);
    static BOOL CALLBACK SetFontForEach(HWND hWnd, LPARAM lParam);
    static BOOL CALLBACK SetFocusFirstChildEach(HWND, LPARAM lParam);

public:
    static void __stdcall BatchMessage(HWND hWnd, LPCBATCHLIST lpBatchList, unsigned long luItems);
    static void __stdcall FillComboBox(HWND hWnd, int nId, const char** lppszList, unsigned long luItems);
    static void __stdcall FillComboBoxMUI(HWND hWnd, int nId, const unsigned int* lpuList, unsigned long luItems);
    static void __stdcall LoadMUI(HWND hWnd);
    static int __stdcall MessageBoxEx(HWND hWnd, const char* lpszText, const char* lpszCaption, unsigned long luStyle);
    static unsigned long __stdcall GetSizeRatio(HWND hWndA, HWND hWndB);
    static void __stdcall SetButtonShield(HWND hWnd, bool bEnable);
    static void __stdcall SetFont(HWND hWnd, HFONT hFont);
    static bool __stdcall GetCheckBoxTick(HWND hWnd, int nId);
    static void __stdcall HHLite(HWND hWnd, LPHELPINFO lpHi);
    static void __stdcall SetFocusFirstChild(HWND hWnd);
    static bool __stdcall IsRemoteSession(void);
};

#endif  /* _UI_H_ */
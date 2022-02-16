#include "windowsx_bugfix.hpp"

/// <summary>
/// 
/// </summary>
/// <param name="hwndCtl">The combobox handler.</param>
/// <param name="indexStart">Index from where to start search.</param>
/// <param name="data">The data you are looking for.</param>
/// <remarks>Improved solution from GitHub user 0xmono for project miranda-ng.</remarks>
/// <returns>The index of the ComboBox item containing the searched data.</returns>
int ComboBox_SelectItemData(HWND hwndCtl, int indexStart, LPARAM data)
{
    int count = ComboBox_GetCount(hwndCtl);

    for (int i = indexStart; i < count; i++) {
        if (data == ComboBox_GetItemData(hwndCtl, i)) {
            ComboBox_SetCurSel(hwndCtl, i);
            return i;
        }
    }

    return CB_ERR;
}
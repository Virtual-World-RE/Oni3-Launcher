#pragma once
#include "pch.h"

#ifdef ComboBox_SelectItemData
#undef ComboBox_SelectItemData
#endif

int ComboBox_SelectItemData(HWND hwndCtl, int indexStart, LPARAM data);

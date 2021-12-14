#include "pch.h"
#include "Trace.h"


Trace::Trace(const std::string &functionName)
{
	this->functionName = functionName;

	std::cout << "[Enter Function] " << this->functionName << std::endl;
}

void Trace::TraceFlag(DWORD flag)
{
    D3DFLAGS flags[11] = {
        {"D3DPRESENTFLAG_DEVICECLIP", D3DPRESENTFLAG_DEVICECLIP},
        {"D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL", D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL},
        {"D3DPRESENTFLAG_LOCKABLE_BACKBUFFER", D3DPRESENTFLAG_LOCKABLE_BACKBUFFER},
        {"D3DPRESENTFLAG_NOAUTOROTATE", D3DPRESENTFLAG_NOAUTOROTATE},
        {"D3DPRESENTFLAG_UNPRUNEDMODE", D3DPRESENTFLAG_UNPRUNEDMODE},
        {"D3DPRESENTFLAG_VIDEO", D3DPRESENTFLAG_VIDEO},
        {"D3DPRESENTFLAG_OVERLAY_LIMITEDRGB", D3DPRESENTFLAG_OVERLAY_LIMITEDRGB},
        {"D3DPRESENTFLAG_OVERLAY_YCbCr_BT709", D3DPRESENTFLAG_OVERLAY_YCbCr_BT709},
        {"D3DPRESENTFLAG_OVERLAY_YCbCr_xvYCC", D3DPRESENTFLAG_OVERLAY_YCbCr_xvYCC},
        {"D3DPRESENTFLAG_RESTRICTED_CONTENT", D3DPRESENTFLAG_RESTRICTED_CONTENT},
        {"D3DPRESENTFLAG_RESTRICT_SHARED_RESOURCE_DRIVER", D3DPRESENTFLAG_RESTRICT_SHARED_RESOURCE_DRIVER}
    };

    for (int i = 0; i != 11; i++)
        if (flag & flags[i].flag)
            std::cout << "\tFlagName: " << flags[i].name << std::endl;
}

void Trace::TraceDwStyle(DWORD dwStyle)
{
    DWORD refacStyle = dwStyle;
    D3DFLAGS groupStyles[2] = {
        {"WS_OVERLAPPEDWINDOW | WS_TILEDWINDOW", WS_OVERLAPPEDWINDOW},
        {"WS_POPUPWINDOW (req. WS_CAPTION)", WS_POPUPWINDOW},
    };
    D3DFLAGS styles[20] = {
        {"WS_BORDER", WS_BORDER},
        {"WS_CAPTION (incl. WS_BORDER)", WS_CAPTION},
        {"WS_CHILD | WS_CHILDWINDOW (incomp. WS_POPUP)", WS_CHILD},
        {"WS_CLIPCHILDREN", WS_CLIPCHILDREN},
        {"WS_CLIPSIBLINGS", WS_CLIPSIBLINGS},
        {"WS_DISABLED", WS_DISABLED},
        {"WS_DLGFRAME", WS_DLGFRAME},
        {"WS_GROUP", WS_GROUP},
        {"WS_HSCROLL", WS_HSCROLL},
        {"WS_ICONIC | WS_MINIMIZE", WS_ICONIC},
        {"WS_MAXIMIZE", WS_MAXIMIZE},
        {"WS_MAXIMIZEBOX (incomp. WS_EX_CONTEXTHELP | req. WS_SYSMENU)", WS_MAXIMIZEBOX},
        {"WS_MINIMIZEBOX (incomp. WS_EX_CONTEXTHELP | req. WS_SYSMENU)", WS_MINIMIZEBOX},
        {"WS_OVERLAPPED | WS_TILED", WS_OVERLAPPED},
        {"WS_POPUP (incomp. WS_CHILD)", WS_POPUP},
        {"WS_SIZEBOX | WS_THICKFRAME", WS_SIZEBOX},
        {"WS_SYSMENU (req. WS_CAPTION)", WS_SYSMENU},
        {"WS_TABSTOP", WS_TABSTOP},
        {"WS_VISIBLE", WS_VISIBLE},
        {"WS_VSCROLL", WS_VSCROLL}
    };

    for (int i = 0; i != 2; i++)
        if ((dwStyle & groupStyles[i].flag) == groupStyles[i].flag) {
            std::cout << "\tFlag Group : " << groupStyles[i].name << std::endl;
            refacStyle = dwStyle & ~groupStyles[i].flag;
        }

    for (int i = 0; i < 20; i++)
        if ((refacStyle & styles[i].flag) == styles[i].flag)
            std::cout << "\tFlag Name : " << styles[i].name << std::endl;
}

void Trace::TraceMessage(const std::string message)
{
	std::cout << "\t" << message << std::endl;
}

void Trace::TraceWchar(LPCTSTR message, LPCTSTR parameter)
{
    _tprintf(TEXT("\t%s: %s\n"), message, parameter);
}

Trace::~Trace()
{
	std::cout << "[Exit Function] " << this->functionName << std::endl;
}

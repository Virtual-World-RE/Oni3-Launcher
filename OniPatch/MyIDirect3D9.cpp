#include "pch.h"
#include "MyIDirect3D9.h"

MyIDirect3D9::MyIDirect3D9(IDirect3D9 *original)
{
    this->original = original;

    this->Version = this->original->Version;
}

MyIDirect3D9::~MyIDirect3D9() {}

/*** IUnknown methods ***/
HRESULT MyIDirect3D9::QueryInterface(THIS_ REFIID riid, void **ppvObj)
{
    return this->original->QueryInterface(riid, ppvObj);
}

ULONG MyIDirect3D9::AddRef(THIS)
{
    return this->original->AddRef();
}

ULONG MyIDirect3D9::Release(THIS)
{
    return this->original->Release();
}

/*** IDirect3D9 methods ***/
HRESULT MyIDirect3D9::RegisterSoftwareDevice(THIS_ void *pInitializeFunction)
{
    return this->original->RegisterSoftwareDevice(pInitializeFunction);
}

UINT MyIDirect3D9::GetAdapterCount(THIS)
{
    return this->original->GetAdapterCount();
}

HRESULT MyIDirect3D9::GetAdapterIdentifier(THIS_ UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 *pIdentifier)
{
    return this->original->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT MyIDirect3D9::GetAdapterModeCount(THIS_ UINT Adapter, D3DFORMAT Format)
{
    UINT result = 0;

    TraceFunc();
    TraceParam("Adapter", Adapter);
    TraceParam("D3DFORMAT", Format);

    result = this->original->GetAdapterModeCount(Adapter, Format);

    TraceParam("ModeCout", result);

    return result;
}

HRESULT MyIDirect3D9::EnumAdapterModes(THIS_ UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE *pMode)
{
    HRESULT hResult;

    TraceFunc();

    TraceParam("Adapter", Adapter);
    TraceParam("D3DFORMAT", Format);
    TraceParam("Mode", Mode);
    TraceParam("pMode ADDR", pMode);
    TraceParam("pMode.Width", pMode->Width);
    TraceParam("pMode.Height", pMode->Height);
    TraceParam("pMode.RefreshRate", pMode->RefreshRate);
    TraceParam("pMode.Format", pMode->Format);

    hResult = this->original->EnumAdapterModes(Adapter, Format, Mode, pMode);

    TraceParam("New pMode.Width", pMode->Width);
    TraceParam("New pMode.Height", pMode->Height);
    TraceParam("New pMode.RefreshRate", pMode->RefreshRate);
    TraceParam("New pMode.Format", pMode->Format);

    return hResult;
}

HRESULT MyIDirect3D9::GetAdapterDisplayMode(THIS_ UINT Adapter, D3DDISPLAYMODE *pMode)
{
    HRESULT hResult;

    TraceFunc();

    TraceParam("Adapter", Adapter);
    TraceParam("pMode ADDR", pMode);
    TraceParam("pMode.Width", pMode->Width);
    TraceParam("pMode.Height", pMode->Height);
    TraceParam("pMode.RefreshRate", pMode->RefreshRate);
    TraceParam("pMode.Format", pMode->Format);

    hResult = this->original->GetAdapterDisplayMode(Adapter, pMode);

    TraceParam("New pMode.Width", pMode->Width);
    TraceParam("New pMode.Height", pMode->Height);
    TraceParam("New pMode.RefreshRate", pMode->RefreshRate);
    TraceParam("New pMode.Format", pMode->Format);


    return hResult;
}

HRESULT MyIDirect3D9::CheckDeviceType(THIS_ UINT iAdapter, D3DDEVTYPE DevType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
    return this->original->CheckDeviceType(iAdapter, DevType, DisplayFormat, BackBufferFormat, bWindowed);
}

HRESULT MyIDirect3D9::CheckDeviceFormat(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
    return this->original->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT MyIDirect3D9::CheckDeviceMultiSampleType(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD *pQualityLevels)
{
    return this->original->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

HRESULT MyIDirect3D9::CheckDepthStencilMatch(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
    return this->original->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT MyIDirect3D9::CheckDeviceFormatConversion(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
    return this->original->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
}

HRESULT MyIDirect3D9::GetDeviceCaps(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9 *pCaps)
{
    return this->original->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HMONITOR MyIDirect3D9::GetAdapterMonitor(THIS_ UINT Adapter)
{
    return this->original->GetAdapterMonitor(Adapter);
}

HRESULT MyIDirect3D9::CreateDevice(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface)
{
    HRESULT hResult;
    IDirect3DDevice9 *trueDevice = NULL;

    TraceFunc();

    TraceParam("Adapter", Adapter);
    TraceParam("DeviceType", DeviceType);
    TraceParam("BehaviorFlags", BehaviorFlags);
    TraceParam("BackBufferWidth", pPresentationParameters->BackBufferWidth);
    TraceParam("BackBufferHeight", pPresentationParameters->BackBufferHeight);
    TraceParam("BackBufferFormat", pPresentationParameters->BackBufferFormat);
    TraceParam("BackBufferCount", pPresentationParameters->BackBufferCount);
    TraceParam("MultiSampleType", pPresentationParameters->MultiSampleType);
    TraceParam("MultiSampleQuality", pPresentationParameters->MultiSampleQuality);
    TraceParam("SwapEffect", pPresentationParameters->SwapEffect);
    //TraceParam("hDeviceWindow", pPresentationParameters->hDeviceWindow);
    TraceParam("Windowed", pPresentationParameters->Windowed);
    TraceParam("EnableAutoDepthStencil", pPresentationParameters->EnableAutoDepthStencil);
    TraceParam("AutoDepthStencilFormat", pPresentationParameters->AutoDepthStencilFormat);
    TraceFlg(pPresentationParameters->Flags);
    TraceParam("FullScreen_RefreshRateInHz", pPresentationParameters->FullScreen_RefreshRateInHz);
    TraceParam("PresentationInterval", pPresentationParameters->PresentationInterval);

    if (pPresentationParameters->BackBufferWidth == 800) {

        TraceMsg("Back buffer updated");

        // TODO: Read this from configuration
        pPresentationParameters->BackBufferWidth = 1920;
        pPresentationParameters->BackBufferHeight = 1080;

        TraceParam("NewBackBufferWidth", pPresentationParameters->BackBufferWidth);
        TraceParam("NewBackBufferHeight", pPresentationParameters->BackBufferHeight);

        //pPresentationParameters->Windowed = FALSE;
    }

    hResult = this->original->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &trueDevice);
    (*ppReturnedDeviceInterface) = new MyIDirect3DDevice9(trueDevice);

    return hResult;
}

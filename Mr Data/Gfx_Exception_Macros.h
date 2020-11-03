#pragma once
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)
#define GFX_THROW_INFO(hr) if(hr != 0) {throw Graphics::HrException(__LINE__,WFILE, hr);}
#define GFX_DEVICE_REMOVED() throw Graphics::DeviceRemovedException(__LINE__, WFILE, pDevice->GetDeviceRemovedReason());

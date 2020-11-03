#pragma once
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)
#define THROW_HRESULT(hr) if(hr != 0) {throw Window::Exception(__LINE__,WFILE, hr);}
#define THROW_LAST_ERROR throw Window::Exception(__LINE__,WFILE, GetLastError());
#define THROW_NO_GFX throw Window::NoGfxException(__LINE__,WFILE, 666);
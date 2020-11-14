#include "NDI_Send.h"
#include <stdlib.h>

NDI_Send::NDI_Send(int in_resX, int in_resY)
	:
	resX(in_resX),
	resY(in_resY)
{
	if (!NDIlib_initialize())
	{
		OutputDebugString(L"Cannot run NDI on this architecture.\n");
	}

	pNDI_send = NDIlib_send_create();

	NDI_video_frame.xres = resX;
	NDI_video_frame.yres = resY;
	NDI_video_frame.FourCC = NDIlib_FourCC_type_RGBA;
	NDI_video_frame.line_stride_in_bytes = resX * 4;
	NDI_video_frame.frame_format_type = NDIlib_frame_format_type_progressive;
	NDI_video_frame.frame_rate_N = 60000;
	NDI_video_frame.frame_rate_D = 1000;
	OutputDebugString(L"NDI Constructor complete...\n");

}

void NDI_Send::SendNDIFrame(Graphics& gfx)
{
	p_frame.clear();
	p_frame.reserve(resX * resY * 4);

	for (unsigned int y = 0; y < resY; y++)
	{
		for (unsigned int x = 0; x < resX; x++)
		{
			ColorWord c = gfx.GetPixel(x, y);
			p_frame.push_back(c.r);
			p_frame.push_back(c.g);
			p_frame.push_back(c.b);
			p_frame.push_back(c.a);

		}
	}

	NDI_video_frame.p_data = &p_frame[0];
	NDIlib_send_send_video_v2(pNDI_send, &NDI_video_frame);



}

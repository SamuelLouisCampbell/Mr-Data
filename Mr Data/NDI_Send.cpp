#include "NDI_Send.h"
#include <stdlib.h>
#include <cassert>


NDI_Send::NDI_Send()
{
}

NDI_Send::~NDI_Send()
{
	NDIlib_destroy();
}

void NDI_Send::NDI_Init(int in_resX, int in_resY)
{
	//check we haven't alrady set res.
	assert(resX == 0 && resY == 0);

	resX = in_resX;
	resY = in_resY;

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
	PROFILE_FUNCTION();
	NDI_video_frame.p_data = &gfx.GetFramePtr()[0];
	NDIlib_send_send_video_async_v2 ( pNDI_send, &NDI_video_frame);

}

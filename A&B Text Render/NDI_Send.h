#pragma once
#include "Processing.NDI.Lib.h"
#include "BarebonesWindows.h"
#include "Color.h"
#include "Graphics.h"
#include <thread>

class NDI_Send
{
public:
	NDI_Send();
	~NDI_Send();
	void NDI_Init(int in_resX = 1440, int in_resY = 900);
	void SendNDIFrame(Graphics& gfx);
private:
	unsigned int resX = 0;
	unsigned int resY = 0;

	//ndi stuff
	NDIlib_send_instance_t pNDI_send;
	NDIlib_video_frame_v2_t NDI_video_frame;
};


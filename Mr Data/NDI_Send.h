#pragma once
#include "Processing.NDI.Lib.h"
#include "BarebonesWindows.h"
#include "Color.h"
#include "Graphics.h"

class NDI_Send
{
public:
	NDI_Send(int in_resX = 1280, int in_resY = 720);
	void SendNDIFrame(Graphics& gfx);
private:
	unsigned int resX;
	unsigned int resY;

	//ndi stuff
	NDIlib_send_instance_t pNDI_send;
	NDIlib_video_frame_v2_t NDI_video_frame;
	std::vector<uint8_t> p_frame;

};


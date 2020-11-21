cbuffer cBuf
{
	float4 face_color;
};

float4 main(uint tid : SV_PrimitiveID) : SV_TARGET
{
	return face_color;
}
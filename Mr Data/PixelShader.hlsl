float4 main(float3 color : COLOR) : SV_TARGET
{
	float4 value = {color.r, color.g, color.b, 1.0f};
	return value;
}
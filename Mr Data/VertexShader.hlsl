struct VSOut
{
	float3 color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer Cbuff
{
	matrix transform;
};

VSOut main(float3 pos : Position, float3 color : Color)
{
	VSOut vso;
	vso.pos = mul(float4(pos.x, pos.y, pos.z, 1.0f), transform);
	vso.color = color;
	return vso;
}
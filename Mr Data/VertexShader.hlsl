float4 main( float2 pos : POSITION ) : SV_POSITION
{
	float4 val = {pos.x, pos.y, 0.0f, 1.0f};
return val;
}
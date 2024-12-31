struct VSOut {
	float4 pos : SV_POSITION;
};

cbuffer const_buffer {
	matrix transform;
};

VSOut main( float3 pos : POSITION)
{
	VSOut output;
	output.pos = mul(float4(pos, 1.0f), transform);

	return output;
}
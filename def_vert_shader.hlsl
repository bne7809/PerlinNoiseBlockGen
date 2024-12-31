struct VSOut {
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

cbuffer const_buffer {
	matrix transform;
};

VSOut main( float3 pos : POSITION, float2 tex : TEXCOORD)
{
	VSOut output;
	output.pos = mul(float4(pos, 1.0f), transform);
	output.tex = tex;

	return output;
}
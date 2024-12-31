Texture2D tex;
SamplerState samp;

struct VSOut {
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

float4 main(VSOut pin) : SV_TARGET
{
	return tex.Sample(samp, pin.tex);
}
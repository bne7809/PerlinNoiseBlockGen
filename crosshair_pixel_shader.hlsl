struct VSOut {
	float4 pos : SV_POSITION;
};

float4 main(VSOut pin) : SV_TARGET
{
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}
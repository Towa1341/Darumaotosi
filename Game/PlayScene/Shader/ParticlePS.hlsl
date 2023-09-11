#include "Particle.hlsli"
Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
	//�摜�\��
	float4 shadow = tex.Sample(samLinear, input.Tex);

	float4 ball = tex2.Sample(samLinear, input.Tex);


	//�^�����Ȕ|���S��
	float4 outputw = float4(1, 1, 1, 1);

	//	�F(RGBA)
	float4 white = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 red   = float4(1.0f, 0.0f, 0.0f, 1.0f);
	float4 green = float4(0.0f, 1.0f, 0.0f, 1.0f);
	float4 blue  = float4(0.0f, 0.0f, 1.0f, 1.0f);
	float4 black = float4(0.0f, 0.0f, 0.0f, 1.0f);

	shadow.rgb = shadow.a;
	if (shadow.a > 0.5)
		shadow.a = 1 - shadow.rgb;

	return shadow;
}
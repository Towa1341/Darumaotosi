#include "Particle.hlsli"
static const int vnum = 4;

static const float4 offset_array[vnum] =
{
	//	四角形
	float4(-0.5f, -0.5f, 0.0f, 0.0f),	// 左下
	float4(-0.5f, +0.5f, 0.0f, 0.0f),	// 左上
	float4(+0.5f, -0.5f, 0.0f, 0.0f),	// 右上
	float4(+0.5f, +0.5f, 0.0f, 0.0f),	// 右下
};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{
	for (int i = 0; i < vnum; i++)
	{
		PS_INPUT element;

		element.Pos = input[0].Pos + offset_array[i];

		element.Pos = mul(element.Pos, matWorld);
		element.Pos = mul(element.Pos, matView);
		element.Pos = mul(element.Pos, matProj);

		element.Color = input[0].Color;

		element.Tex = -offset_array[i].xy + float2(0.5f, 0.5f);
		output.Append(element);
	}
	output.RestartStrip();
}
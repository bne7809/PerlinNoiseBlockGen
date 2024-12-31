#pragma once

#include <DirectXTex.h>
#include "GraphicsHandle.h"

class Texturer {
public:
	Texturer(GraphicsHandle& graphics, const wchar_t* file, UINT tex_slot, UINT sampler_slot);

	void bind();
private:
	GraphicsHandle& graphics;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;

	UINT tex_slot;
	UINT sampler_slot;
};
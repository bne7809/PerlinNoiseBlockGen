#pragma once

#include "GraphicsHandle.h"
#include <d3dcompiler.h>
#include "VertexBuffer.h"

class Shader {
public:
	Shader(GraphicsHandle& graphics, LPCWSTR vertex_shader, LPCWSTR pixel_shader, UINT desc);
	
	void bind();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;

	GraphicsHandle& graphics;
};
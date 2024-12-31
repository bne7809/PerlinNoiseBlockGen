#include "Shader.h"

Shader::Shader(GraphicsHandle& graphics, LPCWSTR vertex_shader, LPCWSTR pixel_shader, UINT desc) 
	: graphics(graphics)
{
	Microsoft::WRL::ComPtr<ID3DBlob> vblob;
	Microsoft::WRL::ComPtr<ID3DBlob> pblob;

	D3DReadFileToBlob(vertex_shader, &vblob);
	D3DReadFileToBlob(pixel_shader, &pblob);

	std::vector<D3D11_INPUT_ELEMENT_DESC> elem_desc;

	UINT n = VERT_DESC_NUM(desc);

	elem_desc.reserve(n);

	UINT offset = 0;
	if (desc & VERT_DESC_POS_3D) {
		elem_desc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT3);
	}
	if (desc & VERT_DESC_TEX_2D) {
		elem_desc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT2);
	}
	if (desc & VERT_DESC_COL_4D) {
		elem_desc.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT4);
	}
	if (desc & VERT_DESC_NORMAL) {
		elem_desc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT3);
	}
	if (desc & VERT_DESC_TANGENT) {
		elem_desc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT3);
	}

	graphics.get_device()->CreateVertexShader(vblob->GetBufferPointer(), vblob->GetBufferSize(), nullptr, &vs);
	graphics.get_device()->CreatePixelShader(pblob->GetBufferPointer(), pblob->GetBufferSize(), nullptr, &ps);
	graphics.get_device()->CreateInputLayout(elem_desc.data(), elem_desc.size(), 
		vblob->GetBufferPointer(), vblob->GetBufferSize(), &layout);
}

void Shader::bind() {
	graphics.get_context()->VSSetShader(vs.Get(), nullptr, 0);
	graphics.get_context()->PSSetShader(ps.Get(), nullptr, 0);
	graphics.get_context()->IASetInputLayout(layout.Get());
}
#pragma once

#include "GraphicsHandle.h"
#include "VertexBuffer.h"

typedef USHORT index_type;
typedef std::vector<index_type> IndexBuffer;

template <typename T>
constexpr DXGI_FORMAT index_format() {
	if constexpr (std::is_same_v<T, UINT>) {
		return DXGI_FORMAT_R32_UINT;
	} else {
		return DXGI_FORMAT_R16_UINT;
	}
}

constexpr DXGI_FORMAT INDEX_FORMAT = index_format<index_type>();

class Mesh {
protected:
	Mesh()
		: num_indices(0), stride(0), num_verts(0) {}

	void load(GraphicsHandle& graphics, VertexBuffer& buffer, const IndexBuffer& indices);
	void draw(GraphicsHandle& graphics);

	Microsoft::WRL::ComPtr<ID3D11Buffer> vb;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ib;

	UINT num_indices;
	UINT num_verts;
	UINT stride;
};
#pragma once

#include "GraphicsHandle.h"

enum class CONST_BUFFER_TYPE {
	VERTEX,
	PIXEL
};

template <typename T, CONST_BUFFER_TYPE type>
class ConstBuffer {
public:
	ConstBuffer(GraphicsHandle& graphics, const T& data, UINT slot)
		: graphics(graphics), slot(slot)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.ByteWidth = sizeof(T);
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0;
		cbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA csd;
		csd.pSysMem = &data;
		csd.SysMemPitch = 0;
		csd.SysMemSlicePitch = 0;

		graphics.get_device()->CreateBuffer(&cbd, &csd, &cb);
	}

	void bind() {
		if constexpr (type == CONST_BUFFER_TYPE::VERTEX) {
			graphics.get_context()->VSSetConstantBuffers(slot, 1, cb.GetAddressOf());
		} else {
			graphics.get_context()->PSSetConstantBuffers(slot, 1, cb.GetAddressOf());
		}
	}

	void set(const T& t) {
		D3D11_MAPPED_SUBRESOURCE ms;
		graphics.get_context()->Map(cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		std::memcpy(ms.pData, &t, sizeof(T));
		graphics.get_context()->Unmap(cb.Get(), 0);
	}
private:
	GraphicsHandle& graphics;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cb;
	UINT slot;
};

template <typename T>
using VertexConstBuff =  ConstBuffer<T, CONST_BUFFER_TYPE::VERTEX>;

template <typename T>
using PixelConstBuff = ConstBuffer<T, CONST_BUFFER_TYPE::PIXEL>;
#pragma once

#include <wrl/client.h>
#include <memory>
#include <d3d11.h>

class GraphicsHandle {
public:
	GraphicsHandle(HWND hwnd, int width, int height);

	inline ID3D11Device* get_device() { return device.Get(); }
	inline ID3D11DeviceContext* get_context() { return context.Get(); }

	void begin_frame(float r, float g, float b, float a);
	void end_frame();

	inline void disable_depth_test() { context->OMSetDepthStencilState(disable_depth.Get(), 1u); }
	inline void enable_depth_test() { context->OMSetDepthStencilState(enable_depth.Get(), 1u); }
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> enable_depth;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> disable_depth;
};
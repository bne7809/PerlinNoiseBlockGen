#include "GraphicsHandle.h"

GraphicsHandle::GraphicsHandle(HWND hwnd, int width, int height) {
	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_buffer;

	DXGI_SWAP_CHAIN_DESC scd;
	scd.BufferCount = 2;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scd.Flags = 0;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
		nullptr, 0, D3D11_SDK_VERSION, &scd, &swap_chain, &device, nullptr, &context);

	swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
	device->CreateRenderTargetView(back_buffer.Get(), nullptr, &target);

	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;

	device->CreateDepthStencilState(&dsd, &enable_depth);
	context->OMSetDepthStencilState(enable_depth.Get(), 1u);

	D3D11_DEPTH_STENCIL_DESC dsd2 = {};
	dsd2.DepthEnable = FALSE;
	dsd2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd2.DepthFunc = D3D11_COMPARISON_ALWAYS;

	device->CreateDepthStencilState(&dsd2, &disable_depth);

	D3D11_TEXTURE2D_DESC dtd = {};
	dtd.Width = width;
	dtd.Height = height;
	dtd.MipLevels = 1u;
	dtd.ArraySize = 1u;
	dtd.Format = DXGI_FORMAT_D32_FLOAT;
	dtd.SampleDesc.Count = 1u;
	dtd.SampleDesc.Quality = 0u;
	dtd.Usage = D3D11_USAGE_DEFAULT;
	dtd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	device->CreateTexture2D(&dtd, nullptr, &depth_buffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC dview = {};
	dview.Format = DXGI_FORMAT_D32_FLOAT;
	dview.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dview.Texture2D.MipSlice = 0u;

	device->CreateDepthStencilView(depth_buffer.Get(), &dview, &depth);

	context->OMSetRenderTargets(1u, target.GetAddressOf(), depth.Get());

	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	context->RSSetViewports(1, &vp);
}

void GraphicsHandle::begin_frame(float r, float g, float b, float a) {
	float col[] = { r, g, b, a };
	context->ClearRenderTargetView(target.Get(), col);
	context->ClearDepthStencilView(depth.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void GraphicsHandle::end_frame() {
	swap_chain->Present(1, 0);
	context->OMSetRenderTargets(1, target.GetAddressOf(), depth.Get());
}

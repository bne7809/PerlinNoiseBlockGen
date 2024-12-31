#include "Texturer.h"

Texturer::Texturer(GraphicsHandle& graphics, const wchar_t* file, UINT tex_slot, UINT sampler_slot) 
	: graphics(graphics), tex_slot(tex_slot), sampler_slot(sampler_slot)
{
	DirectX::ScratchImage img;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> raw_data;
	DirectX::LoadFromWICFile(file, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, img);

	D3D11_TEXTURE2D_DESC td;
	td.Width = img.GetMetadata().width;
	td.Height = img.GetMetadata().height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	graphics.get_device()->CreateTexture2D(&td, nullptr, &raw_data);

	if (img.GetImage(0, 0, 0)->format != DXGI_FORMAT_B8G8R8A8_UNORM) {
		DirectX::ScratchImage convert;
		DirectX::Convert(*img.GetImage(0, 0, 0), DXGI_FORMAT_B8G8R8A8_UNORM, DirectX::TEX_FILTER_DEFAULT,
			DirectX::TEX_THRESHOLD_DEFAULT, convert);
		
		graphics.get_context()->UpdateSubresource(raw_data.Get(), 0, nullptr, convert.GetPixels(),
			convert.GetMetadata().width * sizeof(UINT), 0);
	} else {
		graphics.get_context()->UpdateSubresource(raw_data.Get(), 0, nullptr, img.GetPixels(),
			img.GetMetadata().width * sizeof(UINT), 0);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	srvd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;

	D3D11_SAMPLER_DESC sd;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.BorderColor[0] = 0.0f;
	sd.BorderColor[1] = 0.0f;
	sd.BorderColor[2] = 0.0f;
	sd.BorderColor[3] = 0.0f;
	sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sd.MaxAnisotropy = 1;
	sd.MinLOD = 0.0f;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0.0f;

	graphics.get_device()->CreateShaderResourceView(raw_data.Get(), &srvd, &tex);
	graphics.get_device()->CreateSamplerState(&sd, &sampler);
}

void Texturer::bind() {
	graphics.get_context()->PSSetShaderResources(tex_slot, 1, tex.GetAddressOf());
	graphics.get_context()->PSSetSamplers(sampler_slot, 1, sampler.GetAddressOf());
}

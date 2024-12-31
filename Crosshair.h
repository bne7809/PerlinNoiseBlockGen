#pragma once

#include "GraphicsHandle.h"
#include "VertexBuffer.h"

class Crosshair
{
public:
	Crosshair(GraphicsHandle& graphics);

	void draw(GraphicsHandle& graphics);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vb;
};
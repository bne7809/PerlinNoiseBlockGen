#include "CubeRenderer.h"

CubeRenderer::CubeRenderer(GraphicsHandle& graphics, const std::string& file)
	: graphics(graphics)
{
	VertexBuffer buffer(VERT_DESC_POS_3D | VERT_DESC_TEX_2D);

	D3D11_BUFFER_DESC vbd;
	vbd.ByteWidth = buffer.get_size();
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA vsd;
	vsd.pSysMem = buffer.get_data();
	vsd.SysMemPitch = 0;
	vsd.SysMemSlicePitch = 0;

	graphics.get_device()->CreateBuffer(&vbd, &vsd, &vb);
}

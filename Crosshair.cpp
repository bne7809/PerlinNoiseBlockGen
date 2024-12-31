#include "Crosshair.h"

Crosshair::Crosshair(GraphicsHandle& graphics) {
	VertexBuffer vertices(VERT_DESC_POS_3D);

	vertices.append_vertex(DirectX::XMFLOAT3({ 397.5f, 302.5f, 0.6f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 402.5f, 302.5f, 0.6f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 397.5f, 297.5f, 0.6f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 402.5f, 297.5f, 0.6f }));

	D3D11_BUFFER_DESC vbd;
	vbd.ByteWidth = vertices.get_size();
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vsd;
	vsd.pSysMem = vertices.get_data();
	vsd.SysMemPitch = 0;
	vsd.SysMemSlicePitch = 0;

	graphics.get_device()->CreateBuffer(&vbd, &vsd, &vb);
}

void Crosshair::draw(GraphicsHandle& graphics) {
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;

	graphics.get_context()->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &stride, &offset);
	graphics.get_context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	graphics.get_context()->Draw(4, 0);

}

#include "Mesh.h"

void Mesh::load(GraphicsHandle& graphics, VertexBuffer& buffer, const IndexBuffer& indices) {
	stride = buffer.get_stride();
	num_verts = buffer.get_num_verts();
	num_indices = indices.size();

	D3D11_BUFFER_DESC vbd;
	vbd.ByteWidth = buffer.get_size();
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vsd;
	vsd.pSysMem = buffer.get_data();
	vsd.SysMemPitch = 0;
	vsd.SysMemSlicePitch = 0;

	D3D11_BUFFER_DESC ibd;
	ibd.ByteWidth = indices.size() * sizeof(index_type);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA isd;
	isd.pSysMem = indices.data();
	isd.SysMemPitch = 0;
	isd.SysMemSlicePitch = 0;

	graphics.get_device()->CreateBuffer(&vbd, &vsd, vb.ReleaseAndGetAddressOf());
	graphics.get_device()->CreateBuffer(&ibd, &isd, ib.ReleaseAndGetAddressOf());
}

void Mesh::draw(GraphicsHandle& graphics) {
	UINT offset = 0;

	graphics.get_context()->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &stride, &offset);
	graphics.get_context()->IASetIndexBuffer(ib.Get(), INDEX_FORMAT, 0);
	graphics.get_context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	graphics.get_context()->DrawIndexed(num_indices, 0, 0);
}
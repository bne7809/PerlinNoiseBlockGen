#pragma once

#include "Mesh.h"
#include "Camera.h"

class ChunkRenderer;

class Chunk : public Mesh {
	friend ChunkRenderer;
public:
	static constexpr int CHUNK_SIZE = 32;
	static constexpr int CHUNK_HEIGHT = 32;
	static constexpr float BLOCK_LEN = 1.0f;
	static constexpr float CHUNK_LEN = CHUNK_SIZE * BLOCK_LEN;
private:
	enum block_t : uint8_t {
		air,
		dirt,
		stone
	};

	Chunk(const DirectX::XMFLOAT3& pos, const DirectX::XMINT3& offset)
		: blocks(new uint8_t[CHUNK_VOL]()), pos(pos), offset(offset) {}
	~Chunk();

	void generate_mesh(GraphicsHandle& graphics);
	void regenerate_mesh(GraphicsHandle& graphics, UINT num_verts, UINT num_indices);

	void add_upper_face(VertexBuffer& buffer, std::vector<USHORT>& indices, USHORT& icount, int x, int y, int z, uint8_t type);
	void add_lower_face(VertexBuffer& buffer, std::vector<USHORT>& indices, USHORT& icount, int x, int y, int z, uint8_t type);
	void add_left_face(VertexBuffer& buffer, std::vector<USHORT>& indices, USHORT& icount, int x, int y, int z, uint8_t type);
	void add_right_face(VertexBuffer& buffer, std::vector<USHORT>& indices, USHORT& icount, int x, int y, int z, uint8_t type);
	void add_front_face(VertexBuffer& buffer, std::vector<USHORT>& indices, USHORT& icount, int x, int y, int z, uint8_t type);
	void add_back_face(VertexBuffer& buffer, std::vector<USHORT>& indices, USHORT& icount, int x, int y, int z, uint8_t type);
	
	void remove_block(GraphicsHandle& graphics, int x, int y, int z);

	using Mesh::load;

	uint8_t* blocks;
	DirectX::XMFLOAT3 pos;
	DirectX::XMINT3 offset;

	static constexpr int CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
	static constexpr int CHUNK_VOL = CHUNK_AREA * CHUNK_HEIGHT;
};

class Highlight {
public:
	Highlight(GraphicsHandle& graphics);

	void draw(GraphicsHandle& graphics);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vb;
};

class ChunkRenderer {
public:
	ChunkRenderer(GraphicsHandle& graphics, const DirectX::XMFLOAT3& pos, int seed);
	~ChunkRenderer();

	void update(const Camera& cam, SHORT lpress, DirectX::XMFLOAT3& hpos, bool& coll);
	void draw();

	static constexpr int NUM_CHUNKS = 4;
	static constexpr float FAR_Z = 64.0f;
	static constexpr float FAR_Z_LEN = 64.0f*Chunk::BLOCK_LEN;
private:
	GraphicsHandle& graphics;
	Chunk* chunks;
	DirectX::XMFLOAT3 pos;
	int seed;

	static constexpr int TERRAIN_SIZE = NUM_CHUNKS * Chunk::CHUNK_SIZE;
	static constexpr float TERRAIN_LEN = TERRAIN_SIZE * Chunk::BLOCK_LEN;
	static constexpr int NCHUNK_AREA = NUM_CHUNKS * NUM_CHUNKS;
	static constexpr int NSURR_CHUNK = FAR_Z / (Chunk::BLOCK_LEN * Chunk::CHUNK_SIZE);
};
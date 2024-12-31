#include "Chunk.h"
#include "SimplexNoise.h"
#include <cmath>
#include <string>

void Chunk::generate_mesh(GraphicsHandle& graphics) {
	VertexBuffer vertices(VERT_DESC_POS_3D | VERT_DESC_TEX_2D);
	std::vector<USHORT> indices;

	USHORT inum = 0;
	for (int y = 0; y < CHUNK_HEIGHT; y++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			for (int x = 0; x < CHUNK_SIZE; x++) {
				uint8_t type = blocks[x + z * CHUNK_SIZE + y * CHUNK_AREA];
				if (type != air) {
					if (x == 0) {
						if (offset.x == 0) {
							add_left_face(vertices, indices, inum, x, y, z, type);
						} else if ((this - 1)->blocks[CHUNK_SIZE - 1 + z * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_left_face(vertices, indices, inum, x, y, z, type);
						}

						if (blocks[(x + 1) + z * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_right_face(vertices, indices, inum, x, y, z, type);
						}
					} else {
						if (blocks[(x - 1) + z * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_left_face(vertices, indices, inum, x, y, z, type);
						}

						if (x == (CHUNK_SIZE - 1)) {
							if (offset.x == (ChunkRenderer::NUM_CHUNKS - 1)) {
								add_right_face(vertices, indices, inum, x, y, z, type);
							} else if ((this + 1)->blocks[z * CHUNK_SIZE + y * CHUNK_AREA] == air) {
								add_right_face(vertices, indices, inum, x, y, z, type);
							}
						} else if (blocks[(x + 1) + z * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_right_face(vertices, indices, inum, x, y, z, type);
						}
					}

					if (z == 0) {
						if (offset.z == 0) {
							add_back_face(vertices, indices, inum, x, y, z, type);
						} else if ((this - ChunkRenderer::NUM_CHUNKS)->blocks[x + (CHUNK_SIZE - 1)*CHUNK_SIZE + y*CHUNK_AREA] == air) {
							add_back_face(vertices, indices, inum, x, y, z, type);
						}

						if (blocks[x + (z + 1) * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_front_face(vertices, indices, inum, x, y, z, type);
						}
					} else {
						if (blocks[x + (z - 1) * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_back_face(vertices, indices, inum, x, y, z, type);
						}

						if (z == (CHUNK_SIZE - 1)) {
							if (offset.z == (ChunkRenderer::NUM_CHUNKS - 1)) {
								add_front_face(vertices, indices, inum, x, y, z, type);
							} else if ((this + ChunkRenderer::NUM_CHUNKS)->blocks[x + y*CHUNK_AREA] == air) {
								add_front_face(vertices, indices, inum, x, y, z, type);
							}
						} else if (blocks[x + (z + 1) * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_front_face(vertices, indices, inum, x, y, z, type);
						}
					}

					if (y == 0) {
						add_upper_face(vertices, indices, inum, x, y, z, type);
					} else {
						if (blocks[x + z * CHUNK_SIZE + (y - 1) * CHUNK_AREA] == air) {
							add_upper_face(vertices, indices, inum, x, y, z, type);
						}

						if (y == (CHUNK_HEIGHT - 1)) {
							add_lower_face(vertices, indices, inum, x, y, z, type);
						} else if (blocks[x + z * CHUNK_SIZE + (y + 1) * CHUNK_AREA] == air) {
							add_lower_face(vertices, indices, inum, x, y, z, type);
						}
					}
				}
			}
		}
	}

	load(graphics, vertices, indices);
}

void Chunk::regenerate_mesh(GraphicsHandle& graphics, UINT num_verts, UINT num_indices) {
	VertexBuffer vertices(VERT_DESC_POS_3D | VERT_DESC_TEX_2D, num_verts);
	std::vector<USHORT> indices;
	indices.reserve(num_indices);

	USHORT inum = 0;
	for (int y = 0; y < CHUNK_HEIGHT; y++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			for (int x = 0; x < CHUNK_SIZE; x++) {
				uint8_t type = blocks[x + z * CHUNK_SIZE + y * CHUNK_AREA];
				if (type != air) {
					if (x == 0) {
						add_left_face(vertices, indices, inum, x, y, z, type);

						if (blocks[(x + 1) + z * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_right_face(vertices, indices, inum, x, y, z, type);
						}
					} else {
						if (blocks[(x - 1) + z * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_left_face(vertices, indices, inum, x, y, z, type);
						}

						if (x == (CHUNK_SIZE - 1)) {
							add_right_face(vertices, indices, inum, x, y, z, type);
						} else if (blocks[(x + 1) + z * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_right_face(vertices, indices, inum, x, y, z, type);
						}
					}

					if (z == 0) {
						add_back_face(vertices, indices, inum, x, y, z, type);

						if (blocks[x + (z + 1) * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_front_face(vertices, indices, inum, x, y, z, type);
						}
					} else {
						if (blocks[x + (z - 1) * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_back_face(vertices, indices, inum, x, y, z, type);
						}

						if (z == (CHUNK_SIZE - 1)) {
							add_front_face(vertices, indices, inum, x, y, z, type);
						} else if (blocks[x + (z + 1) * CHUNK_SIZE + y * CHUNK_AREA] == air) {
							add_front_face(vertices, indices, inum, x, y, z, type);
						}
					}

					if (y == 0) {
						add_upper_face(vertices, indices, inum, x, y, z, type);
					} else {
						if (blocks[x + z * CHUNK_SIZE + (y - 1) * CHUNK_AREA] == air) {
							add_upper_face(vertices, indices, inum, x, y, z, type);
						}

						if (y == (CHUNK_HEIGHT - 1)) {
							add_lower_face(vertices, indices, inum, x, y, z, type);
						} else if (blocks[x + z * CHUNK_SIZE + (y + 1) * CHUNK_AREA] == air) {
							add_lower_face(vertices, indices, inum, x, y, z, type);
						}
					}
				}
			}
		}
	}

	load(graphics, vertices, indices);
}

void Chunk::add_upper_face(VertexBuffer& buffer, std::vector<USHORT>& indices, USHORT& icount, 
	int x, int y, int z, uint8_t type) 
{
	USHORT inum = icount;
	if (type == dirt) {
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + x * BLOCK_LEN, pos.y - y * BLOCK_LEN, pos.z - z * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.0f, 0.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + (x + 1) * BLOCK_LEN, pos.y - y * BLOCK_LEN, pos.z - z * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.25f, 0.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + x * BLOCK_LEN, pos.y - y * BLOCK_LEN, pos.z - (z + 1) * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.0f, 1.0f/3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + (x + 1) * BLOCK_LEN, pos.y - y * BLOCK_LEN, pos.z - (z + 1) * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.25f, 1.0f / 3.0f }));
	}

	indices.push_back(inum);
	indices.push_back(inum + 1);
	indices.push_back(inum + 2);
	indices.push_back(inum + 1);
	indices.push_back(inum + 3);
	indices.push_back(inum + 2);
	icount += 4;
}

void Chunk::add_lower_face(VertexBuffer& buffer, std::vector<USHORT>& indices, USHORT& icount, 
	int x, int y, int z, uint8_t type) 
{
	USHORT inum = icount;
	if (type == dirt) {
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + x * BLOCK_LEN, pos.y - (y + 1) * BLOCK_LEN, pos.z - z * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.0f, 2.0f/3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + (x + 1) * BLOCK_LEN, pos.y - (y + 1) * BLOCK_LEN, pos.z - z * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.25f, 2.0f/3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + x * BLOCK_LEN, pos.y - (y + 1) * BLOCK_LEN, pos.z - (z + 1) * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.0f, 1.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + (x + 1) * BLOCK_LEN, pos.y - (y + 1) * BLOCK_LEN, pos.z - (z + 1) * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.25f, 1.0f }));
	}

	indices.push_back(inum + 1);
	indices.push_back(inum + 2);
	indices.push_back(inum + 3);
	indices.push_back(inum + 1);
	indices.push_back(inum);
	indices.push_back(inum + 2);
	icount += 4;
}

void Chunk::add_left_face(VertexBuffer& buffer, std::vector<USHORT>& indices, USHORT& icount, 
	int x, int y, int z, uint8_t type) 
{
	USHORT inum = icount;
	if (type == dirt) {
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + x * BLOCK_LEN, pos.y - y * BLOCK_LEN, pos.z - z * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.0f, 1.0f / 3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + x * BLOCK_LEN, pos.y - y * BLOCK_LEN, pos.z - (z + 1) * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.25f, 1.0f / 3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + x * BLOCK_LEN, pos.y - (y + 1) * BLOCK_LEN, pos.z - z * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.0f, 2.0f/3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + x * BLOCK_LEN, pos.y - (y + 1) * BLOCK_LEN, pos.z - (z + 1) * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.25f, 2.0f/3.0f }));
	}

	indices.push_back(inum);
	indices.push_back(inum + 1);
	indices.push_back(inum + 2);
	indices.push_back(inum + 1);
	indices.push_back(inum + 3);
	indices.push_back(inum + 2);
	icount += 4;
}

void Chunk::add_right_face(VertexBuffer& buffer, std::vector<USHORT>& indices, USHORT& icount, 
	int x, int y, int z, uint8_t type) 
{
	USHORT inum = icount;
	if (type == dirt) {
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + (x + 1) * BLOCK_LEN, pos.y - y * BLOCK_LEN, pos.z - z * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.0f, 1.0f / 3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + (x + 1) * BLOCK_LEN, pos.y - y * BLOCK_LEN, pos.z - (z + 1) * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.25f, 1.0f / 3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + (x + 1) * BLOCK_LEN, pos.y - (y + 1) * BLOCK_LEN, pos.z - z * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.0f, 2.0f / 3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + (x + 1) * BLOCK_LEN, pos.y - (y + 1) * BLOCK_LEN, pos.z - (z + 1) * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.25f, 2.0f / 3.0f }));
	}

	indices.push_back(inum + 1);
	indices.push_back(inum + 2);
	indices.push_back(inum + 3);
	indices.push_back(inum + 1);
	indices.push_back(inum);
	indices.push_back(inum + 2);
	icount += 4;
}

void Chunk::add_front_face(VertexBuffer& buffer, std::vector<USHORT>& indices, USHORT& icount, 
	int x, int y, int z, uint8_t type) 
{
	USHORT inum = icount;
	if (type == dirt) {
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + x * BLOCK_LEN, pos.y - y * BLOCK_LEN, pos.z - (z + 1) * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.0f, 1.0f / 3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + (x + 1) * BLOCK_LEN, pos.y - y * BLOCK_LEN, pos.z - (z + 1) * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.25f, 1.0f / 3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + x * BLOCK_LEN, pos.y - (y + 1) * BLOCK_LEN, pos.z - (z + 1) * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.0f, 2.0f / 3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + (x + 1) * BLOCK_LEN, pos.y - (y + 1) * BLOCK_LEN, pos.z - (z + 1) * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.25f, 2.0f / 3.0f }));
	}

	indices.push_back(inum);
	indices.push_back(inum + 1);
	indices.push_back(inum + 2);
	indices.push_back(inum + 1);
	indices.push_back(inum + 3);
	indices.push_back(inum + 2);
	icount += 4;
}

void Chunk::add_back_face(VertexBuffer& buffer, std::vector<USHORT>& indices, USHORT& icount, 
	int x, int y, int z, uint8_t type) 
{
	USHORT inum = icount;
	if (type == dirt) {
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + x * BLOCK_LEN, pos.y - y * BLOCK_LEN, pos.z - z * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.0f, 1.0f / 3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + (x + 1) * BLOCK_LEN, pos.y - y * BLOCK_LEN, pos.z - z * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.25f, 1.0f / 3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + x * BLOCK_LEN, pos.y - (y + 1) * BLOCK_LEN, pos.z - z * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.0f, 2.0f / 3.0f }));
		buffer.append_vertex(DirectX::XMFLOAT3({
			pos.x + (x + 1) * BLOCK_LEN, pos.y - (y + 1) * BLOCK_LEN, pos.z - z * BLOCK_LEN
			}), DirectX::XMFLOAT2({ 0.25f, 2.0f / 3.0f }));
	}

	indices.push_back(inum + 1);
	indices.push_back(inum + 2);
	indices.push_back(inum + 3);
	indices.push_back(inum + 1);
	indices.push_back(inum);
	indices.push_back(inum + 2);
	icount += 4;
}

void Chunk::remove_block(GraphicsHandle& graphics, int x, int y, int z) {
	blocks[x + z * CHUNK_SIZE + y * CHUNK_AREA] = air;
	UINT nnv = num_verts;
	UINT nni = num_indices;
	if (x == 0) {
		nnv -= 4;
		nni -= 6;

		if (blocks[(x + 1) + z * CHUNK_SIZE + y * CHUNK_AREA] == air) {
			nnv -= 4;
			nni -= 6;
		}
	} else {
		if (blocks[(x - 1) + z * CHUNK_SIZE + y * CHUNK_AREA] == air) {
			nnv -= 4;
			nni -= 6;
		}

		if (x == (CHUNK_SIZE - 1)) {
			nnv -= 4;
			nni -= 6;
		}
		else if (blocks[(x + 1) + z * CHUNK_SIZE + y * CHUNK_AREA] == air) {
			nnv -= 4;
			nni -= 6;
		}
	}

	if (z == 0) {
		nnv -= 4;
		nni -= 6;

		if (blocks[x + (z + 1) * CHUNK_SIZE + y * CHUNK_AREA] == air) {
			nnv -= 4;
			nni -= 6;
		}
	} else {
		if (blocks[x + (z - 1) * CHUNK_SIZE + y * CHUNK_AREA] == air) {
			nnv -= 4;
			nni -= 6;
		}

		if (z == (CHUNK_SIZE - 1)) {
			nnv -= 4;
			nni -= 6;
		}
		else if (blocks[x + (z + 1) * CHUNK_SIZE + y * CHUNK_AREA] == air) {
			nnv -= 4;
			nni -= 6;
		}
	}

	if (y == 0) {
		nnv -= 4;
		nni -= 6;
	} else {
		if (blocks[x + z * CHUNK_SIZE + (y - 1) * CHUNK_AREA] == air) {
			nnv -= 4;
			nni -= 6;
		}

		if (y == (CHUNK_HEIGHT - 1)) {
			nnv -= 4;
			nni -= 6;
		}
		else if (blocks[x + z * CHUNK_SIZE + (y + 1) * CHUNK_AREA] == air) {
			nnv -= 4;
			nni -= 6;
		}
	}
	regenerate_mesh(graphics, nnv, nni);
}

Chunk::~Chunk() {
	delete[] blocks;
}

ChunkRenderer::ChunkRenderer(GraphicsHandle& graphics, const DirectX::XMFLOAT3& pos, int seed) 
	: graphics(graphics), pos(pos), seed(seed)
{
	uint8_t *mem = new uint8_t[sizeof(Chunk) * NCHUNK_AREA];
	chunks = (Chunk*) mem;

	for (int z = 0; z < NUM_CHUNKS; z++) {
		for (int x = 0; x < NUM_CHUNKS; x++) {
			new (mem) Chunk({ pos.x + x * Chunk::CHUNK_SIZE * Chunk::BLOCK_LEN, pos.y, 
				pos.z - z * Chunk::CHUNK_SIZE * Chunk::BLOCK_LEN }, {x, 0, z});
			mem += sizeof(Chunk);
		}
	}
	
	for (int z = 0; z < TERRAIN_SIZE; z++) {
		for (int x = 0; x < TERRAIN_SIZE; x++) {
			int blockx = x / Chunk::CHUNK_SIZE;
			int blockz = z / Chunk::CHUNK_SIZE;
			int xp = x - blockx * Chunk::CHUNK_SIZE;
			int zp = z - blockz * Chunk::CHUNK_SIZE;

			float val = SimplexNoise::noise((seed + x) / 64.0f, (seed + z) / 64.0f);
			val = (val + 1) / 2;

			val *= 30.0f;
			int height = static_cast<int>(val) + 1;

			for (int y = (Chunk::CHUNK_HEIGHT - height); y < Chunk::CHUNK_HEIGHT; y++) {
				chunks[blockx + NUM_CHUNKS * blockz].blocks[xp + zp * Chunk::CHUNK_SIZE + y * Chunk::CHUNK_AREA] = Chunk::dirt;
			}
		}
	}

	for (int i = 0; i < NCHUNK_AREA; i++) {
		chunks[i].generate_mesh(graphics);
	}
}

void ChunkRenderer::update(const Camera& cam, SHORT lpress, DirectX::XMFLOAT3& hpos, bool& coll) {
	DirectX::XMFLOAT3 ray_pos, ray_dir;
	DirectX::XMStoreFloat3(&ray_pos, cam.pos);
	DirectX::XMStoreFloat3(&ray_dir, cam.look_dir);

	ray_pos.x -= pos.x;
	ray_pos.y = pos.y - ray_pos.y;
	ray_pos.z = pos.z - ray_pos.z;

	OutputDebugStringA((std::to_string(ray_pos.x) + " " + std::to_string(ray_pos.y) + " " +
		std::to_string(ray_pos.z) + "\n").c_str());

	DirectX::XMFLOAT3 ray_pos_initial = ray_pos;

	DirectX::XMFLOAT3 ray_target = ray_pos;
	ray_target.x += 4.0f*ray_dir.x;
	ray_target.y += 4.0f*ray_dir.y;
	ray_target.z += 4.0f*ray_dir.z;

	float s_x = 1 + (ray_dir.y / ray_dir.x) * (ray_dir.y / ray_dir.x) + (ray_dir.z / ray_dir.x) * (ray_dir.z / ray_dir.x);
	float s_y = 1 + (ray_dir.x / ray_dir.y) * (ray_dir.x / ray_dir.y) + (ray_dir.z / ray_dir.y) * (ray_dir.z / ray_dir.y);
	float s_z = 1 + (ray_dir.y / ray_dir.z) * (ray_dir.y / ray_dir.z) + (ray_dir.x/ ray_dir.z) * (ray_dir.x / ray_dir.z);
	
	while((ray_pos.x != ray_target.x) && (ray_pos.y != ray_target.y) && (ray_pos.z != ray_target.z)) {
		ray_pos.x += s_x * (1.0f - (ray_pos.x - std::floorf(ray_pos.x))) * (1.0f - (ray_pos.x - std::floorf(ray_pos.x)));
		ray_pos.y += s_y * (1.0f - (ray_pos.y - std::floorf(ray_pos.y))) * (1.0f - (ray_pos.y - std::floorf(ray_pos.y)));
		ray_pos.z += s_z * (1.0f - (ray_pos.z - std::ceilf(ray_pos.z))) * (1.0f - (ray_pos.z - std::ceilf(ray_pos.z)));

		if (!((ray_pos_initial.x == ray_pos.x) && (ray_pos_initial.y == ray_pos.y) && (ray_pos_initial.z == ray_pos.z))) {
			ray_pos.x += s_x;
			ray_pos.y += s_y;
			ray_pos.z += s_z;
		}



		int cx = ray_pos.x / Chunk::CHUNK_LEN;
		int cz = ray_pos.z / Chunk::CHUNK_LEN;

		int bx = (ray_pos.x - (cx * Chunk::CHUNK_LEN)) / Chunk::BLOCK_LEN;
		int by = ray_pos.y / Chunk::BLOCK_LEN;
		int bz = (ray_pos.z - (cz * Chunk::CHUNK_LEN)) / Chunk::BLOCK_LEN;

		if (by >= 0) {
			if (chunks[cx + NUM_CHUNKS * cz].blocks[bx + bz * Chunk::CHUNK_SIZE + by * Chunk::CHUNK_AREA] != Chunk::air) {
				if (lpress) { 
					chunks[cx + NUM_CHUNKS * cz].remove_block(graphics, bx, by, bz);
					coll = false;
				} else {
					coll = true;
					hpos.x = (cx * Chunk::CHUNK_LEN + bx * Chunk::BLOCK_LEN) + pos.x;
					hpos.y =  pos.y - by * Chunk::BLOCK_LEN;
					hpos.z = pos.z - (cz * Chunk::CHUNK_LEN + bz * Chunk::BLOCK_LEN);
				}
			}
		}
	}

	coll = false;
}

void ChunkRenderer::draw() {
	for (int i = 0; i < NCHUNK_AREA; i++) {
		chunks[i].draw(graphics);
	}
}

ChunkRenderer::~ChunkRenderer() {
	uint8_t* mem = (uint8_t*) chunks;

	for (int i = 0; i < NCHUNK_AREA; i++) {
		chunks[i].~Chunk();
	}

	delete[] mem;
}

Highlight::Highlight(GraphicsHandle& graphics) {
	VertexBuffer vertices(VERT_DESC_POS_3D);

	constexpr float len = 1.05f;

	// top
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, 0.0f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, 0.0f }));

	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, 0.0f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, -len }));

	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, -len }));

	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, 0.0f }));

	// front
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, -len }));

	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ len, -len, -len }));

	vertices.append_vertex(DirectX::XMFLOAT3({ len, -len, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, -len, -len }));

	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, -len, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, -len }));

	// bottom
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, -len, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ len, -len, -len }));

	vertices.append_vertex(DirectX::XMFLOAT3({ len, -len, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, -len }));

	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, -len }));

	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, -len, -len }));

	// back
	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, 0.0f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, 0.0f }));

	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, 0.0f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, -len, 0.0f }));

	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, -len, 0.0f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ len, -len, 0.0f }));

	vertices.append_vertex(DirectX::XMFLOAT3({ len, -len, 0.0f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, 0.0f }));

	// left
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, 0.0f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, -len, 0.0f }));

	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, -len, 0.0f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, -len, -len }));

	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, -len, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, -len }));

	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ 0.0f, 0.0f, 0.0f }));

	// right
	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, 0.0f }));

	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, 0.0f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ len, -len, 0.0f }));

	vertices.append_vertex(DirectX::XMFLOAT3({ len, -len, 0.0f }));
	vertices.append_vertex(DirectX::XMFLOAT3({ len, -len, -len }));

	vertices.append_vertex(DirectX::XMFLOAT3({ len, -len, -len }));
	vertices.append_vertex(DirectX::XMFLOAT3({ len, 0.0f, -len }));

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

void Highlight::draw(GraphicsHandle& graphics) {
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;

	graphics.get_context()->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &stride, &offset);
	graphics.get_context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	graphics.get_context()->Draw(48, 0);
}
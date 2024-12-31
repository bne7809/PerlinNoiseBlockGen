#pragma once

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>

enum VERT_DESC {
	VERT_DESC_POS_3D = 0b1,
	VERT_DESC_TEX_2D = 0b10,
	VERT_DESC_COL_4D = 0b100,
	VERT_DESC_NORMAL = 0b1000,
	VERT_DESC_TANGENT = 0b10000
};

#define VERT_DESC_SIZE(d)	((d & VERT_DESC_POS_3D) * sizeof(DirectX::XMFLOAT3) + \
							((d & VERT_DESC_TEX_2D) >> 1) * sizeof(DirectX::XMFLOAT2) + \
							((d & VERT_DESC_COL_4D) >> 2) * sizeof(DirectX::XMFLOAT4) + \
							((d & VERT_DESC_NORMAL) >> 3) * sizeof(DirectX::XMFLOAT3) + \
							((d & VERT_DESC_TANGENT) >> 4) * sizeof(DirectX::XMFLOAT3))

#define VERT_DESC_NUM(d)	((d & VERT_DESC_POS_3D) + \
							((d & VERT_DESC_TEX_2D) >> 1) + \
							((d & VERT_DESC_COL_4D) >> 2) + \
							((d & VERT_DESC_NORMAL) >> 3) + \
							((d & VERT_DESC_TANGENT) >> 4))

class VertexBuffer {
public:
	VertexBuffer(UINT desc)
		: desc(desc) {}

	VertexBuffer(UINT desc, UINT num_verts) 
		: desc(desc)
	{
		data.reserve(VERT_DESC_SIZE(desc) * num_verts);
	}

	/*template <typename T>
	void append_vertex(T&& t) {
		char* ptr = (char*)&t;
		size_t t_size = sizeof(T);
		for (int i = 0; i < t_size; i++) {
			data.push_back(ptr[i]);
		}
	}

	template <typename T, typename ... Rest>
	void append_vertex(T&& t, Rest&&... rest) {
		append_vertex<T>(std::forward<T>(t));
		append_vertex<Rest...>(std::forward<Rest>(rest)...);
	}*/

	template <typename T>
	void append_vertex(const T& t) {
		char* ptr = (char*)&t;
		size_t t_size = sizeof(T);
		for (int i = 0; i < t_size; i++) {
			data.push_back(ptr[i]);
		}
	}

	template <typename T, typename ... Rest>
	void append_vertex(const T& t, const Rest&... rest) {
		append_vertex<T>(t);
		append_vertex<Rest...>(rest...);
	}

	inline UINT get_num_verts() const { return data.size()/VERT_DESC_SIZE(desc); }
	inline void* get_data() { return data.data(); }
	inline size_t get_size() const { return data.size(); }
	inline UINT get_stride() { return VERT_DESC_SIZE(desc); }

	inline UINT get_desc() { return desc; }
private:
	std::vector<char> data;
	UINT desc;
};
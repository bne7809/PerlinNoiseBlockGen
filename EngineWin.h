#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "Chunk.h"
#include "Shader.h"
#include "ConstBuffer.h"
#include "Texturer.h"
#include "Crosshair.h"

class EngineWin {
public:
	EngineWin(LPCSTR title, int width, int height);
	~EngineWin();

	void start();
private:
	struct const_buffer_t {
		DirectX::XMMATRIX transform;
	};

	static LRESULT CALLBACK StaticWinCallback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT CALLBACK win_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	inline void hide_cursor() { while(ShowCursor(FALSE) >= 0); }
	inline void show_cursor() { while (ShowCursor(TRUE) < 0); }
	void confine_cursor();

	void load();

	HWND hwnd;
	HINSTANCE hinst;
	LPCSTR class_name;

	struct mouse_movement {
		int dx = 0;
		int dy = 0;
		bool left_press = false;
	} raw_mouse_input;

	std::unique_ptr<GraphicsHandle> graphics;

	std::unique_ptr<ChunkRenderer> chunks;
	std::unique_ptr<Shader> def_shader;
	std::unique_ptr<VertexConstBuff<const_buffer_t>> cbh;
	std::unique_ptr<Texturer> test_tex;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Crosshair> crosshair;
	std::unique_ptr<Highlight> highlight;
	std::unique_ptr<Shader> cross_shader;
};
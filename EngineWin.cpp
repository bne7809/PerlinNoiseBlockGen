#include "EngineWin.h"

EngineWin::EngineWin(LPCSTR title, int width, int height)
	: class_name(title), hinst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = StaticWinCallback;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinst;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = title;
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(0, title, title, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 200, 200,
		width, height, nullptr, nullptr, hinst, nullptr);

	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	graphics = std::make_unique<GraphicsHandle>(hwnd, width, height);

	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;

	RegisterRawInputDevices(&rid, 1, sizeof(rid));
	
	ShowWindow(hwnd, SW_SHOW);
}

void EngineWin::load() {
	const_buffer_t cb = {
		{
			DirectX::XMMatrixTranspose
			(
				DirectX::XMMatrixRotationY(3.14f / 4.0f) *
				DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, ChunkRenderer::FAR_Z_LEN)
			)
		}
	};
	
	chunks = std::make_unique<ChunkRenderer>(*graphics, 
		DirectX::XMFLOAT3({ -(Chunk::BLOCK_LEN * Chunk::CHUNK_SIZE * ChunkRenderer::NUM_CHUNKS) / 2.0f,
			0.0f, (Chunk::BLOCK_LEN * Chunk::CHUNK_SIZE * ChunkRenderer::NUM_CHUNKS) / 2.0f}), 100);
	def_shader = std::make_unique<Shader>(*graphics, L"x64/Debug/def_vert_shader.cso",
		L"x64/Debug/def_pixel_shader.cso", VERT_DESC_POS_3D | VERT_DESC_TEX_2D);
	cbh = std::make_unique<VertexConstBuff<const_buffer_t>>(*graphics, cb, 0);
	test_tex = std::make_unique<Texturer>(*graphics, L"blocks_spritesheet.png", 0, 0);
	crosshair = std::make_unique<Crosshair>(*graphics);
	highlight = std::make_unique<Highlight>(*graphics);
	cross_shader = std::make_unique<Shader>(*graphics, L"x64/Debug/crosshair_vert_shader.cso", L"x64/Debug/crosshair_pixel_shader.cso",
		VERT_DESC_POS_3D);

	camera = std::make_unique<Camera>();
}

void EngineWin::start() {
	load();

	cbh->bind();
	test_tex->bind();

	hide_cursor();
	confine_cursor();

	MSG msg;
	while (true) {
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) return;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		graphics->begin_frame(1.0f, 1.0f, 1.0f, 1.0f);

		DirectX::XMFLOAT3 hpos;
		bool coll;

		{
			camera->update(raw_mouse_input.dx, raw_mouse_input.dy,
				GetAsyncKeyState('W'), GetAsyncKeyState('S'), GetAsyncKeyState('A'), GetAsyncKeyState('D'));
			def_shader->bind();
			cbh->set(
				{
					{
						DirectX::XMMatrixTranspose
						(
							camera->get_proj() *
							DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, ChunkRenderer::FAR_Z_LEN)
						)
					}
				});

			chunks->update(*camera, GetAsyncKeyState(VK_LBUTTON), hpos, coll);
			chunks->draw();
		}

		{
			cross_shader->bind();

			if (coll) {
				cbh->set(
					{
						{
							DirectX::XMMatrixTranspose
							(
								DirectX::XMMatrixTranslation(hpos.x, hpos.y, hpos.z) *
								//DirectX::XMMatrixTranslation(20.0f, -10.0f, 0.0f) *
								camera->get_proj() *
								DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, ChunkRenderer::FAR_Z_LEN)
							)
						}
					}
				);
				graphics->disable_depth_test();
				highlight->draw(*graphics);
				graphics->enable_depth_test();
			}

			cbh->set(
				{
					{
						DirectX::XMMatrixTranspose
						(
							DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 800.0f, 0.0f, 600.0f, 0.5f, 1.0f)
						)
					}
				});

			crosshair->draw(*graphics);
		}

		graphics->end_frame();
	}
}

LRESULT EngineWin::StaticWinCallback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	EngineWin* const wnd = reinterpret_cast<EngineWin*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	return wnd->win_callback(hwnd, msg, wparam, lparam);
}

LRESULT EngineWin::win_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		confine_cursor();
		break;
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE) PostQuitMessage(0);
		if (wparam == VK_UP) raw_mouse_input.dy -= 5;
		if (wparam == VK_DOWN) raw_mouse_input.dy += 5;
		if (wparam == VK_LEFT) raw_mouse_input.dx -= 5;
		if (wparam == VK_RIGHT) raw_mouse_input.dx += 5;
		break;
	case WM_KEYUP:
		raw_mouse_input.dx = 0;
		raw_mouse_input.dy = 0;
		break;
	/*case WM_MOUSEMOVE:
		raw_mouse_input.dx = GET_X_LPARAM(lparam)/50.0f - raw_mouse_input.dx;
		raw_mouse_input.dy = GET_Y_LPARAM(lparam)/50.0f - raw_mouse_input.dy;
		UINT size = 0;

		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam),
			RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1) break;

		raw_buffer.resize(size);

		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT,
			raw_buffer.data(), &size, sizeof(RAWINPUTHEADER)) != size) break;

		auto& ri = reinterpret_cast<const RAWINPUT&>(*raw_buffer.data());
		
		if (ri.header.dwType == RIM_TYPEMOUSE) {
			if (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0) {
				raw_mouse_input.dx = ri.data.mouse.lLastX;
				raw_mouse_input.dy = ri.data.mouse.lLastY;
			}
			else {
				raw_mouse_input.dx = 0;
				raw_mouse_input.dy = 0;
			}
			raw_mouse_input.left_press = (ri.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN);
		}
		break;*/
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void EngineWin::confine_cursor() {
	RECT rect;
	GetClientRect(hwnd, &rect);
	MapWindowPoints(hwnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

EngineWin::~EngineWin() {
	UnregisterClass(class_name, hinst);
}
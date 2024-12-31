#include "EngineWin.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpScmdline, _In_ int nCmdShow) {
	EngineWin window("3D Test 2", 800, 600);
	window.start();
	
	return 0;
}
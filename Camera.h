#pragma once

#include <DirectXMath.h>

class Camera {
public:
	Camera(DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f }, float pitch = 0.0f, float yaw = 0.0f)
		: pos(DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f)), pitch(pitch), yaw(yaw), 
		look_dir(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)) {}

	void update(float dx, float dy, short move_w, short move_s, short move_a, short move_d) noexcept;
	DirectX::XMMATRIX get_proj();

	DirectX::XMVECTOR pos;
	DirectX::XMVECTOR look_dir;

	float pitch;
	float yaw;

	static constexpr float rot_speed = 0.01f;
	static constexpr float mov_speed = 0.5f;
};
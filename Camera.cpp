#include "Camera.h"
#include <algorithm>

void Camera::update(float dx, float dy, short move_w, short move_s, short move_a, short move_d) noexcept {
	const DirectX::XMVECTOR forward_vector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR side_vector = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	yaw += dx * rot_speed;
	pitch = std::clamp(pitch + dy * rot_speed, -3.1415f / 2.0f, 3.1415f / 2.0f);

	const DirectX::XMMATRIX rot_matrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);

	look_dir = DirectX::XMVector3Transform(forward_vector, rot_matrix);
	side_vector = DirectX::XMVectorScale(DirectX::XMVector3Transform(side_vector, rot_matrix), mov_speed);
	
	DirectX::XMVECTOR move_dir = DirectX::XMVectorScale(look_dir, mov_speed);

	if (move_w) pos = DirectX::XMVectorAdd(pos, move_dir);
	if (move_s) pos = DirectX::XMVectorSubtract(pos, move_dir);
	if (move_a) pos = DirectX::XMVectorSubtract(pos, side_vector);
	if (move_d) pos = DirectX::XMVectorAdd(pos, side_vector);
}

DirectX::XMMATRIX Camera::get_proj() {	
	DirectX::XMVECTOR target = DirectX::XMVectorAdd(pos, look_dir);
	
	return DirectX::XMMatrixLookAtLH(pos, target, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

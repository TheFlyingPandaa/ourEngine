#pragma once
#include <glm\glm.hpp>
#include "Camera.h"
#include <Windows.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\transform.hpp>

const float ORBIT_CAMERA_YAW_DEFAULT = 0.0f;
const float ORBIT_CAMERA_PITCH_DEFUALT = 20.0f;
const float ORBIT_CAMERA_MOUSE_SENSITIVTY = 0.1f;
const float ORBIT_CAMERA_DISTANCE_DEFUALT = 200.0f;
const float ORBIT_CAMERA_ANGLE_DEFUALT = 0.0f;

class OrbitCamera : public Camera
{
public:

	glm::vec3 Position;
	float Yaw;
	float Pitch;
	float MouseSensitivity;
	float DistanceToTarget;
	float AngleAroundTarget;
	glm::vec3 CameraTargetPos;
	glm::vec3 CameraTargetRot;

	OrbitCamera();
	OrbitCamera(glm::vec3 cameraTarget);
	~OrbitCamera();

	void update();
	const glm::mat4 GetViewMatrix() const;
	void ProcessMouseMovement(float offsetX, float offsetY);
	void ProcessMouseScroll(double offsetY);
	float InvertPitch();

private:
	void updateCameraPostion(float horizontalDistance, float verticalDistance);
};

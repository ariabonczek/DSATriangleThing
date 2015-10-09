#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Config.hpp"

NS_BEGIN

class Camera
{
public:
	Camera();

	void Initialize();

	void SetLens(float fovY, float aspect, float zNear, float zFar);
	void SetLensOrtho(float width, float height, float zNear, float zFar);

	void UpdateViewMatrix();

	void Move(Vector3 move);
	void MoveForward(float speed);
	void MoveRight(float speed);
	void SetPosition(Vector3 position);

	void LookAt(Vector3 target, Vector3 up = Vector3::Up);

	void Pitch(float angle);
	void Yaw(float angle);
	void Roll(float angle);
	void RotateY(float angle);

	Matrix GetView()const;
	Matrix GetProjection()const;
	Matrix GetViewProjection()const;
	Vector3 GetPosition()const;
	bool IsDirty()const;

private:
	Vector3 position;
	Vector3 right;
	Vector3 up;
	Vector3 forward;

	float nearZ, farZ;
	float aspectRatio;
	float fovX, fovY;
	float nearWidth, nearHeight, farWidth, farHeight;

	// When dirty, the camera has been moved and the view matrix will need to be recalculated before rendering
	bool dirty;

	Matrix view;
	Matrix projection;
	// Cache this so it isn't computed too frequently
	Matrix viewProjection;
};

NS_END

#endif
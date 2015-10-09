#include "CameraSingletonDSA.hpp"

NS_BEGIN

CameraSingleton::CameraSingleton() 
{}

CameraSingleton::~CameraSingleton()
{}

CameraSingleton* CameraSingleton::GetInstance()
{
	static CameraSingleton c;
	return &c;
}

void CameraSingleton::Initialize()
{
	right = Vector3::Right;
	up = Vector3::Up;
	forward = Vector3::Forward;
	position = Vector3::Zero;
	projection = Matrix::CreatePerspectiveFov(0.25f * 3.141592f, 1280.0f / 720.0f, 0.1f, 100.0f);
	ortho = Matrix::CreateOrthographic(25.0f, 25.0f, 0.1f, 100.0f);
	CalculateViewMatrix();
}

void CameraSingleton::SetPosition(Vector3 position)
{
	this->position = position;
	
	dirty = true;
}

void CameraSingleton::SetTarget(Vector3 target)
{
	forward = (target - position).Normalized();
	up = Vector3::Cross(up, forward);
	right = Vector3::Cross(forward, up);

	dirty = true;
}

void CameraSingleton::SetUp(Vector3 up)
{
	this->up = up.Normalized();

	dirty = true;
}

void CameraSingleton::MoveForward(float d)
{
	position = position + forward * d;
	dirty = true;
}

void CameraSingleton::MoveRight(float d)
{
	position = position + right * d;
	dirty = true;
}

void CameraSingleton::MoveUp(float d)
{
	position = position + up * d;
	dirty = true;
}

void CameraSingleton::Pitch(float angle)
{
	Matrix m = Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(right, angle));

	forward = (forward * m).Normalized();
	right = (right * m).Normalized();
	up = Vector3::Cross(forward, right);

	dirty = true;
}

void CameraSingleton::Yaw(float angle)
{
	Matrix m = Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(up, angle));

	forward = (forward * m).Normalized();
	right = (right * m).Normalized();
	up = Vector3::Cross(forward, right);

	dirty = true;
}

void CameraSingleton::Roll(float angle)
{
	Matrix m = Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(forward, angle));

	forward = (forward * m).Normalized();
	right = (right * m).Normalized();
	up = Vector3::Cross(forward, right);

	dirty = true;
}

void CameraSingleton::RotateY(float angle)
{
	Matrix m = Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, angle));

	forward = (forward * m).Normalized();
	right = (right * m).Normalized();
	up = Vector3::Cross(forward, right);

	dirty = true;
}

Vector3 CameraSingleton::GetPosition()const
{
	return position;
}

Matrix CameraSingleton::GetView()const
{
	return view;
}

Matrix CameraSingleton::GetProjection(bool isOrthographic)const
{
	if (isOrthographic)
		return ortho;
	return projection;
}

bool CameraSingleton::IsDirty()const
{
	return dirty;
}

void CameraSingleton::CalculateViewMatrix()
{
	view = Matrix::CreateLookAt(position, position + forward, up);
	dirty = false;
}

NS_END
//------------------------------------------------------------
// Singleton version of the camera made for a class assignment
//------------------------------------------------------------
#ifndef CAMERA_SINGLETON_HPP
#define CAMERA_SINGLETON_HPP

#include "../Config.hpp"

NS_BEGIN

/// <summary>
/// Singleton class encapsulating a 3D camera
/// </summary>
class CameraSingleton
{
public:
	~CameraSingleton();

	/// <summary>
	/// Returns the single instance of the camera
	/// </summary>
	static CameraSingleton* GetInstance();

	/// <summary>
	/// Initializes the camera to default values
	/// </summary>
	void Initialize();

	/// <summary>
	/// Calculates the View Matrix from forward right up and position
	/// </summary>
	void CalculateViewMatrix();
	
	/// <summary>
	/// Sets the camera's position
	/// </summary>
	void SetPosition(Vector3 position);

	/// <summary>
	/// Sets the camera's target
	/// </summary>
	void SetTarget(Vector3 target);

	/// <summary>
	/// Sets the camera's up vector
	/// </summary>
	void SetUp(Vector3 up);

	/// <summary>
	/// Moves the camera forward by d
	/// </summary>
	void MoveForward(float d);

	/// <summary>
	/// Moves the camera right by d
	/// </summary>
	void MoveRight(float d);

	/// <summary>
	/// Moves the camera up by d
	/// </summary>
	void MoveUp(float d);

	/// <summary>
	/// Pitch the camera
	/// </summary>
	void Pitch(float angle);

	/// <summary>
	/// Yaw the camera
	/// </summary>
	void Yaw(float angle);

	/// <summary>
	/// Roll the camera
	/// </summary>
	void Roll(float angle);

	/// <summary>
	/// Rotate the camera by the world Y
	/// </summary>
	void RotateY(float angle);

	/// <summary>
	/// Returns the position of the camera
	/// </summary>
	Vector3 GetPosition()const;

	/// <summary>
	/// Returns the view matrix
	/// </summary>
	Matrix GetView()const;

	/// <summary>
	/// Returns the projection matrix
	/// </summary>
	Matrix GetProjection(bool isOrthographic)const;

	/// <summary>
	/// Returns true if the view matrix needs to be updated
	/// </summary>
	bool IsDirty()const;
private:
	CameraSingleton();
	CameraSingleton& operator=(const CameraSingleton& c);

	bool dirty;

	Vector3 position;
	Vector3 forward;
	Vector3 right;
	Vector3 up;

	float nearZ, farZ;
	float aspectRatio;
	float fovX, fovY;
	float nearWidth, nearHeight, farWidth, farHeight;

	Matrix view;
	Matrix projection;
	Matrix ortho;
};

NS_END

#endif
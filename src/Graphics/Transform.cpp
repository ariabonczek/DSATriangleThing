#include "Transform.hpp"

NS_BEGIN

Transform::Transform(GameObject* obj) :
object(obj),
localPosition(0.0f, 0.0f, 0.0f),
localRotation(0.0f, 0.0f, 0.0f, 1.0f),
localScale(1.0f, 1.0f, 1.0f),
forward(0.0f, 0.0f, 1.0f),
up(0.0f, 1.0f, 0.0f),
right(1.0f, 0.0f, 0.0f),
parent(nullptr),
worldCache(Matrix::Identity)
{
	children.reserve(3);
}

Transform::Transform(const Transform& transform)
{
	parent = transform.parent;
	children = transform.children;
	localPosition = transform.localPosition;
	localScale = transform.localScale;
	localRotation = transform.localRotation;
	forward = transform.forward;
	up = transform.up;
	right = transform.right;
	worldCache = transform.worldCache;
}

Transform::~Transform()
{}

Transform& Transform::operator=(const Transform& transform)
{
	parent = transform.parent;
	children = transform.children;
	localPosition = transform.localPosition;
	localScale = transform.localScale;
	localRotation = transform.localRotation;
	forward = transform.forward;
	up = transform.up;
	right = transform.right;
	worldCache = transform.worldCache;
	return *this;
}


void Transform::Translate(Vector3 v)
{
	localPosition = localPosition + v;
	UpdateWorldMatrix();
}

void Transform::Rotate(Quaternion rotation)
{
	localRotation = (rotation * localRotation);
	UpdateWorldMatrix();
}

void Transform::SetLocalPosition(float x, float y, float z){ localPosition = Vector3(x, y, z); UpdateWorldMatrix();  } 
void Transform::SetLocalPosition(Vector3 position){ localPosition = position; UpdateWorldMatrix(); }
void Transform::SetLocalRotation(Quaternion rotation){ localRotation = rotation; UpdateWorldMatrix(); } 
void Transform::SetLocalScale(float x, float y, float z) { localScale = Vector3(x, y, z); UpdateWorldMatrix(); } 
void Transform::SetLocalScale(Vector3 scale){ localScale = scale; UpdateWorldMatrix(); }

Vector3 Transform::GetLocalPosition()const{ return localPosition; }
Quaternion Transform::GetLocalRotation()const{ return localRotation; }
Vector3 Transform::GetLocalScale()const { return localScale; }

Vector3 Transform::GetWorldPosition()
{
	if (IsBatman()) return localPosition;
	else
	{
		return localPosition + parent->GetWorldPosition();
	}
}
Quaternion Transform::GetWorldRotation()
{
	if (IsBatman()) return localRotation;
	else
	{
		return localRotation * parent->GetWorldRotation();
	}
}

Vector3 Transform::GetWorldScale()
{
	if (IsBatman()) return localScale;
	else
	{
		return localScale * parent->GetWorldScale();
	}
}

Matrix Transform::GetWorldMatrix()const
{
	if (IsBatman()) return worldCache;
	else
	{
		return worldCache * parent->GetWorldMatrix();
	}
}

Matrix Transform::GetWorldInverseTranspose()
{
	return Matrix::Transpose(Matrix::Inverse(worldCache));
}

void Transform::SetParent(Transform* transform)
{
	if (parent == transform)
		return;

	// TODO: Make this safe
	parent = transform;
	parent->AddChild(this);
}

void Transform::AddChild(Transform* transform)
{
	// Check if the transform is already in list of children
	for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		if (transform == *it)
		{
			return;
		}
	}

	transform->SetParent(this);
	children.push_back(transform);
}

Transform* Transform::GetParent()
{
	return parent;
}

std::vector<Transform*> Transform::GetChildren()
{
	return children;
}

void Transform::UpdateWorldMatrix()
{
	Matrix r, s, t;
	Vector3 worldPosition = GetWorldPosition();
	Quaternion worldRotation = GetWorldRotation();
	Vector3 worldScale = GetWorldScale();

	r = Matrix::CreateFromQuaternion(worldRotation);
	s = Matrix::CreateScale(worldScale);
	t = Matrix::CreateTranslation(worldPosition);

	worldCache = s*r*t;

	forward = Vector3::Normalize(Vector3::Forward * r);
	right = Vector3::Normalize(Vector3::Right * r);
	up = Vector3::Cross(forward, right);

	for (uint i = 0; i < children.size(); i++)
	{
		children.at(i)->UpdateWorldMatrix();
	}
}

bool Transform::IsBatman()const
{
	if (parent)
		return false;
	return true;
}

Vector3 Transform::GetForward()const
{
	return forward;
}

Vector3 Transform::GetUp()const
{
	return up;
}

Vector3 Transform::GetRight()const
{
	return right;
}


NS_END
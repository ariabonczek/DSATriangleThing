#include "GameObject.hpp"

#include "Mesh.hpp"
#include "Material.hpp"

NS_BEGIN

GameObject::GameObject(std::string name, Mesh* mesh, Material* material):
name(name),
mesh(mesh),
material(material)
{
	transform = new Transform(this);
}

GameObject::GameObject(const GameObject& gameObject)
{
	name = gameObject.name;
	mesh = gameObject.mesh;
	material = gameObject.material;
	transform = gameObject.transform;
}

GameObject::~GameObject()
{}

GameObject& GameObject::operator=(const GameObject& gameObject)
{
	name = gameObject.name;
	mesh = gameObject.mesh;
	material = gameObject.material;
	transform = gameObject.transform;
	return *this;
}

void GameObject::Update(float dt)
{
	
}

void GameObject::Draw()
{
	material->Bind();

	material->SetFloat4x4("model", transform->GetWorldMatrix());
	material->SetFloat4x4("modelInverseTranspose", transform->GetWorldInverseTranspose());
	mesh->Draw();
}

void GameObject::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void GameObject::SetMaterial(Material* mat)
{
	this->material = mat;
}

std::string GameObject::GetName()const
{
	return name;
}

Transform* GameObject::GetTransform()const
{
	return transform;
}

Mesh* GameObject::GetMesh()const
{
	return mesh;
}

Material* GameObject::GetMaterial()const
{
	return material;
}

NS_END
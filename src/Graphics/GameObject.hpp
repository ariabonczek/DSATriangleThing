#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "../Config.hpp"
#include "Transform.hpp"

NS_BEGIN

class Mesh;
class Material;

class GameObject
{
public:
	GameObject(std::string name, Mesh* = nullptr, Material* = nullptr);
	GameObject(const GameObject& gameObject);
	~GameObject();

	GameObject& operator=(const GameObject& gameObject);

	void Update(float dt);
	void Draw();

	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* mat);

	std::string GetName()const;
	Transform* GetTransform()const;
	Mesh* GetMesh()const;
	Material* GetMaterial()const;
private:
	std::string name;
	Transform* transform;
	Mesh* mesh;
	Material* material;
};

NS_END


#endif
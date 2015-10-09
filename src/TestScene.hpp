#ifndef TESTSCENE_HPP
#define TESTSCENE_HPP

#include "LuminaGL.hpp"

using namespace LuminaGL;

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	void LoadAssets();
	void Update(float dt);
	void Draw();
	void UnloadAssets();
private:
	void MoveCamera(float dt);

	Camera camera;
	
	const float TRIANGLESIZE = 1.0f;
	bool polygonFlag;

	//Camera camera;
	std::vector<GameObject*> objects;
	std::vector<Mesh*> meshes;
	std::vector<Material*> mats;

	Vector2 mousePosition;
	Vector2 pMousePosition;
};


#endif

#include "TestScene.hpp"
#include "Utility\Timer.hpp"
#include "Graphics\CameraSingletonDSA.hpp"

TestScene::TestScene()
{}

TestScene::~TestScene()
{}

void TestScene::LoadAssets()
{
	camera.Initialize();

	//--------------------------
	// Creates the triangle mesh
	//--------------------------
	float halfSize = TRIANGLESIZE * 0.5f;
	MeshVertex vertices[3] = {
		{ Vector3(-halfSize, -halfSize, 0.0f), Vector2(0.0f, 0.0f), Color::Red, Vector3(0.0f, 0.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f) },
		{ Vector3(halfSize, -halfSize, 0.0f), Vector2(0.0f, 0.0f), Color::Red, Vector3(0.0f, 0.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f) },
		{ Vector3(0.0f, halfSize, 0.0f), Vector2(0.0f, 0.0f), Color::Red, Vector3(0.0f, 0.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f) },
	};
	size_t indices[3] = { 0, 2, 1 };

	MeshData d;
	d.vertices.resize(3);
	d.indices.resize(3);

	memcpy(&d.vertices[0], vertices, sizeof(vertices));
	memcpy(&d.indices[0], indices, sizeof(indices));

	meshes.push_back(new Mesh(d));

	//------------------------------
	// Creates the triangle material
	//------------------------------
	Material* mat = new Material();
	mat->LoadShader("Shaders/default.vert", ShaderType::Vertex);
	mat->LoadShader("Shaders/default.frag", ShaderType::Fragment);
	mats.push_back(mat);

	//
	// Creates the triangle object
	//
	GameObject* triangle = new GameObject("Triangle");

	triangle->SetMesh(meshes[0]);
	triangle->SetMaterial(mats[0]);
	objects.push_back(triangle);

	camera.SetPosition(Vector3(0.0f, 0.0f, -2.0f));
}

void TestScene::Update(float dt)
{
	MoveCamera(dt);

	if (Input::GetKeyDown(GLFW_KEY_SPACE))
	{
		polygonFlag ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		polygonFlag = !polygonFlag;
	}

	if (camera.IsDirty())
	{
		camera.UpdateViewMatrix();
	}
}

void TestScene::Draw()
{
	objects[0]->GetMaterial()->SetFloat4x4("view", camera.GetView());
	objects[0]->GetMaterial()->SetFloat4x4("projection", camera.GetProjection());
	objects[0]->GetMaterial()->SetFloat3("viewPos", camera.GetPosition());
	for (GameObject* o : objects)
	{
		o->GetMaterial()->SetFloat4x4("model", o->GetTransform()->GetWorldMatrix());
		o->GetMaterial()->SetFloat4x4("modelInverseTranspose", o->GetTransform()->GetWorldInverseTranspose());

		o->Draw();
	}
}

void TestScene::UnloadAssets()
{
	for (Mesh* m : meshes)
		delete m;
	for (Material* m : mats)
		delete m;
	for (GameObject* g : objects)
		delete g;
}

void TestScene::MoveCamera(float dt)
{
	pMousePosition = mousePosition;
	mousePosition = Input::GetMousePosition();

	float speed = 10.0f * dt;
	if (Input::GetKey(GLFW_KEY_W))
	{
		camera.MoveForward(speed);
	}
	else if (Input::GetKey(GLFW_KEY_S))
	{
		camera.MoveForward(-speed);
	}

	if (Input::GetKey(GLFW_KEY_A))
	{
		camera.MoveRight(-speed);
	}
	else if (Input::GetKey(GLFW_KEY_D))
	{
		camera.MoveRight(speed);
	}

	speed *= 10.0f;

	if (Input::GetKey(GLFW_KEY_UP))
	{
		camera.Pitch(-speed);
	}
	else if (Input::GetKey(GLFW_KEY_DOWN))
	{
		camera.Pitch(speed);
	}

	if (Input::GetKey(GLFW_KEY_LEFT))
	{
		camera.RotateY(-speed);
	}
	else if (Input::GetKey(GLFW_KEY_RIGHT))
	{
		camera.RotateY(speed);
	}

	// Mouse controls
	if (Input::GetKey(GLFW_KEY_LEFT_ALT))
	{
		Vector2 diff = mousePosition - pMousePosition;

		camera.RotateY(diff.x);
		camera.Pitch(diff.y);
	}
}
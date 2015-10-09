#ifndef SCENE_HPP
#define SCENE_HPP

#include "Config.hpp"

NS_BEGIN

/// <summary>
/// Data class, holds what's in a scene. Overriden for actual scenes
/// </summary>
class Scene
{
public:
	Scene() {}
	virtual ~Scene(){}

	virtual void LoadAssets() = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;	
	virtual void UnloadAssets() = 0;
};

NS_END

#endif
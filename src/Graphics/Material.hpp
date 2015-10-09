#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../Config.hpp"

NS_BEGIN

struct Color;

enum class ShaderType
{
	Vertex,
	TessControl,
	TessEval,
	Geometry,
	Fragment,
	Compute
};

class Material
{
public:
	Material();
	Material(const Material& material);
	~Material();

	Material& operator=(const Material& material);

	bool LoadShader(std::string filepath, ShaderType type);

	void Bind();

	uint GetProgram()const;

	void SetFloat(std::string name, float& value);
	void SetFloat2(std::string name, Vector2& value);
	void SetFloat3(std::string name, Vector3& value);
	void SetFloat4(std::string name, Vector4& value);
	void SetFloat4x4(std::string name, Matrix& value);
	void SetColor(std::string name, Color& color);
private:
	uint vertex, tessControl, tessEval, geometry, fragment, compute;
	uint program;
};

NS_END

#endif
#include "Material.hpp"
#include "../Utility/FileSystem.hpp"
#include "Data.hpp"

NS_BEGIN

Material::Material() :
	program(0)
{}

Material::Material(const Material& material)
{
	vertex = material.vertex;
	tessControl = material.tessControl;
	tessEval = material.tessEval;
	geometry = material.geometry;
	fragment = material.fragment;
	compute = material.compute;
	program = material.program;
}

Material::~Material()
{}

Material& Material::operator=(const Material& material)
{
	vertex = material.vertex;
	tessControl = material.tessControl;
	tessEval = material.tessEval;
	geometry = material.geometry;
	fragment = material.fragment;
	compute = material.compute;
	program = material.program;
	return *this;
}

bool Material::LoadShader(std::string filepath, ShaderType type)
{
	if (!program)
		program = glCreateProgram();

	const char* shaderCode = FileSystem::LoadTextFile(filepath.c_str());

	if (!shaderCode)
		return 0;

	uint* index = 0;
	GLenum t;
	switch (type)
	{
	case ShaderType::Vertex:
		index = &vertex;
		t = GL_VERTEX_SHADER;
		break;
	case ShaderType::TessControl:
		index = &tessControl;
		t = GL_TESS_CONTROL_SHADER;
		break;
	case ShaderType::TessEval:
		index = &tessEval;
		t = GL_TESS_EVALUATION_SHADER;
		break;
	case ShaderType::Geometry:
		index = &geometry;
		t = GL_GEOMETRY_SHADER;
		break;
	case ShaderType::Fragment:
		index = &fragment;
		t = GL_FRAGMENT_SHADER;
		break;
	case ShaderType::Compute:
		index = &compute;
		t = GL_COMPUTE_SHADER;
		break;
	default:
		return 0;
	}

	*index = glCreateShader(t);
	glShaderSource(*index, 1, &shaderCode, 0);
	glCompileShader(*index);

#if LUMINA_DEBUG
	GLint check;
	glGetShaderiv(*index, GL_COMPILE_STATUS, &check);

	if (!check)
	{
		char log[1000] = { 0 };
		int length;
		glGetShaderiv(*index, GL_INFO_LOG_LENGTH, &length);
		glGetShaderInfoLog(*index, length, 0, log);
		std::cout << log << std::endl;

		return 0;
	}

	glAttachShader(program, *index);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &check);
	if (!check)
	{
		char log[1000] = { 0 };
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		glGetShaderInfoLog(program, length, 0, log);
		std::cout << log << std::endl;
		glDeleteProgram(program);

		return 0;
	}
#endif

	return true;
}

void Material::Bind()
{
	glUseProgram(program);
}

uint Material::GetProgram()const
{
	return program;
}

void Material::SetFloat(std::string name, float& value)
{
	uint location = glGetUniformLocation(program, name.c_str());
	glProgramUniform1f(program, location, value);
}

void Material::SetFloat2(std::string name, Vector2& value)
{
	uint location = glGetUniformLocation(program, name.c_str());
	glProgramUniform2f(program, location, value.x, value.y);
}

void Material::SetFloat3(std::string name, Vector3& value)
{
	uint location = glGetUniformLocation(program, name.c_str());
	glProgramUniform3f(program, location, value.x, value.y, value.z);
}

void Material::SetFloat4(std::string name, Vector4& value)
{
	uint location = glGetUniformLocation(program, name.c_str());
	glProgramUniform4f(program, location, value.x, value.y, value.z, value.w);
}

void Material::SetFloat4x4(std::string name, Matrix& value)
{
	uint location = glGetUniformLocation(program, name.c_str());
	glProgramUniformMatrix4fv(program, location, 1, GL_TRUE, &value.m11);
}

void Material::SetColor(std::string name, Color& color)
{
	uint location = glGetUniformLocation(program, name.c_str());
	glProgramUniform4f(program, location, color.r, color.g, color.b, color.a);
}

NS_END
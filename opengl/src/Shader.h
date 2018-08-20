#pragma once
#include <string>
#include "vendor\glm\glm.hpp"
#include <unordered_map>
struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
private:
	const std::string m_FilePath;
	unsigned int m_RendererId;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void SetUniform4f(const std::string name, float f0, float f1, float f2, float f3);
	void SetUniform1f(const std::string name, float f);
	void SetUniform1i(const std::string name, int  i);
	void SetUniformMat4f(const std::string name, const glm::mat4 matrix);
private:
	int GetUniformLocation(const std::string& name);
	unsigned int  CreateShader(const std::string& vertexShader, const std::string& framentShader);
	unsigned int  CompileShader(unsigned int type, const std::string& source);
	ShaderSource  ParseShader(const std::string filepath);

};


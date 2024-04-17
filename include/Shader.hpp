#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

class Shader
{
public:
  Shader(const char* vertexPath, const char* fragmentPath);

  std::string GetFileAsString(const char* filename) const;

  void Use();
  
  void SetUniform(const char* name, const float value) const;
  void SetUniform(const char* name, const int value) const;
  void SetUniform(const char* name, const bool value) const;
  void SetUniform(const char* name, const float x, const float y, const float z) const;
  void SetUniform(const char* name, const glm::mat4& mat) const;

  unsigned int GetID() const;

private:
  unsigned int CreateShader(const char* source, const int shaderType);
  void CreateProgram(unsigned int vertShad, unsigned int fragShad);
  
private:
  unsigned int programID;
};

#endif
#include "Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
  std::string vertStr = GetFileAsString(vertexPath).c_str();
  std::string fragStr = GetFileAsString(fragmentPath).c_str();

  unsigned int vertShad = CreateShader(vertStr.c_str(), GL_VERTEX_SHADER);
  unsigned int fragShad = CreateShader(fragStr.c_str(), GL_FRAGMENT_SHADER);

  CreateProgram(vertShad, fragShad);

  glDeleteShader(vertShad);
  glDeleteShader(fragShad);
}

std::string Shader::GetFileAsString(const char* filename) const
{
  std::ifstream file;
  std::stringstream str;

  file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  try
  {
    file.open(filename);
    str << file.rdbuf();
    file.close();
  }
  catch(std::ifstream::failure e)
  {
    std::cout << "Error loading file " << filename << ":\n";
    std::cerr << e.what() << '\n';
    return "";
  }
  
  return str.str();
}

void Shader::Use()
{
  glUseProgram(programID);
}

void Shader::SetUniform(const char* name, float value) const
{
  glUniform1f(glGetUniformLocation(programID, name), value);
}
void Shader::SetUniform(const char* name, bool value) const
{
  SetUniform(name, (int)value);
}

void Shader::SetUniform(const char* name, int value) const
{
  glUniform1i(glGetUniformLocation(programID, name), value);
}

void Shader::SetUniform(const char* name, float x, float y, float z) const
{
  glUniform3f(glGetUniformLocation(programID, name), x, y, z);
}

void Shader::SetUniform(const char* name, glm::mat4& mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, glm::value_ptr(mat));
}

unsigned int Shader::GetID() const
{
  return programID;
}

unsigned int Shader::CreateShader(const char* source, const int shaderType)
{
  int success;
  char infoLog[512];

  unsigned int shader;
  shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "Shader compilation failed:\n" << infoLog << '\n';
  }

  return shader;
}

void Shader::CreateProgram(unsigned int vertShad, unsigned int fragShad)
{
  int success;
  char infoLog[512];

  programID = glCreateProgram();
  glAttachShader(programID, vertShad);
  glAttachShader(programID, fragShad);
  glLinkProgram(programID);

  glGetProgramiv(programID, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(programID, 512, NULL, infoLog);
    std::cout << "Shader program linking failed:\n" << infoLog << '\n';
  }
}

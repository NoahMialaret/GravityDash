#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Vec2.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Texture
{
public:
  Texture() = default;
  ~Texture() = default;

  void LoadFromFile(const char* filename);
  iVec2 GetSize();
  void Bind();

private:
  unsigned int ID;
  int width = 0;
  int height = 0;
};

#endif
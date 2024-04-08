#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Texture.hpp"
#include "Shader.hpp"
#include "Vec2.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Sprite
{
public:
  Sprite();
  Sprite(Texture* tex, Shader* shad);
  
  void SetTexture(Texture* tex);
  void SetTextureBounds(iVec2 topLeft, iVec2 bottomRight);
  void SetShader(Shader* shad);

  void SetOrigin(fVec2 pos);

  void GetBoundingRectangle();

  void Render();

  void Translate();
  void SetPosition();
  void Rotate();
  void SetRotation();
  void Scale();
  void SetScale(float scale);
  void SetScale(fVec2 scale);

  fVec2 GetTranslation() const;
  float GetRotation() const;
  fVec2 GetScale() const;

private:
  Texture* texture = nullptr;
  Shader* shader = nullptr;

  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;

  // The sprites transformation matrix
  glm::mat4 transform = glm::mat4(1.0f);
  // Determine if the transformation matrix should be updated
  // due to one of the transformations being changed
  bool renderPropertiesChanged = false;

  fVec2 origin;
  fVec2 translation;
  fVec2 scale;
  float rotation;

  float vertices[16] = // In pixel coordinates
  {
    0.0f, 0.0f, 0.0f, 1.0f,   // top left 
    1.0f, 0.0f, 1.0f, 1.0f,   // top right
    0.0f, -1.0f, 0.0f, 0.0f,   // bottom left
    1.0f, -1.0f, 1.0f, 0.0f    // bottom right
  };  
  unsigned int indices[6] = {0, 1, 2, 1, 3, 2}; // Assumes TL-TR-BL-BR
};

#endif
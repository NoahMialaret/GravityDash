#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Rect.hpp"
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
  void SetTextureBounds(IRect bounds);
  void SetShader(Shader* shad);

  void SetOrigin(fVec2 pos);

  Texture* GetTexture() const;

  FRect GetBoundingRectangle() const;
  IRect GetTextureBounds() const;

  void Render() const;

  void Translate(fVec2 value);
  void SetTranslation(fVec2 value);
  void Rotate(float value);
  void SetRotation(float value);
  void Scale(float value);
  void Scale(fVec2 value);
  void SetScale(float value);
  void SetScale(fVec2 value);

  fVec2 GetTranslation() const;
  float GetRotation() const;
  fVec2 GetScale() const;

private:
  mutable Texture* texture = nullptr;
  mutable Shader shader = Shader("assets/vert.vs", "assets/frag.fs");

  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;

  // The sprites transformation matrix
  mutable glm::mat4 transform = glm::mat4(1.0f);
  // Determine if the transformation matrix should be updated
  // due to one of the transformations being changed
  mutable bool renderPropertiesChanged = false;

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
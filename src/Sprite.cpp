#include "Sprite.hpp"

Sprite::Sprite()
{
  // VAO stuff
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // EBO stuff
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // VBO stuff
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Remember to use dynamic draw when using sprites that change
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

Sprite::Sprite(Texture *tex, Shader *shad)
  :
  texture(tex),
  shader(shad)
{
  std::cout << tex->GetSize().x << " - " << tex->GetSize().y << '\n';

  SetOrigin(fVec2(4, 4));
  SetScale(6.0f);
  SetTextureBounds(iVec2(8), iVec2(16));

  // VAO stuff
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // EBO stuff
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // VBO stuff
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Remember to use dynamic draw when using sprites that change
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void Sprite::SetTexture(Texture* tex)
{
  texture = tex;
}

void Sprite::SetTextureBounds(iVec2 topLeft, iVec2 bottomRight)
{
  vertices[0] = - origin.x / 400.0f;
  vertices[4] = ((float)(bottomRight.x - topLeft.x) - origin.x) / 400.0f;
  vertices[8] = - origin.x / 400.0f;
  vertices[12] = ((float)(bottomRight.x - topLeft.x) - origin.x) / 400.0f;

  vertices[1] = origin.y / 300.0f;
  vertices[5] = origin.y / 300.0f;
  vertices[9] = -((float)(bottomRight.y - topLeft.y) - origin.y) / 300.0f;
  vertices[13] = -((float)(bottomRight.y - topLeft.y) - origin.y) / 300.0f;

  vertices[2] = (float)topLeft.x / texture->GetSize().x;
  vertices[6] = (float)bottomRight.x / texture->GetSize().x;
  vertices[10] = (float)topLeft.x / texture->GetSize().x;
  vertices[14] = (float)bottomRight.x / texture->GetSize().x;

  vertices[3] = 1.0f - (float)topLeft.y / texture->GetSize().y;
  vertices[7] = 1.0f - (float)topLeft.y / texture->GetSize().y;
  vertices[11] = 1.0f - (float)bottomRight.y / texture->GetSize().y;
  vertices[15] = 1.0f - (float)bottomRight.y / texture->GetSize().y;
}

void Sprite::SetShader(Shader* shad)
{
  shader = shad;
}

void Sprite::SetOrigin(fVec2 pos)
{
  origin = pos;
}

void Sprite::Render()
{
  shader->Use();
  texture->Bind();

    // glm::mat4 mat = glm::mat4(1.0f);
    // mat = glm::translate(mat, glm::vec3(0.5f, 0.5f, 0.0f));
    // mat = glm::rotate(mat, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    // mat = glm::scale(mat, glm::vec3(2.0f));

    shader->SetUniform("transform", transform);

  glBindVertexArray(VAO);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sprite::SetScale(float value)
{
  scale = fVec2(value);
  transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
  transform = glm::rotate(transform, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
  transform = glm::scale(transform, glm::vec3(scale.x, scale.y, 1.0f));
}

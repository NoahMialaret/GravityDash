#include "Texture.hpp"

void Texture::LoadFromFile(const char* filename)
{
  int numChannels;
  unsigned char* data = stbi_load(filename, &width, &height, &numChannels, 0);
  if (!data)
  {
    std::cout << "Failed to load texture:" << filename << "\n";
  }

  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
}

iVec2 Texture::GetSize()
{
  return iVec2(width, height);
}

void Texture::Bind()
{
  glBindTexture(GL_TEXTURE_2D, ID);
}

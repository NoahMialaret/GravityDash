#include "AssetManager.h"

AssetManager* AssetManager::instance = nullptr;

AssetManager::AssetManager()
{
  std::filesystem::path path;

  // Load Textures
  try
  {  
    path.assign("assets/textures/");
    for (auto& file : std::filesystem::directory_iterator{path})
    {
      if (!file.path().has_stem())
        continue;

      if (!textures[file.path().stem()].loadFromFile(file.path()))
      {
        std::cout << "WARNING: Could not load texture: " << file.path().stem() << '\n';
        textures.erase(file.path().stem());
        continue;
      }
    }
    if (textures.empty())
      std::cout << "WARNING: No textures were loaded, expected textures to be in 'assets/textures'\n";
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  // Load Fonts
  try
  {
    path.assign("assets/fonts/");
    for (auto& file : std::filesystem::directory_iterator{path})
    {
      if (!file.path().has_stem())
        continue;

      if (!fonts[file.path().stem()].loadFromFile(file.path()))
      {
        std::cout << "WARNING: Could not load font: " << file.path().stem() << '\n';
        fonts.erase(file.path().stem());
        continue;
      }
    }
    if (fonts.empty())
      std::cout << "WARNING: No fonts were loaded, expected fonts to be in 'assets/fonts'\n";
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  if (!sf::Shader::isAvailable())
  {
    std::cout << "\tShaders are not available on this hardware!\n";
    throw;
  }

  // Load Shaders
  try
  {
    std::string dir = "assets/shaders/";
    path.assign(dir);
    for (auto& file : std::filesystem::directory_iterator{path})
    {
      if (!file.path().has_stem() || !file.path().has_extension())
        continue;

      if (!shaders[file.path().stem()].loadFromFile(file.path(), sf::Shader::Type::Fragment))
      {
        std::cout << "WARNING: Could not load fragment shader: " << file.path().stem() << "\n";
        shaders.erase(file.path().stem());
        continue;
      }
      shaders[file.path().stem()].setUniform("texture", sf::Shader::CurrentTexture);
    }
    if (shaders.empty())
      std::cout << "WARNING: No shaders were loaded, expected shaders to be in 'assets/shaders'\n";
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}

void AssetManager::Clean()
{
  if (instance != nullptr)
    delete instance;
  instance = nullptr;
}

AssetManager* AssetManager::GetInstance()
{
  if (instance == nullptr)
    instance = new AssetManager();
  return instance;
}

sf::Texture& AssetManager::GetTex(std::string name)
{
  auto it = textures.find(name);
  if (it != textures.end())
    return it->second;
  
  std::cout << "WANRING: Could not find the texture '" << name << "', make sure it exists in 'assets/textures/'\n";
  return textures[name]; 
}

sf::Font& AssetManager::GetFont(std::string name)
{
  auto it = fonts.find(name);
  if (it != fonts.end())
    return it->second;
  
  std::cout << "WANRING: Could not find the font '" << name << "', make sure it exists in 'assets/fonts/'\n";
  return fonts[name]; 
}

sf::Shader& AssetManager::GetShad(std::string name)
{
  auto it = shaders.find(name);
  if (it != shaders.end())
    return it->second;
  
  std::cout << "WANRING: Could not find the shader '" << name << "', make sure it exists in 'assets/shaders/' as both a .vs and .fs shader\n";
  return shaders[name]; 
}

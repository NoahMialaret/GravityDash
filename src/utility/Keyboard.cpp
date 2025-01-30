#include "Keyboard.h"

Keyboard* Keyboard::instance = nullptr;

void Keyboard::Clean()
{
  if (instance != nullptr)
    delete instance;
  instance = nullptr;
}

void Keyboard::Update()
{
  for (auto prev = activeKeys.before_begin(), cur = activeKeys.begin(); cur != activeKeys.end();)
  {
    if (!sf::Keyboard::isKeyPressed(cur->keyCode))
    {
      cur = activeKeys.erase_after(prev);
      break;
    }
    
    cur->Update();
    prev++;
    cur++;
  }
}

void Keyboard::AddKeyPress(sf::Keyboard::Key keyCode)
{
  if(GetKey(keyCode) == nullptr)
	  activeKeys.push_front(Key(keyCode));
}

const Keyboard::Key* Keyboard::GetKey(sf::Keyboard::Key keyCode) const
{
  for (auto& key : activeKeys)
  {
    if (key.keyCode == keyCode)
      return &key;
  }

  return nullptr;
}

Keyboard *Keyboard::GetInstance()
{
  if (instance == nullptr)
    instance = new Keyboard();
  return instance;
}

int Keyboard::GetKeyCodeAtHead() const
{
  if (activeKeys.empty())
    return -1;

  return activeKeys.front().state == Key::State::initialClick 
          ? (int)activeKeys.front().keyCode 
          : -1;
}

bool Keyboard::IsKeyHeld(sf::Keyboard::Key keyCode) const
{
  const Key* key = GetKey(keyCode);

  return key == nullptr ? false : true;
}

bool Keyboard::IsKeyOnInitialClick(sf::Keyboard::Key keyCode) const
{
  const Key* key = GetKey(keyCode);

  return key == nullptr ? false : (key->state == Key::State::initialClick);
}

bool Keyboard::IsKeyClicked(sf::Keyboard::Key keyCode) const
{
  const Key* key = GetKey(keyCode);

  return key == nullptr ? false : (key->state == Key::State::initialClick || key->state == Key::State::click);
}

std::string Keyboard::GetStringFromKeyCode(sf::Keyboard::Key keyCode) const
{
  if (keyCode >= sf::Keyboard::A && keyCode <= sf::Keyboard::Z) // Letters
  {
    std::string ret;
    ret = (char)keyCode + 'a';
    return ret;
  }

  else if (keyCode >= sf::Keyboard::Num0 && keyCode <= sf::Keyboard::Num9) // Numbers
    return std::to_string(int(keyCode - sf::Keyboard::Num0));

  else if (keyCode >= sf::Keyboard::Numpad0 && keyCode <= sf::Keyboard::Numpad9) // Numpad
  {
    std::string ret = "npad" + std::to_string(int(keyCode - sf::Keyboard::Numpad0));
    return ret;
  }

  auto it = keyCodeStringMap.find((int)keyCode);
  if (it != keyCodeStringMap.end()) // Others (symbols and modifiers)
    return it->second;

  return "NULL";
}

sf::Keyboard::Key Keyboard::GetKeyCodeFromString(std::string keyString) const
{
  if (keyString.size() == 1)
  {
    if (keyString[0] >= 'a' && keyString[0] <= 'z') // Letters
      return sf::Keyboard::Key((int)sf::Keyboard::A + (keyString[0] - 'a'));

    else if (keyString[0] >= '0' && keyString[0] <= '9') // Numbers
      return sf::Keyboard::Key((int)sf::Keyboard::Num0 + (keyString[0] - '0'));
  }

  else if (keyString.find("npad") != std::string::npos) // Numpad
    return sf::Keyboard::Key((int)sf::Keyboard::Numpad0 + (keyString.back() - '0'));
  
  for (auto& pair : keyCodeStringMap) // Others (symbols and modifiers)
  {
    if (pair.second == keyString)
      return (sf::Keyboard::Key)pair.first;
  }

  throw std::invalid_argument("Could not find the key code corresponding to the string " + keyString + "!\n");
}

Keyboard::Key::Key(sf::Keyboard::Key keyCode)
  :
  keyCode(keyCode)
{}

void Keyboard::Key::Update()
{
  if (state == State::click || state == State::initialClick)
    state = State::hold;

  clickTimer -= DELTA_TIME;
  
  if (clickTimer < 0)
  {
    state = State::click;
    clickTimer = CLICK_TIME;
  }
}

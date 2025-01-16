#include "Keyboard.h"

std::forward_list<Keyboard::Key> Keyboard::keysPressed; 

// Stores pairs of key codes and strings for easier lookup when converting between the two
const std::vector<std::pair<sf::Keyboard::Key, std::string>> Keyboard::keyCodeStringPairs =
{
  {sf::Keyboard::Key::Escape,     "esc"},
  {sf::Keyboard::Key::Tab,        "tab"},
  {sf::Keyboard::Key::Enter,      "enter"},
  {sf::Keyboard::Key::Backspace,  "bcksp"},
  {sf::Keyboard::Key::LShift,     "lshft"},
  {sf::Keyboard::Key::RShift,     "rshft"},
  {sf::Keyboard::Key::LControl,   "lctrl"},
  {sf::Keyboard::Key::RControl,   "rctrl"},
  {sf::Keyboard::Key::Space,      "space"},
  {sf::Keyboard::Key::LAlt,       "lalt"},
  {sf::Keyboard::Key::RAlt,       "ralt"},
  {sf::Keyboard::Key::Up,         "up"},
  {sf::Keyboard::Key::Down,       "down"},
  {sf::Keyboard::Key::Left,       "left"},
  {sf::Keyboard::Key::Right,      "right"},
  {sf::Keyboard::Key::PageUp,     "p-up"},
  {sf::Keyboard::Key::PageDown,   "p-down"},
  {sf::Keyboard::Key::Grave,      "`"},
  {sf::Keyboard::Key::Hyphen,     "-"},
  {sf::Keyboard::Key::Equal,      "="},
  {sf::Keyboard::Key::LBracket,   "["},
  {sf::Keyboard::Key::RBracket,   "]"},
  {sf::Keyboard::Key::Backslash,  "\\"},
  {sf::Keyboard::Key::Semicolon,  ";"},
  {sf::Keyboard::Key::Apostrophe, "'"},
  {sf::Keyboard::Key::Comma,      ","},
  {sf::Keyboard::Key::Period,     "."},
  {sf::Keyboard::Key::Slash,      "/"}
};

void Keyboard::AddKeyPress(sf::Keyboard::Key keyCode)
{
  if(GetKey(keyCode) == nullptr)
	  keysPressed.push_front(Key(keyCode));
}

bool Keyboard::IsKeyHeld(sf::Keyboard::Key keyCode)
{
  Key* key = GetKey(keyCode);

  return key == nullptr ? false : true;
}

bool Keyboard::IsKeyOnInitialClick(sf::Keyboard::Key keyCode)
{
  Key* key = GetKey(keyCode);

  return key == nullptr ? false : (key->state == Key::State::initialClick);
}

bool Keyboard::IsKeyClicked(sf::Keyboard::Key keyCode)
{
  Key* key = GetKey(keyCode);

  return key == nullptr ? false : (key->state == Key::State::initialClick || key->state == Key::State::click);
}

void Keyboard::Update()
{
  for (auto prev = keysPressed.before_begin(), cur = keysPressed.begin(); cur != keysPressed.end();)
  {
    if (!sf::Keyboard::isKeyPressed(cur->keyCode))
    {
      cur = keysPressed.erase_after(prev);
      break;
    }
    
    cur->Update();
    prev++;
    cur++;
  }
}

std::string Keyboard::GetStringFromKeyCode(sf::Keyboard::Key keyCode)
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

  for (auto& pair : keyCodeStringPairs) // Others (symbols and modifiers)
  {
    if (pair.first == keyCode)
      return pair.second;
  }

  return "NULL";
}

sf::Keyboard::Key Keyboard::GetKeyCodeFromString(std::string keyString)
{
  if (keyString.size() == 1)
  {
    if (keyString[0] >= 'a' && keyString[0] <= 'z') // Letters
      return sf::Keyboard::Key((int)sf::Keyboard::A + (keyString[0] - 'a'));

    else if (keyString[0] >= '0' && keyString[0] <= '9') // Numbers
      return sf::Keyboard::Key((int)sf::Keyboard::Num0 + (keyString[0] - '0'));
  }

  else if (keyString.find("npad") != std::string::npos) // Numpad
    return sf::Keyboard::Key((int)sf::Keyboard::Numpad0 + (keyString[keyString.size() - 1] - '0'));
  
  for (auto& pair : keyCodeStringPairs) // Others (symbols and modifiers)
  {
    if (pair.second == keyString)
      return pair.first;
  }

  throw std::invalid_argument("Could not find the key code corresponding to the string " + keyString + "!\n");
}

Keyboard::Key* Keyboard::GetKey(sf::Keyboard::Key keyCode)
{
  for (auto& key : keysPressed)
  {
    if (key.keyCode == keyCode)
      return &key;
  }

  return nullptr;
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
    clickTimer = 50;
  }
}

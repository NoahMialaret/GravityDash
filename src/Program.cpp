#include "Program.h"

Program::Program(const char* name)
  :
  name(name)
{
  LoadSave(SAVE_FILE);

	std::cout << "--=== Program Init ===--\n"  << "Initialising SFML Window...\n";

    // Finds the scale at which the window would be at least half the size of the desktop
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::Vector2u windowSize = ASPECT_RATIO;
    int scale = 1;
    while (windowSize.x < desktop.width / 2 && windowSize.y < desktop.height / 2)
    {
      windowSize += ASPECT_RATIO;
      scale++;
    }
    Settings::GetInstance()->autoScaleVal = scale;

    UpdateWindow();

    std::cout << "\tGame scale: " << SCALE << '\n';
    std::cout << "\tWindow dimensions: " << window.getSize().x << "px x " << window.getSize().y << "px\n";
    std::cout << "\tDesktop dimensions: " << desktop.width << "px x " << desktop.height << "px\n";

    // Sets the window to be the middle of the desktop
    window.setPosition(sf::Vector2i((desktop.width - window.getSize().x) / 2, (desktop.height - window.getSize().y) / 2));

  // Textures and Shaders

    if (GET_TEXTURE("icon").getSize() != sf::Vector2u(0, 0))
    {
      sf::Image icon = GET_TEXTURE("icon").copyToImage();
      window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }
    
	std::cout << "Initialising Program objects...\n";

    menu = std::make_unique<Menu>(Menu::Type::main);
    gameManager = std::make_unique<GameManager>(GameManager::Preset::title);
    
	std::cout << "Program init done! Starting title sequence...\n";
	
		curState = State::mainMenu;

	std::cout << "--===++++++++++++++===--\n";
}

Program::~Program()
{
  SaveData(SAVE_FILE);

	std::cout << "Cleaning program...\n";

	window.close();

  gameManager = nullptr;
  menu = nullptr;

  Utility::Clean();
  Clock::Clean();
  ParticleManager::Clean();
  EventManager::Clean();
  Keyboard::Clean();
  AssetManager::Clean();

	std::cout << "Program successfully cleaned!\n";
}

void Program::ProcessEvents() 
{
	if (curState == State::notRunning) 
		return;

	Event event;

	while (EventManager::GetInstance()->PollEvent(event))
	{
		switch (event.type)
		{
		case Event::Type::programClose: // Window close (from menu)
			std::cout << "Menu quit button has been pressed, closing program...\n";
			curState = State::notRunning;
			return;

		case Event::Type::gameNew: // New game
			std::cout << "Initialising new game...\n";
      gameManager = std::make_unique<GameManager>((GameManager::Preset)event.data.value);
			menu.get()->Clear();
      ParticleManager::Clean();
			curState = State::gameplay;
			break;

    case Event::Type::pause: // Pause
      assert(curState == State::gameplay);
      menu.get()->Push(Menu::Type::pause);
      curState = State::paused;
      break;

    case Event::Type::resume: // Resume
      assert(curState == State::paused);
      menu.get()->Return();
      curState = State::gameplay;
      break;

    case Event::Type::reloadMenu: // New menu
      menu.get()->ReloadStack((Menu::Type)event.data.value);
      break;
      
    case Event::Type::pushMenu: // Go to menu
      menu.get()->Push((Menu::Type)event.data.value);
      break;
    
    case Event::Type::menuReturn: // Return to previous menu
      menu.get()->Return();
      break;

    case Event::Type::gameExit: // Exit game
      gameManager = std::make_unique<GameManager>(GameManager::Preset::title);
			menu.get()->ReloadStack(Menu::Type::main);
      curState = State::mainMenu;
      break;

    case Event::Type::gameDone: // Game has concluded
      menu.get()->ReloadStack(Menu::Type::gameEnd);
      curState = State::mainMenu;
      break;

    case Event::Type::gameReset: // Game is restarted (with the same preset)
			std::cout << "Resetting game...\n";
      gameManager = std::make_unique<GameManager>(gameManager.get()->GetPreset());
			menu.get()->Clear();
      ParticleManager::Clean();
			curState = State::gameplay;
			break;

    case Event::Type::updateSettings:
      Settings::GetInstance()->SetSetting((Settings::Setting)event.data.updateSettings.setting, 
                                          event.data.updateSettings.value);
      break;

    case Event::Type::updateWindow:
      UpdateWindow();
      break;
		
		default: // Game related event
      gameManager.get()->ProcessEvents(event);
			break;
		}
	}

	sf::Event SFMLevent;
	
	while (window.pollEvent(SFMLevent)) 
	{
		switch (SFMLevent.type) 
		{
		case sf::Event::Closed: // Window close (from window)
			std::cout << "Window close button has been clicked, closing program...\n";
			curState = State::notRunning;
      return;

		case sf::Event::KeyPressed: // Key is pressed
      Keyboard::GetInstance()->AddKeyPress(SFMLevent.key.code);
      break;

		default:
			break;
		}
	}
}

void Program::Update() 
{
	if (curState == State::notRunning) 
		return;

  Clock::GetInstance()->Update();

  if (curState != State::paused)
    gameManager.get()->Update();

  menu.get()->Update();

  if (curState != State::paused)
  ParticleManager::GetInstance()->UpdateParticles();

	Keyboard::GetInstance()->Update();
}

void Program::Render()
{
	window.clear({255, 255, 255});

	if (curState == State::notRunning) 
		return;

  // Rendering order: Game -> Particles -> Menu

  gameManager.get()->Render(&window);

  ParticleManager::GetInstance()->RenderParticles(&window);

  menu.get()->Render(&window);

	window.display();	
}

Program::State Program::GetCurState() const 
{
	return curState;
}

void Program::LoadSave(const char* filename)
{
  try
  {
    std::ifstream file(filename);
    nlohmann::json save = nlohmann::json::parse(file);

    Settings::GetInstance()->Load(save["settings"]);
    Stats::Init(save["stats"]);

    file.close();
  }
  catch (...)
  {
    std::cout << "Save file was corrupted or could not be found, creating new one...\n";
    Settings::GetInstance()->Load();
    Stats::Init();
    SaveData(filename);
  }
}

void Program::SaveData(const char* filename)
{
  nlohmann::json save;

  Settings::GetInstance()->Save(save);

  Stats::Save(save);

  std::ofstream file(filename);

  file << save.dump(2);

  file.close();

  std::cout << "Data saved!\n";
}

void Program::UpdateWindow()
{
  sf::Vector2u windowSize;

  windowSize = (unsigned int)SCALE * ASPECT_RATIO;

  Settings::GetInstance()->windowDim = windowSize;

  window.close();

  if (GET_SETTING(Settings::Setting::fullscreen))
    window.create(sf::VideoMode::getFullscreenModes()[0], name, sf::Style::Fullscreen);
  else
    window.create(sf::VideoMode(windowSize.x, windowSize.y), name, sf::Style::Close);

  mainView = window.getDefaultView();

  mainView.setCenter(ZERO_VECTOR);

  window.setView(mainView);

  // Disables holding the key counting as a press as this is handled in `Keyboard`
  window.setKeyRepeatEnabled(false);

  // Hides the mouse as it is not used
  window.setMouseCursorVisible(false);

  // Sets the max framerate of the window to improve performance
  window.setFramerateLimit(Settings::targetFrameRate);

  window.setVerticalSyncEnabled(true);

  ENTITY_SHADER.setUniform("scale", SCALE);
  WORLD_SHADER.setUniform("scale", SCALE);
  RECT_SHADER.setUniform("scale", SCALE);

  STATIC_SHADER.setUniform("screenDim", {sf::Vector2f(windowSize)});
  ENTITY_SHADER.setUniform("screenDim", {sf::Vector2f(windowSize)});
  RECT_SHADER.setUniform("screenDim", {sf::Vector2f(windowSize)});
}

#include "Program.h"

Program::Program(const char* name)
{
  LoadSave(SAVE_FILE);

	std::cout << "--=== Program Init ===--\n"  << "Initialising SFML Window...\n";

		sf::VideoMode desktop = sf::VideoMode::getDesktopMode();	
		sf::Vector2f minWindowSize(160, 90); // Window has a 16:9 aspect ratio
		sf::Vector2f windowSize = minWindowSize;
		int scale = 1;

    // Creates a window that is at least half the size of the desktop
		while (windowSize.x < desktop.width / 2 && windowSize.y < desktop.height / 2)
		{
			windowSize += minWindowSize;
			scale++;
		}

		std::cout << "\tGame scale: " << scale << '\n';
		std::cout << "\tWindow dimensions: " << windowSize.x << "px x " << windowSize.y << "px\n";
		std::cout << "\tDesktop dimensions: " << desktop.width << "px x " << desktop.height << "px\n";

		Settings::GetInstance()->SetSetting(Settings::Setting::scale, scale);
    Settings::GetInstance()->windowDim = windowSize;

		window.create(sf::VideoMode(windowSize.x, windowSize.y), name, sf::Style::Close);

    // Sets the window to be the middle of the desktop
		window.setPosition(sf::Vector2i((desktop.width - window.getSize().x) / 2, (desktop.height - window.getSize().y) / 2));

		window.setKeyRepeatEnabled(false);

		window.setMouseCursorVisible(false);

		mainView = window.getDefaultView();

		mainView.setCenter({0.0f, 0.0f});

		window.setView(mainView);

		window.setVerticalSyncEnabled(true);

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
			menu.get()->ReloadStack(Menu::Type::pause);
      ParticleManager::Clean();
			curState = State::gameplay;
			break;

    case Event::Type::pause: // Pause
      menu.get()->Push(Menu::Type::pause);
      curState = State::paused;
      break;

    case Event::Type::resume: // Resume
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
			menu.get()->ReloadStack(Menu::Type::pause);
      ParticleManager::Clean();
			curState = State::gameplay;
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
			switch (SFMLevent.key.code) 
			{
			case sf::Keyboard::R: // Quick restart (for debugging)
				std::cout << "Restarting Program!\n";
        gameManager = std::make_unique<GameManager>(GameManager::Preset::title);
        menu = std::make_unique<Menu>(Menu::Type::main);
				curState = State::mainMenu;
				break;

      case sf::Keyboard::Escape:
      {
        if (curState == State::gameplay)
        {
          menu.get()->Push(Menu::Type::pause);
          curState = State::paused;
          break;
        }
        [[fallthrough]];
      }
			
			default: // Add key to keymap
				Keyboard::GetInstance()->AddKeyPress(SFMLevent.key.code);
				break;
			}
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

  if (curState != State::gameplay)
    menu.get()->Update();

  ParticleManager::GetInstance()->UpdateParticles();

	Keyboard::GetInstance()->Update();
}

void Program::Render()
{
	window.clear(sf::Color(230, 176, 138));

	if (curState == State::notRunning) 
		return;

  // Rendering order: Game -> Particles -> Menu

  gameManager.get()->Render(&window);

  ParticleManager::GetInstance()->RenderParticles(&window);

  if (curState != State::gameplay)
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
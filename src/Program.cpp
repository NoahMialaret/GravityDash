#include "Program.h"

std::vector<Event> Event::events;

Program::Program(const char* name)
{
  Utility::LoadSave(SAVE_FILE);

	std::cout << "--=== Program Init ===--\n"  << "Initialising SFML Window...\n";

		sf::VideoMode desktop = sf::VideoMode::getDesktopMode();	
		sf::Vector2f minWindowSize(160, 90);
		sf::Vector2f windowSize = minWindowSize;
		int scale = 1;

		while (windowSize.x < desktop.width / 2 && windowSize.y < desktop.height / 2)
		{
			windowSize += minWindowSize;
			scale++;
		}

		std::cout << "\tGame scale: " << scale << '\n';
		std::cout << "\tWindow dimensions: " << windowSize.x << "px x " << windowSize.y << "px\n";
		std::cout << "\tDesktop dimensions: " << desktop.width << "px x " << desktop.height << "px\n";

		ProgramSettings::gameScale = scale;
    ProgramSettings::windowDim = windowSize;

		window.create(sf::VideoMode(windowSize.x, windowSize.y), name, sf::Style::Close);

		window.setPosition(sf::Vector2i((desktop.width - window.getSize().x) / 2, (desktop.height - window.getSize().y) / 2));

		window.setKeyRepeatEnabled(false);

		window.setMouseCursorVisible(false);

		mainView = window.getDefaultView();

		mainView.setCenter({0.0f, 0.0f});

		window.setView(mainView);

		window.setVerticalSyncEnabled(true);

  // Textures and Shaders

    Textures::LoadTextures();

    sf::Image icon;
    if (!icon.loadFromFile("assets/icon.png"))
    {
      std::cout << "Error loading icon!\n";
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    if (!sf::Shader::isAvailable())
    {
      std::cout << "\tShaders are not available on this hardware!\n";
    }

    if (!Utility::entShad.loadFromFile("assets/vert.vs", "assets/frag.fs"))
    {
      std::cout << "ERROR\n";
    }
    Utility::entShad.setUniform("texture", sf::Shader::CurrentTexture);

    if (!Utility::worldShad.loadFromFile("assets/vert.vs", "assets/bg.fs"))
    {
      std::cout << "ERROR\n";
    }
    Utility::worldShad.setUniform("texture", sf::Shader::CurrentTexture);

	std::cout << "Initialising Program objects...\n";

    menu = std::make_unique<Menu>(Event::MenuType::main);
    LoadMenuGame();

    Clock::Init();
    
	std::cout << "Program init done! Starting title sequence...\n";
	
		curState = State::mainMenu;

	std::cout << "--===++++++++++++++===--\n";
}

Program::~Program()
{
	window.close();

  Utility::particles.clear();

	std::cout << "Program successfully cleaned!\n";
}

void Program::HandleEvents() 
{
	Event event;

	while (Event::PollEvent(event))
	{
		switch (event.type)
		{
		case Event::Type::programClose:
			std::cout << "Quit button has been pressed, closing game...\n";
			ProgramExit();
			break;

		case Event::Type::loadNewGame:
    {
			std::cout << "Create new game event called\n";
      switch (event.gameConfig.type)
      {
      case Event::GameConfig::Type::title:
        game = std::make_unique<Game>(event.gameConfig);
        break;
      case Event::GameConfig::Type::min:
        game = std::make_unique<Min>(event.gameConfig);
        break;
      case Event::GameConfig::Type::rush:
        game = std::make_unique<Rush>(event.gameConfig);
        break;
      default:
        std::cout << "Could not determine the game mode!\n";
        // mainMenu = std::make_unique<MainMenu>();
        continue;
      }
			menu.get()->ReloadStack(Event::MenuType::pause);
      Utility::particles.clear();
			curState = State::gameplay;
			break;
    }

    case Event::Type::pause:
      curState = State::paused;
      break;

    case Event::Type::resumePlay:
      curState = State::gameplay;
      break;

    case Event::Type::reloadMenu:
      menu.get()->ReloadStack(event.menuType);
      break;
      
    case Event::Type::pushMenu:
      menu.get()->Push(event.menuType);
      break;
    
    case Event::Type::menuReturn:
      menu.get()->Return();
      break;

    case Event::Type::exitGame:
      LoadMenuGame();
			menu.get()->ReloadStack(Event::MenuType::main);
      curState = State::mainMenu;
      break;

    case Event::Type::gameDone:
      menu.get()->ReloadStack(Event::MenuType::gameEnd);
      curState = State::mainMenu;
      break;

    case Event::Type::restartGame:
    {
			std::cout << "Resetting game...\n";
      Event::GameConfig config = game.get()->GetConfig();
      switch (config.type)
      {
      case Event::GameConfig::Type::title:
        game = std::make_unique<Game>(config);
        break;
      case Event::GameConfig::Type::min:
        game = std::make_unique<Min>(config);
        break;
      case Event::GameConfig::Type::rush:
        game = std::make_unique<Rush>(config);
        break;
      default:
        std::cout << "Could not determine the game mode!\n";
        continue;
      }
			menu.get()->ReloadStack(Event::MenuType::pause);
      Utility::particles.clear();
			curState = State::gameplay;
			break;
    }
		
		default:
			std::cout << "Event type could not be determined.\n";
			break;
		}
	}

	sf::Event SFMLevent;
	
	while (window.pollEvent(SFMLevent)) 
	{
		switch (SFMLevent.type) 
		{
		case sf::Event::Closed:
			std::cout << "Window close event called.\n";
			ProgramExit();
			break;
		
		case sf::Event::LostFocus:
			//Pause();
			break;

		case sf::Event::KeyPressed:
			switch (SFMLevent.key.code) 
			{
      //   if (curState == State::mainMenu)
      //   {
      //     std::cout << "Quit button has been pressed, closing game...\n";
      //     ProgramExit();          
      //   }
			// 	if (curState == State::startMenu)
			// 	{
			// 		// mainMenu.get()->Return();
			// 	}
			// 	// if gameplay, pause
			// 	break;

			// case sf::Keyboard::Tab:
			// 	Utility::FlushDebugSprites();
			// 	break;

			case sf::Keyboard::R:
				std::cout << "Restarting Game!\n";
        LoadMenuGame();
        menu = std::make_unique<Menu>(Event::MenuType::main); // change to title
				curState = State::mainMenu;
				break;
			
			default:
				Utility::AddKeyPress(SFMLevent.key.code);
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

  Clock::Update();

  if (curState != State::paused)
    game.get()->Update();

  if (curState != State::gameplay)
    menu.get()->Update();

  Utility::UpdateParticles();

	Utility::initialKeyPresses.clear();
}

void Program::Render()
{
	window.clear(sf::Color(230, 176, 138));
	//window.clear(sf::Color(255, 229, 181));
  // if (curState != State::titleSequence)

	if (curState == State::notRunning) 
	{
		return;
	}

  game.get()->Render(&window);

  if (curState != State::gameplay)
    menu.get()->Render(&window);

	// switch (curState)
	// {
  // case State::titleSequence:
  //   title.get()->Render(&window);
  //   break;

	// case State::startMenu:
	// 	mainMenu.get()->Render(&window);
	// 	break;

	// case State::gameplay:
	// 	game.get()->Render(&window);
	// 	break;

	// default:
	// 	break;
	// }

	Utility::Render(&window);

	window.display();	
}

Program::State Program::GetCurState() const 
{
	return curState;
}

void Program::ProgramExit() 
{
	std::cout << "Exiting game...\n";

	curState = State::notRunning;
}

void Program::LoadMenuGame()
{
  Event::GameConfig config;
  config.numPlayers = 0;
  config.numComputers = 4;
  config.sawFrequency = 0;
  config.targetSpawnChance = 90;

  game = std::make_unique<Game>(config);
}

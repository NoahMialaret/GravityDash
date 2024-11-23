#include "Program.h"

std::queue<Event> Event::events;

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
    gameManager = std::make_unique<GameManager>(Event::GamePreset::title);

    Clock::Init();
    
	std::cout << "Program init done! Starting title sequence...\n";
	
		curState = State::mainMenu;

	std::cout << "--===++++++++++++++===--\n";
}

Program::~Program()
{
  Utility::SaveData(SAVE_FILE);

	std::cout << "Cleaning program...\n";

	window.close();

  Utility::particles.clear();

  gameManager = nullptr;
  menu = nullptr;

	std::cout << "Program successfully cleaned!\n";
}

void Program::ProcessEvents() 
{
	Event event;

	while (Event::PollEvent(event))
	{
		switch (event.type)
		{
		case Event::Type::programClose:
			std::cout << "Quit button has been pressed, closing game...\n";
			curState = State::notRunning;
			return;

		case Event::Type::gameNew:
    {
			std::cout << "Create new game event called\n";
      gameManager = std::make_unique<GameManager>(event.gamePreset);
			menu.get()->ReloadStack(Event::MenuType::pause);
      Utility::particles.clear();
			curState = State::gameplay;
			break;
    }

    case Event::Type::pause:
      curState = State::paused;
      break;

    case Event::Type::resume:
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

    case Event::Type::gameExit:
      gameManager = std::make_unique<GameManager>(Event::GamePreset::title);
			menu.get()->ReloadStack(Event::MenuType::main);
      curState = State::mainMenu;
      break;

    case Event::Type::gameDone:
      menu.get()->ReloadStack(Event::MenuType::gameEnd);
      curState = State::mainMenu;
      break;

    case Event::Type::gameReset:
    {
			std::cout << "Resetting game...\n";
      gameManager = std::make_unique<GameManager>(gameManager.get()->GetPreset());
			menu.get()->ReloadStack(Event::MenuType::pause);
      Utility::particles.clear();
			curState = State::gameplay;
			break;
    }
		
		default:
      gameManager.get()->ProcessEvents(event);
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
			curState = State::notRunning;
      return;
		
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
        gameManager = std::make_unique<GameManager>(Event::GamePreset::title);
        menu = std::make_unique<Menu>(Event::MenuType::main); // change to title
				curState = State::mainMenu;
				break;
			
			default:
				Keyboard::AddKeyPress(SFMLevent.key.code);
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
    gameManager.get()->Update();

  if (curState != State::gameplay)
    menu.get()->Update();

  Utility::UpdateParticles();

	Keyboard::Update();
}

void Program::Render()
{
	window.clear(sf::Color(230, 176, 138));
	//window.clear(sf::Color(255, 229, 181));
  // if (curState != State::titleSequence)

	if (curState == State::notRunning) 
		return;

  gameManager.get()->Render(&window);

  Utility::RenderParticles(&window);

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
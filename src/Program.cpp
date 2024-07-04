#include "Program.h"

Program::Program(const char* name)
{
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

		Utility::gameScale = scale;

		window.create(sf::VideoMode(windowSize.x, windowSize.y), name, sf::Style::Close);

		window.setPosition(sf::Vector2i((desktop.width - window.getSize().x) / 2, (desktop.height - window.getSize().y) / 2));

		window.setKeyRepeatEnabled(false);

		window.setMouseCursorVisible(false);

		mainView = window.getDefaultView();

		mainView.setCenter({0.0f, 0.0f});

		window.setView(mainView);

		window.setVerticalSyncEnabled(true);

    renderRect = sf::RectangleShape({windowSize.x, windowSize.y});
    renderRect.setPosition(- windowSize.x / 2, - windowSize.y / 2);

	std::cout << "Loading necessary textures..." << std::endl;

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

		if (!Particle::tex.loadFromFile("assets/GravParticles.png")) 
		{
			std::cout << "\tParticle textures could not be loaded!\n";
		}

		if (!Utility::debugTexture.loadFromFile("assets/DebugPos.png")) 
		{
			std::cout << "\tDebug texture could not be loaded!\n";
		}
		if (!Point::tex.loadFromFile("assets/nums.png")) 
		{
			std::cout << "\tPoints texture could not be loaded!\n";
		}
		if (!Utility::programFont.loadFromFile("assets/GravDash.ttf"))
		{
			std::cout << "\tCould not load the game font!\n";
		}

		GridMenu::SetTexture("assets/Buttons.png");
		
		Utility::debugSprite.setTexture(Utility::debugTexture, true);    
		Utility::debugSprite.setOrigin(0.5f * sf::Vector2f(Utility::spriteDim, Utility::spriteDim));
		Utility::debugSprite.setScale(sf::Vector2f(Utility::gameScale, -Utility::gameScale));

	std::cout << "Initialising Program objects...\n";

		mainMenu = std::make_unique<MainMenu>();

	std::cout << "Program initialisation done! Opening main menu...\n";
	
		curState = State::startMenu;

	std::cout << "--===+++++++++++===--\n";
}

Program::~Program()
{
	window.close();

	Particle::DeleteParticles();

	std::cout << "Program successfully cleaned!\n";
}

void Program::HandleEvents() 
{
	// Event should be its own class
	Event event;

	while (Utility::PollEvent(event))
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
      GameConfig config = mainMenu.get()->GetGameConfig();
			mainMenu = nullptr;
			Particle::DeleteParticles();
			game = std::make_unique<Game>(config);
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
			case sf::Keyboard::Escape:
				if (curState == State::startMenu)
				{
					mainMenu.get()->Return();
				}
				// if gameplay, pause
				break;

			case sf::Keyboard::Tab:
				Utility::FlushDebugSprites();
				break;

			case sf::Keyboard::R:
				std::cout << "Restarting Game!\n";
				game = nullptr;
				mainMenu = std::make_unique<MainMenu>();
				curState = State::startMenu;
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
	{
		return;
	}

	// float cameraDistance = std::max(abs(mainView.getCenter().y - targetPos.y), abs(targetPos.y - mainView.getCenter().y));
	// mainView.move(sf::Vector2f(0.0f, - cameraDistance / 10));
	// window.setView(mainView);

	switch (curState)
	{
	case State::startMenu:
		mainMenu.get()->Update();
		break;

	case State::gameplay:
		game.get()->Update();
		break;
	
	default:
		break;
	}

	Particle::UpdateParticles();

	Utility::initialKeyPresses.clear();
}

void Program::Render()
{
	window.clear(sf::Color(230, 176, 138));
	//window.clear(sf::Color(255, 229, 181));
  window.draw(renderRect, &Utility::worldShad);

	if (curState == State::notRunning) 
	{
		return;
	}

	switch (curState)
	{
	case State::startMenu:
		mainMenu.get()->Render(&window);
		break;

	case State::gameplay:
		game.get()->Render(&window);
		Particle::RenderParticles(&window);
		break;
	
	default:
		break;
	}

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
#include "Program.hpp"

Program::Program(const char* name)
{
	std::cout << "--=== Program Init ===--\n"  << "Initialising SFML Window...\n";

		//sf::VideoMode desktop = sf::VideoMode::getDesktopMode();	
    

    stbi_set_flip_vertically_on_load(true);
    // Initialising GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Initialising the GLFW window
    win = glfwCreateWindow(800, 600, "GravDash", NULL, NULL);
    
    if (win == NULL)
    {
      std::cout << "Could not create a GLFW window!\n";
      glfwTerminate();
      return;
    }
    glfwMakeContextCurrent(win);

		iVec2 minWindowSize(160, 90);
		iVec2 windowSize = minWindowSize;
    iVec2 desktopSize;
    glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), NULL, NULL, &desktopSize.x, &desktopSize.y);
		int scale = 1;

		while (windowSize.x < desktopSize.x / 2 && windowSize.y < desktopSize.y / 2)
		{
			windowSize += minWindowSize;
			scale++;
		}

		std::cout << "\tGame scale: " << scale << '\n';
		std::cout << "\tWindow dimensions: " << windowSize.x << "px x " << windowSize.y << "px\n";
		std::cout << "\tDesktop dimensions: " << desktopSize.x << "px x " << desktopSize.y << "px\n";

		Utility::gameScale = scale;

    glfwSetWindowSize(win, windowSize.x, windowSize.y);

    // Initialise GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Could not load GLAD!\n";
      return;
    }

		// window.create(sf::VideoMode(windowSize.x, windowSize.y), name, sf::Style::Close);

		// window.setPosition(iVec2((desktop.width - window.getSize().x) / 2, (desktop.height - window.getSize().y) / 2));

		// window.setKeyRepeatEnabled(false);

		// window.setMouseCursorVisible(false);

		// mainView = window.getDefaultView();

		// mainView.setCenter({0.0f, 0.0f});

		// window.setView(mainView);

		// window.setVerticalSyncEnabled(true);

	std::cout << "Loading necessary textures..." << std::endl;

    // if (!sf::Shader::isAvailable())
    // {
    //   std::cout << "\tShaders are not available on this hardware!\n";
    // }
    // if (!Utility::shaderTest.loadFromFile("assets/test.vs", "assets/test.fs"))
    // {
    //   std::cout << "ERROR\n";
    // }
    // Utility::shaderTest.setUniform("texture", sf::Shader::CurrentTexture);
    Particle::tex.LoadFromFile("assets/GravParticles.png");
    GridMenu::buttonTex.LoadFromFile("assets/Buttons.png");

		Utility::debugTexture.LoadFromFile("assets/DebugPos.png");
		Point::tex.LoadFromFile("assets/nums.png");
		// Utility::programFont.loadFromFile("assets/GravDash.ttf");
		
		// Utility::debugSprite.SetTexture(&Utility::debugTexture);    
		// Utility::debugSprite.SetOrigin(fVec2(Utility::spriteDim, Utility::spriteDim * 0.5f));
		// Utility::debugSprite.SetScale(fVec2(Utility::gameScale, -Utility::gameScale));

	std::cout << "Initialising Program objects...\n";

		mainMenu = std::make_unique<MainMenu>();

	std::cout << "Program initialisation done! Opening main menu...\n";
	
		curState = State::startMenu;

	std::cout << "--===+++++++++++===--\n";
}

Program::~Program()
{
	// window.close();

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

  glfwPollEvents();

	// sf::Event SFMLevent;
	
	// while (window.pollEvent(SFMLevent)) 
	// {
	// 	switch (SFMLevent.type) 
	// 	{
	// 	case sf::Event::Closed:
	// 		std::cout << "Window close event called.\n";
	// 		ProgramExit();
	// 		break;
		
	// 	case sf::Event::LostFocus:
	// 		//Pause();
	// 		break;

	// 	case sf::Event::KeyPressed:
	// 		switch (SFMLevent.key.code) 
	// 		{
	// 		case sf::Keyboard::Escape:
	// 			if (curState == State::startMenu)
	// 			{
	// 				mainMenu.get()->Return();
	// 			}
	// 			// if gameplay, pause
	// 			break;

	// 		case sf::Keyboard::Tab:
	// 			Utility::FlushDebugSprites();
	// 			break;

	// 		case sf::Keyboard::R:
	// 			std::cout << "Restarting Game!\n";
	// 			game = nullptr;
	// 			mainMenu = std::make_unique<MainMenu>();
	// 			curState = State::startMenu;
	// 			break;
			
	// 		default:
	// 			Utility::AddKeyPress(SFMLevent.key.code);
	// 			break;
	// 		}

	// 		break;

	// 	default:
	// 		break;
	// 	}
	// }
}

void Program::Update() 
{
	if (curState == State::notRunning) 
	{
		return;
	}

	// float cameraDistance = std::max(abs(mainView.getCenter().y - targetPos.y), abs(targetPos.y - mainView.getCenter().y));
	// mainView.move(fVec2(0.0f, - cameraDistance / 10));
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
  glClearColor(230.0f / 255.0f, 176.0f / 255.0f, 138.0f / 255.0f, 1.0f);
	//window.clear(sf::Color(255, 229, 181));

	if (curState == State::notRunning) 
	{
		return;
	}

	switch (curState)
	{
	case State::startMenu:
		mainMenu.get()->Render();
		break;

	case State::gameplay:
		game.get()->Render();
		Particle::RenderParticles();
		break;
	
	default:
		break;
	}

	Utility::Render();

  glfwSwapBuffers(win);
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
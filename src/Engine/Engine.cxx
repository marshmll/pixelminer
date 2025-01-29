#include "Engine/Engine.hxx"
#include "stdafx.hxx"

/* PRIVATE METHODS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Engine::seedRandom()
{
    std::srand(std::time(0));
}

void Engine::identificateSelf()
{
    if (std::filesystem::exists("Assets/Settings/uuid.bin"))
    {
        std::ifstream fUuid("Assets/Settings/uuid.bin", std::ios::binary);
        if (!fUuid.is_open())
            logger.logError("Could not identificate self.");

        char uuid[37];
        fUuid.read(uuid, sizeof(uuid));
        myUuid.append(uuid);

        fUuid.close();
    }
    else
    {
        std::ofstream fUuid("Assets/Settings/uuid.bin", std::ios::binary);
        if (!fUuid.is_open())
            logger.logError("Could not identificate self.");

        myUuid = UUID::generate();
        char *uuid = myUuid.data();

        fUuid.write(uuid, sizeof(char) * 37);
        fUuid.close();
    }
}

void Engine::initGraphicsSettings()
{
    if (gfx.loadFromFile("Assets/Settings/graphics.json"))
    {
        vm = sf::VideoMode({gfx.screenWidth, gfx.screenHeight});

        if (!vm.isValid() && gfx.fullscreen)
        {
            logger.logInfo("Invalid resolution for fullscreen. Using windowed mode.");
            gfx.fullscreen = false;
        }

        if (gfx.fullscreen)
            window = sf::RenderWindow(vm, "PixelMiner", sf::State::Fullscreen);
        else
        {
            window = sf::RenderWindow(vm, "PixelMiner", sf::Style::Close | sf::Style::Titlebar);
            window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().size.x / 2 - window.getSize().x / 2,
                                            sf::VideoMode::getDesktopMode().size.y / 2 - window.getSize().y / 2));
        }

        window.setFramerateLimit(gfx.framerateLimit);

        window.setVerticalSyncEnabled(gfx.vsync);
    }
    else
    {
        logger.logInfo("Using default graphics settings.");

        vm = sf::VideoMode::getDesktopMode();
        window = sf::RenderWindow(vm, "PixelMiner", sf::State::Fullscreen);
        window.setFramerateLimit(60);
    }
}

void Engine::initVariables()
{
    dt = 0.f;
    dtClock.restart();

    gridSize = GRID_SIZE;                                                           // 16x16 pixels tile textures.
    scale = static_cast<unsigned int>(std::round((vm.size.x + vm.size.y) / 693.f)); // Dynamic scaling
}

void Engine::initResourcePacks()
{
    resourcePacks[gfx.resourcePack].load(gfx.resourcePack);
    activeResourcePack = std::make_shared<ResourcePack>(resourcePacks.at(gfx.resourcePack));
}

void Engine::initEngineData()
{
    engineData.uuid = myUuid;
    engineData.gridSize = gridSize;
    engineData.scale = scale;
    engineData.states = &states;
    engineData.resourcePacks = &resourcePacks;
    engineData.activeResourcePack = activeResourcePack;
    engineData.window = &window;
    engineData.vm = &vm;
    engineData.event = &event;
    engineData.mouseData = {};
}

void Engine::initMainMenuState()
{
    states.push(std::make_shared<MainMenuState>(engineData));
}

void Engine::pollWindowEvents()
{
    while ((event = window.pollEvent()))
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        else if (event->is<sf::Event::MouseWheelScrolled>())
            engineData.mouseData = *event->getIf<sf::Event::MouseWheelScrolled>();
    }
}

void Engine::updateDeltaTime()
{
    dt = dtClock.restart().asSeconds();
}

void Engine::update()
{
    updateDeltaTime();

    if (!states.empty())
    {
        if (states.top()->isDead())
        {
            states.pop();
        }
        else if (states.top()->wasReplaced())
        {
            std::shared_ptr<State> replacerState = states.top()->getReplacerState();
            states.pop();
            states.push(replacerState);
        }
        else
        {
            states.top()->update(dt);
        }
    }
    if (states.empty())
        window.close();
}

void Engine::render()
{
    window.clear();

    if (!states.empty())
        states.top()->render(window);

    window.display();
}

/* CONSTRUCTOR | DESTRUCTOR +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=+++++++++++++ */

Engine::Engine() : logger("Engine")
{
    seedRandom();
    identificateSelf();
    initGraphicsSettings();
    initVariables();
    initResourcePacks();
    initEngineData();
    initMainMenuState();
}

Engine::~Engine()
{
    while (!states.empty())
        states.pop();
}

/* PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Engine::run()
{
    while (window.isOpen())
    {
        pollWindowEvents();

        update();

        // Only render if the window is active.
        if (window.hasFocus())
            render();
    }
}

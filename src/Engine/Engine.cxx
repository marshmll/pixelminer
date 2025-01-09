#include "Engine/Engine.hxx"
#include "stdafx.hxx"

/* PRIVATE METHODS */

void Engine::initWindow()
{
    vm = sf::VideoMode({864, 486});
    window = sf::RenderWindow(vm, "PixelMiner", sf::Style::Close | sf::Style::Titlebar);
    // vm = sf::VideoMode::getDesktopMode();
    // window = sf::RenderWindow(vm, "PixelMiner", sf::State::Fullscreen);
    window.setFramerateLimit(60);
}

void Engine::initVariables()
{
    dt = 0.f;
    dtClock.restart();

    gridSize = GRID_SIZE;                                                    // 16x16 pixels tile textures.
    scale = static_cast<float>(std::round((vm.size.x + vm.size.y) / 693.f)); // Dynamic scaling
}

void Engine::initTileData()
{
    sf::Vector2i texture_size(GRID_SIZE, GRID_SIZE);

    tileData[Dirt] = {"Dirt", Dirt, sf::IntRect({0, 0}, texture_size)};
    tileData[Stone] = {"Stone", Stone, sf::IntRect({16, 0}, texture_size)};
    tileData[GrassSide] = {"Grass Side", GrassSide, sf::IntRect({32, 0}, texture_size)};
    tileData[Cobblestone] = {"Cobblestone", Cobblestone, sf::IntRect({48, 0}, texture_size)};
    tileData[GrassTop] = {"Grass Top", GrassTop, sf::IntRect({64, 0}, texture_size)};
}

void Engine::initFonts()
{
    if (!fonts["MinecraftRegular"].openFromFile("Assets/Fonts/MinecraftRegular.otf"))
        throw std::runtime_error("[ Engine::initFonts ] -> ERROR: COULD NOT LOAD FONT \"MinecraftRegular\"\n");

    if (!fonts["MinecraftItalic"].openFromFile("Assets/Fonts/MinecraftItalic.otf"))
        throw std::runtime_error("[ Engine::initFonts ] -> ERROR: COULD NOT LOAD FONT \"MinecraftItalic\"\n");

    if (!fonts["MinecraftBold"].openFromFile("Assets/Fonts/MinecraftBold.otf"))
        throw std::runtime_error("[ Engine::initFonts ] -> ERROR: COULD NOT LOAD FONT \"MinecraftBold\"\n");

    if (!fonts["MinecraftBoldItalic"].openFromFile("Assets/Fonts/MinecraftBoldItalic.otf"))
        throw std::runtime_error("[ Engine::initFonts ] -> ERROR: COULD NOT LOAD FONT \"MinecraftBoldItalic\"\n");
}

void Engine::initTextures()
{
    if (!textures["TexturePack"].loadFromFile("Assets/Images/Textures/texture_pack.png"))
        throw std::runtime_error("[ Engine::initTextures ] -> ERROR: COULD NOT LOAD TEXTURE \"TexturePack\"\n");

    if (!textures["Player1"].loadFromFile("Assets/Images/Sprites/Player/player_1.png"))
        throw std::runtime_error("[ Engine::initTextures ] -> ERROR: COULD NOT LOAD TEXTURE \"Player1\"\n");
}

void Engine::initStateData()
{
    stateData.gridSize = gridSize;
    stateData.scale = scale;
    stateData.states = &states;
    stateData.fonts = &fonts;
    stateData.textures = &textures;
    stateData.tileData = &tileData;
    stateData.window = &window;
    stateData.vm = &vm;
}

void Engine::initMainMenuState()
{
    states.push(std::make_unique<MainMenuState>(stateData));
}

void Engine::pollWindowEvents()
{
    while ((event = window.pollEvent()))
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void Engine::updateDeltaTime()
{
    dt = dtClock.restart().asSeconds();
}

/* CONSTRUCTOR / DESTRUCTOR */

Engine::Engine()
{
    initWindow();
    initVariables();
    initTileData();
    initFonts();
    initTextures();
    initStateData();
    initMainMenuState();
}

Engine::~Engine()
{
    while (!states.empty())
        states.pop();
}

/* PUBLIC METHODS */

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

void Engine::update()
{
    updateDeltaTime();

    if (!states.empty())
    {
        if (states.top()->isDead())
            states.pop();
        else
            states.top()->update(dt);
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

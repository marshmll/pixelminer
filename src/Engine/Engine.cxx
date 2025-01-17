#include "Engine/Engine.hxx"
#include "stdafx.hxx"

/* PRIVATE METHODS */

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

    gridSize = GRID_SIZE;                                                    // 16x16 pixels tile textures.
    scale = static_cast<float>(std::round((vm.size.x + vm.size.y) / 693.f)); // Dynamic scaling
}

void Engine::initTileData()
{
    sf::Vector2i texture_size(GRID_SIZE, GRID_SIZE);

    tileData[Unknown] = {"Unknown", Unknown, sf::IntRect({240, 240}, texture_size)};
    tileData[Dirt] = {"Dirt", Dirt, sf::IntRect({0, 0}, texture_size)};
    tileData[Stone] = {"Stone", Stone, sf::IntRect({16, 0}, texture_size)};
    tileData[GrassSide] = {"Grass Side", GrassSide, sf::IntRect({32, 0}, texture_size)};
    tileData[Cobblestone] = {"Cobblestone", Cobblestone, sf::IntRect({48, 0}, texture_size)};
    tileData[GrassTile] = {"Grass", GrassTile, sf::IntRect({0, 16}, texture_size)};
    tileData[Grass1] = {"Grass", Grass1, sf::IntRect({0, 48}, texture_size)};
    tileData[Grass2] = {"Grass", Grass2, sf::IntRect({0, 32}, texture_size)};
    tileData[Sand] = {"Sand", Sand, sf::IntRect({80, 0}, texture_size)};
    tileData[Snow] = {"Snow", Snow, sf::IntRect({96, 0}, texture_size)};
    tileData[Water] = {"Water", Water, sf::IntRect({112, 0}, texture_size)};
    tileData[GrassTopFront] = {"Grass", GrassTopFront, sf::IntRect({48, 16}, texture_size)};
    tileData[GrassBottomFront] = {"Grass", GrassBottomFront, sf::IntRect({48, 48}, texture_size)};
    tileData[GrassBottomBack] = {"Grass", GrassBottomBack, sf::IntRect({16, 64}, texture_size)};
    tileData[GrassCurveTopFront] = {"Grass", GrassCurveTopFront, sf::IntRect({64, 32}, texture_size)};
    tileData[GrassCurveBottomFront] = {"Grass", GrassCurveBottomFront, sf::IntRect({64, 16}, texture_size)};
    tileData[GrassCurveBottomBack] = {"Grass", GrassCurveBottomBack, sf::IntRect({80, 16}, texture_size)};
    tileData[GrassTop] = {"Grass", GrassTop, sf::IntRect({32, 16}, texture_size)};
    tileData[GrassBottom] = {"Grass", GrassBottom, sf::IntRect({32, 48}, texture_size)};
    tileData[GrassFront] = {"Grass", GrassFront, sf::IntRect({48, 32}, texture_size)};
}

void Engine::initFonts()
{
    if (!fonts["MinecraftRegular"].openFromFile("Assets/Fonts/MinecraftRegular.otf"))
        logger.logError("ERROR: COULD NOT LOAD FONT \"MinecraftRegular\".");

    if (!fonts["MinecraftItalic"].openFromFile("Assets/Fonts/MinecraftItalic.otf"))
        logger.logError("ERROR: COULD NOT LOAD FONT \"MinecraftItalic\".");

    if (!fonts["MinecraftBold"].openFromFile("Assets/Fonts/MinecraftBold.otf"))
        logger.logError("ERROR: COULD NOT LOAD FONT \"MinecraftBold\".");

    if (!fonts["MinecraftBoldItalic"].openFromFile("Assets/Fonts/MinecraftBoldItalic.otf"))
        logger.logError("ERROR: COULD NOT LOAD FONT \"MinecraftBoldItalic\".");
}

void Engine::initTextures()
{
    if (!textures["Stone"].loadFromFile("Assets/Images/Backgrounds/stone.png"))
        logger.logError("ERROR: COULD NOT LOAD TEXTURE \"Stone\".;");

    if (!textures["TexturePack"].loadFromFile("Assets/Images/Textures/texture_pack.png"))
        logger.logError("ERROR: COULD NOT LOAD TEXTURE \"TexturePack\".");

    if (!textures["Player1"].loadFromFile("Assets/Images/Sprites/Player/player_1.png"))
        logger.logError("ERROR: COULD NOT LOAD TEXTURE \"Player1\".");
}

void Engine::initShaders()
{
    if (!shaders["GaussianBlur"].loadFromFile("Assets/Shaders/Gaussian Blur/fragment_shader.frag",
                                              sf::Shader::Type::Fragment))
        logger.logError("ERROR: COULD NOT LOAD SHADER \"GaussianBlur\".");
}

void Engine::initEngineData()
{
    engineData.uuid = myUuid;
    engineData.gridSize = gridSize;
    engineData.scale = scale;
    engineData.states = &states;
    engineData.fonts = &fonts;
    engineData.textures = &textures;
    engineData.shaders = &shaders;
    engineData.tileData = &tileData;
    engineData.window = &window;
    engineData.vm = &vm;
    engineData.event = &event;
}

void Engine::initMainMenuState()
{
    states.push(std::make_unique<MainMenuState>(engineData));
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

Engine::Engine() : logger("Engine")
{
    seedRandom();
    identificateSelf();
    initGraphicsSettings();
    initVariables();
    initTileData();
    initFonts();
    initTextures();
    initShaders();
    initEngineData();
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

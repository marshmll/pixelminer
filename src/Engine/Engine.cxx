#include "Engine/Engine.hxx"
#include "stdafx.hxx"

/* PRIVATE METHODS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void Engine::seedRandom()
{
    std::srand(std::time(0));
}

void Engine::initLocales()
{
    try
    {
        JObject locales_obj = JSON::parse(std::filesystem::path(SETTINGS_FOLDER + "locales.json")).getAs<JObject>();
        currentLocale = locales_obj.at("current").getAs<std::string>();

        setup_i18n(currentLocale);

        // Debugging output
        logger.logInfo(_("Current locale: ") + setlocale(LC_MESSAGES, nullptr));
        logger.logInfo(_("Text domain: ") + textdomain(nullptr));
        logger.logInfo(_("Locale path: ") + bindtextdomain(DOMAIN, nullptr));
    }
    catch (std::runtime_error &e)
    {
        logger.logWarning(_("Failed to load locales from file \"locales.json\".") + e.what() + "\n" +
                          _("Translations might not work correctly."));
    }
}

void Engine::verifyGlobalFolder()
{
    if (std::filesystem::exists(GLOBAL_FOLDER))
    {
        logger.logInfo(_("Found global folder: ") + GLOBAL_FOLDER);
    }
    else
    {
        logger.logInfo(_("Creating global folder: ") + GLOBAL_FOLDER);
        std::filesystem::create_directory(GLOBAL_FOLDER);

        logger.logInfo(_("Creating settings folder: ") + SETTINGS_FOLDER);
        std::filesystem::create_directory(SETTINGS_FOLDER);

        logger.logInfo(_("Copying locales.json to: ") + SETTINGS_FOLDER);
        std::filesystem::copy(".pixelminer/Default/locales.json", SETTINGS_FOLDER);

        logger.logInfo(_("Creating maps folder: ") + MAPS_FOLDER);
        std::filesystem::create_directory(MAPS_FOLDER);

        logger.logInfo(_("Creating resource packs folder: ") + RESOURCES_FOLDER);
        std::filesystem::create_directory(RESOURCES_FOLDER);

        logger.logInfo(_("Copying Vanilla resource pack to: ") + RESOURCES_FOLDER);
        std::filesystem::copy(".pixelminer/Default/Vanilla.zip", RESOURCES_FOLDER);

        logger.logInfo(_("Creating cache folder: ") + CACHE_FOLDER);
        std::filesystem::create_directory(CACHE_FOLDER);

        logger.logInfo(_("Creating Locales folder: ") + LOCALES_FOLDER);
        std::filesystem::create_directory(LOCALES_FOLDER);
    }
}

void Engine::identificateSelf()
{
    if (std::filesystem::exists(SETTINGS_FOLDER + "uuid.bin"))
    {
        std::ifstream fUuid(SETTINGS_FOLDER + "uuid.bin", std::ios::binary);
        if (!fUuid.is_open())
            logger.logError(_("Could not identificate self."));

        char uuid[37];
        ASSERT_ALWAYS((fUuid.read(uuid, sizeof(uuid))), "Invalid UUID read.");
        myUuid.append(uuid);

        fUuid.close();
    }
    else
    {
        std::ofstream fUuid(SETTINGS_FOLDER + "uuid.bin", std::ios::binary);
        if (!fUuid.is_open())
            logger.logError(_("Could not identificate self."));

        myUuid = UUID::generate();
        char *uuid = myUuid.data();

        fUuid.write(uuid, sizeof(char) * 37);
        fUuid.close();
    }
}

void Engine::initGraphicsSettings()
{
    if (gfx.loadFromFile(SETTINGS_FOLDER + "graphics.json"))
    {
        vm = sf::VideoMode(sf::Vector2u(gfx.screenWidth, gfx.screenHeight));

        if (!vm.isValid() && gfx.fullscreen)
        {
            logger.logWarning(_("Invalid resolution for fullscreen. Using windowed mode."));
            gfx.fullscreen = false;
        }

        if (gfx.fullscreen)
            window =
                sf::RenderWindow(vm, "PixelMiner " + static_cast<std::string>(GAME_VERSION), sf::State::Fullscreen);
        else
        {
            window = sf::RenderWindow(vm, "PixelMiner " + static_cast<std::string>(GAME_VERSION),
                                      sf::Style::Close | sf::Style::Titlebar);
            window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().size.x / 2 - window.getSize().x / 2,
                                            sf::VideoMode::getDesktopMode().size.y / 2 - window.getSize().y / 2));
        }

        window.setVerticalSyncEnabled(gfx.vsync);
        window.setFramerateLimit(gfx.framerateLimit);
    }
    else
    {
        logger.logInfo(_("Using default graphics settings."));

        vm = sf::VideoMode::getDesktopMode();
        window = sf::RenderWindow(vm, "PixelMiner " + static_cast<std::string>(GAME_VERSION), sf::State::Fullscreen);
        window.setVerticalSyncEnabled(false);
        window.setFramerateLimit(60);

        gfx.fullscreen = true;
        gfx.screenWidth = window.getSize().x;
        gfx.screenHeight = window.getSize().y;
        gfx.framerateLimit = 60;
        gfx.resourcePack = "Vanilla";
    }

    sf::Image icon;

    if (!icon.loadFromFile((GLOBAL_FOLDER + "Default/icon_5x.png")))
        logger.logWarning(_("Failed to load window icon. Using OS default icon instead."));

    else
        window.setIcon(icon);
}

void Engine::initVariables()
{
    dt = 0.f;
    dtClock.restart();

    gridSize = GRID_SIZE; // 16x16 pixels tile textures.
    scale = std::max(1u, static_cast<unsigned int>(std::roundf((vm.size.x + vm.size.y) / 693.f)));
}

void Engine::initResourcePacks()
{
    try
    {
        resourcePacks[gfx.resourcePack].load(gfx.resourcePack);
        activeResourcePack = std::make_shared<ResourcePack>(resourcePacks.at(gfx.resourcePack));
        activeResourcePack->setFontSmoothness(gfx.fontSmoothness);
        activeResourcePack->setTextureSmoothness(gfx.textureSmoothness);
    }
    catch (std::exception &e)
    {
        logger.logError(_("An error occured while loading resource pack ") + gfx.resourcePack + ": " + e.what());
    }
}

void Engine::initEngineData()
{
    engineData.currentLocale = &currentLocale;
    engineData.uuid = myUuid;
    engineData.gridSize = gridSize;
    engineData.scale = &scale;
    engineData.states = &states;
    engineData.resourcePacks = &resourcePacks;
    engineData.activeResourcePack = activeResourcePack;
    engineData.window = &window;
    engineData.vm = &vm;
    engineData.event = std::nullopt;
    engineData.mouseData = std::nullopt;
    engineData.gfx = &gfx;
}

void Engine::initMainMenuState()
{
    states.push(std::make_shared<MainMenuState>(engineData));
}

void Engine::pollWindowEvents()
{
    engineData.mouseData = std::nullopt;

    while (const auto event = window.pollEvent())
    {
        engineData.event = event;

        if (event->is<sf::Event::Closed>())
        {
            window.close();
            return;
        }

        else if (event->is<sf::Event::MouseWheelScrolled>())
        {
            auto *mouse_data = event->getIf<sf::Event::MouseWheelScrolled>();
            engineData.mouseData = *mouse_data;
        }
    }
}

void Engine::updateDeltaTime()
{
    dt = dtClock.restart().asSeconds();
    std::max(dt, .25f); // Prevent lag spikes and spiral of death
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
            std::shared_ptr<State> replacerState = std::move(states.top()->getReplacerState());
            states.pop();
            states.push(replacerState);
        }
        else if (states.top()->askedToRestartAllStates())
        {
            while (!states.empty())
                states.pop();

            initMainMenuState();
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
    initLocales();
    verifyGlobalFolder();
    identificateSelf();
    initGraphicsSettings();
    initVariables();
    initResourcePacks();
    initEngineData();
    initMainMenuState();
}

Engine::~Engine() = default;

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

    while (!states.empty())
        states.pop();
}

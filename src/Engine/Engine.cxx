#include "Engine/Engine.hxx"
#include "stdafx.hxx"

/* PRIVATE METHODS */

void Engine::initWindow()
{
    // vm = sf::VideoMode({864, 486});
    // window = sf::RenderWindow(vm, "PixelMiner", sf::Style::Close | sf::Style::Titlebar);
    vm = sf::VideoMode::getDesktopMode();
    window = sf::RenderWindow(vm, "PixelMiner", sf::State::Fullscreen);
    window.setFramerateLimit(60);
}

void Engine::initVariables()
{
    dt = 0.f;
    dtClock.restart();
    gridSize = 16; // 16x16 pixels tile textures.
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

void Engine::initStateData()
{
    stateData.gridSize = &gridSize;
    stateData.states = &states;
    stateData.fonts = &fonts;
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
    initFonts();
    initStateData();
    initMainMenuState();

    PerlinNoise noise;

    // Define waves for height, moisture, and heat maps
    std::vector<Wave> height_waves = {{120.f, .005f, 4.f}, {300.f, .002f, 1.5f}, {500.f, .01f, 5.f}};
    std::vector<Wave> moisture_waves = {{622.f, .04f, 5.f}, {200.f, .1f, 1.f}, {400.f, .02f, 1.2f}};
    std::vector<Wave> heat_waves = {{318.6f, .1f, 10.f}, {329.7f, .02f, .5f}};

    // Generate noise maps with adjusted scale for height
    NoiseMap height_map = noise.generateNoiseMap(500, 500, .1f, height_waves, {0.f, 0.f});
    NoiseMap moisture_map = noise.generateNoiseMap(500, 500, .3f, moisture_waves, {10.f, 10.f});
    NoiseMap heat_map = noise.generateNoiseMap(500, 500, .2f, heat_waves, {5.f, 5.f});

    // Define biomes with refined parameters
    std::vector<Biome> biomes = {
        {"Desert", .2f, 0.1f, .8f}, {"Forest", .4f, .6f, .4f},    {"Grassland", .3f, .5f, .5f},
        {"Jungle", .4f, .8f, .7f},  {"Mountains", .9f, .3f, .3f}, {"Ocean", .15f, .8f, .6f},
        {"Tundra", .8f, .3f, .1f},
    };
    sf::Image image({500, 500});

    // Smooth biome transitions
    for (unsigned int y = 0; y < 500; y++)
    {
        for (unsigned int x = 0; x < 500; x++)
        {
            float height = height_map[x][y];
            float moisture = moisture_map[x][y];
            float heat = heat_map[x][y];

            // Weighted selection of biomes
            float max_weight = -1.f;
            std::string biome_name = "";
            sf::Color biome_color;

            for (const auto &biome : biomes)
            {
                float weight = biome.calculateWeight(height, moisture, heat);
                if (weight > max_weight)
                {
                    max_weight = weight;
                    biome_name = biome.getName();
                }
            }

            if (biome_name == "Desert")
                biome_color = sf::Color(196, 178, 94, 255);
            else if (biome_name == "Forest")
                biome_color = sf::Color(29, 110, 29, 255);
            else if (biome_name == "Grassland")
                biome_color = sf::Color(22, 148, 22, 255);
            else if (biome_name == "Jungle")
                biome_color = sf::Color(2, 31, 8, 255);
            else if (biome_name == "Mountains")
                biome_color = sf::Color(91, 99, 93, 255);
            else if (biome_name == "Ocean")
                biome_color = sf::Color(16, 51, 163);
            else if (biome_name == "Tundra")
                biome_color = sf::Color(216, 242, 230, 255);

            image.setPixel({x, y}, biome_color);
        }
    }

    (void)image.saveToFile("Assets/map.png");
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

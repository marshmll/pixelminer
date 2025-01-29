#include "Map/PerlinNoise.hxx"
#include "stdafx.hxx"

float PerlinNoise::fade(float t)
{
    // Fade function: 6t^5 - 15t^4 + 10t^3
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise::lerp(float a, float b, float t)
{
    // Linear interpolation
    return a + t * (b - a);
}

float PerlinNoise::grad(int hash, float x, float y)
{
    // Calculate gradient based on hash value
    int h = hash & 3; // Take the last 2 bits of the hash
    float u = h < 2 ? x : y;
    float v = h < 2 ? y : x;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise::noise(float x, float y)
{
    // Find the unit grid cell containing the point
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;

    // Find relative x, y within the cell
    x -= std::floor(x);
    y -= std::floor(y);

    // Compute fade curves for x, y
    float u = fade(x);
    float v = fade(y);

    // Hash coordinates of the four cell corners
    int aa = permutation[permutation[X] + Y];
    int ab = permutation[permutation[X] + Y + 1];
    int ba = permutation[permutation[X + 1] + Y];
    int bb = permutation[permutation[X + 1] + Y + 1];

    // Add blended results from the corners
    float result =
        lerp(lerp(grad(aa, x, y), grad(ba, x - 1, y), u), lerp(grad(ab, x, y - 1), grad(bb, x - 1, y - 1), u), v);

    return (result + 1.0f) / 2.0f; // Normalize to [0, 1]
}

PerlinNoise::PerlinNoise(const unsigned int &seed)
{
    // Initialize the random generator with the given seed
    Random rng(seed);

    // Initialize permutation vector with values 0-255
    permutation.fill(0);
    for (int i = 0; i < PERMUTATION_SIZE; i++)
        permutation[i] = i;

    // Shuffle the permutation vector
    for (int i = PERMUTATION_SIZE - 1; i > 0; i--)
    {
        int j = rng.nextInt(0, i);
        std::swap(permutation[i], permutation[j]);
    }

    // Duplicate the permutation vector
    for (int i = 0; i < PERMUTATION_SIZE; i++)
        permutation[i + PERMUTATION_SIZE] = permutation[i];
}

PerlinNoise::~PerlinNoise() = default;

const NoiseMap PerlinNoise::generateNoiseMap(const unsigned int &width, const unsigned int &height, const float &scale,
                                             const std::vector<Wave> &waves, const sf::Vector2f &offset)
{
    NoiseMap noise_map(width, std::vector<float>(height, 0.0f));

    // Precompute wave contributions in parallel
    std::vector<std::vector<float>> wave_contributions(waves.size(), std::vector<float>(width * height, 0.0f));

    const int numThreads = std::thread::hardware_concurrency();
    const int chunkSize = height / numThreads;

    std::vector<std::future<void>> futures;
    for (int t = 0; t < numThreads; ++t)
    {
        futures.push_back(std::async(std::launch::async, [this, t, chunkSize, width, height, scale, &waves, &offset,
                                                          &wave_contributions, &numThreads]() {
            const int startY = t * chunkSize;
            const int endY = (t == numThreads - 1) ? height : (t + 1) * chunkSize;

            for (size_t i = 0; i < waves.size(); ++i)
            {
                const auto &wave = waves[i];
                for (int y = startY; y < endY; ++y)
                {
                    for (int x = 0; x < width; ++x)
                    {
                        float sample_pos_x = static_cast<float>(x) * scale + offset.x;
                        float sample_pos_y = static_cast<float>(y) * scale + offset.y;
                        wave_contributions[i][y * width + x] =
                            wave.amplitude *
                            noise(sample_pos_x * wave.frequency + wave.seed, sample_pos_y * wave.frequency + wave.seed);
                    }
                }
            }
        }));
    }

    for (auto &f : futures)
    {
        f.wait();
    }

    // Combine the precomputed contributions into the final noise map
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float normalization = 0.0f;

            for (size_t i = 0; i < waves.size(); ++i)
            {
                noise_map[x][y] += wave_contributions[i][y * width + x];
                normalization += waves[i].amplitude;
            }

            if (normalization == 0.0f)
                normalization = 1.0f; // Prevent division by zero

            noise_map[x][y] /= normalization;
        }
    }

    return noise_map;
}
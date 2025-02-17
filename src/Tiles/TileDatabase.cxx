#include "stdafx.hxx"
#include "Tiles/TileDatabase.hxx"

TileDatabase::TileDatabase()
{
    insert("unknown", "Unknown", 0, 0, GRID_SIZE);
}

TileDatabase::~TileDatabase() = default;

void TileDatabase::insert(const std::string &tag, const std::string &name, const int &rect_index_x,
                          const int &rect_index_y, const int &size_in_pixels)
{
    db[tag] = TileData{
        tag, hash_function(tag), name,
        sf::IntRect({rect_index_x * size_in_pixels, rect_index_y * size_in_pixels}, {size_in_pixels, size_in_pixels}),
        size_in_pixels};
}

const TileData TileDatabase::getByTag(const std::string &tag)
{
    try
    {
        return db.at(tag);
    }
    catch (std::out_of_range &)
    {}

    return db.at("unknown");
}

const TileData TileDatabase::getById(const uint64_t &id)
{
    auto it = std::find_if(db.begin(), db.end(), [&](auto &pair) { return pair.second.id == id; });

    if (it != db.end())
        return it->second;

    return db.at("unknown");
}

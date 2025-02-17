#pragma once

#include "Engine/Configuration.hxx"
#include "Tiles/TileData.hxx"

class TileDatabase
{
  private:
    std::hash<std::string> hash_function;

    std::unordered_map<std::string, TileData> db;

  public:
    TileDatabase();

    ~TileDatabase();

    void insert(const std::string &tag, const std::string &name, const int &rect_index_x, const int &rect_index_y,
                const int &size_in_pixels);

    const TileData getByTag(const std::string &tag);

    const TileData getById(const uint64_t &id);
};

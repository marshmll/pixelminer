#pragma once

#include "Tools/Logger.hxx"
#include "compat/unzip.h"
#include "compat/zip.h"

static constexpr size_t BUFFER_SIZE = 2048;

class Zip
{
  public:
    static const bool extract(const std::filesystem::path &src, const std::filesystem::path &dst);
    static const bool compress(const std::filesystem::path &src, const std::filesystem::path &dst);

  private:
    static const bool addFileToZip(zipFile zf, const std::filesystem::path &file_path,
                                   const std::filesystem::path &zip_entry_name);

    static const bool addFolderToZip(zipFile zf, const std::filesystem::path &folder_path,
                                     const std::filesystem::path &base_folder = "");
};

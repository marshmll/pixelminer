/**
 * @file Zip.hxx
 * @brief Declares the Zip class to manage Zip Archives extraction and compression.
 */

#pragma once

#include "Tools/Logger.hxx"
#include "compat/unzip.h"
#include "compat/zip.h"

static constexpr size_t BUFFER_SIZE = 2048;

/**
 * @class Zip
 * @brief A utility class for compressing and extracting ZIP archives.
 *
 * This class provides static methods for extracting and compressing files and directories
 * to and from ZIP archives. It uses the `unzip` and `zip` methods from the `minizip-ng` library
 * to handle the underlying ZIP file operations.
 */
class Zip
{
  public:
    /**
     * @brief Extracts a ZIP archive to a specified destination directory.
     *
     * This method opens a ZIP file for reading and extracts its contents to the specified
     * destination directory. If the destination directory does not exist, it will be created.
     *
     * @param src The path to the source ZIP file.
     * @param dst The path to the destination directory.
     *
     * @return `true` if extraction was successful, `false` otherwise.
     */
    static const bool extract(const std::filesystem::path &src, const std::filesystem::path &dst);

    /**
     * @brief Compresses a file or directory into a ZIP archive.
     *
     * This method compresses the specified file or directory into a ZIP archive at the
     * specified destination path.
     *
     * @param src The path to the file or directory to be compressed.
     * @param dst The path to the destination ZIP file.
     *
     * @return `true` if compression was successful, `false` otherwise.
     */
    static const bool compress(const std::filesystem::path &src, const std::filesystem::path &dst);

  private:
    /**
     * @brief Adds a file to the ZIP archive.
     *
     * This helper method is used internally to add a single file to the ZIP archive.
     *
     * @param zf The `zipFile` handle for the archive.
     * @param file_path The path to the file to be added.
     * @param zip_entry_name The name of the file in the ZIP archive.
     *
     * @return `true` if the file was successfully added, `false` otherwise.
     */
    static const bool
    addFileToZip(zipFile zf, const std::filesystem::path &file_path, const std::filesystem::path &zip_entry_name);

    /**
     * @brief Adds a folder and its contents to the ZIP archive.
     *
     * This helper method is used internally to recursively add a folder and all of its
     * contents to the ZIP archive.
     *
     * @param zf The `zipFile` handle for the archive.
     * @param folder_path The path to the folder to be added.
     * @param base_folder The base folder name to be used inside the ZIP archive.
     *
     * @return `true` if the folder and its contents were successfully added, `false` otherwise.
     */
    static const bool
    addFolderToZip(zipFile zf, const std::filesystem::path &folder_path, const std::filesystem::path &base_folder = "");
};

/**
 * @file File.hxx
 * @brief Provides file-related utility functions and structures for handling file transfers.
 *
 * This file defines the `FileDescriptor` structure for encapsulating file information and provides functions
 * for file validation and descriptor creation. The file also includes overloaded operators for serializing
 * and deserializing the `FileDescriptor` structure to and from SFML packets for communication over a network.
 */

#pragma once

#include <SFML/Network.hpp>
#include <filesystem>
#include <string>

namespace File
{
/**
 * @struct FileDescriptor
 * @brief Represents metadata of a file for file transfer purposes.
 *
 * This structure holds the necessary information for transferring a file in parts over the network.
 * It includes the file's name, size, transfer mode, and information about its partitioning into multiple parts.
 */
struct FileDescriptor
{
    /**
     * @brief The filename of the file being transferred.
     */
    std::string filename;

    /**
     * @brief The size of the file in bytes.
     */
    std::uintmax_t filesize;

    /**
     * @brief The mode in which the file is being transferred (e.g., binary or text mode).
     */
    int mode;

    /**
     * @brief The current part of the file being transferred.
     * @note This is used when the file is split into multiple parts.
     */
    unsigned short part;

    /**
     * @brief The total number of parts the file has been divided into for transfer.
     */
    unsigned short total_parts;
};

/**
 * @brief Creates a `FileDescriptor` for the given file.
 * @param path The path to the file to be transferred.
 * @param mode The mode in which the file will be transferred (e.g., binary mode or text mode).
 * @return A `FileDescriptor` containing information about the file, such as its name, size, and transfer details.
 */
FileDescriptor createFileDescriptor(std::filesystem::path path, std::ios::openmode mode);

/**
 * @brief Validates if the given file path exists.
 * @param path The path to be validated.
 * @return True if the file exists, false otherwise.
 */
const bool validatePath(const std::filesystem::path &path);
}; // namespace File

/**
 * @brief Serializes a `FileDescriptor` object into an SFML packet.
 * @param packet The packet to which the `FileDescriptor` is being added.
 * @param f_desc The `FileDescriptor` object to be serialized.
 * @return The updated packet with the `FileDescriptor` data.
 */
sf::Packet &operator<<(sf::Packet &packet, const File::FileDescriptor &f_desc);

/**
 * @brief Deserializes a `FileDescriptor` object from an SFML packet.
 * @param packet The packet from which the `FileDescriptor` will be read.
 * @param f_desc The `FileDescriptor` object to be populated with the packet's data.
 * @return The updated packet with the extracted `FileDescriptor` data.
 */
sf::Packet &operator>>(sf::Packet &packet, File::FileDescriptor &f_desc);

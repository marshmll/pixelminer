#pragma once

namespace File
{
struct FileDescriptor
{
    std::string filename;
    std::uintmax_t filesize;
    int mode;
    unsigned short part;        // Represent which part of the file is being sent
    unsigned short total_parts; // Represents how many parts the file was cutted
};
FileDescriptor createFileDescriptor(std::filesystem::path path, std::ios::openmode mode);

const bool validatePath(const std::filesystem::path &path);
}; // namespace File

sf::Packet &operator<<(sf::Packet &packet, const File::FileDescriptor &f_desc);

sf::Packet &operator>>(sf::Packet &packet, File::FileDescriptor &f_desc);
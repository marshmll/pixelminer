#pragma once

struct FileDescriptor
{
    std::string filename;
    std::uintmax_t filesize;
    int mode;
    unsigned short part;        // Represent which part of the file is being sent
    unsigned short total_parts; // Represents how many parts the file was cutted
};

sf::Packet &operator<<(sf::Packet &packet, const FileDescriptor &f_desc);

sf::Packet &operator>>(sf::Packet &packet, FileDescriptor &f_desc);
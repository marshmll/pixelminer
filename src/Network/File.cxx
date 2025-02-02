#include "Network/File.hxx"
#include "stdafx.hxx"

File::FileDescriptor File::createFileDescriptor(std::filesystem::path path, std::ios::openmode mode)
{
    File::FileDescriptor fd;
    fd.filename = path.filename().string();
    fd.filesize = std::filesystem::file_size(path);
    fd.mode = static_cast<int>(mode);
    fd.part = 1;
    fd.total_parts = static_cast<unsigned short>(
        std::ceil(static_cast<float>(fd.filesize) /
                  static_cast<float>(sf::UdpSocket::MaxDatagramSize - sizeof(fd) - sizeof("FILE"))));

    return fd;
}

const bool File::validatePath(const std::filesystem::path &path)
{
    return std::filesystem::exists(path);
}

sf::Packet &operator<<(sf::Packet &packet, const File::FileDescriptor &f_desc)
{
    return packet << f_desc.filename << f_desc.filesize << f_desc.mode << f_desc.part << f_desc.total_parts;
}

sf::Packet &operator>>(sf::Packet &packet, File::FileDescriptor &f_desc)
{
    return packet >> f_desc.filename >> f_desc.filesize >> f_desc.mode >> f_desc.part >> f_desc.total_parts;
}
#include "Network/File.hxx"
#include "stdafx.hxx"

sf::Packet &operator<<(sf::Packet &packet, const FileDescriptor &f_desc)
{
    return packet << f_desc.filename << f_desc.filesize << f_desc.mode << f_desc.part << f_desc.total_parts;
}

sf::Packet &operator>>(sf::Packet &packet, FileDescriptor &f_desc)
{
    return packet >> f_desc.filename >> f_desc.filesize >> f_desc.mode >> f_desc.part >> f_desc.total_parts;
}
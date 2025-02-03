#include "Tools/Zip.hxx"
#include "stdafx.hxx"

/* PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

const bool Zip::extract(const std::filesystem::path &src, const std::filesystem::path &dst)
{
    unzFile zip_file = unzOpen(src.c_str());

    if (!zip_file)
    {
        std::cerr << "[ Zip ] -> Failed to open ZIP archive for extraction: " + src.string() << "\n";
        return false;
    }

    if (unzGoToFirstFile(zip_file) != UNZ_OK)
    {
        std::cerr << "[ Zip ] -> Failed to locate first file of ZIP archive: " + src.string() << "\n";
        unzClose(zip_file);
        return false;
    }

    if (!std::filesystem::exists(dst))
    {
        if (!std::filesystem::create_directories(dst))
        {
            std::cout << "[ Zip ] -> Failed to create output directory \"" << dst.string() << "\"" << "\n";
            unzClose(zip_file);
            return false;
        }

        std::cout << "[ Zip ] -> Created output directory \"" << dst.string() << "\"" << "\n";
    }

    int files_extracted = 0;
    int total_size;

    do
    {
        char filename[256];
        unz_file_info file_info;

        if (unzGetCurrentFileInfo(zip_file, &file_info, filename, sizeof(filename), NULL, 0, NULL, 0) != UNZ_OK)
        {
            std::cerr << "[ Zip ] -> Failed to get file info from ZIP archive: " + src.string() << "\n";
            unzClose(zip_file);
            return false;
        }

        std::filesystem::path full_path = dst / filename;

        if (unzOpenCurrentFile(zip_file) != UNZ_OK)
        {
            std::cerr << "[ Zip ] -> Failed to open file inside ZIP archive: " + src.string() << "\n";
            continue;
        }

        // Assume only directories ends with "/" or "\"
        if (full_path.string().back() == '/' || full_path.string().back() == '\\')
        {
            if (!std::filesystem::create_directory(full_path))
            {
                std::cout << "[ Zip ] -> Failed to create directory \"" << full_path.string() << "\"" << "\n";
                unzClose(zip_file);
                return false;
            }

            std::cout << "[ Zip ] -> Created directory \"" << full_path.string() << "\"" << "\n";
            continue;
        }

        std::ofstream out(full_path, std::ios::binary);

        if (!out.is_open())
        {
            std::cerr << "[ Zip ] -> Failed to create output file: " + full_path.string() << "\n";
            unzClose(zip_file);
            continue;
        }

        char buffer[BUFFER_SIZE];
        int bytes_read = 0;
        int bytes_written = 0;

        while ((bytes_read = unzReadCurrentFile(zip_file, buffer, sizeof(buffer))) > 0)
        {
            out.write(buffer, bytes_read);
            bytes_written += bytes_read;
        }

        std::cout << "[ Zip ] -> Extracted file \"" << full_path.string() << "\" (" << bytes_written << " bytes)"
                  << "\n";

        files_extracted++;
        total_size += bytes_written;

        out.close();
        unzCloseCurrentFile(zip_file);

    } while (unzGoToNextFile(zip_file) == UNZ_OK);

    std::cout << "[ Zip ] -> Successfully extracted " << files_extracted << " files (" << total_size
              << " bytes) from ZIP archive: \"" << src.string() << "\n";

    return true;
}

const bool Zip::compress(const std::filesystem::path &src, const std::filesystem::path &dst)
{
    zipFile zf = zipOpen(dst.c_str(), APPEND_STATUS_CREATE);
    if (!zf)
    {
        std::cerr << "[ Zip ] -> Failed to create ZIP file: " << dst << std::endl;
        return false;
    }

    bool result;
    if (std::filesystem::is_directory(src))
    {
        // Only include the last folder name as the base in ZIP
        std::filesystem::path baseFolder = src.filename();
        result = addFolderToZip(zf, src, baseFolder);
    }
    else
    {
        result = addFileToZip(zf, src, src.filename().string());
    }

    zipClose(zf, nullptr);

    if (result)
        std::cout << "[ Zip ] -> Successfully compressed \"" << src.string() << "\" to ZIP archive: \"" << dst.string()
                  << "\"" << "\n";

    return result;
}

/* PRIVATE METHODS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// Helper function to add a file to the ZIP archive
const bool Zip::addFileToZip(zipFile zf, const std::filesystem::path &file_path,
                             const std::filesystem::path &zip_entry_name)
{
    std::ifstream inFile(file_path, std::ios::binary);

    if (!inFile)
    {
        std::cerr << "[ Zip ] -> Failed to open file: " << file_path.string() << std::endl;
        return false;
    }

    zip_fileinfo zfi = {};
    if (zipOpenNewFileInZip(zf, zip_entry_name.c_str(), &zfi, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED,
                            Z_DEFAULT_COMPRESSION) != ZIP_OK)
    {
        std::cerr << "[ Zip ] -> Failed to add file to ZIP: " << zip_entry_name.string() << std::endl;
        return false;
    }

    char buffer[BUFFER_SIZE];
    while (inFile.read(buffer, sizeof(buffer)) || inFile.gcount() > 0)
    {
        if (zipWriteInFileInZip(zf, buffer, inFile.gcount()) < 0)
        {
            std::cerr << "[ Zip ] -> Failed to write data to ZIP file: " << zip_entry_name.string() << std::endl;
            zipCloseFileInZip(zf);
            return false;
        }
    }

    zipCloseFileInZip(zf);

    std::cout << "[ Zip ] -> Added file: " << zip_entry_name.string() << "\n";

    return true;
}

const bool Zip::addFolderToZip(zipFile zf, const std::filesystem::path &folder_path,
                               const std::filesystem::path &base_folder)
{
    for (const auto &entry : std::filesystem::recursive_directory_iterator(folder_path))
    {
        // Strip everything before base_folder
        std::filesystem::path relative_path = std::filesystem::relative(entry.path(), folder_path);
        relative_path = base_folder / relative_path;

        if (std::filesystem::is_directory(entry.path()))
            continue;

        if (!addFileToZip(zf, entry.path().string(), relative_path.string()))
            return false;
    }

    return true;
}

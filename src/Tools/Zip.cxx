#include "Tools/Zip.hxx"
#include "stdafx.hxx"

/* PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

const bool Zip::extract(const std::filesystem::path &src, const std::filesystem::path &dst, const bool &verbose)
{
    Logger logger("Zip");
    unzFile zip_file = unzOpen(src.c_str());

    if (!zip_file)
    {
        logger.logError(_("Failed to open ZIP archive for extraction: ") + src.string(), false);
        return false;
    }

    if (unzGoToFirstFile(zip_file) != UNZ_OK)
    {
        logger.logError(_("Failed to locate first file of ZIP archive: ") + src.string(), false);
        unzClose(zip_file);
        return false;
    }

    if (!std::filesystem::exists(dst))
    {
        if (!std::filesystem::create_directories(dst))
        {
            logger.logError(_("Failed to create output directory \"") + dst.string() + "\"", false);
            unzClose(zip_file);
            return false;
        }

        if (verbose)
            logger.logInfo(_("Created output directory \"") + dst.string() + "\"");
    }

    unsigned int files_extracted = 0;
    unsigned long long int total_size;

    do
    {
        char filename[256];
        unz_file_info file_info;

        if (unzGetCurrentFileInfo(zip_file, &file_info, filename, sizeof(filename), NULL, 0, NULL, 0) != UNZ_OK)
        {
            logger.logError(_("Failed to get file info from ZIP archive: ") + src.string(), false);
            unzClose(zip_file);
            return false;
        }

        std::filesystem::path full_path = dst / filename;

        if (unzOpenCurrentFile(zip_file) != UNZ_OK)
        {
            logger.logError(_("Failed to open file inside ZIP archive: ") + src.string(), false);
            continue;
        }

        // Assume only directories ends with "/" or "\"
        if (full_path.string().back() == '/' || full_path.string().back() == '\\')
        {
            if (!std::filesystem::create_directory(full_path))
            {
                logger.logError(_("Failed to create directory \"") + full_path.string() + "\"", false);
                unzClose(zip_file);
                return false;
            }

            if (verbose)
                logger.logInfo(_("Created directory \"") + full_path.string() + "\"");

            continue;
        }

        std::ofstream out(full_path, std::ios::binary);

        if (!out.is_open())
        {
            logger.logError(_("Failed to create output file: ") + full_path.string(), false);
            unzClose(zip_file);
            continue;
        }

        char buffer[ZIP_BUFFER_SIZE];
        int bytes_read = 0;
        int bytes_written = 0;

        while ((bytes_read = unzReadCurrentFile(zip_file, buffer, sizeof(buffer))) > 0)
        {
            out.write(buffer, bytes_read);
            bytes_written += bytes_read;
        }

        if (verbose)
            logger.logInfo(_("Extracted file \"") + full_path.string() + "\" (" + std::to_string(bytes_written) +
                           " B)");

        files_extracted++;
        total_size += bytes_written;

        out.close();
        unzCloseCurrentFile(zip_file);

    } while (unzGoToNextFile(zip_file) == UNZ_OK);

    logger.logInfo(_("Successfully extracted ") + std::to_string(files_extracted) + _(" files (") +
                   std::to_string(total_size / 1024) + _(" KB) from ZIP archive: \"") + src.string());

    return true;
}

const bool Zip::compress(const std::filesystem::path &src, const std::filesystem::path &dst, const bool &verbose)
{
    Logger logger("Zip");

    zipFile zf = zipOpen(dst.c_str(), APPEND_STATUS_CREATE);
    if (!zf)
    {
        logger.logError(_("Failed to create ZIP file: ") + dst.string(), false);
        return false;
    }

    bool result;
    if (std::filesystem::is_directory(src))
    {
        // Only include the last folder name as the base in ZIP
        std::filesystem::path baseFolder = src.filename();
        result = addFolderToZip(zf, src, baseFolder, verbose);
    }
    else
    {
        result = addFileToZip(zf, src, src.filename().string(), verbose);
    }

    zipClose(zf, nullptr);

    if (result)
        logger.logInfo(_("Successfully compressed \"") + src.string() + _("\" to ZIP archive: \"") + dst.string() +
                       "\"");

    return result;
}

/* PRIVATE METHODS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// Helper function to add a file to the ZIP archive
const bool Zip::addFileToZip(zipFile zf, const std::filesystem::path &file_path,
                             const std::filesystem::path &zip_entry_name, const bool &verbose)
{
    Logger logger("Zip");

    std::ifstream inFile(file_path, std::ios::binary);

    if (!inFile)
    {
        logger.logError(_("Failed to open file: ") + file_path.string(), false);
        return false;
    }

    zip_fileinfo zfi = {};
    if (zipOpenNewFileInZip(zf, zip_entry_name.c_str(), &zfi, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED,
                            Z_DEFAULT_COMPRESSION) != ZIP_OK)
    {
        logger.logError(_("Failed to add file to ZIP: ") + zip_entry_name.string(), false);
        return false;
    }

    char buffer[ZIP_BUFFER_SIZE];
    while (inFile.read(buffer, sizeof(buffer)) || inFile.gcount() > 0)
    {
        if (zipWriteInFileInZip(zf, buffer, inFile.gcount()) < 0)
        {
            logger.logError(_("Failed to write data to ZIP file: ") + zip_entry_name.string(), false);
            zipCloseFileInZip(zf);
            return false;
        }
    }

    zipCloseFileInZip(zf);

    if (verbose)
        logger.logInfo(_("Added file: ") + zip_entry_name.string());

    return true;
}

const bool Zip::addFolderToZip(zipFile zf, const std::filesystem::path &folder_path,
                               const std::filesystem::path &base_folder, const bool &verbose)
{
    for (const auto &entry : std::filesystem::recursive_directory_iterator(folder_path))
    {
        // Strip everything before base_folder
        std::filesystem::path relative_path = std::filesystem::relative(entry.path(), folder_path);
        relative_path = base_folder / relative_path;

        if (std::filesystem::is_directory(entry.path()))
            continue;

        if (!addFileToZip(zf, entry.path().string(), relative_path.string(), verbose))
            return false;
    }

    return true;
}

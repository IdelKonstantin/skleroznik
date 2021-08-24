//
// Created by olobach on 14.01.2020.
//

#include <iostream>
#include <CMiscUtil.h>
#include <sys/stat.h>
#include <experimental/filesystem>
#include <dirent.h>

bool CMiscUtil::checkIfDirExists(const std::string& sPath)
{
    struct stat info;

    if(::stat( sPath.c_str(), &info ) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

// Извлекает имя дочерней папки> из полного пути sPath
std::string  CMiscUtil::extractChildFolderNameAsString(const std::string& sPath)
{
    std::string sLocalPath(sPath);
    removeTrailingSlashFromPath(sLocalPath);
    size_t nSlashPos = sLocalPath.rfind('/');
    if(nSlashPos == std::string::npos)
        return std::string("");

    return std::string(sLocalPath.substr(++nSlashPos, sLocalPath.length()-1));
}

void CMiscUtil::removeTrailingSlashFromPath(std::string & sPath)
{
    while(sPath[sPath.length()-1] == '/')
    {
        sPath = sPath.substr(0, sPath.length() - 1);
    }
}

bool CMiscUtil::createDirectory(const std::string& sPath)
{
    std::error_code errCode;
    //if (!std::experimental::filesystem::exists(sPath, errCode))
    {
        try
        {
            std::experimental::filesystem::create_directories(sPath);
        }
        catch (std::experimental::filesystem::filesystem_error err)
        {
            std::cout << "Ошибка в функции CMiscUtil::createDirectory - "<< err.what() << std::endl;
            return false;
        }
        return true;
    }
}

bool CMiscUtil::createDirectoryIfNotExist(const std::string& sPath)
{
    //std::error_code errCode;
    if (!checkIfDirExists(sPath))
    {
        try
        {
            std::experimental::filesystem::create_directories(sPath);
        }
        catch (std::experimental::filesystem::filesystem_error err)
        {
            std::cout << "Ошибка в функции CMiscUtil::createDirectoryIfNotExist - "<< err.what() << std::endl;
            return false;
        }

    }
    return true;
}

static void addTrailingSlashToPath(std::string& sPath)
{
    if (!sPath.empty() && sPath.back() != '/')
        sPath.append("/");
}

static void removeTrailingSlashFromPath(std::string & sPath)
{
    while(sPath[sPath.length()-1] == '/')
    {
        sPath = sPath.substr(0, sPath.length() - 1);
    }
}

static size_t getTotalCountFilesInDirectory(const std::string& sPath)
{
    struct dirent *pDirEntry;
    DIR* pDirObject = opendir(sPath.c_str());
    if(!pDirObject)
    {
        std::cout << "getTotalCountFilesInDirectory: Не получается открыть заданную директорию: " << "[" << sPath << std::endl;
    }
    size_t nCount = 0;
    while(pDirEntry = readdir(pDirObject))
    {
        ++nCount;
    }
    closedir(pDirObject);

    return nCount;
}

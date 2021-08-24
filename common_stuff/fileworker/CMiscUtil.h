#ifndef BASEDAEMONTEMPLATE_CMISCUTIL_H
#define BASEDAEMONTEMPLATE_CMISCUTIL_H
#include <string>

class CMiscUtil {
public:
    static bool checkIfDirExists(const std::string &sPath);

    static bool createDirectory(const std::string &sPath);

    static bool createDirectoryIfNotExist(const std::string &sPath);


    static void addTrailingSlashToPath(std::string &sPath);

    static size_t getTotalCountFilesInDirectory(const std::string &sPath);

    // Извлекает имя дочерней папки> из полного пути sPath
    static  std::string extractChildFolderNameAsString(const std::string &sPath);

    // Удаляет финальный слэш из переданного файлового пути sPath
    static void removeTrailingSlashFromPath(std::string &sPath);
};
#endif //BASEDAEMONTEMPLATE_CMISCUTIL_H

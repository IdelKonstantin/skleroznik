#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>

const char* weekDays[] {

    "Вс",
    "Пн",
    "Вт",
    "Ср",
    "Чт",
    "Пт",
    "Сб"
};

const char* months[] {

    "янв",
    "фев",
    "мар",
    "апр",
    "май",
    "июн",
    "июл",
    "авг",
    "сен",
    "окт",
    "ноя",
    "дек",
};


int main() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir("./");
    if (!dir) {
        perror("diropen");
        return 1;
    };

    std::vector<std::string> vec{};

    while((entry = readdir(dir)) != NULL) {

        vec.push_back(std::string(entry->d_name));
    };

    closedir(dir);

    std::sort(vec.begin(), vec.end(), [](const std::string& lhs, const std::string& rhs)
    { 
        return lhs <= rhs;
    });


    for(const auto& i : vec) {

        struct stat fs;
        stat(i.c_str(), &fs);

        struct tm* timeData = localtime(&fs.st_atim.tv_sec);

        /* Структура для pwd */
        struct passwd* userData = getpwuid(fs.st_uid);//https://www.mkssoftware.com/docs/man5/struct_passwd.5.asp + p.232 /etc/passwd

        /* Структура для group-ID */
        struct group * groupData = getgrgid(fs.st_gid);//p.235 /etc/group

        std::cout << std::setw (5) << fs.st_mode << " " << std::setw(2) << fs.st_nlink << " " 
        << std::setw(6) << userData->pw_name << " " << std::setw(6) << groupData->gr_name << " " << std::setw(8) 
        << fs.st_size << " " << months[timeData->tm_mon] << " " 
        << timeData->tm_mday << " " << timeData->tm_hour << ":" 
        << timeData->tm_min << " " << i << std::endl;
    }

    return 0;
};
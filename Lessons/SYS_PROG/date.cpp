#include <iostream>
#include <time.h>

/* см. стр. 246 */

int main()
{
    time_t timeSinceEpoch = time(nullptr);

    struct tm* timeData = localtime(&timeSinceEpoch);

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

    std::cout << weekDays[timeData->tm_wday] << " "
    << months[timeData->tm_mon] << " "
    << timeData->tm_mday << " "
    << timeData->tm_hour << ":" 
    << timeData->tm_min << ":" 
    << timeData->tm_sec << " " 
    << (1900 + timeData->tm_year) << std::endl;
}
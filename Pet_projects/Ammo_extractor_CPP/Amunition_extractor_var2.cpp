#include <iostream>
#include <error_codes.h>
#include <ammo_extractor_saver.h>

int main (int argc, char** argv) {

    if(argc < 2) {

        std::cerr << "Не указан файл для парсинга..." << std::endl;
        return errCode::NO_JSON;
    }

    auto errCode = ammoExtractor{std::string(argv[1])}.parseAndStore();

    if(errCode != errCode::DONE) {

        std::cerr << "Обнаружена ошибка №" << errCode << std::endl;
        return -1;
    }

    std::cout << "Обработка завершена" << std::endl;
    return 0;
}
#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"

#include <iostream>
#include <string>
#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{
    std::string keyDirPath{"../../client/Key"};
    auto keysFilePath{keyDirPath + std::string("/key.data")};

    if (fs::exists(fs::path{keysFilePath})) {

        std::cerr << "Файл с ключами уже существует по пути: [" 
        << keysFilePath << "]" << std::endl;
        return -1;
    }

    std::cout << "Эта утилита генерирует ключи шифрования для чат-клиента по пути: [" 
    << keysFilePath << "]" << std::endl; 

    using namespace CryptoPP;

    AutoSeededRandomPool prng;
    HexEncoder encoder(new FileSink(std::cout));

    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    SecByteBlock iv(AES::BLOCKSIZE);

    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());

    try {
    
        if (!fs::exists(keyDirPath)) {
            fs::create_directories(keyDirPath);
        }
    }
    catch (const std::exception& ex) {

        std::cerr << "Ошибка создания папки [" << keyDirPath 
        <<  "]. Причина: " << ex.what() << std::endl;
        return -1;
    }

    std::ofstream fout;
    fout.open(keysFilePath);
    
    if(!fout.is_open()) {

        std::cerr << "Ошибка открытия файла [" 
        << keysFilePath << "] на запись" << std::endl;
        return -1;
    }

    fout.write((char*)&key, sizeof(SecByteBlock));
    fout.write((char*)&iv, sizeof(SecByteBlock));
    fout.close();

    std::cout << "Ключи сгенерированы: " << std::endl;
    std::cout << "key -> ";
    encoder.Put(key, key.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "iv  -> ";
    encoder.Put(iv, iv.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    return 0;
}

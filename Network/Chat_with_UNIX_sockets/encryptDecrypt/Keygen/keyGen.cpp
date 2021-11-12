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
    using namespace CryptoPP;

    AutoSeededRandomPool prng;
    HexEncoder encoder(new FileSink(std::cout));

    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    SecByteBlock iv(AES::BLOCKSIZE);

    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());

    std::string keyDirPath{"./Key"};

    if (!fs::exists(keyDirPath)) {
        
        fs::create_directories(keyDirPath);
    }

    std::ofstream fout;

    auto iniFilePath{keyDirPath + std::string("/key.ini")};

    fout.open(iniFilePath);
    
    if(!fout.is_open()) {

        std::cerr << "Ошибка открытия файла [" 
        << iniFilePath << "] на запись" << std::endl;
        return -1;
    }

    fout.write((char*)&key, sizeof(SecByteBlock));
    fout.write((char*)&iv, sizeof(SecByteBlock));
    fout.close();

    std::cout << "Keys generated in [" << iniFilePath <<  "]:" << std::endl;
    std::cout << "key:\t";
    encoder.Put(key, key.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "iv:\t";
    encoder.Put(iv, iv.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    // std::ifstream fin;
    // fin.open(iniFilePath);

    // if(!fin.is_open()) {

    //     std::cerr << "Ошибка открытия файл [" 
    //     << iniFilePath << "]  на чтение" << std::endl;
    //     return -1;
    // }

    // SecByteBlock key_(AES::DEFAULT_KEYLENGTH);
    // SecByteBlock iv_(AES::BLOCKSIZE);

    // fin.read((char*)&key_, sizeof(SecByteBlock));
    // fin.read((char*)&iv_, sizeof(SecByteBlock));

    // fin.close();

    // std::cout << "After: " << std::endl;
    // std::cout << "key_: ";
    // encoder.Put(key_, key_.size());
    // encoder.MessageEnd();
    // std::cout << std::endl;

    // std::cout << "iv_: ";
    // encoder.Put(iv_, iv_.size());
    // encoder.MessageEnd();
    // std::cout << std::endl;


    return 0;
}

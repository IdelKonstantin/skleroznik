#ifndef UUIDMANAGER_CUUIDGENERATOR_H
#define UUIDMANAGER_CUUIDGENERATOR_H

#include <string>
#include <chrono>
#include <openssl/md5.h>

using time_stamp_microseconds = std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>;

const size_t DEFAULT_RANDOM_STRING_LEN = 2 * MD5_DIGEST_LENGTH;
const size_t DEFAULT_UUID_STRING_LEN = 2 * MD5_DIGEST_LENGTH;

namespace uuid {
    
    const int SIZE_OF_SYSTEM_UUID = 36;
    namespace generator_type{
        
        const uint64_t SYSTEM_GENERATOR = 1;
        const uint64_t RANDOM_GENERATOR = 2;
        const uint64_t TIME_GENERATOR = 8;
        const uint64_t COMPLETE_GENERATOR = SYSTEM_GENERATOR | RANDOM_GENERATOR | TIME_GENERATOR;
    }

    class CUUIDGenerator {
    public:

        static std::string getNewUUID(uint64_t uuidType = generator_type::COMPLETE_GENERATOR, size_t nRequiredLen = DEFAULT_UUID_STRING_LEN);
        static std::string getRandomString(size_t nLen = DEFAULT_RANDOM_STRING_LEN);
        static std::string getRandomStringFast(size_t nLen = DEFAULT_RANDOM_STRING_LEN);
        static std::string getCurrentTimeString();

    private:
        
        static const std::string& getTemplateString();
        static std::string getMD5(const std::string &strSource);
        static uint32_t getBoundedRandValue(uint32_t range);
    };
}

#ifdef UUID_LIB_DEBUG
#define COUT std::cout
#else
#define COUT if(0) std::cout
#endif

#endif //UUIDMANAGER_CUUIDGENERATOR_H


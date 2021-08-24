#pragma once 

#include <cstdint>

using ammo_error_t = uint8_t;

namespace errCode {

    const ammo_error_t DONE = 0;
    const ammo_error_t NO_JSON = 1;
    const ammo_error_t CANT_READ_JSON = 2;
    const ammo_error_t CANT_PROCESS_JSON = 3;
    const ammo_error_t CANT_PARSE_JSON = 4;
    const ammo_error_t NO_SQLITE_DB = 5;
    const ammo_error_t CANT_DROP_TABLE = 6;
    const ammo_error_t CANT_CREATE_TABLE = 7;
    const ammo_error_t CANT_INSERT_INFO = 8;
};

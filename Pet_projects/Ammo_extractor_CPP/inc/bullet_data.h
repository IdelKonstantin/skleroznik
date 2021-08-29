#pragma once

#include <cstdint>
#include <string>

namespace ammo {

    struct bulletData {

        uint64_t ItemID{};
        std::string ItemGUID{};
        std::string ItemModifiedWhen{};
        std::string Vendor{};
        std::string Caliber{};
        double GrammWeight{};
        double BallisticCoefficient{};
        double BC_G7{};    
        uint16_t MuzzleSpeed{};
        uint16_t GrainWeight{};
        std::string BulletShape{};
    };    
}

#include "modloader/shared/modloader.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

struct Config_t
{
    bool enabled;
    bool overrideLifetime;
    bool enablePCDebris;
    bool enableGravity;
    bool freezeRotations;
    bool monochromeDebris;
    float debrisLifetime;
    float debrisScale;
    float velocityMultiplier;
    float drag;
};

bool LoadConfig();
void SetupConfig();
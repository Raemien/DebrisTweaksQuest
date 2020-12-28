#include "modloader/shared/modloader.hpp"
#include "main.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

struct Config_t
{
    bool enabled = false;
    bool overrideLifetime = true;
    bool enableGravity = true;
    bool FreezeRotations = true;
    bool Monochrome = false;
    float Drag = 0;
    float debrisLifetime = 2;
    float debrisScale = 1;
    float velocityMultiplier = 1;
}
Settings;

void SetupConfig()
{
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    auto& allocator = getConfig().config.GetAllocator();
    getConfig().config.AddMember("enabled", Settings.enabled, allocator);
    getConfig().config.AddMember("overrideLifetime", Settings.overrideLifetime, allocator);
    getConfig().config.AddMember("enableGravity", Settings.enableGravity, allocator);
    getConfig().config.AddMember("debrisLifetime", Settings.debrisLifetime, allocator);
    getConfig().config.AddMember("debrisScale", Settings.debrisScale, allocator);
    getConfig().config.AddMember("velocityMultiplier", Settings.velocityMultiplier, allocator);
    getConfig().config.AddMember("FreezeRotations", Settings.FreezeRotations, allocator);
    getConfig().config.AddMember("Drag", Settings.Drag, allocator);
    getConfig().config.AddMember("MonochromeDebris", Settings.Monochrome, allocator);
    getConfig().Write();
}

bool LoadConfig()
{
    getConfig().Load();
    if(!getConfig().config.HasMember("enabled") || !getConfig().config["enabled"].IsBool()) return false;
    if(!getConfig().config.HasMember("overrideLifetime") || !getConfig().config["overrideLifetime"].IsBool()) return false;
    if(!getConfig().config.HasMember("enableGravity") || !getConfig().config["enableGravity"].IsBool()) return false;
    if(!getConfig().config.HasMember("debrisLifetime") || !getConfig().config["debrisLifetime"].IsFloat()) return false;
    if(!getConfig().config.HasMember("debrisScale") || !getConfig().config["debrisScale"].IsFloat()) return false;
    if(!getConfig().config.HasMember("velocityMultiplier") || !getConfig().config["velocityMultiplier"].IsFloat()) return false;
    if(!getConfig().config.HasMember("FreezeRotations") || !getConfig().config["FreezeRotations"].IsBool()) return false;
    if(!getConfig().config.HasMember("MonochromeDebris") || !getConfig().config["MonochromeDebris"].IsBool()) return false;
    if(!getConfig().config.HasMember("Drag") || !getConfig().config["Drag"].IsFloat()) return false;
    return true;
}
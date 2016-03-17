#pragma once

#include "BaseNPC.h"

#include "UtilitySystems\UtilityScore.h"
#include "UtilitySystems\UtilityValue.h"

#include <map>
#include <string>

class World;

class UtilityNPC : public BaseNPC
{
public:
    UtilityNPC(World* a_pWorld);

    void selectAction(float a_fDeltaTime);

private:
    UtilitySystem::UtilityValue m_waterValue;
    std::map<std::string, UtilitySystem::UtilityScore*> m_utilityScoreMap;
};

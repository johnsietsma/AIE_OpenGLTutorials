#include "UtilityNPC.h"

using namespace UtilitySystem;

UtilityNPC::UtilityNPC(World* a_pWorld) :
    BaseNPC(a_pWorld)
{
    m_waterValue.setNormalizationType(UtilityValue::INVERSE_LINEAR);
    m_waterValue.setMinMaxValues(0, 20);
    m_waterValue.setValue((float)getWaterValue());

    UtilityScore* pWaterScore = new UtilityScore();
    pWaterScore->addUtilityValue(&m_waterValue, 1.0f);
    m_utilityScoreMap["collectWater"] = pWaterScore;
}

void UtilityNPC::selectAction(float a_fDeltaTime)
{
    m_waterValue.setValue((float)getWaterValue());

    float fBestScore = 0.0f;
    std::string strBestAction;

    for (auto score : m_utilityScoreMap)
    {

        float fThisScore = score.second->getUtilityScore();
        if (fThisScore > fBestScore)
        {
            fBestScore = fThisScore;
            strBestAction = score.first;
        }
    }

    if (strBestAction == "collectWater")
    {
        collectWater(a_fDeltaTime);
    }


}
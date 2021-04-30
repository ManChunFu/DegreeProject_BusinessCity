#include "SceneViewData.h"
#include "SceneLocationData.h"

USING_NS_CC;
SceneViewData::SceneViewData()
{
    m_SceneLocations[0] = new SceneLocationData(0, Vec2(440.f, 280.f), Vec2::ZERO, Vec2(440.f, 320.f), Vec2(440.f, 370.f));
    m_SceneLocations[1] = new SceneLocationData(1, Vec2(240.f, 290.f), Vec2(310.f, 420.f));
    m_SceneLocations[2] = new SceneLocationData(2, Vec2(440.f, 300.f), Vec2(500.f, 380.f));
}

SceneViewData::~SceneViewData()
{
    for (auto data : m_SceneLocations)
    {
        delete data.second;
    }
    m_SceneLocations.clear();
}

#pragma once
#include "model_render.h"

class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();
    bool Start();
    void Update();


private:
    ModelRender* m_modelRender1 = { nullptr };
    ModelRender* m_modelRender2 = { nullptr };
    ModelRender* m_modelRender3 = { nullptr };
    ModelRender* m_modelRender4 = { nullptr };
};


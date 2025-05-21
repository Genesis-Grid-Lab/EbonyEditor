#pragma once

#include <UrbanEngine.h>

using namespace UE;

class EditorLayer : public Layer{
public:
    EditorLayer();
    virtual void OnAttach() override;
    virtual void OnUpdate(Timestep ts) override;
    // virtual void OnEvent(Event& e) override;
    virtual void OnImGuiRender() override;

private:
    Ref<Scene> m_ActiveScene;
    Ref<Scene> m_EditorScene;
    ImVec2 m_ViewportBounds[2];
    ImVec2 m_ViewportSize;
};
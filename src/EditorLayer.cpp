#include "EditorLayer.h"

EditorLayer::EditorLayer(){

}

void EditorLayer::OnAttach(){
    m_Console.AddLog("Starting");
    m_ActiveScene = CreateRef<Scene>();

    auto& floor = m_ActiveScene->CreateEntity("floor");
    auto& floorPlane = floor.AddComponent<PlaneComponent>();
    floorPlane.Size = {32.0f, 32.0f};
    floorPlane.color = BLUE;
}

void EditorLayer::OnUpdate(Timestep ts){
    if(m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && 
        (m_ActiveScene->m_Buffer->m_Width != m_ViewportSize.x || m_ActiveScene->m_Buffer->m_Height != m_ViewportSize.y)){

            m_ActiveScene->m_Buffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
    }
    m_ActiveScene->OnUpdateEditor(ts);
}

void EditorLayer::OnImGuiRender(){
    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    // float minWinSizeX = style.WindowMinSize.x;
    // style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    // style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows, 
            // which we can't undo at the moment without finer window depth/z control.
            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                // NewScene();
                m_Console.AddLog("Open New Scene");
            }

            if (ImGui::MenuItem("Open...", "Ctrl+O"))
            {
                // OpenScene();
                m_Console.AddLog("Open Scene");
            }

            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
            {
                // SaveScene();
                m_Console.AddLog("Save Scene");
            }                

            if (ImGui::MenuItem("Exit")){
                m_Console.AddLog("Good bye");
                Application::Get().Close();
            }

            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("View")){

            if(ImGui::MenuItem("Show Boxes"))
            {
                // if(m_ActiveScene->ShowBoxes){
                //     m_ActiveScene->ShowBoxes = false;
                //     console.AddLog("Show Boxes false");
                // }
                // else{
                //     m_ActiveScene->ShowBoxes = true;
                //     console.AddLog("Show Boxes true");
                // }
            }

            if(ImGui::MenuItem("Show Cams")){
                // if(m_ActiveScene->ShowCams){
                //     m_ActiveScene->ShowCams = false;
                //     console.AddLog("Show Cam false");
                // }
                // else{
                //     m_ActiveScene->ShowCams = true;
                //     console.AddLog("Show Cam true");
                // }
            }

            if(ImGui::MenuItem("Show Boxes Play")){
                // if(m_ActiveScene->ShowBoxesPlay){
                //     m_ActiveScene->ShowBoxesPlay = false;
                //     console.AddLog("Show Boxes play false");
                // }
                // else{
                //     m_ActiveScene->ShowBoxesPlay = true;
                //     console.AddLog("Show Boxes play true");
                // }
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    m_SceneHierarchyPanel.OnImGuiRender();
    m_ContentBrowserPanel.OnImGuiRender();
    bool p_open = true;
    m_Console.Draw("Console", &p_open);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");

    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();
    m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
    m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    ImTextureID Frame = (ImTextureID)&m_ActiveScene->m_Buffer->GetTexture().texture;
    ImGui::Image(Frame, ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}
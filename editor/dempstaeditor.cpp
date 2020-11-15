#include "dempstaeditor.h"

#include "Core/assetsmanager.h"
#include "Scene/sceneserializer.h"

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
constexpr const char* SCENE_FILES_FILTER{"Dempsta Scene (*.dempsta)\0*.dempsta\0"};
#elif defined(__linux) || defined(__linux__) || defined(linux)
constexpr const char* SCENE_FILES_FILTER{"Dempsta Scene (*.dempsta) | *.dempsta"};
#else
constexpr const char* SCENE_FILES_FILTER{""};
#endif

namespace de {
DempstaEditor::DempstaEditor() : Layer("DempstaEditor") {}

void DempstaEditor::OnAttach() {
    FrameBufferConfig fConfig = {1280, 720};
    m_frameBuffer = FrameBuffer::Create(fConfig);
    m_activeScene = CreateRef<Scene>();
    m_sceneHierarchyPanel.SetContext(m_activeScene);

    /*auto spriteSheet = CreateRef<Atlas2D>("assets/textures/RPGpack_sheet_2X.png", glm::vec2(128.0f));
    auto spriteBarrel = SubTexture2D::CreateSprite(spriteSheet, glm::vec2({8.f, 12.f}));*/

    /*AssetsManager::GetInstance()
        .AddSprite("sphere_tex", "assets/textures/sphere.png")
        .AddSprite("mask_tex", "assets/textures/mask.png")
        .AddFont("arial_fnt", "assets/fonts/arial.ttf", 60)
        .AddSound("protector_sound", "assets/sound/iamtheprotectorofthissystem.wav")
        .AddSound("bg_music_sound", "assets/sound/bg_music.wav")
        .AddScript("bg_music_script", "assets/scripts/bg_music.lua")
        .AddScript("camera_script", "assets/scripts/camera.lua")
        .AddScript("mask_script", "assets/scripts/mask.lua")
        .AddScript("protector_script", "assets/scripts/protector.lua")
        .AddScript("sphere_script", "assets/scripts/sphere.lua");

    auto camera = m_activeScene->CreateEntity("Primary Camera");
    camera.AddComponent<CameraComponent>().primary = true;
    camera.AddComponent<ScriptComponent>("camera_script");
    auto bgMusic = m_activeScene->CreateEntity("Background Music", false);
    bgMusic.AddComponent<SoundComponent>("bg_music_sound").sound = AssetsManager::GetInstance().GetSoundInstance("bg_music_sound");
    bgMusic.AddComponent<ScriptComponent>("bg_music_script");
    auto protector = m_activeScene->CreateEntity("Protector", false);
    protector.AddComponent<SoundComponent>("protector_sound").sound = AssetsManager::GetInstance().GetSoundInstance("protector_sound");
    protector.AddComponent<ScriptComponent>("protector_script");
    auto sphere = m_activeScene->CreateEntity("Sphere");
    sphere.AddComponent<SpriteComponent>("sphere_tex").anchor = {0.5f, 0.5f};
    sphere.AddComponent<ScriptComponent>("sphere_script");
    for (size_t i = 0; i < 5; ++i) {
        auto mask = m_activeScene->CreateEntity("Mask_" + std::to_string(i));
        mask.AddComponent<SpriteComponent>("mask_tex").anchor = {0.5f, 0.5f};
        mask.AddComponent<ScriptComponent>("mask_script");
    }
    auto text = m_activeScene->CreateEntity("Text");
    text.AddComponent<LabelComponent>("arial_fnt").label = CreateRef<Label>(AssetsManager::GetInstance().GetFont("arial_fnt"), "Marco");*/

    AssetsManager::GetInstance()
        .AddAtlas("RPGpack", "assets/textures/RPGpack_sheet_2X.png", glm::vec2(128.0f))
        .AddScript("sphere_script", "assets/scripts/sphere.lua");
    auto camera = m_activeScene->CreateEntity("Primary Camera");
    camera.AddComponent<CameraComponent>().primary = true;
    auto barrel = m_activeScene->CreateEntity("Barrel");
    auto& sc = barrel.AddComponent<SpriteComponent>("RPGpack");
    sc.sprite = AssetsManager::GetInstance().CreateSprite("RPGpack", {8.f, 11.f});
    sc.anchor = {0.5f, 0.5f};
    barrel.AddComponent<ScriptComponent>("sphere_script");
}

void DempstaEditor::OnDetach() {}

void DempstaEditor::OnUpdate(const TimeStep& ts) {
    m_ts = ts;
    m_timeAccumulator += (float)ts;
    // Update fps every half second
    if (m_timeAccumulator >= 0.5f) {
        m_timeAccumulator = 0.0f;
        m_fps = 1.0f / (float)ts;
    }

    auto fConfig = m_frameBuffer->GetConfig();

    if (m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f && (fConfig.width != m_viewportSize.x || fConfig.height != m_viewportSize.y)) {
        const auto width = (uint32_t)m_viewportSize.x;
        const auto height = (uint32_t)m_viewportSize.y;
        m_frameBuffer->Resize(width, height);
        m_activeScene->OnViewportResize(width, height);
    }

    Renderer2D::ResetStatistics();

    m_frameBuffer->Bind();

    RenderCommand::Clear({0.2f, 0.2f, 0.2f, 1});

    m_activeScene->OnUpdate(ts);

    m_frameBuffer->Unbind();
}

void DempstaEditor::OnEvent(Event& e) { m_activeScene->OnEvent(e); }

void DempstaEditor::OnImGuiRender() {
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if ((dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    static bool p_open = true;
    ImGui::Begin("Dempsta Editor", &p_open, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen) {
        ImGui::PopStyleVar(2);
    }

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_DockingEnable) != 0) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", nullptr, false, true)) {
                m_sceneHierarchyPanel.UnSelectEntity();
                m_activeScene = CreateRef<Scene>();
                m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
                m_sceneHierarchyPanel.SetContext(m_activeScene);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save scene as...", nullptr, false, true)) {
                auto filepath = FileUtils::SaveFile(SCENE_FILES_FILTER);
                if (filepath) {
                    SceneSerializer serializer(m_activeScene);
                    serializer.Serialize(*filepath);
                }
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Load scene as...", nullptr, false, true)) {
                std::optional<std::string> filepath = FileUtils::OpenFile(SCENE_FILES_FILTER);
                if (filepath) {
                    m_activeScene = CreateRef<Scene>();
                    SceneSerializer serializer(m_activeScene);
                    if (!serializer.Deserialize(*filepath)) {
                        LOG_ERROR("Failed to deserialize scene!");
                    }
                    m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
                    m_sceneHierarchyPanel.SetContext(m_activeScene);
                }
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", nullptr, false, true)) {
                Application::GetInstance().Close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (ImGui::Begin("Renderer2D Statistics")) {
        auto stats = Renderer2D::GetStatistics();
        ImGui::Text("Max Quads Per Draw: %d", stats.maxQuadsPerDraw);
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Drawn Quads: %d", stats.quads);
        ImGui::Text("Drawn Vertices: %d", stats.GetDrawnVertices());
        ImGui::Text("Drawn Indices: %d", stats.GetDrawnIndices());
        ImGui::Text("Time Per Frame: %.6f", (float)m_ts);
        ImGui::Text("FPS: %.0f", m_fps);
        ImGui::End();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(100.0f, 100.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (ImGui::Begin("ViewPort", &p_open, ImGuiWindowFlags_NoCollapse)) {
        ImGui::PopStyleVar(2);
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ImGui::Image((void*)(uintptr_t)m_frameBuffer->GetColorAttachment(), viewportPanelSize, {0, 1}, {1, 0});
        m_viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
        ImGui::End();
    }

    m_sceneHierarchyPanel.OnImGuiRender();

    ImGui::End();
}
}  // namespace de

#include <stdafx.h>

#if defined(ENABLE_IMGUI)

#include <Extender/Client/IMGUI/IMGUI.h>
#include <Extender/Client/IMGUI/Objects.h>
#include <CoreLib/Wrappers.h>

#include <Extender/Client/IMGUI/SDL.inl>
#include <Extender/Client/IMGUI/Vulkan.inl>

BEGIN_NS(extui)

PlatformBackend::~PlatformBackend() {}

RenderingBackend::~RenderingBackend() {}

Renderable::~Renderable() {}


void StyledRenderable::Render()
{
    for (auto const& var : StyleVars) {
        ImGui::PushStyleVar(var.Key, var.Value);
    }

    for (auto const& var : StyleColors) {
        ImGui::PushStyleColor(var.Key, ImVec4(var.Value.r, var.Value.g, var.Value.b, var.Value.a));
    }

    StyledRender();

    if (!StyleColors.empty()) {
        ImGui::PopStyleColor(StyleColors.size());
    }

    if (!StyleVars.empty()) {
        ImGui::PopStyleVar(StyleVars.size());
    }
}


void StyledRenderable::SetStyleVar(ImGuiStyleVar var, float value)
{
    for (auto& kv : StyleVars) {
        if (kv.Key == var) {
            kv.Value = value;
            return;
        }
    }

    StyleVars.push_back(StyleVar{ var, value });
}


void StyledRenderable::SetStyleColor(ImGuiCol color, glm::vec4 value)
{
    for (auto& kv : StyleColors) {
        if (kv.Key == color) {
            kv.Value = value;
            return;
        }
    }

    StyleColors.push_back(StyleColor{ color, value });
}


void Window::StyledRender()
{
    if (!ImGui::Begin(Id.c_str(), Closeable ? &Closed : nullptr, Flags)) {
        ImGui::End();
        return;
    }

    for (auto childHandle : Children) {
        auto child = Manager->GetRenderable(childHandle);
        if (child) {
            child->Render();
        }
    }

    ImGui::End();
}


void Text::StyledRender()
{
    ImGui::TextUnformatted(Label.c_str(), Label.c_str() + Label.size());
}


void Button::StyledRender()
{
    if (ImGui::Button(Label.c_str())) {
        // call delegate
    }
}


IMGUIObjectPoolInterface::~IMGUIObjectPoolInterface() {}

IMGUIObjectManager::IMGUIObjectManager()
{
    pools_[(unsigned)IMGUIObjectType::Window] = std::make_unique<IMGUIObjectPool<Window>>();
    pools_[(unsigned)IMGUIObjectType::Text] = std::make_unique<IMGUIObjectPool<Text>>();
    pools_[(unsigned)IMGUIObjectType::Button] = std::make_unique<IMGUIObjectPool<Button>>();
}

Renderable* IMGUIObjectManager::CreateRenderable(IMGUIObjectType type)
{
    auto obj = pools_[(unsigned)type]->Create();
    obj->Manager = this;

    if (type == IMGUIObjectType::Window) {
        windows_.push_back(obj->Handle);
    }

    return obj;
}

Renderable* IMGUIObjectManager::GetRenderable(HandleType handle)
{
    auto type = (handle >> 56);
    if (type > (unsigned)IMGUIObjectType::Max) return nullptr;
    return pools_[type]->Get(handle);
}

bool IMGUIObjectManager::DestroyRenderable(HandleType handle)
{
    auto type = (handle >> 56);
    if (type > (unsigned)IMGUIObjectType::Max) return false;
    return pools_[type]->Destroy(handle);
}

void IMGUIObjectManager::Render()
{
    ImGui::Begin("Stuck in the Epipeline", nullptr, 0);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            ImGui::MenuItem("Main menu bar", NULL, nullptr);
            ImGui::SeparatorText("Mini apps");
            ImGui::MenuItem("Console", NULL, nullptr);
            ImGui::EndMenu();
        }
    }

    ImGui::Text("%s", "ASDSADASADDSADSADSADAS");
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("asdf"))
    {
        ImGui::SeparatorText("asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf");
        ImGui::Indent();
        ImGui::BulletText("fggsedfhghfgfhgfhgfhfgdhgfdhfdhfgdhgfdhfhfhggfgfd");
        ImGui::Unindent();
    }

    if (ImGui::CollapsingHeader("asdfjkl"))
    {
        ImGui::SeparatorText("ABOUT THIS DEMO:");
        ImGui::Indent();
        ImGui::BulletText("Sections below are demonstrating many aspects of the library.");
        ImGui::Unindent();
    }

    ImGui::Button("A button ?!");
    float v = 0.5f;
    float min = 0.0f;
    float max = 1.0f;
    ImGui::SliderScalar("epipeline", ImGuiDataType_Float, &v, &min, &max);

    ImGui::End();

    //ImGui::ShowDemoWindow();

    for (auto windowHandle : windows_) {
        auto window = GetRenderable(windowHandle);
        if (window) {
            window->Render();
        }
    }
}


IMGUIManager::IMGUIManager()
{
    platform_ = std::make_unique<SDLBackend>();
    renderer_ = std::make_unique<VulkanBackend>(*this);
}

IMGUIManager::~IMGUIManager()
{
    DestroyUI();
}

void IMGUIManager::EnableHooks()
{
    platform_->EnableHooks();
    renderer_->EnableHooks();
}

void IMGUIManager::DisableHooks()
{
    platform_->DisableHooks();
    renderer_->DisableHooks();
}

void IMGUIManager::InitializeUI()
{
    if (initialized_) return;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    platform_->InitializeUI();
    renderer_->InitializeUI();
    initialized_ = true;
}

void IMGUIManager::DestroyUI()
{
    if (!initialized_) return;

    initialized_ = false;
    renderer_->DestroyUI();
    platform_->DestroyUI();
    ImGui::DestroyContext();
}

void IMGUIManager::EnableUI(bool enabled)
{
    enableUI_ = enabled;
}

void IMGUIManager::SetObjects(IMGUIObjectManager* objects)
{
    objects_ = objects;
}

void IMGUIManager::OnRenderBackendInitialized()
{
    InitializeUI();
}

void IMGUIManager::OnClientRenderFrame()
{
    if (!enableUI_ || !initialized_) return;

    platform_->NewFrame();
    renderer_->NewFrame();
    ImGui::NewFrame();

    if (objects_) {
        objects_->Render();
    }

    ImGui::Render();
    renderer_->RenderFrame();
}

END_NS()

#endif

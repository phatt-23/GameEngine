//
// Created by phatt on 2/10/25.
//

#include "ImGuiLayer.h"
#include "Application.h"
#include "Platform/OpenGL/ImGui_Impl_OpenGL.h"
#include "Platform/OpenGL/ImGui_Impl_GLFW.h"

#include <GLFW/glfw3.h>

namespace Engine
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer") {}

    ImGuiLayer::~ImGuiLayer()
    = default;

    void ImGuiLayer::OnEvent(Event &event)
    {
         EventDispatcher dispatcher(event);
         dispatcher.Dispatch<MouseButtonPressedEvent>(EG_FORWARD_EVENT_TO_MEM_FN(ImGuiLayer::OnMouseButtonPressedEvent));
         dispatcher.Dispatch<MouseButtonReleasedEvent>(EG_FORWARD_EVENT_TO_MEM_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
         dispatcher.Dispatch<MouseMovedEvent>(EG_FORWARD_EVENT_TO_MEM_FN(ImGuiLayer::OnMouseMovedEvent));
         dispatcher.Dispatch<MouseScrolledEvent>(EG_FORWARD_EVENT_TO_MEM_FN(ImGuiLayer::OnMouseScrolledEvent));
         dispatcher.Dispatch<KeyPressedEvent>(EG_FORWARD_EVENT_TO_MEM_FN(ImGuiLayer::OnKeyPressedEvent));
         dispatcher.Dispatch<KeyReleasedEvent>(EG_FORWARD_EVENT_TO_MEM_FN(ImGuiLayer::OnKeyReleasedEvent));
         dispatcher.Dispatch<KeyTypedEvent>(EG_FORWARD_EVENT_TO_MEM_FN(ImGuiLayer::OnKeyTypedEvent));
         dispatcher.Dispatch<WindowResizeEvent>(EG_FORWARD_EVENT_TO_MEM_FN(ImGuiLayer::OnWindowResize));
    }

    void ImGuiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        Application& app = Application::Get();
        auto* window = (GLFWwindow*)app.GetWindow().GetNativeWindow();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnUpdate()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // start

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        // end
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();

        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0/60.0);
        m_Time = time;
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent &event) {
//        auto& io = ImGui::GetIO();
//        io.AddMouseButtonEvent(event.GetMouseButton(), true);
//        return io.WantCaptureMouse;
        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent &event) {
//        auto& io = ImGui::GetIO();
//        io.AddMouseButtonEvent(event.GetMouseButton(), false);
//        return io.WantCaptureMouse;
        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent &event) {
//        auto& io = ImGui::GetIO();
//        io.AddMousePosEvent(event.GetX(), event.GetY());
//        return io.WantCaptureMouse;
        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent &event) {
//        auto& io = ImGui::GetIO();
//        io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());
//        return io.WantCaptureMouse;
        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent &event) {
        // ImGui::GetIO().AddKeyEvent(ImGui_ImplGlfw_KeyToImGuiKey(event.GetKeyCode(), 0), true);
        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent &event) {
        // ImGui::GetIO().AddKeyEvent(ImGui_ImplGlfw_KeyToImGuiKey(event.GetKeyCode(), 0), false);
        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent &event) {
//        if (0 < event.GetKeyCode() && event.GetKeyCode() < 0x10000)
//        {
//            ImGui::GetIO().AddInputCharacter(event.GetKeyCode());
//        }

        return false;
    }

    bool ImGuiLayer::OnWindowResize(WindowResizeEvent &event) {
//        ImGuiIO& io = ImGui::GetIO();
//        io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
//        io.DisplayFramebufferScale = ImVec2(1.f, 1.f);

        return false;
    }

}

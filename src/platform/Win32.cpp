#include <Sapphire/platform/platform.hpp>

#ifdef SAPPHIRE_IS_WINDOWS

#include <Sapphire/modify/CCEGLView.hpp>
#include "platform.hpp"

using namespace cocos2d;
using namespace sapphire;

ImGuiKey keyFromGLFW(int key) {
    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
        return static_cast<ImGuiKey>(ImGuiKey_0 + (key - GLFW_KEY_0));
    } else if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
        return static_cast<ImGuiKey>(ImGuiKey_A + (key - GLFW_KEY_A));
    }
    switch (key) {
        case GLFW_KEY_SPACE: return ImGuiKey_Space;
        case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
        case GLFW_KEY_COMMA: return ImGuiKey_Comma;
        case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
        case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
        case GLFW_KEY_UP: return ImGuiKey_UpArrow;
        case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
        case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
        case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
        case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
        case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
        case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
        // TODO: rest :-)
    }
    return ImGuiKey_None;
}

class $modify(CCEGLView) {
    void updateWindow(int width, int height) {
        shouldUpdateGDRenderBuffer() = true;
        CCEGLView::updateWindow(width, height);
    }

    void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto& io = ImGui::GetIO();
        CCEGLView::onGLFWKeyCallback(window, key, scancode, action, mods);
        // in practice this is only used for arrow keys
        io.AddKeyEvent(keyFromGLFW(key), action != GLFW_RELEASE);
    }
};

#include "utils.hpp"

std::string formatAddressIntoOffsetImpl(uintptr_t addr) {
    HMODULE mod;

    if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<char*>(addr), &mod)
    ) {
        mod = nullptr;
    }

    wchar_t buffer[MAX_PATH];
    std::string const module_name = (!mod || !GetModuleFileNameW(mod, buffer, MAX_PATH)) ? "Unknown" : ghc::filesystem::path(buffer).filename().string();

    return fmt::format("{} + {:#x}", module_name, addr - reinterpret_cast<uintptr_t>(mod));
}

#endif
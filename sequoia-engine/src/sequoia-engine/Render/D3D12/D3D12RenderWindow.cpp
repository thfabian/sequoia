//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia-engine/Render/D3D12/D3D12.h"
#include "sequoia-engine/Render/D3D12/D3D12RenderWindow.h"
#include "sequoia-engine/Render/Exception.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/UtfString.h"
#include "sequoia-engine/Core/Platform.h"
#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/Win32Util.h"
#include "sequoia-engine/Core/Unreachable.h"
#include <cstring>
#include <memory>
#include <cstdint>

namespace sequoia {

namespace render {

namespace {

static std::int16_t KeyTables[512];

static void createKeyTables() {
  std::memset(KeyTables, Key_Invalid, sizeof(KeyTables));

  KeyTables[0x00B] = Key_0;
  KeyTables[0x002] = Key_1;
  KeyTables[0x003] = Key_2;
  KeyTables[0x004] = Key_3;
  KeyTables[0x005] = Key_4;
  KeyTables[0x006] = Key_5;
  KeyTables[0x007] = Key_6;
  KeyTables[0x008] = Key_7;
  KeyTables[0x009] = Key_8;
  KeyTables[0x00A] = Key_9;
  KeyTables[0x01E] = Key_A;
  KeyTables[0x030] = Key_B;
  KeyTables[0x02E] = Key_C;
  KeyTables[0x020] = Key_D;
  KeyTables[0x012] = Key_E;
  KeyTables[0x021] = Key_F;
  KeyTables[0x022] = Key_G;
  KeyTables[0x023] = Key_H;
  KeyTables[0x017] = Key_I;
  KeyTables[0x024] = Key_J;
  KeyTables[0x025] = Key_K;
  KeyTables[0x026] = Key_L;
  KeyTables[0x032] = Key_M;
  KeyTables[0x031] = Key_N;
  KeyTables[0x018] = Key_O;
  KeyTables[0x019] = Key_P;
  KeyTables[0x010] = Key_Q;
  KeyTables[0x013] = Key_R;
  KeyTables[0x01F] = Key_S;
  KeyTables[0x014] = Key_T;
  KeyTables[0x016] = Key_U;
  KeyTables[0x02F] = Key_V;
  KeyTables[0x011] = Key_W;
  KeyTables[0x02D] = Key_X;
  KeyTables[0x015] = Key_Y;
  KeyTables[0x02C] = Key_Z;

  KeyTables[0x028] = Key_Apostrophe;
  KeyTables[0x02B] = Key_Backslash;
  KeyTables[0x033] = Key_Comma;
  KeyTables[0x00D] = Key_Equal;
  KeyTables[0x029] = Key_GraveAccent;
  KeyTables[0x01A] = Key_LeftBracket;
  KeyTables[0x00C] = Key_Minus;
  KeyTables[0x034] = Key_Period;
  KeyTables[0x01B] = Key_RightBracket;
  KeyTables[0x027] = Key_Semicolon;
  KeyTables[0x035] = Key_Slash;
  KeyTables[0x056] = Key_World2;

  KeyTables[0x00E] = Key_Backspace;
  KeyTables[0x153] = Key_Delete;
  KeyTables[0x14F] = Key_End;
  KeyTables[0x01C] = Key_Enter;
  KeyTables[0x001] = Key_Escape;
  KeyTables[0x147] = Key_Home;
  KeyTables[0x152] = Key_Insert;
  KeyTables[0x15D] = Key_Menu;
  KeyTables[0x151] = Key_PageDown;
  KeyTables[0x149] = Key_PageUp;
  KeyTables[0x045] = Key_Pause;
  KeyTables[0x146] = Key_Pause;
  KeyTables[0x039] = Key_Space;
  KeyTables[0x00F] = Key_Tab;
  KeyTables[0x03A] = Key_CapsLock;
  KeyTables[0x145] = Key_NumLock;
  KeyTables[0x046] = Key_ScrollLock;
  KeyTables[0x03B] = Key_F1;
  KeyTables[0x03C] = Key_F2;
  KeyTables[0x03D] = Key_F3;
  KeyTables[0x03E] = Key_F4;
  KeyTables[0x03F] = Key_F5;
  KeyTables[0x040] = Key_F6;
  KeyTables[0x041] = Key_F7;
  KeyTables[0x042] = Key_F8;
  KeyTables[0x043] = Key_F9;
  KeyTables[0x044] = Key_F10;
  KeyTables[0x057] = Key_F11;
  KeyTables[0x058] = Key_F12;
  KeyTables[0x064] = Key_F13;
  KeyTables[0x065] = Key_F14;
  KeyTables[0x066] = Key_F15;
  KeyTables[0x067] = Key_F16;
  KeyTables[0x068] = Key_F17;
  KeyTables[0x069] = Key_F18;
  KeyTables[0x06A] = Key_F19;
  KeyTables[0x06B] = Key_F20;
  KeyTables[0x06C] = Key_F21;
  KeyTables[0x06D] = Key_F22;
  KeyTables[0x06E] = Key_F23;
  KeyTables[0x076] = Key_F24;
  KeyTables[0x038] = Key_LeftAlt;
  KeyTables[0x01D] = Key_LeftControl;
  KeyTables[0x02A] = Key_LeftShift;
  KeyTables[0x15B] = Key_LeftSuper;
  KeyTables[0x137] = Key_PrintScreen;
  KeyTables[0x138] = Key_RightAlt;
  KeyTables[0x11D] = Key_RightControl;
  KeyTables[0x036] = Key_RightShift;
  KeyTables[0x15C] = Key_RightSuper;
  KeyTables[0x150] = Key_Down;
  KeyTables[0x14B] = Key_Left;
  KeyTables[0x14D] = Key_Right;
  KeyTables[0x148] = Key_Up;

  KeyTables[0x052] = Key_KP_0;
  KeyTables[0x04F] = Key_KP_1;
  KeyTables[0x050] = Key_KP_2;
  KeyTables[0x051] = Key_KP_3;
  KeyTables[0x04B] = Key_KP_4;
  KeyTables[0x04C] = Key_KP_5;
  KeyTables[0x04D] = Key_KP_6;
  KeyTables[0x047] = Key_KP_7;
  KeyTables[0x048] = Key_KP_8;
  KeyTables[0x049] = Key_KP_9;
  KeyTables[0x04E] = Key_KP_Add;
  KeyTables[0x053] = Key_KP_Decimal;
  KeyTables[0x135] = Key_KP_Divide;
  KeyTables[0x11C] = Key_KP_Enter;
  KeyTables[0x037] = Key_KP_Multiply;
  KeyTables[0x04A] = Key_KP_Subtract;
}

/// @brief Translates a Windows key to the corresponding GLFW key
static int translateKey(WPARAM wParam, LPARAM lParam) {
  // The Ctrl keys require special handling
  if(wParam == VK_CONTROL) {
    MSG next;
    DWORD time;

    // Right side keys have the extended key bit set
    if(lParam & 0x01000000)
      return Key_RightControl;

    // HACK: Alt-Gr sends Left-Ctrl and then Right-Alt in close sequence. We only want the
    //       Right-Alt message, so if the next message is Right-Alt we ignore this (synthetic)
    //       Left-Ctrl message
    time = GetMessageTime();
    if(PeekMessageW(&next, NULL, 0, 0, PM_NOREMOVE)) {
      if(next.message == WM_KEYDOWN || next.message == WM_SYSKEYDOWN || next.message == WM_KEYUP ||
         next.message == WM_SYSKEYUP) {
        if(next.wParam == VK_MENU && (next.lParam & 0x01000000) && next.time == time) {
          // Next message is Right-Alt down so discard this
          return Key_Invalid;
        }
      }
    }

    return Key_LeftControl;
  }

  if(wParam == VK_PROCESSKEY)
    // IME notifies that keys have been filtered by setting the virtual key-code to VK_PROCESSKEY
    return Key_Invalid;

  return KeyTables[HIWORD(lParam) & 0x1FF];
}

/// @brief Retrieves and translates modifier keys
static int getKeyMods() {
  int mods = 0;
  if(GetKeyState(VK_SHIFT) & 0x8000)
    mods |= Mod_Shift;
  if(GetKeyState(VK_CONTROL) & 0x8000)
    mods |= Mod_Ctrl;
  if(GetKeyState(VK_MENU) & 0x8000)
    mods |= Mod_Alt;
  if((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000)
    mods |= Mod_Super;
  if(GetKeyState(VK_CAPITAL) & 1)
    mods |= Mod_CapsLock;
  if(GetKeyState(VK_NUMLOCK) & 1)
    mods |= Mod_NumLock;
  return mods;
}

/// @brief Query display information
struct DisplayMonitorInfo {
  int RequestedID;
  int CurrentID;
  std::unique_ptr<HMONITOR> HMonitor;

  static DisplayMonitorInfo* Instance;

  DisplayMonitorInfo(int requestedID) : RequestedID(requestedID), CurrentID(0), HMonitor(nullptr) {
    DisplayMonitorInfo::Instance = this;
    EnumDisplayMonitors(NULL, NULL, DisplayMonitorInfo::call, 0);
  }

  static BOOL CALLBACK call(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
    DisplayMonitorInfo* cb = DisplayMonitorInfo::Instance;
    if(cb->CurrentID == cb->RequestedID) {
      cb->HMonitor = std::make_unique<HMONITOR>();
      std::memcpy(cb->HMonitor.get(), &hMonitor, sizeof(HMONITOR));
      return false;
    }

    cb->CurrentID++;
    return true;
  }
};

DisplayMonitorInfo* DisplayMonitorInfo::Instance = nullptr;

/// @brief Get the monitor info of the monitor identified by id
static void getMonitorInfoFromID(int id, MONITORINFO* monitorInfo) {
  ZeroMemory(monitorInfo, sizeof(MONITORINFO));
  monitorInfo->cbSize = sizeof(MONITORINFO);

  DisplayMonitorInfo dm(id);
  if(!dm.HMonitor)
    SEQUOIA_THROW(RenderSystemException, "invalid monitor id: {}", id);

  GetMonitorInfo(*dm.HMonitor, monitorInfo);
}

/// @brief Called every time the application receives a message
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  D3D12RenderWindow* window =
      reinterpret_cast<D3D12RenderWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

  switch(message) {
  case WM_KEYDOWN:
  case WM_SYSKEYDOWN:
  case WM_KEYUP:
  case WM_SYSKEYUP: {
    if(window) {
      const int key = translateKey(wParam, lParam);
      const int action = ((lParam >> 31) & 1) & 1 ? Action_Released : Action_Pressed;
      const int mods = getKeyMods();
      if(key == Key_Invalid)
        break;

      window->keyCallback(key, action, mods);
    }
    break;
  }
  case WM_CREATE: {
    // Save the D3D12RenderWindow* passed in to CreateWindow
    LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
    SetWindowLongPtr(hWnd, GWLP_USERDATA,
                     reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
    return 0;
  }
  case WM_SIZE: {
    if(window) {
      RECT clientRec;
      ZeroMemory(&clientRec, sizeof(RECT));
      ::GetClientRect(hWnd, &clientRec);
      window->resizeCallback(clientRec.right - clientRec.left, clientRec.bottom - clientRec.top);
    }
    return 0;
  }
  case WM_CLOSE:
  case WM_DESTROY: {
    ::PostQuitMessage(0);
    ::DestroyWindow(hWnd);
    return 0;
  }
  }
  return ::DefWindowProc(hWnd, message, wParam, lParam);
}

} // anonymous namespace

void D3D12RenderWindow::keyCallback(int key, int action, int mods) noexcept {
  KeyboardEvent event{(KeyboardKey)key, (KeyAction)action, mods};
  for(KeyboardListener* listener : getListeners<KeyboardListener>())
    listener->keyboardEvent(event);
}

void D3D12RenderWindow::resizeCallback(int width, int height) noexcept {
  width_ = width;
  height_ = height;

  Log::info("Resizing window {} to {} x {}", core::ptrToStr(this), width_, height_);
  Viewport* viewport = getViewport();
  viewport->updateGeometry(viewport->getX(), viewport->getY(), width_, height_);
}

D3D12RenderWindow::D3D12RenderWindow(const RenderWindow::WindowHint& hints)
    : RenderWindow(RK_D3D12RenderWindow), keyModifier_(0), isClosed_(false), isFullscreen_(false) {
  Log::info("Initializing D3D12 window {} ...", core::ptrToStr(this));

  // Initialize the key translation table
  createKeyTables();

  auto toWStr = [](const std::string& str) { return UtfString(str).toWideString(); };
  auto toStdStr = [](const wchar_t* str) { return UtfString(str).toAnsiString(); };

  // Set a default view-port
  setViewport(std::make_shared<Viewport>(this, 0, 0, 0, 0));

  HINSTANCE hInstance = GetModuleHandle(0);

  // Register class
  WNDCLASSEX wcex;
  ZeroMemory(&wcex, sizeof(WNDCLASSEX));
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WindowProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = nullptr;
  wcex.lpszClassName = L"D3D12RenderWindow";
  wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
  if(::RegisterClassEx(&wcex) == 0)
    SEQUOIA_THROW(RenderSystemException, "unable to register window: {}",
                  Win32Util::getLastError());

  // Create window
  RECT rc = {0, 0, (LONG)hints.Width, (LONG)hints.Height};
  ::AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

  bool useDefaultMonitor = hints.Monitor == -1;
  int xpos = CW_USEDEFAULT, ypos = CW_USEDEFAULT;
  int width = rc.right - rc.left, height = rc.bottom - rc.top;

  // Query Monitor
  MONITORINFO monitorInfo;
  int monitorID = useDefaultMonitor ? 0 : hints.Monitor;
  getMonitorInfoFromID(monitorID, &monitorInfo);

  DISPLAY_DEVICE device;
  ZeroMemory(&device, sizeof(device));
  device.cb = sizeof(DISPLAY_DEVICE);
  EnumDisplayDevices(NULL, monitorID, &device, 0);
  Log::info("Monitor Device: {}", monitorID);
  Log::info("Monitor DeviceName: {}", toStdStr(device.DeviceName));
  Log::info("Monitor DeviceString: {}", toStdStr(device.DeviceString));
  Log::info("Monitor StateFlags: {}",
            toStdStr((device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) ? L"desktop " : L"") +
            toStdStr((device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) ? L"primary " : L"") +
            toStdStr((device.StateFlags & DISPLAY_DEVICE_VGA_COMPATIBLE) ? L"vga " : L"") +
            toStdStr((device.StateFlags & DISPLAY_DEVICE_MULTI_DRIVER) ? L"multi " : L"") +
            toStdStr((device.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) ? L"mirror " : L""));

  if(!useDefaultMonitor) {
    xpos = monitorInfo.rcMonitor.left;
    ypos = monitorInfo.rcMonitor.top;
    if(hints.WindowMode != WindowHint::WK_Window) {
      width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
      height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
    }
  }

  DWORD windowStyle = WS_OVERLAPPEDWINDOW;
  hwnd_ = ::CreateWindow(wcex.lpszClassName, toWStr(hints.Title).c_str(), windowStyle, xpos, ypos,
                         width, height, nullptr, nullptr, hInstance, static_cast<void*>(this));

  if(hwnd_ == 0)
    SEQUOIA_THROW(RenderSystemException, "failed to create window: {}", Win32Util::getLastError());

  if(!hints.HideWindow)
    ::ShowWindow(hwnd_, hints.WindowMode == WindowHint::WK_WindowedFullscreen ? SW_MAXIMIZE
                                                                              : SW_SHOWDEFAULT);

  // Enter fullscreen mode
  if(hints.WindowMode == WindowHint::WK_Fullscreen) {
    if(!!::IsZoomed(hwnd_))
      ::SendMessage(hwnd_, WM_SYSCOMMAND, SC_RESTORE, 0);

    LONG style = ::GetWindowLong(hwnd_, GWL_STYLE);
    LONG exStyle = ::GetWindowLong(hwnd_, GWL_EXSTYLE);

    ::SetWindowLong(hwnd_, GWL_STYLE, style & ~(WS_CAPTION | WS_THICKFRAME));
    ::SetWindowLong(
        hwnd_, GWL_EXSTYLE,
        exStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

    ::SetWindowPos(hwnd_, NULL, xpos, ypos, width, height,
                   SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    isFullscreen_ = true;
  }

  // Query window geometry
  RECT clientRec;
  ZeroMemory(&clientRec, sizeof(RECT));
  ::GetClientRect(hwnd_, &clientRec);
  width_ = clientRec.right - clientRec.left;
  height_ = clientRec.bottom - clientRec.top;

  Log::info("Successfully initialized D3D12 window {}", core::ptrToStr(this));
}

D3D12RenderWindow::~D3D12RenderWindow() {
  Log::info("Terminating D3D12 window {} ...", core::ptrToStr(this));

  Log::info("Done terminating D3D12 window {}", core::ptrToStr(this));
}

bool D3D12RenderWindow::isHidden() { return !IsWindowVisible(hwnd_); }

bool D3D12RenderWindow::isClosed() { return isClosed_; }

bool D3D12RenderWindow::isFullscreen() const { return isFullscreen_; }

int D3D12RenderWindow::getWidth() const { return width_; }

int D3D12RenderWindow::getHeight() const { return height_; }

void D3D12RenderWindow::swapBuffers() {}

void D3D12RenderWindow::setCursorMode(RenderWindow::CursorModeKind mode) {}

void D3D12RenderWindow::centerCursor() {}

void D3D12RenderWindow::pollEvents() {
  MSG msg = {};

  // Check to see if any messages are waiting in the queue
  while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    // Translate the message and dispatch it to WindowProc()
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  if(msg.message == WM_QUIT)
    isClosed_ = true;
}

} // namespace render

} // namespace sequoia

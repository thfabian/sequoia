//===-- sequoia/Game/InputManager.cpp -----------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/GlobalConfiguration.h"
#include "sequoia/Game/InputManager.h"
#include <OGRE/OgreLogManager.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreWindowEventUtilities.h>
#include <OIS/OISException.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

template <>
sequoia::game::InputManager* Ogre::Singleton<sequoia::game::InputManager>::msSingleton = nullptr;

namespace sequoia {

namespace game {

class InputManager::InputManagerImpl : public OIS::MouseListener, public OIS::KeyListener {
public:
  InputManagerImpl()
      : isInitialized_(false), inputSystem_(nullptr), mouse_(nullptr), keyboard_(nullptr) {}

  ~InputManagerImpl() {
    if(isInitialized_)
      finalize();
  }

  //===----------------------------------------------------------------------------------------===//
  //     Initialize & Finalize
  //===----------------------------------------------------------------------------------------===//
  void init(Ogre::RenderWindow* window) {
    Ogre::LogManager::getSingletonPtr()->logMessage("Initializing OIS");

    OIS::ParamList paramList;

    // Window Handle
    size_t windowHnd = 0;
    window->getCustomAttribute("WINDOW", &windowHnd);
    paramList.insert(std::make_pair(std::string("WINDOW"), std::to_string(windowHnd)));

    // Non-exclusive input (no fullscreen)
    if(!GlobalConfiguration::getSingleton().getBoolean("Graphics.FullScreen")) {
      Ogre::LogManager::getSingletonPtr()->logMessage("Setting OIS to non-exclusive input");
#if defined OIS_WIN32_PLATFORM
      paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
      paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
      paramList.insert(
          std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
      paramList.insert(
          std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
      paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
      paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
      paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
      paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif
    }

    try {
      // Initialize InputManger
      inputSystem_ = OIS::InputManager::createInputSystem(paramList);

      // If possible create a buffered keyboard
      if(inputSystem_->getNumberOfDevices(OIS::OISKeyboard) > 0) {
        keyboard_ =
            static_cast<OIS::Keyboard*>(inputSystem_->createInputObject(OIS::OISKeyboard, true));
        keyboard_->setEventCallback(this);
      }

      // If possible create a buffered mouse
      if(inputSystem_->getNumberOfDevices(OIS::OISMouse) > 0) {
        mouse_ = static_cast<OIS::Mouse*>(inputSystem_->createInputObject(OIS::OISMouse, true));
        mouse_->setEventCallback(this);

        // Get window size
        unsigned int width, height, depth;
        int left, top;
        window->getMetrics(width, height, depth, left, top);

        // Set mouse region
        this->setWindowExtents(width, height);
      }

      isInitialized_ = true;
    } catch(OIS::Exception& e) {
      Ogre::LogManager::getSingletonPtr()->logMessage(
          Ogre::LML_CRITICAL, core::format("OIS failed to initialize: %s", e.what()));
      SEQUOIA_THROW(IOException, e.what());
    }

    // Register as window listener
    // window(this);
  }

  void finalize() {
    if(isInitialized_) {
      Ogre::LogManager::getSingletonPtr()->logMessage("Finalizing OIS");
      OIS::InputManager::destroyInputSystem(inputSystem_);
      inputSystem_ = nullptr;
      isInitialized_ = false;
    }
  }

  void capture() {
    SEQUOIA_ASSERT(isInitialized_);
    if(mouse_)
      mouse_->capture();

    if(keyboard_)
      keyboard_->capture();
  }

  void setWindowExtents(int width, int height) {
    const OIS::MouseState& mouseState = mouse_->getMouseState();
    mouseState.width = width;
    mouseState.height = height;
  }

  bool isInitialized() const noexcept { return isInitialized_; }

  //===----------------------------------------------------------------------------------------===//
  //     Mouse
  //===----------------------------------------------------------------------------------------===//

  void addMouseListener(OIS::MouseListener* mouseListener, const std::string& instanceName) {}

  void removeMouseListener(OIS::MouseListener* mouseListener) {}

  OIS::Mouse* mouse() noexcept {
    SEQUOIA_ASSERT(isInitialized_);
    return mouse_;
  }

  const OIS::Mouse* mouse() const noexcept {
    SEQUOIA_ASSERT(isInitialized_);
    return mouse_;
  }

  bool mouseMoved(const OIS::MouseEvent& e) { return true; }
  bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) { return true; }
  bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) { return true; }

  //===----------------------------------------------------------------------------------------===//
  //     Keyboard
  //===----------------------------------------------------------------------------------------===//

  void addKeyListener(OIS::KeyListener* keyListener, const std::string& instanceName) {}

  void removeKeyListener(OIS::KeyListener* keyListener) {}

  OIS::Keyboard* keyboard() noexcept {
    SEQUOIA_ASSERT(isInitialized_);
    return keyboard_;
  }

  const OIS::Keyboard* keyboard() const noexcept {
    SEQUOIA_ASSERT(isInitialized_);
    return keyboard_;
  }

  bool keyPressed(const OIS::KeyEvent& e) { return true; }
  bool keyReleased(const OIS::KeyEvent& e) { return true; }

private:
  bool isInitialized_;
  OIS::InputManager* inputSystem_;
  OIS::Mouse* mouse_;
  OIS::Keyboard* keyboard_;
};

InputManager::InputManager() : impl_(std::make_unique<InputManagerImpl>()) {}

InputManager::~InputManager() { finalize(); }

void InputManager::init(Ogre::RenderWindow* window) { impl_->init(window); }

void InputManager::finalize() { impl_->finalize(); }

void InputManager::capture() { impl_->capture(); }

void InputManager::addKeyListener(OIS::KeyListener* keyListener, const std::string& instanceName) {
  impl_->addKeyListener(keyListener, instanceName);
}

void InputManager::removeKeyListener(OIS::KeyListener* keyListener) {
  impl_->removeKeyListener(keyListener);
}

void InputManager::addMouseListener(OIS::MouseListener* mouseListener,
                                    const std::string& instanceName) {
  impl_->addMouseListener(mouseListener, instanceName);
}

void InputManager::removeMouseListener(OIS::MouseListener* mouseListener) {
  impl_->removeMouseListener(mouseListener);
}

void InputManager::setWindowExtents(int width, int height) {
  impl_->setWindowExtents(width, height);
}

bool InputManager::isInitialized() const noexcept { return impl_->isInitialized(); }

} // namespace game

} // namespace sequoia
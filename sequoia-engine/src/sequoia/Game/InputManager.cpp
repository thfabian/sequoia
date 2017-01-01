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
#include <map>

template <>
sequoia::game::InputManager* Ogre::Singleton<sequoia::game::InputManager>::msSingleton = nullptr;

namespace sequoia {

namespace game {

class InputManager::InputManagerImpl : public OIS::MouseListener,
                                       public OIS::KeyListener,
                                       public Ogre::WindowEventListener {
public:
  InputManagerImpl()
      : isInitialized_(false), window_(nullptr), inputSystem_(nullptr), mouse_(nullptr), keyboard_(nullptr) {}

  ~InputManagerImpl() {
    if(isInitialized_)
      finalize();
  }

  //===----------------------------------------------------------------------------------------===//
  //     Initialize & Finalize
  //===----------------------------------------------------------------------------------------===//
  void init(Ogre::RenderWindow* window) {
    Ogre::LogManager::getSingletonPtr()->logMessage("OIS: Initializing ...");

    SEQUOIA_ASSERT(window);
    window_ = window;

    OIS::ParamList paramList;

    // Window Handle
    size_t windowHnd = 0;
    window->getCustomAttribute("WINDOW", &windowHnd);
    paramList.insert(std::make_pair(std::string("WINDOW"), std::to_string(windowHnd)));

    // Non-exclusive input (no fullscreen)
    if(!GlobalConfiguration::getSingleton().getBoolean("GraphicsSettings.Fullscreen")) {
      Ogre::LogManager::getSingletonPtr()->logMessage("OIS: Setting input to non-exclusive");
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

      // Create a buffered keyboard
      keyboard_ =
          static_cast<OIS::Keyboard*>(inputSystem_->createInputObject(OIS::OISKeyboard, true));
      keyboard_->setEventCallback(this);

      // Create a buffered mouse
      mouse_ = static_cast<OIS::Mouse*>(inputSystem_->createInputObject(OIS::OISMouse, true));
      mouse_->setEventCallback(this);
      windowResized(window); // Set mouse extent

      isInitialized_ = true;
    } catch(OIS::Exception& e) {
      Ogre::LogManager::getSingletonPtr()->logMessage(
          Ogre::LML_CRITICAL, core::format("OIS failed to initialize: %s", e.what()));
      SEQUOIA_THROW(IOException, e.what());
    }

    // Register as window listener
    Ogre::WindowEventUtilities::addWindowEventListener(window_, this);

    Ogre::LogManager::getSingletonPtr()->logMessage("OIS: Successfully initialized");
  }

  void finalize() {
    if(isInitialized_) {
      Ogre::LogManager::getSingletonPtr()->logMessage("OIS: Finalizing ...");

      inputSystem_->destroyInputObject(mouse_);
      mouse_ = nullptr;

      inputSystem_->destroyInputObject(keyboard_);
      keyboard_ = nullptr;

      OIS::InputManager::destroyInputSystem(inputSystem_);
      inputSystem_ = nullptr;

      mouseListeners_.clear();
      keyListeners_.clear();

      isInitialized_ = false;
      
      // Unregister as window listener
      Ogre::WindowEventUtilities::removeWindowEventListener(window_, this);

      Ogre::LogManager::getSingletonPtr()->logMessage("OIS: Successfully finalized");
    }
  }

  void capture() {
    SEQUOIA_ASSERT(isInitialized_);
    mouse_->capture();
    keyboard_->capture();
  }

  void setWindowExtents(int width, int height) {
    const OIS::MouseState& mouseState = mouse_->getMouseState();
    mouseState.width = width;
    mouseState.height = height;
  }

  virtual void windowResized(Ogre::RenderWindow* window) override {
    unsigned int width, height, depth;
    int left, top;
    window->getMetrics(width, height, depth, left, top);
    this->setWindowExtents(width, height);
  }

  bool isInitialized() const noexcept { return isInitialized_; }

  //===----------------------------------------------------------------------------------------===//
  //     Mouse
  //===----------------------------------------------------------------------------------------===//

  void addMouseListener(OIS::MouseListener* mouseListener, const std::string& name) {
    Ogre::LogManager::getSingletonPtr()->logMessage("OIS: Adding MouseListener: " + name);
    mouseListeners_.insert(std::make_pair(name, mouseListener));
  }

  void removeMouseListener(const std::string& name) {
    Ogre::LogManager::getSingletonPtr()->logMessage("OIS: Removing MouseListener: " + name);
    auto ret = mouseListeners_.erase(name);
    SEQUOIA_ASSERT_MSG(ret > 0, "Removing non-exsting MouseListener");
    (void)ret;
  }

  bool mouseMoved(const OIS::MouseEvent& e) override {
    for(auto it = mouseListeners_.begin(), end = mouseListeners_.end(); it != end; ++it)
      if(!it->second->mouseMoved(e))
        break;
    return true;
  }

  bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) override {
    for(auto it = mouseListeners_.begin(), end = mouseListeners_.end(); it != end; ++it)
      if(!it->second->mousePressed(e, id))
        break;
    return true;
  }

  bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) override {
    for(auto it = mouseListeners_.begin(), end = mouseListeners_.end(); it != end; ++it)
      if(!it->second->mouseReleased(e, id))
        break;
    return true;
  }

  //===----------------------------------------------------------------------------------------===//
  //     Keyboard
  //===----------------------------------------------------------------------------------------===//

  void addKeyListener(OIS::KeyListener* keyListener, const std::string& name) {
    Ogre::LogManager::getSingletonPtr()->logMessage("OIS: Adding KeyListener: " + name);
    keyListeners_.insert(std::make_pair(name, keyListener));
  }

  void removeKeyListener(const std::string& name) {
    Ogre::LogManager::getSingletonPtr()->logMessage("OIS: Removing KeyListener: " + name);
    auto ret = keyListeners_.erase(name);
    SEQUOIA_ASSERT_MSG(ret > 0, "Removing non-exsting KeyListener");
    (void)ret;
  }

  bool keyPressed(const OIS::KeyEvent& e) override {
    for(auto it = keyListeners_.begin(), end = keyListeners_.end(); it != end; ++it)
      if(!it->second->keyPressed(e))
        break;
    return true;
  }

  bool keyReleased(const OIS::KeyEvent& e) override {
    for(auto it = keyListeners_.begin(), end = keyListeners_.end(); it != end; ++it)
      if(!it->second->keyReleased(e))
        break;
    return true;
  }

private:
  bool isInitialized_;
  Ogre::RenderWindow* window_;
  OIS::InputManager* inputSystem_;
  OIS::Mouse* mouse_;
  OIS::Keyboard* keyboard_;
  std::map<std::string, OIS::MouseListener*> mouseListeners_;
  std::map<std::string, OIS::KeyListener*> keyListeners_;
};

InputManager::InputManager() : impl_(std::make_unique<InputManagerImpl>()) {}

InputManager::~InputManager() { finalize(); }

void InputManager::init(Ogre::RenderWindow* window) { impl_->init(window); }

void InputManager::finalize() { impl_->finalize(); }

void InputManager::capture() { impl_->capture(); }

void InputManager::addKeyListener(OIS::KeyListener* keyListener, const std::string& name) {
  impl_->addKeyListener(keyListener, name);
}

void InputManager::removeKeyListener(const std::string& name) { impl_->removeKeyListener(name); }

void InputManager::addMouseListener(OIS::MouseListener* mouseListener, const std::string& name) {
  impl_->addMouseListener(mouseListener, name);
}

void InputManager::removeMouseListener(const std::string& name) {
  impl_->removeMouseListener(name);
}

void InputManager::setWindowExtents(int width, int height) {
  impl_->setWindowExtents(width, height);
}

bool InputManager::isInitialized() const noexcept { return impl_->isInitialized(); }

} // namespace game

} // namespace sequoia

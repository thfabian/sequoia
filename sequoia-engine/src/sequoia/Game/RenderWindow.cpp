//===-- sequoia/Game/RenderWindow.cpp -----------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Setup the render window.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Version.h"
#include "sequoia/Core/GlobalConfiguration.h"
#include "sequoia/Game/RenderWindow.h"
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>

namespace sequoia {

namespace game {

namespace {

bool setParam(const char* path, bool defaultValue) {
  auto value = GlobalConfiguration::getSingleton().getBoolean(path, defaultValue);
  GlobalConfiguration::getSingleton().put(path, value);
  return value;
}

int setParam(const char* path, int defaultValue) {
  auto value = GlobalConfiguration::getSingleton().getInteger(path, defaultValue);
  GlobalConfiguration::getSingleton().put(path, value);
  return value;
}

std::string setParam(const char* path, const char* defaultValue) {
  auto value = GlobalConfiguration::getSingleton().getString(path, defaultValue);
  GlobalConfiguration::getSingleton().put(path, value);
  return value;
}

}

RenderWindow::RenderWindow(const std::shared_ptr<Ogre::Root>& root) : root_(root) {}

void RenderWindow::create() {
  Ogre::String windowTitle = "Sequoia (" + core::Version::currentFullVersionString() + ")";
  
  bool fullscreen = setParam("Game.Graphics.Fullscreen", false);
  unsigned int sizeX = setParam("Game.Graphics.WindowSizeX", 1728);
  unsigned int sizeY = setParam("Game.Graphics.WindowSizeY", 972);
  
  Ogre::NameValuePairList params;
  params["FSAA"] = setParam("Game.Graphics.FSAA", "0");
  params["vsync"] = setParam("Game.Graphics.Vsync", "true");
  params["left"] = setParam("Game.Graphics.WindowPositionX", "96");
  params["top"] = setParam("Game.Graphics.WindowPositionY", "54");
  
  window_ = root_->createRenderWindow(windowTitle, sizeX, sizeY, fullscreen, &params);
}

} // namespace game

} // namespace sequoia

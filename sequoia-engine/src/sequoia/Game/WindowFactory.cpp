//===-- sequoia/Game/WindowFactory.cpp ----------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/GlobalConfiguration.h"
#include "sequoia/Core/Version.h"
#include "sequoia/Game/WindowFactory.h"
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

Ogre::RenderWindow* WindowFactory::create(const std::shared_ptr<Ogre::Root>& root) {
  Ogre::String windowTitle = "Sequoia (" + core::Version::currentFullVersionString() + ")";

  // TODO: make generic
  bool fullscreen = setParam("GraphicsSettings.Fullscreen", false);
  unsigned int sizeX = setParam("GraphicsSettings.WindowSizeX", 1728);
  unsigned int sizeY = setParam("GraphicsSettings.WindowSizeY", 972);

  Ogre::NameValuePairList params;
  params["FSAA"] = setParam("GraphicsSettings.FSAA", "0");
  params["vsync"] = setParam("GraphicsSettings.Vsync", "true");
  params["left"] = setParam("GraphicsSettings.WindowPositionX", "96");
  params["top"] = setParam("GraphicsSettings.WindowPositionY", "54");

  return root->createRenderWindow(windowTitle, sizeX, sizeY, fullscreen, &params);
}

} // namespace game

} // namespace sequoia

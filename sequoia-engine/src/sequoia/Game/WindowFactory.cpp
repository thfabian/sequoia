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

#if defined(SEQUOIA_ON_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace sequoia {

namespace game {

namespace {

void getDesktopResolution(int& sizeX, int& sizeY) {
#if defined(SEQUOIA_ON_WIN32)
  RECT desktop;
  const HWND hDesktop = GetDesktopWindow();
  GetWindowRect(hDesktop, &desktop);
  sizeX = desktop.right;
  sizeY = desktop.bottom;
#else
  // TODO
  sizeX = 1650;
  sizeY = 1050;
#endif
}

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
  auto& config = GlobalConfiguration::getSingleton();
  Ogre::String windowTitle = "Sequoia (" + core::Version::currentFullVersionString() + ")";

  bool fullscreen = setParam("GraphicsSettings.Fullscreen", false);

  // Screen resolution
  int desktopSizeX, desktopSizeY;
  getDesktopResolution(desktopSizeX, desktopSizeY);
  unsigned int sizeX = setParam("GraphicsSettings.WindowSizeX", desktopSizeX);
  unsigned int sizeY = setParam("GraphicsSettings.WindowSizeY", desktopSizeY);

  Ogre::NameValuePairList params;
  params["FSAA"] = setParam("GraphicsSettings.FSAA", "0");
  params["vsync"] = setParam("GraphicsSettings.Vsync", "true");
  params["left"] = setParam("GraphicsSettings.WindowPositionX", "0");
  params["top"] = setParam("GraphicsSettings.WindowPositionY", "0");

  // Command-line arguments take precedence
  if(config.has("CommandLine.Fullscreen"))
    fullscreen = config.getBoolean("CommandLine.Fullscreen");

  return root->createRenderWindow(windowTitle, sizeX, sizeY, fullscreen, &params);
}

} // namespace game

} // namespace sequoia

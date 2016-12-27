//===-- sequoia/Game/Game.h ---------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Main class holding all Ogre related objects.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/StringRef.h"
#include "sequoia/Game/RenderSubsystem.h"
#include <OGRE/OgreRenderSystem.h>
#include <OGRE/OgreRoot.h>
#include <string>

namespace sequoia {

namespace game {

RenderSubsystem::RenderSubsystem(const std::shared_ptr<Ogre::Root>& root)
    : root_(root), renderList_(root->getAvailableRenderers()) {}

void RenderSubsystem::create(bool showDialog, std::string preferredRenderSystem) {
  if(renderList_.size() == 0)
    core::ErrorHandler::getSingleton().fatal("No Rendering Subsystem found");

  if(showDialog)
    root_->showConfigDialog();
  else {

    StringRef prs = preferredRenderSystem;
    if(prs.empty()) {
#ifdef SEQUOIA_ON_WIN32
      // Prefer DirectX on Windows
      prs = "Direct3D11";
#else
      prs = "";
#endif
    }

    bool noRenderSystemSet = true;
    if(!prs.empty())
      for(Ogre::RenderSystem* renderSystem : renderList_) {
        if(StringRef(renderSystem->getName()).startswith(prs)) {
          root_->setRenderSystem(renderSystem);
          noRenderSystemSet = false;
          break;
        }
      }

    if(noRenderSystemSet)
      root_->setRenderSystem(renderList_[0]);
  }
}

} // namespace game

} // namespace sequoia

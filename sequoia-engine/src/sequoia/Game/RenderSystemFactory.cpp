//===-- sequoia/Game/RenderSystemFactory.cpp ----------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/StringRef.h"
#include "sequoia/Game/RenderSystemFactory.h"
#include <OGRE/OgreRenderSystem.h>
#include <OGRE/OgreRoot.h>
#include <string>

namespace sequoia {

namespace game {

Ogre::RenderSystem* RenderSystemFactory::create(const std::shared_ptr<Ogre::Root>& root,
                                                bool showDialog,
                                                std::string preferredRenderSystem) {
  auto& renderList = root->getAvailableRenderers();

  if(renderList.size() == 0)
    core::ErrorHandler::getSingleton().fatal("No Rendering Subsystem found");

  if(showDialog && root->showConfigDialog()) {
    return root->getRenderSystem();
  } else {

    StringRef prs = preferredRenderSystem;
    if(prs.empty()) {
#ifdef SEQUOIA_ON_WIN32
      // Prefer DirectX on Windows
      prs = "Direct3D11";
#else
      prs = "";
#endif
    }

    Ogre::RenderSystem* renderSystem = nullptr;
    if(!prs.empty())
      for(Ogre::RenderSystem* rs : renderList) {
        if(StringRef(rs->getName()).startswith(prs)) {
          renderSystem = rs;
          break;
        }
      }

    if(renderSystem)
      renderSystem = renderList[0];

    root->setRenderSystem(renderSystem);
    return renderSystem;
  }
}

} // namespace game

} // namespace sequoia

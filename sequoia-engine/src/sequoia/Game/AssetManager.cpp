//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/UtfString.h"
#include "sequoia/Game/AssetManager.h"

namespace sequoia {

namespace game {

//===------------------------------------------------------------------------------------------===//
//    AssetFile
//===------------------------------------------------------------------------------------------===//

AssetFile::AssetFile(std::size_t id, AssetManager* manager) : id_(id), manager_(manager) {}

StringRef AssetFile::getContent() noexcept { return manager_->getAsset(id_).Content; }

const std::string& AssetFile::getPath() const noexcept { return manager_->getPath(id_); }

std::size_t AssetFile::hash() const noexcept { return std::hash<std::size_t>()(id_); }

bool AssetFile::equals(const File& other) const noexcept {
  return id_ == static_cast<const AssetFile*>(&other)->id_;
}

//===------------------------------------------------------------------------------------------===//
//    AssetManager
//===------------------------------------------------------------------------------------------===//

AssetManager::Asset::Asset(AssetManager* manager, std::size_t id, AssetManager::AssetKind kind,
                           const std::string& path)
    : ID(id), Kind(kind), Path(path), File(std::make_shared<AssetFile>(id, manager)) {}

AssetManager::AssetManager(const platform::String& path) {
  assetPath_ = platform::Path(path) / PLATFORM_STR("assets");
}

std::shared_ptr<File> AssetManager::load(const std::string& path, AssetManager::AssetKind kind) {
  std::lock_guard<std::mutex> lock(mutex_);

  LOG(INFO) << "Loading asset \"" << path << "\" ...";

  auto it = pathLookupMap_.find(path);
  std::size_t id = std::size_t(-1);

  if(it == pathLookupMap_.end()) {
    id = assets_.size();

    // Register new asset
    assets_.emplace_back(std::make_unique<Asset>(this, id, kind, path));

    // Update lookup map
    pathLookupMap_[path] = id;

    // Load content
    loadContent(assets_.back());

  } else {
    id = it->second;
  }

  LOG(INFO) << "Successfully loaded asset \"" << path << "\"";
  return assets_[id]->File;
}

const platform::Path& AssetManager::getAssetPath() const { return assetPath_; }

const std::string& AssetManager::getPath(std::size_t id) const {
  SEQUOIA_ASSERT_MSG(id < assets_.size(), "invalid id");
  return assets_[id]->Path;
}

const AssetManager::Asset& AssetManager::getAsset(std::size_t id) const {
  SEQUOIA_ASSERT_MSG(id < assets_.size(), "invalid id");
  return *assets_[id];
}

void AssetManager::loadContent(std::unique_ptr<AssetManager::Asset>& asset) {
  std::string fullPath = platform::toAnsiString(assetPath_ / platform::asPath(asset->Path));

  std::ifstream file(fullPath.c_str());

  if(!file.is_open())
    SEQUOIA_THROW(core::Exception, "cannot load asset source: '%s'", asset->Path.c_str());

  std::stringstream ss;
  ss << file.rdbuf();
  asset->Content = ss.str();
}

} // namespace game

} // namespace sequoia
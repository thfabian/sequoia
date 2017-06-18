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

#include "sequoia/Game/AssetManager.h"
#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Memory.h"
#include "sequoia/Core/UtfString.h"
#include <fstream>

namespace sequoia {

namespace game {

//===------------------------------------------------------------------------------------------===//
//    AssetFile
//===------------------------------------------------------------------------------------------===//

AssetFile::AssetFile(std::size_t id, AssetManager* manager) : id_(id), manager_(manager) {}

const Byte* AssetFile::getData() { return manager_->getAsset(id_).Data.data(); }

std::size_t AssetFile::getNumBytes() { return manager_->getAsset(id_).Data.size(); }

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

AssetManager::Asset::~Asset() {}

AssetManager::AssetManager(const platform::String& path, const platform::String& archive) {
  assetPath_ = platform::Path(path) / archive;
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
    loadFromDisk(assets_.back());

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

void AssetManager::loadFromDisk(std::unique_ptr<AssetManager::Asset>& asset) {
  std::string fullPath = platform::toAnsiString(assetPath_ / platform::asPath(asset->Path));
  std::ifstream file(fullPath.c_str());

  if(!file.is_open())
    SEQUOIA_THROW(core::Exception, "cannot load asset source: '%s'", asset->Path.c_str());

  // Allocate memory
  file.seekg(0, std::ios_base::end);
  asset->Data.resize(file.tellg());
  file.seekg(0, std::ios_base::beg);

  // Read ASCII file
  file.read(reinterpret_cast<char*>(asset->Data.data()), asset->Data.size());
}

} // namespace game

} // namespace sequoia

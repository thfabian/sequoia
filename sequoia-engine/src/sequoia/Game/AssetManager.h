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

#ifndef SEQUOIA_GAME_ASSETMANAGER_H
#define SEQUOIA_GAME_ASSETMANAGER_H

#include "sequoia/Core/AlignedADT.h"
#include "sequoia/Core/File.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Core/Platform.h"
#include "sequoia/Game/Export.h"
#include <mutex>
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace game {

class AssetManager;

/// @brief Reference to an asset file loaded by `AssetManager`
/// @ingroup game
class SEQUOIA_GAME_API AssetFile : public File {
  /// Unique idetifier of the file
  const std::size_t id_;

  /// Refrence to the manager
  AssetManager* manager_;

public:
  friend class AssetManager;
  AssetFile(std::size_t id, AssetManager* manager);

  /// @copydoc AssetFile::getData
  const Byte* getData() override;

  /// @copydoc AssetFile::getNumBytes
  std::size_t getNumBytes() override;

  /// @copydoc AssetFile::getPath
  const std::string& getPath() const noexcept override;

  /// @copydoc AssetFile::hash
  std::size_t hash() const noexcept override;

  /// @copydoc AssetFile::equals
  bool equals(const File& other) const noexcept override;
};

/// @brief Load assets from disk
///
/// All access to the AssetManager is @b threadsafe.
///
/// @ingroup game
class SEQUOIA_GAME_API AssetManager : public NonCopyable {
public:
  /// @brief Type of asset
  enum AssetKind {
    AK_Text ///< Plain ASCII text file
  };

  /// @brief Internal asset representation
  struct Asset : public NonCopyable {
    Asset(AssetManager* manager, std::size_t id, AssetKind kind, const std::string& path);
    ~Asset();

    /// Unique idetifier of the asset/file
    std::size_t ID;

    /// Kind of asset
    AssetKind Kind;

    /// Path to the asset
    std::string Path;

    /// Content of the file
    aligned_vector<Byte> Data;

    /// Reference to the file
    std::shared_ptr<AssetFile> File;
  };

  /// @brief Initialize the manager with an archive
  ///
  /// @param path     Full path to the archive
  /// @param archive  Name of the archive file or directory
  AssetManager(const platform::String& path, const platform::String& archive);

  /// @brief Load asset from disk
  /// @threadsafe
  std::shared_ptr<File> load(const std::string& path, AssetKind kind = AK_Text);

  /// @brief Get the root path to the assets
  const platform::Path& getAssetPath() const;

  /// @brief Get the path to the file (with respect to the asset `getAssetPath()`)
  const std::string& getPath(std::size_t id) const;

  /// @brief Get the asset of `id`
  const Asset& getAsset(std::size_t id) const;

private:
  void loadFromDisk(std::unique_ptr<Asset>& asset);

private:
  /// Global access mutex
  std::mutex mutex_;

  /// Loaded assets (indexed by `id`s)
  std::vector<std::unique_ptr<Asset>> assets_;

  /// Map to of path to the index (i.e `id`) in `assets`
  std::unordered_map<std::string, std::size_t> pathLookupMap_;

  /// Full path to the assets
  platform::Path assetPath_;
};

} // namespace game

} // namespace sequoia

#endif

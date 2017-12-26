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

#ifndef SEQUOIA_ENGINE_GAME_ASSETMANAGER_H
#define SEQUOIA_ENGINE_GAME_ASSETMANAGER_H

#include "sequoia-engine/Core/AlignedADT.h"
#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/File.h"
#include "sequoia-engine/Core/Image.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Core/Platform.h"
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace game {

class AssetManager;

/// @brief Reference to an asset file loaded by `AssetManager`
/// @ingroup game
class SEQUOIA_API AssetFile : public File {
  /// Unique idetifier of the file
  const std::size_t id_;

  /// Refrence to the manager
  AssetManager* manager_;

public:
  friend class AssetManager;
  AssetFile(FileType type, std::size_t id, AssetManager* manager);

  /// @copydoc File::getData
  const Byte* getData() override;

  /// @copydoc File::getNumBytes
  std::size_t getNumBytes() override;

  /// @copydoc File::getPath
  std::string getPath() const noexcept override;

  /// @copydoc File::hash
  std::size_t hash() const noexcept override;

  /// @copydoc File::equals
  bool equals(const File* other) const noexcept override;

  /// @copydoc File::getFilename
  std::string getFilename() const noexcept override;

  /// @copydoc File::getExtension
  std::string getExtension() const noexcept override;

  /// @brief Get the `id`
  std::size_t getID() const noexcept { return id_; }
};

/// @brief Load assets from disk
///
/// All access to the AssetManager is @b threadsafe.
///
/// @ingroup game
class SEQUOIA_API AssetManager : public NonCopyable {
public:
  /// @brief Internal asset representation
  struct Asset : public NonCopyable {
    Asset(AssetManager* manager, FileType type, std::size_t id, const std::string& path);
    ~Asset();

    /// Unique idetifier of the asset/file
    std::size_t ID;

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
  AssetManager(const std::string& path);

  /// @brief Load asset from disk
  /// @remark Thread-safe
  std::shared_ptr<File> load(const std::string& path, FileType type = FileType::Unknown);

  /// @brief Load image from disk (or file)
  /// @remark Thread-safe
  /// @{
  std::shared_ptr<Image> loadImage(const std::string& path, FileType type = FileType::Unknown) {
    return loadImage(load(path, type));
  }
  std::shared_ptr<Image> loadImage(const std::shared_ptr<File>& file);
  /// @}

  /// @brief Get the root path to the assets
  const platform::Path& getAssetPath() const;

  /// @brief Get the path to the file (with respect to the asset `getAssetPath()`)
  const std::string& getPath(std::size_t id) const;

  /// @brief Get the asset of `id`
  const Asset& getAsset(std::size_t id) const;

private:
  void loadFromDisk(std::unique_ptr<Asset>& asset);

private:
  /// Loaded assets (indexed by `id`s)
  std::vector<std::unique_ptr<Asset>> assets_;

  /// Map to of path to the index (i.e `id`) in `assets`
  std::unordered_map<std::string, std::size_t> pathLookupMap_;

  /// Map of file to image
  std::unordered_map<std::shared_ptr<File>, std::shared_ptr<Image>> imageCache_;

  /// Full path to the assets
  platform::Path assetPath_;
};

} // namespace game

} // namespace sequoia

#endif

//===-- sequoia/Core/Config.h -------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT). 
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Define platform specific definitions.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_CONFIG_H
#define SEQUOIA_CORE_CONFIG_H

// Define if this is Unixish platform 
#cmakedefine SEQUOIA_ON_UNIX ${SEQUOIA_ON_UNIX}

// Define if this is Win32ish platform 
#cmakedefine SEQUOIA_ON_WIN32 ${SEQUOIA_ON_WIN32}

// Major version of Sequoia 
#define SEQUOIA_VERSION_MAJOR ${SEQUOIA_VERSION_MAJOR}

// Minor version of Sequoia 
#define SEQUOIA_VERSION_MINOR ${SEQUOIA_VERSION_MINOR}

// Patch version of Sequoia 
#define SEQUOIA_VERSION_PATCH ${SEQUOIA_VERSION_PATCH}

// Sequoia version string 
#define SEQUOIA_VERSION_STRING "${SEQUOIA_VERSION_STRING}"

// Default path for global configuration (relative) 
#define SEQUOIA_GLOBAL_CONFIG_PATH "${SEQUOIA_GLOBAL_CONFIG_PATH}"

// Default path for Ogre configuration (relative) 
#define SEQUOIA_OGRE_CONFIG_PATH "${SEQUOIA_OGRE_CONFIG_PATH}"

// Default path for Ogre plugins (relative) 
#define SEQUOIA_OGRE_PLUGIN_PATH "${SEQUOIA_OGRE_PLUGIN_PATH}"

// Available Ogre plugins 
#define SEQUOIA_OGRE_PLUGINS "${OGRE_PLUGINS}"


#endif

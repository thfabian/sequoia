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

// Define if this is an Apple platform 
#cmakedefine SEQUOIA_ON_APPLE ${SEQUOIA_ON_APPLE}

// Define if this is a Linux platform 
#cmakedefine SEQUOIA_ON_LINUX ${SEQUOIA_ON_LINUX}

// Major version of Sequoia 
#define SEQUOIA_VERSION_MAJOR ${SEQUOIA_VERSION_MAJOR}

// Minor version of Sequoia 
#define SEQUOIA_VERSION_MINOR ${SEQUOIA_VERSION_MINOR}

// Patch version of Sequoia 
#define SEQUOIA_VERSION_PATCH ${SEQUOIA_VERSION_PATCH}

// Sequoia version string 
#define SEQUOIA_VERSION_STRING "${SEQUOIA_VERSION_STRING}"

// Architecture string
#define SEQUOIA_ARCHITECTURE_STRING "${SEQUOIA_ARCHITECTURE_STRING}"

#endif

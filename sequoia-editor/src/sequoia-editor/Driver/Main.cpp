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

#include "sequoia-editor/Core/Config.h"
#include "sequoia-editor/Core/QtErrorHandler.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/PrettyStackTrace.h"
#include <QApplication>
#include <memory>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("Sequoia");
  QCoreApplication::setApplicationName("Sequoia Editor");
  QCoreApplication::setApplicationVersion(SEQUOIA_EDITOR_FULL_VERSION_STRING);

  // Create engine singletons
  sequoia::PrettyStackTrace trace;
  sequoia::Logger logger;

  // Create editor singletons
  sequoia_editor::QtErrorHandler handler;

  // TODO: this doesn't work as it doesn't show the GUI, maybe we need to call this inside the show
  // QtMainWindow?
  handler.fatal("fuck!");

  // Create mainwindow

  return app.exec();
}

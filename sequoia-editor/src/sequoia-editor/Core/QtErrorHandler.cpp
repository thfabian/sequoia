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

#include "sequoia-editor/Core/QtErrorHandler.h"
#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/ErrorHandler.h"
#include <QErrorMessage>
#include <QString>
#include <cstdlib>
#include <iostream>

namespace sequoia {
SEQUOIA_DECLARE_SINGLETON(sequoia_editor::core::QtErrorHandler);
}

namespace sequoia_editor {

namespace core {

static SEQUOIA_ATTRIBUTE_NORETURN void errorHandlerImpl(const QString& message, bool crash) {
  QErrorMessage msg;
  msg.showMessage(message);

  std::cerr << "sequoia-editor: error: " << message.toStdString() << std::endl;
  if(crash)
    std::abort();
  std::exit(EXIT_FAILURE);
}

static void errorHandler(std::string message, bool crash) {
  errorHandlerImpl(QString::fromStdString(message), crash);
}

static void errorHandlerW(std::wstring message, bool crash) {
  errorHandlerImpl(QString::fromStdWString(message), crash);
}

QtErrorHandler::QtErrorHandler() {
  sequoia::ErrorHandler::setFatalErrorHandler(errorHandler);
  sequoia::ErrorHandler::setFatalErrorHandler(errorHandlerW);
}

QtErrorHandler::~QtErrorHandler() {
  sequoia::ErrorHandler::FatalErrorHandler eh = nullptr;
  sequoia::ErrorHandler::setFatalErrorHandler(eh);

  sequoia::ErrorHandler::FatalErrorHandlerW ehW = nullptr;
  sequoia::ErrorHandler::setFatalErrorHandler(ehW);
}

void QtErrorHandler::fatal(std::string message, bool crash) {
  sequoia::ErrorHandler::fatal(message, crash);
}

void QtErrorHandler::fatal(std::wstring message, bool crash) {
  sequoia::ErrorHandler::fatal(message, crash);
}

} // namespace core

} // namespace sequoia_editor

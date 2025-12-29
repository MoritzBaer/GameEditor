#pragma once

#include "Engine/Debug/Logging.h"

inline Engine::Debug::Logging::Logger editorLogger("Editor");

#define __FILE_NAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define EDITOR_MESSAGE(format, ...)                                                                                    \
  {                                                                                                                    \
    editorLogger.PrintMessage(format " ({}({},0))", __VA_ARGS__, __FILE_NAME__, __LINE__);                             \
  }
#define EDITOR_WARNING(format, ...)                                                                                    \
  {                                                                                                                    \
    editorLogger.PrintWarning(format " ({}({},0))", __VA_ARGS__, __FILE_NAME__, __LINE__);                             \
  }
#define EDITOR_SUCCESS(format, ...)                                                                                    \
  {                                                                                                                    \
    editorLogger.PrintSuccess(format " ({}({},0))", __VA_ARGS__, __FILE_NAME__, __LINE__);                             \
  }
#define EDITOR_ERROR(format, ...)                                                                                      \
  {                                                                                                                    \
    editorLogger.PrintError(format " ({}({},0))", __VA_ARGS__, __FILE_NAME__, __LINE__);                               \
    __debugbreak();                                                                                                    \
  }
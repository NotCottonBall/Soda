#pragma once

#include "Core/App/App.h"
#include "Tools/Debug/Profiler.h"

// this is the main function in the project
// this is where the Engine starts from

extern Soda::App *Soda::CreateApp();

int main(int argc, char **argv)
{
  Soda::Log::Init();

  // creating an app and executing it
  SD_START_PROFILER("App_Start", "AppStart_Profiler.json");
  auto app = Soda::CreateApp();
  SD_STOP_PROFILER();

  SD_START_PROFILER("App_Update", "AppUpdate_Profiler.json");
  app->Run();
  SD_STOP_PROFILER();

  SD_START_PROFILER("App_Close", "AppClose_Profiler.json");
  delete app;
  SD_STOP_PROFILER();
}
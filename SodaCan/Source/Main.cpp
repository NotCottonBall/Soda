#include <print>

#include "Game/Game.h"

int main(int argc, char *argv[])
{
  Game game{};

  if(!game.Initialize("Soda Test"))
  {
    std::println("Failed To Initialize Application");
    return -1;
  }

  // Run doesn't stop until EVENT_Close occurs`
  game.Run();
  game.Quit();

  return 0;
}

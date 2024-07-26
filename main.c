#include <stdio.h>
#include "SDL.h"
#include "game/game.h"
#include "utils/utils.h"

int main(){
  
  printf("pysics engine\n");

  game main_game;
  game_init(&main_game);
  game_run(&main_game);


  print("closing physics engine");
  return 0;
}

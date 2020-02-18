#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <constant.h>
#include <map.h>
#include <window.h>

struct bomb{
	int x, y , time;
	enum etat etat;
};

void bomb_set_current_state(struct bomb *bomb , int s){
  assert(bomb);
  bomb->etat=s;
}

void start_bomb (struct map*map,struct bomb* bomb){

  int x=bomb->x;
  int y = bomb->y;

  for (int i =0 ; i< 4 ; i++){
    int current_time = SDL_GetTicks();
    int timer= current_time - bomb->time ;
    while (timer>1000){
      bomb_set_current_state(bomb, i);
      bomb->time= SDL_GetTicks();
    }

}
}






















void bomb_display(struct bomb* bomb) {
	assert(bomb);
	window_display_image(sprite_get_bomb(bomb->etat),
			bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
}

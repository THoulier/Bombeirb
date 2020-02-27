#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <player.h>
#include <constant.h>
#include <map.h>
#include <window.h>
#include <sprite.h>
#include <monster.h>
#include <bomb.h>


struct bomb* bomb_init() {
	struct bomb* bomb = malloc(sizeof(*bomb));
	bomb->etat = 3;
  bomb->time=SDL_GetTicks();
  return bomb;
}
void bomb_display(struct bomb* bomb) {
	assert(bomb);
	window_display_image(sprite_get_bomb(bomb->etat),
			bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
}

struct bomb* start_bomb(struct bomb* bomb, struct map * map , int x , int y ){
    bomb->x=x;
    bomb->y=y;
    int current_time = SDL_GetTicks();

    if ((current_time - bomb->time) > 1000 && bomb->etat>=0 ){
			bomb_display(bomb);

			window_refresh();
			//map_set_cell_type(map,x,y,CELL_EMPTY);
    	bomb->etat --;
    	bomb->time=SDL_GetTicks();

  }
    return bomb;
}

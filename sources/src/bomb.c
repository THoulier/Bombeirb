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

struct bomb *chaine_init(struct bomb * bomb, int bombs ){
while(bombs>0){
	struct bomb* new_one=malloc(sizeof(*new_one));
	new_one->next=NULL;
	new_one->clic=0;
	if (bomb==NULL)
	bomb=new_one;
	else {
		struct bomb *tmp =bomb;
		while(tmp->next!=NULL)
			tmp=tmp->next;
	new_one->prev=tmp;
	tmp->next=new_one;

}
bombs--;
}
return bomb;
}






struct bomb* start_bomb(struct bomb* bomb, struct map * map ){

    int current_time = SDL_GetTicks();

    if ((current_time - bomb->time) > 1000 && bomb->etat>=0 ){
		//	bomb_display(bomb);

			//window_refresh();
			//map_set_cell_type(map,x,y,CELL_EMPTY);
    	bomb->etat --;
    	bomb->time=current_time;

  }
    return bomb;
}

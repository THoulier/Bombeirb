
#include <bomb.h>


enum compose_type get_compose_type();
void box_explo(struct map * map, struct bomb * bomb, struct player * player);

enum bonus_type get_bonus_type(enum compose_type box);


void bonus_config(struct map *map,int x, int y);



void display_bonus_explo(struct map *map, int x, int y);

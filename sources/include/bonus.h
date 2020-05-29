#include <bomb.h>

//get the type of a box
enum compose_type get_compose_type();
enum bonus_type get_bonus_type(enum compose_type box);

//manage the explosion of a box 
void box_explo(struct map * map, struct bomb * bomb, struct player * player);
void bonus_assign_box(struct map *map, int x, int y,int nummap);





void save_monster(int x,int y,enum direction direction,int nummap);
void save_nummonster(int nummonster);
void save_numbomb(int numbomb);
void save_bomb(int x,int y,enum etats etat,int nummap, int range);

void save_player(int x,int y, int lives, enum direction direction, int level, int bombrange, int key, int bombs, int contact, int dmg_tmp);
void load_listmonster();
void load_listbomb();
void load_player(struct player * player);
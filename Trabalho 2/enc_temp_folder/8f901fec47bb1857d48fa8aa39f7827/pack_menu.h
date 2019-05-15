#ifndef PACK_MENU_H
#define PACK_MENU_H

#include "Travelpack.h"

void pack_menu();
void pack_submenu(Travelpack *ptr);
void show_all_packs();
void show_profits();
void show_n_most_visited();
void show_sold_packs(); //needs fixing to find correct client association to packs

#endif

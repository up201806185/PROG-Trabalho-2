#ifndef CLIENT_MENU_H
#define CLIENT_MENU_H

#include "Client.h"
void client_menu();
void client_submenu(Client* ptr);
void show_all_recommendations();
void make_purchase(Client* selected_client);

#endif 
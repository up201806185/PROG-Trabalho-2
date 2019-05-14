#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Client.h"
#include "Travelpack.h"
#include "Agency.h"
#include "utils.h"

void main_menu(Agency agency, std::string agency_path);
void save_menu(Agency & agency, std::string agency_path);

#endif 
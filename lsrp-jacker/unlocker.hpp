#pragma once

void CALLBACK cmd_open_vehicle(std::string param);
void CALLBACK cmd_noeject(std::string param);
void CALLBACK cmd_breakin(std::string param);
bool CALLBACK hook_do_not_remove(stRakNetHookParams* params);
#include "lsrp-jacker.hpp"

bool noeject_enabled = FALSE;

void CALLBACK cmd_noeject(std::string params)
{
	noeject_enabled = !noeject_enabled;

	if (noeject_enabled)
		pprintf("{00FF00}RemovePlayerFromVehicle NOP'd. You will not be ejected.");
	else
		pprintf("{FF0000}RemovePlayerFromVehicle NOP turned off. You will be ejected.");
}

int get_closet_vehicle_to_player(stVehiclePool *vehs)
{
	// Temporary position (loop), player position.
	CVector tpos, ppos;

	// Vehicle data structures.
	vehicle_info *veh = NULL;

	// Initialize some values for the distance loop.
	float distance = INFINITE;
	int closest_car = -1;

	// Get the player's current position.
	actor_info *me = SF->getSAMP()->getPlayers()->pLocalPlayer->pSAMP_Actor->pGTA_Ped;
	ppos.fX = me->base.matrix[4 * 3];
	ppos.fY = me->base.matrix[(4 * 3) + 1];
	ppos.fZ = me->base.matrix[(4 * 3) + 2];

	// Get the vehicle that's closest to the player.
	for (int i = 0; i <= vehs->iVehicleCount; i++) {
		veh = vehs->pGTA_Vehicle[i];

		if (veh) {
			tpos.fX = veh->base.matrix[4 * 3];
			tpos.fY = veh->base.matrix[(4 * 3) + 1];
			tpos.fZ = veh->base.matrix[(4 * 3) + 2];

			if ((tpos - ppos).Length() < distance) {
				closest_car = i;
				distance = (tpos - ppos).Length();
			}
		}
	}

	if (distance > 10.0)
		return -1;

	return closest_car;
}

void CALLBACK cmd_open_vehicle(std::string param)
{
	// Vehicle data structures.
	stVehiclePool *vehs = SF->getSAMP()->getVehicles();
	int closest_car = get_closet_vehicle_to_player(vehs);
	
	//Unlock the vehicle (using opcode #020A is less idiomatic).
	if (closest_car != -1) {
		vehs->pSAMP_Vehicle[closest_car]->iIsLocked = 0;
		vehs->pGTA_Vehicle[closest_car]->door_status = 1;

		pprintf("{00FF00}Car #%d (%d) unlocked.", closest_car, vehs->GetCarHandleFromSAMPCarID(closest_car));
	}
	else {
		pprintf("{FF0000}There are no cars nearby.");
	}
}

bool CALLBACK hook_do_not_remove(stRakNetHookParams* params)
{
	if (params->packetId == RPC_ScrRemovePlayerFromVehicle && noeject_enabled) {
		return false;
	}
	return true;
}
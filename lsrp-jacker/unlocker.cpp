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

void CALLBACK cmd_open_vehicle(std::string param)
{
	// Temporary position (loop), player position.
	CVector tpos, ppos;

	// Initialize some values for the distance loop.
	float distance = INFINITE;
	int closest_car = -1;

	// Vehicle data structures.
	stVehiclePool *vehs = SF->getSAMP()->getVehicles();
	vehicle_info *veh = NULL;

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
		return pprintf("{FF0000}You are not near a vehicle.");

	//Unlock the vehicle (using opcode #020A is less idiomatic).
	vehs->pSAMP_Vehicle[closest_car]->iIsLocked = 0;
	vehs->pGTA_Vehicle[closest_car]->door_status = 1;

	pprintf("{00FF00}Car #%d (%d) unlocked.", closest_car, vehs->GetCarHandleFromSAMPCarID(closest_car));
}

bool CALLBACK hook_do_not_remove(stRakNetHookParams* params)
{
	if (params->packetId == RPC_ScrRemovePlayerFromVehicle && noeject_enabled) {
		return false;
	}
	return true;
}
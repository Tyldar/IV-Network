/*
* Copyright (C) GTA-Network Team
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*
*     * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above
* copyright notice, this list of conditions and the following disclaimer
* in the documentation and/or other materials provided with the
* distribution.
*     * Neither the name of GTA-Network nor the names of its
* contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* INCLUDING NEGLIGENCE OR OTHERWISE ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef RPCIdentifier_h
#define RPCIdentifier_h

// Type of RPC
typedef unsigned short RPCIdentifier;

// RPCs definitions
enum eRPCIdentifier : RPCIdentifier
{
	RPC_INITIAL_DATA,
	RPC_DOWNLOAD_START,
	RPC_DOWNLOAD_FINISH,

	RPC_NEW_PLAYER,
	RPC_DELETE_PLAYER,
	RPC_SYNC_PACKAGE,

	RPC_PLAYER_CHAT,
	RPC_PLAYER_DEATH,
	RPC_ENTER_VEHICLE,
	RPC_EXIT_VEHICLE,
	RPC_CREATE_VEHICLE,
	RPC_DELETE_VEHICLE,
	RPC_PLAYER_REQUEST_SPAWN,
	
	RPC_GET_VEHICLE,
	RPC_GET_PLAYER,

	RPC_PLAYER_VEHICLE_ENTER_EXIT,

	RPC_PLAYER_SET_HUD_VISIBLE,
	RPC_PLAYER_NAME_CHANGE,
	RPC_PLAYER_SET_POSITION,
	RPC_PLAYER_SET_ROTATION,
	RPC_PLAYER_SET_HEADING,
	RPC_PLAYER_SET_MOVE_SPEED,
	RPC_PLAYER_SET_TURN_SPEED,
	RPC_PLAYER_SET_HEALTH,
	RPC_PLAYER_SET_ARMOUR,
	RPC_PLAYER_SET_MODEL,
	RPC_PLAYER_SET_CLOTHES,
	RPC_PLAYER_SET_MONEY,
	RPC_PLAYER_SET_DIMENSION,
	RPC_PLAYER_SET_WANTED_LEVEL,
	RPC_PLAYER_GIVE_WEAPON,
	RPC_PLAYER_GIVE_MONEY,
	RPC_PLAYER_SET_COLOR,
	RPC_PLAYER_MESSAGE,
	RPC_PLAYER_MESSAGE_TO_ALL,
	RPC_PLAYER_SPAWN,
	RPC_PLAYER_TRIGGER_EVENT,

	RPC_VEHICLE_SET_POSITION,
	RPC_VEHICLE_SET_ROTATION,
	RPC_VEHICLE_SET_MOVE_SPEED,
	RPC_VEHICLE_SET_TURN_SPEED,
	RPC_VEHICLE_SET_HEALTH,
	RPC_VEHICLE_SET_LOCKED,
	RPC_VEHICLE_SET_ENGINE,
	RPC_VEHICLE_SET_SIREN,
	RPC_VEHICLE_SET_LIGHTS,
	RPC_VEHICLE_SET_TAXI_LIGHTS,
	RPC_VEHICLE_SET_DIRT_LEVEL,

	RPC_CREATE_CHECKPOINT,
	RPC_CHECKPOINT_SET_POSITION,
	RPC_CHECKPOINT_SET_TARGET_POSITION,
	RPC_CHECKPOINT_SHOW,
	RPC_CHECKPOINT_HIDE,
	RPC_CHECKPOINT_SET_TYPE,
	RPC_CHECKPOINT_SET_RADIUS,
	RPC_ENTER_CHECKPOINT,
	RPC_EXIT_CHECKPOINT,

	RPC_CREATE_BLIP,
	RPC_BLIP_SET_POSITION,
	RPC_BLIP_SET_ICON,
	RPC_BLIP_SET_COLOR,
	RPC_BLIP_SET_SIZE,
	RPC_BLIP_SET_RANGE,
	RPC_BLIP_SET_VISIBLE,
	RPC_BLIP_SET_NAME,
};

#endif // RPCIdentifier_h
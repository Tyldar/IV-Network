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

#include "CNetworkRPC.h"

#include "CNetworkModule.h"

#include <CLogFile.h>
#include <CServer.h>
#include <Scripting/CEvents.h>
#include <CSettings.h>
#include <time.h>
#include <Entity/CPlayerEntity.h>

#ifndef _WIN32
typedef unsigned char byte;
#endif

#define GET_RPC_CODEX(x) CString("IVN0xF%dF", int(x)).Get()

extern CServer * g_pServer;

bool	CNetworkRPC::m_bRegistered = false;
RakNet::BitStream		bsReject;

void InitialData(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	// Get the playerid
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the player version
	DWORD dwVersion;
	pBitStream->Read(dwVersion);

	// Is the network version invalid?

	RakNet::BitStream bitStream;

	if (dwVersion != NETWORK_VERSION)
	{
		bitStream.Write(REASON_BAD_VERSION);
		CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_PLAYER_KICK_NOTIFICATION), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false);
		CServer::GetInstance()->GetNetworkModule()->KickPlayer(playerId, false);
		return;
	}

	// Send only the resources that have client scripts
	for (auto pResource : CServer::GetInstance()->GetResourceManager()->GetResources())
	{
		if (pResource->HasClientResourceFilesScripts())
			bitStream.Write(RakNet::RakString(pResource->GetName().C_String()));
	}
	CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_DOWNLOAD_START), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false);
}

void DownloadFinished(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{

	// Read the player name
	RakNet::RakString _strName;
	pBitStream->Read(_strName);
	CString strName(_strName.C_String());

	// Read the player serial
	RakNet::RakString _strSerial;
	pBitStream->Read(_strSerial);
	CString strSerial(_strSerial.C_String());

	// Construct a new bitstream	
	RakNet::BitStream bitStream;

	// Is the nickname already in use?
	if (!CVAR_GET_BOOL("namesakes"))
	{
		for (EntityId i = 0; i < CServer::GetInstance()->GetPlayerManager()->GetMax(); ++i)
		{
			if (CServer::GetInstance()->GetPlayerManager()->DoesExists(i))
			{
				if (!strcmp(strName, CServer::GetInstance()->GetPlayerManager()->GetAt(i)->GetName().Get()))
				{
					bitStream.Write(REASON_NAME_IS_USED);
					CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_PLAYER_KICK_NOTIFICATION), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, pPacket->guid.systemIndex, false);
					CServer::GetInstance()->GetNetworkModule()->KickPlayer(pPacket->guid.systemIndex, false);
					return;
				}
			}
		}
	}

	// Is the player banned?
	// TODO: check is banned

	// Add the player to the manager
	// TODO: add to player manager
	CPlayerEntity * pPlayer = new CPlayerEntity();
	pPlayer->SetName(strName);
	pPlayer->SetSerial(strSerial.Get());

	// Do we need the id; maybe internal for easier sync but definetly not public to the scripting engine
	pPlayer->SetId(CServer::GetInstance()->GetPlayerManager()->Add(pPlayer));
	EntityId playerId = pPlayer->GetId();
	srand(time(NULL));
	pPlayer->SetColor(CColor(rand() % 256, rand() % 256, rand() % 256).dwHexColor); //generate random color

	CLogFile::Printf("[join] %s (%i) has connected to the server. (%s)", strName.Get(), playerId, strSerial.Get());

	CScriptArguments args;
	CScriptPlayer * pScriptPlayer = new CScriptPlayer();
	pScriptPlayer->SetEntity(pPlayer);
	pPlayer->SetScriptPlayer(pScriptPlayer);
	args.push(pScriptPlayer);
	CEvents::GetInstance()->Call("playerJoin", &args, CEventHandler::eEventType::NATIVE_EVENT, 0);

	// Write the player id
	bitStream.Write(playerId);

	// Write the player colour
	bitStream.Write(pPlayer->GetColor());

	// Write the server name
	bitStream.Write(RakNet::RakString(CVAR_GET_STRING("hostname")));

	// Write the max player count
	bitStream.Write(CVAR_GET_INTEGER("maxplayers"));

	// Write the port
	bitStream.Write(CVAR_GET_INTEGER("port"));

	// Send it back to the player
	CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_INITIAL_DATA), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false);
	
	bitStream.Reset();

	for (EntityId i = 0; i < CServer::GetInstance()->GetPlayerManager()->GetMax(); ++i)
	{
		if (CServer::GetInstance()->GetPlayerManager()->DoesExists(i) && i != playerId)
		{
			bitStream.Write(i);
			bitStream.Write(CServer::GetInstance()->GetPlayerManager()->GetAt(i)->GetName().Get());
			bitStream.Write(CServer::GetInstance()->GetPlayerManager()->GetAt(i)->GetColor());
			bitStream.Write(CServer::GetInstance()->GetPlayerManager()->GetAt(i)->GetModel());
			//for (unsigned char c = 0; c < 11; c++) bitStream.Write(CServer::GetInstance()->GetPlayerManager()->GetAt(i)->GetClothes(c));
		}
	}

	if (bitStream.GetNumberOfBitsUsed()) CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_GET_PLAYERS), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false);
	

	bitStream.Reset();
	bitStream.Write(playerId);
	bitStream.Write(pPlayer->GetName().Get());
	bitStream.Write(pPlayer->GetColor());

	for (EntityId i = 0; i < CServer::GetInstance()->GetPlayerManager()->GetMax(); ++i)
	{
		if (CServer::GetInstance()->GetPlayerManager()->DoesExists(i) && i != playerId)
		{
			CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_NEW_PLAYER), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, i, false);
		}
	}

	bitStream.Reset();

	CVector3 vecPosition;
	CVector3 vecTargetPosition;

	for (EntityId i = 0; i < CServer::GetInstance()->GetCheckpointManager()->GetMax(); ++i)
	{
		if (CServer::GetInstance()->GetCheckpointManager()->DoesExists(i))
		{			
			CCheckpointEntity * pCheckpoint = CServer::GetInstance()->GetCheckpointManager()->GetAt(i);

			if (pCheckpoint->GetVisible())
			{
				bitStream.Write(pCheckpoint->GetId());
				bitStream.Write(pCheckpoint->GetType());

				pCheckpoint->GetPosition(vecPosition);
				bitStream.Write(vecPosition);
				
				pCheckpoint->GetTargetPosition(vecTargetPosition);
				bitStream.Write(vecTargetPosition);

				bitStream.Write(pCheckpoint->GetRadius());				
			}
		}
	}

	if (bitStream.GetNumberOfBitsUsed()) CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_GET_CHECKPOINTS), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false);

	bitStream.Reset();

	CBlipEntity * pBlip;
	
	for (EntityId i = 0; i < CServer::GetInstance()->GetBlipManager()->GetMax(); ++i)
	{
		if (CServer::GetInstance()->GetBlipManager()->DoesExists(i))
		{
			pBlip = CServer::GetInstance()->GetBlipManager()->GetAt(i);

			if (pBlip->GetVisible())
			{
				bitStream.Write(pBlip->GetId());
				bitStream.Write(pBlip->GetIcon());

				pBlip->GetPosition(vecPosition);
				bitStream.Write(vecPosition);

				bitStream.Write(pBlip->GetRange());				
			}
		}
	}

	if (bitStream.GetNumberOfBitsUsed()) CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_GET_BLIPS), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false);

	CVehicleEntity * pVehicle;
	CVector3 vecRotation;

	for (EntityId i = 0; i < CServer::GetInstance()->GetVehicleManager()->GetMax(); ++i)
	{
		if (CServer::GetInstance()->GetVehicleManager()->DoesExists(i))
		{
			pVehicle = CServer::GetInstance()->GetVehicleManager()->GetAt(i);
			bitStream.Write(pVehicle->GetId());
			bitStream.Write(pVehicle->GetModelId());

			pVehicle->GetPosition(vecPosition);
			bitStream.Write(vecPosition);

			pVehicle->GetRotation(vecRotation);
			bitStream.Write(vecRotation);

			bitStream.Write(pVehicle->GetHealth());
			bitStream.Write(pVehicle->GetEngineState());
			bitStream.Write(pVehicle->GetLightsState());
			bitStream.Write(pVehicle->GetSirenState());
			bitStream.Write(pVehicle->GetTaxiLightsState());

			bitStream.Write(pVehicle->GetColor(1));
			bitStream.Write(pVehicle->GetColor(2));
			bitStream.Write(pVehicle->GetColor(3));
			bitStream.Write(pVehicle->GetColor(4));
			bitStream.Write(pVehicle->GetColor(5));
		}
	}
	if (bitStream.GetNumberOfBitsUsed()) CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_GET_VEHICLES), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false);

}

void DownloadResourceFiles(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	// Get the playerid
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the input
	RakNet::RakString resourceName;
	pBitStream->Read(resourceName);

	CResource* resource = CServer::GetInstance()->GetResourceManager()->GetResource(resourceName.C_String());

	RakNet::BitStream pBitStreamList;
	pBitStreamList.Write(resourceName);
	CServer::GetInstance()->GetResourceManager()->GetResourceFileList(resource)->Serialize(&pBitStreamList);

	CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_DOWNLOAD_RESOURCE_FILES), &pBitStreamList, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false);
}

void PlayerChat(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	// Get the playerid
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read if this is a command
	bool bIsCommand = pBitStream->ReadBit();

	// Read the input
	RakNet::RakString strInput;
	pBitStream->Read(strInput);

	// Is the player active?
	if (CServer::GetInstance()->GetPlayerManager()->DoesExists(playerId))
	{
		// Get a pointer to the player
		CPlayerEntity * pPlayer = CServer::GetInstance()->GetPlayerManager()->GetAt(playerId);

		// Is the pointer valid?
		if (pPlayer)
		{
			CLogFile::Printf((!bIsCommand) ? ("[chat] %s: %s") : ("[command] %s: %s"), pPlayer->GetName().Get(), strInput.C_String());
			CScriptArguments args;
			args.push(strInput);
			args.push(pPlayer->GetScriptPlayer());
			if (bIsCommand) {
				CString cmdName = strInput.C_String();
				size_t sSplit = cmdName.Find(' ', 0);
				cmdName = cmdName.Substring(0, sSplit++);
				cmdName.Format("CMD_%s", cmdName.C_String());
				CEvents::GetInstance()->Call((!CEvents::GetInstance()->IsEventRegistered(cmdName.C_String())) ? ("playerCommand") : cmdName.C_String(), &args, CEventHandler::eEventType::NATIVE_EVENT, nullptr);
			}
			else
			{	
				if (!CEvents::GetInstance()->IsEventRegistered("playerChat")) 
				{ 
					// Send the RPC back to other players
					RakNet::BitStream bitStream;
					bitStream.Write(playerId);
					bitStream.Write(strInput);
					CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_PLAYER_CHAT), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, -1, true);
				}
				else
				{		
					auto arguments = CEvents::GetInstance()->Call("playerChat", &args, CEventHandler::eEventType::NATIVE_EVENT, nullptr);
					for (auto argument : arguments.m_Arguments)
					{
						if (argument->GetType() == CScriptArgument::ArgumentType::ST_INTEGER)
						{
							if (argument->GetInteger() == 1)
							{
								return;
							}
						}
						else if (argument->GetType() == CScriptArgument::ArgumentType::ST_BOOL)
						{
							if (argument->GetBool() == true)
							{
								return;
							}
						}
					}
					RakNet::BitStream bitStream;
					bitStream.Write(playerId);
					bitStream.Write(strInput);
					CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_PLAYER_CHAT), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, -1, true);
				}
			}
		}
	}
}

void PlayerSync(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Is the player active?
	if (CServer::GetInstance()->GetPlayerManager()->DoesExists(playerId))
	{
		// Deserialse the bitstream with the player
		ePackageType eType;
		while (pBitStream->Read(eType))
		{
			switch (eType)
			{
			case RPC_PACKAGE_TYPE_PLAYER_ONFOOT:
			case RPC_PACKAGE_TYPE_PLAYER_WEAPON:
			{
												   CServer::GetInstance()->GetPlayerManager()->GetAt(playerId)->Deserialize(pBitStream, eType);

												   // TODO: dont send the sync directly store (queue) it make some interpolation or somthing and then send it to get a nice and smooth sync
												   break;
			}
			case RPC_PACKAGE_TYPE_PLAYER_PASSENGER:
			case RPC_PACKAGE_TYPE_PLAYER_VEHICLE:
			{
													CServer::GetInstance()->GetPlayerManager()->GetAt(playerId)->Deserialize(pBitStream, eType);
													break;
			}
			}
		}
	}
}

void PlayerDeath(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	CLogFile::Printf("%s", __FUNCTION__);

	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the killer id
	EntityId killerId;
	pBitStream->Read(killerId);

	// Get a pointer to the player
	CPlayerEntity * pPlayer = CServer::GetInstance()->GetPlayerManager()->GetAt(playerId);

	// Is the player pointer valid?
	if (pPlayer)
	{
		// Kill the player
		pPlayer->GetScriptPlayer()->Kill();

		CScriptArguments args;
		args.push(pPlayer->GetScriptPlayer());
		// Is the killer a player?
		if (killerId != INVALID_ENTITY_ID)
		{
			// Find the killer
			CPlayerEntity* pKiller = CServer::GetInstance()->GetPlayerManager()->GetAt(killerId);

			// Is the killer valid?
			if (pKiller) 
			{
				args.push(1);
				args.push(pKiller->GetScriptPlayer());
			}
			else
			{
				args.push(2);
				args.push(-1);
			}
		}
		else
		{
			args.push(0);
			args.push(-1);

		}
		CEvents::GetInstance()->Call("playerDeath", &args, CEventHandler::eEventType::NATIVE_EVENT, 0);
	}
}

void PlayerRequestSpawn(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	CLogFile::Printf("%s", __FUNCTION__);

	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Get a pointer to the player
	CPlayerEntity * pPlayer = CServer::GetInstance()->GetPlayerManager()->GetAt(playerId);

	// Is the player pointer valid?
	if (pPlayer)
	{
		if (CEvents::GetInstance()->IsEventRegistered("playerRequestSpawn"))
		{
			CScriptArguments args;
			args.push(pPlayer->GetScriptPlayer());
			CEvents::GetInstance()->Call("playerRequestSpawn", &args, CEventHandler::eEventType::NATIVE_EVENT, 0);
		}
		else
		{
			RakNet::BitStream bitStream;
			bitStream.Write(CVector3(DEFAULT_SPAWN_POSITION)); //spawnPos
			bitStream.Write(0.0f); //fHeading
			CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_PLAYER_SPAWN), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false);
		}
		CLogFile::Printf("[spawn] %s has requestred a spawn.", pPlayer->GetName().Get());
	}
}

void PlayerChangeName(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	// Get the playerid
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the input
	RakNet::RakString newNick;
	pBitStream->Read(newNick);

	// Is the player active?
	if (CServer::GetInstance()->GetPlayerManager()->DoesExists(playerId))
	{
		// Get a pointer to the player
		CPlayerEntity * pPlayer = CServer::GetInstance()->GetPlayerManager()->GetAt(playerId);

		// Is the pointer valid?
		if (pPlayer)
		{
			// Change the nick of the player
			pPlayer->SetName(CString(newNick.C_String()));

			// Send the info to the other players
			RakNet::BitStream bitStream;
			bitStream.Write(playerId);
			bitStream.Write(newNick);
			CServer::GetInstance()->GetNetworkModule()->Call(GET_RPC_CODEX(RPC_PLAYER_NAME_CHANGE), &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, true);
		}
	}
}

void TriggerServerEvent(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	RakNet::RakString eventName;
	pBitStream->Read(eventName);

	// Get the player instance
	CPlayerEntity * pPlayer = CServer::GetInstance()->GetPlayerManager()->GetAt(playerId);

	// Is the player instance valid?
	if (pPlayer)
	{
		CScriptArguments args;
		// With this line does the event not works
		//args.push(pPlayer->GetScriptPlayer());
		CEvents::GetInstance()->Call(CString(eventName.C_String()), &args, CEventHandler::REMOTE_EVENT, nullptr);
	}
}

void CheckpointEnter(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the checkpoint id
	EntityId checkpointId;
	pBitStream->Read(checkpointId);

	// Get the player instance
	CPlayerEntity * pPlayer = CServer::GetInstance()->GetPlayerManager()->GetAt(playerId);

	// Get the checkpoint instance
	CCheckpointEntity * pCheckpoint = CServer::GetInstance()->GetCheckpointManager()->GetAt(checkpointId);

	// Is the player instance valid?
	if (pPlayer)
	{
		CScriptArguments args;
		args.push(pPlayer->GetScriptPlayer());
		args.push(pCheckpoint->GetScriptCheckpoint());
		CEvents::GetInstance()->Call("playerEnterCheckpoint", &args, CEventHandler::eEventType::NATIVE_EVENT, 0);
	}

	CLogFile::Printf("RPC_ENTER_CHECKPOINT");
}

void CheckpointExit(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	// Get the player id
	EntityId playerId = (EntityId)pPacket->guid.systemIndex;

	// Read the checkpoint id
	EntityId checkpointId;
	pBitStream->Read(checkpointId);

	// Get the player instance
	CPlayerEntity * pPlayer = CServer::GetInstance()->GetPlayerManager()->GetAt(playerId);

	// Get the checkpoint instance
	CCheckpointEntity * pCheckpoint = CServer::GetInstance()->GetCheckpointManager()->GetAt(checkpointId);

	// Is the player instance valid?
	if (pPlayer)
	{
		CScriptArguments args;
		args.push(pPlayer->GetScriptPlayer());
		args.push(pCheckpoint->GetScriptCheckpoint());
		CEvents::GetInstance()->Call("playerExitCheckpoint", &args, CEventHandler::eEventType::NATIVE_EVENT, 0);
	}

	CLogFile::Printf("RPC_EXIT_CHECKPOINT");
}

void CNetworkRPC::Register(RakNet::RPC4 * pRPC)
{
	// Are we already registered?
	if (m_bRegistered)
		return;

	// Default rpcs
	pRPC->RegisterFunction(GET_RPC_CODEX(RPC_INITIAL_DATA), InitialData);
	pRPC->RegisterFunction(GET_RPC_CODEX(RPC_DOWNLOAD_FINISH), DownloadFinished);

	// Resource RCPs
	pRPC->RegisterFunction(GET_RPC_CODEX(RPC_DOWNLOAD_RESOURCE_FILES), DownloadResourceFiles);

	// Player rpcs
	pRPC->RegisterFunction(GET_RPC_CODEX(RPC_PLAYER_CHAT), PlayerChat);
	pRPC->RegisterFunction(GET_RPC_CODEX(RPC_SYNC_PACKAGE), PlayerSync);
	pRPC->RegisterFunction(GET_RPC_CODEX(RPC_PLAYER_DEATH), PlayerDeath);
	pRPC->RegisterFunction(GET_RPC_CODEX(RPC_PLAYER_REQUEST_SPAWN), PlayerRequestSpawn);
	pRPC->RegisterFunction(GET_RPC_CODEX(RPC_PLAYER_NAME_CHANGE), PlayerChangeName);
	pRPC->RegisterFunction(GET_RPC_CODEX(RPC_PLAYER_TRIGGER_EVENT), TriggerServerEvent);

	// Checkpoint rpcs
	pRPC->RegisterFunction(GET_RPC_CODEX(RPC_ENTER_CHECKPOINT), CheckpointEnter);
	pRPC->RegisterFunction(GET_RPC_CODEX(RPC_EXIT_CHECKPOINT), CheckpointExit);
}

void CNetworkRPC::Unregister(RakNet::RPC4 * pRPC)
{
	// Are we not registered?
	if (!m_bRegistered)
		return;

	// Default rpcs
	pRPC->UnregisterFunction(GET_RPC_CODEX(RPC_INITIAL_DATA));
	pRPC->UnregisterFunction(GET_RPC_CODEX(RPC_DOWNLOAD_FINISH));

	// Resource RCPs
	pRPC->UnregisterFunction(GET_RPC_CODEX(RPC_DOWNLOAD_RESOURCE_FILES));

	// Player rpcs
	pRPC->UnregisterFunction(GET_RPC_CODEX(RPC_PLAYER_CHAT));
	pRPC->UnregisterFunction(GET_RPC_CODEX(RPC_SYNC_PACKAGE));
	pRPC->UnregisterFunction(GET_RPC_CODEX(RPC_PLAYER_DEATH));
	pRPC->UnregisterFunction(GET_RPC_CODEX(RPC_PLAYER_REQUEST_SPAWN));
	pRPC->UnregisterFunction(GET_RPC_CODEX(RPC_PLAYER_NAME_CHANGE));

	// Checkpoint rpcs
	pRPC->UnregisterFunction(GET_RPC_CODEX(RPC_ENTER_CHECKPOINT));
	pRPC->UnregisterFunction(GET_RPC_CODEX(RPC_EXIT_CHECKPOINT));
}
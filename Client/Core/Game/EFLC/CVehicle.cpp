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

#include "CVehicle.h"
#include <Game/COffsets.h>
#include <CCore.h>
extern CCore * g_pCore;

_GAME_BEGIN

CVehicle::CVehicle() :
	CPhysical()
{

}

CVehicle::CVehicle(IVehicle * pVehicle) :
	CPhysical(pVehicle)
{

}

CVehicle::~CVehicle()
{

}

void CVehicle::SetVehicle(IVehicle * pVehicle)
{
	SetPhysical(pVehicle);
}

IVehicle * CVehicle::GetVehicle()
{
	return (IVehicle *)GetPhysical();
}

void CVehicle::SetLoadCollision(bool bLoadCollision)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		bool _bLoadCollision = bLoadCollision;

		_asm	push _bLoadCollision;
		_asm	push pVehicle;
		_asm	call COffsets::FUNC_CVehicle__SetLoadCollision;
		_asm	add esp, 8;
	}
}

void CVehicle::SetAlarmDuration(WORD wDuration)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_wAlarm = wDuration;
}

WORD CVehicle::GetAlarmDuration()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_wAlarm;

	return 0;
}

void CVehicle::SetDriver(IPed * pPed)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_pDriver = pPed;
}

IPed * CVehicle::GetDriver()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_pDriver;

	return nullptr;
}

void CVehicle::SetPassenger(BYTE bytePassengerSeatId, IPed * pPassenger)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bytePassengerSeatId < 8)
			pVehicle->m_pPassengers[bytePassengerSeatId] = pPassenger;
	}
}

IPed * CVehicle::GetPassenger(BYTE bytePassengerSeatId)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bytePassengerSeatId < 8)
			return pVehicle->m_pPassengers[bytePassengerSeatId];
	}

	return nullptr;
}

void CVehicle::SetColors(unsigned char ucColor1, unsigned char ucColor2, unsigned char ucColor3, unsigned char ucColor4)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		pVehicle->m_byteColors[0] = ucColor1;
		pVehicle->m_byteColors[1] = ucColor2;
		pVehicle->m_byteColors[2] = ucColor3;
		pVehicle->m_byteColors[3] = ucColor4;

		_asm	mov ecx, pVehicle;
		_asm	call COffsets::FUNC_CVehicle__RefreshColours;
	}
}
void CVehicle::GetColors(unsigned char * ucColor1, unsigned char * ucColor2, unsigned char * ucColor3, unsigned char * ucColor4)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		*ucColor1 = pVehicle->m_byteColors[0];
		*ucColor2 = pVehicle->m_byteColors[1];
		*ucColor3 = pVehicle->m_byteColors[2];
		*ucColor4 = pVehicle->m_byteColors[3];
	}
}

void CVehicle::SetMaxPassengers(BYTE byteMaxPassengers)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_byteMaxPassengers = byteMaxPassengers;
}

BYTE CVehicle::GetMaxPasssengers()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_byteMaxPassengers;

	return 0;
}

void CVehicle::SetSirenState(bool bStatus)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		EFLC::CScript::SwitchCarSiren(g_pCore->GetGame()->GetPools()->GetVehiclePool()->HandleOf(pVehicle), bStatus);
	}
}

bool CVehicle::GetSirenState()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		return EFLC::CScript::IsCarSirenOn(g_pCore->GetGame()->GetPools()->GetVehiclePool()->HandleOf(pVehicle));
	}

	return false;
}

void CVehicle::SetGasPedal(float fGasPedal)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fGasPedal = fGasPedal;
}

float CVehicle::GetGasPedal()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fGasPedal;

	return 0.0f;
}

void CVehicle::SetBrakePedal(float fBrakePedal)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fBrakePedal = fBrakePedal;
}

float CVehicle::GetBrakePedal()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fBrakePedal;

	return 0.0f;
}

void CVehicle::SetSteerBias(float fSteerBias)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fSteerBias = fSteerBias;
}

float CVehicle::GetSteerBias()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fSteerBias;

	return 0.0f;
}

void CVehicle::SetSteeringAngle(float fSteeringAngle)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fSteering = fSteeringAngle;
}

float CVehicle::GetSteeringAngle()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fSteering;

	return 0.0f;
}

void CVehicle::SetEngineHealth(float fEngineHealth)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fEngineHealth = fEngineHealth;
}

float CVehicle::GetEngineHealth()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fEngineHealth;

	return 0.0f;
}

void CVehicle::SetDirtLevel(float fDirtLevel)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fDirtLevel = Math::Clamp(0.0f, fDirtLevel, 15.0f);
}

float CVehicle::GetDirtLevel()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fDirtLevel;

	return 0.0f;
}

void CVehicle::SetPetrolTankHealth(float fPetrolTankHealth)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_fPetrolTankHealth = fPetrolTankHealth;
}

float CVehicle::GetPetrolTankHealth()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_fPetrolTankHealth;

	return 0.0f;
}

void CVehicle::SetDoorLockState(DWORD dwDoorLockState)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		pVehicle->m_dwDoorLockState = dwDoorLockState;
}

DWORD CVehicle::GetDoorLockState()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return pVehicle->m_dwDoorLockState;

	return 0;
}

void CVehicle::SetEngineStatus(bool bStatus, bool bUnknown)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bStatus) {
			bool bUnknownTrue = true;

			_asm	push bUnknownTrue;
			_asm	mov ecx, pVehicle;
			_asm	call COffsets::FUNC_CVehicle__SetEngineOn;
		}
		else {
			_asm mov ecx, pVehicle;
			_asm call COffsets::FUNC_CVehicle__SetEngineOff;
		}
	}
}

bool CVehicle::GetEngineStatus()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(IS_BIT_SET(pVehicle->m_byteFlags1, 8))
		return true;

	return false;
}

void CVehicle::SoundHorn(int iDuration)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if (pVehicle)
	{
		EFLC::CScript::SoundCarHorn(g_pCore->GetGame()->GetPools()->GetVehiclePool()->HandleOf(pVehicle), iDuration);
	}
}

void CVehicle::SetComponentState(BYTE byteComponent, bool bState)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if (pVehicle && byteComponent >= 0 && byteComponent <= 8)
			((int(__thiscall *)(IVehicle*, BYTE, bool))COffsets::FUNC_CVehicle__SetComponentDisabled)(pVehicle, byteComponent + 89, !bState);
}

BYTE CVehicle::GetTextureVariationCount()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return g_pCore->GetGame()->GetModelInfo(pVehicle->m_wModelIndex)->GetModelInfo()->byteTextureVariationCount;

	return 255;
}

void CVehicle::SetTextureVariation(int iVariation)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		// Get the number of texture variations
		BYTE byteTextureVariationCount = GetTextureVariationCount();

		// Do we have no texture variations?
		if(byteTextureVariationCount == 255)
			return;

		// Is the texture variation valid?
		if(iVariation < byteTextureVariationCount)
		{
			DWORD dwTemp = (*(DWORD *)(pVehicle + 0x34) + 4);
			*(DWORD *)(dwTemp + 0xD8) = iVariation;

			_asm	push -1;
			_asm	push pVehicle;
			_asm	mov ecx, dwTemp;
			_asm	call COffsets::FUNC_CVehicle__SetTextureVariation;
		}
	}
}

int CVehicle::GetTextureVariation()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		// Get the number of texture variations
		BYTE byteTextureVariationCount = GetTextureVariationCount();

		// Do we have any texture variations?
		if(byteTextureVariationCount)
		{
			DWORD dwTemp = *(DWORD *)(*(DWORD *)(pVehicle + 0x34) + 4);
			return *(DWORD *)(dwTemp + 0xD8);
		}
	}

	return 0;
}

void CVehicle::SetCanBeVisiblyDamaged(bool bState)
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
	{
		if(bState)
			SET_BIT(pVehicle->m_byteFlags9, 4);
		else
			UNSET_BIT(pVehicle->m_byteFlags9, 4);
	}
}

bool CVehicle::CanBeVisiblyDamaged()
{
	// Do we have a valid vehicle pointer?
	IVehicle * pVehicle = GetVehicle();

	if(pVehicle)
		return IS_BIT_SET(pVehicle->m_byteFlags9, 4);

	return false;
}

void CVehicle::SetGPSState(bool bState)
{
	IVehicle * pVehicle = GetVehicle();
	if(pVehicle)
		*(bool*)(pVehicle + 0xB02) = bState;
}

bool CVehicle::GetGPSState()
{
	IVehicle * pVehicle = GetVehicle();
	if(pVehicle)
		return *(bool*)(pVehicle + 0xB02);
	return false;
}

//TODO: Add Get? *(DWORD *)(pVehicle + 0xFD0); // 4048
void CVehicle::BurstCarTyre(int iTyre)
{
	IVehicle * pVehicle = GetVehicle();
	if(pVehicle)
	{
		_asm	push iTyre;
		_asm	mov ecx, pVehicle;
		_asm	call COffsets::FUNC_CVehicle__BurstCarTyre;
	}
}

void CVehicle::GetDeformation(CVector3& vecPos)
{
	IVehicle * pVehicle = GetVehicle();
	if(pVehicle)
		; //vecPos = pVehicle->m_pDamageManager->vecDamage;
}

void CVehicle::SetCarCanBeDamaged(bool bDamage)
{
	IVehicle * pVehicle = GetVehicle();
	if(pVehicle)
	{
		DWORD byteFlagDamage = *(DWORD *)(pVehicle + 0x118); // 280 (flag?)
		if(bDamage)
			*(DWORD *)byteFlagDamage &= 0x400u;
		else
			*(DWORD *)byteFlagDamage |= 0xFFFFFBFFu;
	}
}

void CVehicle::SetLightsState(int iState)
{
	IVehicle * pVehicle = GetVehicle();
	if(pVehicle)
		iState == 2 ? pVehicle->m_byteLightState ^= (2 ^ pVehicle->m_byteLightState) & 3 : pVehicle->m_byteLightState ^= (1 ^ pVehicle->m_byteLightState) & 3;
}

int CVehicle::GetLightsState()
{
	IVehicle * pVehicle = GetVehicle();
	if(pVehicle)
		return (int)pVehicle->m_byteLightState;

	return 0;
}

void CVehicle::RemoveVehicleWindow(eVehicleWindow window)
{
	IVehicle *pVehicle = GetVehicle();
	if(pVehicle)
	{
		_asm	push window;
		_asm	mov ecx, pVehicle;
		_asm	call COffsets::FUNC_CVehicle__RemoveVehicleWindow;
	}
}

bool CVehicle::IsCarInWater()
{
	IVehicle *pVehicle = GetVehicle();

	bool bInWater = false;
	if(pVehicle)
	{
		if( *(int *)(pVehicle + 0x1354) == 2 )
			bInWater = *(BYTE *)(pVehicle + 0x1360) & 1;   
		else 
		{
			if((*(BYTE *)(pVehicle + 0x118) & 1) || ((*(DWORD *)(pVehicle + 0x6C) != NULL) && *(BYTE *)(pVehicle + 0x141)))
				bInWater = true;
		}
	}
	return  bInWater;
}

void CVehicle::SetCarCanBurstTyres(bool bState)
{
	IVehicle *pVehicle = GetVehicle();
	if(pVehicle)
	{
		if(bState)
			*(BYTE *)(pVehicle + 0xF67) &= 0xEFu;
		else
			*(BYTE *)(pVehicle + 0xF67) |= 0x10u;
	}
}

bool CVehicle::GetCarCanBurstTyres()
{
	IVehicle *pVehicle = GetVehicle();
	if(pVehicle)
		return ((*(BYTE *)(pVehicle + 0xF67) & 0xEFu) != NULL);

	return false;
}

void CVehicle::SetHeadlights(bool bSwitch)
{
	IVehicle * pVehicle = GetVehicle();
	if(pVehicle)
		bSwitch ? SET_BIT(pVehicle->m_byteFlags6, 2) : UNSET_BIT(pVehicle->m_byteFlags6, 2);
}

bool CVehicle::GetHeadlights()
{
	IVehicle * pVehicle = GetVehicle();
	if(pVehicle)
		return IS_BIT_SET(pVehicle->m_byteFlags6, 2);

	return 0;
}

void CVehicle::SetRadioStation(BYTE byteRadioStation)
{
	IVehicle * pVehicle = GetVehicle();
	if(pVehicle)
		pVehicle->m_byteRadioStation = byteRadioStation;
}

BYTE CVehicle::GetRadioStation()
{
	IVehicle * pVehicle = GetVehicle();
	if(pVehicle)
		return pVehicle->m_byteRadioStation;

	return 0;
}

_GAME_END
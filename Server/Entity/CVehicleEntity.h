//========= IV: Network - https://github.com/GTA-Network/IV-Network ============
//
// File: CVehicleEntity.h
// Project: Server.Core
// Author: xForce
// License: See LICENSE in root directory
//
//==============================================================================

#ifndef CVehicleEntity_h
#define CVehicleEntity_h

#include "CNetworkEntity.h"

class CScriptVehicle;

class CVehicleEntity : public CNetworkEntity
{
private:
	CScriptVehicle*	m_pScriptVehicle;
	Matrix			m_Matrix;
	unsigned int	m_fHealth;
	float			m_fPetrolHealth;
	float			m_fSteeringAngle;
	bool			m_bEngineState;
	bool			m_bSirenState;
	bool			m_bLightsState;
	bool			m_bHornState;
	bool			m_bTaxiLightsState;
	int				m_iLockedState;
	int				m_iDirtLevel;
	float			m_fHeading;
	int				m_iModelId;
	DWORD			m_dwColor[5];
	float			m_fQuat[4];

public:
										CVehicleEntity();
										~CVehicleEntity();

	bool								Create() { return true; }
	bool								Destroy() { return true; }

	void								SetMatrix(Matrix &matrix) { m_Matrix = matrix; }
	void								GetMatrix(Matrix &matrix) { matrix = m_Matrix; }

	void								SetQuaternion(float quat[4]) { memcpy(m_fQuat, quat, sizeof(m_fQuat)); }
	void								GetQuaternion(float * quat) { memcpy(quat, m_fQuat, sizeof(m_fQuat)); }

	void								SetHealth(float fHealth) { m_fHealth = fHealth; }
	float								GetHealth() { return m_fHealth; }

	float								GetHeading() { return m_fHeading; }
	void								SetHeading(float fHeading) { m_fHeading = fHeading; }

	void								SetPetrolTankHealth(float fHealth) { m_fPetrolHealth = fHealth; }
	float								GetPetrolTankHealth() { return m_fPetrolHealth; }

	void								SetEngineState(bool bEngineState) { m_bEngineState = bEngineState; }
	bool								GetEngineState() { return m_bEngineState; }

	void								SetSteeringAngle(float fAngle) { m_fSteeringAngle = fAngle; }
	float								GetSteeringAngle() { return m_fSteeringAngle; }

	void								SetLockedState(int iLockedState) { m_iLockedState = iLockedState; }
	int									GetLockedState() { return m_iLockedState; }

	void								SetDirtLevel(int iDirtLevel) { m_iDirtLevel = iDirtLevel; }
	int									GetDirtLevel() { return m_iDirtLevel; }

	void								SetScriptVehicle(CScriptVehicle* pScriptVehicle) { m_pScriptVehicle = pScriptVehicle; }
	CScriptVehicle*						GetScriptVehicle() { return m_pScriptVehicle; }

	void								SetModelId(int iModelId) { m_iModelId = iModelId; }
	int									GetModelId() { return m_iModelId; }

	void								SetColor(BYTE id, DWORD color) { if (id < 1 || id > 5) assert(CString("Set color%i?! ARE YOU IDIOT?!", id).Get()); else m_dwColor[id - 1] = color; }
	DWORD								GetColor(BYTE id) { if (id < 1 || id > 5) assert(CString("Get color%i?! ARE YOU IDIOT?!", id).Get()); return m_dwColor[id - 1]; }

	bool								GetSirenState() { return m_bSirenState; }
	void								SetSirenState(bool bSirenState) { m_bSirenState = bSirenState; }

	bool								GetLightsState() { return m_bLightsState; }
	void								SetLightsState(bool bLightsState) { m_bLightsState = bLightsState; }

	bool								GetHornState() { return m_bHornState; }
	void								SetHornState(bool bHornState) { m_bHornState = bHornState; }

	bool								GetTaxiLightsState() { return m_bTaxiLightsState; }
	void								SetTaxiLightsState(bool bTaxiLightsState) { m_bTaxiLightsState = bTaxiLightsState; }
};

class CScriptVehicle : public CScriptEntity
{
public:
							CScriptVehicle() { };
							~CScriptVehicle() { };

	CVehicleEntity *		GetEntity() { return (CVehicleEntity*)CScriptEntity::GetEntity(); }

	const char *			GetScriptClassName() { return "CVehicleEntity"; }
	
	void					SetPosition(float fX, float fY, float fZ);
	void					SetRotation(float fX, float fY, float fZ);
	CVector3				GetRotation() { CVector3 vecRot; GetEntity()->GetRotation(vecRot); return vecRot; };
	
	void					SetMoveSpeed(float fX, float fY, float fZ);
	void					SetTurnSpeed(float fX, float fY, float fZ);

	CVector3				GetPosition() { CVector3 vecPos; GetEntity()->GetPosition(vecPos); return vecPos; };

	void					SetHealth(float fHealth);
	float					GetHealth() { return GetEntity()->GetHealth(); };

	void					SetLocked(int iLocked);
	int						GetLocked() { return GetEntity()->GetLockedState(); };

	void					SetEngine(bool bEngineState);
	bool					GetEngine() { return GetEntity()->GetEngineState(); };

	bool					GetSiren() { return GetEntity()->GetSirenState(); };
	void					SetSiren(bool bSirenState);

	bool					GetLights() { return GetEntity()->GetLightsState(); };
	void					SetLights(bool bLightsState);	

	bool					GetTaxiLights() { return GetEntity()->GetTaxiLightsState(); };
	void					SetTaxiLights(bool bTaxiLightsState);

	bool					GetHorn() { return GetEntity()->GetHornState(); };

	void					SetDirtLevel(int iDirtLevel);
	int						GetDirtLevel() { return GetEntity()->GetDirtLevel(); };
};

#endif // CVehicleEntity_h

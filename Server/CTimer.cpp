#include "CTimer.h"
#include <CServer.h>

CTimer::CTimer(stScriptFunction function, int interval, int reapeatings)
{
	m_callback = function;
	m_uiInterval = interval;
	m_iRepeatings = reapeatings;
	m_bPaused = false;
	m_bRunning = false;
	m_uiLastPulse = 0;
}

CTimer::~CTimer()
{
}

void CTimer::Stop()
{
	m_bRunning = false;
	CServer::GetInstance()->GetTimerManager()->Remove(this);
	delete m_pScriptTimer;
	delete this;
}

bool CTimer::Pulse()
{
	if (m_bPaused || !m_bRunning)
		return false;


	unsigned int uiNow = SharedUtility::GetTime();

	if (uiNow >= m_uiLastPulse + m_uiInterval)
	{
		for (auto pResource : CServer::GetInstance()->GetResourceManager()->GetResources())
			pResource->GetVM()->Call(m_callback);


		m_uiLastPulse = uiNow;

		if (m_iRepeatings > 0)
		{
			if (--m_iRepeatings == 0)
				return false;
		}
	}

	return true;
}
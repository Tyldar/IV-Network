//========= IV: Network - https://github.com/GTA-Network/IV-Network ============
//
// File: CEvents.cpp
// Project: Shared
// Author: xForce <xf0rc3.11@gmail.com>
// License: See LICENSE in root directory
//
//==============================================================================

#include "CEvents.h"
#include "CScriptArgument.h"

CEvents* CEvents::s_pInstance = 0;

bool CEvents::Add(CString strName, CEventHandler* pEventHandler)
{
	auto itEvent = m_Events.find(strName);
	if(itEvent == m_Events.end())
	{
		// new - create the event
		auto ret = m_Events.insert(std::pair<CString, std::list<CEventHandler*>>(strName, std::list<CEventHandler*>()));
		itEvent = ret.first;
	} else {
		for(auto pEvent : itEvent->second)
		{
			if(pEventHandler->equals(pEvent))
				return false;
		}
	}

	itEvent->second.push_back(pEventHandler);

	// Function for view all stored resources
	for (std::map<CString, std::list<CEventHandler*>>::iterator iter = m_Events.begin(); iter != m_Events.end(); iter++){
		CLogFile::Printf("----- Event %s: %d", (*iter).first.Get(), (*iter).second.size());
		for (std::list< CEventHandler* >::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); iter2++){
			if ((*iter2)->GetVM()->GetResource()->GetName().Get())
				CLogFile::Printf("- Resource %s", (*iter2)->GetVM()->GetResource()->GetName().Get());
			else
				CLogFile::Printf("--- Error: unknown resource");
		}
	}
	return true;
}

CScriptArguments CEvents::Call(CString strName, CScriptArguments* pArguments, CEventHandler::eEventType EventType, IScriptVM * pVM)
{
	CScriptArguments returnArguments;
	auto itEvent = m_Events.find(strName);
	if(itEvent != m_Events.end())
	{
		CScriptArgument ret;
		for(auto pEvent : itEvent->second)
		{
			if(EventType == CEventHandler::eEventType::GLOBAL_EVENT
				&& pEvent->GetType() == CEventHandler::GLOBAL_EVENT)
			{
				pEvent->Call(pArguments, &ret);
				returnArguments.push(ret);
			}
			else if(EventType == CEventHandler::eEventType::RESOURCE_EVENT
				&& pEvent->GetType() == CEventHandler::RESOURCE_EVENT
				&& pEvent->GetVM() == pVM)
			{
				pEvent->Call(pArguments, &ret);
				returnArguments.push(ret);
			}
			else if (EventType == CEventHandler::eEventType::NATIVE_EVENT
				&& pEvent->GetType() == CEventHandler::eEventType::RESOURCE_EVENT
				&& pVM == nullptr)
			{
				//Pruebas
				if (strName.Compare(CString("playerCommand")) == 0){
					CLogFile::Printf("Resource name: %s", pEvent->GetVM()->GetResource()->GetName().Get());
				}
				else
					CLogFile::Printf("playerCommand");
				pEvent->Call(pArguments, &ret);
				returnArguments.push(ret);
			}
			else if (EventType == CEventHandler::eEventType::NATIVE_EVENT
				&& pEvent->GetType() == CEventHandler::eEventType::RESOURCE_EVENT
				&& pEvent->GetVM() == pVM)
			{
				pEvent->Call(pArguments, &ret);
				returnArguments.push(ret);
			}
			else if (EventType == CEventHandler::REMOTE_EVENT
				&& pEvent->GetType() == CEventHandler::REMOTE_EVENT)
			{
				pEvent->Call(pArguments, &ret);
				returnArguments.push(ret);
			}
		}
	}
	return returnArguments;
}

bool CEvents::RemoveEvent(CString strName, CEventHandler::eEventType eventType, IScriptVM* pVM){
	for (std::map<CString, std::list<CEventHandler*>>::iterator iter = m_Events.begin(); iter != m_Events.end(); iter++){
		if ((*iter).first.Compare(strName) == 0){
			for (std::list< CEventHandler* >::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); iter2++){
				if (eventType == CEventHandler::RESOURCE_EVENT && pVM == (*iter2)->GetVM()){
					if ((*iter2)->GetVM()->GetVMType() == eVMType::LUA_VM)
					{
						//TODO: Remove Lua event
					}
					else if ((*iter2)->GetVM()->GetVMType() == eVMType::SQUIRREL_VM)
					{
						sq_release(((CSquirrelVM*)pVM)->GetVM(), &(*iter2)->GetFunction());
						(*iter).second.erase(iter2);
						// Remove events with no handlers
						if ((*iter).second.size() == 0)
							m_Events.erase(iter);
						return true;
					}
				}
				else if ((*iter2)->GetType() == eventType && pVM == nullptr){
					(*iter).second.erase(iter2);
					// Remove events with no handlers
					if ((*iter).second.size() == 0)
						m_Events.erase(iter);
					return true;
				}
			}
		}
	}
	return false;
}

bool CEvents::Remove(CString strName, CEventHandler* pEventHandler)
{
	auto itEvent = m_Events.find(strName);
	if(itEvent != m_Events.end())
	{
		for(auto pEvent = itEvent->second.begin(); pEvent != itEvent->second.end(); pEvent++)
		{
			if(pEventHandler->equals(*pEvent))
			{
				itEvent->second.erase(pEvent);
				if(itEvent->second.size() == 0)
				{
					m_Events.erase(itEvent);
					return true;
				}
			}
		}
	}
	return false;
}

bool CEvents::RemoveResourceEvents(IScriptVM* pVM){

	for (std::map<CString, std::list<CEventHandler*>>::iterator iter = m_Events.begin(); iter != m_Events.end(); iter++){
		for (std::list< CEventHandler* >::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); iter2++){
			if ((*iter2)->GetVM() == pVM){
				if ((*iter2)->GetVM()->GetVMType() == eVMType::LUA_VM)
				{
					//TODO: Remove Lua event
				}
				else if ((*iter2)->GetVM()->GetVMType() == eVMType::SQUIRREL_VM)
				{
					CLogFile::Printf("Deleting event %s...", (*iter).first.C_String());
					CLogFile::Printf("Events before: %d", (*iter).second.size());
					sq_release(((CSquirrelVM*)pVM)->GetVM(), &(*iter2)->GetFunction());
					(*iter).second.erase(iter2--);
					CLogFile::Printf("Event %s DELETED!!", (*iter).first.C_String());
					CLogFile::Printf("Events after: %d", (*iter).second.size());
				}
			}
		}
		// Remove events with no handlers
		if ((*iter).second.size() == 0){
			CLogFile::Printf("Deleting %s empty...", (*iter).first.C_String());
			m_Events.erase(iter);
			CLogFile::Printf("Empty DELETED!!");
		}
	}
	// Function for view all stored resources
	for (std::map<CString, std::list<CEventHandler*>>::iterator iter = m_Events.begin(); iter != m_Events.end(); iter++){
		CLogFile::Printf("----- Event %s: %d", (*iter).first.Get(), (*iter).second.size());
		for (std::list< CEventHandler* >::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); iter2++){
			if ((*iter2)->GetVM()->GetResource()->GetName().Get())
				CLogFile::Printf("- Resource %s", (*iter2)->GetVM()->GetResource()->GetName().Get());
			else
				CLogFile::Printf("--- Error: unknown resource");
		}
	}
	if (m_Events.size() == 0){
		m_Events.clear();
		CLogFile::Printf("There aren' events...");
	}
	return true;
}

bool CEvents::IsEventRegistered(CString strEventName)
{
	if (m_Events.find(strEventName) == m_Events.end())
		return false;

	return true;
}

void CEvents::Clear()
{
	for(auto pEvent : m_Events)
	{
		for(auto event : pEvent.second)
		{
			SAFE_DELETE(event);
		}
		pEvent.second.clear();
	}
	m_Events.clear();
}
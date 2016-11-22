//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CXML.h
// Project: Shared
// Author(s): Einstein
//            jenksta
//			  g3o0or
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

// TODO: Capitalize function names?

#include <tinyxml/tinyxml.h>
#include <tinyxml/ticpp.h>
#include "SharedUtility.h"
#include "CString.h"

#include <Scripting/CScriptClass.h>

// Notes to EinStein:
// Value = NameOfNode
// Text = InNode

class CXML
{
private:
	TiXmlDocument m_document;
	TiXmlElement * m_node;

public:
	CXML();
	~CXML();

	bool         load(CString strFileName);
	bool         save();
	void         setTabSize(int size);
	const char * getAttribute(const char * attributeName);
	void         removeAttribute(const char * attributeName);
	void         setAttribute(const char * attributeName, const char * value);
	const char * nodeName();
	void         nodeSetName(const char * content);
	const char * nodeContent();
	// Warning: This will delete also child nodes, if they exist
	void         nodeSetContent(const char * content);
	void         nodeToRoot();
	bool         findNode(const char * name);
	bool         nextNode();
	bool         previousNode();
	bool         childNodeFirst();
	void         nodeParent();
	void         nodeClear();
	void         newNode(const char * name, bool nodePointerToNewNode = true);
	void         newComment(const char * comment);
	const char * lastError(int * iErrorRow, int * iErrorCol);
	bool         isComment();
};

class CScriptXML
{

private:
	CXML		*m_pXML;

public:
	CScriptXML() { m_pXML = new CXML(); }
	~CScriptXML() { delete m_pXML; }

	CXML *		 GetXML() { return m_pXML; }
	void		 SetXML(CXML* pXML) { m_pXML = pXML; }

	bool         load(CString strFileName){ return GetXML()->load(strFileName); }
	bool         save() { return GetXML()->save(); }
	void		 close() { delete this; } 
	
	void         setTabSize(int size) { CLogFile::Printf("size is %i!", size);GetXML()->setTabSize(size); }
	const char * getAttribute(const char * attributeName) { return GetXML()->getAttribute(attributeName); }
	void         removeAttribute(const char * attributeName) { GetXML()->removeAttribute(attributeName); }
	void         setAttribute(const char * attributeName, const char * value) { GetXML()->setAttribute(attributeName, value); }
	const char * nodeName() { return GetXML()->nodeName(); }
	void         nodeSetName(const char * content) { GetXML()->nodeSetName(content); }
	const char * nodeContent() { return GetXML()->nodeContent(); }
	
	void         nodeSetContent(const char * content) { GetXML()->nodeSetContent(content); }
	void         nodeToRoot() { GetXML()->nodeToRoot(); }
	bool         findNode(const char * name) { return GetXML()->findNode(name); }
	bool         nextNode() { return GetXML()->nextNode(); }
	bool         previousNode() { return GetXML()->previousNode(); }
	bool         childNodeFirst() { return GetXML()->childNodeFirst(); }
	void         nodeParent() { GetXML()->nodeParent(); }
	void         nodeClear() { GetXML()->nodeClear(); }
	void         newNode(const char * name, bool nodePointerToNewNode = true) { CLogFile::Printf("node is %s with %i!", name, nodePointerToNewNode); GetXML()->newNode(name, nodePointerToNewNode); }
	void         newComment(const char * comment) { GetXML()->newComment(comment); }
	const char * lastError(int * iErrorRow, int * iErrorCol) { return GetXML()->lastError(iErrorRow, iErrorCol); }
	bool         isComment() { return GetXML()->isComment(); }
};
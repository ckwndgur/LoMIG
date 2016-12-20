#pragma once
#include <string>
#include <sstream>
#include "tinyxml2.h"
#include "StringManager.h"
#include "UserConfig.h"
#include "FolderManager.h"


using namespace std;

class XMLManager
{
public:
	XMLManager(void);
	~XMLManager(void);

	FolderManager mFolderManager;

	void initXML();
	void CreatXML_AgentInfo(string AgentName);

	bool WriteXML(string NodeTitle, string* ChildElement, string* Contents, int ChildCnt);
	bool WriteAgentInfo_XML(string NodeTitle, string* ChildElement, string* Contents, int ChildCnt);

	bool AddXML(string NodeTitle, string* ChildElement, string* Contents, int ChildCnt);
	bool EditElementXML(string ChildTitle, string ChildElement, string Contents);

	string Parsing_Target_XML(string sTargetDir, string ChildTitle, string ChildElement);
	string ParsingXML(string ChildTitle, string ChildElement);

//private:
	string sConfigDirectory;
	string sConfigFileDirectory;
	string sConfigFileName;

	string sAgentInfoDirectory;
	UserConfig mUserConfig;
	StringManager mStringManager;
	tinyxml2::XMLDocument mXMLDocument;
	tinyxml2::XMLNode *mNode;
	tinyxml2::XMLElement *mElement;
};

#pragma once
#include <map>
#include "Type.h"
class CCommandEntry
{

public:
	static CCommandEntry& CreateInstance(void);
protected:
	CCommandEntry(void);
	~CCommandEntry(void);

public:
	pCommandEntry GetCommandEntryByCommanID(unsigned int nCommandID);
	pUpdateCommandEntry GetUpdateCommandEntryByCommanID(unsigned int nCommandID);

private:
	void InitCommandEntry(void);
	void InitUpdateCommandEntry(void);
	void UnInitCommandEntry(void);
	void UnInitUpdateCommandEntry(void);
private:
	std::map<unsigned int,pCommandEntry> m_CommandEntryMap;
	std::map<unsigned int,pUpdateCommandEntry> m_UpdateCommandEntryMap;
};


#pragma once
#include "common.h"
#include "AFileMgr.h"

enum {
	GXT_VERSION = 0x64,
};

struct CKeyEntry {
	glm::uint32 valueOffset;
	char key[8];
};

class CKeyArray {
public:
	std::vector<CKeyEntry> m_vEntries;
	glm::int32 m_nNumEntries;

	CKeyArray() : m_nNumEntries(0), m_vEntries(0) {}
	~CKeyArray() { Unload(); }
	void Load(glm::uint64 length, AFileMgr& file);
	void Unload();
	CKeyEntry* BinarySearch(const char* key, CKeyEntry* entries, glm::int16 low, glm::int16 high);
	std::string Search(const char* key, glm::uint16* data);
};

class CData {
public:
	std::vector<glm::uint16> m_vChars;
	glm::int32 m_nNumChars;

public:
	CData() : m_vChars(0), m_nNumChars(0) {}
	~CData() { Unload(); }
	void Load(glm::uint64 length, AFileMgr& file);
	void Unload();
};

class CText {
private:
	CKeyArray m_KeyArray;
	CData m_Data;
	char m_cEncoding;

public:
	CText();
	bool Load();
	bool UnLoad();
	std::string Get(const char* key);
};

extern std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> WideStringConvert;

extern CText TheText;

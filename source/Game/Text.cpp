#include "Text.h"
#include "AEngineSettings.h"
#include "AFileMgr.h"
#include "AConsole.h"

CText TheText;
std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> WideStringConvert;

CText::CText() {
	m_KeyArray = {};
	m_Data = {};
	m_cEncoding = 'e';
}

bool CText::Load() {
	const glm::uint8 lang = EngineSettings.m_nLanguage;
	std::string path;

	switch (lang) {
	default:
		path = "data/e.gxt";
		break;
	}

	AFileMgr file;
	if (!file.Open(path)) {
		return false;
	}

	char header[4];
	file.ReadCustom(header, 3);
	header[3] = '\0';

	m_cEncoding = file.ReadUInt8();
	glm::uint16 version = file.ReadUInt16();

	if (strcmp(header, "GBL") || version != GXT_VERSION) {
		return false;
	}

	while (file.GetPosition() < file.GetSize()) {
		char block[4] = {};
		block[0] = file.ReadUInt8();
		block[1] = file.ReadUInt8();
		block[2] = file.ReadUInt8();
		block[3] = file.ReadUInt8();

		glm::uint32 sectlen = file.ReadUInt32();
		
		if (sectlen != 0) {
			if (!strncmp(block, "TKEY", 4))
				m_KeyArray.Load(sectlen, file);
			else if (!strncmp(block, "TDAT", 4))
				m_Data.Load(sectlen, file);
			else
				file.Seek(sectlen);
		}
	}

	return true;
}

bool CText::UnLoad() {
	m_Data.Unload();
	m_KeyArray.Unload();
	return true;
}

std::string CText::Get(const char* key) {
	if (!strncmp(key, "blank", 5))
		return " ";

	return m_KeyArray.Search(key, m_Data.m_vChars.data());
}

void CData::Load(glm::uint64 length, AFileMgr& file) {
	m_nNumChars = static_cast<glm::int32>(length / sizeof(glm::uint16));
	m_vChars.resize(m_nNumChars);
	file.ReadCustom(m_vChars.data(), length);
}

void CData::Unload() {
	m_vChars = {};
	m_nNumChars = 0;
}

void CKeyArray::Load(glm::uint64 length, AFileMgr& file) {
	m_nNumEntries = static_cast<glm::int32>(length / sizeof(CKeyEntry));
	m_vEntries.resize(m_nNumEntries);
	file.ReadCustom(m_vEntries.data(), length);
}

void CKeyArray::Unload() {
	m_vEntries = {};
	m_nNumEntries = 0;
}

CKeyEntry* CKeyArray::BinarySearch(const char* key, CKeyEntry* entries, glm::int16 low, glm::int16 high) {
	if (low > high)
		return nullptr;

	glm::int32 mid = (low + high) / 2;
	glm::int32 diff = strcmp(key, entries[mid].key);

	if (diff == 0)
		return &entries[mid];
	if (diff < 0)
		return BinarySearch(key, entries, low, mid - 1);
	if (diff > 0)
		return BinarySearch(key, entries, mid + 1, high);

	return nullptr;
}

std::string CKeyArray::Search(const char* key, glm::uint16* data) {
	CKeyEntry* found = BinarySearch(key, m_vEntries.data(), 0, m_nNumEntries - 1);
	if (found) {
		wchar_t* c = reinterpret_cast<wchar_t*>(reinterpret_cast<glm::uint8*>(data) + found->valueOffset);
		return WideStringConvert.to_bytes(c);
	}
	return std::format("{} missing", key);
} 

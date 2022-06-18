#pragma once
#pragma warning (disable : 26451)
#include "common.h"
#include "AFileMgr.h"

enum {
	GMP_FILE_VERSION = 0x1F4,
	STY_FILE_VERSION = 0x2BC,
};

enum eChunkType {
	// GMP
	UMAP,
	CMAP,
	DMAP,
	ZONE,
	MOBJ,
	PSXM,
	ANIM,
	LGHT,
	RGEN,
	// STY
	PALX,
	PPAL,
	PALB,
	TILE,
	SPRB,
	SPRX,
	SPRG,
	DELS,
	DELX,
	FONB,
	CARI,
	OBJI,
	PSXT,
	RECY,
	SPEC,
	NUM_CHUNKS,
};

class CFileHeader {
public:
	char m_cFileType[4];
	glm::uint16 m_nFileVersion;
};

class CChunkHeader {
public:
	char m_cChunkType[4];
	glm::uint64 m_nChunkSize;
	int m_nType;
};

class CGBH {
private:
	std::shared_ptr<AFileMgr> file;
	CFileHeader fileHeader;
	CChunkHeader chunkHeader;

public:
	std::shared_ptr<AFileMgr>& GetFile() { return file; }

public:
	CGBH();
	~CGBH();

	bool Init(std::string fileName, glm::uint32 version);
	bool LoopThroughChunks();
	std::string GetFileType();
	glm::uint16 GetVersion();
	std::string GetChunkTypeString();
	glm::uint64 GetChunkSize();
	glm::int32 GetChunkType();
	std::string GetChunkHeader();
	void SkipChunk();
};

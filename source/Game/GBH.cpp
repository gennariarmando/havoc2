#include "GBH.h"

const char* gbhChunkHeaders[] = { 
	"UMAP", "CMAP", "DMAP", "ZONE", "MOBJ", "PSXM", "ANIM", "LGHT", "RGEN",
	"PALX", "PPAL", "PALB", "TILE", "SPRB", "SPRX", "SPRG", "DELS", "DELX",
	"FONB", "CARI", "OBJI", "PSXT", "RECY", "SPEC",
};

bool CGBH::Init(std::string fileName, glm::uint32 version) {
	file.Open(fileName);
	fileHeader.m_cFileType[0] = file.ReadUInt8();
	fileHeader.m_cFileType[1] = file.ReadUInt8();
	fileHeader.m_cFileType[2] = file.ReadUInt8();
	fileHeader.m_cFileType[3] = file.ReadUInt8();
	fileHeader.m_nFileVersion = file.ReadUInt16();

	if (GetVersion() != version) {
		std::cout << "Error: Unable to read gbh file named: " << fileName << std::endl;
		return false;
	}

	return true;
}

bool CGBH::LoopThroughChunks() {
	chunkHeader.m_cChunkType[0] = file.ReadUInt8();
	chunkHeader.m_cChunkType[1] = file.ReadUInt8();
	chunkHeader.m_cChunkType[2] = file.ReadUInt8();
	chunkHeader.m_cChunkType[3] = file.ReadUInt8();
	chunkHeader.m_nChunkSize = file.ReadUInt32();

	std::cout << GetChunkType() << std::endl;
	std::cout << GetChunkSize() << std::endl;

	return (file.GetPosition() < file.GetSize());
}

std::string CGBH::GetFileType() {
	std::string t(fileHeader.m_cFileType, 0, 4);
	return t;
}

glm::uint16 CGBH::GetVersion() {
	return fileHeader.m_nFileVersion;
}

std::string CGBH::GetChunkTypeString() {
	std::string t(chunkHeader.m_cChunkType, 0, 4);
	return t;
}

glm::uint32 CGBH::GetChunkSize() {
	return chunkHeader.m_nChunkSize;
}

glm::uint32 CGBH::GetChunkType() {
	for (glm::uint32 i = UMAP; i < NUM_CHUNKS; i++) {
		if (!GetChunkTypeString().compare(gbhChunkHeaders[i]))
			return i;
	}

	return -1;
}

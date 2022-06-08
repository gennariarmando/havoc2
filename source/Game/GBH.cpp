#include "GBH.h"

const char* gbhChunkHeaders[] = { 
	"UMAP", "CMAP", "DMAP", "ZONE", "MOBJ", "PSXM", "ANIM", "LGHT", "RGEN",
	"PALX", "PPAL", "PALB", "TILE", "SPRB", "SPRX", "SPRG", "DELS", "DELX",
	"FONB", "CARI", "OBJI", "PSXT", "RECY", "SPEC",
};

bool CGBH::Init(std::string fileName, glm::uint32 version) {
	if (!file.Open(fileName)) {
		std::cout << "[CGBH] Unable to read data from file " << "'" << fileName << "'" << std::endl;
		return false;
	}

	std::cout << "[CGBH] Reading data from file " << "'" << fileName << "'" << std::endl;

	fileHeader.m_cFileType[0] = file.ReadUInt8();
	fileHeader.m_cFileType[1] = file.ReadUInt8();
	fileHeader.m_cFileType[2] = file.ReadUInt8();
	fileHeader.m_cFileType[3] = file.ReadUInt8();
	fileHeader.m_nFileVersion = file.ReadUInt16();

	if (GetVersion() != version) {
		std::cout << "[CGBH] File version is " << fileHeader.m_nFileVersion << " must be " << version << std::endl;
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

	for (glm::uint32 i = UMAP; i < NUM_CHUNKS; i++) {
		if (!GetChunkTypeString().compare(gbhChunkHeaders[i])) {
			chunkHeader.m_nType = i;

			std::cout << "[CGBH] Found chunk " << GetChunkHeader() << " with a size of " << GetChunkSize() << " bytes" << std::endl;
		}
	}

	if (file.GetPosition() >= file.GetSize()) {
		std::cout << "[CGBH] Reading complete, closing file" << std::endl;
		std::cout << std::endl;

		file.Close();
		return false;		
	}

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

glm::int32 CGBH::GetChunkType() {
	return chunkHeader.m_nType;
}

std::string CGBH::GetChunkHeader() {
	return gbhChunkHeaders[chunkHeader.m_nType];
}

void CGBH::SkipChunk() {
	GetFile().Seek(GetChunkSize());
	std::cout << "[CGBH] Skipping " << GetChunkSize() << " bytes" << std::endl;
}
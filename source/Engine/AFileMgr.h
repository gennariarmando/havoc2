#pragma once
#include "common.h"

class AFileMgr {
public:
    std::ifstream m_File;
    glm::uint64 m_Size;
    glm::uint64 m_Position;

public:
    AFileMgr();
    AFileMgr(std::string fileName);
    ~AFileMgr();

    bool Open(std::string fileName);

    glm::uint32 ReadUInt32();
    glm::int32 ReadInt32();

    glm::uint16 ReadUInt16();
    glm::int16 ReadInt16();

    glm::uint8 ReadUInt8();
    glm::int8 ReadInt8();

    void ReadCustom(void* out, glm::uint32 size);

    void Seek(glm::uint32 size);
    void Close();

public:
    glm::uint64& GetSize() { return m_Size; }
    glm::uint64& GetPosition() { return m_Position; }
};

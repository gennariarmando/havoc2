#pragma once
#include "Common.h"

class CFileMgr {
public:
    std::ifstream m_File;
    glm::uint32 m_Size;
    glm::uint32 m_Position;

public:
    CFileMgr();
    CFileMgr(std::string fileName);
    ~CFileMgr();

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
    glm::uint32& GetSize() { return m_Size; }
    glm::uint32& GetPosition() { return m_Position; }

};

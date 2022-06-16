#include "AFileMgr.h"

AFileMgr::AFileMgr() {
    m_File = {};
    m_Size = 0;
    m_Position = 0;
}

AFileMgr::AFileMgr(std::string fileName) {
    Open(fileName);
}

AFileMgr::~AFileMgr() {

}

bool AFileMgr::Open(std::string fileName) {
    m_File.open(fileName, std::ios::in | std::ios::binary);

    if (!m_File.is_open()) {
        return false;
    }

    m_File.seekg(0, std::ios::end);
    m_Size = static_cast<glm::int64>(m_File.tellg());
    m_File.seekg(0, std::ios::beg);

    m_Position = static_cast<glm::int64>(m_File.tellg());

    return true;
}

glm::uint32 AFileMgr::ReadUInt32() {
    glm::uint32 result;
    m_File.read(reinterpret_cast<char*>(&result), sizeof(glm::uint32));
    m_Position = static_cast<glm::int64>(m_File.tellg());

    return result;
}

glm::int32 AFileMgr::ReadInt32() {
    glm::int32 result;
    m_File.read(reinterpret_cast<char*>(&result), sizeof(glm::int32));
    m_Position = static_cast<glm::int64>(m_File.tellg());

    return result;
}

glm::uint16 AFileMgr::ReadUInt16() {
    glm::uint16 result;
    m_File.read(reinterpret_cast<char*>(&result), sizeof(glm::uint16));
    m_Position = static_cast<glm::int64>(m_File.tellg());

    return result;
}

glm::int16 AFileMgr::ReadInt16() {
    glm::int16 result;
    m_File.read(reinterpret_cast<char*>(&result), sizeof(glm::int16));
    m_Position = static_cast<glm::int64>(m_File.tellg());

    return result;
}

glm::uint8 AFileMgr::ReadUInt8() {
    glm::uint8 result;
    m_File.read(reinterpret_cast<char*>(&result), sizeof(glm::uint8));
    m_Position = static_cast<glm::int64>(m_File.tellg());

    return result;
}

glm::int8 AFileMgr::ReadInt8() {
    glm::int8 result;
    m_File.read(reinterpret_cast<char*>(&result), sizeof(glm::int8));
    m_Position = static_cast<glm::int64>(m_File.tellg());

    return result;
}

void AFileMgr::ReadCustom(void* out, glm::uint32 size) {
    m_File.read(reinterpret_cast<char*>(out), size);
    m_Position = static_cast<glm::int64>(m_File.tellg());
}

void AFileMgr::Seek(glm::uint32 size) {
    m_File.seekg(size, 1);
    m_Position = static_cast<glm::int64>(m_File.tellg());
}

void AFileMgr::Close() {
    if (!m_File.is_open())
        return;

    m_File.close();
}

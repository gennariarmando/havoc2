#include "FileMgr.h"

CFileMgr::CFileMgr() {
    m_File = {};
    m_Size = 0;
    m_Position = 0;
}

CFileMgr::CFileMgr(std::string fileName) {
    Open(fileName);
}

CFileMgr::~CFileMgr() {

}

void CFileMgr::Open(std::string fileName) {
    m_File.open(fileName, std::ios::in | std::ios::binary);

    if (!m_File.is_open()) {
        std::cout << fileName << " can't be opened." << std::endl;
        return;
    }

    m_File.seekg(0, std::ios::end);
    m_Size = m_File.tellg();
    m_File.seekg(0, std::ios::beg);

    m_Position = m_File.tellg();
}


glm::uint32 CFileMgr::ReadUInt32() {
    glm::uint32 result;
    m_File.read(reinterpret_cast<char*>(&result), sizeof(glm::uint32));
    m_Position = m_File.tellg();

    return result;
}

glm::int32 CFileMgr::ReadInt32() {
    glm::int32 result;
    m_File.read(reinterpret_cast<char*>(&result), sizeof(glm::int32));
    m_Position = m_File.tellg();

    return result;
}

glm::uint16 CFileMgr::ReadUInt16() {
    glm::uint16 result;
    m_File.read(reinterpret_cast<char*>(&result), sizeof(glm::uint16));
    m_Position = m_File.tellg();

    return result;
}

glm::int16 CFileMgr::ReadInt16() {
    glm::int16 result;
    m_File.read(reinterpret_cast<char*>(&result), sizeof(glm::int16));
    m_Position = m_File.tellg();

    return result;
}

glm::uint8 CFileMgr::ReadUInt8() {
    glm::uint8 result;
    m_File.read(reinterpret_cast<char*>(&result), sizeof(glm::uint8));
    m_Position = m_File.tellg();

    return result;
}

glm::int8 CFileMgr::ReadInt8() {
    glm::int8 result;
    m_File.read(reinterpret_cast<char*>(&result), sizeof(glm::int8));
    m_Position = m_File.tellg();

    return result;
}

void CFileMgr::ReadCustom(void* out, glm::uint32 size) {
    m_File.read(reinterpret_cast<char*>(out), size);
    m_Position = m_File.tellg();
}

void CFileMgr::Seek(glm::uint32 size) {
    m_File.seekg(size, 1);
    m_Position = m_File.tellg();
}

void CFileMgr::Close() {
    if (!m_File.is_open())
        return;

    m_File.close();
}

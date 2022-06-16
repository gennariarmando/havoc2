#include "AFileLoader.h"
#include "AConsole.h"
#include "AFileMgr.h"

bool AFileLoader::LoadShadersFromFile(const std::string& vertexPath, const std::string& fragmentPath, std::string& vertexShader, std::string& fragmentShader) {
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        Console.WriteLine(std::format("Loading vertex shader from {} ", vertexPath));
        vertexShaderFile.open(vertexPath);

        Console.WriteLine(std::format("Loading fragment shader from {} ", fragmentPath));
        fragmentShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        vertexShaderFile.close();
        fragmentShaderFile.close();
        vertexShader = vShaderStream.str();
        fragmentShader = fShaderStream.str();
        return true;
    }
    catch (std::ifstream::failure& e) {
        Console.WriteLine(std::format("Error reading shader file {}", e.what()));
    }

    return false;
}

bool AFileLoader::LoadTGAFromFile(const std::string& path, glm::uint16& width, glm::uint16& height, std::shared_ptr<glm::uint8[]>& pixels) {
    AFileMgr file;

    if (!file.Open(path)) {
        Console.WriteLine(std::format("Error loading texture from {}", path));
        return false;
    }

    file.ReadUInt8();
    file.ReadUInt8();
    file.ReadUInt8();
    file.ReadInt16();
    file.ReadInt16();
    file.ReadUInt8();
    file.ReadInt16();
    file.ReadInt16();

    glm::int16 imageWidth = file.ReadInt16();
    glm::int16 imageHeight = file.ReadInt16();
    glm::uint8 imageBitCount = file.ReadUInt8();

    file.ReadUInt8();

    glm::uint32 colorMode = 4;
    glm::uint32 realColorMode = imageBitCount / 8;
    glm::uint32 imageSize = imageWidth * imageHeight * colorMode;

    std::shared_ptr<glm::uint8[]> imageData = std::make_shared<glm::uint8[]>(imageSize);
    std::shared_ptr<glm::uint8[]> buff = std::make_shared<glm::uint8[]>(imageSize);
    file.ReadCustom(imageData.get(), imageSize);

    glm::uint32 j = 0;
    for (glm::uint32 i = 0; i < imageSize; i += colorMode) {
        glm::uint8 r;
        glm::uint8 g;
        glm::uint8 b;

        switch (imageBitCount) {
        case 16:
            r = (imageData[j + 1] >> 2) & 0x1F;
            g = ((imageData[j + 1] << 3) & 0x1C) | ((imageData[j] >> 5) & 0x07);
            b = (imageData[j] & 0x1F);

            r = (r << 3) | (r >> 2);
            g = (g << 3) | (g >> 2);
            b = (b << 3) | (b >> 2);

            buff[i] = b;
            buff[i + 1] = g;
            buff[i + 2] = r;
            buff[i + 3] = 255;
            break;
        default:
            r = imageData[i + 2];
            g = imageData[i + 1];
            b = imageData[i];

            buff[i] = b;
            buff[i + 1] = g;
            buff[i + 2] = r;

            if (realColorMode < 4)
                buff[i + 3] = 255;
            break;
        }
        j += realColorMode;
    }

    imageData = buff;

    // Flip
    size_t bytesPerRow = (size_t)imageWidth * (colorMode * sizeof(glm::uint8));
    glm::uint8 temp[2048];
    glm::uint8* bytes = imageData.get();

    for (glm::int32 i = 0; i < (imageHeight >> 1); i++) {
        glm::uint8* row0 = bytes + i * bytesPerRow;
        glm::uint8* row1 = bytes + (imageHeight - i - 1) * bytesPerRow;

        size_t bytesLeft = bytesPerRow;
        while (bytesLeft) {
            size_t bytes_copy = (bytesLeft < sizeof(temp)) ? bytesLeft : sizeof(temp);
            memcpy(temp, row0, bytes_copy);
            memcpy(row0, row1, bytes_copy);
            memcpy(row1, temp, bytes_copy);
            row0 += bytes_copy;
            row1 += bytes_copy;
            bytesLeft -= bytes_copy;
        }
    }

    width = imageWidth;
    height = imageHeight;
    pixels = imageData;

    Console.WriteLine(std::format("Loading texture from {}", path));

    return true;
}

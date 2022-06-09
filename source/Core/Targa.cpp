#include "Targa.h"
#include "FileMgr.h"

bool CTarga::LoadTexture(std::string filename) {
    CFileMgr file;
    file.Open(filename);

    file.ReadUInt8();
    file.ReadUInt8();
    file.ReadUInt8();
    file.ReadInt16();
    file.ReadInt16();
    file.ReadUInt8();
    file.ReadInt16();
    file.ReadInt16();

    imageWidth = file.ReadInt16();
    imageHeight = file.ReadInt16();

    bitCount = file.ReadUInt8();
    file.ReadUInt8();

    glm::uint32 colorMode = 4;
    glm::uint32 realColorMode = bitCount / 8;
    glm::uint64 imageSize = imageWidth * imageHeight * colorMode;

    imageData = std::make_unique<glm::uint8[]>(imageSize);
    std::shared_ptr<glm::uint8[]> buff = std::make_unique<glm::uint8[]>(imageSize);
    file.ReadCustom(imageData.get(), imageSize);

    glm::int32 j = 0;
    for (glm::int32 i = 0; i < imageSize; i += colorMode) {
        glm::uint8 r;
        glm::uint8 g;
        glm::uint8 b;

        switch (bitCount) {
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
    
    for (glm::uint32 i = 0; i < (imageHeight >> 1); i++) {
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

    return true;
}

void CTarga::Free() {
    ;;
}

#pragma once
#include "GBH.h"
#include "ASprite2D.h"		

enum eBaseIndices {
	BASEINDEX_CARS,
	BASEINDEX_PEDS,
	BASEINDEX_CODEOBJ,
	BASEINDEX_MAPOBJ,
	BASEINDEX_USER,
	BASEINDEX_FONT,
	BASEINDEX_LAST,
	NUM_BASE_INDICES,
};

class CFontBase {
public:
	glm::uint16 fontCount;
	std::vector<glm::uint16> base;
};

class CSpriteEntry {
public:
	glm::uint32 ptr;
	glm::uint8 w;
	glm::uint8 h;
	glm::uint16 pad;
};

class CSpriteBase {
public:
	glm::uint16 car;
	glm::uint16 ped;
	glm::uint16 codeObj;
	glm::uint16 mapObj;
	glm::uint16 user;
	glm::uint16 font;
};

class CPaletteBase {
public:
	glm::uint16 tile;
	glm::uint16 sprite;
	glm::uint16 carRemap;
	glm::uint16 pedRemap;
	glm::uint16 codeObjRemap;
	glm::uint16 mapObjRemap;
	glm::uint16 userRemap;
	glm::uint16 fontRemap;
};

class CPaletteIndex {
public:
	std::vector<glm::uint16> physPalette;
};

class CPhysicalPalette {
public:
	glm::uint8 colors[256][4];
};

class CDelta {
public:
	glm::uint16 offset;
	glm::uint8 length;
	std::vector<glm::uint8> data;
};

class CDeltaEntry {
	glm::uint16 whichSprite;
	glm::uint8 deltaCount;
	glm::uint8 pad;
	std::vector<glm::uint16> deltaSize;
};

class CDeltaSprite {
public:
	glm::uint16 sprite;
	glm::uint8 width;
	glm::uint8 height;
	std::vector<glm::uint16> data;
};

class CDoorInfo {
public:
	glm::int8 rx;
	glm::int8 ry;
};

class CCarInfo {
public:
	glm::uint8 model;
	glm::uint8 sprite;
	glm::uint8 w;
	glm::uint8 h;
	glm::uint8 numRemaps;
	glm::uint8 passengers;
	glm::uint8 wreck;
	glm::uint8 rating;
	glm::int8 frontWheelOffset;
	glm::int8 rearWheelOffset;
	glm::int8 frontWindowOffset;
	glm::int8 rearWindowOffset;
	glm::uint8 infoFlags;
	glm::uint8 infoFlags2;
	std::vector<glm::uint8> remap;
	glm::uint8 numDoors;
	std::vector<CDoorInfo> doors;
};

class CTileData {
public:
	glm::uint8 pixels[64][64];
};

class CSpriteData {
public:
	std::vector<glm::uint8> pixels;
};

class CStyGraphics {
public:
	CPaletteIndex paletteIndex;
	std::vector<CPhysicalPalette> physicalPalette;
	CPaletteBase paletteBase;
	std::vector<CTileData> tileData;
	std::vector<CSpriteData> spriteData;
	std::vector<glm::uint8> spriteGraphics;
	std::vector<CSpriteEntry> spriteIndex;
	CSpriteBase spriteBase;
	std::vector<CDelta> deltaStore;
	std::vector<CDeltaEntry> deltaIndex;
	std::vector<CDeltaSprite> deltaSprites;
	std::vector<CCarInfo> carInfo;
	CFontBase fontBase;
};

class CStyle : CGBH {
public:
	bool m_bFileParsed;
	bool m_bBuildComplete;
	std::shared_ptr<CStyGraphics> m_pGraphics;
	std::shared_ptr<ATexture2D> m_pTextureAtlas;

	std::vector<std::shared_ptr<ATexture2D>> m_pSprites;
	std::vector<std::shared_ptr<ATexture2D>> m_pTextures;

public:
	CStyle();
	CStyle(std::string const& fileName);
	~CStyle();

	void Clear();

private:
	void Read(std::string const& fileName);
	void ReadPALX();
	void ReadPPAL();
	void ReadPALB();
	void ReadTILE();
	void ReadSPRG();
	void ReadSPRX();
	void ReadSPRB();
	void ReadDELS();
	void ReadDELX();
	void ReadFONB();
	void ReadCARI();
	void ReadOBJI();
	void ReadPSXT();
	void ReadRECY();
	void ReadSPEC();

	void BuildTextures();
	void BuildTextureAtlas();
	void BuildSprites();

	void WriteTiles(glm::uint32 i, glm::uint8& w, glm::uint8& h, std::vector<glm::uint32>& pixels);
	void WriteSprites(glm::uint32 i, glm::uint8& w, glm::uint8& h, std::vector<glm::uint32>& pixels);

public:
	std::vector<glm::uint8> GetSingleSpriteData(glm::int32 sprite);
	CPhysicalPalette GetSpritePalette(glm::int32 sprite, glm::int32 type, glm::int32 remap);
	std::vector<std::shared_ptr<ATexture2D>>& GetSprite() { return m_pSprites; }
	std::vector<std::shared_ptr<ATexture2D>>& GetTexture() { return m_pTextures; }
	std::shared_ptr<ATexture2D>& GetTextureAtlas() { return m_pTextureAtlas; }
	glm::uint32 const& GetBaseIndex(eBaseIndices b);
	glm::uint32 const& GetFontBaseIndex(glm::uint8 fontStyle);

	void BuildEverything();

public:
	bool IsLoading() { return !m_bFileParsed && !m_bBuildComplete; }

};

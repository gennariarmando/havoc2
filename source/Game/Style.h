#pragma once
#include "AFileMgr.h"
#include "ASprite.h"		

enum {
	STY_VERSION = 0x2BC
};

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

struct tFontBase {
	glm::uint16 fontCount;
	std::vector<glm::uint16> base;
};

struct tSpriteEntry {
	glm::uint32 ptr;
	glm::uint8 w;
	glm::uint8 h;
	glm::uint16 pad;
};

struct tSpriteBase {
	glm::uint16 car;
	glm::uint16 ped;
	glm::uint16 codeObj;
	glm::uint16 mapObj;
	glm::uint16 user;
	glm::uint16 font;
};

struct tPaletteBase {
	glm::uint16 tile;
	glm::uint16 sprite;
	glm::uint16 carRemap;
	glm::uint16 pedRemap;
	glm::uint16 codeObjRemap;
	glm::uint16 mapObjRemap;
	glm::uint16 userRemap;
	glm::uint16 fontRemap;
};

struct tPaletteIndex {
	std::vector<glm::uint16> physPalette;
};

struct tPhysicalPalette {
	glm::uint8 colors[256][4];
};

struct tDelta {
	glm::uint16 offset;
	glm::uint8 length;
	std::vector<glm::uint8> data;
};

struct tDeltaEntry {
	glm::uint16 whichSprite;
	glm::uint8 deltaCount;
	glm::uint8 pad;
	std::vector<glm::uint16> deltaSize;
};

struct tDeltaSprite {
	glm::uint16 sprite;
	glm::uint8 width;
	glm::uint8 height;
	std::vector<glm::uint16> data;
};

struct tDoorInfo {
	glm::int8 rx;
	glm::int8 ry;
};

struct tCarInfo {
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
	std::vector<tDoorInfo> doors;
};

struct tTileData {
	glm::uint8 pixels[64][64];
};

struct tSpriteData {
	std::vector<glm::uint8> pixels;
};

struct tStyGraphics {
	tPaletteIndex paletteIndex;
	std::vector<tPhysicalPalette> physicalPalette;
	tPaletteBase paletteBase;
	std::vector<tTileData> tileData;
	std::vector<tSpriteData> spriteData;
	std::vector<glm::uint8> spriteGraphics;
	std::vector<tSpriteEntry> spriteIndex;
	tSpriteBase spriteBase;
	std::vector<tDelta> deltaStore;
	std::vector<tDeltaEntry> deltaIndex;
	std::vector<tDeltaSprite> deltaSprites;
	std::vector<tCarInfo> carInfo;
	tFontBase fontBase;
};

class CStyle {
public:
	bool m_bFileParsed;
	bool m_bBuildComplete;
	tStyGraphics* m_pGraphics;
	ATexture2D* m_pTextureAtlas;
	std::vector<ATexture2D*> m_vSprites;

public:
	CStyle();
	~CStyle();

	bool Load(std::string const& fileName);

private:
	void LoadPALX(glm::uint64 length, AFileMgr& file);
	void LoadPPAL(glm::uint64 length, AFileMgr& file);
	void LoadPALB(glm::uint64 length, AFileMgr& file);
	void LoadTILE(glm::uint64 length, AFileMgr& file);
	void LoadSPRG(glm::uint64 length, AFileMgr& file);
	void LoadSPRX(glm::uint64 length, AFileMgr& file);
	void LoadSPRB(glm::uint64 length, AFileMgr& file);
	void LoadDELS(glm::uint64 length, AFileMgr& file);
	void LoadDELX(glm::uint64 length, AFileMgr& file);
	void LoadFONB(glm::uint64 length, AFileMgr& file);
	void LoadCARI(glm::uint64 length, AFileMgr& file);
	void LoadOBJI(glm::uint64 length, AFileMgr& file);
	void LoadPSXT(glm::uint64 length, AFileMgr& file);
	void LoadRECY(glm::uint64 length, AFileMgr& file);
	void LoadSPEC(glm::uint64 length, AFileMgr& file);

	void BuildTextures();
	void BuildTextureAtlas();
	void BuildSprites();

	void WriteTiles(glm::uint32 id, glm::uint8& w, glm::uint8& h, std::vector<glm::uint32>& pixels);
	void WriteSprites(glm::uint32 id, glm::uint8& w, glm::uint8& h, std::vector<glm::uint32>& pixels);

public:
	std::vector<glm::uint8> GetSingleSpriteData(glm::int32 sprite);
	tPhysicalPalette GetSpritePalette(glm::int32 sprite, glm::int32 type, glm::int32 remap);
	std::vector<ATexture2D*> GetSprite() { return m_vSprites; }
	ATexture2D* GetTextureAtlas() { return m_pTextureAtlas; }
	glm::uint32 GetBaseIndex(eBaseIndices b);
	glm::uint32 GetFontBaseIndex(glm::uint8 fontStyle);

	void BuildEverything();

public:
	bool IsLoading() { return !m_bFileParsed && !m_bBuildComplete; }

};

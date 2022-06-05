#pragma once
#include "GBH.h"
#include "Geometry.h"
#include "Style.h"

enum eFaceType {
	FACETYPE_LEFT,
	FACETYPE_RIGHT,
	FACETYPE_TOP,
	FACETYPE_BOTTOM,
	FACETYPE_LID,
	NUM_FACETYPES,
};

enum eZoneType {
	ZONETYPE_GENERAL_PURPOSE,
	ZONETYPE_NAVIGATION,
	ZONETYPE_TRAFFIC_LIGHT,
	ZONETYPE_UNK3,
	ZONETYPE_UNK4,
	ZONETYPE_ARROW_BLOCKER,
	ZONETYPE_RAILWAY_STATION,
	ZONETYPE_BUS_STOP,
	ZONETYPE_GENERAL_TRIGGER,
	ZONETYPE_UNK9,
	ZONETYPE_INFORMATION,
	ZONETYPE_RAILWAY_STATION_ENTRY_POINT,
	ZONETYPE_RAILWAY_STATION_EXIT_POINT,
	ZONETYPE_RAILWAY_STOP_POINT,
	ZONETYPE_GANG,
	ZONETYPE_LOCAL_NAVIGATION,
	ZONETYPE_RESTART,
	ZONETYPE_UNK17,
	ZONETYPE_UNK18,
	ZONETYPE_UNK19,
	ZONETYPE_ARREST_RESTART,
};

enum eSlopeType {
	SLOPETYPE_NONE = 0,
	SLOPETYPE_UP26LOW = 1,
	SLOPETYPE_UP26HIGH = 2,
	SLOPETYPE_DOWN26LOW = 3,
	SLOPETYPE_DOWN26HIGH = 4,
	SLOPETYPE_LEFT26LOW = 5,
	SLOPETYPE_LEFT26HIGH = 6,
	SLOPETYPE_RIGHT26LOW = 7,
	SLOPETYPE_RIGHT26HIGH = 8,
	SLOPETYPE_UP7LOW = 9,
	SLOPETYPE_UP7HIGH0 = 10,
	SLOPETYPE_UP7HIGH1 = 11,
	SLOPETYPE_UP7HIGH2 = 12,
	SLOPETYPE_UP7HIGH3 = 13,
	SLOPETYPE_UP7HIGH4 = 14,
	SLOPETYPE_UP7HIGH5 = 15,
	SLOPETYPE_UP7HIGH6 = 16,
	SLOPETYPE_DOWN7LOW = 17,
	SLOPETYPE_DOWN7HIGH0 = 18,
	SLOPETYPE_DOWN7HIGH1 = 19,
	SLOPETYPE_DOWN7HIGH2 = 20,
	SLOPETYPE_DOWN7HIGH3 = 21,
	SLOPETYPE_DOWN7HIGH4 = 22,
	SLOPETYPE_DOWN7HIGH5 = 23,
	SLOPETYPE_DOWN7HIGH6 = 24,
	SLOPETYPE_LEFT7LOW = 25,
	SLOPETYPE_LEFT7HIGH0 = 26,
	SLOPETYPE_LEFT7HIGH1 = 27,
	SLOPETYPE_LEFT7HIGH2 = 28,
	SLOPETYPE_LEFT7HIGH3 = 29,
	SLOPETYPE_LEFT7HIGH4 = 30,
	SLOPETYPE_LEFT7HIGH5 = 31,
	SLOPETYPE_LEFT7HIGH6 = 32,
	SLOPETYPE_RIGHT7LOW = 33,
	SLOPETYPE_RIGHT7HIGH0 = 34,
	SLOPETYPE_RIGHT7HIGH1 = 35,
	SLOPETYPE_RIGHT7HIGH2 = 36,
	SLOPETYPE_RIGHT7HIGH3 = 37,
	SLOPETYPE_RIGHT7HIGH4 = 38,
	SLOPETYPE_RIGHT7HIGH5 = 39,
	SLOPETYPE_RIGHT7HIGH6 = 40,
	SLOPETYPE_UP45 = 41,
	SLOPETYPE_DOWN45 = 42,
	SLOPETYPE_LEFT45 = 43,
	SLOPETYPE_RIGHT45 = 44,
	SLOPETYPE_DIAGONALFACINGUPLEFT = 45,
	SLOPETYPE_DIAGONALFACINGUPRIGHT = 46,
	SLOPETYPE_DIAGONALFACINGDOWNLEFT = 47,
	SLOPETYPE_DIAGONALFACINGDOWNRIGHT = 48,
	SLOPETYPE_DIAGONALSLOPEFACINGUPLEFT = 49,
	SLOPETYPE_DIAGONALSLOPEFACINGUPRIGHT = 50,
	SLOPETYPE_DIAGONALSLOPEFACINGDOWNLEFT = 51,
	SLOPETYPE_DIAGONALSLOPEFACINGDOWNRIGHT = 52,
	SLOPETYPE_PARTIALBLOCKLEFT = 53,
	SLOPETYPE_PARTIALBLOCKRIGHT = 54,
	SLOPETYPE_PARTIALBLOCKTOP = 55,
	SLOPETYPE_PARTIALBLOCKBOTTOM = 56,
	SLOPETYPE_PARTIALBLOCKTOPLEFT = 57,
	SLOPETYPE_PARTIALBLOCKTOPRIGHT = 58,
	SLOPETYPE_PARTIALBLOCKBOTTOMRIGHT = 59,
	SLOPETYPE_PARTIALBLOCKBOTTOMLEFT = 60,
	SLOPETYPE_PARTIALCENTREBLOCK = 61,
	SLOPETYPE_UNUSEDSLOPE = 62,
	SLOPETYPE_SLOPEABOVE = 63,
	SLOPETYPE_DIAGONALSLOPEFACINGUPLEFTNOZERO = 64,
	SLOPETYPE_DIAGONALSLOPEFACINGUPRIGHTNOZERO = 65,
	SLOPETYPE_DIAGONALSLOPEFACINGDOWNLEFTNOZERO = 66,
	SLOPETYPE_DIAGONALSLOPEFACINGDOWNRIGHTNOZERO = 67,
};

enum {
	MAP_BLOCK_SIZE = 1,
	MAP_NUM_BLOCKS_X = 16,
	MAP_NUM_BLOCKS_Y = 16,
	MAP_NUM_BLOCKS_Z = 8,

	MAP_SCALE_X = MAP_BLOCK_SIZE * MAP_NUM_BLOCKS_X * MAP_NUM_BLOCKS_X,
	MAP_SCALE_Y = MAP_BLOCK_SIZE * MAP_NUM_BLOCKS_Y * MAP_NUM_BLOCKS_Y,
	MAP_SCALE_Z = MAP_BLOCK_SIZE * MAP_NUM_BLOCKS_Z,

	MAP_SIZE = (MAP_SCALE_X * MAP_SCALE_Y),
};

class CMapLights {
public:
	glm::uint8 argb;
	glm::int16 x, y, z;
	glm::int16 radius;
	glm::uint8 intensity;
	glm::uint8 shape;
	glm::uint8 timeOn;
	glm::uint8 timeOff;
};

class CMapTileAnimation {
public:
	glm::uint16 base;
	glm::uint8 frameRate;
	glm::uint8 repeat;
	glm::uint8 animLength;
	glm::uint8 unused;
	glm::uint8 tiles[255];
};

class CMapPsxTable {
public:
	glm::uint8 tileNumber;
	glm::uint8 undefined;
};

class CMapObject {
public:
	glm::int16 x;
	glm::int16 y;
	glm::int8 rot;
	glm::uint8 objectType;
};

class CMapZone {
public:
	eZoneType zoneType;
	glm::uint8 x;
	glm::uint8 y;
	glm::uint8 w;
	glm::uint8 h;
	glm::uint8 nameLength;
	char name[255];
};

class CBlockInfo {
public:
	glm::uint16 face[NUM_FACETYPES];
	glm::uint8 arrows;
	glm::uint8 slopeType;
};

class CBlockDetails {
public:
	glm::uint32 tile;
	glm::uint32 wall;
	glm::uint32 bulletWall;
	bool flat;
	bool flip;
	glm::uint32 rotation;
	glm::uint32 lighting;
	CMapTileAnimation anim;
};

class CBlockInfoDetailed {
public:
	CBlockInfo info;
	CBlockDetails details[NUM_FACETYPES];
	glm::uint8 groundType;
	glm::uint8 slopeType;
};

class CColumnInfo {
public:
	glm::uint8 height;
	glm::uint8 offset;
	glm::uint16 pad;
	glm::uint32 blockd[8];
};

class CMap : CGBH {
private:
	std::vector<std::vector<std::vector<CBlockInfo>>> m_vCityBlocks;
	std::vector<CGeometry> m_vGeometryChunks;
	CGeometry m_ChunkBuffer;
	CStyle m_Style;

public:
	CMap() {}
	CMap(std::string const& fileName, std::string const& styFileName);
	~CMap();

private:
	void Read(std::string const& fileName, std::string const& styFileName);
	void Read32BitMap();
	void ReadZones();
	void ReadObjects();
	void ReadAnimations();
	void ReadLights();

	void BuildMap(std::vector<std::vector<glm::uint32>> base, std::vector<glm::uint32> columns, std::vector<CBlockInfo> blocks);

	bool CheckBit(glm::int32 const& value, glm::int32 const& bitOffset);
	glm::vec2 RotateUV(glm::vec2 uv, float rotation, glm::vec2 center);
	void AddBlock(CBlockInfoDetailed block, glm::vec3 offset);
	void EditBlock(CBlockInfoDetailed block);
	void AddFace(glm::uint32 slopeType, glm::uint8 faceType, glm::uint32 tile, glm::uint32 rot, bool flip, bool flat, glm::vec3 offset);
	void EditFace(glm::int32 base, glm::uint8 frameRate, bool repeat, glm::uint8 length, std::vector<glm::int32> tiles);

public:
	void Render();

};

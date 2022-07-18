#include "Map.h"
#include "Common.h"
#include "Config.h"
#include "Frontend.h"
#include "ACamera.h"
#include "AGraphicDevice.h"
#include "AEngine.h"
#include "AScreen.h"
#include "AConsole.h"
#include "Font.h"
#include "LoadingScreen.h"
#include "World.h"
#include "Collision.h"

CMap::CMap() {
	m_bFileParsed = false;
	m_bBuildComplete = false;
	m_pCompressedMap = new tCompressedMap();
	m_vAnimations = new std::vector<tTileAnimation>();
	m_vZones = new std::vector<tMapZone>();
	m_vObjects = new std::vector<tMapObject>();
	m_vLights = new std::vector<tMapLight>();
	m_vAnimatedFaces.resize(MAP_NUM_BLOCKS_X * MAP_NUM_BLOCKS_Y, std::vector<tFaceInfo>(0));
	m_vCachedAnims = {};

	m_vChunkGeometry.reserve(MAP_NUM_BLOCKS_X * MAP_NUM_BLOCKS_Y);
	m_pCollisionBody = nullptr;

	m_VertexBuffer = {};
}

CMap::~CMap() {
	delete m_pCompressedMap;
	delete m_vAnimations;
	delete m_vZones;
	delete m_vObjects;
	delete m_vLights;
	m_vAnimatedFaces = {};
	m_vCachedAnims = {};

	m_vChunkGeometry = {};

	if (m_pCollisionBody)
		delete m_pCollisionBody;

	m_VertexBuffer = {};
}

bool CMap::Load(std::string const& fileName) {
	if (m_bFileParsed)
		return true;

	AFileMgr file;
	if (!file.Open(fileName)) {
		return false;
	}

	char header[4] = {};
	file.ReadCustom(header, 4);

	glm::uint16 version = file.ReadUInt16();

	if (strncmp(header, "GBMP", 4) || version != GMP_VERSION) {
		return false;
	}

	while (file.GetPosition() < file.GetSize()) {
		char chunk[4] = {};
		chunk[0] = file.ReadUInt8();
		chunk[1] = file.ReadUInt8();
		chunk[2] = file.ReadUInt8();
		chunk[3] = file.ReadUInt8();

		glm::uint64 chunkSize = file.ReadUInt32();

		if (chunkSize != 0) {
			if (!strncmp(chunk, "DMAP", 4))
				LoadDMAP(chunkSize, file);
			else if (!strncmp(chunk, "ZONE", 4))
				LoadZONE(chunkSize, file);
			else if (!strncmp(chunk, "MOBJ", 4))
				LoadMOBJ(chunkSize, file);
			else if (!strncmp(chunk, "ANIM", 4))
				LoadANIM(chunkSize, file);
			else if (!strncmp(chunk, "LGHT", 4))
				LoadLGHT(chunkSize, file);
			else
				file.Seek(chunkSize);
		}
	}

	file.Close();

	m_bFileParsed = true;
	BuildEverything();

	return true;
}

void CMap::LoadDMAP(glm::uint64 length, AFileMgr& file) {
	std::vector<glm::uint32> base(MAP_SCALE_X * MAP_SCALE_Y);
	file.ReadCustom(base.data(), sizeof(glm::uint32) * base.size());

	glm::uint32 columnWords = file.ReadUInt32();
	std::vector<glm::uint32> column(columnWords);
	file.ReadCustom(column.data(), sizeof(glm::uint32) * column.size());

	glm::uint32 numBlocks = file.ReadUInt32();
	std::vector<tBlockInfoDetailed> blocks(numBlocks);

	for (auto& blockInfo : blocks) {
		tBlockInfo block = {};
		block.face[FACETYPE_LEFT] = file.ReadUInt16();
		block.face[FACETYPE_RIGHT] = file.ReadUInt16();
		block.face[FACETYPE_TOP] = file.ReadUInt16();
		block.face[FACETYPE_BOTTOM] = file.ReadUInt16();
		block.face[FACETYPE_LID] = file.ReadUInt16();
		block.arrows = file.ReadUInt8();
		block.slopeType = file.ReadUInt8();

		ParseBlockInfo(block, blockInfo, FACETYPE_LEFT);
		ParseBlockInfo(block, blockInfo, FACETYPE_RIGHT);
		ParseBlockInfo(block, blockInfo, FACETYPE_TOP);
		ParseBlockInfo(block, blockInfo, FACETYPE_BOTTOM);
		ParseBlockInfo(block, blockInfo, FACETYPE_LID);
	}
	m_pCompressedMap->base = base;
	m_pCompressedMap->columnWords = columnWords;
	m_pCompressedMap->column = column;
	m_pCompressedMap->numBlocks = numBlocks;
	m_pCompressedMap->blocks = blocks;
}

void CMap::LoadZONE(glm::uint64 length, AFileMgr& file) {
	glm::uint64 pos = 0;
	while (pos < length) {
		tMapZone zone = {};
		zone.zoneType = static_cast<eZoneType>(file.ReadInt8());
		zone.x = static_cast<float>(file.ReadInt8());
		zone.y = static_cast<float>(file.ReadInt8());
		zone.w = static_cast<float>(file.ReadInt8());
		zone.h = static_cast<float>(file.ReadInt8());
		glm::int8 nameLength = file.ReadUInt8();

		for (int i = 0; i < nameLength; i++)
			zone.name += file.ReadUInt8();

		m_vZones->push_back(zone);
		pos += 6 + nameLength;
	}
}

void CMap::LoadMOBJ(glm::uint64 length, AFileMgr& file) {
	glm::uint64 startOffset = file.GetPosition();
	while (file.GetPosition() < startOffset + length) {
		tMapObject object = {};

		object.x = file.ReadInt16();
		object.y = file.ReadInt16();
		object.rot = file.ReadInt8();
		object.objectType = file.ReadUInt8();
	}
}

void CMap::LoadANIM(glm::uint64 length, AFileMgr& file) {
	glm::uint64 startOffset = file.GetPosition();
	while (file.GetPosition() < startOffset + length) {
		tTileAnimation anim = {};
		anim.base = file.ReadUInt16();
		anim.frameRate = file.ReadUInt8();
		anim.repeat = file.ReadUInt8();

		glm::uint8 animLength = file.ReadUInt8();
		file.ReadUInt8();

		for (glm::int32 i = 0; i < animLength; i++)
			anim.tiles.push_back(file.ReadUInt16());

		m_vAnimations->push_back(anim);
	}
}

void CMap::LoadLGHT(glm::uint64 length, AFileMgr& file) {
	glm::uint64 startOffset = file.GetPosition();
	while (file.GetPosition() < startOffset + length) {
		tMapLight light = {};

		light.color.a = file.ReadUInt8() / 255.0f;
		light.color.r = file.ReadUInt8() / 255.0f;
		light.color.g = file.ReadUInt8() / 255.0f;
		light.color.b = file.ReadUInt8() / 255.0f;

		light.pos.x = static_cast<float>(file.ReadUInt16());
		light.pos.y = static_cast<float>(file.ReadUInt16());
		light.pos.z = static_cast<float>(file.ReadUInt16());

		light.radius = static_cast<float>(file.ReadUInt16());

		light.intensity = file.ReadUInt8();
		light.shape = file.ReadUInt8();
		light.timeOn = file.ReadUInt8();
		light.timeOff = file.ReadUInt8();

		m_vLights->push_back(light);
	}
}

void CMap::ParseBlockInfo(tBlockInfo& block, tBlockInfoDetailed& info, glm::uint32 faceType) {
	for (glm::uint32 k = 0; k < 10; k++) {
		info.details[faceType].tile += (block.face[faceType] & static_cast<glm::int32>(glm::pow(2, k)));
	}

	if (faceType == FACETYPE_LID) {
		glm::uint32 lit1 = 0;
		glm::uint32 lit2 = 0;
		lit1 = GetBit(block.face[faceType], 10);
		lit2 = GetBit(block.face[faceType], 11);

		if (!lit1 && !lit2)
			info.details[faceType].lighting = 0;

		if (lit1 && !lit2)
			info.details[faceType].lighting = 1;

		if (lit1 && lit2)
			info.details[faceType].lighting = 2;

		if (lit1 && lit2)
			info.details[faceType].lighting = 3;
	}
	else {
		info.details[faceType].lighting = 0;

		info.details[faceType].wall = GetBit(block.face[faceType], 10);
		info.details[faceType].bulletWall = GetBit(block.face[faceType], 11);
	}

	info.details[faceType].flat = GetBit(block.face[faceType], 12);

	switch (faceType) {
	case FACETYPE_LEFT:
		info.details[FACETYPE_LEFT].oppositeFlat = GetBit(block.face[FACETYPE_RIGHT], 12);
		break;
	case FACETYPE_TOP:
		info.details[FACETYPE_TOP].oppositeFlat = GetBit(block.face[FACETYPE_BOTTOM], 12);
		break;
	case FACETYPE_RIGHT:
		info.details[FACETYPE_RIGHT].oppositeFlat = GetBit(block.face[FACETYPE_LEFT], 12);
		break;
	case FACETYPE_BOTTOM:
		info.details[FACETYPE_BOTTOM].oppositeFlat = GetBit(block.face[FACETYPE_TOP], 12);
		break;
	default:
		info.details[FACETYPE_LEFT].oppositeFlat = false;
		break;
	}
	info.details[faceType].flip = GetBit(block.face[faceType], 13);

	glm::uint32 rot1 = GetBit(block.face[faceType], 14);
	glm::uint32 rot2 = GetBit(block.face[faceType], 15);

	if (rot1 && !rot2)
		info.details[faceType].rotation = 1;
	else if (!rot1 && rot2)
		info.details[faceType].rotation = 2;
	else if (rot1 && rot2)
		info.details[faceType].rotation = 3;
	else
		info.details[faceType].rotation = 0;

	glm::uint32 groundType = 0;
	groundType += (block.slopeType & 1);
	groundType += (block.slopeType & 2);
	info.groundType = groundType;

	glm::uint32 slopeType = 0;
	for (glm::uint32 k = 2; k < 8; k++) {
		if (GetBit(block.slopeType, k))
			slopeType += static_cast<glm::uint32>(glm::pow(2, k - 2));
	}
	info.slopeType = slopeType;
}

void CMap::BuildEverything() {
	if (!m_bFileParsed)
		return;

	Console.WriteLine("Building chunks...");
	BuildChunks();

	m_bBuildComplete = true;
}

void CMap::BuildChunks() {
	glm::uint32 index = 0;
	std::vector<tCachedAnims> cachedAnims;

	if (!m_pCollisionBody) {
		m_pCollisionBody = new ARigidBody();
		m_pCollisionBody->SetType(BODYTYPE_STATIC);
		m_pCollisionBody->SetPosition({ 0.0f, 0.0f, 0.0f });
	}

	glm::uint32 chunkIndex = 0;
	for (glm::int32 i = 0; i < MAP_SCALE_Y / MAP_NUM_BLOCKS_Y; i++) {
		for (glm::int32 j = 0; j < MAP_SCALE_X / MAP_NUM_BLOCKS_X; j++) {
			for (glm::int32 y = 0; y < MAP_NUM_BLOCKS_Y; y++) {
				for (glm::int32 x = 0; x < MAP_NUM_BLOCKS_X; x++) {
					glm::int32 columnIndex = m_pCompressedMap->base.at((y + i * MAP_NUM_BLOCKS_Y) * 256 + x + j * MAP_NUM_BLOCKS_X);
					glm::int32 height = m_pCompressedMap->column.at(columnIndex) & 0xFF;
					glm::int32 offset = (m_pCompressedMap->column.at(columnIndex) & 0xFF00) >> 8;
					for (glm::int32 z = 0; z < height; z++) {
						if (z < offset)
							continue;

						tBlockInfoDetailed& block = m_pCompressedMap->blocks.at(m_pCompressedMap->column.at(columnIndex + z - offset + 1));

						switch (block.slopeType) {
						case SLOPETYPE_SLOPEABOVE:
							block.slopeType = SLOPETYPE_NONE;
							break;
						case SLOPETYPE_DIAGONALSLOPEFACINGUPLEFT:
						case SLOPETYPE_DIAGONALSLOPEFACINGUPRIGHT:
						case SLOPETYPE_DIAGONALSLOPEFACINGDOWNLEFT:
						case SLOPETYPE_DIAGONALSLOPEFACINGDOWNRIGHT:
							if ((block.details[FACETYPE_LID].tile != 0) && (block.details[FACETYPE_LID].tile) == 1023)
								block.slopeType = (block.slopeType - SLOPETYPE_DIAGONALSLOPEFACINGUPLEFT) + SLOPETYPE_DIAGONALSLOPEFACINGUPLEFTNOZERO;
							break;
						}

						for (glm::uint32 faceType = 0; faceType < NUM_FACETYPES; faceType++) {
							bool found = false;
							for (glm::uint32 animCount = 0; animCount < m_vAnimations->size(); animCount++) {
								tTileAnimation& anim = m_vAnimations->at(animCount);

								if (anim.base && anim.base < 992 && anim.base == block.details[faceType].tile) {
									for (glm::uint32 i = 0; i < cachedAnims.size(); i++) {
										if (cachedAnims.at(i).tile != -1 && cachedAnims.at(i).tile == block.details[faceType].tile && cachedAnims.at(i).flipBook) {
											found = true;
											block.details[faceType].flipBook = cachedAnims.at(i).flipBook;
											break;
										}
									}
									if (found)
										break;

									if (!block.details[faceType].flipBook) {
										block.details[faceType].flipBook = new AFlipbook();
										block.details[faceType].flipBook->Set(anim.tiles, anim.repeat, anim.frameRate / ANIMATIONS_FRAME_RATE);

										tCachedAnims t;
										t.tile = block.details[faceType].tile;
										t.flipBook = block.details[faceType].flipBook;
										cachedAnims.push_back(t);
									}
									break;
								}
							}
						}

						glm::uint32 chunkIndex = i * MAP_NUM_BLOCKS_X + j;
						glm::vec3 offset = glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
						AddBlock(chunkIndex, block, offset, offset, index, nullptr);

						auto hasValidTile = [](tBlockInfoDetailed b) {
							return (b.details[FACETYPE_LEFT].tile && b.details[FACETYPE_LEFT].tile < 992) ||
								(b.details[FACETYPE_TOP].tile && b.details[FACETYPE_TOP].tile < 992) ||
								(b.details[FACETYPE_RIGHT].tile && b.details[FACETYPE_RIGHT].tile < 992) ||
								(b.details[FACETYPE_BOTTOM].tile && b.details[FACETYPE_BOTTOM].tile < 992);
						};

						if (block.groundType != GROUNDTYPE_AIR && hasValidTile(block)) {
							glm::vec3 halfExtend = { 0.5f, 0.5f, 0.5f };
							m_pCollisionBody->AddCollisionTypeBox(offset + halfExtend, halfExtend);
						}
					}
				}
			}
			m_vChunkGeometry.push_back(m_VertexBuffer);
			m_VertexBuffer = { };

			index = 0;
		}
	}

	m_vCachedAnims = cachedAnims;
}

void CMap::Render(CStyle* style) {
	const glm::int32 visibleChunks = 2;
	glm::int32 startx = (static_cast<glm::int32>(Camera.GetPosition().x + (MAP_NUM_BLOCKS_X * 0.5f)) / MAP_NUM_BLOCKS_X) - (visibleChunks);
	glm::int32 endx = (static_cast<glm::int32>(Camera.GetPosition().x + (MAP_NUM_BLOCKS_X * 0.5f)) / MAP_NUM_BLOCKS_X) + (visibleChunks);
	glm::int32 starty = (static_cast<glm::int32>(Camera.GetPosition().y + (MAP_NUM_BLOCKS_Y * 0.5f)) / MAP_NUM_BLOCKS_Y) - (visibleChunks);
	glm::int32 endy = (static_cast<glm::int32>(Camera.GetPosition().y + (MAP_NUM_BLOCKS_Y * 0.5f)) / MAP_NUM_BLOCKS_Y) + (visibleChunks);

	startx = Clamp(startx, 0, MAP_NUM_BLOCKS_X);
	endx = Clamp(endx, 0, MAP_NUM_BLOCKS_X);
	starty = Clamp(starty, 0, MAP_NUM_BLOCKS_Y);
	endy = Clamp(endy, 0, MAP_NUM_BLOCKS_Y);

	for (auto& it : m_vCachedAnims) {
		if (it.flipBook)
			it.flipBook->Update();
	}

	for (glm::int32 i = starty; i < endy; i++) {
		for (glm::int32 j = startx; j < endx; j++) {
			glm::uint32 chunkIndex = i * MAP_NUM_BLOCKS_X + j;
			AVertexBuffer* chunk = &m_vChunkGeometry.at(chunkIndex);

			std::vector<tFaceInfo>& animatedFaces = m_vAnimatedFaces.at(i * MAP_NUM_BLOCKS_X + j);
			
			for (auto& it : animatedFaces) {
				EditFace(chunk, &it);
			}

			glm::vec3 pos = { static_cast<float>(j * MAP_NUM_BLOCKS_X), static_cast<float>(i * MAP_NUM_BLOCKS_Y), 0.0f };

			chunk->SetUpdateData(true);
			chunk->SetPrimitive(GL_TRIANGLES);
			chunk->SetProjection(Camera.GetProjection());
			chunk->SetView(Camera.GetView());

			chunk->SetLocation(pos);
			chunk->SetRotation(0.0f, 0.0f, 1.0f, 0.0f);
			chunk->SetScale(1.0f, 1.0f, 1.0f);
			chunk->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			chunk->SetTexture(style->GetTextureAtlas()->GetID());
			chunk->SetShader(GraphicDevice.m_vShaders.at(SHADER_MESHES));
			chunk->RenderPrimitives();
		}
	}
}

void CMap::AddBlock(glm::uint32 chunkIndex, tBlockInfoDetailed& block, glm::vec3 const& chunkOffset, glm::vec3 const& offset, glm::uint32& index, ARigidBody* colBody) {
	for (glm::int32 faceType = 0; faceType < NUM_FACETYPES; faceType++) {
		if (!block.details[faceType].tile || block.details[faceType].tile >= 992)
			continue;

		tFaceInfo* oppositeFace = NULL;
		tFaceInfo& detail = block.details[faceType];

		switch (faceType) {
		case FACETYPE_LEFT:
			oppositeFace = &block.details[FACETYPE_RIGHT];
			break;
		case FACETYPE_TOP:
			oppositeFace = &block.details[FACETYPE_BOTTOM];
			break;
		case FACETYPE_RIGHT:
			oppositeFace = &block.details[FACETYPE_LEFT];
			break;
		case FACETYPE_BOTTOM:
			oppositeFace = &block.details[FACETYPE_TOP];
			break;
		}

		if (oppositeFace)
			detail.oppositeFlat = oppositeFace->flat;
		else
			detail.oppositeFlat = false;

		glm::uint32 tile = block.details[faceType].tile;
		if ((detail.oppositeFlat && !detail.flat && faceType != FACETYPE_LID)) {
			switch (faceType) {
			case FACETYPE_LEFT:
				tile = block.details[FACETYPE_RIGHT].tile;
				break;
			case FACETYPE_TOP:
				tile = block.details[FACETYPE_BOTTOM].tile;
				break;
			case FACETYPE_RIGHT:
				tile = block.details[FACETYPE_LEFT].tile;
				break;
			case FACETYPE_BOTTOM:
				tile = block.details[FACETYPE_TOP].tile;
				break;
			}
		}

		switch (block.slopeType) {
			//case SLOPETYPE_PARTIALBLOCKLEFT:
			//case SLOPETYPE_PARTIALBLOCKRIGHT:
			//case SLOPETYPE_PARTIALBLOCKTOPLEFT:
			//case SLOPETYPE_PARTIALBLOCKTOPRIGHT:
			//case SLOPETYPE_PARTIALBLOCKBOTTOMRIGHT:
			//case SLOPETYPE_PARTIALBLOCKBOTTOMLEFT:
			//case SLOPETYPE_PARTIALBLOCKTOP:
			//case SLOPETYPE_PARTIALBLOCKBOTTOM:
			//	break;
		case SLOPETYPE_DIAGONALSLOPEFACINGUPLEFTNOZERO:
		case SLOPETYPE_DIAGONALSLOPEFACINGDOWNLEFTNOZERO:
		case SLOPETYPE_DIAGONALSLOPEFACINGUPRIGHTNOZERO:
		case SLOPETYPE_DIAGONALSLOPEFACINGDOWNRIGHTNOZERO:
			AddFace(block.slopeType, FACETYPE_LID, tile, block.details[FACETYPE_LID].rotation, block.details[FACETYPE_LID].flip, false, false, offset, index);
			break;
		default:
			AddFace(block.slopeType, faceType, tile, block.details[faceType].rotation, block.details[faceType].flip, block.details[faceType].flat, block.details[faceType].oppositeFlat, offset, index);
			break;
		}
		block.details[faceType].index = index - 6;

		if (block.details[faceType].flipBook)
			m_vAnimatedFaces[chunkIndex].push_back(block.details[faceType]);
	}
}

bool CMap::GetVecFromSlopeType(glm::uint32 slopeType, glm::vec3& tl, glm::vec3& tr, glm::vec3& bl, glm::vec3& br) {
	bool reverse = false;
	switch (slopeType) {
	case SLOPETYPE_NONE:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_UP26LOW:
		tl = glm::vec3(0.0f, 0.0f, 0.5f);
		tr = glm::vec3(1.0f, 0.0f, 0.5f);
		bl = glm::vec3(0.0f, 1.0f, 0.0f);
		br = glm::vec3(1.0f, 1.0f, 0.0f);
		break;
	case SLOPETYPE_UP26HIGH:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 0.5f);
		br = glm::vec3(1.0f, 1.0f, 0.5f);
		break;
	case SLOPETYPE_DOWN26LOW:
		tl = glm::vec3(0.0f, 0.0f, 0.0f);
		tr = glm::vec3(1.0f, 0.0f, 0.0f);
		bl = glm::vec3(0.0f, 1.0f, 0.5f);
		br = glm::vec3(1.0f, 1.0f, 0.5f);
		break;
	case SLOPETYPE_DOWN26HIGH:
		tl = glm::vec3(0.0f, 0.0f, 0.5f);
		tr = glm::vec3(1.0f, 0.0f, 0.5f),
			bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_LEFT26LOW:
		tl = glm::vec3(0.0f, 0.0f, 0.5f);
		tr = glm::vec3(1.0f, 0.0f, 0.0f);
		bl = glm::vec3(0.0f, 1.0f, 0.5f);
		br = glm::vec3(1.0f, 1.0f, 0.0f);
		break;
	case SLOPETYPE_LEFT26HIGH:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 0.5f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 0.5f);
		break;
	case SLOPETYPE_RIGHT26LOW:
		tl = glm::vec3(0.0f, 0.0f, 0.0f);
		tr = glm::vec3(1.0f, 0.0f, 0.5f);
		bl = glm::vec3(0.0f, 1.0f, 0.0f);
		br = glm::vec3(1.0f, 1.0f, 0.5f);
		break;
	case SLOPETYPE_RIGHT26HIGH:
		tl = glm::vec3(0.0f, 0.0f, 0.5f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 0.5f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_UP7LOW:
	case SLOPETYPE_UP7HIGH0:
	case SLOPETYPE_UP7HIGH1:
	case SLOPETYPE_UP7HIGH2:
	case SLOPETYPE_UP7HIGH3:
	case SLOPETYPE_UP7HIGH4:
	case SLOPETYPE_UP7HIGH5:
	case SLOPETYPE_UP7HIGH6:
		tl = glm::vec3(0.0f, 0.0f, 0.125f * ((slopeType - SLOPETYPE_UP7LOW) + 1));
		tr = glm::vec3(1.0f, 0.0f, 0.125f * ((slopeType - SLOPETYPE_UP7LOW) + 1));
		bl = glm::vec3(0.0f, 1.0f, 0.125f * ((slopeType - SLOPETYPE_UP7LOW) + 0));
		br = glm::vec3(1.0f, 1.0f, 0.125f * ((slopeType - SLOPETYPE_UP7LOW) + 0));
		break;
	case SLOPETYPE_DOWN7LOW:
	case SLOPETYPE_DOWN7HIGH0:
	case SLOPETYPE_DOWN7HIGH1:
	case SLOPETYPE_DOWN7HIGH2:
	case SLOPETYPE_DOWN7HIGH3:
	case SLOPETYPE_DOWN7HIGH4:
	case SLOPETYPE_DOWN7HIGH5:
	case SLOPETYPE_DOWN7HIGH6:
		tl = glm::vec3(0.0f, 0.0f, 0.125f * ((slopeType - SLOPETYPE_DOWN7LOW) + 0));
		tr = glm::vec3(1.0f, 0.0f, 0.125f * ((slopeType - SLOPETYPE_DOWN7LOW) + 0));
		bl = glm::vec3(0.0f, 1.0f, 0.125f * ((slopeType - SLOPETYPE_DOWN7LOW) + 1));
		br = glm::vec3(1.0f, 1.0f, 0.125f * ((slopeType - SLOPETYPE_DOWN7LOW) + 1));
		break;
	case SLOPETYPE_LEFT7LOW:
	case SLOPETYPE_LEFT7HIGH0:
	case SLOPETYPE_LEFT7HIGH1:
	case SLOPETYPE_LEFT7HIGH2:
	case SLOPETYPE_LEFT7HIGH3:
	case SLOPETYPE_LEFT7HIGH4:
	case SLOPETYPE_LEFT7HIGH5:
	case SLOPETYPE_LEFT7HIGH6:
		tl = glm::vec3(0.0f, 0.0f, 0.125f * ((slopeType - SLOPETYPE_LEFT7LOW) + 1));
		tr = glm::vec3(1.0f, 0.0f, 0.125f * ((slopeType - SLOPETYPE_LEFT7LOW) + 0));
		bl = glm::vec3(0.0f, 1.0f, 0.125f * ((slopeType - SLOPETYPE_LEFT7LOW) + 1));
		br = glm::vec3(1.0f, 1.0f, 0.125f * ((slopeType - SLOPETYPE_LEFT7LOW) + 0));
		break;
	case SLOPETYPE_RIGHT7LOW:
	case SLOPETYPE_RIGHT7HIGH0:
	case SLOPETYPE_RIGHT7HIGH1:
	case SLOPETYPE_RIGHT7HIGH2:
	case SLOPETYPE_RIGHT7HIGH3:
	case SLOPETYPE_RIGHT7HIGH4:
	case SLOPETYPE_RIGHT7HIGH5:
	case SLOPETYPE_RIGHT7HIGH6:
		tl = glm::vec3(0.0f, 0.0f, 0.125f * ((slopeType - SLOPETYPE_RIGHT7LOW) + 0));
		tr = glm::vec3(1.0f, 0.0f, 0.125f * ((slopeType - SLOPETYPE_RIGHT7LOW) + 1));
		bl = glm::vec3(0.0f, 1.0f, 0.125f * ((slopeType - SLOPETYPE_RIGHT7LOW) + 0));
		br = glm::vec3(1.0f, 1.0f, 0.125f * ((slopeType - SLOPETYPE_RIGHT7LOW) + 1));
		break;
	case SLOPETYPE_UP45:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 0.0f);
		br = glm::vec3(1.0f, 1.0f, 0.0f);
		break;
	case SLOPETYPE_DOWN45:
		tl = glm::vec3(0.0f, 0.0f, 0.0f);
		tr = glm::vec3(1.0f, 0.0f, 0.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_LEFT45:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 0.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 0.0f);
		break;
	case SLOPETYPE_RIGHT45:
		tl = glm::vec3(0.0f, 0.0f, 0.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 0.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_DIAGONALFACINGUPLEFT:
		tl = glm::vec3(1.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		reverse = true;
		break;
	case SLOPETYPE_DIAGONALFACINGUPRIGHT:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(0.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_DIAGONALFACINGDOWNLEFT:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(1.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_DIAGONALFACINGDOWNRIGHT:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(0.0f, 1.0f, 1.0f);
		reverse = true;
		break;
	case SLOPETYPE_DIAGONALSLOPEFACINGUPLEFT:
		tl = glm::vec3(0.0f, 0.0f, 0.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		reverse = true;
		break;
	case SLOPETYPE_DIAGONALSLOPEFACINGUPRIGHT:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 0.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_DIAGONALSLOPEFACINGDOWNLEFT:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 0.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_DIAGONALSLOPEFACINGDOWNRIGHT:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 0.0f);
		reverse = true;
		break;
	case SLOPETYPE_PARTIALBLOCKLEFT:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(0.5f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(0.5f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_PARTIALBLOCKRIGHT:
		tl = glm::vec3(0.5f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.5f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_PARTIALBLOCKTOP:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 0.5f, 1.0f);
		br = glm::vec3(1.0f, 0.5f, 1.0f);
		break;
	case SLOPETYPE_PARTIALBLOCKBOTTOM:
		tl = glm::vec3(0.0f, 0.5f, 1.0f);
		tr = glm::vec3(1.0f, 0.5f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_PARTIALBLOCKTOPLEFT:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(0.5f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 0.5f, 1.0f);
		br = glm::vec3(0.5f, 0.5f, 1.0f);
		break;
	case SLOPETYPE_PARTIALBLOCKTOPRIGHT:
		tl = glm::vec3(0.5f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.5f, 0.5f, 1.0f);
		br = glm::vec3(1.0f, 0.5f, 1.0f);
		break;
	case SLOPETYPE_PARTIALBLOCKBOTTOMRIGHT:
		tl = glm::vec3(0.5f, 0.5f, 1.0f);
		tr = glm::vec3(1.0f, 0.5f, 1.0f);
		bl = glm::vec3(0.5f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_PARTIALBLOCKBOTTOMLEFT:
		tl = glm::vec3(0.0f, 0.5f, 1.0f);
		tr = glm::vec3(0.5f, 0.5f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(0.5f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_PARTIALCENTREBLOCK:
		tl = glm::vec3(0.25f, 0.25f, 1.0f);
		tr = glm::vec3(0.75f, 0.25f, 1.0f);
		bl = glm::vec3(0.25f, 0.75f, 1.0f);
		br = glm::vec3(0.75f, 0.75f, 1.0f);
		break;
	case SLOPETYPE_UNUSEDSLOPE:
		break;
	case SLOPETYPE_SLOPEABOVE:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 1.0f);
		br = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case SLOPETYPE_DIAGONALSLOPEFACINGUPLEFTNOZERO:
		tl = glm::vec3(1.0f, 1.0f, 1.0f);
		tr = glm::vec3(1.0f, 1.0f, 1.0f);
		bl = glm::vec3(1.0f, 0.0f, 0.0f);
		br = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case SLOPETYPE_DIAGONALSLOPEFACINGUPRIGHTNOZERO:
		tl = glm::vec3(0.0f, 1.0f, 1.0f);
		tr = glm::vec3(0.0f, 1.0f, 1.0f);
		bl = glm::vec3(1.0f, 1.0f, 0.0f);
		br = glm::vec3(0.0f, 0.0f, 0.0f);
		break;
	case SLOPETYPE_DIAGONALSLOPEFACINGDOWNLEFTNOZERO:
		tl = glm::vec3(1.0f, 0.0f, 1.0f);
		tr = glm::vec3(1.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 0.0f, 0.0f);
		br = glm::vec3(1.0f, 1.0f, 0.0f);
		break;
	case SLOPETYPE_DIAGONALSLOPEFACINGDOWNRIGHTNOZERO:
		tl = glm::vec3(0.0f, 0.0f, 1.0f);
		tr = glm::vec3(0.0f, 0.0f, 1.0f);
		bl = glm::vec3(0.0f, 1.0f, 0.0f);
		br = glm::vec3(1.0f, 0.0f, 0.0f);
		break;
	default:
		break;
	}

	return reverse;
}

void CMap::AddFace(glm::uint32 slopeType, glm::uint8 faceType, glm::uint32 tile, glm::uint32 rot, bool flip, bool flat, bool oppositeFlat, glm::vec3 offset, glm::uint32& index) {
	if (oppositeFlat && slopeType == SLOPETYPE_NONE) {
		glm::vec3 f[4] = {
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f,0.0f),
			glm::vec3(0.0f, 1.0f ,0.0f),
			glm::vec3(0.0f, -1.0f ,0.0f)
		};

		f[faceType].x *= 0.99f;
		f[faceType].y *= 0.99f;
		offset += f[faceType];
	}

	glm::vec3 tl = glm::vec3(0.0f);
	glm::vec3 tr = glm::vec3(0.0f);
	glm::vec3 bl = glm::vec3(0.0f);
	glm::vec3 br = glm::vec3(0.0f);

	bool reverse = GetVecFromSlopeType(slopeType, tl, tr, bl, br);

	std::vector<glm::vec3> pos;
	std::vector<glm::vec2> texCoords;

	switch (faceType) {
	case FACETYPE_LEFT:
		pos.push_back(glm::vec3(tl.x, tl.y, tl.z));
		pos.push_back(glm::vec3(bl.x, bl.y, bl.z));
		pos.push_back(glm::vec3(bl.x, bl.y, 0.0f));
		pos.push_back(glm::vec3(tl.x, tl.y, 0.0f));
		reverse = false;
		break;
	case FACETYPE_RIGHT:
		pos.push_back(glm::vec3(br.x, br.y, br.z));
		pos.push_back(glm::vec3(tr.x, tr.y, tr.z));
		pos.push_back(glm::vec3(tr.x, tr.y, 0.0f));
		pos.push_back(glm::vec3(br.x, br.y, 0.0f));
		reverse = false;
		break;
	case FACETYPE_TOP:
		pos.push_back(glm::vec3(tr.x, tr.y, tr.z));
		pos.push_back(glm::vec3(tl.x, tl.y, tl.z));
		pos.push_back(glm::vec3(tl.x, tl.y, 0.0f));
		pos.push_back(glm::vec3(tr.x, tr.y, 0.0f));
		reverse = false;
		break;
	case FACETYPE_BOTTOM:
		pos.push_back(glm::vec3(bl.x, bl.y, bl.z));
		pos.push_back(glm::vec3(br.x, br.y, br.z));
		pos.push_back(glm::vec3(br.x, br.y, 0.0f));
		pos.push_back(glm::vec3(bl.x, bl.y, 0.0f));
		reverse = false;
		break;
	case FACETYPE_LID:
		pos.push_back(glm::vec3(tl.x, tl.y, tl.z));
		pos.push_back(glm::vec3(tr.x, tr.y, tr.z));
		pos.push_back(glm::vec3(br.x, br.y, br.z));
		pos.push_back(glm::vec3(bl.x, bl.y, bl.z));
		break;
	}

	float cxy = (1.0f / 32.0f);
	float x = (tile % 32) * cxy;
	float y = (tile / 32) * cxy;
	float w = (x + cxy);
	float h = (y + cxy);

	texCoords.push_back(glm::vec2(x, y));
	texCoords.push_back(glm::vec2(w, y));
	texCoords.push_back(glm::vec2(w, h));
	texCoords.push_back(glm::vec2(x, h));

	if (flip) {
		std::vector<glm::vec2> texCoordsCopy = texCoords;
		texCoords.at(1) = texCoordsCopy.at(0); texCoords.at(0) = texCoordsCopy.at(1);
		texCoords.at(3) = texCoordsCopy.at(2); texCoords.at(2) = texCoordsCopy.at(3);
	}

	for (glm::uint32 i = 0; i < 4; i++) {
		texCoords[i] = RotateUV(texCoords[i], glm::radians(rot * 90.0f), glm::vec2(x + (cxy * 0.5f), y + (cxy * 0.5f)));
	}

	glm::uint8 defIndices[] = { 0, 2, 1, 0, 3, 2 };
	glm::uint8 revIndices[] = { 0, 3, 1, 1, 3, 2 };
	glm::uint8* indices;

	if ((oppositeFlat && flat && faceType != FACETYPE_LID)) {
		for (glm::int32 i = 0; i < 6; i++) {
			if (reverse)
				indices = revIndices;
			else
				indices = defIndices;

			glm::uint8 flippedIndex = indices[5 - i];
			m_VertexBuffer.SetVertex(pos[flippedIndex] + offset);
			m_VertexBuffer.SetTexCoords(texCoords[flippedIndex]);
			index++;
		}
	}

	for (glm::int32 i = 0; i < 6; i++) {
		if (reverse)
			indices = revIndices;
		else
			indices = defIndices;

		m_VertexBuffer.SetVertex(pos[indices[i]] + offset);
		m_VertexBuffer.SetTexCoords(texCoords[indices[i]]);

		index++;
	}
}

void CMap::EditFace(AVertexBuffer* chunk, tFaceInfo* details) {
	if (!chunk || !details)
		return;

	float cxy = (1.0f / 32.0f);
	float x = (details->flipBook->GetFrame() % 32) * cxy;
	float y = (details->flipBook->GetFrame() / 32) * cxy;
	float w = x + cxy;
	float h = y + cxy;

	glm::uint8 defIndices[] = { 0, 2, 1, 0, 3, 2 };

	std::vector<glm::vec2> texCoords;
	texCoords.push_back(glm::vec2(x, y));
	texCoords.push_back(glm::vec2(w, y));
	texCoords.push_back(glm::vec2(w, h));
	texCoords.push_back(glm::vec2(x, h));

	for (glm::uint32 i = 0; i < 4; i++) {
		texCoords[i] = RotateUV(texCoords[i], glm::radians(details->rotation * 90.0f), glm::vec2(x + (cxy * 0.5f), y + (cxy * 0.5f)));
	}

	for (glm::int32 i = 0; i < 6; i++)
		chunk->EditTexCoords(details->index + i, texCoords[defIndices[i]]);
}

#include "Map.h"
#include "Camera.h"
#include "ABaseGL.h"
#include "Config.h"

CMap::CMap(std::string const& fileName, std::string const& styFileName) {
	m_bInitialised = false;
	m_ChunkBuffer = {};
	m_pStyle = NULL;
	m_vGeometryChunks = {};
	m_vAnimatedFaces = {};
	m_vLights = {};
	m_vZones = {};
	Read(fileName, styFileName);
}

CMap::~CMap() {
	Clear();
}

void CMap::Read(std::string const& fileName, std::string const& styFileName) {
	if (m_bInitialised)
		return;

	if (!Init(fileName, GMP_FILE_VERSION)) {
		return;
	}

	m_pStyle = std::make_unique<CStyle>(styFileName);

	m_vGeometryChunks.reserve(MAP_NUM_BLOCKS_X * MAP_NUM_BLOCKS_Y);
	m_vAnimatedFaces.resize(MAP_NUM_BLOCKS_X * MAP_NUM_BLOCKS_Y, std::vector<CFaceDetails>(0));
	
	std::unique_ptr<std::vector<std::vector<std::vector<CBlockInfoDetailed>>>> detailedBlocks = std::make_unique<std::vector<std::vector<std::vector<CBlockInfoDetailed>>>>(MAP_SCALE_Z, std::vector<std::vector<CBlockInfoDetailed>>(MAP_SCALE_Y, std::vector<CBlockInfoDetailed>(MAP_SCALE_X)));
	std::unique_ptr<std::vector<CMapTileAnimation>> animations = std::make_unique<std::vector<CMapTileAnimation>>();

	while (LoopThroughChunks()) {
		switch (GetChunkType()) {
		case DMAP:
			Read32BitMap(detailedBlocks);
			break;
		case ZONE:
			ReadZones();
			break;
		case MOBJ:
			ReadObjects();
			break;
		case ANIM:
			ReadAnimations(animations);
			break;
		case LGHT:
			ReadLights();
			break;
		default:
			GetFile().Seek(GetChunkSize());
			std::cout << "Skip" << std::endl;
			break;
		}
	}

	BuildDetailedMap(detailedBlocks, animations);
	m_bInitialised = true;
}

void CMap::Clear() {

}

void CMap::Read32BitMap(std::unique_ptr<std::vector<std::vector<std::vector<CBlockInfoDetailed>>>>& detailedBlock) {
	std::vector<std::vector<glm::uint32>> baseOffset(MAP_SCALE_X, std::vector<glm::uint32>(MAP_SCALE_Y));
	for (glm::uint32 i = 0; i < MAP_SCALE_X; i++) {
		for (glm::uint32 j = 0; j < MAP_SCALE_Y; j++) {
			baseOffset[i][j] = GetFile().ReadUInt32();
		}
	}

	glm::uint32 columnCount = GetFile().ReadUInt32();
	std::vector<glm::uint32> columns(columnCount);

	for (glm::uint32 i = 0; i < columnCount; i++)
		columns[i] = GetFile().ReadUInt32();

	glm::uint32 blockCount = GetFile().ReadUInt32();
	std::vector<CBlockInfo> blocks(blockCount);

	for (glm::uint32 i = 0; i < blockCount; i++) {
		blocks[i].face[FACETYPE_LEFT] = GetFile().ReadUInt16();
		blocks[i].face[FACETYPE_RIGHT] = GetFile().ReadUInt16();
		blocks[i].face[FACETYPE_TOP] = GetFile().ReadUInt16();
		blocks[i].face[FACETYPE_BOTTOM] = GetFile().ReadUInt16();
		blocks[i].face[FACETYPE_LID] = GetFile().ReadUInt16();
		blocks[i].arrows = GetFile().ReadUInt8();
		blocks[i].slopeType = GetFile().ReadUInt8();
	}

	for (glm::uint32 i = 0; i < 256; i++) {
		for (glm::uint32 j = 0; j < 256; j++) {
			glm::int32 columnIndex = baseOffset[j][i];
			glm::int32 height = columns[columnIndex] & 0xFF;
			glm::int32 offset = (columns[columnIndex] & 0xFF00) >> 8;

			for (glm::int32 k = 0; k < height; k++) {
				if (offset <= k) {
					detailedBlock->data()[k][j][i].info = blocks[columns[columnIndex + k - offset + 1]];
				}
			}
		}
	}
}

void CMap::ReadZones() {
	glm::uint32 pos = 0;
	while (pos < GetChunkSize()) {
		CMapZone zone = {};
		zone.zoneType = static_cast<eZoneType>(GetFile().ReadInt8());
		zone.x = static_cast<float>(GetFile().ReadInt8());
		zone.y = static_cast<float>(GetFile().ReadInt8());
		zone.w = static_cast<float>(GetFile().ReadInt8());
		zone.h = static_cast<float>(GetFile().ReadInt8());
		glm::int8 nameLength = GetFile().ReadUInt8();

		for (int i = 0; i < nameLength; i++)
			zone.name += GetFile().ReadUInt8();

		m_vZones.push_back(zone);
		pos += 6 + nameLength;
	}
}

void CMap::ReadObjects() {
	glm::int64 startOffset = GetFile().GetPosition();
	while (GetFile().GetPosition() < startOffset + GetChunkSize()) {
		CMapObject object = {};

		object.x = GetFile().ReadInt16();
		object.y = GetFile().ReadInt16();
		object.rot = GetFile().ReadInt8();
		object.objectType = GetFile().ReadUInt8();
	}
}

void CMap::ReadAnimations(std::unique_ptr<std::vector<CMapTileAnimation>>& animations) {
	glm::int64 startOffset = GetFile().GetPosition();
	while (GetFile().GetPosition() < startOffset + GetChunkSize()) {
		CMapTileAnimation anim = {};
		anim.base = GetFile().ReadUInt16();
		anim.frameRate = GetFile().ReadUInt8();
		anim.repeat = GetFile().ReadUInt8();

		glm::uint8 animLength = GetFile().ReadUInt8();
		GetFile().ReadUInt8();

		for (glm::int32 i = 0; i < animLength; i++)
			anim.tiles.push_back(GetFile().ReadUInt16());

		animations->push_back(anim);
	}
}

void CMap::ReadLights() {
	glm::int64 startOffset = GetFile().GetPosition();
	while (GetFile().GetPosition() < startOffset + GetChunkSize()) {
		CMapLight light = {};

		light.color.a = GetFile().ReadUInt8() / 255.0f;
		light.color.r = GetFile().ReadUInt8() / 255.0f;
		light.color.g = GetFile().ReadUInt8() / 255.0f;
		light.color.b = GetFile().ReadUInt8() / 255.0f;

		light.pos.x = static_cast<float>(GetFile().ReadUInt16());
		light.pos.y = static_cast<float>(GetFile().ReadUInt16());
		light.pos.z = static_cast<float>(GetFile().ReadUInt16());

		light.radius = static_cast<float>(GetFile().ReadUInt16());

		light.intensity = static_cast<float>(GetFile().ReadUInt8());
		light.shape = static_cast<float>(GetFile().ReadUInt8());
		light.timeOn = static_cast<float>(GetFile().ReadUInt8());
		light.timeOff = static_cast<float>(GetFile().ReadUInt8());

		m_vLights.push_back(light);
	}
}

void CMap::BuildDetailedMap(std::unique_ptr<std::vector<std::vector<std::vector<CBlockInfoDetailed>>>>& b, std::unique_ptr<std::vector<CMapTileAnimation>>& animations) {
	glm::uint32 index = 0;
	std::vector<CCachedAnims> cachedAnims;

	for (glm::int32 i = 0; i < MAP_SCALE_Y / MAP_NUM_BLOCKS_Y; i++) {
		for (glm::int32 j = 0; j < MAP_SCALE_X / MAP_NUM_BLOCKS_X; j++) {
			for (glm::int32 z = 0; z < MAP_NUM_BLOCKS_Z; z++) {
				for (glm::int32 y = 0; y < MAP_NUM_BLOCKS_Y; y++) {
					for (glm::int32 x = 0; x < MAP_NUM_BLOCKS_X; x++) {
						CBlockInfoDetailed& block = b->data()[z][(y + i * MAP_NUM_BLOCKS_Y)][x + j * MAP_NUM_BLOCKS_X];

						for (glm::uint32 faceType = 0; faceType < NUM_FACETYPES; faceType++) {
							glm::uint16 face = block.info.face[faceType];
							CFaceDetails& detail = block.details[faceType];

							glm::uint16 tile = 0;
							for (glm::uint32 k = 0; k < 10; k++) {
								tile += (face & static_cast<glm::int32>(glm::pow(2, k)));
							}
							detail.tile = tile;

							if (faceType == FACETYPE_LID) {
								glm::uint32 lit1 = 0;
								glm::uint32 lit2 = 0;
								lit1 = CheckBit(face, 10);
								lit2 = CheckBit(face, 11);

								if (!lit1 && !lit2)
									detail.lighting = 0;

								if (lit1 && !lit2)
									detail.lighting = 1;

								if (lit1 && lit2)
									detail.lighting = 2;

								if (lit1 && lit2)
									detail.lighting = 3;

								detail.flat = false;
							}
							else {
								detail.lighting = 0;

								detail.wall = CheckBit(face, 10);
								detail.bulletWall = CheckBit(face, 11);

								glm::uint16 invertedFace = face;
								switch (faceType) {
								case FACETYPE_LEFT:
									invertedFace = block.info.face[FACETYPE_RIGHT];
									break;
								case FACETYPE_TOP:
									invertedFace = block.info.face[FACETYPE_BOTTOM];
									break;
								case FACETYPE_RIGHT:
									invertedFace = block.info.face[FACETYPE_LEFT];
									break;
								case FACETYPE_BOTTOM:
									invertedFace = block.info.face[FACETYPE_TOP];
									break;
								}

								detail.flat = CheckBit(invertedFace, 12);
							}
							detail.flip = CheckBit(face, 13);

							glm::uint32 rot1 = CheckBit(face, 14);
							glm::uint32 rot2 = CheckBit(face, 15);

							if (rot1 && !rot2)
								detail.rotation = 1;
							else if (!rot1 && rot2)
								detail.rotation = 2;
							else if (rot1 && rot2)
								detail.rotation = 3;
							else
								detail.rotation = 0;

							glm::uint32 groundType = 0;
							groundType += (block.info.slopeType & 1);
							groundType += (block.info.slopeType & 2);
							block.groundType = groundType;

							glm::uint32 slopeType = 0;

							for (glm::uint32 k = 2; k < 8; k++) {
								if (CheckBit(block.info.slopeType, k))
									slopeType += glm::pow(2, k - 2);
							}

							switch (slopeType) {
							case SLOPETYPE_SLOPEABOVE:
								slopeType = SLOPETYPE_NONE;
								break;
							case SLOPETYPE_DIAGONALSLOPEFACINGUPLEFT:
							case SLOPETYPE_DIAGONALSLOPEFACINGUPRIGHT:
							case SLOPETYPE_DIAGONALSLOPEFACINGDOWNLEFT:
							case SLOPETYPE_DIAGONALSLOPEFACINGDOWNRIGHT:
								if ((block.info.face[FACETYPE_LID] & 0x3FF) == 1023)
									slopeType = (slopeType - SLOPETYPE_DIAGONALSLOPEFACINGUPLEFT) + SLOPETYPE_DIAGONALSLOPEFACINGUPLEFTNOZERO;
								break;
							}

							block.slopeType = slopeType;

							bool found = false;
							for (glm::int32 animCount = 0; animCount < animations->size(); animCount++) {
								CMapTileAnimation& anim = animations->at(animCount);

								if (anim.base && anim.base < 992 && anim.base == tile) {
									for (glm::uint32 i = 0; i < cachedAnims.size(); i++) {
										if (cachedAnims.at(i).tile != -1 && cachedAnims.at(i).tile == tile && cachedAnims.at(i).flipBook) {
											found = true;
											detail.flipBook = cachedAnims.at(i).flipBook;
											break;
										}
									}
									if (found)
										break;

									if (!detail.flipBook) {
										detail.flipBook = std::make_shared<CFlipbook>();

										detail.flipBook->frames = anim.tiles;
										detail.flipBook->loop = !anim.repeat;
										detail.flipBook->timeToPassForNextFrame = anim.frameRate / ANIMATIONS_FRAME_RATE;
										detail.flipBook->frames.push_back(tile);

										CCachedAnims t;
										t.tile = tile;
										t.flipBook = detail.flipBook;
										cachedAnims.push_back(t);
									}
									break;
								}
							}
						}

						AddBlock(block, glm::vec3(static_cast<float>(x * 1.0f), static_cast<float>(y * 1.0f), static_cast<float>(z * 1.0f - 1.0f)), index);
						for (glm::int32 faceType = 0; faceType < NUM_FACETYPES; faceType++) {
							if (block.details[faceType].flipBook)
								m_vAnimatedFaces[i * MAP_NUM_BLOCKS_X + j].push_back(block.details[faceType]);
						}
					}
				}
			}
			m_vGeometryChunks.push_back(m_ChunkBuffer);
			m_ChunkBuffer.Clear();
			index = 0;
		}
	}
}

void CMap::Render() {
	if (!m_bInitialised)
		return;

	const glm::int32 visibleChunks = 2;
	glm::int32 startx = (static_cast<glm::int32>(Camera.GetPosition().x + (MAP_NUM_BLOCKS_X * 0.5f)) / MAP_NUM_BLOCKS_X) - (visibleChunks);
	glm::int32 endx = (static_cast<glm::int32>(Camera.GetPosition().x + (MAP_NUM_BLOCKS_X * 0.5f)) / MAP_NUM_BLOCKS_X) + (visibleChunks);
	glm::int32 starty = (static_cast<glm::int32>(Camera.GetPosition().y + (MAP_NUM_BLOCKS_Y * 0.5f)) / MAP_NUM_BLOCKS_Y) - (visibleChunks);
	glm::int32 endy = (static_cast<glm::int32>(Camera.GetPosition().y + (MAP_NUM_BLOCKS_Y * 0.5f)) / MAP_NUM_BLOCKS_Y) + (visibleChunks);

	startx = clamp(startx, 0, MAP_NUM_BLOCKS_X);
	endx = clamp(endx, 0, MAP_NUM_BLOCKS_X);
	starty = clamp(starty, 0, MAP_NUM_BLOCKS_Y);
	endy = clamp(endy, 0, MAP_NUM_BLOCKS_Y);

	for (glm::int32 i = starty; i < endy; i++) {
		for (glm::int32 j = startx; j < endx; j++) {
			CGeometry* chunk = &m_vGeometryChunks.at(i * MAP_NUM_BLOCKS_X + j);
			std::vector<CFaceDetails>& animatedFaces = m_vAnimatedFaces.at(i * MAP_NUM_BLOCKS_X + j);
			for (auto& it : animatedFaces) {
				EditFace(chunk, &it);
			}

			glm::vec3 pos = { static_cast<float>(j * MAP_NUM_BLOCKS_X), static_cast<float>(i * MAP_NUM_BLOCKS_Y), 0.0f };
			chunk->SetLocation(pos);
			chunk->SetRotation(0.0f, 0.0f, 1.0f, 0.0f);
			chunk->SetScale(1.0f, 1.0f, 1.0f);
			chunk->Render();
			chunk = NULL;
		}
	}
}

void CMap::AddBlock(CBlockInfoDetailed& block, glm::vec3 offset, glm::uint32& index) {
	for (glm::int32 faceType = 0; faceType < NUM_FACETYPES; faceType++) {
		if (block.details[faceType].tile && block.details[faceType].tile < 992) {
			switch (block.slopeType) {
			case SLOPETYPE_DIAGONALSLOPEFACINGUPLEFTNOZERO:
			case SLOPETYPE_DIAGONALSLOPEFACINGDOWNLEFTNOZERO:
				AddFace(block.slopeType, FACETYPE_LID, block.details[FACETYPE_LEFT].tile, block.details[FACETYPE_LID].rotation, block.details[FACETYPE_LID].flip, false, offset, index);
				break;
			case SLOPETYPE_DIAGONALSLOPEFACINGUPRIGHTNOZERO:
			case SLOPETYPE_DIAGONALSLOPEFACINGDOWNRIGHTNOZERO:
				AddFace(block.slopeType, FACETYPE_LID, block.details[FACETYPE_RIGHT].tile, block.details[FACETYPE_LID].rotation, block.details[FACETYPE_LID].flip, false, offset, index);
				break;
			default:
				AddFace(block.slopeType, faceType, block.details[faceType].tile, block.details[faceType].rotation, block.details[faceType].flip, faceType == FACETYPE_LID ? false : block.details[faceType].flat, offset, index);
				break;
			}
			block.details[faceType].index = index - 6;
		}
	}
}

bool CMap::CheckBit(glm::int32 const& value, glm::int32 const& bitOffset) {
	if (bitOffset < 0 || bitOffset >= sizeof(glm::int32) * 8)
		throw "Argument out of range";
	return (value & (1 << bitOffset)) == (1 << bitOffset);
}

glm::vec2 CMap::RotateUV(glm::vec2 uv, float rotation, glm::vec2 center) {
	glm::vec2 v = { 
		glm::cos(rotation) * (uv.x - center.x) + glm::sin(rotation) * (uv.y - center.y) + center.x,
		glm::cos(rotation) * (uv.y - center.y) - glm::sin(rotation) * (uv.x - center.x) + center.y
	};

	return v;
}

void CMap::AddFace(glm::uint32 slopeType, glm::uint8 faceType, glm::uint32 tile, glm::uint32 rot, bool flip, bool flat, glm::vec3 offset, glm::uint32& index) {
	m_ChunkBuffer.SetTexture(GetStyle()->GetTextureAtlas()->GetID());

	if (flat && slopeType == SLOPETYPE_NONE) {
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
		return;
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
		return;
	}

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
	float w = x + cxy;
	float h = y + cxy;

	if (flip) {
		 texCoords.push_back(glm::vec2(w, y)); texCoords.push_back(glm::vec2(x, y));
		 texCoords.push_back(glm::vec2(x, h)); texCoords.push_back(glm::vec2(w, h));
	}
	else {
		texCoords.push_back(glm::vec2(x, y));
		texCoords.push_back(glm::vec2(w, y));
		texCoords.push_back(glm::vec2(w, h));
		texCoords.push_back(glm::vec2(x, h));
	}

	for (glm::uint32 i = 0; i < 4; i++) {
		texCoords[i] = RotateUV(texCoords[i], DegToRad(rot * 90.0f), glm::vec2(x + (cxy * 0.5f), y + (cxy * 0.5f)));
	}
	
	for (glm::int32 i = 0; i < 6; i++) {
		glm::uint8 defIndices[] = { 0, 2, 1, 0, 3, 2 };
		glm::uint8 revIndices[] = { 0, 3, 1, 1, 3, 2 };
		glm::uint8* indices;

		if (reverse)
			indices = revIndices;
		else
			indices = defIndices;

		m_ChunkBuffer.SetVertex(pos[indices[i]] + offset);
		m_ChunkBuffer.SetTexCoords(texCoords[indices[i]].x, texCoords[indices[i]].y);
		index++;
	}
}

void CMap::EditFace(CGeometry* chunk, CFaceDetails* details) {
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
		texCoords[i] = RotateUV(texCoords[i], DegToRad(details->rotation * 90.0f), glm::vec2(x + (cxy * 0.5f), y + (cxy * 0.5f)));
	}

	for (glm::int32 i = 0; i < 6; i++)
		chunk->EditTexCoords(details->index + i, texCoords[defIndices[i]]);
}

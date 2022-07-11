#include "Style.h"
#include "AEngine.h"
#include "LoadingScreen.h"
#include "AConsole.h"

CStyle::CStyle() {
	Clear();
}

CStyle::CStyle(std::string const& fileName) {	
	Clear();
	Load(fileName);
}

CStyle::~CStyle() {

}

void CStyle::Clear() {
	m_bFileParsed = false;
	m_bBuildComplete = false;
	m_pGraphics = NULL;
	m_pTextureAtlas = NULL;
	m_pSprites = {};
}

bool CStyle::Load(std::string const& fileName) {
	if (m_bFileParsed)
		return true;

	AFileMgr file;
	if (!file.Open(fileName)) {
		return false;
	}

	char header[4] = {};
	file.ReadCustom(header, 4);

	glm::uint16 version = file.ReadUInt16();

	if (strncmp(header, "GBST", 4) || version != STY_VERSION) {
		return false;
	}

	m_pGraphics = std::make_shared<tStyGraphics>();

	while (file.GetPosition() < file.GetSize()) {
		char chunk[4] = {};
		chunk[0] = file.ReadUInt8();
		chunk[1] = file.ReadUInt8();
		chunk[2] = file.ReadUInt8();
		chunk[3] = file.ReadUInt8();

		glm::uint64 chunkSize = file.ReadUInt32();
		
		if (chunkSize != 0) {
			if (!strncmp(chunk, "PALX", 4))
				LoadPALX(chunkSize, file);
			else if (!strncmp(chunk, "PPAL", 4))
				LoadPPAL(chunkSize, file);
			else if (!strncmp(chunk, "PALB", 4))
				LoadPALB(chunkSize, file);
			else if (!strncmp(chunk, "TILE", 4))
				LoadTILE(chunkSize, file);
			else if (!strncmp(chunk, "SPRB", 4))
				LoadSPRB(chunkSize, file);
			else if (!strncmp(chunk, "SPRX", 4))
				LoadSPRX(chunkSize, file);
			else if (!strncmp(chunk, "SPRG", 4))
				LoadSPRG(chunkSize, file);
			else if (!strncmp(chunk, "DELS", 4))
				LoadDELS(chunkSize, file);
			else if (!strncmp(chunk, "DELX", 4))
				LoadDELX(chunkSize, file);
			else if (!strncmp(chunk, "FONB", 4))
				LoadFONB(chunkSize, file);
			else if (!strncmp(chunk, "CARI", 4))
				LoadCARI(chunkSize, file);
			else if (!strncmp(chunk, "OBJI", 4))
				LoadOBJI(chunkSize, file);
			else if (!strncmp(chunk, "PSXT", 4))
				LoadPSXT(chunkSize, file);
			else if (!strncmp(chunk, "RECY", 4))
				LoadRECY(chunkSize, file);
			else if (!strncmp(chunk, "SPEC", 4))
				LoadSPEC(chunkSize, file);
			else
				file.Seek(chunkSize);
		}
	}

	file.Close();

	m_bFileParsed = true;
	BuildEverything();

	return true;
}

void CStyle::LoadPALX(glm::uint64 length, AFileMgr& file) {
	tPaletteIndex palx;

	palx.physPalette.resize(16384);
	file.ReadCustom(palx.physPalette.data(), static_cast<glm::uint64>(16384) * sizeof(glm::uint16));

	m_pGraphics->paletteIndex = palx;
}

void CStyle::LoadPPAL(glm::uint64 length, AFileMgr& file) {
	const glm::uint32 size = 64 * 256;
	std::unique_ptr<std::unique_ptr<glm::uint8[]>[]> page = std::make_unique<std::unique_ptr<glm::uint8[]>[]>(size);

	glm::uint64 pageCount = length / (size * 4);
	std::vector<tPhysicalPalette> palettes;
	for (glm::int32 p = 0; p < pageCount; p++) {
		for (glm::int32 i = 0; i < size; i++) {
			page[i] = std::make_unique<glm::uint8[]>(4);
			page[i][0] = file.ReadUInt8();
			page[i][1] = file.ReadUInt8();
			page[i][2] = file.ReadUInt8();
			page[i][3] = file.ReadUInt8();
		}

		tPhysicalPalette palette;
		for (glm::int32 i = 0; i < 64; i++) {
			for (glm::int32 j = 0; j < 256; j++) {
				for (glm::int32 k = 0; k < 4; k++) {
					palette.colors[j][k] = page[i + j * 64][k];
				}
			}
			palettes.push_back(palette);
		}
	}

	m_pGraphics->physicalPalette = palettes;
}

void CStyle::LoadPALB(glm::uint64 length, AFileMgr& file) {
	tPaletteBase palb;
	file.ReadCustom(&palb, sizeof(palb));

	m_pGraphics->paletteBase = palb;
}

void CStyle::LoadTILE(glm::uint64 length, AFileMgr& file) {
	std::vector<tTileData> tiles;
	std::unique_ptr<tTileData[]> tile = std::make_unique<tTileData[]>(4);
	glm::uint64 halfPageCount = length / (sizeof(tTileData) * 4);
	for (glm::uint32 i = 0; i < halfPageCount; i++) {
		for (glm::uint32 row = 0; row < 64; row++) {
			file.ReadCustom(&tile[0].pixels[row], sizeof(tile[0].pixels[row]));
			file.ReadCustom(&tile[1].pixels[row], sizeof(tile[1].pixels[row]));
			file.ReadCustom(&tile[2].pixels[row], sizeof(tile[2].pixels[row]));
			file.ReadCustom(&tile[3].pixels[row], sizeof(tile[3].pixels[row]));
		}
		tiles.push_back(tile[0]);
		tiles.push_back(tile[1]);
		tiles.push_back(tile[2]);
		tiles.push_back(tile[3]);
	}

	m_pGraphics->tileData = tiles;
}

void CStyle::LoadSPRG(glm::uint64 length, AFileMgr& file) {
	std::vector<glm::uint8> sprg(static_cast<glm::uint32>(length));
	file.ReadCustom(sprg.data(), sizeof(glm::uint8) * sprg.size());

	m_pGraphics->spriteGraphics = sprg;

	std::vector<tSpriteData> sprites;
	for (auto&& spriteData : m_pGraphics->spriteIndex) {
		tSpriteData sprite;
		int width = spriteData.w + spriteData.w % 4;
		for (int i = 0; i < spriteData.h; i++) {
			for (int j = 0; j < width; j++) {
				sprite.pixels.push_back(sprg[spriteData.ptr + j + i * 256]);
			}
		}
		sprites.push_back(sprite);
	}

	m_pGraphics->spriteData = sprites;
}

void CStyle::LoadSPRX(glm::uint64 length, AFileMgr& file) {
	std::vector<tSpriteEntry> sprx(static_cast<glm::uint32>(length));

	file.ReadCustom(sprx.data(), length);
	m_pGraphics->spriteIndex = sprx;
}

void CStyle::LoadSPRB(glm::uint64 length, AFileMgr& file) {
	tSpriteBase sprb;
	file.ReadCustom(&sprb, sizeof(sprb));
	m_pGraphics->spriteBase = sprb;
}

void CStyle::LoadDELS(glm::uint64 length, AFileMgr& file) {
	file.Seek(length);
}

void CStyle::LoadDELX(glm::uint64 length, AFileMgr& file) {
	file.Seek(length);
}

void CStyle::LoadFONB(glm::uint64 length, AFileMgr& file) {
	tFontBase fonb;

	fonb.fontCount = file.ReadUInt16();

	glm::uint16 lastBase = 0;
	for (glm::uint32 i = 0; i < fonb.fontCount; i++) {
		uint16_t base;
		base = file.ReadUInt16();
		fonb.base.push_back(lastBase);
		lastBase += base;
	}

	m_pGraphics->fontBase = fonb;
}

void CStyle::LoadCARI(glm::uint64 length, AFileMgr& file) {
	glm::int64 startOffset = file.GetPosition();
	std::vector<tCarInfo> cari_vec;
	while (file.GetPosition() < startOffset + length) {
		tCarInfo cari;

		cari.model = file.ReadUInt8();
		cari.sprite = file.ReadUInt8();
		cari.w = file.ReadUInt8();
		cari.h = file.ReadUInt8();
		cari.numRemaps = file.ReadUInt8();
		cari.passengers = file.ReadUInt8();
		cari.wreck = file.ReadUInt8();
		cari.rating = file.ReadUInt8();
		cari.frontWheelOffset = file.ReadInt8();
		cari.rearWheelOffset = file.ReadInt8();
		cari.frontWindowOffset = file.ReadInt8();
		cari.rearWindowOffset = file.ReadInt8();
		cari.infoFlags = file.ReadUInt8();
		cari.infoFlags2 = file.ReadUInt8();

		for (glm::int32 i = 0; i < cari.numRemaps; i++) {
			glm::uint8 remap;
			remap = file.ReadUInt8();
			cari.remap.push_back(remap);
		}

		cari.numDoors = file.ReadUInt8();

		for (glm::int32 i = 0; i < cari.numDoors; i++) {
			tDoorInfo door;
			door.rx = file.ReadInt8();
			door.ry = file.ReadInt8();
			cari.doors.push_back(door);
		}
		cari_vec.push_back(cari);
	}

	m_pGraphics->carInfo = cari_vec;
}

void CStyle::LoadOBJI(glm::uint64 length, AFileMgr& file) {
	file.Seek(length);
}

void CStyle::LoadPSXT(glm::uint64 length, AFileMgr& file) {
	file.Seek(length);
}

void CStyle::LoadRECY(glm::uint64 length, AFileMgr& file) {
	file.Seek(length);
}

void CStyle::LoadSPEC(glm::uint64 length, AFileMgr& file) {
	file.Seek(length);
}

std::vector<glm::uint8> CStyle::GetSingleSpriteData(glm::int32 sprite) {
	std::vector<glm::uint8> spriteData;
	tSpriteEntry spriteInfo = m_pGraphics->spriteIndex[sprite];

	for (glm::int32 i = 0; i < spriteInfo.h; i++) {
		for (glm::int32 j = 0; j < spriteInfo.w; j++) {
			spriteData.push_back(m_pGraphics->spriteGraphics[spriteInfo.ptr + i * 256 + j]);
		}
	}

	return spriteData;
}

tPhysicalPalette CStyle::GetSpritePalette(glm::int32 sprite, glm::int32 type, glm::int32 remap) {
	glm::int32 virtualPalette = sprite + m_pGraphics->paletteBase.tile;

	if (remap > -1) {
		if (type == 0) {
			if (m_pGraphics->carInfo[sprite].numRemaps > remap) {
				virtualPalette += m_pGraphics->paletteBase.carRemap + m_pGraphics->carInfo[sprite].remap[remap];
			}
			else {
				virtualPalette = m_pGraphics->paletteBase.sprite + sprite;
			}
		}
		else if (type == 1) {
			virtualPalette = m_pGraphics->paletteBase.pedRemap + remap;
		}
		else {
			virtualPalette = sprite + m_pGraphics->paletteBase.sprite + remap;
		}
	}

	return m_pGraphics->physicalPalette[m_pGraphics->paletteIndex.physPalette[virtualPalette]];
}

void CStyle::BuildEverything() {
	if (!m_bFileParsed)
		return;

	Console.WriteLine("Building textures...");
	BuildTextures();

	Console.WriteLine("Building sprites...");
	BuildSprites();

	m_bBuildComplete = true;
}

void CStyle::BuildTextures() {
	if (!m_pGraphics)
		return;

	if (m_pGraphics->tileData.size() > 0)
		BuildTextureAtlas();
}

void CStyle::BuildTextureAtlas() {
	m_pTextureAtlas = std::make_shared<ATexture2D>();
	m_pTextureAtlas->Build(0, 2048, 2048);

	glm::int32 x = 0;
	glm::int32 y = 0;
	for (glm::uint32 i = 0; i < 992; i++) {
		std::vector<glm::uint32> pixels;
		glm::uint8 w, h;
		WriteTiles(i, w, h, pixels);
		m_pTextureAtlas->Update(pixels.data(), x, y, w, h);
		x += w;

		if (x >= 2048) {
			x = 0;
			y += h;
		}
	}
}

void CStyle::BuildSprites() {
	if (!m_pGraphics)
		return;

	glm::uint32 count = GetBaseIndex(BASEINDEX_LAST);

	for (glm::uint32 i = 0; i < count; i++) {
		std::vector<glm::uint32> pixels;
		glm::uint8 w, h;
		WriteSprites(i, w, h, pixels);

		std::shared_ptr<ATexture2D> sprite = std::make_shared<ATexture2D>();
		sprite->Build(pixels.data(), w, h);
		m_pSprites.push_back(sprite);
	}
}

void CStyle::WriteTiles(glm::uint32 i, glm::uint8& w, glm::uint8& h, std::vector<glm::uint32>& pixels) {
	glm::uint32 vpalette = m_pGraphics->paletteIndex.physPalette[i];

	w = 64;
	h = 64;
	pixels.resize(w * h);

	for (glm::uint32 y = 0; y < h; y++) {
		for (glm::uint32 x = 0; x < w; x++) {
			glm::uint32* color = reinterpret_cast<glm::uint32*>(m_pGraphics->physicalPalette[vpalette].colors[m_pGraphics->tileData[i].pixels[y][x]]);
			glm::uint32 alpha = ((m_pGraphics->tileData[i].pixels[y][x] > 0) * 0xff000000);

			pixels[x + w * y] = (*color) + alpha;
		}
	}
}

void CStyle::WriteSprites(glm::uint32 i, glm::uint8& w, glm::uint8& h, std::vector<glm::uint32>& pixels) {
	tPhysicalPalette p = GetSpritePalette(i, -1, -1);
	tSpriteEntry s = m_pGraphics->spriteIndex[i];

	w = s.w;
	h = s.h;
	pixels.resize(w * h);

	std::vector<glm::uint8> spriteData = GetSingleSpriteData(i);

	for (glm::uint32 y = 0; y < h; y++) {
		for (glm::uint32 x = 0; x < w; x++) {
			glm::uint32* color = reinterpret_cast<glm::uint32*>(p.colors[spriteData[y * s.w + x]]);
			glm::uint32 alpha = ((spriteData[y * s.w + x] > 0) * 0xff000000);

			pixels[x + s.w * y] = *color + alpha;
		}
	}
}

glm::uint32 CStyle::GetBaseIndex(eBaseIndices b) {
	glm::uint32 base = 0;
	switch (b) {
	case BASEINDEX_CARS:
		base = 0;
		break;
	case BASEINDEX_PEDS:
		base += m_pGraphics->spriteBase.car;
		break;
	case BASEINDEX_CODEOBJ:
		base += m_pGraphics->spriteBase.car + m_pGraphics->spriteBase.ped;
		break;
	case BASEINDEX_MAPOBJ:
		base += m_pGraphics->spriteBase.car + m_pGraphics->spriteBase.ped + m_pGraphics->spriteBase.codeObj;
		break;
	case BASEINDEX_USER:
		base += m_pGraphics->spriteBase.car + m_pGraphics->spriteBase.ped + m_pGraphics->spriteBase.codeObj + m_pGraphics->spriteBase.mapObj;
		break;
	case BASEINDEX_FONT:
		base += m_pGraphics->spriteBase.car + m_pGraphics->spriteBase.ped + m_pGraphics->spriteBase.codeObj + m_pGraphics->spriteBase.mapObj + m_pGraphics->spriteBase.user;
		break;
	case BASEINDEX_LAST:
		base += m_pGraphics->spriteBase.car + m_pGraphics->spriteBase.ped + m_pGraphics->spriteBase.codeObj + m_pGraphics->spriteBase.mapObj + m_pGraphics->spriteBase.user + m_pGraphics->spriteBase.font;
		break;
	}
	return base;
}

glm::uint32 CStyle::GetFontBaseIndex(glm::uint8 fontStyle) {
	return GetBaseIndex(BASEINDEX_FONT) + m_pGraphics->fontBase.base[fontStyle];
}

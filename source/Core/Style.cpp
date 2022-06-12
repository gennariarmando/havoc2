#include "Style.h"
#include "Frontend.h"

CStyle::CStyle() {
	Clear();
}

CStyle::CStyle(std::string const& fileName) {	
	Clear();
	Read(fileName);
}

CStyle::~CStyle() {

}

void CStyle::Clear() {
	m_bFileParsed = false;
	m_bBuildComplete = false;
	m_pGraphics = NULL;
	m_pTextureAtlas = NULL;
	m_pSprites = {};
	m_pTextures = {};
}

void CStyle::Read(std::string const& fileName) {
	if (m_bFileParsed)
		return;

	if (!Init(fileName, STY_FILE_VERSION)) {
		return;
	}

	m_pGraphics = std::make_shared<CStyGraphics>();

	while (LoopThroughChunks()) {
		switch (GetChunkType()) {
		case PALX:
			ReadPALX();
			break;
		case PPAL:
			ReadPPAL();
			break;
		case PALB:
			ReadPALB();
			break;
		case TILE:
			ReadTILE();
			break;
		case SPRB:
			ReadSPRB();
			break;
		case SPRX:
			ReadSPRX();
			break;
		case SPRG:
			ReadSPRG();
			break;
		case DELS:
			ReadDELS();
			break;
		case DELX:
			ReadDELX();
			break;
		case FONB:
			ReadFONB();
			break;
		case CARI:
			ReadCARI();
			break;
		case OBJI:
			ReadOBJI();
			break;
		case PSXT:
			ReadPSXT();
			break;
		case RECY:
			ReadRECY();
			break;
		case SPEC:
			ReadSPEC();
			break;
		default:
			SkipChunk();
			break;
		}
	}

	m_bFileParsed = true;
}

void CStyle::ReadPALX() {
	CPaletteIndex palx;

	for (glm::uint32 i = 0; i < 16384; i++)
		palx.physPalette.push_back(GetFile().ReadUInt16());

	m_pGraphics->paletteIndex = palx;
}

void CStyle::ReadPPAL() {
	const glm::uint32 size = 64 * 256;
	std::unique_ptr<std::unique_ptr<glm::uint8[]>[]> page = std::make_unique<std::unique_ptr<glm::uint8[]>[]>(size);

	glm::int32 pageCount = GetChunkSize() / (size * 4);
	std::vector<CPhysicalPalette> palettes;
	for (glm::int32 p = 0; p < pageCount; p++) {
		for (glm::int32 i = 0; i < size; i++) {
			page[i] = std::make_unique<glm::uint8[]>(4);
			page[i][0] = GetFile().ReadUInt8();
			page[i][1] = GetFile().ReadUInt8();
			page[i][2] = GetFile().ReadUInt8();
			page[i][3] = GetFile().ReadUInt8();
		}

		CPhysicalPalette palette;
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

void CStyle::ReadPALB() {
	CPaletteBase palb;
	GetFile().ReadCustom(&palb, sizeof(palb));

	m_pGraphics->paletteBase = palb;
}

void CStyle::ReadTILE() {
	std::vector<CTileData> tiles;
	std::unique_ptr<CTileData[]> tile = std::make_unique<CTileData[]>(4);
	glm::int32 halfPageCount = GetChunkSize() / (sizeof(CTileData) * 4);
	for (glm::int32 i = 0; i < halfPageCount; i++) {
		for (glm::int32 row = 0; row < 64; row++) {
			GetFile().ReadCustom(&tile[0].pixels[row], sizeof(tile[0].pixels[row]));
			GetFile().ReadCustom(&tile[1].pixels[row], sizeof(tile[1].pixels[row]));
			GetFile().ReadCustom(&tile[2].pixels[row], sizeof(tile[2].pixels[row]));
			GetFile().ReadCustom(&tile[3].pixels[row], sizeof(tile[3].pixels[row]));
		}
		tiles.push_back(tile[0]);
		tiles.push_back(tile[1]);
		tiles.push_back(tile[2]);
		tiles.push_back(tile[3]);
	}

	m_pGraphics->tileData = tiles;
}

void CStyle::ReadSPRG() {
	std::vector<glm::uint8> sprg(GetChunkSize());
	for (glm::uint32 i = 0; i < GetChunkSize(); i++)
		sprg[i] = GetFile().ReadUInt8();

	m_pGraphics->spriteGraphics = sprg;

	std::vector<CSpriteData> sprites;
	for (auto&& spriteData : m_pGraphics->spriteIndex) {
		CSpriteData sprite;
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

void CStyle::ReadSPRX() {
	std::vector<CSpriteEntry> sprx(GetChunkSize());

	GetFile().ReadCustom(sprx.data(), GetChunkSize());
	m_pGraphics->spriteIndex = sprx;
}

void CStyle::ReadSPRB() {
	CSpriteBase sprb;
	GetFile().ReadCustom(&sprb, sizeof(sprb));
	m_pGraphics->spriteBase = sprb;
}

void CStyle::ReadDELS() {
	SkipChunk();
}

void CStyle::ReadDELX() {
	SkipChunk();
}

void CStyle::ReadFONB() {
	CFontBase fonb;

	fonb.fontCount = GetFile().ReadUInt16();

	glm::uint16 lastBase = 0;
	for (glm::uint32 i = 0; i < fonb.fontCount; i++) {
		uint16_t base;
		base = GetFile().ReadUInt16();
		fonb.base.push_back(lastBase);
		lastBase += base;
	}

	m_pGraphics->fontBase = fonb;
}

void CStyle::ReadCARI() {
	glm::int64 startOffset = GetFile().GetPosition();
	std::vector<CCarInfo> cari_vec;
	while (GetFile().GetPosition() < startOffset + GetChunkSize()) {
		CCarInfo cari;

		cari.model = GetFile().ReadUInt8();
		cari.sprite = GetFile().ReadUInt8();
		cari.w = GetFile().ReadUInt8();
		cari.h = GetFile().ReadUInt8();
		cari.numRemaps = GetFile().ReadUInt8();
		cari.passengers = GetFile().ReadUInt8();
		cari.wreck = GetFile().ReadUInt8();
		cari.rating = GetFile().ReadUInt8();
		cari.frontWheelOffset = GetFile().ReadInt8();
		cari.rearWheelOffset = GetFile().ReadInt8();
		cari.frontWindowOffset = GetFile().ReadInt8();
		cari.rearWindowOffset = GetFile().ReadInt8();
		cari.infoFlags = GetFile().ReadUInt8();
		cari.infoFlags2 = GetFile().ReadUInt8();

		for (glm::int32 i = 0; i < cari.numRemaps; i++) {
			glm::uint8 remap;
			remap = GetFile().ReadUInt8();
			cari.remap.push_back(remap);
		}

		cari.numDoors = GetFile().ReadUInt8();

		for (glm::int32 i = 0; i < cari.numDoors; i++) {
			CDoorInfo door;
			door.rx = GetFile().ReadInt8();
			door.ry = GetFile().ReadInt8();
			cari.doors.push_back(door);
		}
		cari_vec.push_back(cari);
	}

	m_pGraphics->carInfo = cari_vec;
}

void CStyle::ReadOBJI() {
	SkipChunk();
}

void CStyle::ReadPSXT() {
	SkipChunk();
}

void CStyle::ReadRECY() {
	SkipChunk();
}

void CStyle::ReadSPEC() {
	SkipChunk();
}

std::vector<glm::uint8> CStyle::GetSingleSpriteData(glm::int32 sprite) {
	std::vector<glm::uint8> spriteData;
	CSpriteEntry spriteInfo = m_pGraphics->spriteIndex[sprite];

	for (glm::int32 i = 0; i < spriteInfo.h; i++) {
		for (glm::int32 j = 0; j < spriteInfo.w; j++) {
			spriteData.push_back(m_pGraphics->spriteGraphics[spriteInfo.ptr + i * 256 + j]);
		}
	}

	return spriteData;
}

CPhysicalPalette CStyle::GetSpritePalette(glm::int32 sprite, glm::int32 type, glm::int32 remap) {
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
	if (m_bBuildComplete || !m_bFileParsed)
		return;

	BuildTextures();
	BuildSprites();

	m_bBuildComplete = true;
}

void CStyle::BuildTextures() {
	if (!m_pGraphics)
		return;

	if (m_pGraphics->tileData.size() > 0) {
		for (glm::uint32 i = 0; i < 992; i++) {
			glm::uint32 vpalette = m_pGraphics->paletteIndex.physPalette[i];

			std::vector<glm::uint32> pixels;
			glm::uint8 w, h;
			WriteTiles(i, w, h, pixels);

			std::shared_ptr<CTexture2D> texture = std::make_shared<CTexture2D>();
			texture->Build(pixels.data(), w, h, 4, GL_NEAREST);
			m_pTextures.push_back(texture);
		}

		BuildTextureAtlas();
	}
}

void CStyle::BuildTextureAtlas() {
	m_pTextureAtlas = std::make_shared<CTexture2D>();
	m_pTextureAtlas->Build(0, 2048, 2048, 4, GL_NEAREST);

	glm::int32 x = 0;
	glm::int32 y = 0;
	for (glm::uint32 i = 0; i < 992; i++) {
		glm::uint32 vpalette = m_pGraphics->paletteIndex.physPalette[i];

		std::vector<glm::uint32> pixels;
		glm::uint8 w, h;
		WriteTiles(i, w, h, pixels);
		m_pTextureAtlas->Update(pixels.data(), x, y, w, h, 4, GL_NEAREST);
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

		std::shared_ptr<CTexture2D> sprite = std::make_shared<CTexture2D>();
		sprite->Build(pixels.data(), w, h, 4, GL_NEAREST);
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
inline glm::int32 qRgb(glm::int32 r, glm::int32 g, glm::int32 b) {
	return (0xffu << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu);
}

void CStyle::WriteSprites(glm::uint32 i, glm::uint8& w, glm::uint8& h, std::vector<glm::uint32>& pixels) {
	CPhysicalPalette p = GetSpritePalette(i, -1, -1);
	CSpriteEntry s = m_pGraphics->spriteIndex[i];

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

glm::uint32 const& CStyle::GetBaseIndex(eBaseIndices b) {
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

glm::uint32 const& CStyle::GetFontBaseIndex(glm::uint8 fontStyle) {
	return GetBaseIndex(BASEINDEX_FONT) + m_pGraphics->fontBase.base[fontStyle];
}
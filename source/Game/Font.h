#pragma once
#include "Common.h"
#include "Sprite2D.h"
#include "Style.h"

#define FSTYLE_FONT_START 8

enum eFontStyle {
	FONT_BIG_MESSAGES,
	FONT_SUBTITLES,
	FONT_VEHICLE_NAMES,
	FONT_ZONE_NAMES = 5,
	FONT_HEADING,
	// fstyle
	FONT_BANK = FSTYLE_FONT_START + 1,
	FONT_REGISTER_DEMIBOLD = FONT_BANK + 3,
	FONT_MENU = FONT_REGISTER_DEMIBOLD + 1,
};

enum eFontAlignment {
	FONT_ALIGN_LEFT,
	FONT_ALIGN_CENTER,
	FONT_ALIGN_RIGHT,
};

class CFont {
private:
	std::unique_ptr<CSprite2D> m_pSprite;
	eFontStyle m_eFontStyle;
	eFontAlignment m_eFontAlignment;
	float m_fScale;
	float m_fWrapX;
	glm::vec4 m_vColor;

public:
	void Init();
	std::shared_ptr<CStyle> GetStyleForThisFont(eFontStyle);
	eFontStyle GetFontStyleFrontendShift();
	void SetFontStyle(eFontStyle fontStyle);
	glm::vec2 GetCharacterSize(char c);
	float GetStringWidth(const char* s, bool spaces);
	void SetScale(float scale);
	void SetWrapX(float x);
	void SetFontAlignment(eFontAlignment align);
	void SetColor(glm::vec4 col);
	void PrintString(glm::vec2 pos, std::string str);
};

extern CFont Font;
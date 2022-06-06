#pragma once
#include "Common.h"
#include "Sprite2D.h"
#include "Style.h"

enum eFontStyle {
	FONT_BIG_MESSAGES,
	FONT_SMALL_MESSAGES,
	NUM_FONTS,
};

enum eFontAlignment {
	FONT_ALIGN_LEFT,
	FONT_ALIGN_CENTER,
	FONT_ALIGN_RIGHT,
};

class CFont {
private:
	std::shared_ptr<CStyle> m_pStyle;
	std::unique_ptr<CSprite2D> m_pSprite;
	eFontStyle m_eFontStyle;
	eFontAlignment m_eFontAlignment;
	glm::vec2 m_vScale;

public:
	void Init(std::shared_ptr<CStyle>& style);
	void SetFontStyle(eFontStyle fontStyle);
	float GetCharacterSize(char c);
	float GetStringWidth(const char* s, bool spaces);
	void SetScale(glm::vec2 scale);
	void SetFontAlignment(eFontAlignment align);
	void PrintString(glm::vec2 pos, std::string str);
};

extern CFont Font;
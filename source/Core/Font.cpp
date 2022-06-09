#include "Font.h"
#include "Frontend.h"
#include "World.h"
#include "Screen.h"

CFont Font;

CFont::CFont() {
	m_pSprite = std::make_unique<CSprite2D>();
	Reset();
}

CFont::~CFont() {
	m_pSprite.release();
}

std::shared_ptr<CStyle> CFont::GetStyleForThisFont(eFontStyle fontStyle) {
	if (fontStyle < FSTYLE_FONT_START)
		return World.GetStyle();

	return Frontend.GetStyle();
}

eFontStyle CFont::GetFontStyleFrontendShift() {
	if (m_eFontStyle > FSTYLE_FONT_START)
		return static_cast<eFontStyle>(m_eFontStyle - FSTYLE_FONT_START - 1);

	return m_eFontStyle;	
}

glm::vec2 CFont::GetCharacterSize(char c) {
	auto style = GetStyleForThisFont(m_eFontStyle);
	if (!style)
		return { 0.0f, 0.0f };

	glm::uint32 index = style->GetFontBaseIndex(GetFontStyleFrontendShift()) + c - '!';
	glm::vec2 v = { style->GetSprite().at(index)->GetWidth(), style->GetSprite().at(index)->GetHeight()};

	float targetAR = 16.0f / 16.0f;
	float imageAR = v.y / v.x;

	v.x = roundf(16.0f / imageAR * targetAR);
	v.y = 16.0f;

	v.x *= m_fScale;
	v.y *= m_fScale;

	return v;
}

float CFont::GetStringWidth(std::string str, bool spaces) {
	float w = 0.0f;
	const char* s = str.c_str();
	for (; (*s != ' ' || spaces) && *s != '\0'; s++) {
		w += GetCharacterSize(*s).x;
	}
	return w;
}

void CFont::Reset() {
	m_eFontStyle = FONT_BIG_MESSAGES;
	m_eFontAlignment = FONT_ALIGN_LEFT;
	m_fScale = 1.0f;
	m_fWrapX = SCREEN_WIDTH;
	m_vColor = { 1.0f, 1.0f, 1.0f, 1.0f };
}

void CFont::SetFontStyle(eFontStyle fontStyle) {
	m_eFontStyle = fontStyle;
}

void CFont::SetScale(float scale) {
	m_fScale = scale;
}

void CFont::SetWrapX(float x) {
	m_fWrapX = x;
}

void CFont::SetFontAlignment(eFontAlignment align) {
	m_eFontAlignment = align;
}

void CFont::SetColor(glm::vec4 col) {
	m_vColor = col;
}

void CFont::PrintString(glm::vec2 pos, std::string str) {
	auto style = GetStyleForThisFont(m_eFontStyle);
	if (!style)
		return;

	switch (m_eFontAlignment) {
	case FONT_ALIGN_RIGHT:
		pos.x -= GetStringWidth(str.c_str(), true);
		break;
	case FONT_ALIGN_CENTER:
		pos.x -= GetStringWidth(str.c_str(), true) * 0.5f;
		break;
	default:
		break;
	}

	const glm::vec2 initialPos = pos;
	for (auto c : str) {
		switch (m_eFontStyle) {
		case FONT_BIG_MESSAGES:
		case FONT_VEHICLE_NAMES:
		case FONT_ZONE_NAMES:
		case FONT_HEADING:
		case FONT_BANK:
		case FONT_MENU:
			if (c >= 'a' && c <= 'z') {
				c = c - ('a' - 'A');
			}
			break;
		}

		if (pos.x + GetCharacterSize(c).x > m_fWrapX) {
			pos.x = initialPos.x;
			pos.y += GetCharacterSize(c).y;
		}

		if (c != ' ') {
			glm::uint32 index = style->GetFontBaseIndex(GetFontStyleFrontendShift()) + c - '!';
			m_pSprite->SetTexture(style->GetSprite().at(index)->GetID());
			m_pSprite->Draw(pos.x, pos.y, GetCharacterSize(c).x, GetCharacterSize(c).y, m_vColor);
		}

		pos.x += GetCharacterSize(c).x;
	}
}

#include "Font.h"
#include "Frontend.h"
#include "World.h"
#include "Screen.h"

CFont Font;

void CFont::Init() {
	m_pSprite = std::make_unique<CSprite2D>();
	m_eFontStyle = FONT_BIG_MESSAGES;
	m_eFontAlignment = FONT_ALIGN_LEFT;
	m_fScale = 1.0f;
	m_fWrapX = SCREEN_WIDTH;
	m_vColor = { 1.0f, 1.0f, 1.0f, 1.0f };
}

std::shared_ptr<CStyle> CFont::GetStyleForThisFont(eFontStyle fontStyle) {
	if (World.IsLevelLoaded() && fontStyle < FSTYLE_FONT_START)
		return World.GetStyle();

	return Frontend.GetStyle();
}

eFontStyle CFont::GetFontStyleFrontendShift() {
	if (m_eFontStyle > FSTYLE_FONT_START)
		return static_cast<eFontStyle>(m_eFontStyle - FSTYLE_FONT_START - 1);

	return m_eFontStyle;	
}

void CFont::SetFontStyle(eFontStyle fontStyle) {
	m_eFontStyle = fontStyle;
}

glm::vec2 CFont::GetCharacterSize(char c) {
	glm::uint32 index = GetStyleForThisFont(m_eFontStyle)->GetFontBaseIndex(GetFontStyleFrontendShift()) + c - '!';
	glm::vec2 v = { GetStyleForThisFont(m_eFontStyle)->GetSprite().at(index)->GetWidth(), GetStyleForThisFont(m_eFontStyle)->GetSprite().at(index)->GetHeight()};

	float targetAR = 16.0f / 16.0f;
	float imageAR = v.y / v.x;

	v.x = roundf(16.0f / imageAR * targetAR);
	v.y = 16.0f;

	v.x *= m_fScale;
	v.y *= m_fScale;

	return v;
}

float CFont::GetStringWidth(const char* s, bool spaces) {
	float w;

	w = 0.0f;
	for (; (*s != ' ' || spaces) && *s != '\0'; s++) {
		w += GetCharacterSize(*s).x;
	}
	return w;
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
			glm::uint32 index = GetStyleForThisFont(m_eFontStyle)->GetFontBaseIndex(GetFontStyleFrontendShift()) + c - '!';
			m_pSprite->SetTexture(GetStyleForThisFont(m_eFontStyle)->GetSprite().at(index)->GetID());
			m_pSprite->Draw(pos.x, pos.y, GetCharacterSize(c).x, GetCharacterSize(c).y, m_vColor);
		}

		pos.x += GetCharacterSize(c).x;
	}
}


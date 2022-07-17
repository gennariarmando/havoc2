#include "Font.h"
#include "Frontend.h"
#include "World.h"
#include "AScreen.h"

CFont Font;

CFont::CFont() {
	m_pSprite = new ASprite();
	Reset();
}

CFont::~CFont() {
	delete m_pSprite;
}

CStyle* CFont::GetStyleForThisFont(eFontStyle fontStyle) {
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

	v.x *= m_fScale / 8.0f;
	v.y *= m_fScale / 8.0f;

	return v;
}

float CFont::GetStringWidth(std::string str) {
	float w = 0.0f;

	for (auto c : str) {
		switch (m_eFontStyle) {
		case FONT_STYLE_BIGMESSAGE:
		case FONT_STYLE_VEHICLE:
		case FONT_STYLE_ZONE:
		case FONT_STYLE_HEADING:
		case FONT_STYLE_BANK:
		case FONT_STYLE_MENU:
			if (c >= 'a' && c <= 'z') {
				c = c - ('a' - 'A');
			}
			break;
		}

		w += GetCharacterSize(c).x;
	}
	return w;
}

void CFont::Reset() {
	m_eFontStyle = FONT_STYLE_BIGMESSAGE;
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

void CFont::ParseToken(std::string& str) {

}

void CFont::PrintString(glm::vec2 pos, std::string str) {
	auto style = GetStyleForThisFont(m_eFontStyle);
	if (!style)
		return;

	if (pos.x < 0.0f || pos.x > SCREEN_WIDTH || pos.y < 0.0f || pos.y > SCREEN_HEIGHT)
		return;

	switch (m_eFontAlignment) {
	case FONT_ALIGN_RIGHT:
		pos.x -= GetStringWidth(str);
		break;
	case FONT_ALIGN_CENTER:
		pos.x -= GetStringWidth(str) * 0.5f;
		break;
	default:
		break;
	}

	const glm::vec2 initialPos = pos;
	for (auto c : str) {
		switch (m_eFontStyle) {
		case FONT_STYLE_BIGMESSAGE:
		case FONT_STYLE_VEHICLE:
		case FONT_STYLE_ZONE:
		case FONT_STYLE_HEADING:
		case FONT_STYLE_BANK:
		case FONT_STYLE_MENU:
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
			m_pSprite->SetTexture(style->GetSprite().at(index));
			m_pSprite->Draw(pos.x, pos.y, GetCharacterSize(c).x, GetCharacterSize(c).y, m_vColor);
		}

		pos.x += GetCharacterSize(c).x;
	}
}

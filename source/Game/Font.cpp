#include "Font.h"

CFont Font;

void CFont::Init(std::shared_ptr<CStyle>& style) {
	m_pStyle = style;
	m_pSprite = std::make_unique<CSprite2D>();
	m_eFontStyle = FONT_SMALL_MESSAGES;
	m_eFontAlignment = FONT_ALIGN_LEFT;
	m_vScale = { 1.0f, 1.0f };
}

void CFont::SetFontStyle(eFontStyle fontStyle) {
	m_eFontStyle = fontStyle;
}

float CFont::GetCharacterSize(char c) {
	glm::uint32 index = m_pStyle->m_pGraphics->spriteBase.car + m_pStyle->m_pGraphics->spriteBase.ped + m_pStyle->m_pGraphics->spriteBase.codeObj +
		m_pStyle->m_pGraphics->spriteBase.mapObj + m_pStyle->m_pGraphics->spriteBase.user + m_pStyle->m_pGraphics->fontBase.base[m_eFontStyle] + c - '!';

	return m_pStyle->GetSprite().at(index)->GetWidth() * m_vScale.x;
}

float CFont::GetStringWidth(const char* s, bool spaces) {
	float w;

	w = 0.0f;
	for (; (*s != ' ' || spaces) && *s != '\0'; s++) {
		w += GetCharacterSize(*s);
	}
	return w;
}

void CFont::SetScale(glm::vec2 scale) {
	m_vScale = scale;
}

void CFont::SetFontAlignment(eFontAlignment align) {
	m_eFontAlignment = align;
}

void CFont::PrintString(glm::vec2 pos, std::string str) {
	bool first = true;
	for (auto c : str) {
		switch (m_eFontStyle) {
		case FONT_BIG_MESSAGES:
			if (c >= 'a' && c <= 'z') {
				c = c - ('a' - 'A');
			}
			break;
		default:
			break;
		}

		glm::uint32 index = m_pStyle->m_pGraphics->spriteBase.car + m_pStyle->m_pGraphics->spriteBase.ped + m_pStyle->m_pGraphics->spriteBase.codeObj +
			m_pStyle->m_pGraphics->spriteBase.mapObj + m_pStyle->m_pGraphics->spriteBase.user + m_pStyle->m_pGraphics->fontBase.base[m_eFontStyle] + c - '!';

		if (first) {
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
			first = false;
		}

		if (c != ' ') {
			m_pSprite->SetTexture(m_pStyle->GetSprite().at(index)->GetID());
			m_pSprite->Draw(pos.x, pos.y, m_vScale.x * m_pStyle->GetSprite().at(index)->GetWidth(), m_vScale.y * m_pStyle->GetSprite().at(index)->GetHeight(), glm::vec4(1.0f));
		}

		pos.x += GetCharacterSize(c);
	}
}

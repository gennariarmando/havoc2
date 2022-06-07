#pragma once
#include "ABaseObject.h"
#include "Style.h"

class CFrontend : public ABaseObject {
private:
	std::shared_ptr<CStyle> m_pStyle;

public:
	std::shared_ptr<CStyle> const& GetStyle() { return m_pStyle; }

public:
	CFrontend();
	~CFrontend();

	void Init();
	void Update();
	void Draw2D();
	void Shutdown();
};

extern CFrontend Frontend;
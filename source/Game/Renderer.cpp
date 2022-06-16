#include "Renderer.h"
#include "World.h"
#include "AGraphicDevice.h"
#include "Hud.h"
#include "Frontend.h"

CRenderer Renderer;

CRenderer::CRenderer() {

}

void CRenderer::Render() {
	SetRenderState(RENDERSTATE_STENCILTESTENABLE, false);
	SetRenderState(RENDERSTATE_DEPTHTESTENABLE, true);
	SetRenderState(RENDERSTATE_DEPTHMODE, DEPTHTEST_LESS);
	SetRenderState(RENDERSTATE_CULLENABLE, true);
	SetRenderState(RENDERSTATE_CULLMODE, CULLMODE_BACK);

	World.GetMap()->Render(World.GetStyle());

	SetRenderState(RENDERSTATE_CULLENABLE, false);
	SetRenderState(RENDERSTATE_CULLMODE, CULLMODE_NONE);
	for (auto& it : World.GetEntityList()) {
		it->Update();
		it->Render();
	}

	Draw2D();
}

void CRenderer::Draw2D() {
	Hud.Draw();
	Hud.DrawDebugStuff();

	Frontend.Draw();
}

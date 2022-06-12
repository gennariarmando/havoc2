#pragma once
#include "ABaseObject.h"

class CDebug : public ABaseObject {
public:
    CDebug();

    void BeginPlay() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;
    void Draw2D()  override;
    void Draw2DDebug()  override;
    void EndPlay()  override;
};

extern CDebug Debug;

#pragma once
#include "ABaseObject.h"

enum eCameraMode {
    MODE_DEBUG3D,
    MODE_FOLLOWENTITY,
};

enum eCameraProjection {
    PROJECTION_PERSPECTIVE,
    PROJECTION_ORTHOGRAPHIC,
};

enum eFrustumPlanes {
    FPLANE_LEFT,
    FPLANE_RIGHT,
    FPLANE_BOTTOM,
    FPLANE_TOP,
    FPLANE_NEAR,
    FPLANE_FAR,
    Count,
    Combinations = Count * (Count - 1) / 2
};

template<eFrustumPlanes i, eFrustumPlanes j>
struct ij2k {
    enum { k = i * (9 - i) / 2 + j - 1 };
};

class CCamera : public ABaseObject {
private:
    glm::uint8 m_nMode;
    glm::uint8 m_nProjType;
    glm::mat4 m_mProjection;
    glm::mat4 m_mView;
    glm::vec3 m_vPosition;
    glm::vec3 m_vFront;
    glm::vec3 m_vUp;
    glm::vec3 m_vRight;
    glm::vec3 m_vWorldUp;
    glm::vec2 m_vAngle;
    float m_fFOV;
    float m_fFrontDist;
    bool m_bScene3D;
    float m_fFarClip;
    float m_fNearClip;
    glm::vec4 m_vPlanes[Count];
    glm::vec3 m_vPoints[8];

public:
    CCamera();
    ~CCamera();
    void Init();
    void Update();
    void LateUpdate();
    void Shutdown();

    void ComputeProjection();

    template<eFrustumPlanes a, eFrustumPlanes b, eFrustumPlanes c>
    glm::vec3 Intersection(const glm::vec3* crosses) const;

    void Frustum(glm::mat4 m);
    bool IsBoxVisible(const glm::vec3& minp, const glm::vec3& maxp) const;

public:
    void UpdateCameraVectors();
    void ProcessDebug();

public:
    glm::uint8& GetMode() { return m_nMode; }
    glm::uint8& GetProjectionType() { return m_nProjType; }
    glm::mat4& GetProjection() { return m_mProjection; }
    glm::mat4& GetView() { return m_mView; }
    glm::vec3& GetPosition() { return m_vPosition; }
    glm::vec3& GetFront() { return m_vFront; }
    glm::vec3& GetUp() { return m_vUp; }
    glm::vec3& GetRight() { return m_vRight; }
    glm::vec3& GetWorldUp() { return m_vWorldUp; }
    float& GetYaw() { return m_vAngle.x; }
    float& GetPitch() { return m_vAngle.y; }
    float& GetFov() { return m_fFOV; }
    bool& GetScene3D() { return m_bScene3D; }
    float& GetFarClip () { return m_fFarClip; }
    float& GetNearClip() { return m_fNearClip; }

public:
    glm::mat4 const GetProjection(glm::mat4 const& m) { return m_mProjection * m; }

};

extern CCamera Camera;
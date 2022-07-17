#include "ACamera.h"
#include "AScreen.h"
#include "AInput.h"
#include "ATime.h"
#include "AGraphicDevice.h"

ACamera Camera;

ACamera::ACamera() {
    m_nMode = MODE_DEBUG3D;
    m_mProjection = {};
    m_mView = {};
    m_vPosition = glm::vec3(159.50f, 139.50f, 10.0f);
    m_vFront = glm::vec3(0.0f, -1.0f, 0.0f);
    m_vUp = glm::vec3(0.0f, 0.0f, 1.0f);
    m_vRight = glm::vec3(0.0f, 0.0f, 0.0f);
    m_vWorldUp = m_vUp;
    m_vAngle.x = glm::radians(0.0f);
    m_vAngle.y = glm::radians(-90.0f);
    m_fFOV = 45.0f;
    m_fFrontDist = 0.0f;
    m_fNearClip = 0.1f;
    m_fFarClip = 100.0f;

    for (glm::uint32 i = 0; i < FPLANE_COUNT; i++) {
        m_vPlanes[i] = {};
    }

    for (glm::uint32 i = 0; i < FPLANE_COUNT + 2; i++) {
        m_vPoints[i] = {};
    }

    m_pTargetEntity = nullptr;
}

bool ACamera::Init() {
    return true;
}

void ACamera::Update() {
    GraphicDevice.SetCursorOnOff(false);
    ComputeProjection();

    switch (GetMode()) {
    case MODE_DEBUG3D:
        ProcessDebug();
        break;
    case MODE_FOLLOWENTITY:
        ProcessFollowEntity();
        break;
    }

    UpdateCameraVectors();

    if (Input.GetKeyJustDown(KEY_F1)) {
        GetMode() = GetMode() == MODE_DEBUG3D;

    }

}

void ACamera::Shutdown() {

}

void ACamera::ComputeProjection() {
    glm::mat4 proj = glm::perspective(glm::radians(Camera.GetFov()), SCREEN_ASPECT_RATIO, GetNearClip(), GetFarClip());
    glm::mat4 look = glm::lookAt(m_vPosition, m_vPosition + m_vFront, m_vUp);

    m_mProjection = proj;
    m_mView = look;
}

void ACamera::UpdateCameraVectors() {
    while (m_vAngle.x >= glm::pi<float>()) m_vAngle.x -= 2.0f * glm::pi<float>();
    while (m_vAngle.x < -glm::pi<float>()) m_vAngle.x += 2.0f * glm::pi<float>();
    while (m_vAngle.y >= glm::pi<float>()) m_vAngle.y -= 2.0f * glm::pi<float>();
    while (m_vAngle.y < -glm::pi<float>()) m_vAngle.y += 2.0f * glm::pi<float>();

    float yaw = m_vAngle.x - glm::half_pi<float>();
    float pitch = m_vAngle.y;

    glm::vec3 f;
    f.x = cos(yaw) * cos(pitch);
    f.y = sin(yaw) * cos(pitch);
    f.z = sin(pitch);

    glm::vec3 u(0.0f, 0.0f, 1.0f);

    m_vFront = glm::normalize(f);
    m_vRight = glm::normalize(glm::cross(u, m_vFront));
    m_vUp = glm::normalize(glm::cross(m_vFront, m_vRight));
}

void ACamera::ProcessDebug() {
    m_fFOV = 60.0f;

    float right = 0.0f;
    float up = 0.0f;

    right = Input.m_NewMouse.delta.x * 0.05f;
    up = Input.m_NewMouse.delta.y * 0.05f;

    if (right)
        m_vAngle.x += glm::radians(right);

    if (up)
        m_vAngle.y += glm::radians(up);

    if (m_vAngle.y > glm::radians(89.9f)) m_vAngle.y = glm::radians(89.9f);
    if (m_vAngle.y < -glm::radians(89.9f)) m_vAngle.y = -glm::radians(89.9f);

    right = 0.0f;
    up = 0.0f;

    float velocity = 10.0f * Time.GetDeltaTime();
    float forward = 0.0f;
    float side = 0.0f;

    if (Input.GetKeyDown(KEY_LEFT_SHIFT))
        velocity = 50.0f * Time.GetDeltaTime();

    if (Input.GetKeyDown(KEY_W))
        forward = velocity;
    if (Input.GetKeyDown(KEY_S))
        forward = -velocity;

    if (Input.GetKeyDown(KEY_A))
        side = -velocity;
    if (Input.GetKeyDown(KEY_D))
        side = velocity;

    if (Input.GetKeyDown(KEY_R))
        up = velocity;
    if (Input.GetKeyDown(KEY_F))
        up = -velocity;

    m_vPosition += GetFront() * forward;
    m_vPosition += GetRight() * side;
}

void ACamera::ProcessFollowEntity() {
    if (!GetTargetEntity())
        return;

    m_fFOV = 45.0f;  

    m_vAngle.x = glm::radians(0.0f);
    m_vAngle.y = glm::radians(-90.0f);

    if (m_vAngle.y > glm::radians(89.9f)) m_vAngle.y = glm::radians(89.9f);
    if (m_vAngle.y < -glm::radians(89.9f)) m_vAngle.y = -glm::radians(89.9f);

    glm::vec3 pos = GetTargetEntity()->GetRigidBody()->GetPosition();
    pos.z += 8.0f;
    m_vPosition = pos;
}

void ACamera::SetTargetEntity(CEntity* e) {
    m_pTargetEntity = e;
}

void ACamera::Frustum(glm::mat4 m) {
    m = glm::transpose(m);
    m_vPlanes[FPLANE_LEFT] = m[3] + m[0];
    m_vPlanes[FPLANE_RIGHT] = m[3] - m[0];
    m_vPlanes[FPLANE_BOTTOM] = m[3] + m[1];
    m_vPlanes[FPLANE_TOP] = m[3] - m[1];
    m_vPlanes[FPLANE_NEAR] = m[3] + m[2];
    m_vPlanes[FPLANE_FAR] = m[3] - m[2];

    glm::vec3 crosses[FPLANE_COMBINATIONS] = {
        glm::cross(glm::vec3(m_vPlanes[FPLANE_LEFT]), glm::vec3(m_vPlanes[FPLANE_RIGHT])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_LEFT]), glm::vec3(m_vPlanes[FPLANE_BOTTOM])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_LEFT]), glm::vec3(m_vPlanes[FPLANE_TOP])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_LEFT]), glm::vec3(m_vPlanes[FPLANE_NEAR])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_LEFT]), glm::vec3(m_vPlanes[FPLANE_FAR])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_RIGHT]), glm::vec3(m_vPlanes[FPLANE_BOTTOM])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_RIGHT]), glm::vec3(m_vPlanes[FPLANE_TOP])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_RIGHT]), glm::vec3(m_vPlanes[FPLANE_NEAR])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_RIGHT]), glm::vec3(m_vPlanes[FPLANE_FAR])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_BOTTOM]), glm::vec3(m_vPlanes[FPLANE_TOP])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_BOTTOM]), glm::vec3(m_vPlanes[FPLANE_NEAR])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_BOTTOM]), glm::vec3(m_vPlanes[FPLANE_FAR])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_TOP]), glm::vec3(m_vPlanes[FPLANE_NEAR])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_TOP]), glm::vec3(m_vPlanes[FPLANE_FAR])),
        glm::cross(glm::vec3(m_vPlanes[FPLANE_NEAR]), glm::vec3(m_vPlanes[FPLANE_FAR]))
    };

    m_vPoints[0] = Intersection<FPLANE_LEFT, FPLANE_BOTTOM, FPLANE_NEAR>(crosses);
    m_vPoints[1] = Intersection<FPLANE_LEFT, FPLANE_TOP, FPLANE_NEAR>(crosses);
    m_vPoints[2] = Intersection<FPLANE_RIGHT, FPLANE_BOTTOM, FPLANE_NEAR>(crosses);
    m_vPoints[3] = Intersection<FPLANE_RIGHT, FPLANE_TOP, FPLANE_NEAR>(crosses);
    m_vPoints[4] = Intersection<FPLANE_LEFT, FPLANE_BOTTOM, FPLANE_FAR>(crosses);
    m_vPoints[5] = Intersection<FPLANE_LEFT, FPLANE_TOP, FPLANE_FAR>(crosses);
    m_vPoints[6] = Intersection<FPLANE_RIGHT, FPLANE_BOTTOM, FPLANE_FAR>(crosses);
    m_vPoints[7] = Intersection<FPLANE_RIGHT, FPLANE_TOP, FPLANE_FAR>(crosses);
}

bool ACamera::IsBoxVisible(const glm::vec3& minp, const glm::vec3& maxp) const {
    for (int i = 0; i < FPLANE_COUNT; i++) {
        if ((glm::dot(m_vPlanes[i], glm::vec4(minp.x, minp.y, minp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_vPlanes[i], glm::vec4(maxp.x, minp.y, minp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_vPlanes[i], glm::vec4(minp.x, maxp.y, minp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_vPlanes[i], glm::vec4(maxp.x, maxp.y, minp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_vPlanes[i], glm::vec4(minp.x, minp.y, maxp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_vPlanes[i], glm::vec4(maxp.x, minp.y, maxp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_vPlanes[i], glm::vec4(minp.x, maxp.y, maxp.z, 1.0f)) < 0.0) &&
            (glm::dot(m_vPlanes[i], glm::vec4(maxp.x, maxp.y, maxp.z, 1.0f)) < 0.0)) {
            return false;
        }
    }

    glm::int32 out;
    out = 0; for (int i = 0; i < 8; i++) out += ((m_vPoints[i].x > maxp.x) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((m_vPoints[i].x < minp.x) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((m_vPoints[i].y > maxp.y) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((m_vPoints[i].y < minp.y) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((m_vPoints[i].z > maxp.z) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((m_vPoints[i].z < minp.z) ? 1 : 0); if (out == 8) return false;

    return true;
}

template<eFrustumPlanes a, eFrustumPlanes b, eFrustumPlanes c>
glm::vec3 ACamera::Intersection(const glm::vec3* crosses) const {
    float D = glm::dot(glm::vec3(m_vPlanes[a]), crosses[ij2k<b, c>::k]);
    glm::vec3 res = glm::mat3(crosses[ij2k<b, c>::k], -crosses[ij2k<a, c>::k], crosses[ij2k<a, b>::k]) *
        glm::vec3(m_vPlanes[a].w, m_vPlanes[b].w, m_vPlanes[c].w);
    return res * (-1.0f / D);
}

#include "ACamera.h"
#include "AScreen.h"
#include "AInput.h"
#include "ATime.h"

ACamera Camera;

ACamera::ACamera() {
    m_nMode = MODE_DEBUG3D;
    m_nProjType = PROJECTION_PERSPECTIVE;
    m_mProjection = {};
    m_mView = {};
    m_vPosition = {0.0f, 0.0f, 0.0f};//glm::vec3(159.50f, 139.50f, 10.0f);
    m_vFront = glm::vec3(0.0f, -1.0f, 0.0f);
    m_vUp = glm::vec3(0.0f, 0.0f, 1.0f);
    m_vRight = glm::vec3(0.0f, 0.0f, 0.0f);
    m_vWorldUp = m_vUp;
    m_vAngle.x = -90.0f;
    m_vAngle.y = -90.0f;
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
}

bool ACamera::Init() {
    return true;
}

void ACamera::BeginFrame() {
    ComputeProjection();

    switch (GetMode()) {
    case MODE_DEBUG3D:
        ProcessDebug();
        break;
    case MODE_FOLLOWENTITY:
        break;
    }

    UpdateCameraVectors();
}

void ACamera::EndFrame() {

}

void ACamera::Shutdown() {

}

void ACamera::ComputeProjection() {
    glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 look = glm::lookAt(m_vPosition, m_vPosition + m_vFront, m_vUp);

    switch (Camera.GetMode()) {
    case PROJECTION_PERSPECTIVE:
        proj = glm::perspective(glm::radians(Camera.GetFov()), 16.0f / 9.0f, GetNearClip(), GetFarClip());
        break;
    case PROJECTION_ORTHOGRAPHIC:
        proj = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, GetNearClip(), GetFarClip());
        break;
    }

    m_mProjection = proj;
    m_mView = look;
}

void ACamera::UpdateCameraVectors() {
    glm::vec3 f;
    f.x = cos(glm::radians(m_vAngle.x)) * cos(glm::radians(m_vAngle.y));
    f.y = sin(glm::radians(m_vAngle.x)) * cos(glm::radians(m_vAngle.y));
    f.z = sin(glm::radians(m_vAngle.y));

    glm::vec3 u(0.0f, 0.0f, 1.0f);

    m_vFront = glm::normalize(f);
    m_vRight = glm::normalize(glm::cross(u, m_vFront));
    m_vUp = glm::normalize(glm::cross(m_vFront, m_vRight));

    // Limit camera to map
    //if (m_vPosition.x < MAP_NUM_BLOCKS_X * 0.5f)
    //    m_vPosition.x = MAP_NUM_BLOCKS_X * 0.5f;
    //
    //if (m_vPosition.x > MAP_SCALE_X - (MAP_NUM_BLOCKS_X * 0.5f))
    //    m_vPosition.x = MAP_SCALE_X - (MAP_NUM_BLOCKS_X * 0.5f);
    //
    //if (m_vPosition.y < MAP_NUM_BLOCKS_Y * 0.5f)
    //    m_vPosition.y = MAP_NUM_BLOCKS_Y * 0.5f;
    //
    //if (m_vPosition.y > MAP_SCALE_Y - (MAP_NUM_BLOCKS_X * 0.5f))
    //    m_vPosition.y = MAP_SCALE_Y - (MAP_NUM_BLOCKS_X * 0.5f);
    //
    //if (m_vPosition.z < MAP_NUM_BLOCKS_Z * 0.5f)
    //    m_vPosition.z = MAP_NUM_BLOCKS_Z * 0.5f;
    //
    //if (m_vPosition.z > MAP_SCALE_Z)
    //    m_vPosition.z = MAP_SCALE_Z;
}

void ACamera::ProcessDebug() {
    m_fFOV = 60.0f;

    float right = 0.0f;
    float up = 0.0f;

    right = Input.m_NewMouse.delta.x  * 0.05f;
    up = Input.m_NewMouse.delta.y * 0.05f;

    m_vAngle.x += right;
    m_vAngle.y += up;
    right = 0.0f;
    up = 0.0f;

    if (m_vAngle.y > 89.9f)
        m_vAngle.y = 89.9f;
    if (m_vAngle.y < -89.9f)
        m_vAngle.y = -89.9f;

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

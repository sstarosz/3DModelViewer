#ifndef ST_CORE_CAMERA_HPP
#define ST_CORE_CAMERA_HPP

#include "StObject.hpp"
#include <memory>

namespace st::core
{
    class Camera : public StObject
    {
    public:
        Camera();
        Camera(float angleOfView, float focalLength, float nearClippingPlane, float farClippingPlane);
        ~Camera();
        Camera(const Camera&) = delete;
        Camera(Camera&&) noexcept;
        Camera& operator=(const Camera&) = delete;
        Camera& operator=(Camera&&);

        // Setters
        void setAngleOfView(float angleOfView);
        void setFocalLength(float focalLength);
        void setNearClippingPlane(float nearClippingPlane);
        void setFarClippingPlane(float farClippingPlane);

        // Getters
        float getAngleOfView() const;
        float getFocalLength() const;
        float getNearClippingPlane() const;
        float getFarClippingPlane() const;



    private:
        class PrivateCamera;
        std::unique_ptr<PrivateCamera> m_privateCamera;
    };
} // namespace st::core

#endif // ST_CORE_CAMERA_HPP

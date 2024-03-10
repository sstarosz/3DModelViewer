#ifndef ST_CORE_CAMERA_HPP
#define ST_CORE_CAMERA_HPP

#include "StObject.hpp"
#include <Eigen/Core>
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
        Camera(Camera&&) = delete;
        Camera& operator=(const Camera&) = delete;
        Camera& operator=(Camera&&) = delete;

        // Setters
        void setAngleOfView(float angleOfView);
        void setFocalLength(float focalLength);
        void setNearClippingPlane(float nearClippingPlane);
        void setFarClippingPlane(float farClippingPlane);

        // Getters
        const float getAngleOfView() const;
        const float getFocalLength() const;
        const float getNearClippingPlane() const;
        const float getFarClippingPlane() const;



    private:
        class PrivateCamera;
        std::unique_ptr<PrivateCamera> m_privateCamera;
    };
} // namespace st::core

#endif // ST_CORE_CAMERA_HPP

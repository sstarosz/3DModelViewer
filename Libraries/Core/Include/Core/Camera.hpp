#ifndef ST_CORE_CAMERA_HPP
#define ST_CORE_CAMERA_HPP

#include <memory>
#include <Eigen/Core>

namespace st::core
{
    class Camera
    {
    public:
        Camera();
        Camera(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up);
        ~Camera();
        Camera(const Camera&) = delete;
        Camera(Camera&&) = delete;
        Camera& operator=(const Camera&) = delete;
        Camera& operator=(Camera&&) = delete;

        // Setters
        void setViewMatrix(const Eigen::Matrix4f& viewMatrix);
        void setProjectionMatrix(const Eigen::Matrix4f& projectionMatrix);

        // Getters
        const Eigen::Matrix4f getViewMatrix() const;
        const Eigen::Matrix4f getProjectionMatrix() const;



    private:
        class PrivateCamera;
        std::unique_ptr<PrivateCamera> m_privateCamera;
    };
} // namespace st::core

#endif // ST_CORE_CAMERA_HPP

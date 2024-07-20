#ifndef ST_CORE_CAMERA_HPP
#define ST_CORE_CAMERA_HPP

#include "Node.hpp"
#include "StObject.hpp"
#include <memory>

namespace st::core
{
    class Camera : public Node
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

        
        virtual bool initialize()
        {
            return true;
        }

		virtual bool compute() 
        {
            return true;
        }


        std::weak_ptr<StObject> create() {return std::weak_ptr<StObject>();}; //TODO clean this up

    private:
        class PrivateCamera;
        std::unique_ptr<PrivateCamera> m_privateCamera;
    };


    class Camera2 : public Node
    {
        Camera2(float angleOfView, float focalLength, float nearClippingPlane, float farClippingPlane)
        : Node(),
            m_angleOfView(angleOfView),
            m_focalLength(focalLength),
            m_nearClippingPlane(nearClippingPlane),
            m_farClippingPlane(farClippingPlane)
            {
            }

        virtual bool initialize()
        {
            return true;
        }

		virtual bool compute() 
        {
            return true;
        }


        private:
        float m_angleOfView;
        float m_focalLength;
        float m_nearClippingPlane;
        float m_farClippingPlane;
    };


    

} // namespace st::core

#endif // ST_CORE_CAMERA_HPP

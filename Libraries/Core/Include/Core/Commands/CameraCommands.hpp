#ifndef ST_CORE_COMMANDS_CAMERACOMMANDS_HPP
#define ST_CORE_COMMANDS_CAMERACOMMANDS_HPP

#include "Core/Command.hpp"
#include "Core/ContentManager.hpp"
#include "Core/Nodes/CameraNode.hpp"
#include "Core/Nodes/TransformNode.hpp"


namespace st::core
{
    class CreateCameraCommand : public Command<std::shared_ptr<CameraNode>>
    {
    public:
        CreateCameraCommand(core::ContentManagerHandler contentManager, 
                            const float angleOfView = 45.0f, 
                            const float focalLength = 50.0f,
                            const float nearClippingPlane = 0.1f,
                            const float farClippingPlane = 100.0f);

        std::shared_ptr<CameraNode> execute() override;
        void undo() override;

    private:
        core::ContentManagerHandler m_contentManager;
        std::shared_ptr<CameraNode> m_camera;
        std::shared_ptr<TransformNode> m_transfrom;
        float m_angleOfView;
        float m_focalLength;
        float m_nearClippingPlane;
        float m_farClippingPlane;
    };



} // namespace st::core





#endif // ST_CORE_COMMANDS_CAMERACOMMANDS_HPP
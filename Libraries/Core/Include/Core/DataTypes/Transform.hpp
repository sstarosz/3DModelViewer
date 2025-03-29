#ifndef ST_CORE_TRANSFORM_HPP
#define ST_CORE_TRANSFORM_HPP

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>


#include <spdlog/spdlog.h>

namespace st::core
{
    struct EulerRotation
    {
        enum class RotationOrder
        {
            XYZ,
            XZY,
            YXZ,
            YZX,
            ZXY,
            ZYX
        };

        float x{0.0f};
        float y{0.0f};
        float z{0.0f};

        RotationOrder order{RotationOrder::XYZ};
    };

	struct Transformation
	{
        Eigen::Vector4f translation = Eigen::Vector4f::Zero();
        EulerRotation rotation = {0.0f, 0.0f, 0.0f, EulerRotation::RotationOrder::XYZ};
        Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();
	};




} // namespace st::core

#endif // ST_CORE_CAMERA_HPP
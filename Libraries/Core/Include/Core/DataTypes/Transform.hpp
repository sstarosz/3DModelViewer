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


        float x;
        float y;
        float z;

        RotationOrder order;
    };

	struct Transformation
	{
	  public:
        Transformation() = default;

        
        Eigen::Vector4f translation;
        EulerRotation rotation;
        Eigen::Matrix4f matrix;
	};




} // namespace st::core

#endif // ST_CORE_CAMERA_HPP
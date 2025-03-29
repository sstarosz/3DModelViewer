#include "TransformNode.hpp"

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>

#include <numbers>

namespace st::core
{
    float deg2rad(const float angle)
    {
        return angle * (std::numbers::pi_v<float> / 180.0f);
    }

    float rad2deg(const float rad)
    {
        return rad * (180.0f / std::numbers::pi_v<float>);
    }



    TransformNode::TransformNode() :
        Node()
    {
        spdlog::info("TransformNode: Default constructor");
    }

    bool TransformNode::initialize()
    {
        //TODO: Add default values

        return true;
    }

    bool TransformNode::compute()
    {
        spdlog::info("TransformNode::execute()");

        //Eigen::Matrix4f translationMatrix = Eigen::Matrix4f::Identity();
        //translationMatrix(0, 3) = m_input.translation->value().x();
        //translationMatrix(1, 3) = m_input.translation->value().y();
        //translationMatrix(2, 3) = m_input.translation->value().z();
        //
        //Eigen::Matrix4f rotationMatrix = Eigen::Matrix4f::Identity();
        //rotationMatrix.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_input.rotation->value().x(), Eigen::Vector3f::UnitX()).toRotationMatrix();
        //rotationMatrix.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_input.rotation->value().y(), Eigen::Vector3f::UnitY()).toRotationMatrix();
        //rotationMatrix.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_input.rotation->value().z(), Eigen::Vector3f::UnitZ()).toRotationMatrix();
        //
        //m_output.matrix->setValue(translationMatrix * rotationMatrix);

        return true;
    }

	Eigen::Matrix4f TransformNode::createRotationXMatrix(const float angle)
	{
		const float cosX = std::cos(deg2rad(angle));
        const float sinX = std::sin(deg2rad(angle));

        return Eigen::Matrix4f(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cosX, -sinX, 0.0f,
            0.0f, sinX, cosX, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
	}

    Eigen::Matrix4f TransformNode::createRotationYMatrix(const float angle)
    {
        const float cosY = std::cos(deg2rad(angle));
        const float sinY = std::sin(deg2rad(angle));

        return Eigen::Matrix4f(
             cosY, 0.0f, sinY, 0.0f,
             0.0f, 1.0f, 0.0f, 0.0f,
            -sinY, 0.0f, cosY, 0.0f,
             0.0f, 0.0f, 0.0f, 1.0f);
    }

    Eigen::Matrix4f TransformNode::createRotationZMatrix(const float angle)
    {
        const float cosZ = std::cos(deg2rad(angle));
        const float sinZ = std::sin(deg2rad(angle));

        return Eigen::Matrix4f(
            cosZ, -sinZ, 0.0f, 0.0f,
            sinZ, cosZ, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }


} // namespace st::core
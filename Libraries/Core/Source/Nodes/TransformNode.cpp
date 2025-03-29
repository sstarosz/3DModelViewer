#include "TransformNode.hpp"

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>

#include <numbers>


template<>
struct fmt::formatter<Eigen::Vector3f> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Eigen::Vector3f& v, FormatContext& ctx) const {
        return format_to(ctx.out(), "[{:.2f}, {:.2f}, {:.2f}]", 
            v.x(), v.y(), v.z());
    }
};

template<>
struct fmt::formatter<Eigen::Vector4f> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Eigen::Vector4f& v, FormatContext& ctx) const {
        return format_to(ctx.out(), "[{:.2f}, {:.2f}, {:.2f}, {:.2f}]", 
            v.x(), v.y(), v.z(), v.w());
    }
};

template<>
struct std::formatter<Eigen::Matrix4f>
{
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Eigen::Matrix4f& m, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "[[{:.2f}, {:.2f}, {:.2f}, {:.2f}],\n"
                                         " [{:.2f}, {:.2f}, {:.2f}, {:.2f}],\n"
                                         " [{:.2f}, {:.2f}, {:.2f}, {:.2f}],\n"
                                         " [{:.2f}, {:.2f}, {:.2f}, {:.2f}]]",
            m(0, 0), m(0, 1), m(0, 2), m(0, 3),
            m(1, 0), m(1, 1), m(1, 2), m(1, 3),
            m(2, 0), m(2, 1), m(2, 2), m(2, 3),
            m(3, 0), m(3, 1), m(3, 2), m(3, 3));
    }
};

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
        defineNode("TransformNode");

        return true;
    }

    bool TransformNode::compute()
    {
        spdlog::info("TransformNode::execute()");

        //Eigen::Matrix4f translationMatrix = Eigen::Matrix4f::Identity();
        //translationMatrix(0, 3) = m_input.translation->value().x();
        //translationMatrix(1, 3) = m_input.translation->value().y();
        //translationMatrix(2, 3) = m_input.translation->value().z();

        Eigen::Matrix4f rotationMatrixX = createRotationXMatrix(m_transformation.rotation.x);
        Eigen::Matrix4f rotationMatrixY = createRotationYMatrix(m_transformation.rotation.y);
        Eigen::Matrix4f rotationMatrixZ = createRotationZMatrix(m_transformation.rotation.z);

        Eigen::Matrix4f rotationMatrix = rotationMatrixZ * rotationMatrixY * rotationMatrixX;

        Eigen::Matrix4f translationMatrix = Eigen::Matrix4f::Identity();
        translationMatrix(0, 3) = m_transformation.translation.x();
        translationMatrix(1, 3) = m_transformation.translation.y();
        translationMatrix(2, 3) = m_transformation.translation.z();

        spdlog::warn("TransformNode::compute() - Translation: {}", m_transformation.translation);


        m_transformation.matrix = rotationMatrix * translationMatrix;

        //
        //Eigen::Matrix4f rotationMatrix = Eigen::Matrix4f::Identity();
        //rotationMatrix.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_input.rotation->value().x(), Eigen::Vector3f::UnitX()).toRotationMatrix();
        //rotationMatrix.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_input.rotation->value().y(), Eigen::Vector3f::UnitY()).toRotationMatrix();
        //rotationMatrix.block<3, 3>(0, 0) = Eigen::AngleAxisf(m_input.rotation->value().z(), Eigen::Vector3f::UnitZ()).toRotationMatrix();
        //
        //m_output.matrix->setValue(translationMatrix * rotationMatrix);

        return true;
    }

    Eigen::Matrix4f TransformNode::getLocalMatrix() const
    {
        return m_transformation.matrix;
    }

    void TransformNode::translateBy(const Eigen::Vector4f & translation)
    {
        m_transformation.translation += translation;
    }

void TransformNode::rotateX(const float angle)
    {
        m_transformation.rotation.x += angle;
    }

    void TransformNode::rotateY(const float angle)
    {
        m_transformation.rotation.y += angle;
    }

    void TransformNode::rotateZ(const float angle)
    {
        m_transformation.rotation.z += angle;
    }


	Eigen::Matrix4f TransformNode::createRotationXMatrix(const float angle)
	{
		const float cosX = std::cos(deg2rad(angle));
        const float sinX = std::sin(deg2rad(angle));

        Eigen::Matrix4f rotationXMatrix = Eigen::Matrix4f::Identity();
        rotationXMatrix <<
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cosX, -sinX, 0.0f,
            0.0f, sinX, cosX, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f;

        return rotationXMatrix;
	}

    Eigen::Matrix4f TransformNode::createRotationYMatrix(const float angle)
    {
        const float cosY = std::cos(deg2rad(angle));
        const float sinY = std::sin(deg2rad(angle));

        Eigen::Matrix4f rotationYMatrix = Eigen::Matrix4f::Identity();
        rotationYMatrix <<
            cosY, 0.0f, sinY, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sinY, 0.0f, cosY, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f;

        return rotationYMatrix;
    }

    Eigen::Matrix4f TransformNode::createRotationZMatrix(const float angle)
    {
        const float cosZ = std::cos(deg2rad(angle));
        const float sinZ = std::sin(deg2rad(angle));

        Eigen::Matrix4f rotationZMatrix = Eigen::Matrix4f::Identity();
        rotationZMatrix <<
            cosZ, -sinZ, 0.0f, 0.0f,
            sinZ, cosZ, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f;

        return rotationZMatrix;
    }


} // namespace st::core
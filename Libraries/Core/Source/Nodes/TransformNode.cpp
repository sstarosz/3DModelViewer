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
        spdlog::warn("TransformNode::initialize()");
        //TODO: Add default values
        defineNode("TransformNode");

        m_input.translation = core::TypedAttribute<Eigen::Vector3f>::Builder("Translation")
            .setReadable(true)
            .setWritable(true)
            .setDefaultValue(Eigen::Vector3f(0.0f, 0.0f, 0.0f))
            .build();
        addAttribute(m_input.translation);

        m_input.rotation = core::TypedAttribute<Eigen::Vector3f>::Builder("Rotation")
            .setReadable(true)
            .setWritable(true)
            .setDefaultValue(Eigen::Vector3f(0.0f, 0.0f, 0.0f))
            .build();
        addAttribute(m_input.rotation);

        m_input.scale = core::TypedAttribute<Eigen::Vector3f>::Builder("Scale")
            .setReadable(true)
            .setWritable(true)
            .setDefaultValue(Eigen::Vector3f(1.0f, 1.0f, 1.0f))
            .build();
        addAttribute(m_input.scale);
        
        m_output.translation = core::TypedAttribute<Eigen::Vector3f>::Builder("Translation")
            .setReadable(true)
            .setWritable(false)
            .setDefaultValue(Eigen::Vector3f(0.0f, 0.0f, 0.0f))
            .build();

        addAttribute(m_output.translation);

        m_output.rotation = core::TypedAttribute<Eigen::Vector3f>::Builder("Rotation")
            .setReadable(true)
            .setWritable(false)
            .setDefaultValue(Eigen::Vector3f(0.0f, 0.0f, 0.0f))
            .build();

        addAttribute(m_output.rotation);

        m_output.scale = core::TypedAttribute<Eigen::Vector3f>::Builder("Scale")
            .setReadable(true)
            .setWritable(false)
            .setDefaultValue(Eigen::Vector3f(1.0f, 1.0f, 1.0f))
            .build();
        addAttribute(m_output.scale);

        

        return true;
    }

    bool TransformNode::compute()
    {
        spdlog::info("TransformNode::execute()");


        Eigen::Matrix4f rotationMatrixX = createRotationXMatrix(m_input.rotation->x());
        Eigen::Matrix4f rotationMatrixY = createRotationYMatrix(m_input.rotation->y());
        Eigen::Matrix4f rotationMatrixZ = createRotationZMatrix(m_input.rotation->z());

        Eigen::Matrix4f rotationMatrix = rotationMatrixZ * rotationMatrixY * rotationMatrixX;

        Eigen::Matrix4f translationMatrix = Eigen::Matrix4f::Identity();
        translationMatrix(0, 3) =  m_input.translation->x();
        translationMatrix(1, 3) =  m_input.translation->y();
        translationMatrix(2, 3) =  m_input.translation->z();

        spdlog::warn("TransformNode::compute() - Translation: {}",  *m_input.translation);


        m_transformation.matrix = rotationMatrix * translationMatrix;


        return true;
    }

    Eigen::Matrix4f TransformNode::getLocalMatrix() const
    {
        return m_transformation.matrix;
    }

    void TransformNode::translateBy(const Eigen::Vector4f & translation)
    {
        m_input.translation =  *m_input.translation + Eigen::Vector3f(translation.x(), translation.y(), translation.z());
    }

    void TransformNode::rotateX(const float angle)
    {
        m_input.rotation = Eigen::Vector3f(m_input.rotation->x() + angle, m_input.rotation->y(), m_input.rotation->z());
    }

    void TransformNode::rotateY(const float angle)
    {
        m_input.rotation = Eigen::Vector3f(m_input.rotation->x(), m_input.rotation->y() + angle, m_input.rotation->z());
    }

    void TransformNode::rotateZ(const float angle)
    {
        m_input.rotation = Eigen::Vector3f(m_input.rotation->x(), m_input.rotation->y(), m_input.rotation->z() + angle);
        
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
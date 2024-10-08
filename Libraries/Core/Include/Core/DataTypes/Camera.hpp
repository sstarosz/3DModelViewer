#ifndef ST_CORE_CAMERA_HPP
#define ST_CORE_CAMERA_HPP

#include <Eigen/Core>

#include <spdlog/spdlog.h>

namespace st::core
{

	class Camera
	{
	  public:
		Camera() = default;
		Camera(float angleOfView, float focalLength,
			   float nearClippingPlane, float farClippingPlane);

		void setPosition(const Eigen::Vector3f& position);

		Eigen::Matrix4f getViewMatrix() const;
		Eigen::Matrix4f getProjectionMatrix() const;




		// private:
		// Camera location
		Eigen::Vector3f m_position;
		Eigen::Vector3f m_target;
		Eigen::Vector3f m_up;

		// Camera parameters
		float m_angleOfView;
		float m_focalLength;
		float m_nearClippingPlane;
		float m_farClippingPlane;

		float m_width;
		float m_height;

		// Camera state
		enum class State
		{
			eIdle,
			eOrbit,
			ePan,
			eZoom
		};

		State m_currentState = State::eIdle;
		float m_mouseClickX;
		float m_mouseClickY;

	};

} // namespace st::core

namespace fmt {
	template <>
	struct formatter<Eigen::Matrix4f>
	{
		constexpr auto parse(format_parse_context& ctx)
		{
			return ctx.end();
		}

		template <typename FormatContext>
		auto format(const Eigen::Matrix4f& m, FormatContext& ctx) const
		{
			return format_to(ctx.out(), "\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]",
						 m(0, 0), m(0, 1), m(0, 2), m(0, 3),
						 m(1, 0), m(1, 1), m(1, 2), m(1, 3),
						 m(2, 0), m(2, 1), m(2, 2), m(2, 3),
						 m(3, 0), m(3, 1), m(3, 2), m(3, 3));
		}
	};

	template <>
	struct formatter<Eigen::Vector3f>
	{
		constexpr auto parse(format_parse_context& ctx)
		{
			return ctx.end();
		}

		template <typename FormatContext>
		auto format(const Eigen::Vector3f& v, FormatContext& ctx) const
		{
			return format_to(ctx.out(), "[{}, {}, {}]", v.x(), v.y(), v.z());
		}
	};
}


#endif // ST_CORE_CAMERA_HPP
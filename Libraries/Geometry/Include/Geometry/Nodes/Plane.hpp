#ifndef ST_GEOMETRY_PLANE_HPP
#define ST_GEOMETRY_PLANE_HPP

#include "Core/Node.hpp"
#include "Core/NodeRegistry.hpp"
#include "Geometry/DataTypes/Mesh.hpp"

namespace st::geometry
{
	class Plane : public core::Node
	{
	  public:
		/*----------------------*/
		/*-------Methods--------*/
		/*----------------------*/
		bool initialize() override;
		bool compute() override;

		/*----------------------*/
		/*-------Setters--------*/
		/*----------------------*/
		void setWidth(float width);
		void setHeight(float height);
		void setSubdivisionWidth(uint32_t subdivisionWidth);
		void setSubdivisionHeight(uint32_t subdivisionHeight);

		/*----------------------*/
		/*-------Getters--------*/
		/*----------------------*/
		float getWidth() const;
		float getHeight() const;
		uint32_t getSubdivisionWidth() const;
		uint32_t getSubdivisionHeight() const;

		geometry::Mesh getMesh() const;


	  private:
		struct Inputs
		{
			core::NumericInputHandler<float> width;
			core::NumericInputHandler<float> height;

			core::NumericInputHandler<uint32_t> subdivisionWidth;
			core::NumericInputHandler<uint32_t> subdivisionHeight;
		};

		struct Outputs
		{
			core::TypedOutputHandler<geometry::Mesh> mesh;
		};

		Inputs m_inputs;
		Outputs m_outputs;
	};

	static core::RegisterNode<Plane> plane("Plane");

} // namespace st::geometry

#endif //  ST_GEOMETRY_PLANE_HPP
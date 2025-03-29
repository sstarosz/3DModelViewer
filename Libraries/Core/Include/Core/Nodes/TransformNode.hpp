#ifndef ST_CORE_NODES_TRANSFORMNODE_HPP
#define ST_CORE_NODES_TRANSFORMNODE_HPP

#include "Core/Node.hpp"
#include "Core/DataTypes/Transform.hpp"

#include <Eigen/Core>

namespace st::core
{

	class TransformNode : public Node //TODO: Make actual node with input and output
	{
    public:
		TransformNode();

		/*----------------------*/
		/*-------Methods--------*/
		/*----------------------*/
		virtual bool initialize() override;
		virtual bool compute() override;
		virtual Eigen::Matrix4f getLocalMatrix() const override; 

		/*----------------------*/
		/*-------Setters--------*/
		/*----------------------*/
		void translateBy(const Eigen::Vector4f& translation);

		void rotateX(const float angle);
		void rotateY(const float angle);
		void rotateZ(const float angle);

		/*----------------------*/
		/*-------Getters--------*/
		/*----------------------*/


    private:
		struct Inputs
		{
			//TODO: Add input attributes
		} m_input;
		
		struct Outputs
		{
			//TODO: Add output attributes
		} m_output;

		Transformation m_transformation;


        Eigen::Matrix4f createRotationXMatrix(const float angle);
        Eigen::Matrix4f createRotationYMatrix(const float angle);
        Eigen::Matrix4f createRotationZMatrix(const float angle);

	};

} // namespace st::core

#endif // ST_CORE_NODES_TRANSFORMNODE_HPP

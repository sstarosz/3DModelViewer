#ifndef ST_GEOMETRY_PLANE_HPP
#define ST_GEOMETRY_PLANE_HPP

#include "Core//NodeGraph.hpp"
#include "Core/Nodes/Node.hpp"
#include "Core/Mesh.hpp"
#include <Eigen/Core>
#include <vector>

namespace st::renderer
{

    /**
     * @brief 
     * 
     */
    class Plane : public core::Node
    {
        public:
            Plane()
            {
                
                m_vertices.push_back(geometry::Vertex(Eigen::Vector3f(-0.5F, 0.0F, -0.5F)));
                m_vertices.push_back(geometry::Vertex(Eigen::Vector3f(0.5F, 0.0F, -0.5F)));
                m_vertices.push_back(geometry::Vertex(Eigen::Vector3f(0.5F, 0.0F, 0.5F)));
                m_vertices.push_back(geometry::Vertex(Eigen::Vector3f(-0.5F, 0.0F, 0.5F)));

                m_indices.push_back(0);
                m_indices.push_back(1);
                m_indices.push_back(2);

                m_indices.push_back(2);
                m_indices.push_back(3);
                m_indices.push_back(0);
            }
 
            const std::vector<geometry::Vertex> getVertices() const
            {
                return m_vertices;
            }

            const std::vector<uint32_t> getIndices() const
            {
                return m_indices;
            }

        private:
            std::vector<geometry::Vertex> m_vertices;
            std::vector<uint32_t> m_indices;

    };



    class Plane2 : public core::Node2
    {
        public:
        Plane2()
        {
        }


        //TODO change return type to std::expected
        bool initialize() override
        {
            defineNode("Plane");

            m_inputs.width.setName("Width");
            m_inputs.width.setDefault(1.0F);
            m_inputs.width.setMin(0.001F);
            m_inputs.width.setMax(100000.0F);
            m_inputs.width.setSoftMinValue(0.001F);
            m_inputs.width.setSoftMaxValue(100.0F);
            addInput(m_inputs.width);

            m_inputs.height.setName("Height");
            m_inputs.height.setDefault(1.0F);
            m_inputs.height.setMin(0.001F);
            m_inputs.height.setMax(100000.0F);
            m_inputs.height.setSoftMinValue(0.001F);
            m_inputs.height.setSoftMaxValue(100.0F);
            addInput(m_inputs.height);

            m_inputs.subdivisionWidth.setName("Subdivision Width");
            m_inputs.subdivisionWidth.setDefault(1);
            m_inputs.subdivisionWidth.setMin(1);
            m_inputs.subdivisionWidth.setMax(100000);
            m_inputs.subdivisionWidth.setSoftMinValue(1);
            m_inputs.subdivisionWidth.setSoftMaxValue(100);
            addInput(m_inputs.subdivisionWidth);

            m_inputs.subdivisionHeight.setName("Subdivision Height");
            m_inputs.subdivisionHeight.setDefault(1);
            m_inputs.subdivisionHeight.setMin(1);
            m_inputs.subdivisionHeight.setMax(100000);
            m_inputs.subdivisionHeight.setSoftMinValue(1);
            m_inputs.subdivisionHeight.setSoftMaxValue(100);
            addInput(m_inputs.subdivisionHeight);
            

            m_outputs.mesh.setName("Mesh");
            addOutput(m_outputs.mesh);
            
            return true;
        }

        bool compute() override
        {
            //TODO implement
            //for(int i = 0; i < m_inputs.subdivisionWidth; i++)
            //{
            //    for(int j = 0; j < m_inputs.subdivisionHeight; j++)
            //    {
                    core::FloatPointVector vertices;
                    core::IntVector indices;

                    vertices.push_back(Eigen::Vector3f(-0.5F, 0.0F, -0.5F));
                    vertices.push_back(Eigen::Vector3f(0.5F, 0.0F, -0.5F));
                    vertices.push_back(Eigen::Vector3f(0.5F, 0.0F, 0.5F));
                    vertices.push_back(Eigen::Vector3f(-0.5F, 0.0F, 0.5F));

                    indices.push_back(0);
                    indices.push_back(1);
                    indices.push_back(2);

                    indices.push_back(2);
                    indices.push_back(3);
                    indices.push_back(0);
           //    }
           //}

            m_outputs.mesh = core::MeshData(vertices, indices); 


            return true;
        }


       struct Inputs
       {
         core::Input<float> width;
         core::Input<float> height;
         core::Input<uint32_t> subdivisionWidth;
         core::Input<uint32_t> subdivisionHeight;
       };

        struct Outputs
        {
            core::Output<core::MeshData> mesh;
        };

        private:
            //std::vector<geometry::Vertex> m_vertices;
            //std::vector<uint32_t> m_indices;

        Inputs m_inputs;
        Outputs m_outputs;

    };


    //RegisterNode<Plane2> plane2("Plane2");


} // namespace st::renderer

#endif //  ST_GEOMETRY_GRID_HPP
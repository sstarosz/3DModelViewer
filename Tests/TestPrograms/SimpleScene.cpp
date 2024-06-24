#include "Core/Camera.hpp"
#include "Core/ContentManager.hpp"
#include "Core/Scene.hpp"
#include "Core/Transform.hpp"

#include "Renderer/Grid.hpp"
#include "Renderer/MaterialManager.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/Plane.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/StandardMaterial.hpp"
#include "Ui/MainWindow.hpp"
#include "Ui/Viewport.hpp"

#include <QApplication>

using namespace st;

namespace st
{

	class GuiManager
	{
	  public:
		GuiManager(core::ContentManager& contentManager) :
			m_mainWindow(contentManager)
		{
		}

	  public:
		void initialize()
		{
			m_mainWindow.initialize();
		}

		void show()
		{
			m_mainWindow.show();
		}

	  private:
		ui::MainWindow m_mainWindow;
	};

	class Creator
	{
	  public:
		Creator(core::ContentManager& contentManager) :
			m_contentManager(contentManager)
		{
		}

		std::weak_ptr<core::Node> camera(
			const float angleOfView = 45.0f,
			const float focalLength = 35.0f,
			const float nearClippingPlane = 0.1f,
			const float farClippingPlane = 10000.0f)
		{
			std::shared_ptr<core::Camera> camera = std::make_shared<core::Camera>(angleOfView,
																				  focalLength,
																				  nearClippingPlane,
																				  farClippingPlane);

			// m_contentManager.add(camera);

			return std::weak_ptr<core::Node>{camera};
		}

		std::weak_ptr<core::Node2> plane(
			[[mayby_unused]] const float width = 1.0f,
			[[mayby_unused]] const float height = 1.0f)
		{
			std::shared_ptr<renderer::Plane2> plane = std::make_shared<renderer::Plane2>();
			// std::shared_ptr<renderer::Mesh> mesh = std::make_shared<renderer::Mesh>();

			m_contentManager.add(plane);
			plane->initialize(); // TODO clean up
			// m_contentManager.add(mesh);
			return std::weak_ptr<core::Node2>{plane};
		}

		std::weak_ptr<renderer::StandardMaterial2> standardMaterial()
		{
			std::shared_ptr<renderer::StandardMaterial2> standardMaterial = std::make_shared<renderer::StandardMaterial2>();
			m_contentManager.add(standardMaterial);
			standardMaterial->initialize();

			return std::weak_ptr<renderer::StandardMaterial2>{standardMaterial};
		}

		std::weak_ptr<renderer::Renderer> renderer(std::weak_ptr<core::Node> camera)
		{
			std::shared_ptr<renderer::Renderer> renderer = std::make_shared<renderer::Renderer>();
			m_contentManager.add(renderer);
			renderer->initialize();

			return std::weak_ptr<renderer::Renderer>{renderer};
		}

	  private:
		core::ContentManager& m_contentManager;
	};

	class Transformer
	{
	  public:
		Transformer& translate(Eigen::Vector3f translation)
		{
			// core::Transform transform{node};
			// transform.setTranslation(translation);
			return *this;
		}

		Transformer& scale(Eigen::Vector3f scale)
		{
			// core::Transform transform{node};
			// transform.setScale(scale);
			return *this;
		}
	};

	class ModifyContext
	{
	  public:
		core::ContentManager* m_contentManager;
		std::weak_ptr<core::Node2> selectedNode;
	};

	class MaterialModifier
	{
	  public:
		MaterialModifier(ModifyContext context):
			m_context(context)
		{
		}

		void assign(std::weak_ptr<renderer::StandardMaterial2> material)
		{
			// renderer::MaterialManager materialManager;
			// materialManager.assignMaterialToMesh(material, node);

			// Check if mesh node has MeshData output

			std::shared_ptr<renderer::StandardMaterial2> targetMaterial = material.lock();
			std::shared_ptr<core::Node2> sourceNode = m_context.selectedNode.lock();

			std::shared_ptr<core::AttributeBase> sourceAttribute = nullptr;
			std::shared_ptr<core::AttributeBase> targetAttribute = nullptr;

			// Check if mesh node has MeshData output
			if (sourceNode)
			{
				auto attributes = sourceNode->getAttributes();
				for (auto& attribute : attributes)
				{
					if(core::MeshData* meshData = dynamic_cast<core::MeshData*>(attribute.get()))
					{
						std::println("Found MeshData");
					    sourceAttribute = attribute;
					}
				}
			}

			// Check if material has Material output
			if (auto materialNode = material.lock())
			{
				auto attributes = materialNode->getAttributes();
				for (auto& attribute : attributes)
				{
					if (renderer::Renderable* materialData = dynamic_cast<renderer::Renderable*>(attribute.get()))
					{
						std::println("Found Material");
						targetAttribute = attribute;
					}
				}
			}

			// Add connection
			m_context.m_contentManager->getMainNodeGraph().addConnection(sourceNode,
																		sourceAttribute,
																		targetMaterial,
																		targetAttribute);
		}

	  private:
		ModifyContext m_context;
	};


    /**
     * @brief Main class responsible for modifying nodes
     * 
     * Contain sub-classes for modifying different aspects of a node
     */
	class Modifier
	{
	  public:
		Modifier(ModifyContext context):
			m_context(context)
		{
		}

		std::weak_ptr<core::Transform> transform()
		{
			// m_transformer = Transformer(node);
			std::shared_ptr<core::Transform> transform = std::make_shared<core::Transform>();

			// m_contentManager.addToCurrentContext(transform, m_currentContext);
			return std::weak_ptr<core::Transform>{transform};
		}

		MaterialModifier material()
		{
			//TODO refactor
			// Throw exception if node can't have material
			if(auto node = m_context.selectedNode.lock())
			{
				//Throw if node is not of type plane
				if(!std::dynamic_pointer_cast<renderer::Plane2>(node))
				{
					throw std::runtime_error("Node is not of type Plane");
				}
			}


			MaterialModifier materialManager{m_context};
			return materialManager;
		}
    
        private:
        ModifyContext m_context;
	};

	class Application
	{
	  public:
		Application(int argc, char* argv[]) :
			m_app(argc, argv),
			m_contentManager(),
			m_guiManager(m_contentManager),
			m_creator(m_contentManager)
		{
		}

		int run()
		{
			// Show all created gui elements if any
			m_guiManager.initialize();
			m_guiManager.show();

			return m_app.exec();
		}

		Creator& create()
		{
			return m_creator;
		}

		Modifier modify(std::weak_ptr<core::Node2> node)
		{
			ModifyContext context;
            context.m_contentManager = &m_contentManager;
			context.selectedNode = node;


			Modifier modifier{context};
			return modifier;
		}


	  private:
		QApplication m_app;

		core::ContentManager m_contentManager;
		GuiManager m_guiManager;

		Creator m_creator;
		// GuiCreator m_guiCreator; To investigate future use
	};

} // namespace st

int main(int argc, char* argv[])
{
	Application app(argc, argv);

	// Add Camera
	auto camera = app.create()
					  .camera(45.0f,
							  35.0f,
							  0.1f,
							  10000.0f);

	// TODO revert -app.modify(camera).transform();
	//.translate(Eigen::Vector3f{0.0f, 0.0f, 6.0f})
	//.scale(Eigen::Vector3f{1.0f, 1.0f, 1.0f})
	//.translate(Eigen::Vector3f{0.0f, 0.0f, -6.0f})
	//.translate(Eigen::Vector3f{0.0f, 0.0f, 6.0f});

	/*-----------------------------------------------------------------------------------------------*/
	/*--------------------------------------Add Scene Content----------------------------------------*/
	/*-----------------------------------------------------------------------------------------------*/
	// Add Plane to Scene
	auto plane = app.create().plane();

	/*-----------------------------------------------------------------------------------------------*/
	/*--------------------------------------Add
	---------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------*/
	auto standardMaterial = app.create().standardMaterial();

	// Assign Material to Plane
	// app.modify().material().assignMaterialToMesh(standardMaterial, plane);
	// app.modify().material(standardMaterial).assignTo(plane);
	// app.modify(plane).material(standardMaterial).assign();
	app.modify(plane).material().assign(standardMaterial);

	/*-----------------------------------------------------------------------------------------------*/
	/*-------------------------------------Setup renderer--------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------*/
	auto renderer = app.create().renderer(camera);

	/*-----------------------------------------------------------------------------------------------*/
	/*-------------------------------------Setup Gui-------------------------------------------------*/
	/*-----------------------------------------------------------------------------------------------*/

	return app.run();
}
#include "Core/Camera.hpp"
#include "Core/ContentManager.hpp"
#include "Core/Scene.hpp"
#include "Core/Transform.hpp"
#include "Core/EventSystem.hpp"

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

	class Command
	{
	  public:
		virtual ~Command() = default;
		virtual void execute() = 0;
		virtual void undo() = 0;
	};

	class CommandManager
	{
		public:
		CommandManager() = default;

		void execute(std::unique_ptr<Command> command)
		{
			command->execute();
			m_undoStack.push_back(std::move(command));
			m_redoStack.clear();
		}

		void undo()
		{
			if(!m_undoStack.empty())
			{
				std::unique_ptr<Command> command = std::move(m_undoStack.back());
				m_undoStack.pop_back();
				command->undo();
				m_redoStack.push_back(std::move(command));
			}
		}

		void redo()
		{
			if(m_redoStack.empty())
			{
				std::unique_ptr<Command> command = std::move(m_redoStack.back());
				m_redoStack.pop_back();
				command->execute();
				m_undoStack.push_back(std::move(command));
			}
		}

	  private:
		std::vector<std::unique_ptr<Command>> m_undoStack;
		std::vector<std::unique_ptr<Command>> m_redoStack;

	};
	class CreatePlaneCommand : public Command
	{
		public:
		CreatePlaneCommand(core::ContentManagerHandler contentManager) :
			m_contentManager(contentManager)
		{
		}

		void execute() override
		{
			std::shared_ptr<renderer::Plane2> plane = std::make_shared<renderer::Plane2>();
			plane->initialize();
			m_contentManager->add(plane);
		}

		void undo() override
		{
			//TODO implement
			//m_contentManager.remove(m
		}

	  private:
		core::ContentManagerHandler m_contentManager;
	};

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
		Creator(core::ContentManagerHandler contentManager) :
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

		std::weak_ptr<core::Node> plane(
			[[mayby_unused]] const float width = 1.0f,
			[[mayby_unused]] const float height = 1.0f)
		{
			//Create Plane command
			std::unique_ptr<CreatePlaneCommand> command = std::make_unique<CreatePlaneCommand>(m_contentManager);

			//return std::weak_ptr<core::Node>{plane};
			return std::weak_ptr<core::Node>{};

		}

		std::weak_ptr<renderer::StandardMaterial2> standardMaterial()
		{
			std::shared_ptr<renderer::StandardMaterial2> standardMaterial = std::make_shared<renderer::StandardMaterial2>();
			m_contentManager.add(standardMaterial);

			return std::weak_ptr<renderer::StandardMaterial2>{standardMaterial};
		}

		std::weak_ptr<renderer::Renderer> renderer(std::weak_ptr<core::Node> camera)
		{
			std::shared_ptr<renderer::Renderer> renderer = std::make_shared<renderer::Renderer>();
			m_contentManager.add(renderer);

			//Make material nodes automatically connect to renderer
			core::NodeGraphHandler nodeGraph = m_contentManager.getMainNodeGraph();

			std::shared_ptr<core::Attribute> targetAttribute = nullptr;
			for(auto& attribute : renderer->getAttributes())
			{
				if (std::shared_ptr<core::TypedAttribute<renderer::Renderable>> renderable = std::dynamic_pointer_cast<core::TypedAttribute<renderer::Renderable>>(attribute))
				{
					std::println("Found Renderable");
					targetAttribute = attribute;
				}
			}



			for(auto& node : nodeGraph.getNodes())
			{
				if(std::dynamic_pointer_cast<renderer::StandardMaterial2>(node))
				{
					for(auto& attribute : node->getAttributes())
					{
						if (std::shared_ptr<core::TypedAttribute<renderer::Renderable>> renderable = std::dynamic_pointer_cast<core::TypedAttribute<renderer::Renderable>>(attribute))
						{
							std::println("Found Renderable");
							nodeGraph.addConnection(node, renderable, renderer, targetAttribute);
						}
					}
				}
			}


			return std::weak_ptr<renderer::Renderer>{renderer};
		}

	  private:
		core::ContentManagerHandler m_contentManager;
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
		std::weak_ptr<core::Node> selectedNode;
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
			std::shared_ptr<core::Node> sourceNode = m_context.selectedNode.lock();

			std::shared_ptr<core::Attribute> sourceAttribute = nullptr;
			std::shared_ptr<core::Attribute> targetAttribute = nullptr;

			// Check if mesh node has MeshData output
			if (sourceNode)
			{
				auto attributes = sourceNode->getAttributes();
				for (auto& attribute : attributes)
				{
					//Check if attribute is a TypedAttribute
					if(std::shared_ptr<core::TypedAttribute<core::MeshData>> meshData = std::dynamic_pointer_cast<core::TypedAttribute<core::MeshData>>(attribute))
					{
						std::println("Found MeshData");
					    sourceAttribute = attribute;
					}
				}
			}

			// Check if material has input for meshData
			if (auto materialNode = material.lock())
			{
				auto attributes = materialNode->getAttributes();
				for (auto& attribute : attributes)
				{
					if (std::shared_ptr<core::TypedAttribute<core::MeshData>> meshData = std::dynamic_pointer_cast<core::TypedAttribute<core::MeshData>>(attribute))
					{
						std::println("Found MeshData");
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
			m_eventSystem(),
			m_contentManager(m_eventSystem),
			m_guiManager(m_contentManager),
			m_creator(core::ContentManagerHandler(&m_contentManager))
		{
		}

		int init()
		{
			m_contentManager.initialize();
			m_guiManager.initialize();

			return 0;
		}


		int run()
		{

			
			// Show all created gui elements if any
			m_guiManager.show();

			return m_app.exec();
		}

		Creator& create()
		{
			return m_creator;
		}

		Modifier modify(std::weak_ptr<core::Node> node)
		{
			ModifyContext context;
            context.m_contentManager = &m_contentManager;
			context.selectedNode = node;


			Modifier modifier{context};
			return modifier;
		}


	  private:
		QApplication m_app;


		CommandManager m_commandManager;
		core::EventSystem m_eventSystem;
		core::ContentManager m_contentManager;
		GuiManager m_guiManager;

		Creator m_creator;
		// GuiCreator m_guiCreator; To investigate future use
	};

} // namespace st

int main(int argc, char* argv[])
{
	Application app(argc, argv);
	app.init();

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
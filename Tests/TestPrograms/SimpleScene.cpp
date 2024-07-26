#include "Core/Nodes/Camera.hpp"
#include "Core/ContentManager.hpp"
#include "Core/CommandManager.hpp"

#include "Renderer/MaterialManager.hpp"
#include "Geometry/DataTypes/Mesh.hpp"
#include "Geometry/Nodes/Plane.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/StandardMaterial.hpp"
#include "Ui/MainWindow.hpp"
#include "Ui/Viewport.hpp"

#include <QApplication>

#include <spdlog/spdlog.h>

using namespace st;

namespace st
{
	class CreatePlaneCommand : public core::Command
	{
		public:
		CreatePlaneCommand(core::ContentManagerHandler contentManager) :
			m_contentManager(contentManager)
		{
		}

		void execute() override
		{
			m_plane = std::make_shared<geometry::Plane>();
			m_plane->initialize();
			m_contentManager->getMainNodeGraph()->addNode(m_plane);
		}

		void undo() override
		{
			//TODO implement
			//m_contentManager.remove(m
		}

		std::shared_ptr<geometry::Plane> getResult()
		{
			return m_plane;
		}

	  private:
		core::ContentManagerHandler m_contentManager;
		std::shared_ptr<geometry::Plane> m_plane;
	};


	class CreateStandardMaterialCommand : public core::Command
	{
		public:
		CreateStandardMaterialCommand(core::ContentManagerHandler contentManager) :
			m_contentManager(contentManager)
		{
		}

		void execute() override
		{
			m_standardMaterial = std::make_shared<renderer::StandardMaterial2>();
			m_standardMaterial->initialize();
			m_contentManager->getMainNodeGraph()->addNode(m_standardMaterial);
		}

		void undo() override
		{
			//TODO implement
			//m_contentManager.remove(m
		}

		std::shared_ptr<renderer::StandardMaterial2> getResult()
		{
			return m_standardMaterial;
		}

	  private:
		core::ContentManagerHandler m_contentManager;
		std::shared_ptr<renderer::StandardMaterial2> m_standardMaterial;
	};

	class CreateRendererCommand : public core::Command
	{
		public:
		CreateRendererCommand(core::ContentManagerHandler contentManager) :
			m_contentManager(contentManager)
		{
		}

		void execute() override
		{
			m_renderer = std::make_shared<renderer::Renderer>();
			m_renderer->initialize();

			core::NodeGraphHandler nodeGraph = m_contentManager->getMainNodeGraph();
			nodeGraph->addNode(m_renderer);

			//Make material nodes automatically connect to renderer

			std::shared_ptr<core::Attribute> targetAttribute = nullptr;
			for(auto& attribute : m_renderer->getAttributes())
			{
				if (std::shared_ptr<core::TypedAttribute<renderer::Renderable>> renderable = std::dynamic_pointer_cast<core::TypedAttribute<renderer::Renderable>>(attribute))
				{
					std::println("Found Renderable");
					targetAttribute = attribute;
				}
			}

			for(auto& node : nodeGraph->getNodes())
			{
				if(std::dynamic_pointer_cast<renderer::StandardMaterial2>(node))
				{
					for(auto& attribute : node->getAttributes())
					{
						if (std::shared_ptr<core::TypedAttribute<renderer::Renderable>> renderable = std::dynamic_pointer_cast<core::TypedAttribute<renderer::Renderable>>(attribute))
						{
							std::println("Found Renderable");
							nodeGraph->addConnection(node, renderable, m_renderer, targetAttribute);
						}
					}
				}
			}
		}

		void undo() override
		{
			//TODO implement
			//m_contentManager.remove(m
		}

		std::shared_ptr<renderer::Renderer> getResult()
		{
			return m_renderer;
		}

	  private:
		core::ContentManagerHandler m_contentManager;
		std::shared_ptr<renderer::Renderer> m_renderer;
	};

	class CommandAssignMaterial : public core::Command
	{
		public:
		CommandAssignMaterial(core::ContentManagerHandler contentManager,
							  std::weak_ptr<renderer::StandardMaterial2> material,
							  std::weak_ptr<core::Node> node) :
			m_contentManager(contentManager),
			m_material(material),
			m_node(node)
		{
		}

		void execute() override
		{
			std::shared_ptr<renderer::StandardMaterial2> material = m_material.lock();
			std::shared_ptr<core::Node> node = m_node.lock();

			std::shared_ptr<core::Attribute> sourceAttribute = nullptr;
			std::shared_ptr<core::Attribute> targetAttribute = nullptr;

			// Check if mesh node has Mesh output
			if (auto meshNode = node)
			{
				auto attributes = meshNode->getAttributes();
				for (auto& attribute : attributes)
				{
					//Check if attribute is a TypedAttribute
					if (std::shared_ptr<core::TypedAttribute<geometry::Mesh>> meshData = std::dynamic_pointer_cast<core::TypedAttribute<geometry::Mesh>>(attribute))
					{
						std::println("Found Mesh");
						sourceAttribute = attribute;
					}
				}
			}

			// Check if material has input for meshData
			if (auto materialNode = material)
			{
				auto attributes = materialNode->getAttributes();
				for (auto& attribute : attributes)
				{
					if (std::shared_ptr<core::TypedAttribute<geometry::Mesh>> meshData = std::dynamic_pointer_cast<core::TypedAttribute<geometry::Mesh>>(attribute))
					{
						std::println("Found Mesh");
						targetAttribute = attribute;
					}
				}
			}

			// Add connection
			m_contentManager->getMainNodeGraph()->addConnection(node,
																sourceAttribute,
																material,
																targetAttribute);
		}

		void undo() override
		{
			//TODO implement
		}

	  private:
		core::ContentManagerHandler m_contentManager;
		std::weak_ptr<renderer::StandardMaterial2> m_material;
		std::weak_ptr<core::Node> m_node;
	};
	

	class GuiManager
	{
	  public:
		GuiManager(core::ContentManagerHandler contentManager) :
			m_contentManager(contentManager),
			m_mainWindow(contentManager)
		{
			spdlog::info("GuiManager::GuiManager()");
		}

	  public:
		void initialize()
		{
			spdlog::info("GuiManager::initialize()");
			m_mainWindow.initialize();
		}

		void show()
		{
			m_mainWindow.show();
		}

	  private:
		core::ContentManagerHandler m_contentManager;
		ui::MainWindow m_mainWindow;
	};

	class Creator
	{
	  public:
		Creator(core::ContentManagerHandler contentManager,
				core::CommandManagerHandler commandManager):
			m_contentManager(contentManager),
			m_commandManager(commandManager)
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

			//TODO

			return std::weak_ptr<core::Node>{camera};
		}

		std::weak_ptr<core::Node> plane(
			[[maybe_unused]] const float width = 1.0f,
			[[maybe_unused]] const float height = 1.0f)
		{
			spdlog::info("Creator::plane()");
			//Create Plane command
			std::unique_ptr<CreatePlaneCommand> command = std::make_unique<CreatePlaneCommand>(m_contentManager);
			CreatePlaneCommand* commandPtr = command.get();
			m_commandManager->execute(std::move(command));

			return std::weak_ptr<core::Node>{commandPtr->getResult()};

		}

		std::weak_ptr<renderer::StandardMaterial2> standardMaterial()
		{
			spdlog::info("Creator::standardMaterial()");
			std::unique_ptr<CreateStandardMaterialCommand> command = std::make_unique<CreateStandardMaterialCommand>(m_contentManager);
			CreateStandardMaterialCommand* commandPtr = command.get();
			m_commandManager->execute(std::move(command));

			return std::weak_ptr<renderer::StandardMaterial2>{commandPtr->getResult()};

		}

		std::weak_ptr<renderer::Renderer> renderer(std::weak_ptr<core::Node> camera)
		{
			spdlog::info("Creator::renderer()");
			std::unique_ptr<CreateRendererCommand> command = std::make_unique<CreateRendererCommand>(m_contentManager);
			CreateRendererCommand* commandPtr = command.get();
			m_commandManager->execute(std::move(command));
			return std::weak_ptr<renderer::Renderer>{commandPtr->getResult()};
		}

	  private:
		core::ContentManagerHandler m_contentManager;
		core::CommandManagerHandler m_commandManager;
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
		MaterialModifier(ModifyContext context, core::CommandManagerHandler commandManager):
			m_context(context),
			m_commandManager(commandManager)
		{
		}

		void assign(std::weak_ptr<renderer::StandardMaterial2> material)
		{
			std::unique_ptr<CommandAssignMaterial> command = std::make_unique<CommandAssignMaterial>(m_context.m_contentManager, material, m_context.selectedNode);
			m_commandManager->execute(std::move(command));
		}

	  private:
		ModifyContext m_context;
		 core::CommandManagerHandler m_commandManager;
	};


    /**
     * @brief Main class responsible for modifying nodes
     * 
     * Contain sub-classes for modifying different aspects of a node
     */
	class Modifier
	{
	  public:
		Modifier(ModifyContext context,
				  core::CommandManagerHandler commandManager):
			m_context(context),
			m_commandManager(commandManager)
		{
		}

		//TODO refactor
		void transform()
		{

		}

		MaterialModifier material()
		{
			//TODO refactor
			// Throw exception if node can't have material
			if(auto node = m_context.selectedNode.lock())
			{
				//Throw if node is not of type plane
				if(!std::dynamic_pointer_cast<geometry::Plane>(node))
				{
					throw std::runtime_error("Node is not of type Plane");
				}
			}


			MaterialModifier materialManager{m_context, m_commandManager};
			return materialManager;
		}
    
        private:
        ModifyContext m_context;
		 core::CommandManagerHandler m_commandManager;
	};

	class Application
	{
	  public:
		Application(int argc, char* argv[]) :
			m_app(argc, argv),
			m_commandManager(),
			m_contentManager(),
			m_guiManager(core::ContentManagerHandler(&m_contentManager)),
			m_creator(core::ContentManagerHandler(&m_contentManager), core::CommandManagerHandler(&m_commandManager))
		{
			spdlog::set_level(spdlog::level::debug);
			spdlog::info("Application::Application()");
			spdlog::info("Application::Application() - Done");
			spdlog::info("----------------------");
		}


		int initialize()
		{
			spdlog::info("Application::initialize()");
			m_contentManager.initialize();
			m_commandManager.initialize();
			m_guiManager.initialize();

			spdlog::info("Application::initialize() - Done");
			spdlog::info("----------------------");
			return 0;
		}


		int run()
		{	
			spdlog::info("Application::run()");	

			m_contentManager.onStart();
			
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


			Modifier modifier{context, core::CommandManagerHandler(&m_commandManager)};
			return modifier;
		}


	  private:
		QApplication m_app;

		//Communication withing the application
		core::CommandManager m_commandManager;


		// Content
		core::ContentManager m_contentManager;
		Creator m_creator;

		// Gui
		GuiManager m_guiManager;

		// GuiCreator m_guiCreator; To investigate future use
	};

} // namespace st

int main(int argc, char* argv[])
{
	Application app(argc, argv);
	app.initialize();

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
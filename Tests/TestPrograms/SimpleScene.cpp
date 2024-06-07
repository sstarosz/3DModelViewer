#include "Core/Scene.hpp"
#include "Core/Camera.hpp"
#include "Core/Transform.hpp"
#include "Core/ContentManager.hpp"

#include "Renderer/Renderer.hpp"
#include "Ui/Viewport.hpp"
#include "Ui/MainWindow.hpp"
#include "Renderer/Grid.hpp"
#include "Renderer/Plane.hpp"
#include "Renderer/StandardMaterial.hpp"
#include "Renderer/MaterialManager.hpp"
#include "Renderer/Mesh.hpp"

#include <QApplication>

using namespace st;


namespace st
{
    
    class GuiManager
    {
        public:
        GuiManager(core::ContentManager& contentManager): 
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
        Creator(core::ContentManager& contentManager)
            : m_contentManager(contentManager)
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

            //m_contentManager.add(camera);

			return std::weak_ptr<core::Node>{camera};
        }

        std::weak_ptr<core::Node2> plane(
            [[mayby_unused]]const float width = 1.0f,
            [[mayby_unused]]const float height = 1.0f)
        {
            std::shared_ptr<renderer::Plane2> plane = std::make_shared<renderer::Plane2>();
            //std::shared_ptr<renderer::Mesh> mesh = std::make_shared<renderer::Mesh>();
            

            m_contentManager.add(plane);
            plane->initialize(); //TODO clean up
            //m_contentManager.add(mesh);
            return std::weak_ptr<core::Node2>{plane};

        }

        std::weak_ptr<core::Node> standardMaterial()
        {
            return std::weak_ptr<core::Node>{};
        }

        std::weak_ptr<core::Node> renderer(std::weak_ptr<core::Node> camera)
        {
            
            return std::weak_ptr<core::Node>{};
        }


        private:
        core::ContentManager& m_contentManager;
    };



    

    class Transformer
    {
        public:


        Transformer& translate(Eigen::Vector3f translation)
        {   
            //core::Transform transform{node};
            //transform.setTranslation(translation);
            return *this;
        }

        Transformer& scale(Eigen::Vector3f scale)
        {
            //core::Transform transform{node};
            //transform.setScale(scale);
            return *this;
        }

    };

    class MaterialManager
    {
    public:
        void assign(std::weak_ptr<core::Node> material)
        {
            //renderer::MaterialManager materialManager;
            //materialManager.assignMaterialToMesh(material, node);
        }
    };

    class Modifier
    {
        public:

        std::weak_ptr<core::Transform> transform()
        {
            //m_transformer = Transformer(node);
            std::shared_ptr<core::Transform> transform = std::make_shared<core::Transform>();
            
            //m_contentManager.addToCurrentContext(transform, m_currentContext);
            return std::weak_ptr<core::Transform>{transform};
        }

        MaterialManager& material()
        {
            //renderer::MaterialManager materialManager;
            //materialManager.assignMaterialToMesh(material, node);
            return m_materialManager;
        }

        void setCurentContext(std::weak_ptr<core::Node> node)
        {
            m_currentContext = node;
        }

        private:
        core::ContentManager m_contentManager;
        MaterialManager m_materialManager;

        std::weak_ptr<core::Node> m_currentContext;
    };

    class ModificationContest
    {


    };

    
    class Application
    {
    public:
        Application(int argc, char* argv[])
            : m_app(argc, argv),
              m_contentManager(),
              m_guiManager(m_contentManager),
              m_creator(m_contentManager),
              m_modifier()
        {
        }

        int run()
        {
            //Show all created gui elements if any
            m_guiManager.initialize();
            m_guiManager.show();

            return m_app.exec();
        }

        Creator& create()
        {
            return m_creator;
        }

        Modifier& modify(std::weak_ptr<core::Node> node)
        {
            m_modifier.setCurentContext(node);
            return m_modifier;
        }

        private:
        QApplication m_app;

        core::ContentManager m_contentManager;
        GuiManager m_guiManager;

        Creator m_creator;
        //GuiCreator m_guiCreator; To investigate future use
        Modifier m_modifier;

    };
    
}





int main(int argc, char* argv[])
{
    Application app(argc, argv);

    //Add Camera
	//auto camera = app.create()
	//				  .camera(45.0f,
	//						  35.0f,
	//						  0.1f,
	//						  10000.0f);

	//TODO revert -app.modify(camera).transform();
        //.translate(Eigen::Vector3f{0.0f, 0.0f, 6.0f})
        //.scale(Eigen::Vector3f{1.0f, 1.0f, 1.0f})
        //.translate(Eigen::Vector3f{0.0f, 0.0f, -6.0f})
        //.translate(Eigen::Vector3f{0.0f, 0.0f, 6.0f});


	/*-----------------------------------------------------------------------------------------------*/
    /*--------------------------------------Add Scene Content----------------------------------------*/
    /*-----------------------------------------------------------------------------------------------*/
    //Add Plane to Scene
    auto plane = app.create().plane();


    /*-----------------------------------------------------------------------------------------------*/
    /*--------------------------------------Add 
    ---------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------*/
    auto standardMaterial = app.create().standardMaterial();

    //Assign Material to Plane
    //TODO revert - app.modify(plane).material().assign(standardMaterial);

    /*-----------------------------------------------------------------------------------------------*/
    /*-------------------------------------Setup renderer--------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------*/
    //TODO revert - auto renderer = app.create().renderer(camera);
    
    /*-----------------------------------------------------------------------------------------------*/
    /*-------------------------------------Setup Gui-------------------------------------------------*/
    /*-----------------------------------------------------------------------------------------------*/


    return app.run();
}
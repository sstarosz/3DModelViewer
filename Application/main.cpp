#include "Application/Application.hpp"

int main(int argc, char* argv[])
{
	st::application::Application app(argc, argv);
	app.initialize();

	return app.run();
}

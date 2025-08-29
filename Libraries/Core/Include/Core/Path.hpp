#ifndef ST_CORE_PATH_HPP
#define ST_CORE_PATH_HPP

#include <string_view>
#include <string>
#include <ranges>
#include <algorithm>
#include <print>


namespace st::core
{

    /**
     * @brief 
     * 
     */
	class Path
	{
	public:

        Path() = default;
        Path(const std::string& path) : m_path(path) {}

		// Path is a node when the path follow the pattern "World/nodes/nodeName"
		bool isNode() const
		{
            using namespace std::string_view_literals;

            // Check if the path starts with "/World/
            if (m_path.starts_with("/World/"))
            {
                auto parts = std::views::split(m_path, "/"sv);
                //std::print("Parts: {}\n", parts);
                // Skip the first part (World) and check the rest
                auto it = std::ranges::drop_view(parts, 2);

                for (const auto& part : it)
                {
                    //std::print("Part: {}\n", part);
                    // Check if each part is a valid name (alphanumeric)
                    if (!std::ranges::all_of(part, [](char c) { return std::isalnum(c);}))
                    {
                        return false;
                    }
                }

                return true; 
            }
            

			return false;
		}

		// Path is a attribute when the path follow the pattern "World/NodeA/NodeB.attributeName"
		bool isAttribute() const
		{
            using namespace std::string_view_literals;

            // Check if the path starts with "/World/"
            if (m_path.starts_with("/World/"))
            {
                // Split the path into parts
                auto parts = std::string_view{m_path} | std::views::split("/"sv);

                // Get the last part
                std::string_view lastPart;
                for (const auto& part : parts)
                {
                    lastPart = std::string_view(part.begin(), part.end());
                }

                // Check if the last part contains a dot (indicating an attribute)
                if(lastPart.find('.') != std::string_view::npos)
                {
                    return true;
                }  
            }

			return false;
		}


        std::string getPath() const
        {
            return m_path;
        }


        bool isStringValidPath(const std::string& path) const
        {
            return std::all_of(path.begin(), path.end(), [](char c) { return std::isalnum(c) || c == '/' || c == '.'; });
        }


        bool operator==(const Path& other) const
        {
            return m_path == other.m_path;
        }


		std::string m_path;
	};

}


namespace std
{
    template <>
    struct hash<st::core::Path>
    {
        size_t operator()(const st::core::Path& path) const noexcept
        {
            return std::hash<std::string>()(path.getPath());
        }
    };
}

#endif // ST_CORE_PATH_HPP
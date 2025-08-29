#include "Attribute.hpp"

#include "Node.hpp"
namespace st::core
{

    std::string Attribute::getName() const
    {
        return m_name;
    }

    bool Attribute::isReadable() const
    {
        return m_readable;
    }

    bool Attribute::isWritable() const
    {
        return m_writable;
    }

    /*----------------------*/
    /*-------Setters--------*/
    /*----------------------*/
    void Attribute::setName(const std::string& name)
    {
        m_name = name;
    }

    void Attribute::setReadable(bool value)
    {
        m_readable = value;
    }

    void Attribute::setWritable(bool value)
    {
        m_writable = value;
    }

	void Attribute::markParentDirty()
	{
        if (auto parentNode = m_parentNode.lock())
        {
            parentNode->markDirty();
        }
    }


} // namespace st::core
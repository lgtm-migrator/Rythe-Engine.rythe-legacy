#include <core/ecs/handles/entity.hpp>

#include <core/ecs/registry.hpp>

namespace legion::core::ecs
{
    entity::operator id_type () const noexcept
    {
        return (data && data->alive) ? data->id : invalid_id;
    }

    bool entity::valid() const noexcept
    {
        return data && data->alive;
    }

    entity_data* entity::operator->() noexcept
    {
        return data;
    }

    const entity_data* entity::operator->() const noexcept
    {
        return data;
    }

    void entity::set_parent(id_type parent)
    {
        if (data->parent)
            data->parent->children.erase(*this);

        if (parent)
        {
            Registry::entityData(parent).children.insert(*this);

            data->parent = entity{ &Registry::entityData(parent) };
        }
        else
            data->parent = entity{ nullptr };
    }

    void entity::set_parent(entity parent)
    {
        if (data->parent)
            data->parent->children.erase(*this);

        if (parent)
            parent->children.insert(*this);

        data->parent = parent;
    }

    entity entity::get_parent() const
    {
        return data->parent;
    }

    void entity::add_child(id_type child)
    {
        add_child(entity{ &Registry::entityData(child) });
    }

    void entity::add_child(entity child)
    {
        child.set_parent(*this);
    }

    void entity::remove_child(id_type child)
    {
        remove_child(entity{ &Registry::entityData(child) });
    }

    void entity::remove_child(entity child)
    {
        child.set_parent(world);
    }

    void entity::remove_children()
    {
        for (auto& child : data->children)
        {
            world->children.insert(child);
            child->parent = world;
        }

        data->children.clear();
    }

    void entity::destroy_children(bool recurse)
    {
        for (auto& child : data->children.reverse_range())
            Registry::destroyEntity(child, recurse);
    }

    entity_set& entity::children()
    {
        return data->children;
    }

    const entity_set& entity::children() const
    {
        return data->children;
    }

    entity entity::get_child(size_type index) const
    {
        return data->children.at(index);
    }

    entity_set::iterator entity::begin()
    {
        return data->children.begin();
    }

    entity_set::const_iterator entity::begin() const
    {
        return data->children.cbegin();
    }

    entity_set::const_iterator entity::cbegin() const
    {
        return data->children.cbegin();
    }

    entity_set::reverse_iterator entity::rbegin()
    {
        return data->children.rbegin();
    }

    entity_set::const_reverse_iterator entity::rbegin() const
    {
        return data->children.crbegin();
    }

    entity_set::const_reverse_iterator entity::crbegin() const
    {
        return data->children.crbegin();
    }

    entity_set::iterator entity::end()
    {
        return data->children.end();
    }

    entity_set::const_iterator entity::end() const
    {
        return data->children.cend();
    }

    entity_set::const_iterator entity::cend() const
    {
        return data->children.cend();
    }

    entity_set::reverse_iterator entity::rend()
    {
        return data->children.rend();
    }

    entity_set::const_reverse_iterator entity::rend() const
    {
        return data->children.crend();
    }

    entity_set::const_reverse_iterator entity::crend() const
    {
        return data->children.crend();
    }

    void entity::destroy(bool recurse)
    {
        Registry::destroyEntity(*this, recurse);
    }

}

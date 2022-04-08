#include "autogen_prototype_child_reverse_iterator.hpp"
#include "../../core/ecs/handles/entity.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::ecs::child_reverse_iterator>(const legion::core::ecs::child_reverse_iterator& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::ecs::child_reverse_iterator>();
        prot.typeName = "legion::core::ecs::child_reverse_iterator";
        return prot;
    }
}

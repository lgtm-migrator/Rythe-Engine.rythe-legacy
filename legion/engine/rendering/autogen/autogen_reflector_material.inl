#include "autogen_reflector_material.hpp"
#include "../../rendering/data/material.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::material>(legion::rendering::material& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::material>();
        refl.typeName = "legion::rendering::material";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::material>(const legion::rendering::material& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::material>();
        refl.typeName = "legion::rendering::material";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}

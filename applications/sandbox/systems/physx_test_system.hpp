#pragma once
#include <core/core.hpp>
#include <rendering/rendering.hpp>
#include <physics/physics_helpers.hpp>

namespace lgn = legion;

struct ShootPhysXBox : public lgn::app::input_action<ShootPhysXBox> {};
struct ShootPhysXSphere : public lgn::app::input_action<ShootPhysXSphere> {};
struct ShootFrictionAndForceCubes : public lgn::app::input_action<ShootFrictionAndForceCubes> {};

struct MoveForward : public lgn::app::input_action<MoveForward> {};
struct MoveBackward : public lgn::app::input_action<MoveBackward> {};

struct MoveLeft : public lgn::app::input_action<MoveLeft> {};
struct MoveRight: public lgn::app::input_action<MoveRight> {};

struct CharacterJump : public lgn::app::input_action<CharacterJump> {};

struct self_destruct_component
{
    float selfDestructTimer = 0.0f;
};

namespace legion::physics
{
    class PhysXTestSystem : public System<PhysXTestSystem>
    {
    public:

        virtual void setup();

        void update(legion::time::span deltaTime);

    private:

        //wide block, 1 normal cube on the center, 1 rotated default cube on top of it
        void setupCubeWorldTestScene();

        void setupBoxAndStackTestScene();

        void setupCharacterControllerTestScene();

        //------------------------ Rigidbody Shooter -------------------------------------------//

        void shootPhysXCubes(ShootPhysXBox& action);

        void shootPhysXSphere(ShootPhysXSphere& action);

        void shootFrictionTest(ShootFrictionAndForceCubes& action);

        void getCameraPositionAndDirection(math::vec3& cameraDirection, math::vec3& cameraPosition);


        //------------------------ Character Controls -------------------------------------------//

        void MoveCharacter(const math::vec3& displacement);

        void OnCharacterJump(CharacterJump& action);

        enum move_dir : size_type
        {
            forward, backward, left, right,max
        };

        std::array<bool, move_dir::max> moveBools;

        void onPressForward(MoveForward& action)
        {
            moveBools[move_dir::forward] = action.pressed();
        }

        void onPresBackward(MoveBackward& action)
        {
            moveBools[move_dir::backward] = action.pressed();
        }

        void onPressLeft(MoveLeft& action)
        {
            moveBools[move_dir::left] = action.pressed();
        }

        void onPressRight(MoveRight& action)
        {
            moveBools[move_dir::right] = action.pressed();
        }

        //-------------------------- Scene Setup Helpers ---------------------------------------//

        void initializeLitMaterial(rendering::material_handle& materialToInitialize, rendering::shader_handle& litShader,
            const fs::view& albedoFile, const fs::view& normalFile, const fs::view& roughnessFile);

        //entity,transform,and mesh
        ecs::entity createDefaultMeshEntity(math::vec3 position, rendering::model_handle cubeH,
            rendering::material_handle TextureH);

        ecs::entity createStaticColliderWall(math::vec3 position, rendering::material_handle TextureH, math::vec3 scale = math::vec3(1.0f),
            math::quat rot = math::quat(1,0,0,0));

        void suzzaneRainTick(legion::time::span deltaTime);

        void createCubeStack(const math::vec3& extents, size_t stackSize, const math::vec3& startPos,int stopAfterStack = -1);


        //--------------------------- Rendering Variables ---------------------------------------//

        legion::rendering::shader_handle litShader;

        //materials
        rendering::material_handle legionLogoMat;
        rendering::material_handle tileMat;
        rendering::material_handle concreteMat;

        rendering::material_handle directionalLightMH;
        
        //models
        rendering::model_handle cubeH;
        rendering::model_handle sphereH;
        rendering::model_handle directionalLightH;

        rendering::model_handle colaCanH;
        rendering::model_handle suzanneH;
        rendering::model_handle statueH;

        bool m_isRainingSuzanne = false;
        float m_currentInterval = 0.0f;

        float m_rainInterval = 1.5f;

        math::vec3 m_rainStartPos = math::vec3(10,12,-5);
        math::vec3 m_rainExtents = math::vec3(10,0,10);

        const char* m_defaultNonBouncy = "DefaultNonBouncy";

        ecs::entity inflatableBlock;
        ecs::entity inflatableSphere;
        
        ecs::entity m_characterControllerEnt;
    };
}

#define LEGION_ENTRY
//#if defined(NDEBUG)
#define LEGION_KEEP_CONSOLE
//#endif

#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

#include "module/examplemodule.hpp"

void LEGION_CCONV reportModules(legion::Engine* engine)
{
    using namespace legion;
    log::filter(log::severity_debug);
    engine->reportModule<app::ApplicationModule>();
    engine->reportModule<gfx::RenderingModule>();
    engine->reportModule<audio::AudioModule>();
    engine->reportModule<ExampleModule>();
}

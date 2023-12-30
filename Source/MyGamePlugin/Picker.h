#pragma once

#include <Urho3D/Plugins/PluginApplication.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>

namespace Urho3D
{

/// A custom component provided by the plugin.
class Picker : public LogicComponent
{
    URHO3D_OBJECT(Picker, LogicComponent);

public:
    Picker(Context* context)
        : LogicComponent(context)
    {
        SetUpdateEventMask(USE_UPDATE);
    }

    void DelayedStart() override;

    void Update(float timeStep) override;

    static void RegisterObject(Context* context);

private:
    bool animate_ = true;
};

} // namespace Urho3D

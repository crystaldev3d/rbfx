#pragma once

#include <Urho3D/Plugins/PluginApplication.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>

namespace Urho3D
{

/// A custom component provided by the plugin.
class AutoTransform : public LogicComponent
{
    URHO3D_OBJECT(AutoTransform, LogicComponent);

public:
    AutoTransform(Context* context)
        : LogicComponent(context)
    {
        SetUpdateEventMask(USE_UPDATE);
    }

    void DelayedStart() override;

    void Update(float timeStep) override;

    static void RegisterObject(Context* context);

private:

    bool animate_ = true;
    float speed = 1.0f;
    bool p = false;
    Vector3 startPos = Vector3::ZERO;
    Vector3 endPos = Vector3::ZERO;
    Node* start = nullptr;
    Node* end = nullptr;
    Node* target = nullptr;
};

}




#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>

#include "GamePlugin.h"
#include "AutoTransform.h"

void Urho3D::AutoTransform::DelayedStart()
{

    start = GetNode()->GetChild("start");
    end = GetNode()->GetChild("end");
    target = GetNode()->GetChild("target");

    if (!start || !end || !target)
        return;

    startPos = start->GetWorldPosition();
    endPos = end->GetWorldPosition();
}

void Urho3D::AutoTransform::Update(float timeStep)
{
    if (!target)
        return;

    if (p)
    {
        float distance = target->GetWorldPosition().DistanceToPoint(endPos);
        Vector3 direction = endPos - target->GetWorldPosition();
        direction.Normalize();

        if (distance > 1.0f)
        {
            target->Translate(direction * speed * timeStep, Urho3D::TS_WORLD);
        }
        else
        {
            p = false;
        }

    }
    else
    {

        float distance = target->GetWorldPosition().DistanceToPoint(startPos);
        Vector3 direction = startPos - target->GetWorldPosition();
        direction.Normalize();

        if (distance > 1.0f)
        {
            target->Translate(direction * speed * timeStep, Urho3D::TS_WORLD);
        }
        else
        {
            p = true;
        }
    }

    if (animate_)
        target->Rotate(Quaternion(10 * timeStep, 20 * timeStep, 30 * timeStep));
}

void Urho3D::AutoTransform::RegisterObject(Context* context)
{
    context->AddFactoryReflection<AutoTransform>(Category_MyGamePlugin);
    URHO3D_ATTRIBUTE("Animate", bool, animate_, true, AM_EDIT);
    URHO3D_ATTRIBUTE("Speed", float, speed, 1.0f, AM_EDIT);
}

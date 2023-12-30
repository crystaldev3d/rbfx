

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>

#include "GamePlugin.h"
#include "Picker.h"

void Urho3D::Picker::DelayedStart()
{

}

void Urho3D::Picker::Update(float timeStep)
{

}

void Urho3D::Picker::RegisterObject(Context* context)
{
    context->AddFactoryReflection<Picker>(Category_MyGamePlugin);
    URHO3D_ATTRIBUTE("Animate", bool, animate_, true, AM_EDIT);
}

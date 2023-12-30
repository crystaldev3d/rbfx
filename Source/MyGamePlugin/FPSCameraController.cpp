#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>

#include "GamePlugin.h"
#include "FPSCameraController.h"

Urho3D::FPSCameraController::FPSCameraController(Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(USE_UPDATE);
}

void Urho3D::FPSCameraController::Update(float timeStep)
{
    Input* input = context_->GetSubsystem<Input>();
    IntVector2 delta = input->GetMouseMove();

    auto yaw = GetNode()->GetRotation().EulerAngles().x_;
    if ((yaw > -90.f && yaw < 90.f) || (yaw <= -90.f && delta.y_ > 0) || (yaw >= 90.f && delta.y_ < 0))
        GetNode()->RotateAround(Vector3::ZERO, Quaternion(0.1f * delta.y_, Vector3::RIGHT), TS_LOCAL);
    GetNode()->RotateAround(GetNode()->GetPosition(), Quaternion(0.1f * delta.x_, Vector3::UP), TS_WORLD);
     
    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    if (input->GetKeyDown(KEY_W))
        GetNode()->Translate(Vector3::FORWARD * timeStep);
    if (input->GetKeyDown(KEY_S))
        GetNode()->Translate(Vector3::BACK * timeStep);
    if (input->GetKeyDown(KEY_A))
        GetNode()->Translate(Vector3::LEFT * timeStep);
    if (input->GetKeyDown(KEY_D))
        GetNode()->Translate(Vector3::RIGHT * timeStep);
}

void Urho3D::FPSCameraController::DelayedStart()
{

}

void Urho3D::FPSCameraController::RegisterObject(Context* context)
{
    context->AddFactoryReflection<FPSCameraController>(Category_MyGamePlugin);
    //context->AddReflection<FPSCameraController>(Category_MyGamePlugin);
}

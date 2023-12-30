//
// Copyright (c) 2017-2020 the rbfx project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "GamePlugin.h"
#include "RotateObject.h"
#include "PixelArtGenerator.h"

#include "AutoTransform.h"
#include "Picker.h"
#include "FPSCameraController.h"


#include <Urho3D/Input/FreeFlyController.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/TextureCube.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Math/RandomEngine.h>
#include <Urho3D/Resource/ResourceCache.h>

URHO3D_DEFINE_PLUGIN_MAIN(Urho3D::GamePlugin);

namespace Urho3D
{

GamePlugin::GamePlugin(Context* context)
    : PluginApplication(context)
{
    URHO3D_LOGDEBUG("GamePlugin::GamePlugin");
}

GamePlugin::~GamePlugin()
{
    URHO3D_LOGDEBUG("GamePlugin::~GamePlugin()");
}

void GamePlugin::Load()
{
    URHO3D_LOGDEBUG("GamePlugin::Load");

    RegisterObject<RotateObject>();
    RegisterObject<PixelArtGenerator>();
    RegisterObject<AutoTransform>();
    RegisterObject<Picker>();
    RegisterObject<FPSCameraController>();
}

void GamePlugin::Unload()
{
    URHO3D_LOGDEBUG("GamePlugin::Unload()");
    //context_->RemoveReflection<RotateObject>();

}

void GamePlugin::Start(bool isMain)
{
    URHO3D_LOGDEBUG("GamePlugin::Start");
}

void GamePlugin::Stop()
{
    URHO3D_LOGDEBUG("GamePlugin::Stop()");
}

}

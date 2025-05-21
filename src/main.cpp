#include <UrbanEngine.h>
#include <Core/EntryPoint.h>
#include "EditorApp.h"

UE::Application* UE::CreateApplication(){

    return new EditorApp();
}
#pragma once

#include <UrbanEngine.h>
#include "EditorLayer.h"

using namespace UE;
class EditorApp : public Application {
public:
    EditorApp()
        :Application("EbonyEditor", 1600, 900){
            PushLayer(new EditorLayer());
        }

    ~EditorApp(){}
};
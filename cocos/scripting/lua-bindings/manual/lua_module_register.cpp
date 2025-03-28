/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "scripting/lua-bindings/manual/lua_module_register.h"

#include "scripting/lua-bindings/manual/network/lua_cocos2dx_network_manual.h"
#include "scripting/lua-bindings/manual/cocostudio/lua_cocos2dx_coco_studio_manual.hpp"
#include "scripting/lua-bindings/manual/ui/lua_cocos2dx_ui_manual.hpp"
//#include "scripting/lua-bindings/manual/spine/lua_cocos2dx_spine_manual.hpp"
//#include "scripting/lua-bindings/manual/dragonBones/lua_dragonBones.hpp"
//#include "scripting/lua-bindings/manual/fairygui/lua_fairygui.hpp"
#include "scripting/lua-bindings/manual/audio/lua_audio_manual.h"
#include "scripting/lua-bindings/manual/extra/lua_extra_manual.h"

int lua_module_register(lua_State* L)
{
    // Don't change the module register order unless you know what your are doing
    register_extra_manual_all(L);
    register_audio_module(L);
    register_network_module(L);
    register_cocostudio_module(L);
    register_ui_module(L);
    //register_spine_module(L);
    //register_dragonBones_manual(L);
    //register_fairygui_manual(L);
    return 1;
}


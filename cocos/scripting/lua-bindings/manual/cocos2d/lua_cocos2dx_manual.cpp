/****************************************************************************
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2020-2022 cocos2d-lua.org

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
#include "scripting/lua-bindings/manual/cocos2d/lua_cocos2dx_manual.hpp"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "scripting/lua-bindings/manual/CCLuaValue.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#if defined(_MSC_VER) || defined(__MINGW32__)
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#endif

#include "2d/CCActionCamera.h"
#include "2d/CCActionCatmullRom.h"
#include "2d/CCActionGrid.h"
#include "2d/CCActionInterval.h"
#include "2d/CCAtlasNode.h"
#include "2d/CCCamera.h"
#include "2d/CCMotionStreak.h"
#include "2d/CCParticleBatchNode.h"
#include "2d/CCParticleSystem.h"
#include "2d/CCScene.h"
#include "2d/CCSpriteBatchNode.h"
#include "2d/CCTMXLayer.h"
#include "2d/CCTMXTiledMap.h"
#include "2d/CCRenderTexture.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventListenerMouse.h"
#include "base/CCEventListenerTouch.h"
#include "base/CCScheduler.h"
#include "base/CCUserDefault.h"
#include "base/ccUtils.h"
#include "platform/CCApplication.h"
#include "platform/CCDevice.h"
#include "platform/CCFileUtils.h"
#include "platform/CCGLView.h"
#include "renderer/CCTextureCache.h"
#include "renderer/ccShaders.h"

void LuaNode::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
    int handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)this,
        ScriptHandlerMgr::HandlerType::LUANODE_DRAW);
    if (0 != handler)
    {
        LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
        auto* L = stack->getLuaState();

        lua_newtable(L);
        for (int i = 0; i < 16; i++)
        {
            stack->pushFloat(transform.m[i]);
            lua_rawseti(L, -2, i + 1);
        }

        stack->pushFloat(_globalZOrder);
        stack->executeFunctionByHandler(handler, 2);
        stack->clean();
    }
}

static void tolua_reg_LuaNode_type(lua_State* tolua_S)
{
    tolua_usertype(tolua_S, "cc.LuaNode");
}

static int tolua_collect_LuaNode(lua_State* tolua_S)
{
    LuaNode* self = (LuaNode*)tolua_tousertype(tolua_S, 1, 0);
    Mtolua_delete(self);
    return 0;
}

#ifndef TOLUA_DISABLE_tolua_Cocos2d_LuaNode_create00
static int tolua_Cocos2d_LuaNode_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S, 1, "cc.LuaNode", 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 2, &tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        LuaNode* luaNode = new (std::nothrow) LuaNode();
        if (NULL != luaNode)
        {
            luaNode->autorelease();
            int nID = (int)luaNode->_ID;
            int* pLuaID = &luaNode->_luaID;
            toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)luaNode, "cc.LuaNode");
        }
        else
        {
            return 0;
        }
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror :
    tolua_error(tolua_S, "#ferror in function 'create'.", &tolua_err);
    return 0;
#endif
}
#endif

static int tolua_Cocos2d_LuaNode_registerScriptDrawHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.LuaNode", 0, &tolua_err) ||
        (tolua_isvaluenil(tolua_S, 2, &tolua_err) ||
            !toluafix_isfunction(tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err)) ||
        !tolua_isnoobj(tolua_S, 3, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        LuaNode* luaNode = (LuaNode*) tolua_tousertype(tolua_S, 1, 0);
        LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 2, 0));
        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)luaNode, handler, ScriptHandlerMgr::HandlerType::LUANODE_DRAW);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'registerScriptDrawHandler'.", &tolua_err);
        return 0;
#endif
}

static int tolua_Cocos2d_LuaNode_unregisterScriptDrawHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.LuaNode", 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        LuaNode* luaNode = (LuaNode*)tolua_tousertype(tolua_S, 1, 0);
        ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)luaNode, ScriptHandlerMgr::HandlerType::LUANODE_DRAW);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'unregisterScriptDrawhandler'.", &tolua_err);
    return 0;
#endif
}

TOLUA_API int tolua_luanode_open(lua_State* tolua_S)
{
    tolua_open(tolua_S);
    tolua_reg_LuaNode_type(tolua_S);
    tolua_module(tolua_S, "cc", 0);
    tolua_beginmodule(tolua_S, "cc");
      tolua_cclass(tolua_S, "LuaNode", "cc.LuaNode", "cc.Node", tolua_collect_LuaNode);
        tolua_beginmodule(tolua_S, "LuaNode");
            tolua_function(tolua_S, "create", tolua_Cocos2d_LuaNode_create00);
        tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
    return 1;
}

int register_luanode_manual(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    lua_pushstring(tolua_S, "cc.LuaNode");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S, -1))
    {
        lua_pushstring(tolua_S, "registerScriptDrawHandler");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_LuaNode_registerScriptDrawHandler00);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "unregisterScriptDrawHandler");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_LuaNode_unregisterScriptDrawHandler00);
        lua_rawset(tolua_S, -3);
    }
    lua_pop(tolua_S, 1);
    return 1;
}

static int tolua_cocos2d_Scheduler_scheduleScriptFunc(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    Scheduler* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"cc.Scheduler",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<cocos2d::Scheduler*>(tolua_tousertype(tolua_S,1,0));

#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_Scheduler_scheduleScriptFunc'\n", NULL);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (3 == argc) {
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
            !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
            !tolua_isboolean(tolua_S,4,0,&tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler =  toluafix_ref_function(tolua_S,2,0);
        float interval = (float)  tolua_tonumber(tolua_S,3,0);
        bool  paused   = (bool)  tolua_toboolean(tolua_S,4,0);
        unsigned int tolua_ret = (unsigned int)  self->scheduleScriptFunc(handler,interval,paused);
        tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.Scheduler:scheduleScriptFunc",  argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Scheduler_scheduleScriptFunc'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_Scheduler_unscheduleScriptEntry(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    Scheduler* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"cc.Scheduler",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<cocos2d::Scheduler*>(tolua_tousertype(tolua_S,1,0));

#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_Scheduler_unscheduleScriptEntry'\n", NULL);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (1 == argc) {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
        {
            goto tolua_lerror;
        }
#endif

        unsigned int scheduleScriptEntryID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
        self->unscheduleScriptEntry(scheduleScriptEntryID);
        return 0;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.Scheduler:unscheduleScriptEntry",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Scheduler_unscheduleScriptEntry'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_RenderTexture_newImage(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::RenderTexture* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.RenderTexture",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (cocos2d::RenderTexture*)tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_cocos2d_RenderTexture_newImage'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1 || argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        ok &= toluafix_isfunction(tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'tolua_cocos2d_RenderTexture_newImage'", nullptr);
            return 0;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S, 2, 0);
        auto callback = [=](cocos2d::Image* image){
            auto stack = LuaEngine::getInstance()->getLuaStack();
            stack->pushObject(image, "cc.Image");
            stack->executeFunctionByHandler(handler, 1);
        };
        
        if(argc == 2)
        {
            bool flipImage;
            ok &= luaval_to_boolean(tolua_S, 3,&flipImage, "cc.RenderTexture:newImage");
            cobj->newImage(callback, flipImage);
        }
        else
        {
            cobj->newImage(callback);
        }
        return 0;
    }
    
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.RenderTexture:newImage",argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_RenderTexture_newImage'.",&tolua_err);
#endif
    
    return 0;
}

int tolua_cocos2d_Sequence_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;

    tolua_Error tolua_err;

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S,1,"cc.Sequence",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;
    if(argc > 0)
    {
        Vector<FiniteTimeAction*> array;

        if (1 == argc && tolua_istable(tolua_S, 2, 0, &tolua_err))
        {
            luaval_to_ccvector(tolua_S, 2, &array, "cc.Sequence:create");
        }
        else
        {
            int i = 1;
            while (i <= argc)
            {
#if COCOS2D_DEBUG >= 1
                if (!tolua_isusertype(tolua_S, 1 + i, "cc.FiniteTimeAction", 0, &tolua_err))
                    goto tolua_lerror;
#endif

                cocos2d::FiniteTimeAction* item = static_cast<cocos2d::FiniteTimeAction*>(tolua_tousertype(tolua_S, 1 + i, nullptr));
                if (nullptr != item)
                {
                    array.pushBack(item);
                }
                ++i;
            }
        }

        cocos2d::Sequence* tolua_ret = cocos2d::Sequence::create(array);
        //issue 2433 uncheck
        int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : nullptr;
        toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.Sequence");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.Sequence:create", argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Sequence_create'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_CallFunc_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"cc.CallFunc",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1 || argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        if(!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err))
            goto tolua_lerror;
#endif

        LUA_FUNCTION handler =  toluafix_ref_function(tolua_S,2,0);

        bool hasExtraData = false;
        int  ref  = 0;
        if (argc == 2)
        {
#if COCOS2D_DEBUG >= 1
            if(!tolua_istable(tolua_S, 3, 0, &tolua_err))
                goto tolua_lerror;
#endif
            lua_pushvalue(tolua_S, 3);
            ref = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
            hasExtraData = true;
        }
        LuaCallFunc* tolua_ret = new (std::nothrow) LuaCallFunc();
        tolua_ret->initWithFunction([=](void* self,Node* target){
            int callbackHandler =  ScriptHandlerMgr::getInstance()->getObjectHandler((void*)tolua_ret, ScriptHandlerMgr::HandlerType::CALLFUNC);

            if (0 != callbackHandler)
            {
                LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
                int argNums = 1;
                if (nullptr != target)
                {
                    stack->pushObject(target, "cc.Node");
                }
                else
                {
                    stack->pushNil();
                }

                if (hasExtraData)
                {
                    lua_rawgeti(tolua_S, LUA_REGISTRYINDEX,ref);
                    if (lua_istable(tolua_S, -1))
                    {
                        argNums += 1;
                    }
                    else
                    {
                        lua_pop(tolua_S, 1);
                    }
                }
                stack->executeFunctionByHandler(callbackHandler, argNums);
                if (hasExtraData)
                {
                    luaL_unref(tolua_S, LUA_REGISTRYINDEX,ref);
                }
                stack->clean();
            }
        });
        tolua_ret->autorelease();
        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)tolua_ret, handler, ScriptHandlerMgr::HandlerType::CALLFUNC);

        int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.CallFunc");
        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.CallFunc:create", argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_CallFunc_create'.",&tolua_err);
    return 0;
#endif

}

static int tolua_cocos2d_Node_registerScriptHandler(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    Node* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_Node_registerScriptHandler'\n", NULL);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if(!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err))
            goto tolua_lerror;
#endif

        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,2,0);
        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType::NODE);

        return 0;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.Node:registerScriptHandler",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Node_registerScriptHandler'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_Node_unregisterScriptHandler(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    Node* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_Node_unregisterScriptHandler'\n", NULL);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)self, ScriptHandlerMgr::HandlerType::NODE);
        return 0;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.Node:unregisterScriptHandler", argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Node_unregisterScriptHandler'.",&tolua_err);
    return 0;
#endif
}


static int tolua_Cocos2d_Node_scheduleUpdateWithPriorityLua(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    Node* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_Cocos2d_Node_scheduleUpdateWithPriorityLua'\n", NULL);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        if(!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err))
            goto tolua_lerror;
#endif

        LUA_FUNCTION handler =  toluafix_ref_function(tolua_S,2,0);
        int priority = 0;
        if (luaval_to_int32(tolua_S, 3, &priority, "cc.Node:scheduleUpdateWithPriorityLua"))
        {
           self->scheduleUpdateWithPriorityLua(handler,priority);
        }
        return 0;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n","cc.Node:scheduleUpdateWithPriorityLua",  argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_Cocos2d_Node_scheduleUpdateWithPriorityLua'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_Node_unscheduleUpdate(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    Node* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_Node_unscheduleUpdate'\n", NULL);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (0 == argc)
    {
        self->unscheduleUpdate();
        return 0;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.Node:unscheduleUpdate", argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Node_unscheduleUpdate'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_Node_setContentSize(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::Node* cobj = NULL;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::Node*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Node_setContentSize'", NULL);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;

    if (1 == argc)
    {
        cocos2d::Size size;
        ok &= luaval_to_size(tolua_S, 2, &size, "cc.Node:setContentSize");
        if (!ok)
            return 0;

        cobj->setContentSize(size);
        lua_settop(tolua_S, 1);
        return 1;
    }
    else if(2 == argc)
    {
        double width;
        ok &= luaval_to_number(tolua_S, 2,&width, "cc.Node:setContentSize");

        if (!ok)
            return 0;

        double height;
        ok &= luaval_to_number(tolua_S, 3,&height, "cc.Node:setContentSize");

        if (!ok)
            return 0;

        cobj->setContentSize(Size(width, height));
        lua_settop(tolua_S, 1);
        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.Node:setContentSize",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Node_setContentSize'.",&tolua_err);
#endif
    return 0;
}

int tolua_cocos2d_Node_setAnchorPoint(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::Node* cobj = NULL;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::Node*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Node_setAnchorPoint'", NULL);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;

    if (1 == argc)
    {
        cocos2d::Vec2 pt;
        ok &= luaval_to_vec2(tolua_S, 2, &pt, "cc.Node:setAnchorPoint");
        if (!ok)
            return 0;

        cobj->setAnchorPoint(pt);
        lua_settop(tolua_S, 1);
        return 1;
    }
    else if(2 == argc)
    {
        double x;
        ok &= luaval_to_number(tolua_S, 2,&x, "cc.Node:setAnchorPoint");

        if (!ok)
            return 0;

        double y;
        ok &= luaval_to_number(tolua_S, 3,&y, "cc.Node:setAnchorPoint");

        if (!ok)
            return 0;

        cobj->setAnchorPoint(cocos2d::Vec2((float)x,(float)y));
        lua_settop(tolua_S, 1);
        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.Node:setAnchorPoint",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Node_setAnchorPoint'.",&tolua_err);
#endif
    return 0;
}

static int tolua_cocos2d_Node_getPosition(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    Node* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_Node_getPosition'\n", NULL);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc >= 0 && argc <= 2)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isnumber(tolua_S,2,1,&tolua_err) || !tolua_isnumber(tolua_S,3,1,&tolua_err) )
            goto tolua_lerror;
#endif
        float x = (float)  tolua_tonumber(tolua_S,2,0);
        float y = (float)  tolua_tonumber(tolua_S,3,0);

        self->getPosition(&x,&y);

        tolua_pushnumber(tolua_S,(lua_Number)x);
        tolua_pushnumber(tolua_S,(lua_Number)y);

        return 2;
    }

    luaL_error(tolua_S, "%s function in Node has wrong number of arguments: %d, was expecting %d\n", "cc.Node:getPosition",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Node_getPosition'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_Node_enumerateChildren(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::Node* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::Node*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Node_enumerateChildren'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
            !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err))
        {
            goto tolua_lerror;
        }
#endif

        std::string name = (std::string)tolua_tocppstring(tolua_S,2,0);
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,3,0);

        cobj->enumerateChildren(name, [=](Node* node)->bool{
            int id = node ? (int)node->_ID : -1;
            int* luaID = node ? &node->_luaID : nullptr;
            toluafix_pushusertype_ccobject(tolua_S, id, luaID, (void*)node,"cc.Node");
            bool ret = LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 1);

            return ret;
        });
        LuaEngine::getInstance()->removeScriptHandler(handler);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "enumerateChildren",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Node_enumerateChildren'.",&tolua_err);
#endif

    return 0;
}

int lua_cocos2dx_Node_setAdditionalTransform(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::Node* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::Node*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Node_setAdditionalTransform'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            cocos2d::AffineTransform arg0;
            ok &= luaval_to_affinetransform(tolua_S, 2, &arg0, "cc.Node:setAdditionalTransform");

            if (!ok) { break; }
            cobj->setAdditionalTransform(arg0);
            lua_settop(tolua_S, 1);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 1) {
            cocos2d::Mat4 arg0;
            ok &= luaval_to_mat4(tolua_S, 2, &arg0, "cc.Node:setAdditionalTransform");

            if (!ok) { break; }
            cobj->setAdditionalTransform(&arg0);
            lua_settop(tolua_S, 1);
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "cc.Node:setAdditionalTransform",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Node_setAdditionalTransform'.",&tolua_err);
#endif

    return 0;
}

int lua_cocos2dx_Node_setRotationQuat(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::Node* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::Node*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Node_setRotationQuat'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            cocos2d::Quaternion arg0;
            ok &= luaval_to_quaternion(tolua_S, 2, &arg0, "cc.Node:setRotationQuat");

            if (!ok) { break; }
            cobj->setRotationQuat(arg0);
            lua_settop(tolua_S, 1);
            return 1;
        }
    }while(0);

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "cc.Node:setRotationQuat",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Node_setRotationQuat'.",&tolua_err);
#endif

    return 0;
}

static int tolua_cocos2d_Spawn_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;

    tolua_Error tolua_err;
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.Spawn",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc > 0)
    {
        Vector<FiniteTimeAction*> array;
        int i = 1;

        if (1 == argc && tolua_istable(tolua_S, 2, 0, &tolua_err))
        {
            luaval_to_ccvector(tolua_S, 2, &array, "cc.Spawn:create");
        }
        else
        {
            while (i <= argc)
            {
#if COCOS2D_DEBUG >= 1
                if (!tolua_isusertype(tolua_S, 1 + i, "cc.FiniteTimeAction", 0, &tolua_err))
                    goto tolua_lerror;
#endif

                cocos2d::FiniteTimeAction* item = static_cast<cocos2d::FiniteTimeAction*>(tolua_tousertype(tolua_S, 1 + i, NULL));
                if (NULL != item)
                {
                    array.pushBack(item);
                    ++i;
                }
            }
        }

        cocos2d::Spawn * tolua_ret = cocos2d::Spawn::create(array);
        int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.Spawn");
        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.Spawn:create", argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Spawn_create'.",&tolua_err);
    return 0;
#endif
}

int lua_cocos2d_CardinalSplineBy_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    bool ok = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.CardinalSplineBy",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 3)
    {
        double dur = 0.0;
        ok &= luaval_to_number(tolua_S, 2, &dur, "cc.CardinalSplineBy:create");
        if (!ok)
            return 0;

        int num = 0;
        cocos2d::Vec2 *arr = NULL;
        ok &= luaval_to_array_of_vec2(tolua_S, 3, &arr, &num, "cc.CardinalSplineBy:create");
        if (!ok)
            return 0;

        double ten = 0.0;
        ok &= luaval_to_number(tolua_S, 4, &ten, "cc.CardinalSplineBy:create");
        if (!ok)
        {
            CC_SAFE_DELETE_ARRAY(arr);
            return 0;
        }

        if (num > 0)
        {
            PointArray* points = PointArray::create(num);

            if (NULL == points)
            {
                CC_SAFE_DELETE_ARRAY(arr);
                return 0;
            }

            for( int i = 0; i < num; i++) {
                points->addControlPoint(arr[i]);
            }

            CC_SAFE_DELETE_ARRAY(arr);
            CardinalSplineBy* tolua_ret = CardinalSplineBy::create((float)dur, points, (float)ten);
            if (NULL != tolua_ret)
            {
                int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
                int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
                toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.CardinalSplineBy");
                return 1;
            }
        }
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.CardinalSplineBy:create", argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2d_CardinalSplineBy_create'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_CatmullRomBy_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    bool ok = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.CatmullRomBy",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        double dur = 0.0;
        ok &= luaval_to_number(tolua_S, 2, &dur, "cc.CatmullRomBy:create");
        if (!ok)
            return 0;

        int num = 0;
        cocos2d::Vec2 *arr = NULL;
        ok &= luaval_to_array_of_vec2(tolua_S, 3, &arr, &num, "cc.CatmullRomBy:create");
        if (!ok)
            return 0;

        if (num > 0)
        {
            PointArray* points = PointArray::create(num);

            if (NULL == points)
            {
                CC_SAFE_DELETE_ARRAY(arr);
                return 0;
            }

            for( int i = 0; i < num; i++) {
                points->addControlPoint(arr[i]);
            }

            CC_SAFE_DELETE_ARRAY(arr);
            CatmullRomBy* tolua_ret = CatmullRomBy::create((float)dur, points);
            if (NULL != tolua_ret)
            {
                int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
                int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
                toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.CatmullRomBy");
                return 1;
            }
        }
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.CatmullRomBy:create", argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_CatmullRomBy_create'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_CatmullRomTo_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    bool ok = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.CatmullRomTo",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        double dur = 0.0;
        ok &= luaval_to_number(tolua_S, 2, &dur, "cc.CatmullRomTo:create");
        if (!ok)
            return 0;

        int num = 0;
        cocos2d::Vec2 *arr = NULL;
        ok &= luaval_to_array_of_vec2(tolua_S, 3, &arr, &num, "cc.CatmullRomTo:create");
        if (!ok)
            return 0;

        if (num > 0)
        {
            PointArray* points = PointArray::create(num);

            if (NULL == points)
            {
                CC_SAFE_DELETE_ARRAY(arr);
                return 0;
            }

            for( int i = 0; i < num; i++) {
                points->addControlPoint(arr[i]);
            }

            CC_SAFE_DELETE_ARRAY(arr);
            CatmullRomTo* tolua_ret = CatmullRomTo::create((float)dur, points);
            if (NULL != tolua_ret)
            {
                int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
                int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
                toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.CatmullRomTo");
                return 1;
            }
        }
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.CatmullRomTo:create", argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_CatmullRomTo_create'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_BezierBy_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    bool ok = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.BezierBy",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        double t = 0.0;
        ok &= luaval_to_number(tolua_S, 2, &t, "cc.BezierBy:create");
        if (!ok)
            return 0;

        int num = 0;
        cocos2d::Vec2 *arr = NULL;
        ok &= luaval_to_array_of_vec2(tolua_S, 3, &arr, &num, "cc.BezierBy:create");
        if (!ok)
            return 0;

        if (num < 3)
        {
            CC_SAFE_DELETE_ARRAY(arr);
            return 0;
        }

        ccBezierConfig config;
        config.controlPoint_1 = arr[0];
        config.controlPoint_2 = arr[1];
        config.endPosition = arr[2];
        CC_SAFE_DELETE_ARRAY(arr);

        BezierBy* tolua_ret = BezierBy::create((float)t, config);
        if (NULL != tolua_ret)
        {
            int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
            int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
            toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.BezierBy");
            return 1;
        }
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.BezierBy:create",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_BezierBy_create'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_BezierTo_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    bool ok = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.BezierTo",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        double t = 0.0;
        ok &= luaval_to_number(tolua_S, 2, &t, "cc.BezierTo:create");
        if (!ok)
            return 0;

        int num = 0;
        cocos2d::Vec2 *arr = NULL;
        ok &= luaval_to_array_of_vec2(tolua_S, 3, &arr, &num, "cc.BezierTo:create");
        if (!ok)
            return 0;

        if (num < 3)
        {
            CC_SAFE_DELETE_ARRAY(arr);
            return 0;
        }

        ccBezierConfig config;
        config.controlPoint_1 = arr[0];
        config.controlPoint_2 = arr[1];
        config.endPosition = arr[2];
        CC_SAFE_DELETE_ARRAY(arr);

        BezierTo* tolua_ret = BezierTo::create((float)t, config);
        if (NULL != tolua_ret)
        {
            int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
            int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
            toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.BezierTo");
            return 1;
        }
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.BezierTo:create", argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_BezierTo_create'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2dx_DrawNode_drawPolygon(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    DrawNode* self = nullptr;

    tolua_Error tolua_err;
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.DrawNode",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<cocos2d::DrawNode*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_DrawNode_drawPolygon'\n", NULL);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (5 == argc)
    {
#if COCOS2D_DEBUG >= 1
        if(
        !tolua_istable(tolua_S, 2, 0, &tolua_err)  ||
        !tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
        !tolua_istable(tolua_S, 4, 0,&tolua_err)   ||
        !tolua_isnumber(tolua_S, 5, 0, &tolua_err) ||
        !tolua_istable(tolua_S,6, 0,&tolua_err) )
        {
            goto tolua_lerror;
        }
#endif
        int size = lua_tointeger(tolua_S, 3);
        if ( size > 0 )
        {
            cocos2d::Vec2* points = new (std::nothrow) cocos2d::Vec2[size];
            if (NULL == points)
                return 0;

            for (int i = 0; i < size; i++)
            {
                lua_pushnumber(tolua_S,i + 1);
                lua_gettable(tolua_S,2);
                if (!tolua_istable(tolua_S,-1, 0, &tolua_err))
                {
                    CC_SAFE_DELETE_ARRAY(points);
#if COCOS2D_DEBUG >= 1
                    goto tolua_lerror;
#endif
                }

                if(!luaval_to_vec2(tolua_S, lua_gettop(tolua_S), &points[i], "cc.DrawNode:drawPolygon"))
                {
                    lua_pop(tolua_S, 1);
                    CC_SAFE_DELETE_ARRAY(points);
                    return 0;
                }
                lua_pop(tolua_S, 1);
            }

            Color4F fillColor;
            if (!luaval_to_color4f(tolua_S, 4, &fillColor, "cc.DrawNode:drawPolygon"))
            {
                CC_SAFE_DELETE_ARRAY(points);
                return 0;
            }

            float borderWidth  = (float)tolua_tonumber(tolua_S, 5, 0);

            Color4F borderColor;
            if (!luaval_to_color4f(tolua_S, 6, &borderColor, "cc.DrawNode:drawPolygon"))
            {
                CC_SAFE_DELETE_ARRAY(points);
                return 0;
            }

            self->drawPolygon(points, (int)size, fillColor, borderWidth, borderColor);
            CC_SAFE_DELETE_ARRAY(points);
            return 0;
        }
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.DrawNode:drawPolygon", argc, 5);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_DrawNode_drawPolygon'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2dx_DrawNode_drawSolidPoly(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::DrawNode* self = nullptr;
    bool ok  = true;

    tolua_Error tolua_err;

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.DrawNode",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::DrawNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_DrawNode_drawSolidPoly'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3)
    {
        unsigned int size;
        luaval_to_uint32(tolua_S, 3, &size, "cc.DrawNode:drawSolidPoly");
        if ( size > 0 )
        {
            cocos2d::Vec2* points = new (std::nothrow) cocos2d::Vec2[size];
            if (NULL == points)
                return 0;

            for (unsigned int i = 0; i < size; i++)
            {
                lua_pushnumber(tolua_S,i + 1);
                lua_gettable(tolua_S,2);
                if (!tolua_istable(tolua_S,-1, 0, &tolua_err))
                {
                    CC_SAFE_DELETE_ARRAY(points);
#if COCOS2D_DEBUG >= 1
                    goto tolua_lerror;
#endif
                }

                if(!luaval_to_vec2(tolua_S, lua_gettop(tolua_S), &points[i], "cc.DrawNode:drawSolidPoly"))
                {
                    lua_pop(tolua_S, 1);
                    CC_SAFE_DELETE_ARRAY(points);
                    return 0;
                }
                lua_pop(tolua_S, 1);
            }

            cocos2d::Color4F arg2;

            ok &=luaval_to_color4f(tolua_S, 4, &arg2, "cc.DrawNode:drawSolidPoly");
            if(!ok)
                return 0;
            self->drawSolidPoly(points, size, arg2);
            CC_SAFE_DELETE_ARRAY(points);
            return 0;
        }
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.DrawNode:drawSolidPoly",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_DrawNode_drawSolidPoly'.",&tolua_err);
#endif

    return 0;
}

int tolua_cocos2dx_DrawNode_drawPoly(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    DrawNode* self = nullptr;
    bool ok  = true;

    tolua_Error tolua_err;

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.DrawNode",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<cocos2d::DrawNode*>(tolua_tousertype(tolua_S,1,0));

#if COCOS2D_DEBUG >= 1
    if (!self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_DrawNode_drawPoly'", NULL);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4)
    {
        unsigned int size;
        luaval_to_uint32(tolua_S, 3, &size, "cc.DrawNode:drawPoly");
        if ( size > 0 )
        {
            cocos2d::Vec2* points = new (std::nothrow) cocos2d::Vec2[size];
            if (NULL == points)
                return 0;

            for (unsigned int i = 0; i < size; i++)
            {
                lua_pushnumber(tolua_S,i + 1);
                lua_gettable(tolua_S,2);
                if (!tolua_istable(tolua_S,-1, 0, &tolua_err))
                {
                    CC_SAFE_DELETE_ARRAY(points);
#if COCOS2D_DEBUG >= 1
                    goto tolua_lerror;
#endif
                }

                if(!luaval_to_vec2(tolua_S, lua_gettop(tolua_S), &points[i], "cc.DrawNode:drawPoly"))
                {
                    lua_pop(tolua_S, 1);
                    CC_SAFE_DELETE_ARRAY(points);
                    return 0;
                }
                lua_pop(tolua_S, 1);
            }

            bool arg2;
            cocos2d::Color4F arg3;

            ok &= luaval_to_boolean(tolua_S, 4,&arg2, "cc.DrawNode:drawPoly");

            ok &= luaval_to_color4f(tolua_S, 5, &arg3, "cc.DrawNode:drawPoly");
            if(!ok)
                return 0;

            self->drawPoly(points, size, arg2, arg3);
            CC_SAFE_DELETE_ARRAY(points);
            return 0;
        }
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.DrawNode:drawPoly",argc, 4);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_DrawNode_drawPoly'.",&tolua_err);
#endif

    return 0;
}

int tolua_cocos2dx_DrawNode_drawCardinalSpline(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::DrawNode* self = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.DrawNode",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::DrawNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_DrawNode_drawCardinalSpline'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4)
    {
        int num = 0;
        cocos2d::Vec2 *arr = NULL;
        if (!luaval_to_array_of_vec2(tolua_S, 2, &arr, &num, "cc.DrawNode:drawCardinalSpline"))
            return 0;
        PointArray* config = PointArray::create(num);
        if (NULL == config)
        {
            CC_SAFE_DELETE_ARRAY(arr);
            return 0;
        }

        for( int i = 0; i < num; i++) {
            config->addControlPoint(arr[i]);
        }
        CC_SAFE_DELETE_ARRAY(arr);

        double arg1;
        unsigned int arg2;
        cocos2d::Color4F arg3;

        ok &= luaval_to_number(tolua_S, 3,&arg1, "cc.DrawNode:drawCardinalSpline");

        ok &= luaval_to_uint32(tolua_S, 4,&arg2, "cc.DrawNode:drawCardinalSpline");

        ok &= luaval_to_color4f(tolua_S, 5, &arg3, "cc.DrawNode:drawCardinalSpline");
        if(!ok)
            return 0;
        self->drawCardinalSpline(config, (float)arg1, arg2, arg3);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.DrawNode:drawCardinalSpline",argc, 4);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_DrawNode_drawCardinalSpline'.",&tolua_err);
#endif

    return 0;
}

int tolua_cocos2dx_DrawNode_drawCatmullRom(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::DrawNode* self = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.DrawNode",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::DrawNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_DrawNode_drawCatmullRom'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3)
    {
        int num = 0;
        cocos2d::Vec2 *arr = NULL;
        if (!luaval_to_array_of_vec2(tolua_S, 2, &arr, &num, "cc.DrawNode:drawCatmullRom"))
            return 0;
        PointArray* config = PointArray::create(num);
        if (NULL == config)
        {
            CC_SAFE_DELETE_ARRAY(arr);
            return 0;
        }

        for( int i = 0; i < num; i++) {
            config->addControlPoint(arr[i]);
        }
        CC_SAFE_DELETE_ARRAY(arr);

        unsigned int arg1;
        cocos2d::Color4F arg2;

        ok &= luaval_to_uint32(tolua_S, 3,&arg1, "cc.DrawNode:drawCatmullRom");

        ok &=luaval_to_color4f(tolua_S, 4, &arg2, "cc.DrawNode:drawCatmullRom");
        if(!ok)
            return 0;
        self->drawCatmullRom(config, arg1, arg2);
        return 0;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.DrawNode:drawCatmullRom",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_DrawNode_drawCatmullRom'.",&tolua_err);
#endif

    return 0;
}

int tolua_cocos2dx_DrawNode_drawPoints(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::DrawNode* self = nullptr;
    bool ok  = true;

    tolua_Error tolua_err;

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.DrawNode",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::DrawNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_DrawNode_drawPoints'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3)
    {
        unsigned int size;
        luaval_to_uint32(tolua_S, 3, &size, "cc.DrawNode:drawPoints");
        if ( size > 0 )
        {
            cocos2d::Vec2* points = new (std::nothrow) cocos2d::Vec2[size];
            if (NULL == points)
                return 0;

            for (unsigned int i = 0; i < size; i++)
            {
                lua_pushnumber(tolua_S,i + 1);
                lua_gettable(tolua_S,2);
                if (!tolua_istable(tolua_S,-1, 0, &tolua_err))
                {
                    CC_SAFE_DELETE_ARRAY(points);
#if COCOS2D_DEBUG >= 1
                    goto tolua_lerror;
#endif
                }

                if(!luaval_to_vec2(tolua_S, lua_gettop(tolua_S), &points[i], "cc.DrawNode:drawPoints"))
                {
                    lua_pop(tolua_S, 1);
                    CC_SAFE_DELETE_ARRAY(points);
                    return 0;
                }
                lua_pop(tolua_S, 1);
            }

            cocos2d::Color4F arg2;

            ok &=luaval_to_color4f(tolua_S, 4, &arg2, "cc.DrawNode:drawPoints");
            if(!ok)
                return 0;
            self->drawPoints(points, size, arg2);
            return 0;
        }
    }
    else if (argc == 4)
    {
        unsigned int size;
        luaval_to_uint32(tolua_S, 3, &size, "cc.DrawNode:drawPoints");
        if ( size > 0 )
        {
            cocos2d::Vec2* points = new (std::nothrow) cocos2d::Vec2[size];
            if (nullptr == points)
                return 0;

            for (unsigned int i = 0; i < size; i++)
            {
                lua_pushnumber(tolua_S,i + 1);
                lua_gettable(tolua_S,2);
                if (!tolua_istable(tolua_S,-1, 0, &tolua_err))
                {
                    CC_SAFE_DELETE_ARRAY(points);
#if COCOS2D_DEBUG >= 1
                    goto tolua_lerror;
#endif
                }

                if(!luaval_to_vec2(tolua_S, lua_gettop(tolua_S), &points[i], "cc.DrawNode:drawPoints"))
                {
                    lua_pop(tolua_S, 1);
                    CC_SAFE_DELETE_ARRAY(points);
                    return 0;
                }
                lua_pop(tolua_S, 1);
            }

            float pointSize = (float)tolua_tonumber(tolua_S, 4, 0);
            cocos2d::Color4F color;
            ok &=luaval_to_color4f(tolua_S, 5, &color, "cc.DrawNode:drawPoints");
            if(!ok)
                return 0;
            self->drawPoints(points, size, pointSize, color);
            return 0;
        }
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.DrawNode:drawPoints",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_DrawNode_drawPoints'.",&tolua_err);
#endif

    return 0;
}

static int tolua_cocos2dx_FileUtils_getStringFromFile(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
    FileUtils* self = nullptr;
    bool ok = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.FileUtils",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<FileUtils *>(tolua_tousertype(tolua_S,1,0));

#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_FileUtils_getStringFromFile'\n", nullptr);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (1 == argc)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2, &arg0, "cc.FileUtils:getStringFromFile");
        if (ok)
        {
            std::string contentsOfFile = FileUtils::getInstance()->getStringFromFile(arg0);
            tolua_pushstring(tolua_S, contentsOfFile.c_str());
            return 1;
        }
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.FileUtils:getStringFromFile", argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_FileUtils_getStringFromFile'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2dx_FileUtils_getDataFromFile(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;
	FileUtils* self = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "cc.FileUtils", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<FileUtils*>(tolua_tousertype(tolua_S, 1, 0));

#if COCOS2D_DEBUG >= 1
	if (nullptr == self)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'tolua_cocos2dx_FileUtils_getDataFromFile'\n", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (1 == argc)
	{
		std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2, &arg0, "cc.FileUtils:getDataFromFile");
		if (ok)
		{
			auto data = FileUtils::getInstance()->getDataFromFile(arg0);
			if (!data.isNull())
				lua_pushlstring(tolua_S, reinterpret_cast<const char*>(data.getBytes()), static_cast<size_t>(data.getSize()));
			else
				lua_pushnil(tolua_S);
			return 1;
		}
	} else if (2 == argc) {
		std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2, &arg0, "cc.FileUtils:getDataFromFile");
#if COCOS2D_DEBUG >= 1
        ok &= toluafix_isfunction(tolua_S, 3, "LUA_FUNCTION", 0, &tolua_err);
#endif
        if (ok)
        {
            int arg1 = toluafix_ref_function(tolua_S, 3, 0);
            ScriptHandlerMgr::HandlerType handlerType = ScriptHandlerMgr::getInstance()->addCustomHandler(self, arg1);
            FileUtils::getInstance()->getDataFromFile(arg0, [=](Data data) {
                LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
                if (!data.isNull())
                {
                    stack->pushString(reinterpret_cast<const char*>(data.getBytes()), static_cast<size_t>(data.getSize()));
                } else {
                    stack->pushNil();
                }
                stack->executeFunctionByHandler(arg1, 1);
                ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)self, handlerType);
                });
            return 1;
        }
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.FileUtils:getDataFromFile", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_cocos2dx_FileUtils_getDataFromFile'.", &tolua_err);
	return 0;
#endif
}

static int tolua_cocos2dx_UserDefault_getInstance(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.UserDefault",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if(0 == argc)
    {
        UserDefault* tolua_ret = (UserDefault*)  UserDefault::getInstance();
        tolua_pushusertype(tolua_S,(void*)tolua_ret,"cc.UserDefault");
        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.UserDefault:getInstance",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_UserDefault_getInstance'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2dx_Texture2D_setTexParameters(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
    Texture2D* self = nullptr;
    backend::SamplerFilter      arg1;
    backend::SamplerFilter      arg2;
    backend::SamplerAddressMode arg3;
    backend::SamplerAddressMode arg4;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.Texture2D",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (Texture2D*)  tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_Texture2D_setTexParameters'\n", nullptr);
		return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (4 == argc)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
            !tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
            !tolua_isnumber(tolua_S, 4, 0, &tolua_err) ||
            !tolua_isnumber(tolua_S, 5, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif

        arg1 = (backend::SamplerFilter)(int)tolua_tonumber(tolua_S, 2, 0);
        arg2 = (backend::SamplerFilter)(int)tolua_tonumber(tolua_S, 3, 0);
        arg3 = (backend::SamplerAddressMode)(int)tolua_tonumber(tolua_S, 4, 0);
        arg4 = (backend::SamplerAddressMode)(int)tolua_tonumber(tolua_S, 5, 0);

        Texture2D::TexParams param(arg1, arg2, arg3, arg4);

        self->setTexParameters(param);

        return 0;
    }

    luaL_error(tolua_S, "'setTexParameters' function of Texture2D wrong number of arguments: %d, was expecting %d\n", argc,4);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setTexParameters'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2dx_SpriteBatchNode_getDescendants(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    cocos2d::SpriteBatchNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.SpriteBatchNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::SpriteBatchNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_cocos2dx_SpriteBatchNode_getDescendants'", NULL);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
            return 0;
        std::vector<Sprite*> ret = cobj->getDescendants();

        lua_newtable(tolua_S);

        if (ret.empty())
            return 1;

        auto iter = ret.begin();
        int  indexTable = 1;
        for (; iter != ret.end(); ++iter)
        {
            if (nullptr == *iter)
                continue;

            lua_pushnumber(tolua_S, (lua_Number)indexTable);
            toluafix_pushusertype_ccobject(tolua_S, (*iter)->_ID, &((*iter)->_luaID), (void*)(*iter),"cc.Sprite");
            lua_rawset(tolua_S, -3);
            (*iter)->retain();
            ++indexTable;
        }

        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.SpriteBatchNode:getDescendants",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_SpriteBatchNode_getDescendants'.",&tolua_err);
#endif
    return 0;
}

static void extendScene(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.Scene");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
    }
    lua_pop(tolua_S, 1);
}

static void extendTexture2D(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.Texture2D");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"setTexParameters");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_Texture2D_setTexParameters );
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendNode(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.Node");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"registerScriptHandler");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Node_registerScriptHandler);
        lua_rawset(tolua_S,-3);
        lua_pushstring(tolua_S,"unregisterScriptHandler");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Node_unregisterScriptHandler);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S,"scheduleUpdateWithPriorityLua");
        lua_pushcfunction(tolua_S,tolua_Cocos2d_Node_scheduleUpdateWithPriorityLua);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S,"unscheduleUpdate");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Node_unscheduleUpdate);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S,"getPosition");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Node_getPosition);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "setContentSize");
        lua_pushcfunction(tolua_S, tolua_cocos2d_Node_setContentSize);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "setAnchorPoint");
        lua_pushcfunction(tolua_S, tolua_cocos2d_Node_setAnchorPoint);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "enumerateChildren");
        lua_pushcfunction(tolua_S, lua_cocos2dx_Node_enumerateChildren);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "setAdditionalTransform");
        lua_pushcfunction(tolua_S, lua_cocos2dx_Node_setAdditionalTransform);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "setRotationQuat");
        lua_pushcfunction(tolua_S, lua_cocos2dx_Node_setRotationQuat);
        lua_rawset(tolua_S, -3);
    }
    lua_pop(tolua_S, 1);
}

static void extendScheduler(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.Scheduler");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"scheduleScriptFunc");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Scheduler_scheduleScriptFunc);
        lua_rawset(tolua_S,-3);
        lua_pushstring(tolua_S, "unscheduleScriptEntry");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Scheduler_unscheduleScriptEntry);
        lua_rawset(tolua_S, -3);
    }
    lua_pop(tolua_S, 1);
}

static void extendRenderTexture(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.RenderTexture");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"newImage");
        lua_pushcfunction(tolua_S,tolua_cocos2d_RenderTexture_newImage);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}


static void extendSequence(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.Sequence");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Sequence_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendCallFunc(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.CallFunc");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_CallFunc_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendSpawn(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.Spawn");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Spawn_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendCardinalSplineBy(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.CardinalSplineBy");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,lua_cocos2d_CardinalSplineBy_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendCatmullRomBy(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.CatmullRomBy");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_CatmullRomBy_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendCatmullRomTo(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.CatmullRomTo");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_CatmullRomTo_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendBezierBy(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.BezierBy");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_BezierBy_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendBezierTo(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.BezierTo");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_BezierTo_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendDrawNode(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.DrawNode");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"drawPolygon");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_DrawNode_drawPolygon);
        lua_rawset(tolua_S,-3);

        lua_pushstring(tolua_S,"drawSolidPoly");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_DrawNode_drawSolidPoly);
        lua_rawset(tolua_S,-3);

        lua_pushstring(tolua_S,"drawPoly");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_DrawNode_drawPoly);
        lua_rawset(tolua_S,-3);

        lua_pushstring(tolua_S,"drawCardinalSpline");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_DrawNode_drawCardinalSpline);
        lua_rawset(tolua_S,-3);

        lua_pushstring(tolua_S,"drawCatmullRom");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_DrawNode_drawCatmullRom);
        lua_rawset(tolua_S,-3);

        lua_pushstring(tolua_S,"drawPoints");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_DrawNode_drawPoints);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

int lua_cocos2dx_Sprite_initWithPolygon(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::Sprite* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Sprite",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::Sprite*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Sprite_initWithPolygon'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        cocos2d::PolygonInfo* arg0;

        ok &= luaval_to_object<cocos2d::PolygonInfo>(tolua_S, 2, "cc.PolygonInfo",&arg0, "cc.Sprite:initWithPolygon");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_Sprite_initWithPolygon'", nullptr);
            return 0;
        }
        bool ret = cobj->initWithPolygon(*arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.Sprite:initWithPolygon",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Sprite_initWithPolygon'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_Sprite_setPolygonInfo(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::Sprite* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Sprite",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::Sprite*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Sprite_setPolygonInfo'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        cocos2d::PolygonInfo* arg0;

        ok &= luaval_to_object<cocos2d::PolygonInfo>(tolua_S, 2, "cc.PolygonInfo",&arg0, "cc.Sprite:setPolygonInfo");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_Sprite_setPolygonInfo'", nullptr);
            return 0;
        }
        cobj->setPolygonInfo(*arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.Sprite:setPolygonInfo",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Sprite_setPolygonInfo'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_Sprite_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.Sprite",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S)-1;

    do
    {
        if (argc == 1)
        {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "cc.Sprite:create");
            if (!ok) { break; }
            cocos2d::Sprite* ret = cocos2d::Sprite::create(arg0);
            object_to_luaval<cocos2d::Sprite>(tolua_S, "cc.Sprite",(cocos2d::Sprite*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do
    {
        if (argc == 0)
        {
            cocos2d::Sprite* ret = cocos2d::Sprite::create();
            object_to_luaval<cocos2d::Sprite>(tolua_S, "cc.Sprite",(cocos2d::Sprite*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do
    {
        if (argc == 1)
        {
            cocos2d::PolygonInfo* arg0;
            ok &= luaval_to_object<cocos2d::PolygonInfo>(tolua_S, 2, "cc.PolygonInfo",&arg0, "cc.Sprite:create");
            if (!ok) { break; }
            cocos2d::Sprite* ret = cocos2d::Sprite::create(*arg0);
            object_to_luaval<cocos2d::Sprite>(tolua_S, "cc.Sprite",(cocos2d::Sprite*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do
    {
        if (argc == 2)
        {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "cc.Sprite:create");
            if (!ok) { break; }
            cocos2d::Rect arg1;
            ok &= luaval_to_rect(tolua_S, 3, &arg1, "cc.Sprite:create");
            if (!ok) { break; }
            cocos2d::Sprite* ret = cocos2d::Sprite::create(arg0, arg1);
            object_to_luaval<cocos2d::Sprite>(tolua_S, "cc.Sprite",(cocos2d::Sprite*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "cc.Sprite:create",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Sprite_create'.",&tolua_err);
#endif
    return 0;
}

static void extendSprite(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.Sprite");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "initWithPolygon", lua_cocos2dx_Sprite_initWithPolygon);
        tolua_function(tolua_S, "setPolygonInfo", lua_cocos2dx_Sprite_setPolygonInfo);
        tolua_function(tolua_S, "create", lua_cocos2dx_Sprite_create);
    }
    lua_pop(tolua_S, 1);
}

static void extendFileUtils(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.FileUtils");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"getStringFromFile");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_FileUtils_getStringFromFile );
        lua_rawset(tolua_S,-3);

        lua_pushstring(tolua_S,"getDataFromFile");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_FileUtils_getDataFromFile );
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendUserDefault(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.UserDefault");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"getInstance");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_UserDefault_getInstance );
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendSpriteBatchNode(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.SpriteBatchNode");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"getDescendants");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_SpriteBatchNode_getDescendants );
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

NS_CC_BEGIN
EventListenerAcceleration* LuaEventListenerAcceleration::create()
{
    EventListenerAcceleration* eventAcceleration = new (std::nothrow) EventListenerAcceleration();
    if (nullptr == eventAcceleration)
        return nullptr;

    if ( eventAcceleration->init([=](Acceleration* acc, Event* event){
        LuaEventAccelerationData listenerData((void*)acc,event);
        BasicScriptData data(eventAcceleration,(void*)&listenerData);
        LuaEngine::getInstance()->handleEvent(ScriptHandlerMgr::HandlerType::EVENT_ACC, (void*)&data);
    }))
    {
        eventAcceleration->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(eventAcceleration);
    }
    return eventAcceleration;
}

EventListenerCustom* LuaEventListenerCustom::create(const std::string& eventName)
{
    EventListenerCustom* eventCustom = new (std::nothrow) EventListenerCustom();
    if (nullptr == eventCustom)
        return nullptr;

    if ( eventCustom->init(eventName, [=](EventCustom* event){
        BasicScriptData data((void*)eventCustom,(void*)event);
        LuaEngine::getInstance()->handleEvent(ScriptHandlerMgr::HandlerType::EVENT_CUSTIOM, (void*)&data );
    }))
    {
        eventCustom->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(eventCustom);
    }
    return eventCustom;
}
NS_CC_END

static int tolua_cocos2dx_LuaEventListenerAcceleration_create(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, "cc.EventListenerAcceleration", 0, &tolua_err))  goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,2,0);
        cocos2d::EventListenerAcceleration* tolua_ret = cocos2d::LuaEventListenerAcceleration::create();
        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)tolua_ret, handler, ScriptHandlerMgr::HandlerType::EVENT_ACC);
        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerAcceleration");

        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerAcceleration:create",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_LuaEventListenerAcceleration_create'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_LuaEventListenerCustom_create(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, "cc.EventListenerCustom", 0, &tolua_err))  goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(tolua_S,2,0,&tolua_err) ||
            !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const std::string eventName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,3,0);
        cocos2d::EventListenerCustom* tolua_ret = LuaEventListenerCustom::create(eventName);
        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)tolua_ret, handler, ScriptHandlerMgr::HandlerType::EVENT_CUSTIOM);

        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerCustom");

        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerCustom:create", argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_LuaEventListenerCustom_create'.",&tolua_err);
    return 0;
#endif
}

static void extendEventListenerCustom(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.EventListenerCustom");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "create",tolua_cocos2d_LuaEventListenerCustom_create);
    }
    lua_pop(tolua_S, 1);
}

static void extendEventListenerAcceleration(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.EventListenerAcceleration");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "create",tolua_cocos2dx_LuaEventListenerAcceleration_create);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_cocos2dx_EventListenerKeyboard_create(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, "cc.EventListenerKeyboard", 0, &tolua_err))  goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        cocos2d::EventListenerKeyboard* tolua_ret = cocos2d::EventListenerKeyboard::create();
        if(nullptr == tolua_ret)
            return 0;

        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerKeyboard");

        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerKeyboard:create", argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_EventListenerKeyboard_create'.",&tolua_err);
    return 0;
#endif
}

static void cloneKeyboardHandler(const EventListenerKeyboard* src,EventListenerKeyboard* dst,  ScriptHandlerMgr::HandlerType type)
{
    if (nullptr == src || nullptr == dst)
        return;

    LUA_FUNCTION handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)src, type);
    if (0 != handler)
    {
        int newscriptHandler = cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->reallocateScriptHandler(handler);

        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)dst, newscriptHandler, type);
        switch (type)
        {
            case ScriptHandlerMgr::HandlerType::EVENT_KEYBOARD_PRESSED:
                {
                    dst->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
                        LuaEventKeyboarData listenerData((int)keyCode, event);
                        BasicScriptData data((void*)dst,(void*)&listenerData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_KEYBOARD_RELEASED:
                {
                    dst->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
                        LuaEventKeyboarData listenerData((int)keyCode, event);
                        BasicScriptData data((void*)dst,(void*)&listenerData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            default:
                break;
        }
    }
}

static int tolua_cocos2dx_EventListenerKeyboard_clone(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
    EventListenerKeyboard* self = nullptr;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.EventListenerKeyboard", 0, &tolua_err))  goto tolua_lerror;
#endif

    self = static_cast<EventListenerKeyboard*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_EventListenerKeyboard_clone'\n", nullptr);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        cocos2d::EventListenerKeyboard* tolua_ret = cocos2d::EventListenerKeyboard::create();
        if(nullptr == tolua_ret)
            return 0;

        cloneKeyboardHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_KEYBOARD_PRESSED);
        cloneKeyboardHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_KEYBOARD_RELEASED);

        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerKeyboard");

        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerKeyboard:clone",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_EventListenerKeyboard_clone'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2dx_EventListenerKeyboard_registerScriptHandler(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
    EventListenerKeyboard* self = nullptr;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.EventListenerKeyboard", 0, &tolua_err))  goto tolua_lerror;
#endif

    self = static_cast<EventListenerKeyboard*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_EventListenerKeyboard_registerScriptHandler'\n", nullptr);
		return 0;
	}
#endif
    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
            !tolua_isnumber(tolua_S, 3, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif

        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,2,0);
        ScriptHandlerMgr::HandlerType type = static_cast<ScriptHandlerMgr::HandlerType>((int)tolua_tonumber(tolua_S, 3, 0));
        switch (type)
        {
            case ScriptHandlerMgr::HandlerType::EVENT_KEYBOARD_PRESSED:
                {
                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
                    self->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
                        LuaEventKeyboarData listenerData((int)keyCode, event);
                        BasicScriptData data((void*)self,(void*)&listenerData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_KEYBOARD_RELEASED:
                {
                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
                    self->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
                        LuaEventKeyboarData listenerData((int)keyCode, event);
                        BasicScriptData data((void*)self,(void*)&listenerData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            default:
                break;
        }

        return 0;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerKeyboard:registerScriptHandler", argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_EventListenerKeyboard_registerScriptHandler'.",&tolua_err);
    return 0;
#endif
}

static void extendEventListenerKeyboard(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.EventListenerKeyboard");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "create",tolua_cocos2dx_EventListenerKeyboard_create);
        tolua_function(tolua_S, "registerScriptHandler", tolua_cocos2dx_EventListenerKeyboard_registerScriptHandler);
        tolua_function(tolua_S, "clone", tolua_cocos2dx_EventListenerKeyboard_clone);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_cocos2dx_EventListenerTouchOneByOne_create(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, "cc.EventListenerTouchOneByOne", 0, &tolua_err))  goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        cocos2d::EventListenerTouchOneByOne* tolua_ret = cocos2d::EventListenerTouchOneByOne::create();
        if(nullptr == tolua_ret)
            return 0;

        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerTouchOneByOne");

        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerTouchOneByOne:create",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_EventListenerTouchOneByOne_create'.",&tolua_err);
    return 0;
#endif
}

static void cloneTouchOneByOneHandler(const EventListenerTouchOneByOne* src,EventListenerTouchOneByOne* dst,  ScriptHandlerMgr::HandlerType type)
{
    if (nullptr == src || nullptr == dst)
        return;

    LUA_FUNCTION handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)src, type);
    if (0 != handler)
    {
        int newscriptHandler = cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->reallocateScriptHandler(handler);

        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)dst, newscriptHandler, type);
        switch (type)
        {
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCH_BEGAN:
                {
                    dst->onTouchBegan = [=](Touch* touch, Event* event){
                        LuaEventTouchData touchData(touch, event);
                        BasicScriptData data((void*)dst,(void*)&touchData);
                        return LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCH_MOVED:
                {
                    dst->onTouchMoved = [=](Touch* touch, Event* event){
                        LuaEventTouchData touchData(touch, event);
                        BasicScriptData data((void*)dst,(void*)&touchData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                        };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCH_ENDED:
                {
                    dst->onTouchEnded = [=](Touch* touch, Event* event){
                        LuaEventTouchData touchData(touch, event);
                        BasicScriptData data((void*)dst,(void*)&touchData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCH_CANCELLED:
                {
                    dst->onTouchCancelled = [=](Touch* touch, Event* event){
                        LuaEventTouchData touchData(touch, event);
                        BasicScriptData data((void*)dst,(void*)&touchData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            default:
                break;
        }
    }
}

static int tolua_cocos2dx_EventListenerTouchOneByOne_clone(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
    EventListenerTouchOneByOne* self = nullptr;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.EventListenerTouchOneByOne", 0, &tolua_err))  goto tolua_lerror;
#endif

    self = static_cast<EventListenerTouchOneByOne*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_EventListenerTouchOneByOne_clone'\n", nullptr);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        cocos2d::EventListenerTouchOneByOne* tolua_ret = cocos2d::EventListenerTouchOneByOne::create();
        if(nullptr == tolua_ret)
            return 0;

        cloneTouchOneByOneHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_TOUCH_BEGAN);
        cloneTouchOneByOneHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_TOUCH_MOVED);
        cloneTouchOneByOneHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_TOUCH_ENDED);
        cloneTouchOneByOneHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_TOUCH_CANCELLED);
        tolua_ret->setSwallowTouches(self->isSwallowTouches());

        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerTouchOneByOne");

        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerTouchOneByOne:create", argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_EventListenerTouchOneByOne_clone'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2dx_EventListenerTouchOneByOne_registerScriptHandler(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
    EventListenerTouchOneByOne* self = nullptr;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.EventListenerTouchOneByOne", 0, &tolua_err))  goto tolua_lerror;
#endif

    self = static_cast<EventListenerTouchOneByOne*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_EventListenerTouchOneByOne_registerScriptHandler'\n", nullptr);
		return 0;
	}
#endif
    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
            !tolua_isnumber(tolua_S, 3, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,2,0);
        ScriptHandlerMgr::HandlerType type        = static_cast<ScriptHandlerMgr::HandlerType>((int)tolua_tonumber(tolua_S, 3, 0));
        switch (type)
        {
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCH_BEGAN:
                {
                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);

                    self->onTouchBegan = [=](Touch* touch, Event* event){
                        LuaEventTouchData touchData(touch, event);
                        BasicScriptData data((void*)self,(void*)&touchData);
                        return LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCH_MOVED:
                {
                    self->onTouchMoved = [=](Touch* touch, Event* event){
                        LuaEventTouchData touchData(touch, event);
                        BasicScriptData data((void*)self,(void*)&touchData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };

                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCH_ENDED:
                {
                    self->onTouchEnded = [=](Touch* touch, Event* event){
                        LuaEventTouchData touchData(touch, event);
                        BasicScriptData data((void*)self,(void*)&touchData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };

                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCH_CANCELLED:
                {
                    self->onTouchCancelled = [=](Touch* touch, Event* event){
                        LuaEventTouchData touchData(touch, event);
                        BasicScriptData data((void*)self,(void*)&touchData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };

                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
                }
                break;
            default:
                break;
        }
        return 0;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerTouchOneByOne:registerScriptHandler", argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_EventListenerTouchOneByOne_registerScriptHandler'.",&tolua_err);
    return 0;
#endif
}

static void extendEventListenerTouchOneByOne(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.EventListenerTouchOneByOne");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "create", tolua_cocos2dx_EventListenerTouchOneByOne_create);
        tolua_function(tolua_S, "registerScriptHandler", tolua_cocos2dx_EventListenerTouchOneByOne_registerScriptHandler);
        tolua_function(tolua_S, "clone", tolua_cocos2dx_EventListenerTouchOneByOne_clone);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_cocos2dx_EventListenerTouchAllAtOnce_create(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, "cc.EventListenerTouchAllAtOnce", 0, &tolua_err))  goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        cocos2d::EventListenerTouchAllAtOnce* tolua_ret = cocos2d::EventListenerTouchAllAtOnce::create();
        if(nullptr == tolua_ret)
            return 0;

        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerTouchAllAtOnce");

        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerTouchAllAtOnce:registerScriptHandler",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_EventListenerTouchAllAtOnce_create'.",&tolua_err);
    return 0;
#endif
}

static void cloneTouchAllAtOnceHandler(const EventListenerTouchAllAtOnce* src,EventListenerTouchAllAtOnce* dst,  ScriptHandlerMgr::HandlerType type)
{
    if (nullptr == src || nullptr == dst)
        return;

    LUA_FUNCTION handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)src, type);
    if (0 != handler)
    {
        int newscriptHandler = cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->reallocateScriptHandler(handler);

        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)dst, newscriptHandler, type);
        switch (type)
        {
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_BEGAN:
                {
                    dst->onTouchesBegan = [=](const std::vector<Touch*>& touches, Event* event){
                        LuaEventTouchesData touchesData(touches, event);
                        BasicScriptData data((void*)dst,(void*)&touchesData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_MOVED:
                {
                    dst->onTouchesMoved = [=](const std::vector<Touch*>& touches, Event* event){
                        LuaEventTouchesData touchesData(touches, event);
                        BasicScriptData data((void*)dst,(void*)&touchesData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_ENDED:
                {
                    dst->onTouchesEnded = [=](const std::vector<Touch*>& touches, Event* event){
                        LuaEventTouchesData touchesData(touches, event);
                        BasicScriptData data((void*)dst,(void*)&touchesData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_CANCELLED:
                {
                    dst->onTouchesCancelled = [=](const std::vector<Touch*>& touches, Event* event){
                        LuaEventTouchesData touchesData(touches, event);
                        BasicScriptData data((void*)dst,(void*)&touchesData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            default:
                break;
        }
    }
}

static int tolua_cocos2dx_EventListenerTouchAllAtOnce_clone(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
    EventListenerTouchAllAtOnce* self = nullptr;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.EventListenerTouchAllAtOnce", 0, &tolua_err))  goto tolua_lerror;
#endif

    self = static_cast<EventListenerTouchAllAtOnce*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_EventListenerTouchAllAtOnce_clone'\n", nullptr);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        cocos2d::EventListenerTouchAllAtOnce* tolua_ret = cocos2d::EventListenerTouchAllAtOnce::create();
        if(nullptr == tolua_ret)
            return 0;

        cloneTouchAllAtOnceHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_BEGAN);
        cloneTouchAllAtOnceHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_MOVED);
        cloneTouchAllAtOnceHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_ENDED);
        cloneTouchAllAtOnceHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_CANCELLED);

        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerTouchAllAtOnce");

        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerTouchAllAtOnce:clone", argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_EventListenerTouchAllAtOnce_clone'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2dx_EventListenerTouchAllAtOnce_registerScriptHandler(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
    EventListenerTouchAllAtOnce* self = nullptr;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.EventListenerTouchAllAtOnce", 0, &tolua_err))  goto tolua_lerror;
#endif

    self = static_cast<EventListenerTouchAllAtOnce*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_EventListenerTouchAllAtOnce_registerScriptHandler'\n", nullptr);
		return 0;
	}
#endif
    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
            !tolua_isnumber(tolua_S, 3, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,2,0);
        ScriptHandlerMgr::HandlerType type        = static_cast<ScriptHandlerMgr::HandlerType>((int)tolua_tonumber(tolua_S, 3, 0));
        switch (type)
        {
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_BEGAN:
                {
                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);

                    self->onTouchesBegan = [=](const std::vector<Touch*>& touches, Event* event){
                        LuaEventTouchesData touchesData(touches, event);
                        BasicScriptData data((void*)self,(void*)&touchesData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_MOVED:
            {
                self->onTouchesMoved = [=](const std::vector<Touch*>& touches, Event* event){
                    LuaEventTouchesData touchesData(touches, event);
                    BasicScriptData data((void*)self,(void*)&touchesData);
                    LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                };

                ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
            }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_ENDED:
            {
                self->onTouchesEnded = [=](const std::vector<Touch*>& touches, Event* event){
                    LuaEventTouchesData touchesData(touches, event);
                    BasicScriptData data((void*)self,(void*)&touchesData);
                    LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                };

                ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
            }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_TOUCHES_CANCELLED:
            {
                self->onTouchesCancelled = [=](const std::vector<Touch*>& touches, Event* event){
                    LuaEventTouchesData touchesData(touches, event);
                    BasicScriptData data((void*)self,(void*)&touchesData);
                    LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                };

                ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
            }
                break;
            default:
                break;
        }
        return 0;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerTouchAllAtOnce:registerScriptHandler",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_EventListenerTouchAllAtOnce_registerScriptHandler'.",&tolua_err);
    return 0;
#endif
}

static void extendEventListenerTouchAllAtOnce(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.EventListenerTouchAllAtOnce");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "create", tolua_cocos2dx_EventListenerTouchAllAtOnce_create);
        tolua_function(tolua_S, "registerScriptHandler", tolua_cocos2dx_EventListenerTouchAllAtOnce_registerScriptHandler);
        tolua_function(tolua_S, "clone", tolua_cocos2dx_EventListenerTouchAllAtOnce_clone);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_cocos2dx_EventListenerMouse_create(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, "cc.EventListenerMouse", 0, &tolua_err))  goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        cocos2d::EventListenerMouse* tolua_ret = cocos2d::EventListenerMouse::create();
        if(nullptr == tolua_ret)
            return 0;

        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerMouse");

        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerMouse:create",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_EventListenerMouse_create'.",&tolua_err);
    return 0;
#endif
}

static void cloneMouseHandler(const EventListenerMouse* src,EventListenerMouse* dst,  ScriptHandlerMgr::HandlerType type)
{
    if (nullptr == src || nullptr == dst)
        return;

    LUA_FUNCTION handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)src, type);
    if (0 != handler)
    {
        int newscriptHandler = cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->reallocateScriptHandler(handler);

        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)dst, newscriptHandler, type);
        switch (type)
        {
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_DOWN:
                {
                    dst->onMouseDown = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)dst,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_UP:
                {
                    dst->onMouseUp = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)dst,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_MOVE:
                {
                    dst->onMouseMove = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)dst,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_SCROLL:
                {
                    dst->onMouseScroll = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)dst,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            default:
                break;
        }
    }
}

static int tolua_cocos2dx_EventListenerMouse_clone(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
    EventListenerMouse* self = nullptr;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.EventListenerMouse", 0, &tolua_err))  goto tolua_lerror;
#endif

    self = static_cast<EventListenerMouse*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_EventListenerMouse_clone'\n", nullptr);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        cocos2d::EventListenerMouse* tolua_ret = cocos2d::EventListenerMouse::create();
        if(nullptr == tolua_ret)
            return 0;

        cloneMouseHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_MOUSE_DOWN);
        cloneMouseHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_MOUSE_MOVE);
        cloneMouseHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_MOUSE_SCROLL);
        cloneMouseHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_MOUSE_UP);

        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerMouse");

        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerMouse:clone", argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_EventListenerMouse_clone'.",&tolua_err);
    return 0;
#endif
}


static int tolua_cocos2dx_EventListenerMouse_registerScriptHandler(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    int argc = 0;
    EventListenerMouse* self = nullptr;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.EventListenerMouse", 0, &tolua_err))  goto tolua_lerror;
#endif

    self = static_cast<EventListenerMouse*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_EventListenerMouse_registerScriptHandler'\n", nullptr);
		return 0;
	}
#endif
    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
            !tolua_isnumber(tolua_S, 3, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,2,0);
        ScriptHandlerMgr::HandlerType type = static_cast<ScriptHandlerMgr::HandlerType>((int)tolua_tonumber(tolua_S, 3, 0) ) ;

        switch (type)
        {
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_DOWN:
                {
                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);

                    self->onMouseDown = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)self,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_UP:
                {
                    self->onMouseUp = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)self,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };

                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_MOVE:
                {
                    self->onMouseMove = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)self,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };

                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_SCROLL:
                {
                    self->onMouseScroll = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)self,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };

                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
                }
                break;
            default:
                break;
        }
        return 0;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.EventListenerMouse:registerScriptHandler",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_EventListenerMouse_registerScriptHandler'.",&tolua_err);
    return 0;
#endif
}

static void extendEventListenerMouse(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.EventListenerMouse");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "create", tolua_cocos2dx_EventListenerMouse_create);
        tolua_function(tolua_S, "registerScriptHandler", tolua_cocos2dx_EventListenerMouse_registerScriptHandler);
        tolua_function(tolua_S, "clone", tolua_cocos2dx_EventListenerMouse_clone);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_cocos2dx_ActionCamera_reverse(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    cocos2d::ActionCamera* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.ActionCamera",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::ActionCamera*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_cocos2dx_ActionCamera_reverse'", NULL);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        cocos2d::ActionCamera* reverse = cobj->reverse();
        toluafix_pushusertype_ccobject(tolua_S, reverse->_ID, &(reverse->_luaID), (void*)(reverse),"cc.ActionCamera");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.ActionCamera:reverse",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_ActionCamera_reverse'.",&tolua_err);
#endif
    return 0;
}

static void extendActionCamera(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.ActionCamera");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "reverse", tolua_cocos2dx_ActionCamera_reverse);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_cocos2dx_GridAction_reverse(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    int argc = 0;
    cocos2d::GridAction* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.GridAction",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::GridAction*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_cocos2dx_GridAction_reverse'", NULL);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        cocos2d::GridAction* reverse = cobj->reverse();
        toluafix_pushusertype_ccobject(tolua_S, reverse->_ID, &(reverse->_luaID), (void*)(reverse),"cc.GridAction");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.GridAction:reverse",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_GridAction_reverse'.",&tolua_err);
#endif
    return 0;
}

static void extendGridAction(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.GridAction");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "reverse", tolua_cocos2dx_GridAction_reverse);
    }
    lua_pop(tolua_S, 1);
}

static int lua_cocos2dx_Label_createWithTTF00(lua_State* L)
{
    if (nullptr == L)
        return 0;

    int argc = 0;
    bool ok = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(L,1,"cc.Label",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(L) - 1;

    if (argc >= 2 && argc <= 4)
    {

#if COCOS2D_DEBUG >= 1
        if (!tolua_istable(L, 2, 0, &tolua_err)  ||
            !tolua_isstring(L, 3, 0, &tolua_err) ||
            !tolua_isnumber(L, 4, 1, &tolua_err) ||
            !tolua_isnumber(L, 5, 1, &tolua_err) )
        {
            goto tolua_lerror;
        }
#endif
        TTFConfig ttfConfig("");
        std::string text = "";

        ok &= luaval_to_ttfconfig(L, 2, &ttfConfig, "cc.Label:createWithTTF");
        if (!ok)
            return 0;

        ok &= luaval_to_std_string(L, 3, &text,  "cc.Label:createWithTTF");
        if (!ok)
            return 0;


        int alignment = (int)tolua_tonumber(L, 4, 1);
        int lineSize  = (int)tolua_tonumber(L, 5, 0);
        cocos2d::Label* ret = cocos2d::Label::createWithTTF(ttfConfig, text, static_cast<TextHAlignment>(alignment), lineSize);
        int ID = ret ? (int)(ret->_ID) : -1;
        int* luaID = ret ? &(ret->_luaID) : nullptr;
        toluafix_pushusertype_ccobject(L,ID, luaID, (void*)ret,"cc.Label");
        return 1;
    }
    luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d\n ", "cc.Label:createWithTTF",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L,"#ferror in function 'lua_cocos2dx_Label_createWithTTF'.",&tolua_err);
#endif
    return 0;
}

static int lua_cocos2dx_Label_createWithTTF01(lua_State* L)
{
    if (nullptr == L)
        return 0;

    int argc = 0;

    tolua_Error tolua_err;
    if (!tolua_isusertable(L,1,"cc.Label",0,&tolua_err)) goto tolua_lerror;

    argc = lua_gettop(L) - 1;

    if (argc >= 3 && argc <= 6)
    {
        if (!tolua_isstring(L, 2, 0, &tolua_err)  ||
            !tolua_isstring(L, 3, 0, &tolua_err)  ||
            !tolua_isnumber(L, 4, 0, &tolua_err)  ||
            !tolua_istable(L, 5, 1, &tolua_err)   ||
            !tolua_isnumber(L, 6, 1, &tolua_err)  ||
            !tolua_isnumber(L, 7, 1, &tolua_err) )
        {
            goto tolua_lerror;
        }
        else
        {
            std::string text = tolua_tostring(L, 2, "");
            std::string fontFile = tolua_tostring(L, 3, "");
            float fontSize   = (float)tolua_tonumber(L, 4, 0);
            cocos2d::Size dimensions = cocos2d::Size::ZERO;
            if (lua_istable(L, 5))
            {
                luaval_to_size(L, 5, &dimensions,  "cc.Label:createWithTTF");
            }
            TextHAlignment hAlignment = static_cast<TextHAlignment>((int)tolua_tonumber(L, 6, 0));
            TextVAlignment vAlignment = static_cast<TextVAlignment>((int)tolua_tonumber(L, 7, 0));

            cocos2d::Label* ret = cocos2d::Label::createWithTTF(text, fontFile, fontSize, dimensions, hAlignment, vAlignment);

            int ID = ret ? (int)(ret->_ID) : -1;
            int* luaID = ret ? &(ret->_luaID) : nullptr;
            toluafix_pushusertype_ccobject(L,ID, luaID, (void*)ret,"cc.Label");
            return 1;
        }
    }

tolua_lerror:
    return lua_cocos2dx_Label_createWithTTF00(L);
}

static void extendLabel(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.Label");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "createWithTTF", lua_cocos2dx_Label_createWithTTF00);
        tolua_function(tolua_S, "createWithTTF", lua_cocos2dx_Label_createWithTTF01);
    }
    lua_pop(tolua_S, 1);
}

static int lua_cocos2dx_TMXTiledMap_getPropertiesForGID(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::TMXTiledMap* cobj = NULL;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.TMXTiledMap",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::TMXTiledMap*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_TMXTiledMap_getPropertiesForGID'", NULL);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;

    if (argc == 1)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0,  "cc.TMXTiledMap:getPropertiesForGID");
            
        if (!ok)
            return 0;
        cocos2d::Value ret = cobj->getPropertiesForGID(arg0);
        if (ret.getType() != cocos2d::Value::Type::NONE) {
            ccvalue_to_luaval(tolua_S, ret);
            return 1;
        }
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.TMXTiledMap:getPropertiesForGID",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_TMXTiledMap_getPropertiesForGID'.",&tolua_err);
#endif
    
    return 0;
}

static void extendTMXTiledMap(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.TMXTiledMap");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "getPropertiesForGID", lua_cocos2dx_TMXTiledMap_getPropertiesForGID);
    }
    lua_pop(tolua_S, 1);
}

static int lua_cocos2dx_Console_send(lua_State* tolua_S)
{
    cocos2d::Console* cobj = nullptr;
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Console",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::Console*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Console_send'", NULL);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;

    if (argc == 2)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,&arg0, "cc.Console:send");
        std::string arg1;
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "cc.Console:send");
        if(!ok)
            return 0;

        send(arg0, arg1.c_str(), arg1.length(), 0);
        return 0;
    }
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "cc.Console:send",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Console_send'.",&tolua_err);
#endif

    return 0;
}

static int lua_cocos2dx_Console_wait(lua_State* tolua_S)
{
    cocos2d::Console* cobj = nullptr;
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Console",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::Console*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Console_wait'", NULL);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;

    if (argc == 1)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,&arg0, "cc.Console:wait");
        if(!ok)
            return 0;

        std::chrono::milliseconds dura( arg0 * 1000 );
        std::this_thread::sleep_for( dura );
        return 0;
    }
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "cc.Console:wait",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Console_wait'.",&tolua_err);
#endif

    return 0;
}

static int lua_cocos2dx_Console_addCommand(lua_State* tolua_S)
{
    cocos2d::Console* cobj = nullptr;
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Console",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::Console*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Console_addCommand'", NULL);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (ok && argc == 2)
    {
        ValueMap arg0;
        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0, "cc.Console:addCommand");
        //
        std::string name = std::string(arg0["name"].asString());
        std::string help = std::string(arg0["help"].asString());

#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S, 3, "LUA_FUNCTION", 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = 0;
        if (ok) {
            handler = (  toluafix_ref_function(tolua_S,3,0));
            ScriptHandlerMgr::HandlerType handlerType = ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);

            Console::Command outValue = {
                name,
                help,
                [=](int fd, const std::string& args)
                {
                    //lua-callback, the third param;
                    tolua_pushnumber(tolua_S, fd);
                    tolua_pushstring(tolua_S, args.c_str());

                    LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 2);
                    ScriptHandlerMgr::getInstance()->removeObjectHandler(cobj, handlerType);
                }
            };
            cobj->addCommand(outValue);
        }
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "cc.Console:addCommand",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Console_addCommand'.",&tolua_err);
#endif

    return 0;
}

static void extendConsole(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.Console");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S,"wait", lua_cocos2dx_Console_wait);
        tolua_function(tolua_S,"send", lua_cocos2dx_Console_send);
        tolua_function(tolua_S,"addCommand", lua_cocos2dx_Console_addCommand);
    }
    lua_pop(tolua_S, 1);
}

static int lua_cocos2dx_OrbitCamera_sphericalRadius(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::OrbitCamera* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.OrbitCamera",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::OrbitCamera*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!self)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_OrbitCamera_sphericalRadius'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
            !tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
            !tolua_isnumber(tolua_S, 4, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif

        float newRadius = (float)tolua_tonumber(tolua_S, 2, 0);
        float zenith    = (float)tolua_tonumber(tolua_S, 3, 0);
        float azimuth   = (float)tolua_tonumber(tolua_S, 4, 0);

        self->sphericalRadius(&newRadius, &zenith, &azimuth);

        tolua_pushnumber(tolua_S, (lua_Number)newRadius);
        tolua_pushnumber(tolua_S, (lua_Number)zenith);
        tolua_pushnumber(tolua_S, (lua_Number)azimuth);

        return 3;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.OrbitCamera:sphericalRadius",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_OrbitCamera_sphericalRadius'.",&tolua_err);
#endif

    return 0;
}

static void extendOrbitCamera(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.OrbitCamera");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S,"sphericalRadius", lua_cocos2dx_OrbitCamera_sphericalRadius);
    }
    lua_pop(tolua_S, 1);
}

int lua_cocos2dx_TMXLayer_getTileGIDAt(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::TMXLayer* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.TMXLayer",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (cocos2d::TMXLayer*)tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_TMXLayer_getTileGIDAt'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        cocos2d::Vec2 arg0;
        
        ok &= luaval_to_vec2(tolua_S, 2, &arg0, "cc.TMXLayer:getTileGIDAt");
        if(!ok)
            return 0;
        unsigned int ret = cobj->getTileGIDAt(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        tolua_pushnumber(tolua_S,(lua_Number)0);
        return 2;
    }
    if (argc == 2)
    {
        cocos2d::Vec2 arg0;
        int arg1;
        
        ok &= luaval_to_vec2(tolua_S, 2, &arg0, "cc.TMXLayer:getTileGIDAt");
        ok &= luaval_to_int32(tolua_S, 3, &arg1, "cc.TMXLayer:getTileGIDAt");
        
        if(!ok)
            return 0;
        
        unsigned int ret = cobj->getTileGIDAt(arg0, (cocos2d::TMXTileFlags*)&arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        tolua_pushnumber(tolua_S,(lua_Number)arg1);
        return 2;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.TMXLayer:getTileGIDAt",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_TMXLayer_getTileGIDAt'.",&tolua_err);
#endif
    
    return 0;
}

int lua_cocos2dx_TMXLayer_setTiles(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::TMXLayer* cobj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.TMXLayer",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (cocos2d::TMXLayer*)tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_TMXLayer_setTiles'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        uint32_t* arg0 = nullptr;
#if COCOS2D_DEBUG >= 1
        if (!tolua_istable(tolua_S, 2, 0, &tolua_err))
            goto tolua_lerror;
#endif
        
        size_t len = lua_objlen(tolua_S, 2);
        if (len == 0 )
        {
            luaL_error(tolua_S, "Table's len equal 0");
            return 0;
        }
        arg0 = new (std::nothrow) uint32_t[len];
        
        if (nullptr == arg0)
        {
            luaL_error(tolua_S, "Allocate uint32_t array in the lua_cocos2dx_TMXLayer_setTiles failed!");
            return 0;
        }
        
        for (size_t i = 1 ; i <= len; i++)
        {
            arg0[i - 1] = (uint32_t)tolua_tofieldnumber(tolua_S, 2, i, 0);
        }
        
        cobj->setTiles(arg0);
        
        CC_SAFE_DELETE_ARRAY(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.TMXLayer:setTiles",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_TMXLayer_setTiles'.",&tolua_err);
#endif
    
    return 0;
}

static void extendTMXLayer(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.TMXLayer");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S,"getTileGIDAt", lua_cocos2dx_TMXLayer_getTileGIDAt);
        tolua_function(tolua_S, "setTiles", lua_cocos2dx_TMXLayer_setTiles);
    }
    lua_pop(tolua_S, 1);
}

int lua_cocos2dx_Application_isIOS64bit(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::Application* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Application",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::Application*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Application_isIOS64bit'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        bool isIOS64bit = false;
        Application::Platform platform = cocos2d::Application::getInstance()->getTargetPlatform();
        if (Application::Platform::OS_IPHONE == platform || Application::Platform::OS_IPAD == platform)
        {
#if defined(__LP64__)
            isIOS64bit = true;
#endif
        }

        tolua_pushboolean(tolua_S, isIOS64bit);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.Application:isIOS64bit",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Application_isIOS64bit'.",&tolua_err);
#endif

    return 0;
}

int lua_cocos2dx_Application_is64BitIOSDevice(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::Application* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Application",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::Application*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Application_is64BitIOSDevice'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        bool is64BitIOSDevice = false;
        Application::Platform platform = cocos2d::Application::getInstance()->getTargetPlatform();
        if (Application::Platform::OS_IPHONE == platform || Application::Platform::OS_IPAD == platform || Application::Platform::OS_ANDROID == platform)
        {
#if defined(__arm64__) || defined(__aarch64__)
            is64BitIOSDevice = true;
#endif
        }

        tolua_pushboolean(tolua_S, is64BitIOSDevice);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.Application:is64BitIOSDevice",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Application_is64BitIOSDevice'.",&tolua_err);
#endif

    return 0;
}

static void extendApplication(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.Application");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "isIOS64bit", lua_cocos2dx_Application_isIOS64bit);
        tolua_function(tolua_S, "is64BitIOSDevice", lua_cocos2dx_Application_is64BitIOSDevice);
    }
    lua_pop(tolua_S, 1);
}

static int lua_cocos2dx_TextureCache_addImageAsync(lua_State* tolua_S)
{
    int argc = 0;
    TextureCache* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "cc.TextureCache", 0, &tolua_err)) goto tolua_lerror;
#endif

    cobj = static_cast<TextureCache*>(tolua_tousertype(tolua_S, 1, 0));

#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_TextureCache_addImageAsync'\n", nullptr);
		return 0;
	}
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (argc >= 2)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(tolua_S, 2, 0, &tolua_err)  ||
            !toluafix_isfunction(tolua_S, 3, "LUA_FUNCTION", 0, &tolua_err))
        {
            goto tolua_lerror;
        }
        if (argc >= 3 && !tolua_isstring(tolua_S, 4, 0, &tolua_err)) {
            goto tolua_lerror;
        }
        if (argc >= 4 && !tolua_isnumber(tolua_S, 5, 0, &tolua_err)) {
            goto tolua_lerror;
        }
#endif
        const char* filePath = tolua_tostring(tolua_S, 2, "");
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S, 3, 0);
        const char* callkey = filePath;
        backend::PixelFormat pixelFormat = Texture2D::getDefaultAlphaPixelFormat();

        if (argc >= 3) {
            callkey = tolua_tostring(tolua_S, 4, "");
        }

        if (argc >= 4) {
            pixelFormat = (backend::PixelFormat)lua_tointeger(tolua_S, 5);
        }

        cobj->addImageAsync(filePath, [=](cocos2d::Texture2D* tex) {
            object_to_luaval<cocos2d::Texture2D>(tolua_S, "cc.Texture2D", tex);
            LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 1);
            toluafix_remove_function_by_refid(tolua_S, handler);
        }, callkey, pixelFormat);

        return 0;
    }

    luaL_error(tolua_S, "%s function of TextureCache has wrong number of arguments: %d, was expecting 2~5\n", "cc.TextureCache:addImageAsync", argc);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_TextureCache_addImageAsync'.", &tolua_err);
    return 0;
#endif
}

static void extendTextureCache(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.TextureCache");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "addImageAsync", lua_cocos2dx_TextureCache_addImageAsync);
    }
    lua_pop(tolua_S, 1);
}

int lua_cocos2dx_GLView_getAllTouches(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::GLView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.GLView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::GLView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_GLView_getAllTouches'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
            return 0;

        std::vector<cocos2d::Touch*> ret = cobj->getAllTouches();
        lua_newtable(tolua_S);
        if (ret.empty())
            return 1;

        int index = 1;
        for (const auto& obj : ret)
        {
            if (nullptr == obj)
                continue;

            lua_pushnumber(tolua_S, (lua_Number)index);
            int ID = (obj) ? (int)obj->_ID : -1;
            int* luaID = (obj) ? &obj->_luaID : nullptr;
            toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)obj, "cc.Touch");
            lua_rawset(tolua_S, -3);
            ++index;
        }

        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.GLView:getAllTouches",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_GLView_getAllTouches'.",&tolua_err);
#endif

    return 0;
}

static void extendGLView(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.GLView");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "getAllTouches", lua_cocos2dx_GLView_getAllTouches);
    }
    lua_pop(tolua_S, 1);
}

int lua_cocos2dx_Camera_unproject(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::Camera* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Camera",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::Camera*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Camera_unproject'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        cocos2d::Vec3 arg0;

        ok &= luaval_to_vec3(tolua_S, 2, &arg0, "cc.Camera:project");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_Camera_project'", nullptr);
            return 0;
        }
        auto ret = cobj->unproject(arg0);
        vec3_to_luaval(tolua_S, ret);
        return 1;
    }
    if (argc == 3)
    {
        cocos2d::Size arg0;
        cocos2d::Vec3 arg1;
        cocos2d::Vec3 arg2;

        ok &= luaval_to_size(tolua_S, 2, &arg0, "cc.Camera:unproject");

        ok &= luaval_to_vec3(tolua_S, 3, &arg1, "cc.Camera:unproject");

        ok &= luaval_to_vec3(tolua_S, 4, &arg2, "cc.Camera:unproject");

        if(!ok)
            return 0;
        cobj->unproject(arg0, &arg1, &arg2);
        vec3_to_luaval(tolua_S, arg2);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.Camera:unproject",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Camera_unproject'.",&tolua_err);
#endif

    return 0;
}

static void extendCamera(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.Camera");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "unproject", lua_cocos2dx_Camera_unproject);
    }
    lua_pop(tolua_S, 1);
}

int lua_cocos2dx_get_PolygonInfo_rect(lua_State* tolua_S)
{
    cocos2d::PolygonInfo* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.PolygonInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::PolygonInfo*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == cobj)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_get_PolygonInfo_rect'\n", nullptr);
        return 0;
    }
#endif
    rect_to_luaval(tolua_S, cobj->getRect());
    return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_get_PolygonInfo_rect'.",&tolua_err);
    return 0;
#endif
}

int lua_cocos2dx_set_PolygonInfo_rect(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::PolygonInfo* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.PolygonInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::PolygonInfo*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_set_PolygonInfo_rect'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (1 == argc)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_istable(tolua_S, 2, 0, &tolua_err))
            goto tolua_lerror;
#endif
        Rect outRect;
        luaval_to_rect(tolua_S, 2, &outRect);
        self->setRect(outRect);
        return 0;
    }

    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_set_PolygonInfo_rect'.",&tolua_err);
    return 0;
#endif
}

int lua_cocos2dx_get_PolygonInfo_filename(lua_State* tolua_S)
{
    cocos2d::PolygonInfo* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.PolygonInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::PolygonInfo*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == cobj)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_get_PolygonInfo_filename'\n", nullptr);
        return 0;
    }
#endif
    tolua_pushcppstring(tolua_S, cobj->getFilename());
    return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_get_PolygonInfo_filename'.",&tolua_err);
    return 0;
#endif
}

int lua_cocos2dx_set_PolygonInfo_filename(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::PolygonInfo* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.PolygonInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::PolygonInfo*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_set_PolygonInfo_filename'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (1 == argc)
    {
        std::string outFilename;
        luaval_to_std_string(tolua_S, 2, &outFilename);
        self->setFilename(outFilename);
        return 0;
    }

    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_set_PolygonInfo_filename'.",&tolua_err);
    return 0;
#endif
}

static int lua_collect_PolygonInfo (lua_State* tolua_S)
{
    cocos2d::PolygonInfo* self = (cocos2d::PolygonInfo*) tolua_tousertype(tolua_S,1,0);
    CC_SAFE_DELETE(self);
    return 0;
}


int lua_cocos2dx_get_PipelineDescriptor_programState(lua_State* tolua_S)
{
    cocos2d::PipelineDescriptor* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.PipelineDescriptor", 0, &tolua_err)) goto tolua_lerror;
#endif

    cobj = (cocos2d::PipelineDescriptor*)  tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == cobj)
    {
        tolua_error(tolua_S, "invalid 'self' in function 'lua_cocos2dx_get_PipelineDescriptor_programState'\n", nullptr);
        return 0;
    }
#endif
    tolua_pushusertype(tolua_S, cobj->programState, "ccb.ProgramState");
    return 1;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
                tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_get_PipelineDescriptor_programState'.", &tolua_err);
                return 0;
#endif
}



int lua_cocos2dx_set_PipelineDescriptor_programState(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::PipelineDescriptor* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.PipelineDescriptor", 0, &tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::PipelineDescriptor*)  tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S, "invalid 'self' in function 'lua_cocos2dx_set_PipelineDescriptor_programState'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (1 == argc)
    {
        cocos2d::backend::ProgramState *programState = nullptr;
        luaval_to_object<cocos2d::backend::ProgramState>(tolua_S, 2, "ccb.ProgramState", &programState, "lua_cocos2dx_set_PipelineDescriptor_programState");
        self->programState = programState;
        return 0;
    }

    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
                tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_set_PipelineDescriptor_programState'.", &tolua_err);
                return 0;
#endif
}

static void extendPolygonInfo(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.PolygonInfo");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_variable(tolua_S, "rect", lua_cocos2dx_get_PolygonInfo_rect, lua_cocos2dx_set_PolygonInfo_rect);
        tolua_variable(tolua_S, "filename", lua_cocos2dx_get_PolygonInfo_filename, lua_cocos2dx_set_PolygonInfo_filename);
    }
    lua_pop(tolua_S, 1);

    luaL_getmetatable(tolua_S, "cc.PolygonInfo");
    if (lua_istable(tolua_S, -1))
    {
        tolua_function(tolua_S, ".collector", lua_collect_PolygonInfo);
    }
    lua_pop(tolua_S, 1);
}

static void extendPipelineDescriptor(lua_State *tolua_S)
{
    lua_pushstring(tolua_S, "cc.PipelineDescriptor");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S, -1))
    {
        tolua_variable(tolua_S, "programState", lua_cocos2dx_get_PipelineDescriptor_programState, lua_cocos2dx_set_PipelineDescriptor_programState);
    }
    lua_pop(tolua_S, 1);
}

static int lua_cocos2dx_backend_ProgramState_getUniformLocation(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::backend::ProgramState* cobj = nullptr;
    bool ok = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S, 1, "ccb.ProgramState", 0, &tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::backend::ProgramState*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S, "invalid 'cobj' in function 'lua_cocos2dx_backend_ProgramState_getUniformLocation'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S) - 1;
    do {
        if (argc == 1) {
            if (lua_isstring(tolua_S, -1))
            {
                std::string arg0;
                ok &= luaval_to_std_string(tolua_S, 2, &arg0, "ccb.ProgramState:getUniformLocation");

                if (!ok) { break; }
                cocos2d::backend::UniformLocation ret = cobj->getUniformLocation(arg0);
                uniformLocation_to_luaval(tolua_S, ret); 
                return 1;
            }
            else if (lua_isnumber(tolua_S, -1))
            {
                cocos2d::backend::Uniform arg0;
                ok &= luaval_to_int32(tolua_S, 2, (int *)&arg0, "ccb.ProgramState:getUniformLocation");

                if (!ok) { break; }
                cocos2d::backend::UniformLocation ret = cobj->getUniformLocation(arg0);
                uniformLocation_to_luaval(tolua_S, ret);
                return 1;
            }
        }
    } while (0);
    
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ccb.ProgramState:getUniformLocation", argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_backend_ProgramState_getUniformLocation'.", &tolua_err);
#endif
    return 0;
}

static int lua_cocos2dx_ProgramState_setUniform(lua_State *tolua_S)
{
    bool ok = true;
    int argc = 0;
    cocos2d::backend::ProgramState* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "ccb.ProgramState", 0, &tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::backend::ProgramState*) tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
        tolua_error(tolua_S, "invalid 'self' in function 'lua_cocos2dx_ProgramState_setUniform'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (2 == argc) {
        cocos2d::backend::UniformLocation location;
        
        if (lua_isstring(tolua_S, 2)) {
            location = self->getUniformLocation(lua_tostring(tolua_S, 2));
        }  else if (lua_istable(tolua_S, 2)) {
            ok &= luaval_to_uniformLocation(tolua_S, 2, location, "ccb.ProgramState:setUniform");
        }

        if (ok && lua_istable(tolua_S, 3)) {
            int len = lua_objlen(tolua_S, 3);
            std::vector<uint8_t> buffer(len);

            for (int i = 0; i < len; i++) {
                lua_rawgeti(tolua_S, 3, i + 1);
                buffer[i] = lua_tointeger(tolua_S, -1);
                lua_pop(tolua_S, 1);
            }
            self->setUniform(location, buffer.data(), buffer.size());
        } else {
            luaL_error(tolua_S, "`lua_cocos2dx_ProgramState_setUniform` argument invalidate");
        }
        return 0;
    }

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_ProgramState_setUniform'.", &tolua_err);
#endif
    return 0;
}

static int lua_cocos2dx_ProgramState_setUniformVec2(lua_State *tolua_S)
{
    bool ok = true;
    int argc = 0;
    cocos2d::backend::ProgramState* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "ccb.ProgramState", 0, &tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::backend::ProgramState*) tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
        tolua_error(tolua_S, "invalid 'self' in function 'lua_cocos2dx_ProgramState_setUniformVec2'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (2 == argc) {
        cocos2d::backend::UniformLocation location;
        cocos2d::Vec2 value;
        
        if (lua_isstring(tolua_S, 2)) {
            location = self->getUniformLocation(lua_tostring(tolua_S, 2));
        }  else if (lua_istable(tolua_S, 2)) {
            ok &= luaval_to_uniformLocation(tolua_S, 2, location, "ccb.ProgramState:setUniformVec2");
        }

        ok &= luaval_to_vec2(tolua_S, 3, &value);
        if (ok) {
            self->setUniform(location, &value, sizeof(cocos2d::Vec2));
        } else {
            luaL_error(tolua_S, "`lua_cocos2dx_ProgramState_setUniformVec2` argument invalidate");
        }
        return 0;
    }

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_ProgramState_setUniformVec2'.", &tolua_err);
#endif
    return 0;
}

static int lua_cocos2dx_ProgramState_setUniformVec3(lua_State *tolua_S)
{
    bool ok = true;
    int argc = 0;
    cocos2d::backend::ProgramState* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "ccb.ProgramState", 0, &tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::backend::ProgramState*) tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
        tolua_error(tolua_S, "invalid 'self' in function 'lua_cocos2dx_ProgramState_setUniformVec3'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (2 == argc) {
        cocos2d::backend::UniformLocation location;
        cocos2d::Vec3 value;
        
        if (lua_isstring(tolua_S, 2)) {
            location = self->getUniformLocation(lua_tostring(tolua_S, 2));
        }  else if (lua_istable(tolua_S, 2)) {
            ok &= luaval_to_uniformLocation(tolua_S, 2, location, "ccb.ProgramState:setUniformVec3");
        }

        ok &= luaval_to_vec3(tolua_S, 3, &value);
        if (ok) {
            self->setUniform(location, &value, sizeof(cocos2d::Vec3));
        } else {
            luaL_error(tolua_S, "`lua_cocos2dx_ProgramState_setUniformVec3` argument invalidate");
        }
        return 0;
    }

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_ProgramState_setUniformVec3'.", &tolua_err);
#endif
    return 0;
}

static int lua_cocos2dx_ProgramState_setUniformVec4(lua_State *tolua_S)
{
    bool ok = true;
    int argc = 0;
    cocos2d::backend::ProgramState* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "ccb.ProgramState", 0, &tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::backend::ProgramState*) tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
        tolua_error(tolua_S, "invalid 'self' in function 'lua_cocos2dx_ProgramState_setUniformVec4'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (2 == argc) {
        cocos2d::backend::UniformLocation location;
        cocos2d::Vec4 value;
        
        if (lua_isstring(tolua_S, 2)) {
            location = self->getUniformLocation(lua_tostring(tolua_S, 2));
        }  else if (lua_istable(tolua_S, 2)) {
            ok &= luaval_to_uniformLocation(tolua_S, 2, location, "ccb.ProgramState:setUniformVec4");
        }

        ok &= luaval_to_vec4(tolua_S, 3, &value);
        if (ok) {
            self->setUniform(location, &value, sizeof(cocos2d::Vec4));
        } else {
            luaL_error(tolua_S, "`lua_cocos2dx_ProgramState_setUniformVec4` argument invalidate");
        }
        return 0;
    }

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_ProgramState_setUniformVec4'.", &tolua_err);
#endif
    return 0;
}

static int lua_cocos2dx_ProgramState_setUniformInt(lua_State *tolua_S)
{
    bool ok = true;
    int argc = 0;
    cocos2d::backend::ProgramState* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "ccb.ProgramState", 0, &tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::backend::ProgramState*) tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
        tolua_error(tolua_S, "invalid 'self' in function 'lua_cocos2dx_ProgramState_setUniformInt'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (2 == argc) {
        cocos2d::backend::UniformLocation location;
        int value;
        
        if (lua_isstring(tolua_S, 2)) {
            location = self->getUniformLocation(lua_tostring(tolua_S, 2));
        }  else if (lua_istable(tolua_S, 2)) {
            ok &= luaval_to_uniformLocation(tolua_S, 2, location, "ccb.ProgramState:setUniformInt");
        }

        ok &= luaval_to_int32(tolua_S, 3, &value);
        if (ok) {
            self->setUniform(location, &value, sizeof(int));
        } else {
            luaL_error(tolua_S, "`lua_cocos2dx_ProgramState_setUniformInt` argument invalidate");
        }
        return 0;
    }

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_ProgramState_setUniformInt'.", &tolua_err);
#endif
    return 0;
}

static int lua_cocos2dx_ProgramState_setUniformFloat(lua_State *tolua_S)
{
    bool ok = true;
    int argc = 0;
    cocos2d::backend::ProgramState* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "ccb.ProgramState", 0, &tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::backend::ProgramState*) tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
        tolua_error(tolua_S, "invalid 'self' in function 'lua_cocos2dx_ProgramState_setUniformFloat'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (2 == argc) {
        cocos2d::backend::UniformLocation location;
        float value;
        
        if (lua_isstring(tolua_S, 2)) {
            location = self->getUniformLocation(lua_tostring(tolua_S, 2));
        }  else if (lua_istable(tolua_S, 2)) {
            ok &= luaval_to_uniformLocation(tolua_S, 2, location, "ccb.ProgramState:setUniformFloat");
        }

        ok &= luaval_to_float(tolua_S, 3, &value);
        if (ok) {
            self->setUniform(location, &value, sizeof(float));
        } else {
            luaL_error(tolua_S, "`lua_cocos2dx_ProgramState_setUniformFloat` argument invalidate");
        }
        return 0;
    }

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_ProgramState_setUniformFloat'.", &tolua_err);
#endif
    return 0;
}

static int lua_cocos2dx_ProgramState_setUniformMat4(lua_State *tolua_S)
{
    bool ok = true;
    int argc = 0;
    cocos2d::backend::ProgramState* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "ccb.ProgramState", 0, &tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::backend::ProgramState*) tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
        tolua_error(tolua_S, "invalid 'self' in function 'lua_cocos2dx_ProgramState_setUniformMat4'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (2 == argc) {
        cocos2d::backend::UniformLocation location;
        cocos2d::Mat4 value;
        
        if (lua_isstring(tolua_S, 2)) {
            location = self->getUniformLocation(lua_tostring(tolua_S, 2));
        }  else if (lua_istable(tolua_S, 2)) {
            ok &= luaval_to_uniformLocation(tolua_S, 2, location, "ccb.ProgramState:setUniformMat4");
        }

        ok &= luaval_to_mat4(tolua_S, 3, &value);
        if (ok) {
            self->setUniform(location, &value, sizeof(cocos2d::Mat4));
        } else {
            luaL_error(tolua_S, "`lua_cocos2dx_ProgramState_setUniformMat4` argument invalidate");
        }
        return 0;
    }

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_ProgramState_setUniformMat4'.", &tolua_err);
#endif
    return 0;
}

static int lua_cocos2dx_ProgramState_setUniformTexture(lua_State *tolua_S)
{
    bool ok = true;
    int argc = 0;
    cocos2d::backend::ProgramState* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "ccb.ProgramState", 0, &tolua_err)) goto tolua_lerror;
#endif

    self = (cocos2d::backend::ProgramState*) tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
        tolua_error(tolua_S, "invalid 'self' in function 'lua_cocos2dx_ProgramState_setUniformTexture'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (3 == argc) {
        cocos2d::backend::UniformLocation location;
        unsigned int slot;
        cocos2d::backend::TextureBackend *texture;
        
        if (lua_isstring(tolua_S, 2)) {
            location = self->getUniformLocation(lua_tostring(tolua_S, 2));
        }  else if (lua_istable(tolua_S, 2)) {
            ok &= luaval_to_uniformLocation(tolua_S, 2, location, "ccb.ProgramState:setUniformTexture");
        }
        
        ok &= luaval_to_uint32(tolua_S, 3, &slot, "ccb.ProgramState:setUniformTexture");
        ok &= luaval_to_object<cocos2d::backend::TextureBackend>(tolua_S, 4, "ccb.TextureBackend", &texture, "ccb.ProgramState:setUniformTexture");
        if (ok) {
            self->setTexture(location, slot, texture);
        } else {
            luaL_error(tolua_S, "`lua_cocos2dx_ProgramState_setUniformTexture` argument invalidate");
        }
        return 0;
    }

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_ProgramState_setUniformTexture'.", &tolua_err);
#endif
    return 0;
}

static void extendProgramState(lua_State *tolua_S)
{
    //lua_pushstring(tolua_S, "ccb.ProgramState");
    //lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    
    tolua_beginmodule(tolua_S, "ccb");
    tolua_beginmodule(tolua_S, "ProgramState");
    if (lua_istable(tolua_S, -1))
    {
        tolua_function(tolua_S, "getUniformLocation", lua_cocos2dx_backend_ProgramState_getUniformLocation);
        tolua_function(tolua_S, "setUniform", lua_cocos2dx_ProgramState_setUniform);// unfriendly for Lua
        tolua_function(tolua_S, "setUniformVec2", lua_cocos2dx_ProgramState_setUniformVec2);
        tolua_function(tolua_S, "setUniformVec3", lua_cocos2dx_ProgramState_setUniformVec3);
        tolua_function(tolua_S, "setUniformVec4", lua_cocos2dx_ProgramState_setUniformVec4);
        tolua_function(tolua_S, "setUniformInt", lua_cocos2dx_ProgramState_setUniformInt);
        tolua_function(tolua_S, "setUniformFloat", lua_cocos2dx_ProgramState_setUniformFloat);
        tolua_function(tolua_S, "setUniformMat4", lua_cocos2dx_ProgramState_setUniformMat4);
        tolua_function(tolua_S, "setUniformTexture", lua_cocos2dx_ProgramState_setUniformTexture);
    }
    tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
    //lua_pop(tolua_S, 1);
}

int lua_cocos2dx_AutoPolygon_generatePolygon(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.AutoPolygon",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "cc.AutoPolygon:generatePolygon");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_AutoPolygon_generatePolygon'", nullptr);
            return 0;
        }
        cocos2d::PolygonInfo* ret = new (std::nothrow) cocos2d::PolygonInfo(cocos2d::AutoPolygon::generatePolygon(arg0));
        object_to_luaval<cocos2d::PolygonInfo>(tolua_S, "cc.PolygonInfo",(cocos2d::PolygonInfo*)ret);
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    if (argc == 2)
    {
        std::string arg0;
        cocos2d::Rect arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "cc.AutoPolygon:generatePolygon");
        ok &= luaval_to_rect(tolua_S, 3, &arg1, "cc.AutoPolygon:generatePolygon");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_AutoPolygon_generatePolygon'", nullptr);
            return 0;
        }
        cocos2d::PolygonInfo* ret = new (std::nothrow) cocos2d::PolygonInfo(cocos2d::AutoPolygon::generatePolygon(arg0, arg1));
        object_to_luaval<cocos2d::PolygonInfo>(tolua_S, "cc.PolygonInfo",(cocos2d::PolygonInfo*)ret);
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    if (argc == 3)
    {
        std::string arg0;
        cocos2d::Rect arg1;
        double arg2;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "cc.AutoPolygon:generatePolygon");
        ok &= luaval_to_rect(tolua_S, 3, &arg1, "cc.AutoPolygon:generatePolygon");
        ok &= luaval_to_number(tolua_S, 4,&arg2, "cc.AutoPolygon:generatePolygon");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_AutoPolygon_generatePolygon'", nullptr);
            return 0;
        }
        cocos2d::PolygonInfo* ret = new (std::nothrow) cocos2d::PolygonInfo(cocos2d::AutoPolygon::generatePolygon(arg0, arg1, (float)arg2));
        object_to_luaval<cocos2d::PolygonInfo>(tolua_S, "cc.PolygonInfo",(cocos2d::PolygonInfo*)ret);
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    if (argc == 4)
    {
        std::string arg0;
        cocos2d::Rect arg1;
        double arg2;
        double arg3;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "cc.AutoPolygon:generatePolygon");
        ok &= luaval_to_rect(tolua_S, 3, &arg1, "cc.AutoPolygon:generatePolygon");
        ok &= luaval_to_number(tolua_S, 4,&arg2, "cc.AutoPolygon:generatePolygon");
        ok &= luaval_to_number(tolua_S, 5,&arg3, "cc.AutoPolygon:generatePolygon");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_AutoPolygon_generatePolygon'", nullptr);
            return 0;
        }
        cocos2d::PolygonInfo* ret = new (std::nothrow) cocos2d::PolygonInfo(cocos2d::AutoPolygon::generatePolygon(arg0, arg1, (float)arg2, (float)arg3));
        object_to_luaval<cocos2d::PolygonInfo>(tolua_S, "cc.PolygonInfo",(cocos2d::PolygonInfo*)ret);
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "cc.AutoPolygon:generatePolygon",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_AutoPolygon_generatePolygon'.",&tolua_err);
#endif
    return 0;
}

static int lua_collect_AutoPolygon (lua_State* tolua_S)
{
    cocos2d::AutoPolygon* self = (cocos2d::AutoPolygon*) tolua_tousertype(tolua_S,1,0);
    CC_SAFE_DELETE(self);
    return 0;
}

static void extendAutoPolygon(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.AutoPolygon");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "generatePolygon", lua_cocos2dx_AutoPolygon_generatePolygon);
    }
    lua_pop(tolua_S, 1);

    luaL_getmetatable(tolua_S, "cc.AutoPolygon");
    if (lua_istable(tolua_S, -1))
    {
        tolua_function(tolua_S, ".collector", lua_collect_AutoPolygon);
    }
    lua_pop(tolua_S, 1);
}

int register_all_cocos2dx_manual(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;

    extendNode(tolua_S);
    extendScene(tolua_S);
    extendScheduler(tolua_S);
    extendSequence(tolua_S);
    extendCallFunc(tolua_S);
    extendSpawn(tolua_S);
    extendCardinalSplineBy(tolua_S);
    extendCatmullRomBy(tolua_S);
    extendCatmullRomTo(tolua_S);
    extendBezierBy(tolua_S);
    extendBezierTo(tolua_S);
    extendDrawNode(tolua_S);
    extendSprite(tolua_S);
    extendFileUtils(tolua_S);
    extendUserDefault(tolua_S);
    extendTexture2D(tolua_S);
    extendSpriteBatchNode(tolua_S);
    extendEventListenerKeyboard(tolua_S);
    extendEventListenerTouchOneByOne(tolua_S);
    extendEventListenerTouchAllAtOnce(tolua_S);
    extendEventListenerMouse(tolua_S);
    extendEventListenerCustom(tolua_S);
    extendEventListenerAcceleration(tolua_S);
    extendActionCamera(tolua_S);
    extendGridAction(tolua_S);
    extendPipelineDescriptor(tolua_S);
    extendProgramState(tolua_S);

    extendLabel(tolua_S);
    extendTMXTiledMap(tolua_S);
    extendConsole(tolua_S);
    extendOrbitCamera(tolua_S);
    extendTMXLayer(tolua_S);
    extendApplication(tolua_S);
    extendTextureCache(tolua_S);
    extendGLView(tolua_S);
    extendCamera(tolua_S);
    extendAutoPolygon(tolua_S);
    extendPolygonInfo(tolua_S);
    extendRenderTexture(tolua_S);
    return 0;
}

static int tolua_cocos2d_utils_captureScreen(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_istable(tolua_S,1,0, &tolua_err) ||
        !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,2,0);
        std::string fileName = "";
        if (lua_isstring(tolua_S, 3)) {
            fileName = tolua_tocppstring(tolua_S, 3, "");
        }
        
        cocos2d::utils::captureScreen([=](cocos2d::Image *image) {
            // failed
            if (!image) {
                tolua_pushboolean(tolua_S, false);
                tolua_pushstring(tolua_S, fileName.c_str());
                LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 2);
                LuaEngine::getInstance()->removeScriptHandler(handler);
            }
            
            // not save to file, just return (image)
            if (fileName.length() == 0) {
                // NOTE: need call image:release() in Lua code.
                object_to_luaval<cocos2d::Image>(tolua_S,"cc.Image",(cocos2d::Image *)image);
                LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 1);
                LuaEngine::getInstance()->removeScriptHandler(handler);
                return;
            }
            
            // save to file, return (isSuccess, fullPath)
            std::string outputFile = "";
            if (FileUtils::getInstance()->isAbsolutePath(fileName)) {
                outputFile = fileName;
            } else {
                CCASSERT(fileName.find('/') == std::string::npos, "The existence of a relative path is not guaranteed!");
                outputFile = FileUtils::getInstance()->getWritablePath() + fileName;
            }
            // Save image in AsyncTaskPool::TaskType::TASK_IO thread, and callback to Lua in mainThread
            std::function<void(void*)> mainThread = [tolua_S, handler, outputFile](void*) {
                tolua_pushboolean(tolua_S, true);
                tolua_pushstring(tolua_S, outputFile.c_str());
                LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 2);
                LuaEngine::getInstance()->removeScriptHandler(handler);
            };
            AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO, std::move(mainThread), nullptr, [image, outputFile]()
            {
                image->saveToFile(outputFile);
                delete image;
            });
        });
        return 0;
    }
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_utils_captureScreen'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_utils_captureNode(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_istable(tolua_S,1,0, &tolua_err) ||
        !tolua_isusertype(tolua_S,2,"cc.Node",0,&tolua_err) ||
        !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        cocos2d::Node *node = (cocos2d::Node *)tolua_tousertype(tolua_S,2,0);
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,3,0);
        float scale = 1.0;
        if (lua_isnumber(tolua_S, 4)) {
            scale = tolua_tonumber(tolua_S,4,1);
        }
        cocos2d::utils::captureNode(node, [=](cocos2d::Image *image) {
            if (image) {
                // NOTE: need call image:release() in Lua code.
                object_to_luaval<cocos2d::Image>(tolua_S,"cc.Image",(cocos2d::Image *)image);
            } else {
                lua_pushnil(tolua_S);
            }
            LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 1);
            LuaEngine::getInstance()->removeScriptHandler(handler);
        }, scale);
        return 0;
    }
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_utils_captureNode'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_utils_findChildren(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_istable(tolua_S,1,0, &tolua_err) ||
        !tolua_isusertype(tolua_S, 2, "cc.Node", 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 3, 0, &tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        cocos2d::Node* node = static_cast<Node*>(tolua_tousertype(tolua_S, 2, nullptr));
        std::string  name = tolua_tocppstring(tolua_S, 3, "");
        std::vector<Node*> children = cocos2d::utils::findChildren(*node, name);
        lua_newtable(tolua_S);
        int index = 1;
        for (const auto& obj : children)
        {
            if (nullptr == obj)
                continue;

            lua_pushnumber(tolua_S, (lua_Number)index);
            int ID = (obj) ? (int)obj->_ID : -1;
            int* luaID = (obj) ? &obj->_luaID : NULL;
            toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)obj, "cc.Node");
            lua_rawset(tolua_S, -3);
            ++index;
        }
        return 1;
    }
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_utils_findChildren'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_utils_findChild(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_istable(tolua_S, 1, 0, &tolua_err) ||
        !tolua_isusertype(tolua_S, 2, "cc.Node", 0, &tolua_err) ||
        !tolua_isstring(tolua_S, 3, 0, &tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        cocos2d::Node* node = static_cast<Node*>(tolua_tousertype(tolua_S, 2, nullptr));
        std::string name = tolua_tocppstring(tolua_S, 3, "");
        auto obj = cocos2d::utils::findChild(node, name);
        int ID = (obj) ? (int)obj->_ID : -1;
        int* luaID = (obj) ? &obj->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)obj, "cc.Node");
        return 1;
    }
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'tolua_cocos2d_utils_findChild'.", &tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_utils_gettime(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_istable(tolua_S, 1, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S, 2, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        double time = cocos2d::utils::gettime();
        lua_pushnumber(tolua_S, time);
        return 1;
    }
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S, "#ferror in function 'tolua_cocos2d_utils_gettime'.", &tolua_err);
    return 0;
#endif
}

int register_all_cocos2dx_module_manual(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    tolua_open(tolua_S);
    tolua_module(tolua_S, "cc", 0);
    tolua_beginmodule(tolua_S, "cc");
        tolua_module(tolua_S, "utils", 0);
        tolua_beginmodule(tolua_S,"utils");
            tolua_function(tolua_S, "captureScreen", tolua_cocos2d_utils_captureScreen);
            tolua_function(tolua_S, "captureNode", tolua_cocos2d_utils_captureNode);
            tolua_function(tolua_S, "findChildren", tolua_cocos2d_utils_findChildren);
            tolua_function(tolua_S, "findChild", tolua_cocos2d_utils_findChild);
            tolua_function(tolua_S, "gettime", tolua_cocos2d_utils_gettime);
        tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);

    return 0;
}

static int tolua_cocos2d_Mat4_getInversed(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_istable(tolua_S, 1, 0, &tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        cocos2d::Mat4 mat;
        bool ok = luaval_to_mat4(tolua_S, 1, &mat);
        if (ok)
        {
            mat4_to_luaval(tolua_S, mat.getInversed());
            return 1;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'mat4_getInversed'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_Mat4_transformVector(lua_State* tolua_S)
{
    bool ok = true;
    int argc = lua_gettop(tolua_S);

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 2 || argc == 3) /* Allow 3 arguments for compatibility with old version */
    {
#if COCOS2D_DEBUG >= 1

        if (!tolua_istable(tolua_S, 1, 0, &tolua_err) ||
            !tolua_istable(tolua_S, 2, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Mat4 mat;
            cocos2d::Vec4 vector;
            cocos2d::Vec4 dst;
            ok &= luaval_to_mat4(tolua_S, 1, &mat);
            if (!ok)
                return 0;

            ok &= luaval_to_vec4(tolua_S, 2, &vector);
            if (!ok)
                return 0;

            mat.transformVector(vector, &dst);
            vec4_to_luaval(tolua_S, dst);
            return 1;
        }
    }
    else if(argc == 5 || argc == 6) /* Allow 6 arguments for compatibility with old version */
    {
        /*
         float x, float y, float z, float w, Vec3* dst
         */
#if COCOS2D_DEBUG >= 1
        if (!tolua_istable(tolua_S, 1, 0, &tolua_err)  ||
            !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
            !tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
            !tolua_isnumber(tolua_S, 4, 0, &tolua_err) ||
            !tolua_isnumber(tolua_S, 5, 0, &tolua_err) )
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Mat4 mat;
            float x,y,z,w;
            cocos2d::Vec3 dst;
            ok &= luaval_to_mat4(tolua_S, 1, &mat);
            if (!ok)
                return 0;

            x = tolua_tonumber(tolua_S, 2, 0);
            y = tolua_tonumber(tolua_S, 3, 0);
            z = tolua_tonumber(tolua_S, 4, 0);
            w = tolua_tonumber(tolua_S, 5, 0);

            mat.transformVector(x,y,z,w, &dst);
            vec3_to_luaval(tolua_S, dst);
            return 1;
        }
    }

    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'mat4_transformVector'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_Mat4_decompose(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;

    if (!tolua_istable(tolua_S, 1, 0, &tolua_err) ||
        (!lua_isnil(tolua_S, 2) && !tolua_istable(tolua_S, 2, 0, &tolua_err)) ||
        (!lua_isnil(tolua_S, 3) && !tolua_istable(tolua_S, 3, 0, &tolua_err)) ||
        (!lua_isnil(tolua_S, 4) && !tolua_istable(tolua_S, 4, 0, &tolua_err)) )
        goto tolua_lerror;
    else
#endif
    {
        cocos2d::Mat4 mat;
        cocos2d::Vec3 scale;
        cocos2d::Quaternion rotation;
        cocos2d::Vec3 translation;
        bool ok = true;

        ok &= luaval_to_mat4(tolua_S, 1, &mat);
        if (!ok)
            return 0;

        if (lua_isnil(tolua_S, 2) && !lua_isnil(tolua_S, 3) && !lua_isnil(tolua_S, 4))
        {
            ok &= luaval_to_quaternion(tolua_S, 3, &rotation);
            if (!ok)
                return 0;


            ok &= luaval_to_vec3(tolua_S, 4, &translation);
            if (!ok)
                return 0;


            mat.decompose(nullptr, &rotation, &translation);

            lua_newtable(tolua_S);

            lua_pushstring(tolua_S, "scale");
            lua_pushnil(tolua_S);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "rotation");
            quaternion_to_luaval(tolua_S, rotation);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "translation");
            vec3_to_luaval(tolua_S, translation);
            lua_rawset(tolua_S, -3);

            return 1;
        }

        if (lua_isnil(tolua_S, 2) && lua_isnil(tolua_S, 3) && !lua_isnil(tolua_S, 4))
        {
            ok &= luaval_to_vec3(tolua_S, 4, &translation);
            if (!ok)
                return 0;


            mat.decompose(nullptr, nullptr, &translation);

            lua_newtable(tolua_S);

            lua_pushstring(tolua_S, "scale");
            lua_pushnil(tolua_S);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "rotation");
            lua_pushnil(tolua_S);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "translation");
            vec3_to_luaval(tolua_S, translation);
            lua_rawset(tolua_S, -3);

            return 1;
        }

        if (!lua_isnil(tolua_S, 2) && lua_isnil(tolua_S, 3) && !lua_isnil(tolua_S, 4))
        {
            ok &= luaval_to_vec3(tolua_S, 2, &scale);
            if (!ok)
                return 0;

            ok &= luaval_to_vec3(tolua_S, 4, &translation);
            if (!ok)
                return 0;

            mat.decompose(&scale, nullptr, &translation);

            lua_newtable(tolua_S);

            lua_pushstring(tolua_S, "scale");
            vec3_to_luaval(tolua_S, scale);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "rotation");
            lua_pushnil(tolua_S);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "translation");
            vec3_to_luaval(tolua_S, translation);
            lua_rawset(tolua_S, -3);

            return 1;
        }

        if (!lua_isnil(tolua_S, 2) && lua_isnil(tolua_S, 3) && lua_isnil(tolua_S, 4))
        {
            ok &= luaval_to_vec3(tolua_S, 2, &scale);
            if (!ok)
                return 0;


            mat.decompose(&scale, nullptr, nullptr);

            lua_newtable(tolua_S);

            lua_pushstring(tolua_S, "scale");
            vec3_to_luaval(tolua_S, scale);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "rotation");
            lua_pushnil(tolua_S);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "translation");
            lua_pushnil(tolua_S);
            lua_rawset(tolua_S, -3);

            return 1;
        }

        if (!lua_isnil(tolua_S, 2) && !lua_isnil(tolua_S, 3) && lua_isnil(tolua_S, 4))
        {
            ok &= luaval_to_vec3(tolua_S, 2, &scale);
            if (!ok)
                return 0;

            ok &= luaval_to_quaternion(tolua_S, 3, &rotation);
            if (!ok)
                return 0;

            mat.decompose(&scale, &rotation, nullptr);

            lua_newtable(tolua_S);

            lua_pushstring(tolua_S, "scale");
            vec3_to_luaval(tolua_S, scale);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "rotation");
            quaternion_to_luaval(tolua_S, rotation);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "translation");
            lua_pushnil(tolua_S);
            lua_rawset(tolua_S, -3);

            return 1;

        }

        if (lua_isnil(tolua_S, 2) && !lua_isnil(tolua_S, 3) && lua_isnil(tolua_S, 4))
        {
            ok &= luaval_to_quaternion(tolua_S, 3, &rotation);
            if (!ok)
                return 0;

            mat.decompose(nullptr, &rotation, nullptr);

            lua_newtable(tolua_S);

            lua_pushstring(tolua_S, "scale");
            lua_pushnil(tolua_S);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "rotation");
            quaternion_to_luaval(tolua_S, rotation);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "translation");
            lua_pushnil(tolua_S);
            lua_rawset(tolua_S, -3);
        }

        if (!lua_isnil(tolua_S, 2) && !lua_isnil(tolua_S, 3) && !lua_isnil(tolua_S, 4))
        {
            ok &= luaval_to_vec3(tolua_S, 2, &scale);
            if (!ok)
                return 0;

            ok &= luaval_to_quaternion(tolua_S, 3, &rotation);
            if (!ok)
                return 0;

            ok &= luaval_to_vec3(tolua_S, 4, &translation);
            if (!ok)
                return 0;

            mat.decompose(&scale, &rotation, &translation);

            lua_newtable(tolua_S);

            lua_pushstring(tolua_S, "scale");
            vec3_to_luaval(tolua_S, scale);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "rotation");
            quaternion_to_luaval(tolua_S, rotation);
            lua_rawset(tolua_S, -3);

            lua_pushstring(tolua_S, "translation");
            vec3_to_luaval(tolua_S, translation);
            lua_rawset(tolua_S, -3);

            return 1;
        }

        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'mat4_decompose'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_Vec3_cross(lua_State* tolua_S)
{
    int argc = lua_gettop(tolua_S);

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

    if (2 == argc)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_istable(tolua_S, 1, 0, &tolua_err) ||
            !tolua_istable(tolua_S, 2, 0, &tolua_err) )
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Vec3 cobj;
            cocos2d::Vec3 v;

            bool ok = true;

            ok &= luaval_to_vec3(tolua_S, 1, &cobj);
            if (!ok)
                return 0;

            ok &= luaval_to_vec3(tolua_S, 2, &v);
            if (!ok)
                return 0;

            cobj.cross(v);

            vec3_to_luaval(tolua_S, cobj);
            return 1;
        }
    }
    else if (3 == argc)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_istable(tolua_S, 1, 0, &tolua_err) ||
            !tolua_istable(tolua_S, 2, 0, &tolua_err) ||
            !tolua_istable(tolua_S, 3, 0, &tolua_err) )
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Vec3 v1;
            cocos2d::Vec3 v2;
            cocos2d::Vec3 dst;
            bool ok = true;


            ok &= luaval_to_vec3(tolua_S, 1, &v1);
            if (!ok)
                return 0;

            ok &= luaval_to_vec3(tolua_S, 2, &v2);
            if (!ok)
                return 0;

            ok &= luaval_to_vec3(tolua_S, 3, &dst);
            if (!ok)
                return 0;

            cocos2d::Vec3::cross(v1, v2, &dst);

            vec3_to_luaval(tolua_S, dst);
            return 1;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'MathUtil_gcrossVec3'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_Mat4_multiply(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_istable(tolua_S, 1, 0, &tolua_err) ||
        !tolua_istable(tolua_S, 2, 0, &tolua_err) )
        goto tolua_lerror;
    else
#endif
    {
        cocos2d::Mat4 mat1;
        bool ok = luaval_to_mat4(tolua_S, 1, &mat1);
        if(!ok)
            return 0;

        cocos2d::Mat4 mat2;
        ok = luaval_to_mat4(tolua_S, 2, &mat2);
        if(!ok)
            return 0;

        cocos2d::Mat4 ret = mat1 * mat2;
        mat4_to_luaval(tolua_S, ret);
        return 1;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'mat4_multiply'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_Mat4_translate(lua_State* tolua_S)
{
    bool ok = true;
    int argc = lua_gettop(tolua_S);

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1

        if (!tolua_istable(tolua_S, 1, 0, &tolua_err) ||
            !tolua_istable(tolua_S, 2, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Mat4 mat;
            cocos2d::Vec3 vec3;
            ok &= luaval_to_mat4(tolua_S, 1, &mat);
            if (!ok)
                return 0;

            ok &= luaval_to_vec3(tolua_S, 2, &vec3);
            if (!ok)
                return 0;

            mat.translate(vec3);
            mat4_to_luaval(tolua_S, mat);
            return 1;
        }
    }

    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Mat4_translate'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_Mat4_createRotationZ(lua_State* tolua_S)
{
    bool ok = true;
    int argc = lua_gettop(tolua_S);

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1

        if (!tolua_istable(tolua_S, 1, 0, &tolua_err) ||
            !tolua_isnumber(tolua_S, 2, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Mat4 mat;
            float angle;
            ok &= luaval_to_mat4(tolua_S, 1, &mat);
            if (!ok)
                return 0;
            angle = (float)lua_tonumber(tolua_S, 2);
            cocos2d::Mat4::createRotationZ(angle, &mat);
            mat4_to_luaval(tolua_S, mat);
            return 1;
        }
    }

    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Mat4_createRotationZ'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_Mat4_setIdentity(lua_State* tolua_S)
{
    bool ok = true;
    int argc = lua_gettop(tolua_S);

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1

        if (!tolua_istable(tolua_S, 1, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Mat4 mat;
            ok &= luaval_to_mat4(tolua_S, 1, &mat);
            if (!ok)
                return 0;
            mat.setIdentity();
            mat4_to_luaval(tolua_S, mat);
            return 1;
        }
    }

    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Mat4_setIdentity'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_Mat4_createTranslation(lua_State* tolua_S)
{
    bool ok = true;
    int argc = lua_gettop(tolua_S);

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 3)
    {
#if COCOS2D_DEBUG >= 1

        if ( !tolua_isnumber(tolua_S, 1, 0, &tolua_err) ||
             !tolua_isnumber(tolua_S, 2, 0, &tolua_err)  ||
             !tolua_isnumber(tolua_S, 3, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Mat4 dst;
            float xTranslation = (float)lua_tonumber(tolua_S, 1);
            float yTranslation = (float)lua_tonumber(tolua_S, 2);
            float zTranslation = (float)lua_tonumber(tolua_S, 3);

            cocos2d::Mat4::createTranslation(xTranslation, yTranslation, zTranslation, &dst);
            mat4_to_luaval(tolua_S, dst);
            return 1;
        }
    }
    else if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if ( !tolua_istable(tolua_S, 1, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Vec3 translation;
            cocos2d::Mat4 dst;

            ok &= luaval_to_vec3(tolua_S, 1, &translation, "cc.Mat4.createTranslation");
            if (!ok)
                return 0;

            cocos2d::Mat4::createTranslation(translation, &dst);
            mat4_to_luaval(tolua_S, dst);
            return 1;
        }
    }

    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Mat4_createTranslation'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_Mat4_createRotation(lua_State* tolua_S)
{
    bool ok = true;
    int argc = lua_gettop(tolua_S);

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1

        if ( !tolua_istable(tolua_S, 1, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Quaternion quat;
            cocos2d::Mat4 dst;

            ok &= luaval_to_quaternion(tolua_S, 1, &quat, "cc.Mat4.createRotation");
            if (!ok)
                return 0;

            cocos2d::Mat4::createRotation(quat, &dst);
            mat4_to_luaval(tolua_S, dst);
            return 1;
        }
    }
    else if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1

        if ( !tolua_istable(tolua_S, 1, 0, &tolua_err) ||
             !tolua_isnumber(tolua_S, 2, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Vec3 axis;
            cocos2d::Mat4 dst;

            ok &= luaval_to_vec3(tolua_S, 1, &axis, "cc.Mat4.createRotation");
            if (!ok)
                return 0;

            float angle = (float)tolua_tonumber(tolua_S, 2, 0);

            cocos2d::Mat4::createRotation(axis, angle, &dst);
            mat4_to_luaval(tolua_S, dst);
            return 1;
        }
    }

    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_Mat4_createRotation'.",&tolua_err);
    return 0;
#endif
}

int register_all_cocos2dx_math_manual(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;


    tolua_module(tolua_S, nullptr, 0);
    tolua_beginmodule(tolua_S, nullptr);
        tolua_function(tolua_S, "mat4_getInversed", tolua_cocos2d_Mat4_getInversed);
        tolua_function(tolua_S, "mat4_transformVector", tolua_cocos2d_Mat4_transformVector);
        tolua_function(tolua_S, "mat4_decompose", tolua_cocos2d_Mat4_decompose);
        tolua_function(tolua_S, "mat4_multiply", tolua_cocos2d_Mat4_multiply);
        tolua_function(tolua_S, "mat4_translate", tolua_cocos2d_Mat4_translate);
        tolua_function(tolua_S, "mat4_createRotationZ", tolua_cocos2d_Mat4_createRotationZ);
        tolua_function(tolua_S, "mat4_setIdentity", tolua_cocos2d_Mat4_setIdentity);
        tolua_function(tolua_S, "mat4_createTranslation", tolua_cocos2d_Mat4_createTranslation);
        tolua_function(tolua_S, "mat4_createRotation", tolua_cocos2d_Mat4_createRotation);
        tolua_function(tolua_S, "vec3_cross", tolua_cocos2d_Vec3_cross);
    tolua_endmodule(tolua_S);
    return 0;
}


int register_all_cocos2dx_shaders_manual(lua_State *tolua_S) 
{
    if (nullptr == tolua_S)
        return 0;
#define set_lua_field(field) do {\
    lua_pushstring(tolua_S, #field); \
    lua_pushstring(tolua_S, field); \
    lua_rawset(tolua_S, -3); \
    } while(false)

    tolua_open(tolua_S);
    tolua_module(tolua_S, "cc", 0);
    tolua_beginmodule(tolua_S, "cc");
        set_lua_field(positionColor_vert);
        set_lua_field(positionColor_frag);
        set_lua_field(positionTexture_vert);
        set_lua_field(positionTexture_frag);
        set_lua_field(positionTextureColor_vert);
        set_lua_field(positionTextureColor_frag);
        set_lua_field(positionTextureColorAlphaTest_frag);
        set_lua_field(label_normal_frag);
        set_lua_field(label_distanceNormal_frag);
        set_lua_field(labelOutline_frag);
        set_lua_field(labelDistanceFieldGlow_frag);
        set_lua_field(positionColorLengthTexture_vert);
        set_lua_field(positionColorLengthTexture_frag);
        set_lua_field(positionColorTextureAsPointsize_vert);
        set_lua_field(position_vert);
        set_lua_field(positionNoMVP_vert);
        set_lua_field(layer_radialGradient_frag);
        set_lua_field(grayScale_frag);
        set_lua_field(positionTextureUColor_vert);
        set_lua_field(positionTextureUColor_frag);
        set_lua_field(positionUColor_vert);
        set_lua_field(positionUColor_frag);
        set_lua_field(etc1_frag);
        set_lua_field(etc1Gray_frag);
        set_lua_field(cameraClear_vert);
        set_lua_field(cameraClear_frag);
    tolua_endmodule(tolua_S);
    return 0;
}

static int tolua_cocos2d_bytearray_vec2(lua_State *L)
{
    bool ok = true;
    int argc = lua_gettop(L);
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        
        if (!tolua_istable(L, 1, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Vec2 arg;
            ok &= luaval_to_vec2(L, 1, &arg, "tolua_cocos2d_bytearray_template");
            if (!ok)
                return 0;
            
            lua_pop(L, 1);
            lua_newtable(L);
            uint8_t *bytes = (uint8_t*)&arg;
            for (auto idx = 0; idx < sizeof(arg); idx++)
            {
                lua_pushnumber(L, bytes[idx]);
                lua_rawseti(L, 1, idx + 1);
            }
            return 1;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'tolua_cocos2d_bytearray_template'.", &tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_bytearray_vec3(lua_State *L)
{
    bool ok = true;
    int argc = lua_gettop(L);
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        
        if (!tolua_istable(L, 1, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Vec3 arg;
            ok &= luaval_to_vec3(L, 1, &arg, "tolua_cocos2d_bytearray_template");
            if (!ok)
                return 0;
            
            lua_pop(L, 1);
            lua_newtable(L);
            uint8_t *bytes = (uint8_t*)&arg;
            for (auto idx = 0; idx < sizeof(arg); idx++)
            {
                lua_pushnumber(L, bytes[idx]);
                lua_rawseti(L, 1, idx + 1);
            }
            return 1;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'tolua_cocos2d_bytearray_template'.", &tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_bytearray_vec4(lua_State *L)
{
    bool ok = true;
    int argc = lua_gettop(L);
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        
        if (!tolua_istable(L, 1, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Vec4 arg;
            ok &= luaval_to_vec4(L, 1, &arg, "tolua_cocos2d_bytearray_template");
            if (!ok)
                return 0;
            
            lua_pop(L, 1);
            lua_newtable(L);
            uint8_t *bytes = (uint8_t*)&arg;
            for (auto idx = 0; idx < sizeof(arg); idx++)
            {
                lua_pushnumber(L, bytes[idx]);
                lua_rawseti(L, 1, idx + 1);
            }
            return 1;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'tolua_cocos2d_bytearray_template'.", &tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_bytearray_mat4(lua_State *L)
{
    bool ok = true;
    int argc = lua_gettop(L);
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        
        if (!tolua_istable(L, 1, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            cocos2d::Mat4 arg;
            ok &= luaval_to_mat4(L, 1, &arg, "tolua_cocos2d_bytearray_template");
            if (!ok)
                return 0;
            
            lua_pop(L, 1);
            lua_newtable(L);
            uint8_t *bytes = (uint8_t*)&arg;
            for (auto idx = 0; idx < sizeof(arg); idx++)
            {
                lua_pushnumber(L, bytes[idx]);
                lua_rawseti(L, 1, idx + 1);
            }
            return 1;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'tolua_cocos2d_bytearray_template'.", &tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_bytearray_int(lua_State *L)
{
    bool ok = true;
    int argc = lua_gettop(L);
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        
        if (!tolua_isnumber(L, 1, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            int arg;
            ok &= luaval_to_int32(L, 1, &arg, "tolua_cocos2d_bytearray_template");
            if (!ok)
                return 0;
            
            lua_pop(L, 1);
            lua_newtable(L);
            uint8_t *bytes = (uint8_t*)&arg;
            for (auto idx = 0; idx < sizeof(arg); idx++)
            {
                lua_pushnumber(L, bytes[idx]);
                lua_rawseti(L, 1, idx + 1);
            }
            return 1;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'tolua_cocos2d_bytearray_template'.", &tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_bytearray_float(lua_State *L)
{
    bool ok = true;
    int argc = lua_gettop(L);
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        
        if (!tolua_isnumber(L, 1, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            float arg;
            ok &= luaval_to_float(L, 1, &arg, "tolua_cocos2d_bytearray_template");
            if (!ok)
                return 0;
            
            lua_pop(L, 1);
            lua_newtable(L);
            uint8_t *bytes = (uint8_t*)&arg;
            for (auto idx = 0; idx < sizeof(arg); idx++)
            {
                lua_pushnumber(L, bytes[idx]);
                lua_rawseti(L, 1, idx + 1);
            }
            return 1;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'tolua_cocos2d_bytearray_template'.", &tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_bytearray_intv(lua_State *L)
{
    int argc = lua_gettop(L);

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1

        if (!tolua_istable(L, 1, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            int size = lua_objlen(L, 1);
            std::vector<uint8_t> arg(size * sizeof(int));

            int e;
            int *p = (int*)arg.data();

            for (auto idx = 0; idx < size; idx++)
            {
                lua_rawgeti(L, 1, idx + 1);
                luaval_to_int32(L, -1, &e, "tolua_cocos2d_bytearray_elev");
                p[idx] = e;
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
            lua_newtable(L);
            for (size_t idx = 0; idx < arg.size(); idx++)
            {
                lua_pushnumber(L, arg[idx]);
                lua_rawseti(L, 1, idx + 1);
            }
            return 1;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
                tolua_error(L, "#ferror in function 'tolua_cocos2d_bytearray_elev'.", &tolua_err);
                return 0;
#endif
}

static int tolua_cocos2d_bytearray_floatv(lua_State *L)
{
    int argc = lua_gettop(L);
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        
        if (!tolua_istable(L, 1, 0, &tolua_err))
            goto tolua_lerror;
        else
#endif
        {
            int size = lua_objlen(L, 1);
            std::vector<uint8_t> arg(size * sizeof(float));
            
            float e;
            float *p = (float*)arg.data();
            
            for (auto idx = 0; idx < size; idx++)
            {
                lua_rawgeti(L, 1, idx + 1);
                luaval_to_float(L, -1, &e, "tolua_cocos2d_bytearray_elev");
                p[idx] = e;
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
            lua_newtable(L);
            for (auto idx = 0; idx < arg.size(); idx++)
            {
                lua_pushnumber(L, arg[idx]);
                lua_rawseti(L, 1, idx + 1);
            }
            return 1;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L, "#ferror in function 'tolua_cocos2d_bytearray_elev'.", &tolua_err);
    return 0;
#endif
}

int register_all_cocos2dx_bytearray_manual(lua_State *tolua_S)
{
    if (nullptr == tolua_S)
        return 0;

    tolua_module(tolua_S, "cc", 0);
    tolua_beginmodule(tolua_S, "cc");
    tolua_module(tolua_S, "bytearray",0);
    tolua_beginmodule(tolua_S, "bytearray");
        tolua_function(tolua_S, "from_vec2", tolua_cocos2d_bytearray_vec2);
        tolua_function(tolua_S, "from_vec3", tolua_cocos2d_bytearray_vec3);
        tolua_function(tolua_S, "from_vec4", tolua_cocos2d_bytearray_vec4);
        tolua_function(tolua_S, "from_mat4", tolua_cocos2d_bytearray_mat4);
        tolua_function(tolua_S, "from_int", tolua_cocos2d_bytearray_int);
        tolua_function(tolua_S, "from_float", tolua_cocos2d_bytearray_float);
        tolua_function(tolua_S, "from_intv", tolua_cocos2d_bytearray_intv);
        tolua_function(tolua_S, "from_floatv", tolua_cocos2d_bytearray_floatv);
    tolua_endmodule(tolua_S);
    tolua_endmodule(tolua_S);
    return 0;
}

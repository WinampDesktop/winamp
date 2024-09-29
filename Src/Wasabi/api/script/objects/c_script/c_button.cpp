	/* This file was generated by Maki Compiler, do not edit manually */
	
	#include <precomp.h>
	
	#include "c_button.h"
	#include <api/script/objcontroller.h>
	
	C_Button::C_Button(ScriptObject *object) : C_GuiObject(object) {
    inited = 0;
    C_hook(object);
	}
	
	C_Button::C_Button() {
    inited = 0;
    obj = NULL;
	}
	
	void C_Button::C_hook(ScriptObject *object) {
    ASSERT(!inited);
    ScriptObjectController *controller = object->vcpu_getController();
    obj = controller->cast(object, buttonGuid);
    if (obj != object && obj != NULL)
controller = obj->vcpu_getController();
    else
obj = NULL;
	
    int iter = WASABI_API_APP->app_getInitCount();
    if (!loaded || loaded != iter) {
loaded = iter;
onactivate_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onActivate", this);
onleftclick_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onLeftClick", this);
onrightclick_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onRightClick", this);
setactivated_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"setActivated", this);
setactivatednocallback_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"setActivatedNoCallback", this);
getactivated_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"getActivated", this);
leftclick_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"leftClick", this);
rightclick_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"rightClick", this);
    }
    inited = 1;
	}
	
	C_Button::~C_Button() {
	}
	
	ScriptObject *C_Button::getScriptObject() {
    if (obj != NULL) return obj;
    return C_BUTTON_PARENT::getScriptObject();
	}
	
	void C_Button::onActivate(int activated) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_INT(activated);
    scriptVar *params[1] = {&a};
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), onactivate_id, params);
	}
	
	void C_Button::onLeftClick() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), onleftclick_id, NULL);
	}
	
	void C_Button::onRightClick() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), onrightclick_id, NULL);
	}
	
	void C_Button::setActivated(int onoff) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_INT(onoff);
    scriptVar *params[1] = {&a};
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), setactivated_id, params);
	}
	
	void C_Button::setActivatedNoCallback(int onoff) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_INT(onoff);
    scriptVar *params[1] = {&a};
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), setactivatednocallback_id, params);
	}
	
	int C_Button::getActivated() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), getactivated_id, NULL));
	}
	
	void C_Button::leftClick() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), leftclick_id, NULL);
	}
	
	void C_Button::rightClick() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), rightclick_id, NULL);
	}
	
	int C_Button::loaded=0;
	int C_Button::onactivate_id=0;
	int C_Button::onleftclick_id=0;
	int C_Button::onrightclick_id=0;
	int C_Button::setactivated_id=0;
	int C_Button::setactivatednocallback_id=0;
	int C_Button::getactivated_id=0;
	int C_Button::leftclick_id=0;
	int C_Button::rightclick_id=0;
	
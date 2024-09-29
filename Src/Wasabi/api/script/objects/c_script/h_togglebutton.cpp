	/* This file was generated by Maki Compiler, do not edit manually */
	
	#include <precomp.h>
	
	#include <api/script/objcontroller.h>
	#include "h_togglebutton.h"
	
	H_ToggleButton::H_ToggleButton(ScriptObject *o) : H_Button(o) {
    inited = 0;
    obj = NULL;
    H_hook(o);
	}
	
	H_ToggleButton::H_ToggleButton() {
    inited = 0;
    obj = NULL;
	}
	
	void H_ToggleButton::H_hook(ScriptObject *o) {
    ASSERT(!inited);
    ScriptObjectController *controller = o->vcpu_getController();
    obj = controller->cast(o, toggleButtonGuid);
    if (obj != o && obj != NULL)
o = obj;
    else
obj = NULL;
	
    addMonitorObject(o, &toggleButtonGuid);
	
    int iter = WASABI_API_APP->app_getInitCount();
    if (!loaded || loaded != iter) {
ontoggle_id= WASABI_API_MAKI->maki_addDlfRef(o->vcpu_getController(), L"onToggle", this);
loaded = 1;
    }
    inited=1;
	}
	
	H_ToggleButton::~H_ToggleButton() {
    if (!inited) return;
    WASABI_API_MAKI->maki_remDlfRef(this);
	}
	
	ScriptObject *H_ToggleButton::getHookedObject() {
    if (obj != NULL) return obj;
    return H_TOGGLEBUTTON_PARENT::getHookedObject();
	}
	
	int H_ToggleButton::eventCallback(ScriptObject *object, int dlfid, scriptVar **params, int nparams) {
    if (object != getHookedObject()) return 0;
    if (H_TOGGLEBUTTON_PARENT::eventCallback(object, dlfid, params, nparams)) return 1;
    if (dlfid == ontoggle_id) { hook_onToggle(GET_SCRIPT_INT(*params[0])); return 1; }
    return 0;
	}
	
	int H_ToggleButton::ontoggle_id=0;
	int H_ToggleButton::loaded=0;
	
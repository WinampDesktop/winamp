	/* This file was generated by Maki Compiler, do not edit manually */
	
	#include <precomp.h>
	#include <api/api.h>
	#include "c_grouplist.h"
	#include <api/script/objcontroller.h>
	
	C_GroupList::C_GroupList(ScriptObject *object) : C_GuiObject(object) {
    inited = 0;
    obj = NULL;
    C_hook(object);
	}
	
	C_GroupList::C_GroupList() {
    inited = 0;
    obj = NULL;
	}
	
	void C_GroupList::C_hook(ScriptObject *object) {
    ASSERT(!inited);
    ScriptObjectController *controller = object->vcpu_getController();
    obj = controller->cast(object, groupListGuid);
    if (obj != object && obj != NULL)
controller = obj->vcpu_getController();
    else
obj = NULL;
	
    int iter = WASABI_API_APP->app_getInitCount();
    if (!loaded || loaded != iter) {
loaded = iter;
instantiate_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"instantiate", this);
getnumitems_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"getNumItems", this);
enumitem_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"enumItem", this);
removeall_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"removeAll", this);
scrolltopercent_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"scrollToPercent", this);
    }
    inited = 1;
	}
	
	C_GroupList::~C_GroupList() {
	}
	
	ScriptObject *C_GroupList::getScriptObject() {
    if (obj != NULL) return obj;
    return C_GROUPLIST_PARENT::getScriptObject();
	}
	
	ScriptObject *C_GroupList::instantiate(const char *group_id, int num_groups) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_STRING(group_id);
    scriptVar b = MAKE_SCRIPT_INT(num_groups);
    scriptVar *params[2] = {&a, &b};
    return GET_SCRIPT_OBJECT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), instantiate_id, params));
	}
	
	int C_GroupList::getNumItems() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), getnumitems_id, NULL));
	}
	
	ScriptObject *C_GroupList::enumItem(int num) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_INT(num);
    scriptVar *params[1] = {&a};
    return GET_SCRIPT_OBJECT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), enumitem_id, params));
	}
	
	void C_GroupList::removeAll() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), removeall_id, NULL);
	}
	
	void C_GroupList::scrollToPercent(int percent) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_INT(percent);
    scriptVar *params[1] = {&a};
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), scrolltopercent_id, params);
	}
	
	int C_GroupList::loaded=0;
	int C_GroupList::instantiate_id=0;
	int C_GroupList::getnumitems_id=0;
	int C_GroupList::enumitem_id=0;
	int C_GroupList::removeall_id=0;
	int C_GroupList::scrolltopercent_id=0;
	
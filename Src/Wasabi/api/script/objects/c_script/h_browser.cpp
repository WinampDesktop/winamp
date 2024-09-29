		/* This file was generated by Maki Compiler, do not edit manually */
		
		#include <precomp.h>
		
		#include <api/script/objcontroller.h>
		#include "h_browser.h"
		
		H_Browser::H_Browser(ScriptObject *o) : H_GuiObject(o)
		{
	inited = 0;
	obj = NULL;
	H_hook(o);
		}
		
		H_Browser::H_Browser()
		{
	inited = 0;
	obj = NULL;
		}
		
		void H_Browser::H_hook(ScriptObject *o)
		{
	ASSERT(!inited);
	ScriptObjectController *controller = o->vcpu_getController();
	obj = controller->cast(o, browserGuid);
	if (obj != o && obj != NULL)
o = obj;
	else
obj = NULL;
		
	addMonitorObject(o, &browserGuid);
		
	int iter = WASABI_API_APP->app_getInitCount();
	if (!loaded || loaded != iter)
	{
onbeforenavigate_id= WASABI_API_MAKI->maki_addDlfRef(o->vcpu_getController(), L"onBeforeNavigate", this);
ondocumentcomplete_id= WASABI_API_MAKI->maki_addDlfRef(o->vcpu_getController(), L"onDocumentComplete", this);
onmedialink_id= WASABI_API_MAKI->maki_addDlfRef(o->vcpu_getController(), L"onMediaLink", this);
loaded = 1;
	}
	inited=1;
		}
		
		H_Browser::~H_Browser()
		{
	if (!inited) return;
	WASABI_API_MAKI->maki_remDlfRef(this);
		}
		
		ScriptObject *H_Browser::getHookedObject()
		{
	if (obj != NULL) return obj;
	return H_BROWSER_PARENT::getHookedObject();
		}
		
		int H_Browser::eventCallback(ScriptObject *object, int dlfid, scriptVar **params, int nparams)
		{
	if (object != getHookedObject()) return 0;
	if (H_BROWSER_PARENT::eventCallback(object, dlfid, params, nparams)) return 1;
		
	if (dlfid == onbeforenavigate_id)
	{
hook_onBeforeNavigate(GET_SCRIPT_STRING(*params[0]), GET_SCRIPT_INT(*params[1]), GET_SCRIPT_STRING(*params[2]));
return 1;
	}
	else if (dlfid == ondocumentcomplete_id)
	{
hook_onDocumentComplete(GET_SCRIPT_STRING(*params[0]));
return 1;
	}
	else if (dlfid == onmedialink_id)
	{
		
hook_onMediaLink(GET_SCRIPT_STRING(*params[0]));
return 1;
	}
		
	return 0;
		
		}
		
		int H_Browser::onbeforenavigate_id=0;
		int H_Browser::ondocumentcomplete_id=0;
		int H_Browser::onmedialink_id=0;
		int H_Browser::loaded=0;
		
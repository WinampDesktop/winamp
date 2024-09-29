/* This file was generated by Maki Compiler, do not edit manually */

#ifndef __HOOK_BUTTON_H
#define __HOOK_BUTTON_H

#include "h_guiobject.h"

#define H_BUTTON_PARENT H_GuiObject

class H_Button : public H_BUTTON_PARENT {

public:

H_Button(ScriptObject *o);
H_Button();
virtual ~H_Button();
virtual void H_hook(ScriptObject *o);
ScriptObject *getHookedObject();

virtual int eventCallback(ScriptObject *object, int dlfid, scriptVar **params, int nparams);
virtual void hook_onActivate(int activated) {  }
virtual void hook_onLeftClick() {  }
virtual void hook_onRightClick() {  }

private:

ScriptObject *obj;
int inited;
static int loaded;
static int onactivate_id;
static int onleftclick_id;
static int onrightclick_id;
};

#endif

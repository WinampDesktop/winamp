/* This file was generated by Maki Compiler, do not edit manually */

#ifndef __HOOK_QUERYLIST_H
#define __HOOK_QUERYLIST_H

#include "h_guiobject.h"

#define H_QUERYLIST_PARENT H_GuiObject

class H_QueryList : public H_QUERYLIST_PARENT {

public:

H_QueryList(ScriptObject *o);
H_QueryList();
virtual ~H_QueryList();
virtual void H_hook(ScriptObject *o);
ScriptObject *getHookedObject();

virtual int eventCallback(ScriptObject *object, int dlfid, scriptVar **params, int nparams);
virtual void hook_onResetQuery() {  }

private:

ScriptObject *obj;
int inited;
static int loaded;
static int onresetquery_id;
};

#endif

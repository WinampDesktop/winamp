	/* This file was generated by Maki Compiler, do not edit manually */
	
	#include <precomp.h>
	
	#include "c_treeitem.h"
	#include <api/script/objcontroller.h>
	
	C_TreeItem::C_TreeItem(ScriptObject *object) : C_RootObject(object) {
    inited = 0;
    obj = NULL;
    C_hook(object);
	}
	
	C_TreeItem::C_TreeItem() {
    inited = 0;
    obj = NULL;
	}
	
	void C_TreeItem::C_hook(ScriptObject *object) {
    ASSERT(!inited);
    ScriptObjectController *controller = object->vcpu_getController();
    obj = controller->cast(object, treeItemGuid);
    if (obj != object && obj != NULL)
controller = obj->vcpu_getController();
    else
obj = NULL;
	
    int iter = WASABI_API_APP->app_getInitCount();
    if (!loaded || loaded != iter) {
loaded = iter;
getnumchildren_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"getNumChildren", this);
setlabel_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"setLabel", this);
getlabel_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"getLabel", this);
ensurevisible_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"ensureVisible", this);
getnthchild_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"getNthChild", this);
getchild_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"getChild", this);
getchildsibling_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"getChildSibling", this);
getsibling_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"getSibling", this);
getparent_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"getParent", this);
editlabel_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"editLabel", this);
hassubitems_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"hasSubItems", this);
setsorted_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"setSorted", this);
setchildtab_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"setChildTab", this);
issorted_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"isSorted", this);
iscollapsed_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"isCollapsed", this);
isexpanded_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"isExpanded", this);
invalidate_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"invalidate", this);
isselected_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"isSelected", this);
ishilited_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"isHilited", this);
sethilited_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"setHilited", this);
collapse_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"collapse", this);
expand_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"expand", this);
gettree_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"getTree", this);
ontreeadd_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onTreeAdd", this);
ontreeremove_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onTreeRemove", this);
onselect_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onSelect", this);
ondeselect_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onDeselect", this);
onleftdoubleclick_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onLeftDoubleClick", this);
onrightdoubleclick_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onRightDoubleClick", this);
onchar_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onChar", this);
onexpand_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onExpand", this);
oncollapse_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onCollapse", this);
onbeginlabeledit_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onBeginLabelEdit", this);
onendlabeledit_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onEndLabelEdit", this);
oncontextmenu_id = WASABI_API_MAKI->maki_addDlfRef(controller, L"onContextMenu", this);
    }
    inited = 1;
	}
	
	C_TreeItem::~C_TreeItem() {
	}
	
	ScriptObject *C_TreeItem::getScriptObject() {
    if (obj != NULL) return obj;
    return C_TREEITEM_PARENT::getScriptObject();
	}
	
	int C_TreeItem::getNumChildren() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), getnumchildren_id, NULL));
	}
	
	void C_TreeItem::setLabel(const wchar_t *label) 
	{
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_STRING(label);
    scriptVar *params[1] = {&a};
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), setlabel_id, params);
	}
	
	const wchar_t *C_TreeItem::getLabel() 
	{
    ASSERT(inited);
    return GET_SCRIPT_STRING(WASABI_API_MAKI->maki_callFunction(getScriptObject(), getlabel_id, NULL));
	}
	
	void C_TreeItem::ensureVisible() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), ensurevisible_id, NULL);
	}
	
	ScriptObject *C_TreeItem::getNthChild(int nth) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_INT(nth);
    scriptVar *params[1] = {&a};
    return GET_SCRIPT_OBJECT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), getnthchild_id, params));
	}
	
	ScriptObject *C_TreeItem::getChild() {
    ASSERT(inited);
    return GET_SCRIPT_OBJECT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), getchild_id, NULL));
	}
	
	ScriptObject *C_TreeItem::getChildSibling(ScriptObject *_item) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_OBJECT(_item);
    scriptVar *params[1] = {&a};
    return GET_SCRIPT_OBJECT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), getchildsibling_id, params));
	}
	
	ScriptObject *C_TreeItem::getSibling() {
    ASSERT(inited);
    return GET_SCRIPT_OBJECT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), getsibling_id, NULL));
	}
	
	ScriptObject *C_TreeItem::getParent() {
    ASSERT(inited);
    return GET_SCRIPT_OBJECT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), getparent_id, NULL));
	}
	
	void C_TreeItem::editLabel() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), editlabel_id, NULL);
	}
	
	int C_TreeItem::hasSubItems() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), hassubitems_id, NULL));
	}
	
	void C_TreeItem::setSorted(int issorted) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_INT(issorted);
    scriptVar *params[1] = {&a};
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), setsorted_id, params);
	}
	
	void C_TreeItem::setChildTab(int haschildtab) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_INT(haschildtab);
    scriptVar *params[1] = {&a};
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), setchildtab_id, params);
	}
	
	int C_TreeItem::isSorted() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), issorted_id, NULL));
	}
	
	int C_TreeItem::isCollapsed() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), iscollapsed_id, NULL));
	}
	
	int C_TreeItem::isExpanded() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), isexpanded_id, NULL));
	}
	
	void C_TreeItem::invalidate() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), invalidate_id, NULL);
	}
	
	int C_TreeItem::isSelected() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), isselected_id, NULL));
	}
	
	int C_TreeItem::isHilited() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), ishilited_id, NULL));
	}
	
	void C_TreeItem::setHilited(int ishilited) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_INT(ishilited);
    scriptVar *params[1] = {&a};
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), sethilited_id, params);
	}
	
	int C_TreeItem::collapse() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), collapse_id, NULL));
	}
	
	int C_TreeItem::expand() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), expand_id, NULL));
	}
	
	ScriptObject *C_TreeItem::getTree() {
    ASSERT(inited);
    return GET_SCRIPT_OBJECT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), gettree_id, NULL));
	}
	
	void C_TreeItem::onTreeAdd() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), ontreeadd_id, NULL);
	}
	
	void C_TreeItem::onTreeRemove() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), ontreeremove_id, NULL);
	}
	
	void C_TreeItem::onSelect() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), onselect_id, NULL);
	}
	
	void C_TreeItem::onDeselect() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), ondeselect_id, NULL);
	}
	
	int C_TreeItem::onLeftDoubleClick() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), onleftdoubleclick_id, NULL));
	}
	
	int C_TreeItem::onRightDoubleClick() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), onrightdoubleclick_id, NULL));
	}
	
	int C_TreeItem::onChar(int key) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_INT(key);
    scriptVar *params[1] = {&a};
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), onchar_id, params));
	}
	
	void C_TreeItem::onExpand() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), onexpand_id, NULL);
	}
	
	void C_TreeItem::onCollapse() {
    ASSERT(inited);
    WASABI_API_MAKI->maki_callFunction(getScriptObject(), oncollapse_id, NULL);
	}
	
	int C_TreeItem::onBeginLabelEdit() {
    ASSERT(inited);
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), onbeginlabeledit_id, NULL));
	}
	
	int C_TreeItem::onEndLabelEdit(const wchar_t *newlabel) 
	{
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_STRING(newlabel);
    scriptVar *params[1] = {&a};
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), onendlabeledit_id, params));
	}
	
	int C_TreeItem::onContextMenu(int x, int y) {
    ASSERT(inited);
    scriptVar a = MAKE_SCRIPT_INT(x);
    scriptVar b = MAKE_SCRIPT_INT(y);
    scriptVar *params[2] = {&a, &b};
    return GET_SCRIPT_INT(WASABI_API_MAKI->maki_callFunction(getScriptObject(), oncontextmenu_id, params));
	}
	
	int C_TreeItem::loaded=0;
	int C_TreeItem::getnumchildren_id=0;
	int C_TreeItem::setlabel_id=0;
	int C_TreeItem::getlabel_id=0;
	int C_TreeItem::ensurevisible_id=0;
	int C_TreeItem::getnthchild_id=0;
	int C_TreeItem::getchild_id=0;
	int C_TreeItem::getchildsibling_id=0;
	int C_TreeItem::getsibling_id=0;
	int C_TreeItem::getparent_id=0;
	int C_TreeItem::editlabel_id=0;
	int C_TreeItem::hassubitems_id=0;
	int C_TreeItem::setsorted_id=0;
	int C_TreeItem::setchildtab_id=0;
	int C_TreeItem::issorted_id=0;
	int C_TreeItem::iscollapsed_id=0;
	int C_TreeItem::isexpanded_id=0;
	int C_TreeItem::invalidate_id=0;
	int C_TreeItem::isselected_id=0;
	int C_TreeItem::ishilited_id=0;
	int C_TreeItem::sethilited_id=0;
	int C_TreeItem::collapse_id=0;
	int C_TreeItem::expand_id=0;
	int C_TreeItem::gettree_id=0;
	int C_TreeItem::ontreeadd_id=0;
	int C_TreeItem::ontreeremove_id=0;
	int C_TreeItem::onselect_id=0;
	int C_TreeItem::ondeselect_id=0;
	int C_TreeItem::onleftdoubleclick_id=0;
	int C_TreeItem::onrightdoubleclick_id=0;
	int C_TreeItem::onchar_id=0;
	int C_TreeItem::onexpand_id=0;
	int C_TreeItem::oncollapse_id=0;
	int C_TreeItem::onbeginlabeledit_id=0;
	int C_TreeItem::onendlabeledit_id=0;
	int C_TreeItem::oncontextmenu_id=0;
	
/* This file was generated by Maki Compiler, do not edit manually */

#ifndef __C_GUIOBJECT_H
#define __C_GUIOBJECT_H

#include "c_rootobj.h"

#define C_GUIOBJECT_PARENT C_RootObject

class C_GuiObject : public C_GUIOBJECT_PARENT {
public:

C_GuiObject(ScriptObject *object);
C_GuiObject();
virtual ~C_GuiObject();

virtual void C_hook(ScriptObject *o);

ScriptObject *getScriptObject();

virtual void show();
virtual void hide();
virtual int isVisible();
virtual void onSetVisible(int onoff);
virtual void setAlpha(int alpha);
virtual int getAlpha();
virtual void onLeftButtonUp(int x, int y);
virtual void onLeftButtonDown(int x, int y);
virtual void onRightButtonUp(int x, int y);
virtual void onRightButtonDown(int x, int y);
virtual void onRightButtonDblClk(int x, int y);
virtual void onLeftButtonDblClk(int x, int y);
virtual void onMouseMove(int x, int y);
virtual void onEnterArea();
virtual void onLeaveArea();
virtual void setEnabled(int onoff);
virtual int getEnabled();
virtual void onEnable(int onoff);
virtual void resize(int x, int y, int w, int h);
virtual void onResize(int x, int y, int w, int h);
virtual int isMouseOver(int x, int y);
virtual int getLeft();
virtual int getTop();
virtual int getWidth();
virtual int getHeight();
virtual void setTargetX(int x);
virtual void setTargetY(int y);
virtual void setTargetW(int w);
virtual void setTargetH(int r);
virtual void setTargetA(int alpha);
virtual void setTargetSpeed(float insecond);
virtual void gotoTarget();
virtual void onTargetReached();
virtual void cancelTarget();
virtual void reverseTarget(int reverse);
virtual void onStartup();
virtual int isGoingToTarget();
virtual void setXmlParam(const wchar_t *param, const wchar_t *value);
virtual const wchar_t *getXmlParam(const wchar_t *param);
virtual void init(ScriptObject *parent);
virtual void bringToFront();
virtual void bringToBack();
virtual void bringAbove(ScriptObject *guiobj);
virtual void bringBelow(ScriptObject *guiobj);
virtual int getGuiX();
virtual int getGuiY();
virtual int getGuiW();
virtual int getGuiH();
virtual int getGuiRelatX();
virtual int getGuiRelatY();
virtual int getGuiRelatW();
virtual int getGuiRelatH();
virtual int isActive();
virtual ScriptObject *getParent();
virtual ScriptObject *getParentLayout();
virtual ScriptObject *getTopParent();
virtual int runModal();
virtual void endModal(int retcode);
virtual ScriptObject *findObject(const wchar_t *id);
virtual ScriptObject *findObjectXY(int x, int y);
virtual const wchar_t *getName();
virtual int clientToScreenX(int x);
virtual int clientToScreenY(int y);
virtual int clientToScreenW(int w);
virtual int clientToScreenH(int h);
virtual int screenToClientX(int x);
virtual int screenToClientY(int y);
virtual int screenToClientW(int w);
virtual int screenToClientH(int h);
virtual int getAutoWidth();
virtual int getAutoHeight();
virtual void setFocus();
virtual void onChar(const wchar_t *c);
virtual void onAccelerator(const wchar_t *accel);
virtual int isMouseOverRect();
virtual ScriptObject *getInterface(const wchar_t *interface_guid);
virtual void onKeyDown(int vk_code);
virtual void onKeyUp(int vk_code);
virtual void onGetFocus();
virtual void onKillFocus();
virtual int sendAction(const wchar_t *action, const wchar_t *param, int x, int y, intptr_t p1, intptr_t p2);
virtual int onAction(const wchar_t *action, const wchar_t *param, int x, int y, intptr_t p1, intptr_t p2, ScriptObject *source);

private:

ScriptObject *obj;
int inited;
static int loaded;
static int show_id;
static int hide_id;
static int isvisible_id;
static int onsetvisible_id;
static int setalpha_id;
static int getalpha_id;
static int onleftbuttonup_id;
static int onleftbuttondown_id;
static int onrightbuttonup_id;
static int onrightbuttondown_id;
static int onrightbuttondblclk_id;
static int onleftbuttondblclk_id;
static int onmousemove_id;
static int onenterarea_id;
static int onleavearea_id;
static int setenabled_id;
static int getenabled_id;
static int onenable_id;
static int resize_id;
static int onresize_id;
static int ismouseover_id;
static int getleft_id;
static int gettop_id;
static int getwidth_id;
static int getheight_id;
static int settargetx_id;
static int settargety_id;
static int settargetw_id;
static int settargeth_id;
static int settargeta_id;
static int settargetspeed_id;
static int gototarget_id;
static int ontargetreached_id;
static int canceltarget_id;
static int reversetarget_id;
static int onstartup_id;
static int isgoingtotarget_id;
static int setxmlparam_id;
static int getxmlparam_id;
static int init_id;
static int bringtofront_id;
static int bringtoback_id;
static int bringabove_id;
static int bringbelow_id;
static int getguix_id;
static int getguiy_id;
static int getguiw_id;
static int getguih_id;
static int getguirelatx_id;
static int getguirelaty_id;
static int getguirelatw_id;
static int getguirelath_id;
static int isactive_id;
static int getparent_id;
static int getparentlayout_id;
static int gettopparent_id;
static int runmodal_id;
static int endmodal_id;
static int findobject_id;
static int findobjectxy_id;
static int getname_id;
static int clienttoscreenx_id;
static int clienttoscreeny_id;
static int clienttoscreenw_id;
static int clienttoscreenh_id;
static int screentoclientx_id;
static int screentoclienty_id;
static int screentoclientw_id;
static int screentoclienth_id;
static int getautowidth_id;
static int getautoheight_id;
static int setfocus_id;
static int onchar_id;
static int onaccelerator_id;
static int ismouseoverrect_id;
static int getinterface_id;
static int onkeydown_id;
static int onkeyup_id;
static int ongetfocus_id;
static int onkillfocus_id;
static int sendaction_id;
static int onaction_id;
};

#endif

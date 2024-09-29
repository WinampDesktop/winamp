/* This file was generated by Maki Compiler, do not edit manually */

#ifndef __C_GUILIST_H
#define __C_GUILIST_H

#include "c_guiobject.h"

#define C_GUILIST_PARENT C_GuiObject

class C_GuiList : public C_GUILIST_PARENT {
public:

C_GuiList(ScriptObject *object);
C_GuiList();
virtual ~C_GuiList();

virtual void C_hook(ScriptObject *o);

ScriptObject *getScriptObject();

virtual int getNumItems();
virtual int getWantAutoDeselect();
virtual void setWantAutoDeselect(int want);
virtual void onSetVisible(int show);
virtual void setAutoSort(int dosort);
virtual void next();
virtual void selectCurrent();
virtual void selectFirstEntry();
virtual void previous();
virtual void pagedown();
virtual void pageup();
virtual void home();
virtual void end();
virtual void reset();
virtual int addColumn(const wchar_t *name, int width, int numeric);
virtual int getNumColumns();
virtual int getColumnWidth(int column);
virtual void setColumnWidth(int column, int newwidth);
virtual const wchar_t *getColumnLabel(int column);
virtual void setColumnLabel(int column, const wchar_t *newlabel);
virtual int getColumnNumeric(int column);
virtual void setColumnDynamic(int column, int isdynamic);
virtual int isColumnDynamic(int column);
virtual void setMinimumSize(int size);
virtual int addItem(const wchar_t *label);
virtual int insertItem(int pos, const wchar_t *label);
virtual int getLastAddedItemPos();
virtual void setSubItem(int pos, int subpos, const wchar_t *txt);
virtual void deleteAllItems();
virtual int deleteByPos(int pos);
virtual const wchar_t *getItemLabel(int pos, int subpos);
virtual void setItemLabel(int pos, const wchar_t *_text);
virtual int getItemSelected(int pos);
virtual int isItemFocused(int pos);
virtual int getItemFocused();
virtual void setItemFocused(int pos);
virtual void ensureItemVisible(int pos);
virtual void invalidateColumns();
virtual int scrollAbsolute(int x);
virtual int scrollRelative(int x);
virtual void scrollLeft(int lines);
virtual void scrollRight(int lines);
virtual void scrollUp(int lines);
virtual void scrollDown(int lines);
virtual const wchar_t *getSubitemText(int pos, int subpos);
virtual int getFirstItemSelected();
virtual int getNextItemSelected(int lastpos);
virtual int selectAll();
virtual int deselectAll();
virtual int invertSelection();
virtual int invalidateItem(int pos);
virtual int getFirstItemVisible();
virtual int getLastItemVisible();
virtual int setFontSize(int size);
virtual int getFontSize();
virtual void jumpToNext(int c);
virtual void scrollToItem(int pos);
virtual void resort();
virtual int getSortDirection();
virtual int getSortColumn();
virtual void setSortColumn(int col);
virtual void setSortDirection(int dir);
virtual int getItemCount();
virtual void setSelectionStart(int pos);
virtual void setSelectionEnd(int pos);
virtual void setSelected(int pos, int selected);
virtual void toggleSelection(int pos, int setfocus);
virtual int getHeaderHeight();
virtual int getPreventMultipleSelection();
virtual int setPreventMultipleSelection(int val);
virtual void moveItem(int from, int to);
virtual void onSelectAll();
virtual void onDelete();
virtual void onDoubleClick(int itemnum);
virtual void onLeftClick(int itemnum);
virtual void onSecondLeftClick(int itemnum);
virtual int onRightClick(int itemnum);
virtual int onColumnDblClick(int col, int x, int y);
virtual int onColumnLabelClick(int col, int x, int y);
virtual void onItemSelection(int itemnum, int selected);

private:

ScriptObject *obj;
int inited;
static int loaded;
static int getnumitems_id;
static int getwantautodeselect_id;
static int setwantautodeselect_id;
static int onsetvisible_id;
static int setautosort_id;
static int next_id;
static int selectcurrent_id;
static int selectfirstentry_id;
static int previous_id;
static int pagedown_id;
static int pageup_id;
static int home_id;
static int end_id;
static int reset_id;
static int addcolumn_id;
static int getnumcolumns_id;
static int getcolumnwidth_id;
static int setcolumnwidth_id;
static int getcolumnlabel_id;
static int setcolumnlabel_id;
static int getcolumnnumeric_id;
static int setcolumndynamic_id;
static int iscolumndynamic_id;
static int setminimumsize_id;
static int additem_id;
static int insertitem_id;
static int getlastaddeditempos_id;
static int setsubitem_id;
static int deleteallitems_id;
static int deletebypos_id;
static int getitemlabel_id;
static int setitemlabel_id;
static int getitemselected_id;
static int isitemfocused_id;
static int getitemfocused_id;
static int setitemfocused_id;
static int ensureitemvisible_id;
static int invalidatecolumns_id;
static int scrollabsolute_id;
static int scrollrelative_id;
static int scrollleft_id;
static int scrollright_id;
static int scrollup_id;
static int scrolldown_id;
static int getsubitemtext_id;
static int getfirstitemselected_id;
static int getnextitemselected_id;
static int selectall_id;
static int deselectall_id;
static int invertselection_id;
static int invalidateitem_id;
static int getfirstitemvisible_id;
static int getlastitemvisible_id;
static int setfontsize_id;
static int getfontsize_id;
static int jumptonext_id;
static int scrolltoitem_id;
static int resort_id;
static int getsortdirection_id;
static int getsortcolumn_id;
static int setsortcolumn_id;
static int setsortdirection_id;
static int getitemcount_id;
static int setselectionstart_id;
static int setselectionend_id;
static int setselected_id;
static int toggleselection_id;
static int getheaderheight_id;
static int getpreventmultipleselection_id;
static int setpreventmultipleselection_id;
static int moveitem_id;
static int onselectall_id;
static int ondelete_id;
static int ondoubleclick_id;
static int onleftclick_id;
static int onsecondleftclick_id;
static int onrightclick_id;
static int oncolumndblclick_id;
static int oncolumnlabelclick_id;
static int onitemselection_id;
};

#endif

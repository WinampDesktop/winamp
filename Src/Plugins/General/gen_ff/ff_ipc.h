#ifndef _FF_IPC_H
#define _FF_IPC_H

// -----------------------------------------------------------------------------------------------------
// ----- IPC_FF_GETSKINCOLOR : Ask for a skin color -- the color is filtered for the current theme -----
// -----------------------------------------------------------------------------------------------------

#define IPC_FF_GETSKINCOLOR IPC_FF_FIRST + 1 // data = ff_skincolor struct with .colorname, fills in .color

typedef struct {
char colorname[256];
COLORREF color;
} ff_skincolor;

// List of default colors as of june 30, 2003. see freeform/xml/wasabi/xml/system-colors.xml for latest/complete list

// Trees
#define SKINCOLOR_TREE_ITEMTEXT        "wasabi.tree.text"
#define SKINCOLOR_TREE_SELITEMBKG      L"wasabi.tree.selected"
#define SKINCOLOR_TREE_HILITEDROP      "wasabi.tree.hiliteddrop"

// Lists
#define SKINCOLOR_LIST_ITEMTEXT        L"wasabi.list.text"
#define SKINCOLOR_LIST_SELITEMBKG      "wasabi.list.item.selected"
#define SKINCOLOR_LIST_FOCUSITEMBKG    "wasabi.list.item.focused"
#define SKINCOLOR_LIST_COLUMNBKG       L"wasabi.list.column.background"
#define SKINCOLOR_LIST_COLUMNTEXT      L"wasabi.list.column.text"
#define SKINCOLOR_LIST_SELITEMTEXT     "wasabi.list.item.selected.fg"
#define SKINCOLOR_LIST_COLUMNSEPARATOR "wasabi.list.column.separator"

// Buttons
#define SKINCOLOR_BUTTON_TEXT          L"wasabi.button.text" 
#define SKINCOLOR_BUTTON_HILITETEXT    "wasabi.button.hiliteText"
#define SKINCOLOR_BUTTON_DIMMEDTEXT    "wasabi.button.dimmedText"


// ----------------------------------------------------------------------------------------
// ----- IPC_FF_GENSKINBITMAP: Ask gen_ff to create a bitmap of various skin elements -----
// ----------------------------------------------------------------------------------------

// NOTE: You should free the hbitmap eventually using DeleteObject

#define IPC_FF_GENSKINBITMAP IPC_FF_FIRST + 2 // data = ff_skinbitmap with bitmap .id .w .h and .state, fills in .bitmap 

typedef struct {
int id; // see below
int w, h;
int state; // id specific, see below
HBITMAP bitmap;
} ff_skinbitmap;

// Bitmap IDs :

#define SKINBITMAP_BUTTON               0 // Generate a button bitmap. states are as follows :

#define BUTTONSTATE_NORMAL 0
#define BUTTONSTATE_PUSHED 1

#define SKINBITMAP_SCROLLBARUPBUTTON    1 // Generate a scrollbar up button bitmap. states are button states
#define SKINBITMAP_SCROLLBARDOWNBUTTON  2 // Generate a scrollbar down button bitmap. states are button states
#define SKINBITMAP_SCROLLBARLEFTBUTTON  3 // Generate a scrollbar left button bitmap. states are button states
#define SKINBITMAP_SCROLLBARRIGHTBUTTON 4 // Generate a scrollbar right button bitmap. states are button states
#define SKINBITMAP_SCROLLBARVBUTTON     5 // Generate a scrollbar vertical button bitmap. states are button states
#define SKINBITMAP_SCROLLBARHBUTTON     6 // Generate a scrollbar horizontal button bitmap. states are button states

#define SKINBITMAP_SCROLLBAR_FF_UPBUTTON     7 // Generate a freeform scrollbar up button bitmap. states are scrollbar states
#define SKINBITMAP_SCROLLBAR_FF_DOWNBUTTON   8 // Generate a freeform scrollbar down button bitmap. states are scrollbar states
#define SKINBITMAP_SCROLLBAR_FF_LEFTBUTTON   9 // Generate a freeform scrollbar left button bitmap. states are scrollbar states
#define SKINBITMAP_SCROLLBAR_FF_RIGHTBUTTON 10 // Generate a freeform scrollbar right button bitmap. states are scrollbar states
#define SKINBITMAP_SCROLLBAR_FF_BARHBUTTON  11 // Generate a freeform scrollbar vertical button bitmap. states are scrollbar states
#define SKINBITMAP_SCROLLBAR_FF_BARVBUTTON  12 // Generate a freeform scrollbar horizontal button bitmap. states are scrollbar states

#define SCROLLBARSTATE_NORMAL  0
#define SCROLLBARSTATE_PRESSED 1
#define SCROLLBARSTATE_HOVER   2

// -----------------------------------------------------------------------------------------------
// ----- IPC_FF_ONCOLORTHEMECHANGED: CALLBACK - sent when the skin's color theme has changed -----
// -----------------------------------------------------------------------------------------------

#define IPC_FF_ONCOLORTHEMECHANGED IPC_FF_FIRST + 3 // data = name of the new color theme (const char *)
#define IPC_FF_ISMAINWND IPC_FF_FIRST + 4 // data = hwnd, returns 1 if hwnd is main window or any of its windowshade
#define IPC_FF_GETCONTENTWND IPC_FF_FIRST + 5 // data = HWND, returns the wa2 window that is embedded in the window's container (ie if hwnd is the pledit windowshade hwnd, it returns the wa2 pledit hwnd). if no content is found (ie, the window has nothing embedded) it returns the parameter you gave it
#define IPC_FF_NOTIFYHOTKEY IPC_FF_FIRST + 6 // data = const char * to hotkey description

#endif

/*
  ==============================================================================

    MainMenu.cpp
    Created: 14 Dec 2015 2:30:48pm
    Author:  falindrith

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainMenu.h"
#include "globals.h"

//==============================================================================
MainMenu::MainMenu()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

MainMenu::~MainMenu()
{
}

StringArray MainMenu::getMenuBarNames() {
  const char* names[] = { "File", "Edit", "Explore", "Analyze", "Window", nullptr };
  return StringArray(names);
}

PopupMenu MainMenu::getMenuForIndex(int topLevelMenuIndex, const String& /* menuName */) {
  ApplicationCommandManager* cm = getApplicationCommandManager();

  PopupMenu menu;

  if (topLevelMenuIndex == 0) {
    menu.addCommandItem(cm, command::OPEN);
    menu.addCommandItem(cm, command::OPEN_MASK);
    menu.addSeparator();
    menu.addCommandItem(cm, command::SAVE);
    menu.addCommandItem(cm, command::SAVE_AS);
    menu.addCommandItem(cm, command::SAVE_RENDER);
    menu.addCommandItem(cm, command::SAVE_IDEAS);
    menu.addCommandItem(cm, command::LOAD_IDEAS);
    menu.addSeparator();
    menu.addCommandItem(cm, command::LOAD_ATTRS);
    menu.addCommandItem(cm, command::RELOAD_ATTRS);
  }
  else if (topLevelMenuIndex == 1) {
    menu.addCommandItem(cm, command::COPY_DEVICE);
    menu.addCommandItem(cm, command::PASTE_ALL);
    menu.addCommandItem(cm, command::PASTE_INTENS);
    menu.addCommandItem(cm, command::PASTE_COLOR);
    menu.addSeparator();
    menu.addCommandItem(cm, command::SET_TO_FULL);
    menu.addCommandItem(cm, command::SET_TO_OFF);
    menu.addCommandItem(cm, command::SET_TO_WHITE);
    menu.addSeparator();
    menu.addCommandItem(cm, command::ARNOLD_RENDER);
    menu.addCommandItem(cm, command::TOGGLE_SELECT_VIEW);
    menu.addSeparator();
    menu.addCommandItem(cm, command::RESET_ALL);
    menu.addSeparator();
    menu.addCommandItem(cm, command::SYNC);
    menu.addCommandItem(cm, command::SYNC_SELECT);
    //menu.addCommandItem(cm, command::GET_FROM_ARNOLD);
    //menu.addCommandItem(cm, command::RESET_TIMER);
  }
  else if (topLevelMenuIndex == 2) {
    menu.addCommandItem(cm, command::SEARCH);
    menu.addCommandItem(cm, command::STOP_SEARCH);
		menu.addSeparator();
    menu.addCommandItem(cm, command::DELETE_ALL_PINS);
    //menu.addCommandItem(cm, command::RECLUSTER);
		//menu.addCommandItem(cm, command::SAVE_CLUSTERS);
		//menu.addCommandItem(cm, command::LOAD_CLUSTERS);
    menu.addSeparator();
    menu.addCommandItem(cm, command::UNLOCK_ALL);
    menu.addCommandItem(cm, command::LOCK_ALL_SELECTED);
    menu.addCommandItem(cm, command::LOCK_SELECTED_INTENSITY);
    menu.addCommandItem(cm, command::LOCK_SELECTED_COLOR);
    menu.addCommandItem(cm, command::UNLOCK_ALL_SELECTED);
    menu.addCommandItem(cm, command::UNLOCK_SELECTED_INTENSITY);
    menu.addCommandItem(cm, command::UNLOCK_SELECTED_COLOR);

    PopupMenu lockSubmenu;
    lockSubmenu.addCommandItem(cm, command::LOCK_ALL_AREAS_EXCEPT);
    lockSubmenu.addCommandItem(cm, command::LOCK_AREA);
    lockSubmenu.addCommandItem(cm, command::LOCK_ALL_SYSTEMS_EXCEPT);
    lockSubmenu.addCommandItem(cm, command::LOCK_SYSTEM);
    lockSubmenu.addCommandItem(cm, command::LOCK_ALL_COLOR);
    lockSubmenu.addCommandItem(cm, command::LOCK_ALL_INTENSITY);

    menu.addSubMenu("Lock Groups", lockSubmenu);
  }
  else if (topLevelMenuIndex == 3) {
    menu.addCommandItem(cm, command::SAVE_RESULTS);
    menu.addCommandItem(cm, command::LOAD_RESULTS);
    menu.addSeparator();
    menu.addCommandItem(cm, command::LOAD_TRACES);
    menu.addCommandItem(cm, command::PICK_TRACE);
  }
  else if (topLevelMenuIndex == 4) {
    menu.addCommandItem(cm, command::INTERFACE_NEW);
    menu.addCommandItem(cm, command::INTERFACE_OLD);
    menu.addCommandItem(cm, command::INTERFACE_ALL);
    menu.addSeparator();
    menu.addCommandItem(cm, command::SHOW_PROMPT);
    menu.addSeparator();
    menu.addCommandItem(cm, command::ABOUT);
  }

  return menu;
}

void MainMenu::menuItemSelected(int /* menuItemID */, int /* topLevelMenuIndex */)
{
  // Custom item handling, if needed.
}

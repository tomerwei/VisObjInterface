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
  const char* names[] = { "File", "Edit", "Render", "Explore", nullptr };
  return StringArray(names);
}

PopupMenu MainMenu::getMenuForIndex(int topLevelMenuIndex, const String& menuName) {
  ApplicationCommandManager* cm = getApplicationCommandManager();

  PopupMenu menu;

  if (topLevelMenuIndex == 0) {
    menu.addCommandItem(cm, command::OPEN);
    menu.addSeparator();
    menu.addCommandItem(cm, command::SAVE);
    menu.addCommandItem(cm, command::SAVE_AS);
  }
  else if (topLevelMenuIndex == 1) {
    menu.addCommandItem(cm, command::SETTINGS);
  }
  else if (topLevelMenuIndex == 2) {
    menu.addCommandItem(cm, command::ARNOLD_RENDER);
  }
  else if (topLevelMenuIndex == 3) {
    menu.addCommandItem(cm, command::SEARCH);
    menu.addCommandItem(cm, command::RECLUSTER);
  }

  return menu;
}

void MainMenu::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
  // Custom item handling, if needed.
}

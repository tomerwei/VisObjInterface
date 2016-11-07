/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "globals.h"
#include "MainMenu.h"

//#include <vld.h>

//==============================================================================
class NewProjectApplication  : public JUCEApplication
{
public:
    //==============================================================================
    NewProjectApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
      // This method is where you should put your application's initialisation code..
      // process command line args
      int start = 0;
      int end = 0;
      while (start <= commandLine.length()) {
        end = commandLine.indexOf(start, " ");
        String substr = commandLine.substring(start, end);

        if (substr == "--preload" || substr == "-p") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();
          getGlobalSettings()->_commandLineArgs["preload"] = commandLine.substring(start, end);
        }
        else if (substr == "--auto" || substr == "-a") {
          // indicates process should automatically run something then close.
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be the search method id. see SearchMode in globals.h
          getGlobalSettings()->_commandLineArgs["auto"] = commandLine.substring(start, end);
        }
        else if (substr == "--img-attr" || substr == "-ia") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a filename or "auto" for automatically generate one.
          getGlobalSettings()->_commandLineArgs["img-attr"] = commandLine.substring(start, end);
        }
        else if (substr == "--more" || substr == "-m") {
          // Indicates more for the attribute
          getGlobalSettings()->_commandLineArgs["more"] = "true";
        }
        else if (substr == "--less" || substr == "-l") {
          // Indicates more for the attribute
          getGlobalSettings()->_commandLineArgs["less"] = "true";
        }
        else if (substr == "--samples" || substr == "-n") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a number
          getGlobalSettings()->_commandLineArgs["samples"] = commandLine.substring(start, end);
        }
        else if (substr == "--out" || substr == "-o") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a directory
          getGlobalSettings()->_commandLineArgs["out"] = commandLine.substring(start, end);
        }
        else if (substr == "--timeout" || substr == "-t") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a number in minutes
          getGlobalSettings()->_commandLineArgs["timeout"] = commandLine.substring(start, end);
        }
        else if (substr == "--jnd" || substr == "-j") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a number
          getGlobalSettings()->_commandLineArgs["jnd"] = commandLine.substring(start, end);
        }
        else if (substr == "--ev-weight") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a number
          getGlobalSettings()->_commandLineArgs["evWeight"] = commandLine.substring(start, end);
        }
        else if (substr == "--temperature" || substr == "-T") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a number
          getGlobalSettings()->_commandLineArgs["T"] = commandLine.substring(start, end);
        }
        else if (substr == "--chain-length" || substr == "-c") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a number
          getGlobalSettings()->_commandLineArgs["chainLength"] = commandLine.substring(start, end);
        }
        else if (substr == "--step-size" || substr == "-s") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a number
          getGlobalSettings()->_commandLineArgs["stepSize"] = commandLine.substring(start, end);
        }
        else if (substr == "--session-name") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a number
          getGlobalSettings()->_commandLineArgs["outputFolderName"] = commandLine.substring(start, end);
        }
        else if (substr == "--uniform-edits") {
          // this should be a number
          getGlobalSettings()->_commandLineArgs["uniformEdits"] = "true";
        }
        else if (substr == "--random-init") {
          // this should be a number
          getGlobalSettings()->_commandLineArgs["randomInit"] = "true";
        }
        else if (substr == "--resample-time") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a number
          getGlobalSettings()->_commandLineArgs["resampleTime"] = commandLine.substring(start, end);
        }
        else if (substr == "--free-threads") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a number
          getGlobalSettings()->_commandLineArgs["resampleThreads"] = commandLine.substring(start, end);
        }
        else if (substr == "--edit-mode" || substr == "-em") {
          start = end + 1;
          end = commandLine.indexOf(start, " ");
          if (end == -1)
            end = commandLine.length();

          // this should be a number
          getGlobalSettings()->_commandLineArgs["editMode"] = commandLine.substring(start, end);
        }

        start = end + 1;

        if (start <= 0)
          break;
      }

      mainWindow = new MainWindow (getApplicationName());

      // if the auto flag is present, redirect to different function to get that started.
      // pass it through the application command manager to make it async
      if (getGlobalSettings()->_commandLineArgs.count("auto") != 0) {
        getApplicationCommandManager()->invokeDirectly(START_AUTO, true);
      }
    }

    void shutdown() override
    {
      mainWindow = nullptr; // (deletes our window)

      // Add your application's shutdown code here..
      cleanUpGlobals();
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& /* commandLine */) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainContentComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            MainContentComponent* cmp = new MainContentComponent();
            setContentOwned (cmp, true);

            setFullScreen(true);
            //centreWithSize (getWidth(), getHeight());
            setResizable(true, false);
            setVisible (true);

            _menu = new MainMenu();
            setMenuBar(_menu, 0);

            getApplicationCommandManager()->registerAllCommandsForTarget(cmp);
            getApplicationCommandManager()->getKeyMappings()->resetToDefaultMappings();
            addKeyListener(getApplicationCommandManager()->getKeyMappings());
            getRecorder()->log(SYSTEM, "Interface ready.");
            getGlobalSettings();
        }

        ~MainWindow() {
          setMenuBar(nullptr);
        }

        void closeButtonPressed() override
        {
          // This is called when the user tries to close this window. Here, we'll just
          // ask the app to quit when this happens, but you can change this to do
          // whatever you need.
          JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
      JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
      ScopedPointer<MainMenu> _menu;
    };

private:
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (NewProjectApplication)

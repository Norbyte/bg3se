# Troubleshooting Script Extender

Below are some fixes that can be attempted if you are having problems with Script Extender.

## My game is crashing!

If your game is crashing, here are some steps you can take to try to resolve the issue:

  - **Check that Script Extender has been verified to work with your game version.**

  	New versions of Script Extender must be built whenever the game is updated or patched. You can
    check whether Script Extender has been verified to work with your game version by reading the
    notes for the latest [Updater Release](https://github.com/Norbyte/bg3se/releases). Note that
    because GitHub releases are only created when Script Extender's updater is changed, the latest
    release may *look* old, however its notes are updated to reflect Script Extender's current game
    compatibility.
  
  	If a version of Script Extender is not available for your game version, then you can either
    downgrade your game to a supported version, or wait. **Do not create a new issue for version
    incompatibilities unless you are sure one hasn't already been created.**

  - **Check whether any mods are incompatible with your game version.** 

    Many mods, especially mods that alter the GUI or dialogues, may be incompatible after a game
    update. The fastest way to check this is to:
      - Press `<Windows Key>+R` to bring up the "Run" dialogue.
      - Type `%LOCALAPPDATA%/Larian Studios/Baldur's Gate 3` into the "Run" dialogue, then press
        enter.
      - In the Explorer window that opens, rename the `Mods` folder to `Disabled Mods`.
      - In the same Explorer window, rename the `PlayerProfiles` folder to `Disabled
        PlayerProfiles`. Note, THIS WILL TEMPORARILY RESET YOUR GAME.
      - Start the game, the start a new save file. If the game starts and and you are able to create
        a new save file, then it is likely that one or more mods are incompatible with your version
        of Script Extender or the game.
      - To restore your old profile, refresh your Explorer window. The game will have created new
        `PlayerProfiles` and `Mods` folders. Delete them, then undo the name changes made to the `Disabled
        PlayerProfiles` and `Disabled Mods` folders.

    Note, **simply disabling mods through a mod manager may not be sufficient to check whether an
    issue is caused by a mod.** Some mods override core game files, even when they are marked as
    disabled.

    If a mod is incompatible with your game version, check if there's an update available for it.
    Otherwise, you'll need to contact that mod's author. 


## Characters are behaving strangely/Items don't work/I'm experiencing game issues other than crashes

These kinds of issues are usually caused by the following:
  - A bug in a mod you are using.
  - Multiple mods conflicting with each other.
  - An incompatibility with your version of a mod and your version of Script Extender. 
  - An incompatibility with your version of a mod and your version of the game.

In all these cases, you should either update the mod to a version compatible with your versions of
Script Extender and the game, or report the problem to the author of the mod.

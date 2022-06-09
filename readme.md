# Sprite Font Builder 

Sprite Font Builder is an open-source cross-platform bitmap font generator created using Qt. It intends to provide a layered production system similar to Photoshop FX layers so that you can build complicated fonts in one place for your games.

## Build Notes

There are two projects included in this repository.

**SpriteFontBuilder.pro**: The main application project. You should just be able to open this project in Qt creator, click build and get going.

**libs/SFBWidgets/SFBWidgets.pro**: All of the new UI widgets for SpriteFontBuilder are in this library. The files here are actually also inclued in the SpriteFontBuilder project (so no funny stuff is needed just to compile the application), but if you want to load the SFB widgets into Qt Designer, then this project will let you build a plugin DLL.

Creating an installable executable is a bit of a faff with the Open Source Qt Creator, but there are some scripts and text files containing notes lying around the repository to help with it. I hope to tidy these up one day!

## Gimme the App

If you have stumbled across this page but just want to download the application binaries, head over to [this website](http://www.johnwordsworth.com/projects/sprite-font-builder/) and enjoy!

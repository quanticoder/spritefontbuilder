#-------------------------------------------------
#
# Project created by QtCreator 2014-03-08T19:19:58
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml

TARGET = SpriteFontBuilder
TEMPLATE = app

CONFIG(debug, debug|release) {
    CURBUILD = debug
} else {
    CURBUILD = release
}

DESTDIR = $${OUT_PWD}/$${CURBUILD}

macx {
    QMAKE_CXXFLAGS += -stdlib=libc++
    QMAKE_LFLAGS += -stdlib=libc++
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
    QMAKE_INFO_PLIST = "platforms/osx/Info.plist"
    CONFIG += c++11
    APP_ICON.files = "platforms/osx/spritefontbuilder.icns"
    APP_ICON.path = Contents/Resources/
    QMAKE_BUNDLE_DATA += APP_ICON
}

ios {
    QMAKE_INFO_PLIST = "platforms/ios/Info.plist"
}

win32 {
    # Flags for minGW
    QMAKE_CXXFLAGS += -std=gnu++0x
    QMAKE_LFLAGS += -static-libgcc

    # Icon RC File
    RC_FILE = "platforms/win/spritefontbuilder.rc"
}

win32:CONFIG(release, debug|release) {
    EXTRA_BINFILES += \
        $$[QT_INSTALL_LIBS]/../bin/Qt5Core.dll \
        $$[QT_INSTALL_LIBS]/../bin/Qt5Gui.dll \
        $$[QT_INSTALL_LIBS]/../bin/Qt5Widgets.dll \
        $$[QT_INSTALL_LIBS]/../bin/Qt5Xml.dll \
        $$[QT_INSTALL_LIBS]/../bin/Qt5Concurrent.dll \
        $$[QT_INSTALL_LIBS]/../bin/icudt51.dll \
        $$[QT_INSTALL_LIBS]/../bin/icuin51.dll \
        $$[QT_INSTALL_LIBS]/../bin/icuuc51.dll \
        $$[QT_INSTALL_LIBS]/../bin/libwinpthread-1.dll \
        $$[QT_INSTALL_LIBS]/../bin/libstdc++-6.dll \
        $$[QT_INSTALL_LIBS]/../bin/libgcc_s_dw2-1.dll

        # MinGW Builds
        # $$[QT_INSTALL_LIBS]/../bin/libwinpthread-1.dll \
        # $$[QT_INSTALL_LIBS]/../bin/libstdc++-6.dll \
        # $$[QT_INSTALL_LIBS]/../bin/libgcc_s_dw2-1.dll

        # Angle Build Only
        # $$[QT_INSTALL_LIBS]/../bin/libEGL.dll \
        # $$[QT_INSTALL_LIBS]/../bin/libGLESv2.dll \
        # $$[QT_INSTALL_LIBS]/../bin/d3dcompiler_46.dll

    EXTRA_BINFILES_WIN = $${EXTRA_BINFILES}
    EXTRA_BINFILES_WIN ~= s,/,\\,g
    DESTDIR_WIN = $$DESTDIR
    DESTDIR_WIN ~= s,/,\\,g

    for(FILE,EXTRA_BINFILES_WIN){
        QMAKE_POST_LINK += $$QMAKE_COPY \"$$quote($$FILE)\" \"$$quote($$DESTDIR_WIN)\" $$escape_expand(\\n\\t)
    }

    !exists(\"$$DESTDIR_WIN\\platforms\\\") {
        QMAKE_POST_LINK += mkdir \"$$DESTDIR_WIN\\platforms\\\" $$escape_expand(\\n\\t)
    }

    QMAKE_POST_LINK += $$QMAKE_COPY \"$$[QT_INSTALL_LIBS]\\..\\plugins\\platforms\\qminimal.dll\" \"$$DESTDIR_WIN\\platforms\\\" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_COPY \"$$[QT_INSTALL_LIBS]\\..\\plugins\\platforms\\qwindows.dll\" \"$$DESTDIR_WIN\\platforms\\\" $$escape_expand(\\n\\t)
}

INCLUDEPATH += $$quote($$PWD)
INCLUDEPATH += "libs/SFBWidgets/include/"

Release:DESTDIR = release
Release:OBJECTS_DIR = release/.obj
Release:MOC_DIR = release/.moc
Release:RCC_DIR = release/.rcc
Release:UI_DIR = release/.ui

Debug:DESTDIR = debug
Debug:OBJECTS_DIR = debug/.obj
Debug:MOC_DIR = debug/.moc
Debug:RCC_DIR = debug/.rcc
Debug:UI_DIR = debug/.ui

FORMS += \
    forms/MainWindow.ui \
    src/UI/Dialogs/GradientEditorDialog.ui \
    src/UI/Widgets/InputSettingsPanel.ui \
    src/UI/Widgets/GenerationSettingsPanel.ui \
    src/UI/Widgets/FillEffectSettingsPanel.ui \
    src/UI/Widgets/StrokeEffectSettingsPanel.ui \
    src/UI/Widgets/ShadowEffectSettingsPanel.ui \
    src/UI/Widgets/EffectListRow.ui \
    src/UI/Widgets/ExportSettingsPanel.ui

HEADERS += \
    src/MainWindow.h \
    src/Model/SFBDocument.h \
    src/Model/SFBTypes.h \
    src/Model/SFBGlyph.h \
    src/UI/Widgets/ColorSwatchWidget.h \
    src/Model/Effects/SFBEffect.h \
    src/Model/Effects/SFBFillEffect.h \
    src/Model/Effects/SFBStrokeEffect.h \
    src/Model/Effects/SFBShadowEffect.h \
    src/Model/Effects/SFBEffectTypes.h \
    src/Model/Effects/Effects.h \
    src/UI/Widgets/GradientSwatchWidget.h \
    src/UI/Dialogs/GradientEditorDialog.h \
    src/UI/Widgets/GradientStopEditorWidget.h \
    src/Model/SFBGradient.h \
    src/Model/Exporters/SFBFontExporter.h \
    src/Model/SFBSpriteFont.h \
    src/Model/Exporters/SFBFontExporterBMFont.h \
    src/Model/Exporters/SFBFontExporterPNG.h \
    src/Model/Exporters/SFBFontExporters.h \
    src/UI/Widgets/InputSettingsPanel.h \
    src/UI/Widgets/GenerationSettingsPanel.h \
    src/UI/Widgets/FillEffectSettingsPanel.h \
    src/UI/Widgets/StrokeEffectSettingsPanel.h \
    src/UI/Widgets/ShadowEffectSettingsPanel.h \
    src/Model/SFBFile/SFBFileWriter.h \
    src/Model/SFBFile/SFBDocumentElement.h \
    src/Model/SFBFile/SFBFileReader.h \
    src/UI/Widgets/PatternSwatchWidget.h \
    src/UI/Widgets/EffectListRow.h \
    src/UI/Widgets/ExportSettingsPanel.h \
    src/Model/SFBExportSettings.h \
    src/Model/Exporters/SFBFontExporterTypes.h \
    src/Model/SFBInputSettings.h \
    src/Model/SFBGenerationSettings.h

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/Model/SFBDocument.cpp \
    src/Model/SFBTypes.cpp \
    src/Model/SFBGlyph.cpp \
    src/UI/Widgets/ColorSwatchWidget.cpp \
    src/Model/Effects/SFBEffect.cpp \
    src/Model/Effects/SFBFillEffect.cpp \
    src/Model/Effects/SFBStrokeEffect.cpp \
    src/Model/Effects/SFBShadowEffect.cpp \
    src/UI/Widgets/GradientSwatchWidget.cpp \
    src/UI/Dialogs/GradientEditorDialog.cpp \
    src/UI/Widgets/GradientStopEditorWidget.cpp \
    src/Model/SFBGradient.cpp \
    src/Model/Exporters/SFBFontExporter.cpp \
    src/Model/Exporters/SFBFontExporterBMFont.cpp \
    src/Model/Exporters/SFBFontExporterPNG.cpp \
    src/UI/Widgets/InputSettingsPanel.cpp \
    src/UI/Widgets/GenerationSettingsPanel.cpp \
    src/UI/Widgets/FillEffectSettingsPanel.cpp \
    src/Model/Effects/SFBEffectTypes.cpp \
    src/UI/Widgets/StrokeEffectSettingsPanel.cpp \
    src/UI/Widgets/ShadowEffectSettingsPanel.cpp \
    src/Model/SFBFile/SFBFileWriter.cpp \
    src/Model/SFBFile/SFBDocumentElement.cpp \
    src/Model/SFBFile/SFBFileReader.cpp \
    src/UI/Widgets/PatternSwatchWidget.cpp \
    src/UI/Widgets/EffectListRow.cpp \
    src/UI/Widgets/ExportSettingsPanel.cpp \
    src/Model/SFBExportSettings.cpp \
    src/Model/SFBInputSettings.cpp \
    src/Model/SFBGenerationSettings.cpp

OTHER_FILES += \
    platforms/osx/Info.plist

RESOURCES += \
    resources/resources.qrc


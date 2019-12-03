QT += core gui widgets
TARGET = IDE
TEMPLATE = app
RC_FILE = resources/windows.rc

CONFIG(release, debug|release): LIBS += -lqjson -ladvapi32
else:CONFIG(debug, debug|release): LIBS += -lqjsond -ladvapi32
INCLUDEPATH += $$[QT_INSTALL_HEADERS] include

SOURCES += \
    Main.cpp\
    MainWindow.cpp \
    Compiler.cpp \
    Dialogs/WelcomeDialog.cpp \
    Project/Project.cpp \
    Dialogs/NewProjectDialog.cpp \
    Forms/TextureEditor.cpp \
    Forms/SoundEditor.cpp \
    Dialogs/ConfigurationDialog.cpp \
    Project/SceneData.cpp \
    Forms/SceneEditor.cpp \
    Dialogs/ScenePropertiesDialog.cpp \
    Forms/ScriptEditor.cpp \
    Dialogs/AboutDialog.cpp \
    Forms/JsEdit.cpp \
    Forms/AssetForm.cpp \
    Util.cpp \
    MainWindow.ProjectExplorer.cpp \
    MainWindow.MenuBar.cpp \
    Project/JsonObject.cpp \
    Dialogs/NewAssetDialog.cpp \
    Project/Lexer/TokenStream.cpp \
    Project/Lexer/Tokenizer.cpp \
    Project/Lexer/StringStream.cpp \
    IdePreferences.cpp \
    Dialogs/IdePreferencesDialog.cpp \
    Forms/SceneEntityForm.cpp
HEADERS += \
    include/MainWindow.hpp \
    include/Compiler.hpp \
    include/Dialogs/WelcomeDialog.hpp \
    include/Project/Project.hpp \
    include/Dialogs/NewProjectDialog.hpp \
    include/Forms/TextureEditor.hpp \
    include/Forms/SoundEditor.hpp \
    include/Dialogs/ConfigurationDialog.hpp \
    include/Forms/JsEdit.hpp \
    include/Project/SceneData.hpp \
    include/Forms/SceneEditor.hpp \
    include/Dialogs/ScenePropertiesDialog.hpp \
    include/Forms/ScriptEditor.hpp \
    include/Dialogs/AboutDialog.hpp \
    include/Util.hpp \
    include/Forms/AssetForm.hpp \
    include/Project/JsonObject.hpp \
    include/Dialogs/NewAssetDialog.hpp \
    include/Project/Lexer/TokenStream.hpp \
    include/Project/Lexer/Tokenizer.hpp \
    include/Project/Lexer/Token.hpp \
    include/Project/Lexer/StringStream.hpp \
    include/IdePreferences.hpp \
    include/Dialogs/IdePreferencesDialog.hpp \
    include/Forms/SceneEntityForm.hpp
FORMS += \
    ui/MainWindow.ui \
    ui/Dialogs/WelcomeDialog.ui \
    ui/Dialogs/NewProjectDialog.ui \
    ui/Forms/TextureEditor.ui \
    ui/Forms/SoundEditor.ui \
    ui/Dialogs/ConfigurationDialog.ui \
    ui/Dialogs/ScenePropertiesDialog.ui \
    ui/Dialogs/AboutDialog.ui \
    ui/Dialogs/NewAssetDialog.ui \
    ui/Dialogs/IdePreferencesDialog.ui \
    ui/Forms/SceneEntityEditorForm.ui
RESOURCES += \
    resources/icons.qrc \
    resources/fonts.qrc

OTHER_FILES += \
    resources/windows.rc

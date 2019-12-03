/*
 *  ScriptEditor.cpp
 *  Script Editor Form
 */

#include <QMessageBox>
#include <Forms/ScriptEditor.hpp>

ScriptEditor::ScriptEditor(AssetLink* assetLink, QWidget *parent)
    : AssetForm(assetLink, parent),
	  watcher(),
      frameTarget(this),
      gridLayoutTarget(&frameTarget),
      editor(this),
      asset(dynamic_cast<Script*>(GetAssetLink()->asset))
{
    // Reserve this asset
    GetAssetLink()->opened = true;

    // Don't mess with the order here
    SetupUi();
    LoadAsset();
    SetupConnections();
}

ScriptEditor::~ScriptEditor()
{

}

void ScriptEditor::SetupUi()
{
    // Create layout so the editor fills the entire form
    frameTarget.setFrameShape(QFrame::StyledPanel);
    frameTarget.setFrameShadow(QFrame::Raised);
    gridLayoutTarget.setSpacing(0);
    gridLayoutTarget.setContentsMargins(0, 0, 0, 0);
    setLayout(&gridLayoutTarget);

    // Style the editor
    QFont font("Consolas", 11, 4);
    font.setPixelSize(13);
    editor.setFont(font);
    editor.setTabStopWidth(QFontMetrics(font).width(' ') * 4);
    editor.setTextWrapEnabled(false);

    // Add it
    gridLayoutTarget.addWidget(&editor);

	editor.show();
}

void ScriptEditor::SetupConnections()
{
    connect(&editor, &QPlainTextEdit::textChanged, [=]() { SetModified(); });

	// Watch this script
	watcher.addPath(asset->GetPath());
	connect(&watcher, &QFileSystemWatcher::fileChanged, [=](const QString& path)
	{
		// Don't watch the file while we're reloading it
		watcher.blockSignals(true);

		// TODO Autoreload preference
		int index = QMessageBox::question(this, "What do you want to do?",
										  "This script has changed on the disk would you like to reload it?",
										  QMessageBox::Yes, QMessageBox::No);
		switch(index)
		{
			case QMessageBox::Yes:
				LoadAsset();
				SetModified(false);
				break;

			case QMessageBox::No:
			default:
				SetModified(true);
				break;
		}

		// It's safe to add the watch back
		watcher.blockSignals(false);
	});
}

void ScriptEditor::LoadAsset()
{
    // Open source file
    QFile sourceFile(asset->GetPath());
    if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Cannot open script.");
        close();
        return;
    }

    // Append its contexts to the editor
	editor.clear();
    editor.appendPlainText(sourceFile.readAll());
    QTextCursor cursor = editor.textCursor();
    cursor.movePosition(QTextCursor::Start);
    editor.setTextCursor(cursor);
}

void ScriptEditor::Save()
{
	// Don't watch the file when it's being saved, lol
	watcher.removePath(asset->GetPath());

    // Save the source file
    QFile sourceFile(asset->GetPath());
	if (sourceFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		sourceFile.write(editor.toPlainText().toLatin1());

		// NOTE We explicitly close the file here because RAII will close() the file
		//		when we exit the method AFTER we add the watch back, which signals
		//		a false change.
		sourceFile.close();

		// We're no longer dirty
		SetModified(false);
	}
	else
		QMessageBox::warning(this, "Error", "Cannot save script.");

	// It's safe to add the watch back
	watcher.addPath(asset->GetPath());
}

void ScriptEditor::TabFocusIn()
{
	watcher.blockSignals(false);
	editor.setFocus();
}

void ScriptEditor::TabFocusOut()
{
	watcher.blockSignals(true);
}

void ScriptEditor::RunToLine(int line)
{
    editor.setLineError(line);

    QTextCursor cursor = editor.textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, line - 1);
    cursor.movePosition(QTextCursor::EndOfLine);
    editor.setTextCursor(cursor);
    editor.setFocus();
}

void ScriptEditor::ClearErrors()
{
    editor.clearErrors();
}

/*
  This file is part of the Ofi Labs X2 project.

  Copyright (C) 2011 Ariya Hidayat <ariya.hidayat@gmail.com>
  Copyright (C) 2010 Ariya Hidayat <ariya.hidayat@gmail.com>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QtGui>
#include <Forms/JsEdit.hpp>

class JSBlockData: public QTextBlockUserData
{
public:
    QList<int> bracketPositions;
};

class JSHighlighter : public QSyntaxHighlighter
{
public:
    JSHighlighter(QTextDocument *parent = 0);
    void setColor(JSEdit::ColorComponent component, const QColor &color);
    void mark(const QString &str, Qt::CaseSensitivity caseSensitivity);

    QStringList keywords() const;
    void setKeywords(const QStringList &keywords);

protected:
    void highlightBlock(const QString &text);

private:
    QSet<QString> m_keywords;
    QSet<QString> m_knownIds;
    QHash<JSEdit::ColorComponent, QColor> m_colors;
    QString m_markString;
    Qt::CaseSensitivity m_markCaseSensitivity;
};

JSHighlighter::JSHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
    , m_markCaseSensitivity(Qt::CaseInsensitive)
{
    // default color scheme
    m_colors[JSEdit::Normal]     = QColor(0, 0, 0);
    m_colors[JSEdit::Comment]    = QColor(32, 140, 32);//QColor(0, 128, 0);
    m_colors[JSEdit::Number]     = QColor(247, 115, 7);//33, 48, 150);
    m_colors[JSEdit::String]     = QColor(163, 21, 21);
    m_colors[JSEdit::Operator]   = QColor(0, 0, 0);
    m_colors[JSEdit::Identifier] = QColor(0, 94, 158);//QColor(0, 0, 240);
    m_colors[JSEdit::Keyword]    = QColor(0, 0, 240);
    m_colors[JSEdit::BuiltIn]    = QColor(0, 94, 158);
    m_colors[JSEdit::Marker]     = QColor(178, 228, 255);

    // https://developer.mozilla.org/en/JavaScript/Reference/Reserved_Words
    m_keywords << "class";
    m_keywords << "state";
    m_keywords << "entity";
    m_keywords << "method";
    m_keywords << "enum";
    m_keywords << "const";
    m_keywords << "var";
	m_keywords << "property";
	m_keywords << "get";
	m_keywords << "set";
	m_keywords << "value";

    m_keywords << "public";
    m_keywords << "private";
    m_keywords << "protected";
	m_keywords << "static";

    m_keywords << "switch";
    m_keywords << "case";
    m_keywords << "default";
    m_keywords << "continue";
    m_keywords << "break";
    m_keywords << "while";
    m_keywords << "do";
    m_keywords << "else";
    m_keywords << "for";
    m_keywords << "foreach";
    m_keywords << "if";
    m_keywords << "in";
    m_keywords << "return";
    m_keywords << "this";
    m_keywords << "typeof";
    m_keywords << "repeat";
	m_keywords << "len";

    m_keywords << "nil";
    m_keywords << "int";
    m_keywords << "double";
    m_keywords << "str";
    m_keywords << "list";
    m_keywords << "dict";
	m_keywords << "object";
	m_keywords << "is";

    m_keywords << "true";
    m_keywords << "false";
    m_keywords << "null";

    m_knownIds << "Sprite";
    m_knownIds << "Sound";
    m_knownIds << "Keyboard";
    m_knownIds << "Mouse";
    m_knownIds << "Color";
    m_knownIds << "Random";
    m_knownIds << "Time";
	m_knownIds << "BinaryFile";
	m_knownIds << "Debug";
	m_knownIds << "TextFile";
    m_knownIds << "Game";
    m_knownIds << "Grid";
	m_knownIds << "FileSystem";
	m_knownIds << "Math";

    m_knownIds << "Textures";
    m_knownIds << "Sounds";
    m_knownIds << "Fonts";
    m_knownIds << "Scenes";
}

void JSHighlighter::setColor(JSEdit::ColorComponent component, const QColor &color)
{
    m_colors[component] = color;
    rehighlight();
}

void JSHighlighter::highlightBlock(const QString &text)
{
    // parsing state
    enum {
        Start,
        Number,
        Identifier,
        String,
        Comment
    };

    QList<int> bracketPositions;

    int blockState = previousBlockState();
	int bracketLevel = (blockState >> 4) & 0x0F;
	int commentScope = (blockState >> 8) & 0x0F;
    int state = blockState & 15;
    if (blockState < 0) {
        bracketLevel = 0;
        state = Start;
    }

    int start = 0;
    int i = 0;
    while (i <= text.length()) {
        QChar ch = (i < text.length()) ? text.at(i) : QChar();
        QChar next = (i < text.length() - 1) ? text.at(i + 1) : QChar();

        switch (state) {

        case Start:
            start = i;
            if (ch.isSpace()) {
                ++i;
            } else if (ch.isDigit()) {
                ++i;
                state = Number;
				if (next == 'x' || next == 'b')
					++i;
            } else if (ch.isLetter() || ch == '_') {
                ++i;
                state = Identifier;
            } else if (ch == '\'' || ch == '\"') {
                ++i;
                state = String;
            } else if (ch == '/' && next == '*') {
                ++i;
                ++i;
				commentScope = 1;
                state = Comment;
            } else if (ch == '/' && next == '/') {
                i = text.length();
                setFormat(start, text.length(), m_colors[JSEdit::Comment]);
            } else {
                if (!QString("(){}[]").contains(ch))
                    setFormat(start, 1, m_colors[JSEdit::Operator]);
                if (ch =='{' || ch == '}') {
                    bracketPositions += i;
                    if (ch == '{')
                        bracketLevel++;
                    else
                        bracketLevel--;
                }
                ++i;
                state = Start;
            }
            break;

        case Number:
            if (ch.isSpace() || !ch.isDigit()) {
                setFormat(start, i - start, m_colors[JSEdit::Number]);
                state = Start;
            } else {
                ++i;
            }
            break;

        case Identifier:
            if (ch.isSpace() || !(ch.isDigit() || ch.isLetter() || ch == '_')) {
                QString token = text.mid(start, i - start).trimmed();
                if (m_keywords.contains(token))
                    setFormat(start, i - start, m_colors[JSEdit::Keyword]);
                else if (m_knownIds.contains(token))
                    setFormat(start, i - start, m_colors[JSEdit::BuiltIn]);
                state = Start;
            } else {
                ++i;
            }
            break;

        case String:
            if (ch == text.at(start)) {
                QChar prev = (i > 0) ? text.at(i - 1) : QChar();
                if (prev != '\\') {
                    ++i;
                    setFormat(start, i - start, m_colors[JSEdit::String]);
                    state = Start;
                } else {
                    ++i;
                }
            } else {
                ++i;
            }
            break;

        case Comment:
			if (ch == '/' && next == '*')
			{
				++commentScope;
				i += 2;
			}
			else if (ch == '*' && next == '/') {
                ++i;
                ++i;

				if (--commentScope <= 0)
				{
					commentScope = 0;
					setFormat(start, i - start, m_colors[JSEdit::Comment]);
					state = Start;
				}
            } else {
                ++i;
            }
            break;

        default:
            state = Start;
            break;
        }
    }

    if (state == Comment)
        setFormat(start, text.length(), m_colors[JSEdit::Comment]);
    else
        state = Start;

    if (!m_markString.isEmpty()) {
        int pos = 0;
        int len = m_markString.length();
        QTextCharFormat markerFormat;
        markerFormat.setBackground(m_colors[JSEdit::Marker]);
        markerFormat.setForeground(m_colors[JSEdit::Normal]);
        for (;;) {
            pos = text.indexOf(m_markString, pos, m_markCaseSensitivity);
            if (pos < 0)
                break;
            setFormat(pos, len, markerFormat);
            ++pos;
        }
    }

    if (!bracketPositions.isEmpty()) {
        JSBlockData *blockData = reinterpret_cast<JSBlockData*>(currentBlock().userData());
        if (!blockData) {
            blockData = new JSBlockData;
            currentBlock().setUserData(blockData);
        }
        blockData->bracketPositions = bracketPositions;
    }

	blockState = (state & 15) | ((commentScope & 0x0F) << 8) | ((bracketLevel & 0x0F) << 4);
    setCurrentBlockState(blockState);
}

void JSHighlighter::mark(const QString &str, Qt::CaseSensitivity caseSensitivity)
{
    m_markString = str;
    m_markCaseSensitivity = caseSensitivity;
    rehighlight();
}

QStringList JSHighlighter::keywords() const
{
    return m_keywords.toList();
}

void JSHighlighter::setKeywords(const QStringList &keywords)
{
    m_keywords = QSet<QString>::fromList(keywords);
    rehighlight();
}

struct BlockInfo {
    int position;
    int number;
    bool foldable: 1;
    bool folded : 1;
    bool error : 1;
};

Q_DECLARE_TYPEINFO(BlockInfo, Q_PRIMITIVE_TYPE);

class SidebarWidget : public QWidget
{
public:
    SidebarWidget(JSEdit *editor);
    QVector<BlockInfo> lineNumbers;
    QColor backgroundColor;
    QColor lineNumberColor;
    QColor indicatorColor;
    QColor foldIndicatorColor;
    QFont font;
    int foldIndicatorWidth;
    QPixmap rightArrowIcon;
    QPixmap downArrowIcon;
    QPixmap errorIcon;
protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
};

SidebarWidget::SidebarWidget(JSEdit *editor)
    : QWidget(editor)
    , foldIndicatorWidth(0)
{
    backgroundColor = QColor(240, 240, 240);
    lineNumberColor = QColor(160, 160, 160);
    indicatorColor = Qt::white;
    foldIndicatorColor = Qt::lightGray;
}

void SidebarWidget::mousePressEvent(QMouseEvent *event)
{
    if (foldIndicatorWidth > 0) {
        int xofs = width() - foldIndicatorWidth;
        int lineNo = -1;
        int fh = fontMetrics().lineSpacing();
        int ys = event->pos().y();
        if (event->pos().x() > xofs) {
            foreach (BlockInfo ln, lineNumbers)
                if (ln.position < ys && (ln.position + fh) > ys) {
                    if (ln.foldable)
                        lineNo = ln.number;
                    break;
                }
        }
        if (lineNo >= 0) {
            JSEdit *editor = qobject_cast<JSEdit*>(parent());
            if (editor)
                editor->toggleFold(lineNo);
        }
    }
}

void SidebarWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.fillRect(event->rect(), backgroundColor);
    p.setPen(lineNumberColor);
    p.setFont(font);
    int fh = QFontMetrics(font).height();
    foreach (BlockInfo ln, lineNumbers)
        p.drawText(0, ln.position, width() - 4 - foldIndicatorWidth, fh, Qt::AlignRight, QString::number(ln.number));

    if (foldIndicatorWidth > 0) {
        int xofs = width() - foldIndicatorWidth;
        p.fillRect(xofs, 0, foldIndicatorWidth, height(), indicatorColor);

        // initialize (or recreate) the arrow icons whenever necessary
        if (foldIndicatorWidth != rightArrowIcon.width()) {
            QPainter iconPainter;
            QPolygonF polygon;

            int dim = foldIndicatorWidth;
            rightArrowIcon = QPixmap(dim, dim);
            rightArrowIcon.fill(Qt::transparent);
            downArrowIcon = rightArrowIcon;

            polygon << QPointF(dim * 0.4, dim * 0.25);
            polygon << QPointF(dim * 0.4, dim * 0.75);
            polygon << QPointF(dim * 0.8, dim * 0.5);
            iconPainter.begin(&rightArrowIcon);
            iconPainter.setRenderHint(QPainter::Antialiasing);
            iconPainter.setPen(Qt::NoPen);
            iconPainter.setBrush(foldIndicatorColor);
            iconPainter.drawPolygon(polygon);
            iconPainter.end();

            polygon.clear();
            polygon << QPointF(dim * 0.25, dim * 0.4);
            polygon << QPointF(dim * 0.75, dim * 0.4);
            polygon << QPointF(dim * 0.5, dim * 0.8);
            iconPainter.begin(&downArrowIcon);
            iconPainter.setRenderHint(QPainter::Antialiasing);
            iconPainter.setPen(Qt::NoPen);
            iconPainter.setBrush(foldIndicatorColor);
            iconPainter.drawPolygon(polygon);
            iconPainter.end();

            errorIcon = QPixmap(dim, dim);
            errorIcon.fill(Qt::transparent);
            iconPainter.begin(&errorIcon);
            iconPainter.setRenderHint(QPainter::Antialiasing);
            iconPainter.setPen(Qt::NoPen);
            iconPainter.drawImage(0, 0, QImage(":/icons/error.png"));
            iconPainter.end();
        }

        foreach (BlockInfo ln, lineNumbers)
        {
            if (ln.foldable) {
                if (ln.folded)
                    p.drawPixmap(xofs, ln.position, rightArrowIcon);
                else
                    p.drawPixmap(xofs, ln.position, downArrowIcon);
            }

            if (ln.error)
                p.drawPixmap(0, ln.position, errorIcon);
        }
    }
}

static int findClosingMatch(const QTextDocument *doc, int cursorPosition)
{
    QTextBlock block = doc->findBlock(cursorPosition);
    JSBlockData *blockData = reinterpret_cast<JSBlockData*>(block.userData());
    if (!blockData->bracketPositions.isEmpty()) {
        int depth = 1;
        while (block.isValid()) {
            blockData = reinterpret_cast<JSBlockData*>(block.userData());
            if (blockData && !blockData->bracketPositions.isEmpty()) {
                for (int c = 0; c < blockData->bracketPositions.count(); ++c) {
                    int absPos = block.position() + blockData->bracketPositions.at(c);
                    if (absPos <= cursorPosition)
                        continue;
                    if (doc->characterAt(absPos) == '{')
                        depth++;
                    else
                        depth--;
                    if (depth == 0)
                        return absPos;
                }
            }
            block = block.next();
        }
    }
    return -1;
}

static int findOpeningMatch(const QTextDocument *doc, int cursorPosition)
{
    QTextBlock block = doc->findBlock(cursorPosition);
    JSBlockData *blockData = reinterpret_cast<JSBlockData*>(block.userData());
    if (!blockData->bracketPositions.isEmpty()) {
        int depth = 1;
        while (block.isValid()) {
            blockData = reinterpret_cast<JSBlockData*>(block.userData());
            if (blockData && !blockData->bracketPositions.isEmpty()) {
                for (int c = blockData->bracketPositions.count() - 1; c >= 0; --c) {
                    int absPos = block.position() + blockData->bracketPositions.at(c);
                    if (absPos >= cursorPosition - 1)
                        continue;
                    if (doc->characterAt(absPos) == '}')
                        depth++;
                    else
                        depth--;
                    if (depth == 0)
                        return absPos;
                }
            }
            block = block.previous();
        }
    }
    return -1;
}

class JSDocLayout: public QPlainTextDocumentLayout
{
public:
    JSDocLayout(QTextDocument *doc);
    void forceUpdate();
};

JSDocLayout::JSDocLayout(QTextDocument *doc)
    : QPlainTextDocumentLayout(doc)
{
}

void JSDocLayout::forceUpdate()
{
    emit documentSizeChanged(documentSize());
}

class JSEditPrivate
{
public:
    JSEdit *editor;
    JSDocLayout *layout;
    JSHighlighter *highlighter;
    SidebarWidget *sidebar;
    bool showLineNumbers;
    bool textWrap;
    QColor cursorColor;
    bool bracketsMatching;
    QList<int> matchPositions;
    QColor bracketMatchColor;
    QList<int> errorPositions;
    QList<int> erroneousLines;
    QColor bracketErrorColor;
    bool codeFolding : 1;
};

JSEdit::JSEdit(QWidget *parent)
    : QPlainTextEdit(parent)
    , d_ptr(new JSEditPrivate)
{
    d_ptr->editor = this;
    d_ptr->layout = new JSDocLayout(document());
    d_ptr->highlighter = new JSHighlighter(document());
    d_ptr->sidebar = new SidebarWidget(this);
    d_ptr->showLineNumbers = true;
    d_ptr->textWrap = true;
    d_ptr->bracketsMatching = true;
    d_ptr->cursorColor = QColor(244, 251, 255);
    d_ptr->bracketMatchColor = QColor(180, 238, 180);
    d_ptr->bracketErrorColor = QColor(224, 128, 128);
    d_ptr->codeFolding = true;

    document()->setDocumentLayout(d_ptr->layout);
    setStyleSheet("QPlainTextEdit { border: 0px; }");

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateCursor()));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateSidebar()));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateSidebar(QRect, int)));

#if defined(Q_OS_MAC)
    QFont textFont = font();
    textFont.setPointSize(12);
    textFont.setFamily("Monaco");
    setFont(textFont);
#elif defined(Q_OS_UNIX)
    QFont textFont = font();
    textFont.setFamily("Monospace");
    setFont(textFont);
#endif
}

JSEdit::~JSEdit()
{
    delete d_ptr->layout;
}

#include <QDebug>

void JSEdit::keyPressEvent(QKeyEvent* ev)
{
    Q_D(JSEdit);

    int initialLine = textCursor().block().blockNumber() + 1;

	// NOTE What the fuck is up with the modifiers here? They're flags...
	//		I'm not sure what I was trying to do and I'm afraid to mess up
	//		anything that's working, so don't touch.
    if (ev->key() == Qt::Key_Return && (ev->modifiers() == Qt::NoModifier ||
                                        ev->modifiers() == Qt::ControlModifier ||
                                        ev->modifiers() == Qt::ShiftModifier))
    {
		QTextCursor updateCursor = textCursor();
		QString data = toPlainText();
        int cursorPosition = updateCursor.position();
        int i;

        for (i = cursorPosition - 2; i >= 0; --i)
        {
            if (data.mid(i, 1) == "\n")
                break;
        }

        updateCursor.insertText("\n");
        while (data.mid(i + 1, 1) == "\t")
        {
            updateCursor.insertText("\t");
            i++;
        }
    }
	else if (ev->key() == Qt::Key_Tab || ev->key() == Qt::Key_Backtab)
	{
		QString data = toPlainText();

		if (textCursor().hasSelection() &&
			(textCursor().selectionStart() == 0 || data[textCursor().selectionStart() - 1] == '\n'))
		{
			int startPosition = textCursor().selectionStart();

			QString block = textCursor().selectedText();

			if (ev->key() == Qt::Key_Tab)
			{
				// TAB block

				block.insert(0, '\t');

				// NOTE Yeah, I know block.length() is slow, but I don't really care
				for(int i = 0; i < block.length(); ++i)
				{
					if (block[i] == '\n' || block[i].toLatin1() == 0)
						block.insert(++i, '\t');
				}
			}
			else
			{
				// BACKTAB block
				for(int i = 0; i < block.length(); ++i)
				{
					if (block[i] == '\t' && block[i + 1] != '\t')
						block.remove(i++, 1);
				}
			}

			textCursor().insertText(block);

			int endPosition = textCursor().position();

			QTextCursor cur = textCursor();
			cur.setPosition(startPosition);
			cur.setPosition(endPosition, QTextCursor::KeepAnchor);
			setTextCursor(cur);
		}
		else
			QPlainTextEdit::keyPressEvent(ev);
	}
	else
		QPlainTextEdit::keyPressEvent(ev);

    if (ev->key() == Qt::Key_Return || ev->key() == Qt::Key_Backspace && (ev->modifiers() == Qt::NoModifier ||
                                                                          ev->modifiers() == Qt::ControlModifier ||
                                                                          ev->modifiers() == Qt::ShiftModifier))
    {
        int currentLine = textCursor().block().blockNumber() + 1;
        int lineDelta = currentLine - initialLine;
        for(int i = 0; i < d->erroneousLines.size(); ++i)
            d->erroneousLines[i] += lineDelta;

        updateSidebar();
    }

    ev->accept();
}

void JSEdit::setColor(ColorComponent component, const QColor &color)
{
    Q_D(JSEdit);

    if (component == Background) {
        QPalette pal = palette();
        pal.setColor(QPalette::Base, color);
        setPalette(pal);
        d->sidebar->indicatorColor = color;
        updateSidebar();
    } else if (component == Normal) {
        QPalette pal = palette();
        pal.setColor(QPalette::Text, color);
        setPalette(pal);
    } else if (component == Sidebar) {
        d->sidebar->backgroundColor = color;
        updateSidebar();
    } else if (component == LineNumber) {
        d->sidebar->lineNumberColor = color;
        updateSidebar();
    } else if (component == Cursor) {
        d->cursorColor = color;
        updateCursor();
    } else if (component == BracketMatch) {
        d->bracketMatchColor = color;
        updateCursor();
    } else if (component == BracketError) {
        d->bracketErrorColor = color;
        updateCursor();
    } else if (component == FoldIndicator) {
        d->sidebar->foldIndicatorColor = color;
        updateSidebar();
    } else {
        d->highlighter->setColor(component, color);
        updateCursor();
    }
}

QStringList JSEdit::keywords() const
{
    return d_ptr->highlighter->keywords();
}

void JSEdit::setKeywords(const QStringList &keywords)
{
    d_ptr->highlighter->setKeywords(keywords);
}

bool JSEdit::isLineNumbersVisible() const
{
    return d_ptr->showLineNumbers;
}

void JSEdit::setLineNumbersVisible(bool visible)
{
    d_ptr->showLineNumbers = visible;
    updateSidebar();
}

bool JSEdit::isTextWrapEnabled() const
{
    return d_ptr->textWrap;
}

void JSEdit::setTextWrapEnabled(bool enable)
{
    d_ptr->textWrap = enable;
    setLineWrapMode(enable ? WidgetWidth : NoWrap);
}

bool JSEdit::isBracketsMatchingEnabled() const
{
    return d_ptr->bracketsMatching;
}

void JSEdit::setBracketsMatchingEnabled(bool enable)
{
    d_ptr->bracketsMatching = enable;
    updateCursor();
}

bool JSEdit::isCodeFoldingEnabled() const
{
    return d_ptr->codeFolding;
}

void JSEdit::setCodeFoldingEnabled(bool enable)
{
    d_ptr->codeFolding = enable;
    updateSidebar();
}

static int findClosingConstruct(const QTextBlock &block)
{
    if (!block.isValid())
        return -1;
    JSBlockData *blockData = reinterpret_cast<JSBlockData*>(block.userData());
    if (!blockData)
        return -1;
    if (blockData->bracketPositions.isEmpty())
        return -1;
    const QTextDocument *doc = block.document();
    int offset = block.position();
    foreach (int pos, blockData->bracketPositions) {
        int absPos = offset + pos;
        if (doc->characterAt(absPos) == '{') {
            int matchPos = findClosingMatch(doc, absPos);
            if (matchPos >= 0)
                return matchPos;
        }
    }
    return -1;
}

bool JSEdit::isFoldable(int line) const
{
    int matchPos = findClosingConstruct(document()->findBlockByNumber(line - 1));
    if (matchPos >= 0) {
        QTextBlock matchBlock = document()->findBlock(matchPos);
        if (matchBlock.isValid() && matchBlock.blockNumber() > line)
            return true;
    }
    return false;
}

bool JSEdit::isFolded(int line) const
{
    QTextBlock block = document()->findBlockByNumber(line - 1);
    if (!block.isValid())
        return false;
    block = block.next();
    if (!block.isValid())
        return false;
    return !block.isVisible();
}

void JSEdit::fold(int line)
{
    QTextBlock startBlock = document()->findBlockByNumber(line - 1);
    int endPos = findClosingConstruct(startBlock);
    if (endPos < 0)
        return;
    QTextBlock endBlock = document()->findBlock(endPos);

    QTextBlock block = startBlock.next();
    while (block.isValid() && block != endBlock) {
        block.setVisible(false);
        block.setLineCount(0);
        block = block.next();
    }

    document()->markContentsDirty(startBlock.position(), endPos - startBlock.position() + 1);
    updateSidebar();
    update();

    JSDocLayout *layout = reinterpret_cast<JSDocLayout*>(document()->documentLayout());
    layout->forceUpdate();
}

void JSEdit::unfold(int line)
{
    QTextBlock startBlock = document()->findBlockByNumber(line - 1);
    int endPos = findClosingConstruct(startBlock);

    QTextBlock block = startBlock.next();
    while (block.isValid() && !block.isVisible()) {
        block.setVisible(true);
        block.setLineCount(block.layout()->lineCount());
        endPos = block.position() + block.length();
        block = block.next();
    }

    document()->markContentsDirty(startBlock.position(), endPos - startBlock.position() + 1);
    updateSidebar();
    update();

    JSDocLayout *layout = reinterpret_cast<JSDocLayout*>(document()->documentLayout());
    layout->forceUpdate();
}

void JSEdit::toggleFold(int line)
{
    if (isFolded(line))
        unfold(line);
    else
        fold(line);
}

void JSEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    updateSidebar();
}

void JSEdit::wheelEvent(QWheelEvent *e)
{
	Q_D(JSEdit);

    if (e->modifiers() == Qt::ControlModifier) {
        int steps = e->delta() / 20;
        steps = qBound(-3, steps, 3);
        QFont textFont = font();
        int pointSize = textFont.pointSize() + steps;
        pointSize = qBound(10, pointSize, 40);
        textFont.setPointSize(pointSize);
        setFont(textFont);
        updateSidebar();
        e->accept();
        return;
    }

    QPlainTextEdit::wheelEvent(e);
}


void JSEdit::updateCursor()
{
    Q_D(JSEdit);

    if (isReadOnly()) {
        setExtraSelections(QList<QTextEdit::ExtraSelection>());
    } else {

        d->matchPositions.clear();
        d->errorPositions.clear();

        if (d->bracketsMatching && textCursor().block().userData()) {
            QTextCursor cursor = textCursor();
            int cursorPosition = cursor.position();

            if (document()->characterAt(cursorPosition) == '{') {
                int matchPos = findClosingMatch(document(), cursorPosition);
                if (matchPos < 0) {
                    d->errorPositions += cursorPosition;
                } else {
                    d->matchPositions += cursorPosition;
                    d->matchPositions += matchPos;
                }
            }

            if (document()->characterAt(cursorPosition - 1) == '}') {
                int matchPos = findOpeningMatch(document(), cursorPosition);
                if (matchPos < 0) {
                    d->errorPositions += cursorPosition - 1;
                } else {
                    d->matchPositions += cursorPosition - 1;
                    d->matchPositions += matchPos;
                }
            }
        }

        QTextEdit::ExtraSelection highlight;
        highlight.format.setBackground(d->cursorColor);
        highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
        highlight.cursor = textCursor();
        highlight.cursor.clearSelection();

        QList<QTextEdit::ExtraSelection> extraSelections;
        extraSelections.append(highlight);

        for (int i = 0; i < d->matchPositions.count(); ++i) {
            int pos = d->matchPositions.at(i);
            QTextEdit::ExtraSelection matchHighlight;
            matchHighlight.format.setBackground(d->bracketMatchColor);
            matchHighlight.cursor = textCursor();
            matchHighlight.cursor.setPosition(pos);
            matchHighlight.cursor.setPosition(pos + 1, QTextCursor::KeepAnchor);
            extraSelections.append(matchHighlight);
        }

        for (int i = 0; i < d->errorPositions.count(); ++i) {
            int pos = d->errorPositions.at(i);
            QTextEdit::ExtraSelection errorHighlight;
            errorHighlight.format.setBackground(d->bracketErrorColor);
            errorHighlight.cursor = textCursor();
            errorHighlight.cursor.setPosition(pos);
            errorHighlight.cursor.setPosition(pos + 1, QTextCursor::KeepAnchor);
            extraSelections.append(errorHighlight);
        }

        setExtraSelections(extraSelections);
    }
}

void JSEdit::updateSidebar(const QRect &rect, int d)
{
    Q_UNUSED(rect)
    if (d != 0)
        updateSidebar();
}

void JSEdit::updateSidebar()
{
    Q_D(JSEdit);

    if (!d->showLineNumbers && !d->codeFolding) {
        d->sidebar->hide();
        setViewportMargins(0, 0, 0, 0);
        d->sidebar->setGeometry(3, 0, 0, height());
        return;
    }

    d->sidebar->foldIndicatorWidth = 0;
    d->sidebar->font = this->font();
    d->sidebar->show();

    int sw = 16;
    if (d->showLineNumbers) {
        int digits = 2;
        int maxLines = blockCount();
        for (int number = 10; number < maxLines; number *= 10)
            ++digits;
        sw += fontMetrics().width('w') * digits;
    }
    if (d->codeFolding) {
        int fh = fontMetrics().lineSpacing();
        int fw = fontMetrics().width('w');
        d->sidebar->foldIndicatorWidth = qMax(fw, fh);
        sw += d->sidebar->foldIndicatorWidth;
    }
    setViewportMargins(sw, 0, 0, 0);

    d->sidebar->setGeometry(0, 0, sw, height());
    QRectF sidebarRect(0, 0, sw, height());

    QTextBlock block = firstVisibleBlock();
    int index = 0;
    while (block.isValid()) {
        if (block.isVisible()) {
            QRectF rect = blockBoundingGeometry(block).translated(contentOffset());
            if (sidebarRect.intersects(rect)) {
                if (d->sidebar->lineNumbers.count() >= index)
                    d->sidebar->lineNumbers.resize(index + 1);
                d->sidebar->lineNumbers[index].position = rect.top();
                d->sidebar->lineNumbers[index].number = block.blockNumber() + 1;
                d->sidebar->lineNumbers[index].foldable = d->codeFolding ? isFoldable(block.blockNumber() + 1) : false;
                d->sidebar->lineNumbers[index].folded = d->codeFolding ? isFolded(block.blockNumber() + 1) : false;
				d->sidebar->lineNumbers[index].error = d->erroneousLines.contains(block.blockNumber());
                ++index;
            }
            if (rect.top() > sidebarRect.bottom())
                break;
        }
        block = block.next();
    }
    d->sidebar->lineNumbers.resize(index);
    d->sidebar->update();
}

void JSEdit::mark(const QString &str, Qt::CaseSensitivity sens)
{
    d_ptr->highlighter->mark(str, sens);
}

void JSEdit::setLineError(int line)
{
    Q_D(JSEdit);

    line -= 1;

    d->erroneousLines.push_back(line);
    updateSidebar();
}

void JSEdit::clearErrors()
{
    Q_D(JSEdit);

    d->erroneousLines.clear();
    updateSidebar();
}

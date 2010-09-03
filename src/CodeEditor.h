#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QtGui>
#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;
class LuaHighlighter;

class CodeEditor : public QPlainTextEdit
{
        Q_OBJECT

        public:
                CodeEditor(QWidget *parent = 0);

                void lineNumberAreaPaintEvent(QPaintEvent *event);
                int lineNumberAreaWidth();

        protected:
                void resizeEvent(QResizeEvent *event);

        private slots:
                void updateLineNumberAreaWidth(int newBlockCount);
                void highlightCurrentLine();
                void updateLineNumberArea(const QRect &, int);
                void matchParentheses();

        private:
                QWidget *lineNumberArea;
                LuaHighlighter *highlighter;

                bool matchLeftParenthesis(QTextBlock currentBlock, int index, int numRightParentheses);
                bool matchRightParenthesis(QTextBlock currentBlock, int index, int numLeftParentheses);
                void createParenthesisSelection(int pos);
};

class LineNumberArea : public QWidget
{
        Q_OBJECT

        public:
                LineNumberArea(CodeEditor *editor) : QWidget(editor) {
                        codeEditor = editor;
                }

                QSize sizeHint() const {
                        return QSize(codeEditor->lineNumberAreaWidth(), 0);
                }

        protected:
                void paintEvent(QPaintEvent *event) {
                        codeEditor->lineNumberAreaPaintEvent(event);
                }

        private:
                CodeEditor *codeEditor;
};

struct ParenthesisInfo
{
        char character;
        int position;
};

class TextBlockData : public QTextBlockUserData
{
        public:
                TextBlockData();

                QVector<ParenthesisInfo *> parentheses();
                void insert(ParenthesisInfo *info);

        private:
                QVector<ParenthesisInfo *> m_parentheses;
};

class LuaHighlighter : public QSyntaxHighlighter
{
        Q_OBJECT

        public:
                LuaHighlighter(QTextDocument *parent = 0);

        protected:
                void highlightBlock(const QString &text);

        private:
                struct HighlightingRule
                {
                        QRegExp pattern;
                        QTextCharFormat format;
                };
                QVector<HighlightingRule> highlightingRules;

                QRegExp commentStartExpression;
                QRegExp commentEndExpression;

                QTextCharFormat keywordFormat;
                QTextCharFormat operatorFormat;
                QTextCharFormat calaosFormat;
                QTextCharFormat singleLineCommentFormat;
                QTextCharFormat multiLineCommentFormat;
                QTextCharFormat quotationFormat;
                QTextCharFormat functionFormat;
};

#endif

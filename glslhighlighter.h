#ifndef GLSLHIGHLIGHTER_H
#define GLSLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QStringList>
#include <QVector>

class GLSLHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit GLSLHighlighter(QTextDocument *parent = Q_NULLPTR);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    void highlightMultiLineComments(const QString &text);

    void setupHighlightingRules();
    void setupKeywordRules();
    void setupTypeRules();
    void setupVariableRules();
    void setupFunctionRules();
    void addRules(const QStringList &patterns, const QTextCharFormat &format);
    void addRule(const QString &pattern, const QTextCharFormat &format);

    struct HighlightingRule
    {
        HighlightingRule() {}

        HighlightingRule(const QRegularExpression &pattern,
                         const QTextCharFormat &format) :
            pattern(pattern),
            format(format)
        {
        }

        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> rules;
    // multi-line comment patterns
    QRegularExpression commentStart;
    QRegularExpression commentEnd;

    QTextCharFormat multiLineCommentFormat;
    QColor keywordColor;
    QColor typeColor;
    QColor variableColor;
    QColor functionColor;
    QColor commentColor;
    QColor preprocessorColor;
};

#endif // GLSLHIGHLIGHTER_H

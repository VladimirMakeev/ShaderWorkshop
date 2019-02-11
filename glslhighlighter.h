/*
 * This file is part of ShaderWorkshop (https://github.com/VladimirMakeev/ShaderWorkshop).
 * Copyright (C) 2019 Vladimir Makeev.
 *
 * ShaderWorkshop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * ShaderWorkshop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with ShaderWorkshop.  If not, see <https://www.gnu.org/licenses/>.
 */

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

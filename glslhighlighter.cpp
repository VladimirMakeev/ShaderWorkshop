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

#include "glslhighlighter.h"

GLSLHighlighter::GLSLHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent),
    commentStart("/\\*"),
    commentEnd("\\*/"),
    keywordColor(Qt::black),
    typeColor("#0086b3"),
    variableColor(Qt::black),
    functionColor(Qt::blue),
    commentColor("#8a8a8a"),
    preprocessorColor("#8a8a8a")
{
    setupHighlightingRules();
}

void GLSLHighlighter::highlightBlock(const QString &text)
{
    for (auto rule : rules) {
        QRegularExpressionMatchIterator it = rule.pattern.globalMatch(text);

        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();

            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    highlightMultiLineComments(text);
}

void GLSLHighlighter::highlightMultiLineComments(const QString &text)
{
    setCurrentBlockState(0);

    int startIndex = 0;

    if (previousBlockState() != 1) {
        startIndex = text.indexOf(commentStart);
    }

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEnd.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;

        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }

        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStart, startIndex + commentLength);
    }
}

void GLSLHighlighter::setupHighlightingRules()
{
    setupKeywordRules();
    setupTypeRules();
    setupVariableRules();
    setupFunctionRules();

    QTextCharFormat format;
    // add highlighting rule for preprocessor directives
    format.setForeground(preprocessorColor);
    format.setFontWeight(QFont::Bold);
    addRule("#[^\n]*", format);
    // add rule for single-line comments
    format.setForeground(commentColor);
    format.setFontWeight(QFont::Normal);
    addRule("//[^\n]*", format);

    multiLineCommentFormat.setForeground(commentColor);
}

void GLSLHighlighter::setupKeywordRules()
{
    QTextCharFormat format;

    format.setForeground(Qt::black);
    format.setFontWeight(QFont::Bold);

    // GLSL keywords
    QStringList keywordPatterns{
        "\\battribute\\b", "\\bconst\\b", "\\buniform\\b", "\\bvarying\\b",
        "\\bbuffer\\b", "\\bshared\\b", "\\bcoherent\\b", "\\bvolatile\\b",
        "\\brestrict\\b", "\\breadonly\\b", "\\bwriteonly\\b", "\\blayout\\b",
        "\\bcentroid\\b", "\\bflat\\b", "\\bsmooth\\b", "\\bnoperspective\\b",
        "\\bpatch\\b", "\\bsample\\b", "\\bbreak\\b", "\\bcontinue\\b",
        "\\bdo\\b", "\\bfor\\b", "\\bwhile\\b", "\\bswitch\\b", "\\bcase\\b",
        "\\bdefault\\b", "\\bif\\b", "\\belse\\b", "\\bsubroutine\\b",
        "\\bin\\b", "\\bout\\b", "\\binout\\b", "\\btrue\\b", "\\bfalse\\b",
        "\\binvariant\\b", "\\bprecise\\b", "\\bdiscard\\b", "\\breturn\\b",
        "\\blowp\\b", "\\bmediump\\b", "\\bhighp\\b", "\\bprecision\\b",
        "\\bstruct\\b"
    };

    addRules(keywordPatterns, format);
}

void GLSLHighlighter::setupTypeRules()
{
    QTextCharFormat format;

    format.setForeground(typeColor);
    format.setFontWeight(QFont::Bold);
    // GLSL built-in types
    QStringList typePatterns{
        "\\bvoid\\b", "\\bbool\\b", "\\bint\\b", "\\buint\\b", "\\bfloat\\b",
        "\\bdouble\\b", "\\bvec2\\b", "\\bvec3\\b", "\\bvec4\\b", "\\bbvec2\\b",
        "\\bbvec3\\b", "\\bbvec4\\b", "\\bivec2\\b", "\\bivec3\\b", "\\bivec4\\b",
        "\\buvec2\\b", "\\buvec3\\b", "\\buvec4\\b", "\\bmat2\\b", "\\bmat3\\b",
        "\\bmat4\\b", "\\bmat2x2\\b", "\\bmat2x3\\b", "\\bmat2x4\\b",
        "\\bmat3x2\\b", "\\bmat3x3\\b", "\\bmat3x4\\b", "\\bmat4x2\\b",
        "\\bmat4x3\\b", "\\bmat4x4\\b", "\\bsampler1D\\b", "\\bimage1D\\b",
        "\\bsampler2D\\b", "\\bimage2D\\b", "\\bsampler3D\\b", "\\bimage3D\\b"
    };

    addRules(typePatterns, format);
}

void GLSLHighlighter::setupVariableRules()
{
    QTextCharFormat format;

    format.setForeground(variableColor);
    format.setFontWeight(QFont::Bold);
    // GLSL built-in special variables
    QStringList variablePatterns{
        "\\bgl_FragCoord\\b", "\\bgl_FrontFacing\\b", "\\bgl_ClipDistance\\b",
        "\\bgl_CullDistance\\b", "\\bgl_PointCoord\\b", "\\bgl_PrimitiveID\\b",
        "\\bgl_SampleID\\b", "\\bgl_SamplePosition\\b", "\\bgl_SampleMaskIn\\b",
        "\\bgl_Layer\\b", "\\bgl_ViewportIndex\\b", "\\bgl_HelperInvocation\\b",
        "\\bgl_FragDepth\\b", "\\bgl_SampleMask\\b"
    };

    addRules(variablePatterns, format);
}

void GLSLHighlighter::setupFunctionRules()
{
    QTextCharFormat format;

    format.setForeground(functionColor);
    format.setFontWeight(QFont::Bold);
    // GLSL built-in functions
    QStringList functionPatterns{
        "\\bradians\\b", "\\bdegrees\\b", "\\bsin\\b", "\\bcos\\b", "\\btan\\b",
        "\\basin\\b", "\\bacos\\b", "\\batan\\b", "\\bsinh\\b", "\\bcosh\\b",
        "\\btanh\\b", "\\basinh\\b", "\\bacosh\\b", "\\batanh\\b", "\\bpow\\b",
        "\\bexp\\b", "\\blog\\b", "\\bexp2\\b", "\\blog2\\b", "\\bsqrt\\b",
        "\\binversesqrt\\b", "\\babs\\b", "\\bsign\\b", "\\bfloor\\b",
        "\\btrunc\\b", "\\bround\\b", "\\broundEven\\b", "\\bceil\\b",
        "\\bfract\\b", "\\bmod\\b", "\\bmodf\\b", "\\bmin\\b", "\\bmax\\b",
        "\\bclamp\\b", "\\bmix\\b", "\\bstep\\b", "\\bsmoothstep\\b",
        "\\bisnan\\b", "\\bisinf\\b", "\\bfma\\b", "\\bfrexp\\b", "\\bldexp\\b",
        "\\blength\\b", "\\bdistance\\b", "\\bdot\\b", "\\bcross\\b",
        "\\bnormalize\\b", "\\bftransform\\b", "\\bfaceforward\\b", "\\breflect\\b",
        "\\brefract\\b", "\\bmatrixCompMult\\b", "\\bouterProduct\\b",
        "\\btranspose\\b", "\\bdeterminant\\b", "\\binverse\\b", "\\blessThan\\b",
        "\\blessThanEqual\\b", "\\bgreaterThan\\b", "\\bgreaterThanEqual\\b",
        "\\bequal\\b", "\\bnotEqual\\b", "\\btextureSize\\b", "\\btextureQueryLod\\b",
        "\\btextureQueryLevels\\b", "\\btextureSamples\\b", "\\btexture\\b",
        "\\btextureProj\\b", "\\btextureLod\\b", "\\btextureOffset\\b",
        "\\btexelFetch\\b", "\\btexelFetchOffset\\b", "\\btextureProjOffset\\b",
        "\\btextureLodOffset\\b", "\\btextureProjLod\\b", "\\btextureProjLodOffset\\b",
        "\\btextureGrad\\b", "\\btextureGradOffset\\b", "\\btextureProjGrad\\b",
        "\\btextureProjGradOffset\\b"
    };

    addRules(functionPatterns, format);
}

void GLSLHighlighter::addRules(const QStringList &patterns,
                               const QTextCharFormat &format)
{
    for (auto &pattern : patterns) {
        addRule(pattern, format);
    }
}

void GLSLHighlighter::addRule(const QString &pattern, const QTextCharFormat &format)
{
    rules.append(HighlightingRule(QRegularExpression(pattern), format));
}

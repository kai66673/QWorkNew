/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

// #include "../../pythoneditorconstants.h"
// #include "../../pythoneditorplugin.h"
#include "pythonscanner.h"

#include <QString>
#include <QSet>

namespace PythonEditor {
namespace Internal {

/*******************************************************************************
 * List of Python magic methods and attributes
 ******************************************************************************/
static Format classifyMagics2( const QString &word )
{
    if ( word[0] == 'o' ) {
        if ( word[1] == 'r' ) {
            return Format_MagicAttr;
        }
    } else if ( word[0] == 'e' ) {
        if ( word[1] == 'q' ) {
            return Format_MagicAttr;
        }
    } else if ( word[0] == 'n' ) {
        if ( word[1] == 'e' ) {
            return Format_MagicAttr;
        }
    } else if ( word[0] == 'g' ) {
        if ( word[1] == 't' ) {
            return Format_MagicAttr;
        } else if ( word[1] == 'e' ) {
            return Format_MagicAttr;
        }
    } else if ( word[0] == 'l' ) {
        if ( word[1] == 't' ) {
            return Format_MagicAttr;
        } else if ( word[1] == 'e' ) {
            return Format_MagicAttr;
        }
    }

    return Format_Identifier;
}

static Format classifyMagics3( const QString &word )
{
    if ( word[0] == 'a' ) {
        if ( word[1] == 'b' ) {
            if ( word[2] == 's' ) {
                return Format_MagicAttr;
            }
        } else if ( word[1] == 'd' ) {
            if ( word[2] == 'd' ) {
                return Format_MagicAttr;
            }
        } else if ( word[1] == 'n' ) {
            if ( word[2] == 'd' ) {
                return Format_MagicAttr;
            }
        }
    } else if ( word[0] == 'c' ) {
        if ( word[1] == 'm' ) {
            if ( word[2] == 'p' ) {
                return Format_MagicAttr;
            }
        }
    } else if ( word[0] == 'd' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 'l' ) {
                return Format_MagicAttr;
            }
        } else if ( word[1] == 'o' ) {
            if ( word[2] == 'c' ) {
                return Format_MagicAttr;
            }
        }
    } else if ( word[0] == 'h' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 'x' ) {
                return Format_MagicAttr;
            }
        }
    } else if ( word[0] == 'i' ) {
        if ( word[1] == 'n' ) {
            if ( word[2] == 't' ) {
                return Format_MagicAttr;
            } else if ( word[2] == 'v' ) {
                return Format_MagicAttr;
            }
        }
    } else if ( word[0] == 'l' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 'n' ) {
                return Format_MagicAttr;
            }
        }
    } else if ( word[0] == 'm' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 'd' ) {
                return Format_MagicAttr;
            }
        } else  if ( word[1] == 'u' ) {
            if ( word[2] == 'l' ) {
                return Format_MagicAttr;
            }
        }
    } else if ( word[0] == 'n' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 'g' ) {
                return Format_MagicAttr;
            }
        }
    } else if ( word[0] == 'o' ) {
        if ( word[1] == 'c' ) {
            if ( word[2] == 't' ) {
                return Format_MagicAttr;;
            }
        }
    } else if ( word[0] == 'p' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 's' ) {
                return Format_MagicAttr;
            } else if ( word[2] == 'w' ) {
                return Format_MagicAttr;
            }
        }
    } else if ( word[0] == 's' ) {
        if ( word[1] == 't' ) {
            if ( word[2] == 's' ) {
                return Format_MagicAttr;
            }
        } else if ( word[1] == 'u' ) {
            if ( word[2] == 'b' ) {
                return Format_MagicAttr;
            }
        }
    } else if ( word[0] == 'x' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 'r' ) {
                return Format_MagicAttr;
            }
        }
    }

    return Format_Identifier;
}

static Format classifyMagics4( const QString &word )
{
    if ( word[0] == 'c' ) {
        if ( word[1] == 'a' ) {
            if ( word[2] == 'l' ) {
                if ( word[3] == 'l' ) {
                    return Format_MagicAttr;
                }
            }
        } else if ( word[1] == 'o' ) {
            if ( word[2] == 'p' ) {
                if ( word[3] == 'y' ) {
                    return Format_MagicAttr;
                }
            }
        }
    } else if ( word[0] == 'd' ) {
        if ( word[1] == 'i' ) {
            if ( word[2] == 'c' ) {
                if ( word[3] == 't' ) {
                    return Format_MagicAttr;
                }
            }
        }
    } else if ( word[0] == 'h' ) {
        if ( word[1] == 'a' ) {
            if ( word[2] == 's' ) {
                if ( word[3] == 'h' ) {
                    return Format_MagicAttr;
                }
            }
        }
    } else if ( word[0] == 'i' ) {
        if ( word[1] == 'n' ) {
            if ( word[2] == 'i' ) {
                if ( word[3] == 't' ) {
                    return Format_MagicAttr;
                }
            }
        } else if ( word[1] == 't' ) {
            if ( word[2] == 'e' ) {
                if ( word[3] == 'r' ) {
                    return Format_MagicAttr;
                }
            }
        }
    } else if ( word[0] == 'l' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 'n' ) {
                if ( word[3] == 'g' ) {
                    return Format_MagicAttr;
                }
            }
        }
    } else if ( word[0] == 'n' ) {
        if ( word[1] == 'a' ) {
            if ( word[2] == 'm' ) {
                if ( word[3] == 'e' ) {
                    return Format_MagicAttr;
                }
            }
        }
    } else if ( word[0] == 'r' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 'p' ) {
                if ( word[3] == 'r' ) {
                    return Format_MagicAttr;
                }
            }
        }
    }


    return Format_Identifier;
}

static Format classifyMagics5( const QString &word )
{
    if ( word[0] == 'b' ) {
        if ( word[1] == 'a' ) {
            if ( word[2] == 's' ) {
                if ( word[3] == 'e' ) {
                    if ( word[4] == 's' ) {
                        return Format_MagicAttr;
                    }
                }
            }
        }
    } else if ( word[0] == 'f' ) {
        if ( word[1] == 'l' ) {
            if ( word[2] == 'o' ) {
                if ( word[3] == 'a' ) {
                    if ( word[4] == 't' ) {
                        return Format_MagicAttr;
                    }
                }
            }
        }
    } else if ( word[0] == 'i' ) {
        if ( word[1] == 'n' ) {
            if ( word[2] == 'd' ) {
                if ( word[3] == 'e' ) {
                    if ( word[4] == 'x' ) {
                        return Format_MagicAttr;
                    }
                }
            }
        }
    } else if ( word[0] == 't' ) {
        if ( word[1] == 'r' ) {
            if ( word[2] == 'u' ) {
                if ( word[3] == 'n' ) {
                    if ( word[4] == 'c' ) {
                        return Format_MagicAttr;
                    }
                }
            }
        }
    }

    return Format_Identifier;
}

static Format classifyMagics6( const QString &word )
{
    if ( word[0] == 'd' ) {
        if ( word[1] == 'i' ) {
            if ( word[2] == 'v' ) {
                if ( word[3] == 'm' ) {
                    if ( word[4] == 'o' ) {
                        if ( word[5] == 'd' ) {
                            return Format_MagicAttr;
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'f' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 'r' ) {
                if ( word[3] == 'm' ) {
                    if ( word[4] == 'a' ) {
                        if ( word[5] == 't' ) {
                            return Format_MagicAttr;
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'l' ) {
        if ( word[1] == 's' ) {
            if ( word[2] == 'h' ) {
                if ( word[3] == 'i' ) {
                    if ( word[4] == 'f' ) {
                        if ( word[5] == 't' ) {
                            return Format_MagicAttr;
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'm' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 'd' ) {
                if ( word[3] == 'u' ) {
                    if ( word[4] == 'l' ) {
                        if ( word[5] == 'e' ) {
                            return Format_MagicAttr;
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'r' ) {
        if ( word[1] == 's' ) {
            if ( word[2] == 'h' ) {
                if ( word[3] == 'i' ) {
                    if ( word[4] == 'f' ) {
                        if ( word[5] == 't' ) {
                            return Format_MagicAttr;
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 's' ) {
        if ( word[1] == 'i' ) {
            if ( word[2] == 'z' ) {
                if ( word[3] == 'e' ) {
                    if ( word[4] == 'o' ) {
                        if ( word[5] == 'f' ) {
                            return Format_MagicAttr;
                        }
                    }
                }
            }
        }
    }

    return Format_Identifier;
}

static Format classifyMagics7( const QString &word )
{

    if ( word[0] == 'c' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 'm' ) {
                if ( word[3] == 'p' ) {
                    if ( word[4] == 'l' ) {
                        if ( word[5] == 'e' ) {
                            if ( word[6] == 'x' ) {
                                return Format_MagicAttr;
                            }
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'd' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 'l' ) {
                if ( word[3] == 'a' ) {
                    if ( word[4] == 't' ) {
                        if ( word[5] == 't' ) {
                            if ( word[6] == 'r' ) {
                                return Format_MagicAttr;
                            }
                        }
                    }
                } else if ( word[3] == 'i' ) {
                    if ( word[4] == 't' ) {
                        if ( word[5] == 'e' ) {
                            if ( word[6] == 'm' ) {
                                return Format_MagicAttr;
                            }
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'g' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 't' ) {
                if ( word[3] == 'a' ) {
                    if ( word[4] == 't' ) {
                        if ( word[5] == 't' ) {
                            if ( word[6] == 'r' ) {
                                return Format_MagicAttr;
                            }
                        }
                    }
                } else if ( word[3] == 'i' ) {
                    if ( word[4] == 't' ) {
                        if ( word[5] == 'e' ) {
                            if ( word[6] == 'm' ) {
                                return Format_MagicAttr;
                            }
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'n' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 'n' ) {
                if ( word[3] == 'z' ) {
                    if ( word[4] == 'e' ) {
                        if ( word[5] == 'r' ) {
                            if ( word[6] == 'o' ) {
                                return Format_MagicAttr;
                            }
                        }
                    }
                }
            }
        }
    }else if ( word[0] == 's' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 't' ) {
                if ( word[3] == 'a' ) {
                    if ( word[4] == 't' ) {
                        if ( word[5] == 't' ) {
                            if ( word[6] == 'r' ) {
                                return Format_MagicAttr;
                            }
                        }
                    }
                } else if ( word[3] == 'i' ) {
                    if ( word[4] == 't' ) {
                        if ( word[5] == 'e' ) {
                            if ( word[6] == 'm' ) {
                                return Format_MagicAttr;
                            }
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 't' ) {
        if ( word[1] == 'r' ) {
            if ( word[2] == 'u' ) {
                if ( word[3] == 'e' ) {
                    if ( word[4] == 'd' ) {
                        if ( word[5] == 'i' ) {
                            if ( word[6] == 'v' ) {
                                return Format_MagicAttr;
                            }
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'u' ) {
        if ( word[1] == 'n' ) {
            if ( word[2] == 'i' ) {
                if ( word[3] == 'c' ) {
                    if ( word[4] == 'o' ) {
                        if ( word[5] == 'd' ) {
                            if ( word[6] == 'e' ) {
                                return Format_MagicAttr;
                            }
                        }
                    }
                }
            }
        }
    }
    return Format_Identifier;
}

static Format classifyMagics8( const QString &word )
{
    if ( word[0] == 'c' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 'n' ) {
                if ( word[3] == 't' ) {
                    if ( word[4] == 'a' ) {
                        if ( word[5] == 'i' ) {
                            if ( word[6] == 'n' ) {
                                if ( word[7] == 's' ) {
                                    return Format_MagicAttr;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'd' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 'e' ) {
                if ( word[3] == 'p' ) {
                    if ( word[4] == 'c' ) {
                        if ( word[5] == 'o' ) {
                            if ( word[6] == 'p' ) {
                                if ( word[7] == 'y' ) {
                                    return Format_MagicAttr;
                                }
                            }
                        }
                    }
                }
            } else if ( word[2] == 'l' ) {
                if ( word[3] == 's' ) {
                    if ( word[4] == 'l' ) {
                        if ( word[5] == 'i' ) {
                            if ( word[6] == 'c' ) {
                                if ( word[7] == 'e' ) {
                                    return Format_MagicAttr;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'g' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 't' ) {
                if ( word[3] == 's' ) {
                    if ( word[4] == 'l' ) {
                        if ( word[5] == 'i' ) {
                            if ( word[6] == 'c' ) {
                                if ( word[7] == 'e' ) {
                                    return Format_MagicAttr;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'f' ) {
        if ( word[1] == 'l' ) {
            if ( word[2] == 'o' ) {
                if ( word[3] == 'o' ) {
                    if ( word[4] == 'r' ) {
                        if ( word[5] == 'd' ) {
                            if ( word[6] == 'i' ) {
                                if ( word[7] == 'v' ) {
                                    return Format_MagicAttr;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 's' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 't' ) {
                if ( word[3] == 's' ) {
                    if ( word[4] == 'l' ) {
                        if ( word[5] == 'i' ) {
                            if ( word[6] == 'c' ) {
                                if ( word[7] == 'e' ) {
                                    return Format_MagicAttr;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'r' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 's' ) {
                if ( word[3] == 'e' ) {
                    if ( word[4] == 'r' ) {
                        if ( word[5] == 'v' ) {
                            if ( word[6] == 'e' ) {
                                if ( word[7] == 'd' ) {
                                    return Format_MagicAttr;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return Format_Identifier;
}

static Format classifyMagics( const QString &word )
{
    if ( !word.endsWith("__") )
        return Format_Identifier;

    QString innerWord = word.mid(2, word.length() - 4);
    switch ( innerWord.length() ) {
        case 2: return classifyMagics2(innerWord);
        case 3: return classifyMagics3(innerWord);
        case 4: return classifyMagics4(innerWord);
        case 5: return classifyMagics5(innerWord);
        case 6: return classifyMagics6(innerWord);
        case 7: return classifyMagics7(innerWord);
        case 8: return classifyMagics8(innerWord);
    }

    return Format_Identifier;
}

/*******************************************************************************
 * List of python built-in functions and objects
 ******************************************************************************/
static Format classifyWord2( const QString &word )
{
    if ( word[0] == 'a' ) {
        if ( word[1] == 's' ) {
            return Format_Keyword;
        }
    } else if ( word[0] == 'i' ) {
        if ( word[1] == 'f' ) {
            return Format_Keyword;
        } else if ( word[1] == 'n' ) {
            return Format_Keyword;
        } else if ( word[1] == 's' ) {
            return Format_Keyword;
        }
    } else if ( word[0] == 'o' ) {
        if ( word[1] == 'r' ) {
            return Format_Keyword;
        }
    }

    return Format_Identifier;
}

static Format classifyWord3( const QString &word )
{
    if ( word[0] == 'a' ) {
        if ( word[1] == 'b' ) {
            if ( word[2] == 's' ) {
                return Format_Type;
            }
        } else if ( word[1] == 'n' ) {
            if ( word[2] == 'd' ) {
                return Format_Keyword;
            }
        }
    } else if ( word[0] == 'c' ) {
        if ( word[1] == 'h' ) {
            if ( word[2] == 'r' ) {
                return Format_Type;
            }
        }
    } else if ( word[0] == 'd' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 'f' ) {
                return Format_Keyword;
            } else if ( word[2] == 'l' ) {
                return Format_Keyword;
            }
        }
    } else if ( word[0] == 'f' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 'r' ) {
                return Format_Keyword;
            }
        }
    } else if ( word[0] == 'h' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 'x' ) {
                return Format_Type;
            }
        }
    } else if ( word[0] == 'i' ) {
        if ( word[1] == 'n' ) {
            if ( word[2] == 't' ) {
                return Format_Type;
            }
        }
    } else if ( word[0] == 'l' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 'n' ) {
                return Format_Type;
            }
        }
    } else if ( word[0] == 'n' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 't' ) {
                return Format_Keyword;
            }
        }
    } else if ( word[0] == 'o' ) {
        if ( word[1] == 'c' ) {
            if ( word[2] == 't' ) {
                return Format_Type;
            }
        } else if ( word[1] == 'r' ) {
            if ( word[2] == 'd' ) {
                return Format_Type;
            }
        }
    } else if ( word[0] == 't' ) {
        if ( word[1] == 'r' ) {
            if ( word[2] == 'y' ) {
                return Format_Keyword;
            }
        }
    }

    return Format_Identifier;
}

static Format classifyWord4( const QString &word )
{
    if ( word[0] == 'e' ) {
        if ( word[1] == 'l' ) {
            if ( word[2] == 'i' ) {
                if ( word[3] == 'f' ) {
                    return Format_Keyword;
                }
            } else if ( word[2] == 's' ) {
                if ( word[3] == 'e' ) {
                    return Format_Keyword;
                }
            }
        } else if ( word[1] == 'x' ) {
            if ( word[2] == 'e' ) {
                if ( word[3] == 'c' ) {
                    return Format_Keyword;
                }
            }
        }
    } else if ( word[0] == 'f' ) {
        if ( word[1] == 'r' ) {
            if ( word[2] == 'o' ) {
                if ( word[3] == 'm' ) {
                    return Format_Keyword;
                }
            }
        }
    } else if ( word[0] == 'l' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 'n' ) {
                if ( word[3] == 'g' ) {
                    return Format_Type;
                }
            }
        }
    } else if ( word[0] == 'p' ) {
        if ( word[1] == 'a' ) {
            if ( word[2] == 's' ) {
                if ( word[3] == 's' ) {
                    return Format_Keyword;
                }
            }
        }
    } else if ( word[0] == 'w' ) {
        if ( word[1] == 'i' ) {
            if ( word[2] == 't' ) {
                if ( word[3] == 'h' ) {
                    return Format_Keyword;
                }
            }
        }
    } else if ( word[0] == 'N' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 'n' ) {
                if ( word[3] == 'e' ) {
                    return Format_Keyword;
                }
            }
        }
    } else if ( word[0] == 'T' ) {
        if ( word[1] == 'r' ) {
            if ( word[2] == 'u' ) {
                if ( word[3] == 'e' ) {
                    return Format_Keyword;
                }
            }
        }
    }

    return Format_Identifier;
}

static Format classifyWord5( const QString &word )
{
    if ( word[0] == 'b' ) {
        if ( word[1] == 'r' ) {
            if ( word[2] == 'e' ) {
                if ( word[3] == 'a' ) {
                    if ( word[4] == 'k' ) {
                        return Format_Keyword;
                    }
                }
            }
        }
    } else if ( word[0] == 'c' ) {
        if ( word[1] == 'l' ) {
            if ( word[2] == 'a' ) {
                if ( word[3] == 's' ) {
                    if ( word[4] == 's' ) {
                        return Format_Keyword;
                    }
                }
            }
        }
    } else if ( word[0] == 'f' ) {
        if ( word[1] == 'l' ) {
            if ( word[2] == 'o' ) {
                if ( word[3] == 'a' ) {
                    if ( word[4] == 't' ) {
                        return Format_Type;
                    }
                }
            }
        }
    } else if ( word[0] == 'p' ) {
        if ( word[1] == 'r' ) {
            if ( word[2] == 'i' ) {
                if ( word[3] == 'n' ) {
                    if ( word[4] == 't' ) {
                        return Format_Keyword;
                    }
                }
            }
        }
    } else if ( word[0] == 'r' ) {
        if ( word[1] == 'a' ) {
            if ( word[2] == 'i' ) {
                if ( word[3] == 's' ) {
                    if ( word[4] == 'e' ) {
                        return Format_Keyword;
                    }
                }
            } else if ( word[2] == 'n' ) {
                if ( word[3] == 'g' ) {
                    if ( word[4] == 'e' ) {
                        return Format_Type;
                    }
                }
            }
        }
    } else if ( word[0] == 'w' ) {
        if ( word[1] == 'h' ) {
            if ( word[2] == 'i' ) {
                if ( word[3] == 'l' ) {
                    if ( word[4] == 'e' ) {
                        return Format_Keyword;
                    }
                }
            }
        }
    } else if ( word[0] == 'y' ) {
        if ( word[1] == 'i' ) {
            if ( word[2] == 'e' ) {
                if ( word[3] == 'l' ) {
                    if ( word[4] == 'd' ) {
                        return Format_Keyword;
                    }
                }
            }
        }
    } else if ( word[0] == 'F' ) {
        if ( word[1] == 'a' ) {
            if ( word[2] == 'l' ) {
                if ( word[3] == 's' ) {
                    if ( word[4] == 'e' ) {
                        return Format_Type;
                    }
                }
            }
        }
    }

    return Format_Identifier;
}

static Format classifyWord6( const QString &word )
{
    if ( word[0] == 'a' ) {
        if ( word[1] == 's' ) {
            if ( word[2] == 's' ) {
                if ( word[3] == 'e' ) {
                    if ( word[4] == 'r' ) {
                        if ( word[5] == 't' ) {
                            return Format_Keyword;
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'e' ) {
        if ( word[1] == 'x' ) {
            if ( word[2] == 'c' ) {
                if ( word[3] == 'e' ) {
                    if ( word[4] == 'p' ) {
                        if ( word[5] == 't' ) {
                            return Format_Keyword;
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'g' ) {
        if ( word[1] == 'l' ) {
            if ( word[2] == 'o' ) {
                if ( word[3] == 'b' ) {
                    if ( word[4] == 'a' ) {
                        if ( word[5] == 'l' ) {
                            return Format_Keyword;
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'i' ) {
        if ( word[1] == 'm' ) {
            if ( word[2] == 'p' ) {
                if ( word[3] == 'o' ) {
                    if ( word[4] == 'r' ) {
                        if ( word[5] == 't' ) {
                            return Format_Keyword;
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'l' ) {
        if ( word[1] == 'a' ) {
            if ( word[2] == 'm' ) {
                if ( word[3] == 'b' ) {
                    if ( word[4] == 'd' ) {
                        if ( word[5] == 'a' ) {
                            return Format_Keyword;
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'r' ) {
        if ( word[1] == 'e' ) {
            if ( word[2] == 't' ) {
                if ( word[3] == 'u' ) {
                    if ( word[4] == 'r' ) {
                        if ( word[5] == 'n' ) {
                            return Format_Keyword;
                        }
                    }
                }
            }
        }
    } else if ( word[0] == 'x' ) {
        if ( word[1] == 'r' ) {
            if ( word[2] == 'a' ) {
                if ( word[3] == 'n' ) {
                    if ( word[4] == 'g' ) {
                        if ( word[5] == 'e' ) {
                            return Format_Type;
                        }
                    }
                }
            }
        }
    }

    return Format_Identifier;
}

static Format classifyWord7( const QString &word )
{
    if ( word[0] == 'f' ) {
        if ( word[1] == 'i' ) {
            if ( word[2] == 'n' ) {
                if ( word[3] == 'a' ) {
                    if ( word[4] == 'l' ) {
                        if ( word[5] == 'l' ) {
                            if ( word[6] == 'y' ) {
                                return Format_Keyword;
                            }
                        }
                    }
                }
            }
        }
    }
    return Format_Identifier;
}

static Format classifyWord8( const QString &word )
{
    if ( word[0] == 'c' ) {
        if ( word[1] == 'o' ) {
            if ( word[2] == 'n' ) {
                if ( word[3] == 't' ) {
                    if ( word[4] == 'i' ) {
                        if ( word[5] == 'n' ) {
                            if ( word[6] == 'u' ) {
                                if ( word[7] == 'e' ) {
                                    return Format_Keyword;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return Format_Identifier;
}

static Format classifyWord( const QString &word )
{
    if ( word.startsWith("__") )
        return classifyMagics(word);

    switch ( word.length() ) {
        case 2: return classifyWord2(word);
        case 3: return classifyWord3(word);
        case 4: return classifyWord4(word);
        case 5: return classifyWord5(word);
        case 6: return classifyWord6(word);
        case 7: return classifyWord7(word);
        case 8: return classifyWord8(word);
    }

    return Format_Identifier;
}

Scanner::Scanner(const QChar *text, const int length, TextEditor::Parentheses *parentheses)
    : m_src(text, length)
    , m_state(0)
    , m_braceDepth(0)
    , m_parentheses(parentheses)
{
}

Scanner::~Scanner()
{
}

void Scanner::setState(int state)
{
    m_state = state;
}

int Scanner::state() const
{
    return m_state;
}

void Scanner::setBraceDepth(int depth)
{
    m_braceDepth = depth;
}

int Scanner::braceDepth() const
{
    return m_braceDepth;
}

FormatToken Scanner::read()
{
    m_src.setAnchor();
    if (m_src.isEnd())
        return FormatToken(Format_EndOfBlock, m_src.anchor(), 0);

    State state;
    QChar saved;
    parseState(state, saved);
    switch (state) {
    case State_String:
        return readStringLiteral(saved);
    case State_MultiLineString:
        return readMultiLineStringLiteral(saved);
    default:
        return onDefaultState();
    }
}

QString Scanner::value(const FormatToken &tk) const
{
    return m_src.value(tk.begin(), tk.length());
}

FormatToken Scanner::onDefaultState()
{
    QChar first = m_src.peek();
    m_src.move();

    if (first == QLatin1Char('\\') && m_src.peek() == QLatin1Char('\n')) {
        m_src.move();
        return FormatToken(Format_Whitespace, m_src.anchor(), 2);
    }

    if (first == QLatin1Char('.') && m_src.peek().isDigit())
        return readFloatNumber();

    if (first == QLatin1Char('\'') || first == QLatin1Char('\"'))
        return readStringLiteral(first);

    if (first.isLetter() || (first == QLatin1Char('_')))
        return readIdentifier();

    if (first.isDigit())
        return readNumber();

    if (first == QLatin1Char('#')) {
        if (m_src.peek() == QLatin1Char('#'))
            return readDoxygenComment();
        return readComment();
    }

    if (first.isSpace())
        return readWhiteSpace();

    return readOperator();
}

/**
 * @brief Lexer::passEscapeCharacter
 * @return returns true if escape sequence doesn't end with newline
 */
void Scanner::checkEscapeSequence(QChar quoteChar)
{
    if (m_src.peek() == QLatin1Char('\\')) {
        m_src.move();
        QChar ch = m_src.peek();
        if (ch == QLatin1Char('\n') || ch.isNull())
            saveState(State_String, quoteChar);
    }
}

/**
  reads single-line string literal, surrounded by ' or " quotes
  */
FormatToken Scanner::readStringLiteral(QChar quoteChar)
{
    QChar ch = m_src.peek();
    if (ch == quoteChar && m_src.peek(1) == quoteChar) {
        saveState(State_MultiLineString, quoteChar);
        return readMultiLineStringLiteral(quoteChar);
    }

    while (ch != quoteChar && !ch.isNull()) {
        checkEscapeSequence(quoteChar);
        m_src.move();
        ch = m_src.peek();
    }
    if (ch == quoteChar)
        clearState();
    m_src.move();
    return FormatToken(Format_String, m_src.anchor(), m_src.length());
}

/**
  reads multi-line string literal, surrounded by ''' or """ sequencies
  */
FormatToken Scanner::readMultiLineStringLiteral(QChar quoteChar)
{
    for (;;) {
        QChar ch = m_src.peek();
        if (ch.isNull())
            break;
        if (ch == quoteChar
                && (m_src.peek(1) == quoteChar)
                && (m_src.peek(2) == quoteChar)) {
            clearState();
            m_src.move();
            m_src.move();
            m_src.move();
            break;
        }
        m_src.move();
    }

    return FormatToken(Format_String, m_src.anchor(), m_src.length());
}

/**
  reads identifier and classifies it
  */
FormatToken Scanner::readIdentifier()
{
    QChar ch = m_src.peek();
    while (ch.isLetterOrNumber() || (ch == QLatin1Char('_'))) {
        m_src.move();
        ch = m_src.peek();
    }
    QString value = m_src.value();

    Format tkFormat = Format_Identifier;
    if (value == QLatin1String("self"))
        tkFormat = Format_ClassField;
    else
        tkFormat = classifyWord(value);

    return FormatToken(tkFormat, m_src.anchor(), m_src.length());
}

inline static bool isHexDigit(QChar ch)
{
    return (ch.isDigit()
            || (ch >= QLatin1Char('a') && ch <= QLatin1Char('f'))
            || (ch >= QLatin1Char('A') && ch <= QLatin1Char('F')));
}

inline static bool isOctalDigit(QChar ch)
{
    return (ch.isDigit() && ch != QLatin1Char('8') && ch != QLatin1Char('9'));
}

inline static bool isBinaryDigit(QChar ch)
{
    return (ch == QLatin1Char('0') || ch == QLatin1Char('1'));
}

inline static bool isValidIntegerSuffix(QChar ch)
{
    return (ch == QLatin1Char('l') || ch == QLatin1Char('L'));
}

inline static bool isValidComplexSuffix(QChar ch)
{
    return (ch == QLatin1Char('j') || ch == QLatin1Char('J'));
}

FormatToken Scanner::readNumber()
{
    if (!m_src.isEnd()) {
        QChar ch = m_src.peek();
        if (ch.toLower() == QLatin1Char('b')) {
            m_src.move();
            while (isBinaryDigit(m_src.peek()))
                m_src.move();
        } else if (ch.toLower() == QLatin1Char('o')) {
            m_src.move();
            while (isOctalDigit(m_src.peek()))
                m_src.move();
        } else if (ch.toLower() == QLatin1Char('x')) {
            m_src.move();
            while (isHexDigit(m_src.peek()))
                m_src.move();
        } else { // either integer or float number
            return readFloatNumber();
        }
        if (isValidIntegerSuffix(m_src.peek()))
            m_src.move();
    }
    return FormatToken(Format_Number, m_src.anchor(), m_src.length());
}

FormatToken Scanner::readFloatNumber()
{
    enum
    {
        State_INTEGER,
        State_FRACTION,
        State_EXPONENT
    } state;
    state = (m_src.peek(-1) == QLatin1Char('.')) ? State_FRACTION : State_INTEGER;

    for (;;) {
        QChar ch = m_src.peek();
        if (ch.isNull())
            break;

        if (state == State_INTEGER) {
            if (ch == QLatin1Char('.'))
                state = State_FRACTION;
            else if (!ch.isDigit())
                break;
        } else if (state == State_FRACTION) {
            if (ch == QLatin1Char('e') || ch == QLatin1Char('E')) {
                QChar next = m_src.peek(1);
                QChar next2 = m_src.peek(2);
                bool isExp = next.isDigit()
                        || (((next == QLatin1Char('-')) || (next == QLatin1Char('+'))) && next2.isDigit());
                if (isExp) {
                    m_src.move();
                    state = State_EXPONENT;
                } else {
                    break;
                }
            } else if (!ch.isDigit()) {
                break;
            }
        } else if (!ch.isDigit()) {
            break;
        }
        m_src.move();
    }

    QChar ch = m_src.peek();
    if ((state == State_INTEGER && (ch == QLatin1Char('l') || ch == QLatin1Char('L')))
            || (ch == QLatin1Char('j') || ch == QLatin1Char('J')))
        m_src.move();

    return FormatToken(Format_Number, m_src.anchor(), m_src.length());
}

/**
  reads single-line python comment, started with "#"
  */
FormatToken Scanner::readComment()
{
    QChar ch = m_src.peek();
    while (ch != QLatin1Char('\n') && !ch.isNull()) {
        m_src.move();
        ch = m_src.peek();
    }
    return FormatToken(Format_Comment, m_src.anchor(), m_src.length());
}

/**
  reads single-line python doxygen comment, started with "##"
  */
FormatToken Scanner::readDoxygenComment()
{
    QChar ch = m_src.peek();
    while (ch != QLatin1Char('\n') && !ch.isNull()) {
        m_src.move();
        ch = m_src.peek();
    }
    return FormatToken(Format_Doxygen, m_src.anchor(), m_src.length());
}

/**
  reads whitespace
  */
FormatToken Scanner::readWhiteSpace()
{
    while (m_src.peek().isSpace())
        m_src.move();
    return FormatToken(Format_Whitespace, m_src.anchor(), m_src.length());
}

/**
  reads punctuation symbols, excluding some special
  */
FormatToken Scanner::readOperator()
{
    QChar ch = m_src.peek(-1);
    int pos = m_src.position() - 1;
    switch ( ch.toLatin1() ) {
        case '{':
        case '[':
        case '(':
            if ( m_parentheses )
                m_parentheses->append(TextEditor::Parenthesis(TextEditor::Parenthesis::Opened, ch, pos));
            m_braceDepth++;
            break;
        case '}':
        case ']':
        case ')':
            m_braceDepth--;
            if ( m_braceDepth < 0 )
                m_braceDepth = 0;
            if ( m_parentheses )
                m_parentheses->append(TextEditor::Parenthesis(TextEditor::Parenthesis::Closed, ch, pos));
            break;
    }

    const QString EXCLUDED_CHARS = QLatin1String("\'\"_#");
    ch = m_src.peek();
    while (ch.isPunct() && !EXCLUDED_CHARS.contains(ch)) {
        pos++;
        switch ( ch.toLatin1() ) {
            case '{':
            case '[':
            case '(':
                if ( m_parentheses )
                    m_parentheses->append(TextEditor::Parenthesis(TextEditor::Parenthesis::Opened, ch, pos));
                m_braceDepth++;
                break;
            case '}':
            case ']':
            case ')':
                m_braceDepth--;
                if ( m_braceDepth < 0 )
                    m_braceDepth = 0;
                if ( m_parentheses )
                    m_parentheses->append(TextEditor::Parenthesis(TextEditor::Parenthesis::Closed, ch, pos));
                break;
        }

        m_src.move();
        ch = m_src.peek();
    }
    return FormatToken(Format_Operator, m_src.anchor(), m_src.length());
}

void Scanner::clearState()
{
    m_state = 0;
}

void Scanner::saveState(State state, QChar savedData)
{
    int bit3 = 0;
    if ( savedData == '\'' )
        bit3 = 0x04;
    m_state = state | bit3;
//    m_state = (state << 16) | static_cast<int>(savedData.unicode());
}

void Scanner::parseState(State &state, QChar &savedData) const
{
    state = static_cast<State>(m_state & 0x03);
    if ( m_state & 0x04 )
        savedData = '\'';
    else
        savedData = '\"';
//    state = static_cast<State>(m_state >> 16);
//    savedData = static_cast<ushort>(m_state);
}

} // namespace Internal
} // namespace PythonEditor

// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "Lexer.h"
#include "Token.h"

using namespace CPlusPlus;

static inline int classify2(const char *s, bool, bool) {
  if (s[0] == 'd') {
    if (s[1] == 'o') {
      return T_DO;
    }
  }
  else if (s[0] == 'i') {
    if (s[1] == 'f') {
      return T_IF;
    }
  }
  return T_IDENTIFIER;
}

static inline int classify3(const char *s, bool q, bool) {
  if (s[0] == 'a') {
    if (s[1] == 's') {
      if (s[2] == 'm') {
        return T_ASM;
      }
    }
  }
  else if (s[0] == 'f') {
    if (s[1] == 'o') {
      if (s[2] == 'r') {
        return T_FOR;
      }
    }
  }
  else if (s[0] == 'i') {
    if (s[1] == 'n') {
      if (s[2] == 't') {
        return TT_INT;
      }
    }
  }
  else if (s[0] == 'n') {
    if (s[1] == 'e') {
      if (s[2] == 'w') {
        return T_NEW;
      }
    }
  }
  else if (s[0] == 't') {
    if (s[1] == 'r') {
      if (s[2] == 'y') {
        return T_TRY;
      }
    }
  }
  else if (q && s[0] == 'Q') {
    if (s[1] == '_') {
      if (s[2] == 'D') {
        return T_Q_D;
      }
      else if (s[2] == 'Q') {
        return T_Q_Q;
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify4(const char *s, bool q, bool) {
  if (s[0] == 'a') {
    if (s[1] == 'u') {
      if (s[2] == 't') {
        if (s[3] == 'o') {
          return T_AUTO;
        }
      }
    }
  }
  else if (s[0] == 'b') {
    if (s[1] == 'o') {
      if (s[2] == 'o') {
        if (s[3] == 'l') {
          return TT_BOOL;
        }
      }
    }
  }
  else if (s[0] == 'c') {
    if (s[1] == 'a') {
      if (s[2] == 's') {
        if (s[3] == 'e') {
          return T_CASE;
        }
      }
    }
    else if (s[1] == 'h') {
      if (s[2] == 'a') {
        if (s[3] == 'r') {
          return TT_CHAR;
        }
      }
    }
  }
  else if (s[0] == 'e') {
    if (s[1] == 'l') {
      if (s[2] == 's') {
        if (s[3] == 'e') {
          return T_ELSE;
        }
      }
    }
    else if (s[1] == 'n') {
      if (s[2] == 'u') {
        if (s[3] == 'm') {
          return T_ENUM;
        }
      }
    }
  }
  else if (s[0] == 'g') {
    if (s[1] == 'o') {
      if (s[2] == 't') {
        if (s[3] == 'o') {
          return T_GOTO;
        }
      }
    }
  }
  else if (s[0] == 'l') {
    if (s[1] == 'o') {
      if (s[2] == 'n') {
        if (s[3] == 'g') {
          return TT_LONG;
        }
      }
    }
  }
  else if (s[0] == 't') {
    if (s[1] == 'h') {
      if (s[2] == 'i') {
        if (s[3] == 's') {
          return T_THIS;
        }
      }
    }
    else if (s[1] == 'r') {
      if (s[2] == 'u') {
        if (s[3] == 'e') {
          return T_TRUE;
        }
      }
    }
  }
  else if (s[0] == 'v') {
    if (s[1] == 'o') {
      if (s[2] == 'i') {
        if (s[3] == 'd') {
          return T_VOID;
        }
      }
    }
  }
  else if (q && s[0] == 'S') {
    if (s[1] == 'L') {
      if (s[2] == 'O') {
        if (s[3] == 'T') {
          return T_SLOT;
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify5(const char *s, bool q, bool) {
  if (s[0] == '_') {
    if (s[1] == '_') {
      if (s[2] == 'a') {
        if (s[3] == 's') {
          if (s[4] == 'm') {
            return T___ASM;
          }
        }
      }
    }
  }
  else if (s[0] == 'b') {
    if (s[1] == 'r') {
      if (s[2] == 'e') {
        if (s[3] == 'a') {
          if (s[4] == 'k') {
            return T_BREAK;
          }
        }
      }
    }
  }
  else if (s[0] == 'c') {
    if (s[1] == 'a') {
      if (s[2] == 't') {
        if (s[3] == 'c') {
          if (s[4] == 'h') {
            return T_CATCH;
          }
        }
      }
    }
    else if (s[1] == 'l') {
      if (s[2] == 'a') {
        if (s[3] == 's') {
          if (s[4] == 's') {
            return T_CLASS;
          }
        }
      }
    }
    else if (s[1] == 'o') {
      if (s[2] == 'n') {
        if (s[3] == 's') {
          if (s[4] == 't') {
            return T_CONST;
          }
        }
      }
    }
  }
  else if (s[0] == 'f') {
    if (s[1] == 'a') {
      if (s[2] == 'l') {
        if (s[3] == 's') {
          if (s[4] == 'e') {
            return T_FALSE;
          }
        }
      }
    }
    else if (s[1] == 'l') {
      if (s[2] == 'o') {
        if (s[3] == 'a') {
          if (s[4] == 't') {
            return TT_FLOAT;
          }
        }
      }
    }
  }
  else if (s[0] == 's') {
    if (s[1] == 'h') {
      if (s[2] == 'o') {
        if (s[3] == 'r') {
          if (s[4] == 't') {
            return TT_SHORT;
          }
        }
      }
    }
    else if (q) {
      if (s[1] == 'l') {
        if (s[2] == 'o') {
          if (s[3] == 't') {
            if (s[4] == 's') {
              return T_Q_SLOTS;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 't') {
    if (s[1] == 'h') {
      if (s[2] == 'r') {
        if (s[3] == 'o') {
          if (s[4] == 'w') {
            return T_THROW;
          }
        }
      }
    }
  }
  else if (s[0] == 'u') {
    if (s[1] == 'n') {
      if (s[2] == 'i') {
        if (s[3] == 'o') {
          if (s[4] == 'n') {
            return T_UNION;
          }
        }
      }
    }
    else if (s[1] == 's') {
      if (s[2] == 'i') {
        if (s[3] == 'n') {
          if (s[4] == 'g') {
            return T_USING;
          }
        }
      }
    }
  }
  else if (s[0] == 'w') {
    if (s[1] == 'h') {
      if (s[2] == 'i') {
        if (s[3] == 'l') {
          if (s[4] == 'e') {
            return T_WHILE;
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify6(const char *s, bool q, bool) {
  if (s[0] == 'd') {
    if (s[1] == 'e') {
      if (s[2] == 'l') {
        if (s[3] == 'e') {
          if (s[4] == 't') {
            if (s[5] == 'e') {
              return T_DELETE;
            }
          }
        }
      }
    }
    else if (s[1] == 'o') {
      if (s[2] == 'u') {
        if (s[3] == 'b') {
          if (s[4] == 'l') {
            if (s[5] == 'e') {
              return TT_DOUBLE;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'e') {
    if (s[1] == 'x') {
      if (s[2] == 'p') {
        if (s[3] == 'o') {
          if (s[4] == 'r') {
            if (s[5] == 't') {
              return T_EXPORT;
            }
          }
        }
      }
      else if (s[2] == 't') {
        if (s[3] == 'e') {
          if (s[4] == 'r') {
            if (s[5] == 'n') {
              return T_EXTERN;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'f') {
    if (s[1] == 'r') {
      if (s[2] == 'i') {
        if (s[3] == 'e') {
          if (s[4] == 'n') {
            if (s[5] == 'd') {
              return T_FRIEND;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'i') {
    if (s[1] == 'n') {
      if (s[2] == 'l') {
        if (s[3] == 'i') {
          if (s[4] == 'n') {
            if (s[5] == 'e') {
              return T_INLINE;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'p') {
    if (s[1] == 'u') {
      if (s[2] == 'b') {
        if (s[3] == 'l') {
          if (s[4] == 'i') {
            if (s[5] == 'c') {
              return T_PUBLIC;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'r') {
    if (s[1] == 'e') {
      if (s[2] == 't') {
        if (s[3] == 'u') {
          if (s[4] == 'r') {
            if (s[5] == 'n') {
              return T_RETURN;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 's') {
    if (s[1] == 'i') {
      if (s[2] == 'g') {
        if (s[3] == 'n') {
          if (s[4] == 'e') {
            if (s[5] == 'd') {
              return T_SIGNED;
            }
          }
        }
      }
      else if (s[2] == 'z') {
        if (s[3] == 'e') {
          if (s[4] == 'o') {
            if (s[5] == 'f') {
              return T_SIZEOF;
            }
          }
        }
      }
    }
    else if (s[1] == 't') {
      if (s[2] == 'a') {
        if (s[3] == 't') {
          if (s[4] == 'i') {
            if (s[5] == 'c') {
              return T_STATIC;
            }
          }
        }
      }
      else if (s[2] == 'r') {
        if (s[3] == 'u') {
          if (s[4] == 'c') {
            if (s[5] == 't') {
              return T_STRUCT;
            }
          }
        }
      }
    }
    else if (s[1] == 'w') {
      if (s[2] == 'i') {
        if (s[3] == 't') {
          if (s[4] == 'c') {
            if (s[5] == 'h') {
              return T_SWITCH;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 't') {
    if (s[1] == 'y') {
      if (s[2] == 'p') {
        if (s[3] == 'e') {
          if (s[4] == 'i') {
            if (s[5] == 'd') {
              return T_TYPEID;
            }
          }
          else if (s[4] == 'o') {
            if (s[5] == 'f') {
              return T_TYPEOF;
            }
          }
        }
      }
    }
  }
  else if (q && s[0] == 'S') {
    if (s[1] == 'I') {
      if (s[2] == 'G') {
        if (s[3] == 'N') {
          if (s[4] == 'A') {
            if (s[5] == 'L') {
              return T_SIGNAL;
            }
          }
        }
      }
    }
  }
  else if (q && s[0] == 'Q') {
    if (s[1] == '_') {
      if (s[2] == 'S') {
        if (s[3] == 'L') {
          if (s[4] == 'O') {
            if (s[5] == 'T') {
              return T_Q_SLOT;
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify7(const char *s, bool q, bool) {
  if (s[0] == '_') {
    if (s[1] == '_') {
      if (s[2] == 'a') {
        if (s[3] == 's') {
          if (s[4] == 'm') {
            if (s[5] == '_') {
              if (s[6] == '_') {
                return T___ASM__;
              }
            }
          }
        }
      }
      else if (s[2] == 'c') {
        if (s[3] == 'o') {
          if (s[4] == 'n') {
            if (s[5] == 's') {
              if (s[6] == 't') {
                return T___CONST;
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'd') {
    if (s[1] == 'e') {
      if (s[2] == 'f') {
        if (s[3] == 'a') {
          if (s[4] == 'u') {
            if (s[5] == 'l') {
              if (s[6] == 't') {
                return T_DEFAULT;
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'm') {
    if (s[1] == 'u') {
      if (s[2] == 't') {
        if (s[3] == 'a') {
          if (s[4] == 'b') {
            if (s[5] == 'l') {
              if (s[6] == 'e') {
                return T_MUTABLE;
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'p') {
    if (s[1] == 'r') {
      if (s[2] == 'i') {
        if (s[3] == 'v') {
          if (s[4] == 'a') {
            if (s[5] == 't') {
              if (s[6] == 'e') {
                return T_PRIVATE;
              }
            }
          }
        }
      }
    }
  }
  else if (q && s[0] == 'f') {
    if (s[1] == 'o') {
      if (s[2] == 'r') {
        if (s[3] == 'e') {
          if (s[4] == 'a') {
            if (s[5] == 'c') {
              if (s[6] == 'h') {
                return T_Q_FOREACH;
              }
            }
          }
        }
      }
    }
  }
  else if (q && s[0] == 's') {
    if (s[1] == 'i') {
      if (s[2] == 'g') {
        if (s[3] == 'n') {
          if (s[4] == 'a') {
            if (s[5] == 'l') {
              if (s[6] == 's') {
                return T_Q_SIGNALS;
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 't') {
    if (s[1] == 'y') {
      if (s[2] == 'p') {
        if (s[3] == 'e') {
          if (s[4] == 'd') {
            if (s[5] == 'e') {
              if (s[6] == 'f') {
                return T_TYPEDEF;
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'v') {
    if (s[1] == 'i') {
      if (s[2] == 'r') {
        if (s[3] == 't') {
          if (s[4] == 'u') {
            if (s[5] == 'a') {
              if (s[6] == 'l') {
                return T_VIRTUAL;
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'w') {
    if (s[1] == 'c') {
      if (s[2] == 'h') {
        if (s[3] == 'a') {
          if (s[4] == 'r') {
            if (s[5] == '_') {
              if (s[6] == 't') {
                return T_WCHAR_T;
              }
            }
          }
        }
      }
    }
  }
  else if (q && s[0] == 'Q') {
    if (s[1] == '_') {
      if (s[2] == 'S') {
        if (s[3] == 'L') {
          if (s[4] == 'O') {
            if (s[5] == 'T') {
              if (s[6] == 'S') {
                return T_Q_SLOTS;
              }
            }
          }
        }
      }
      else if (s[2] == 'E') {
        if (s[3] == 'N') {
          if (s[4] == 'U') {
            if (s[5] == 'M') {
              if (s[6] == 'S') {
                return T_Q_ENUMS;
              }
            }
          }
        }
      }
      else if (s[2] == 'F') {
        if (s[3] == 'L') {
          if (s[4] == 'A') {
            if (s[5] == 'G') {
              if (s[6] == 'S') {
                return T_Q_FLAGS;
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify8(const char *s, bool q, bool x) {
  if (s[0] == '_') {
    if (s[1] == '_') {
      if (s[2] == 'i') {
        if (s[3] == 'n') {
          if (s[4] == 'l') {
            if (s[5] == 'i') {
              if (s[6] == 'n') {
                if (s[7] == 'e') {
                  return T___INLINE;
                }
              }
            }
          }
        }
      }
      else if (s[2] == 't') {
        if (s[3] == 'y') {
          if (s[4] == 'p') {
            if (s[5] == 'e') {
              if (s[6] == 'o') {
                if (s[7] == 'f') {
                  return T___TYPEOF;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'c') {
    if (s[1] == 'o') {
      if (s[2] == 'n') {
        if (s[3] == 't') {
          if (s[4] == 'i') {
            if (s[5] == 'n') {
              if (s[6] == 'u') {
                if (s[7] == 'e') {
                  return T_CONTINUE;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (x && s[0] == 'd') {
    if (s[1] == 'e') {
      if (s[2] == 'c') {
        if (s[3] == 'l') {
          if (s[4] == 't') {
            if (s[5] == 'y') {
              if (s[6] == 'p') {
                if (s[7] == 'e') {
                  return T___TYPEOF;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'e') {
    if (s[1] == 'x') {
      if (s[2] == 'p') {
        if (s[3] == 'l') {
          if (s[4] == 'i') {
            if (s[5] == 'c') {
              if (s[6] == 'i') {
                if (s[7] == 't') {
                  return T_EXPLICIT;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'o') {
    if (s[1] == 'p') {
      if (s[2] == 'e') {
        if (s[3] == 'r') {
          if (s[4] == 'a') {
            if (s[5] == 't') {
              if (s[6] == 'o') {
                if (s[7] == 'r') {
                  return T_OPERATOR;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'r') {
    if (s[1] == 'e') {
      if (s[2] == 'g') {
        if (s[3] == 'i') {
          if (s[4] == 's') {
            if (s[5] == 't') {
              if (s[6] == 'e') {
                if (s[7] == 'r') {
                  return T_REGISTER;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 't') {
    if (s[1] == 'e') {
      if (s[2] == 'm') {
        if (s[3] == 'p') {
          if (s[4] == 'l') {
            if (s[5] == 'a') {
              if (s[6] == 't') {
                if (s[7] == 'e') {
                  return T_TEMPLATE;
                }
              }
            }
          }
        }
      }
    }
    else if (s[1] == 'y') {
      if (s[2] == 'p') {
        if (s[3] == 'e') {
          if (s[4] == 'n') {
            if (s[5] == 'a') {
              if (s[6] == 'm') {
                if (s[7] == 'e') {
                  return T_TYPENAME;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'u') {
    if (s[1] == 'n') {
      if (s[2] == 's') {
        if (s[3] == 'i') {
          if (s[4] == 'g') {
            if (s[5] == 'n') {
              if (s[6] == 'e') {
                if (s[7] == 'd') {
                  return T_UNSIGNED;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'v') {
    if (s[1] == 'o') {
      if (s[2] == 'l') {
        if (s[3] == 'a') {
          if (s[4] == 't') {
            if (s[5] == 'i') {
              if (s[6] == 'l') {
                if (s[7] == 'e') {
                  return T_VOLATILE;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (q && s[0] == 'Q') {
    if (s[1] == '_') {
      if (s[2] == 'G') {
        if (s[3] == 'A') {
          if (s[4] == 'D') {
            if (s[5] == 'G') {
              if (s[6] == 'E') {
                if (s[7] == 'T') {
                  return T_Q_GADGET;
                }
              }
            }
          }
        }
      }
      else if (s[2] == 'O') {
        if (s[3] == 'B') {
          if (s[4] == 'J') {
            if (s[5] == 'E') {
              if (s[6] == 'C') {
                if (s[7] == 'T') {
                  return T_Q_OBJECT;
                }
              }
            }
          }
        }
      }
      else if (s[2] == 'S') {
        if (s[3] == 'I') {
          if (s[4] == 'G') {
            if (s[5] == 'N') {
              if (s[6] == 'A') {
                if (s[7] == 'L') {
                  return T_Q_SIGNAL;
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify9(const char *s, bool q, bool) {
  if (s[0] == '_') {
    if (s[1] == '_') {
      if (s[2] == 'c') {
        if (s[3] == 'o') {
          if (s[4] == 'n') {
            if (s[5] == 's') {
              if (s[6] == 't') {
                if (s[7] == '_') {
                  if (s[8] == '_') {
                    return T___CONST__;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'n') {
    if (s[1] == 'a') {
      if (s[2] == 'm') {
        if (s[3] == 'e') {
          if (s[4] == 's') {
            if (s[5] == 'p') {
              if (s[6] == 'a') {
                if (s[7] == 'c') {
                  if (s[8] == 'e') {
                    return T_NAMESPACE;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'p') {
    if (s[1] == 'r') {
      if (s[2] == 'o') {
        if (s[3] == 't') {
          if (s[4] == 'e') {
            if (s[5] == 'c') {
              if (s[6] == 't') {
                if (s[7] == 'e') {
                  if (s[8] == 'd') {
                    return T_PROTECTED;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if (q && s[0] == 'Q') {
    if (s[1] == '_') {
      if (s[2] == 'S') {
        if (s[3] == 'I') {
          if (s[4] == 'G') {
            if (s[5] == 'N') {
              if (s[6] == 'A') {
                if (s[7] == 'L') {
                  if (s[8] == 'S') {
                    return T_Q_SIGNALS;
                  }
                }
              }
            }
          }
        }
      } else if (s[2] == 'F') {
        if (s[3] == 'O') {
          if (s[4] == 'R') {
            if (s[5] == 'E') {
              if (s[6] == 'A') {
                if (s[7] == 'C') {
                  if (s[8] == 'H') {
                    return T_Q_FOREACH;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify10(const char *s, bool q, bool) {
  if (s[0] == '_') {
    if (s[1] == '_') {
      if (s[2] == 'i') {
        if (s[3] == 'n') {
          if (s[4] == 'l') {
            if (s[5] == 'i') {
              if (s[6] == 'n') {
                if (s[7] == 'e') {
                  if (s[8] == '_') {
                    if (s[9] == '_') {
                      return T___INLINE__;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if (s[2] == 't') {
        if (s[3] == 'y') {
          if (s[4] == 'p') {
            if (s[5] == 'e') {
              if (s[6] == 'o') {
                if (s[7] == 'f') {
                  if (s[8] == '_') {
                    if (s[9] == '_') {
                      return T___TYPEOF__;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if (s[2] == 'v') {
        if (s[3] == 'o') {
          if (s[4] == 'l') {
            if (s[5] == 'a') {
              if (s[6] == 't') {
                if (s[7] == 'i') {
                  if (s[8] == 'l') {
                    if (s[9] == 'e') {
                      return T___VOLATILE;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'c') {
    if (s[1] == 'o') {
      if (s[2] == 'n') {
        if (s[3] == 's') {
          if (s[4] == 't') {
            if (s[5] == '_') {
              if (s[6] == 'c') {
                if (s[7] == 'a') {
                  if (s[8] == 's') {
                    if (s[9] == 't') {
                      return T_CONST_CAST;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if (q && s[0] == 'Q') {
    if (s[1] == '_') {
      if (s[2] == 'O') {
        if (s[3] == 'V') {
          if (s[4] == 'E') {
            if (s[5] == 'R') {
              if (s[6] == 'R') {
                if (s[7] == 'I') {
                  if (s[8] == 'D') {
                    if (s[9] == 'E') {
                      return T_Q_PROPERTY; // Q_OVERRIDE is just an alias for Q_PROPERTY
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if (s[2] == 'P') {
        if (s[3] == 'R') {
          if (s[4] == 'O') {
            if (s[5] == 'P') {
              if (s[6] == 'E') {
                if (s[7] == 'R') {
                  if (s[8] == 'T') {
                    if (s[9] == 'Y') {
                      return T_Q_PROPERTY;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify11(const char *s, bool q, bool) {
  if (s[0] == '_') {
    if (s[1] == '_') {
      if (s[2] == 'a') {
        if (s[3] == 't') {
          if (s[4] == 't') {
            if (s[5] == 'r') {
              if (s[6] == 'i') {
                if (s[7] == 'b') {
                  if (s[8] == 'u') {
                    if (s[9] == 't') {
                      if (s[10] == 'e') {
                        return T___ATTRIBUTE;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 's') {
    if (s[1] == 't') {
      if (s[2] == 'a') {
        if (s[3] == 't') {
          if (s[4] == 'i') {
            if (s[5] == 'c') {
              if (s[6] == '_') {
                if (s[7] == 'c') {
                  if (s[8] == 'a') {
                    if (s[9] == 's') {
                      if (s[10] == 't') {
                        return T_STATIC_CAST;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if (q && s[0] == 'Q') {
    if (s[1] == '_') {
      if (s[2] == 'I') {
        if (s[3] == 'N') {
          if (s[4] == 'V') {
            if (s[5] == 'O') {
              if (s[6] == 'K') {
                if (s[7] == 'A') {
                  if (s[8] == 'B') {
                    if (s[9] == 'L') {
                      if (s[10] == 'E') {
                        return T_Q_INVOKABLE;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify12(const char *s, bool q, bool) {
  if (s[0] == '_') {
    if (s[1] == '_') {
      if (s[2] == 'v') {
        if (s[3] == 'o') {
          if (s[4] == 'l') {
            if (s[5] == 'a') {
              if (s[6] == 't') {
                if (s[7] == 'i') {
                  if (s[8] == 'l') {
                    if (s[9] == 'e') {
                      if (s[10] == '_') {
                        if (s[11] == '_') {
                          return T___VOLATILE__;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if (q && s[0] == 'Q') {
    if (s[1] == '_') {
      if (s[2] == 'I') {
        if (s[3] == 'N') {
          if (s[4] == 'T') {
            if (s[5] == 'E') {
              if (s[6] == 'R') {
                if (s[7] == 'F') {
                  if (s[8] == 'A') {
                    if (s[9] == 'C') {
                      if (s[10] == 'E') {
                        if (s[11] == 'S') {
                          return T_Q_INTERFACES;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'd') {
    if (s[1] == 'y') {
      if (s[2] == 'n') {
        if (s[3] == 'a') {
          if (s[4] == 'm') {
            if (s[5] == 'i') {
              if (s[6] == 'c') {
                if (s[7] == '_') {
                  if (s[8] == 'c') {
                    if (s[9] == 'a') {
                      if (s[10] == 's') {
                        if (s[11] == 't') {
                          return T_DYNAMIC_CAST;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify13(const char *s, bool, bool) {
  if (s[0] == '_') {
    if (s[1] == '_') {
      if (s[2] == 'a') {
        if (s[3] == 't') {
          if (s[4] == 't') {
            if (s[5] == 'r') {
              if (s[6] == 'i') {
                if (s[7] == 'b') {
                  if (s[8] == 'u') {
                    if (s[9] == 't') {
                      if (s[10] == 'e') {
                        if (s[11] == '_') {
                          if (s[12] == '_') {
                            return T___ATTRIBUTE__;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify16(const char *s, bool, bool) {
  if (s[0] == 'r') {
    if (s[1] == 'e') {
      if (s[2] == 'i') {
        if (s[3] == 'n') {
          if (s[4] == 't') {
            if (s[5] == 'e') {
              if (s[6] == 'r') {
                if (s[7] == 'p') {
                  if (s[8] == 'r') {
                    if (s[9] == 'e') {
                      if (s[10] == 't') {
                        if (s[11] == '_') {
                          if (s[12] == 'c') {
                            if (s[13] == 'a') {
                              if (s[14] == 's') {
                                if (s[15] == 't') {
                                  return T_REINTERPRET_CAST;
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify14(const char *s, bool q, bool) {
  if (q && s[0] == 'Q') {
    if (s[1] == '_') {
      if (s[2] == 'P') {
        if (s[3] == 'R') {
          if (s[4] == 'I') {
            if (s[5] == 'V') {
              if (s[6] == 'A') {
                if (s[7] == 'T') {
                  if (s[8] == 'E') {
                    if (s[9] == '_') {
                      if (s[10] == 'S') {
                        if (s[11] == 'L') {
                          if (s[12] == 'O') {
                            if (s[13] == 'T') {
                              return T_Q_PRIVATE_SLOT;
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify18(const char *s, bool q, bool) {
  if (q && s[0] == 'Q') {
    if (s[1] == '_') {
      if (s[2] == 'P') {
        if (s[3] == 'R') {
          if (s[4] == 'I') {
            if (s[5] == 'V') {
              if (s[6] == 'A') {
                if (s[7] == 'T') {
                  if (s[8] == 'E') {
                    if (s[9] == '_') {
                      if (s[10] == 'P') {
                        if (s[11] == 'R') {
                          if (s[12] == 'O') {
                            if (s[13] == 'P') {
                              if (s[14] == 'E') {
                                if (s[15] == 'R') {
                                  if (s[16] == 'T') {
                                    if (s[17] == 'Y') {
                                      return T_Q_PRIVATE_PROPERTY;
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classify19(const char *s, bool q, bool) {
  if (q && s[0] == 'Q') {
    if (s[1] == '_') {
      if (s[2] == 'D') {
        if (s[3] == 'E') {
          if (s[4] == 'C') {
            if (s[5] == 'L') {
              if (s[6] == 'A') {
                if (s[7] == 'R') {
                  if (s[8] == 'E') {
                    if (s[9] == '_') {
                      if (s[10] == 'I') {
                        if (s[11] == 'N') {
                          if (s[12] == 'T') {
                            if (s[13] == 'E') {
                              if (s[14] == 'R') {
                                if (s[15] == 'F') {
                                  if (s[16] == 'A') {
                                    if (s[17] == 'C') {
                                      if (s[18] == 'E') {
                                        return T_Q_DECLARE_INTERFACE;
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyExtraExecSql2(const char *s)
{
  if (s[0] == 'A' || s[0] == 'a') {
    if (s[1] == 'S' || s[1] == 's') {
      return T_SQL_AS;
    }
  }
  else if (s[0] == 'B' || s[0] == 'b') {
    if (s[1] == 'Y' || s[1] == 'y') {
      return T_SQL_BY;
    }
  }
  else if (s[0] == 'I' || s[0] == 'i') {
    if (s[1] == 'N' || s[1] == 'n') {
      return T_SQL_IN;
    }
    else if (s[1] == 'S' || s[1] == 's') {
      return T_SQL_IS;
    }
  }
  else if (s[0] == 'O' || s[0] == 'o') {
    if (s[1] == 'F' || s[1] == 'f') {
      return T_SQL_OF;
    }
    else if (s[1] == 'N' || s[1] == 'n') {
      return T_SQL_ON;
    }
    else if (s[1] == 'R' || s[1] == 'r') {
      return T_SQL_OR;
    }
  }
  else if (s[0] == 'T' || s[0] == 't') {
    if (s[1] == 'O' || s[1] == 'o') {
      return T_SQL_TO;
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyExtraExecSql3(const char *s)
{
  if (s[0] == 'A' || s[0] == 'a') {
    if (s[1] == 'L' || s[1] == 'l') {
      if (s[2] == 'L' || s[2] == 'l') {
        return T_SQL_ALL;
      }
    }
    else if (s[1] == 'N' || s[1] == 'n') {
      if (s[2] == 'D' || s[2] == 'd') {
        return T_SQL_AND;
      }
    }
    else if (s[1] == 'S' || s[1] == 's') {
      if (s[2] == 'C' || s[2] == 'c') {
        return T_SQL_ASC;
      }
    }
  }
  else if (s[0] == 'F' || s[0] == 'f') {
    if (s[1] == 'O' || s[1] == 'o') {
      if (s[2] == 'R' || s[2] == 'r') {
        return T_SQL_FOR;
      }
    }
  }
  else if (s[0] == 'M' || s[0] == 'm') {
    if (s[1] == 'A' || s[1] == 'a') {
      if (s[1] == 'X' || s[2] == 'x') {
        return T_SQL_MAX;
      }
    }
    else if (s[1] == 'I' || s[1] == 'i') {
      if (s[2] == 'N' || s[2] == 'n') {
        return T_SQL_MIN;
      }
    }
  }
  else if (s[0] == 'N' || s[0] == 'n') {
    if (s[1] == 'O' || s[1] == 'o') {
      if (s[2] == 'T' || s[2] == 't') {
        return T_SQL_NOT;
      }
    }
  }
  else if (s[0] == 'R' || s[0] == 'r') {
    if (s[1] == 'O' || s[1] == 'o') {
      if (s[2] == 'W' || s[2] == 'w') {
        return T_SQL_ROW;
      }
    }
  }
  else if (s[0] == 'S' || s[0] == 's') {
    if (s[1] == 'E' || s[1] == 'e') {
      if (s[2] == 'T' || s[2] == 't') {
        return T_SQL_SET;
      }
    }
    else if (s[1] == 'Q' || s[1] == 'q') {
      if (s[2] == 'L' || s[2] == 'l') {
        return T_SQL_SQL;
      }
    }
    else if (s[1] == 'U' || s[1] == 'u') {
      if (s[2] == 'M' || s[2] == 'm') {
        return T_SQL_SUM;
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyExtraExecSql4(const char *s)
{
  if (s[0] == 'D' || s[0] == 'd') {
    if (s[1] == 'E' || s[1] == 'e') {
      if (s[2] == 'S' || s[2] == 's') {
        if (s[3] == 'C' || s[3] == 'c') {
          return T_SQL_DESC;
        }
      }
    }
  }
  else if (s[0] == 'E' || s[0] == 'e') {
    if (s[1] == 'X' || s[1] == 'x') {
      if (s[2] == 'E' || s[2] == 'e') {
        if (s[3] == 'C' || s[3] == 'c') {
          return T_SQL_EXEC;
        }
      }
    }
  }
  else if (s[0] == 'F' || s[0] == 'f') {
    if (s[1] == 'R' || s[1] == 'r') {
      if (s[2] == 'O' || s[2] == 'o') {
        if (s[3] == 'M' || s[3] == 'm') {
          return T_SQL_FROM;
        }
      }
    }
  }
  else if (s[0] == 'G' || s[0] == 'g') {
    if (s[1] == 'O' || s[1] == 'o') {
      if (s[2] == 'T' || s[2] == 't') {
        if (s[3] == 'O' || s[3] == 'o') {
          return T_SQL_GOTO;
        }
      }
    }
  }
  else if (s[0] == 'H' || s[0] == 'h') {
    if (s[1] == 'O' || s[1] == 'o') {
      if (s[2] == 'L' || s[2] == 'l') {
        if (s[3] == 'D' || s[3] == 'd') {
          return T_SQL_HOLD;
        }
      }
    }
  }
  else if (s[0] == 'I' || s[0] == 'i') {
    if (s[1] == 'N' || s[1] == 'n') {
      if (s[2] == 'T' || s[2] == 't') {
        if (s[3] == 'O' || s[3] == 'o') {
          return T_SQL_INTO;
        }
      }
    }
  }
  else if (s[0] == 'J' || s[0] == 'j') {
    if (s[1] == 'O' || s[1] == 'o') {
      if (s[2] == 'I' || s[2] == 'i') {
        if (s[3] == 'N' || s[3] == 'n') {
          return T_SQL_JOIN;
        }
      }
    }
  }
  else if (s[0] == 'L' || s[0] == 'l') {
    if (s[1] == 'E' || s[1] == 'e') {
      if (s[2] == 'F' || s[2] == 'f') {
        if (s[3] == 'T' || s[3] == 't') {
          return T_SQL_LEFT;
        }
      }
    }
    else if (s[1] == 'I' || s[1] == 'i') {
      if (s[2] == 'K' || s[2] == 'k') {
        if (s[3] == 'E' || s[3] == 'e') {
          return T_SQL_LIKE;
        }
      }
      else if (s[2] == 'S' || s[2] == 's') {
        if (s[3] == 'T' || s[3] == 't') {
          return T_SQL_LIST;
        }
      }
    }
  }
  else if (s[0] == 'O' || s[0] == 'o') {
    if (s[1] == 'N' || s[1] == 'n') {
      if (s[2] == 'L' || s[2] == 'l') {
        if (s[3] == 'Y' || s[3] == 'y') {
          return T_SQL_ONLY;
        }
      }
    }
    else if (s[1] == 'P' || s[1] == 'p') {
      if (s[2] == 'E' || s[2] == 'e') {
        if (s[3] == 'N' || s[3] == 'n') {
          return T_SQL_OPEN;
        }
      }
    }
  }
  else if (s[0] == 'R' || s[0] == 'r') {
    if (s[1] == 'E' || s[1] == 'e') {
      if (s[2] == 'A' || s[2] == 'a') {
        if (s[3] == 'D' || s[3] == 'd') {
          return T_SQL_READ;
        }
      }
    }
    else if (s[1] == 'O' || s[1] == 'o') {
      if (s[2] == 'W' || s[2] == 'w') {
        if (s[3] == 'S' || s[3] == 's') {
          return T_SQL_ROWS;
        }
      }
    }
  }
  else if (s[0] == 'W' || s[0] == 'w') {
    if (s[1] == 'I' || s[1] == 'i') {
      if (s[2] == 'T' || s[2] == 't') {
        if (s[3] == 'H' || s[3] == 'h') {
          return T_SQL_WITH;
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyExtraExecSql5(const char *s)
{
  if (s[0] == 'A' || s[0] == 'a') {
    if (s[1] == 'L' || s[1] == 'l') {
      if (s[2] == 'I' || s[2] == 'i') {
        if (s[3] == 'A' || s[3] == 'a') {
          if (s[4] == 'S' || s[4] == 's') {
            return T_SQL_ALIAS;
          }
        }
      }
    }
  }
  else if (s[0] == 'C' || s[0] == 'c') {
    if (s[1] == 'L' || s[1] == 'l') {
      if (s[2] == 'O' || s[2] == 'o') {
        if (s[3] == 'S' || s[3] == 's') {
          if (s[4] == 'E' || s[4] == 'e') {
            return T_SQL_CLOSE;
          }
        }
      }
    }
    else if (s[1] == 'O' || s[1] == 'o') {
      if (s[2] == 'U' || s[2] == 'u') {
        if (s[3] == 'N' || s[3] == 'n') {
          if (s[4] == 'T' || s[4] == 't') {
            return T_SQL_COUNT;
          }
        }
      }
    }
    else if (s[1] == 'R' || s[1] == 'r') {
      if (s[2] == 'O' || s[2] == 'o') {
        if (s[3] == 'S' || s[3] == 's') {
          if (s[4] == 'S' || s[4] == 's') {
            return T_SQL_CROSS;
          }
        }
      }
    }
  }
  else if (s[0] == 'F' || s[0] == 'f') {
    if (s[1] == 'E' || s[1] == 'e') {
      if (s[2] == 'T' || s[2] == 't') {
        if (s[3] == 'C' || s[3] == 'c') {
          if (s[4] == 'H' || s[4] == 'h') {
            return T_SQL_FETCH;
          }
        }
      }
    }
  }
  else if (s[0] == 'G' || s[0] == 'g') {
    if (s[1] == 'R' || s[1] == 'r') {
      if (s[2] == 'O' || s[2] == 'o') {
        if (s[3] == 'U' || s[3] == 'u') {
          if (s[4] == 'P' || s[4] == 'p') {
            return T_SQL_GROUP;
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            return T_SQL_INNER;
          }
        }
      }
    }
  }
  else if (s[0] == 'L' || s[0] == 'l') {
    if (s[1] == 'I' || s[1] == 'i') {
      if (s[2] == 'M' || s[2] == 'm') {
        if (s[3] == 'I' || s[3] == 'i') {
          if (s[4] == 'T' || s[4] == 't') {
            return T_SQL_LIMIT;
          }
        }
      }
    }
  }
  else if (s[0] == 'O' || s[0] == 'o') {
    if (s[1] == 'R' || s[1] == 'r') {
      if (s[2] == 'D' || s[2] == 'd') {
        if (s[3] == 'E' || s[3] == 'e') {
          if (s[4] == 'R' || s[4] == 'r') {
            return T_SQL_ORDER;
          }
        }
      }
    }
  }
  else if (s[0] == 'R' || s[0] == 'r') {
    if (s[1] == 'I' || s[1] == 'i') {
      if (s[2] == 'G' || s[2] == 'g') {
        if (s[3] == 'H' || s[3] == 'h') {
          if (s[4] == 'T' || s[4] == 't') {
            return T_SQL_RIGHT;
          }
        }
      }
    }
  }
  else if (s[0] == 'T' || s[0] == 't') {
    if (s[1] == 'A' || s[1] == 'a') {
      if (s[2] == 'B' || s[2] == 'b') {
        if (s[3] == 'L' || s[3] == 'l') {
          if (s[4] == 'E' || s[4] == 'e') {
            return T_SQL_TABLE;
          }
        }
      }
    }
  }
  else if (s[0] == 'W' || s[0] == 'w') {
    if (s[1] == 'H' || s[1] == 'h') {
      if (s[2] == 'E' || s[2] == 'e') {
        if (s[3] == 'R' || s[3] == 'r') {
          if (s[4] == 'E' || s[4] == 'e') {
            return T_SQL_WHERE;
          }
        }
      }
    }
    else if (s[1] == 'R' || s[1] == 'r') {
      if (s[2] == 'I' || s[2] == 'i') {
        if (s[3] == 'T' || s[3] == 't') {
          if (s[4] == 'E' || s[4] == 'e') {
            return T_SQL_WRITE;
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyExtraExecSql6(const char *s)
{
  if (s[0] == 'C' || s[0] == 'c') {
    if (s[1] == 'U' || s[1] == 'u') {
      if (s[2] == 'R' || s[2] == 'r') {
        if (s[3] == 'S' || s[3] == 's') {
          if (s[4] == 'O' || s[4] == 'o') {
            if (s[5] == 'R' || s[5] == 'r') {
              return T_SQL_CURSOR;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'D' || s[0] == 'd') {
    if (s[1] == 'E' || s[1] == 'e') {
      if (s[2] == 'L' || s[2] == 'l') {
        if (s[3] == 'E' || s[3] == 'e') {
          if (s[4] == 'T' || s[4] == 't') {
            if (s[5] == 'E' || s[5] == 'e') {
              return T_SQL_DELETE;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'I' || s[0] == 'i') {
    if (s[1] == 'N' || s[1] == 'n') {
      if (s[2] == 'S' || s[2] == 's') {
        if (s[3] == 'E' || s[3] == 'e') {
          if (s[4] == 'R' || s[4] == 'r') {
            if (s[5] == 'T' || s[5] == 't') {
              return T_SQL_INSERT;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'S' || s[0] == 's') {
    if (s[1] == 'E' || s[1] == 'e') {
      if (s[2] == 'L' || s[2] == 'l') {
        if (s[3] == 'E' || s[3] == 'e') {
          if (s[4] == 'C' || s[4] == 'c') {
            if (s[5] == 'T' || s[5] == 't') {
              return T_SQL_SELECT;
            }
          }
        }
      }
    }
    else if (s[1] == 'H' || s[1] == 'h') {
      if (s[2] == 'A' || s[2] == 'a') {
        if (s[3] == 'R' || s[3] == 'r') {
          if (s[4] == 'E' || s[4] == 'e') {
            if (s[5] == 'D' || s[5] == 'd') {
              return T_SQL_SHARED;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'U' || s[0] == 'u') {
    if (s[1] == 'P' || s[1] == 'p') {
      if (s[2] == 'D' || s[2] == 'd') {
        if (s[3] == 'A' || s[3] == 'a') {
          if (s[4] == 'T' || s[4] == 't') {
            if (s[5] == 'E' || s[5] == 'e') {
              return T_SQL_UPDATE;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'V' || s[0] == 'v') {
    if (s[1] == 'A' || s[1] == 'a') {
      if (s[2] == 'L' || s[2] == 'l') {
        if (s[3] == 'U' || s[3] == 'u') {
          if (s[4] == 'E' || s[4] == 'e') {
            if (s[5] == 'S' || s[5] == 's') {
              return T_SQL_VALUES;
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyExtraExecSql7(const char *s)
{
  if (s[0] == 'B' || s[0] == 'b') {
    if (s[1] == 'E' || s[1] == 'e') {
      if (s[2] == 'T' || s[2] == 't') {
        if (s[3] == 'W' || s[3] == 'w') {
          if (s[4] == 'E' || s[4] == 'e') {
            if (s[5] == 'E' || s[5] == 'e') {
              if (s[6] == 'N' || s[6] == 'n') {
                return T_SQL_BETWEEN;
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'C' || s[0] == 'c') {
    if (s[1] == 'U' || s[1] == 'u') {
      if (s[2] == 'R' || s[2] == 'r') {
        if (s[3] == 'R' || s[3] == 'r') {
          if (s[4] == 'E' || s[4] == 'e') {
            if (s[5] == 'N' || s[5] == 'n') {
              if (s[6] == 'T' || s[6] == 't') {
                return T_SQL_CURRENT;
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'D' || s[0] == 'd') {
    if (s[1] == 'E' || s[1] == 'e') {
      if (s[2] == 'C' || s[2] == 'c') {
        if (s[3] == 'L' || s[3] == 'l') {
          if (s[4] == 'A' || s[4] == 'a') {
            if (s[5] == 'R' || s[5] == 'r') {
              if (s[6] == 'E' || s[6] == 'e') {
                return T_SQL_DECLARE;
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'I' || s[0] == 'i') {
    if (s[1] == 'N' || s[1] == 'n') {
      if (s[2] == 'C' || s[2] == 'c') {
        if (s[3] == 'L' || s[3] == 'l') {
          if (s[4] == 'U' || s[4] == 'u') {
            if (s[5] == 'D' || s[5] == 'd') {
              if (s[6] == 'E' || s[6] == 'e') {
                return T_SQL_INCLUDE;
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'P' || s[0] == 'p') {
    if (s[1] == 'R' || s[1] == 'r') {
      if (s[2] == 'E' || s[2] == 'e') {
        if (s[3] == 'P' || s[3] == 'p') {
          if (s[4] == 'A' || s[4] == 'a') {
            if (s[5] == 'R' || s[5] == 'r') {
              if (s[6] == 'E' || s[6] == 'e') {
                return T_SQL_PREPARE;
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyExtraExecSql8(const char *s)
{
  if (s[0] == 'D' || s[0] == 'd') {
    if (s[1] == 'I' || s[1] == 'i') {
      if (s[2] == 'S' || s[2] == 's') {
        if (s[3] == 'T' || s[3] == 't') {
          if (s[4] == 'I' || s[4] == 'i') {
            if (s[5] == 'N' || s[5] == 'n') {
              if (s[6] == 'C' || s[6] == 'c') {
                if (s[7] == 'T' || s[7] == 't') {
                  return T_SQL_DISTINCT;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'E' || s[0] == 'e') {
    if (s[1] == 'X' || s[1] == 'x') {
      if (s[2] == 'T' || s[2] == 't') {
        if (s[3] == 'E' || s[3] == 'e') {
          if (s[4] == 'R' || s[4] == 'r') {
            if (s[5] == 'N' || s[5] == 'n') {
              if (s[6] == 'A' || s[6] == 'a') {
                if (s[7] == 'L' || s[7] == 'l') {
                  return T_SQL_EXTERNAL;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'F' || s[0] == 'f') {
    if (s[1] == 'I' || s[1] == 'i') {
      if (s[2] == 'L' || s[2] == 'l') {
        if (s[3] == 'E' || s[3] == 'e') {
          if (s[4] == 'N' || s[4] == 'n') {
            if (s[5] == 'A' || s[5] == 'a') {
              if (s[6] == 'M' || s[6] == 'm') {
                if (s[7] == 'E' || s[7] == 'e') {
                  return T_SQL_FILENAME;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'P' || s[0] == 'p') {
    if (s[1] == 'R' || s[1] == 'r') {
      if (s[2] == 'E' || s[2] == 'e') {
        if (s[3] == 'S' || s[3] == 's') {
          if (s[4] == 'E' || s[4] == 'e') {
            if (s[5] == 'R' || s[5] == 'r') {
              if (s[6] == 'V' || s[6] == 'v') {
                if (s[7] == 'E' || s[7] == 'e') {
                  return T_SQL_PRESERVE;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'S' || s[0] == 's') {
    if (s[1] == 'Q' || s[1] == 'q') {
      if (s[2] == 'L' || s[2] == 'l') {
        if (s[3] == 'E' || s[3] == 'e') {
          if (s[4] == 'R' || s[4] == 'r') {
            if (s[5] == 'R' || s[5] == 'r') {
              if (s[6] == 'O' || s[6] == 'o') {
                if (s[7] == 'R' || s[7] == 'r') {
                  return T_SQL_SQLERROR;
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'W' || s[0] == 'w') {
    if (s[1] == 'H' || s[1] == 'h') {
      if (s[2] == 'E' || s[2] == 'e') {
        if (s[3] == 'N' || s[3] == 'n') {
          if (s[4] == 'E' || s[4] == 'e') {
            if (s[5] == 'V' || s[5] == 'v') {
              if (s[6] == 'E' || s[6] == 'e') {
                if (s[7] == 'R' || s[7] == 'r') {
                  return T_SQL_WHENEVER;
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyExtraExecSql9(const char *s)
{
  if (s[0] == 'C' || s[0] == 'c') {
    if (s[1] == '$') {
      if (s[2] == 'I' || s[2] == 'i') {
        if (s[3] == 'N' || s[3] == 'n') {
          if (s[4] == 'C' || s[4] == 'c') {
            if (s[5] == 'L' || s[5] == 'l') {
              if (s[6] == 'U' || s[6] == 'u') {
                if (s[7] == 'D' || s[7] == 'd') {
                  if (s[8] == 'E' || s[8] == 'e') {
                    return T_SQL_CINCLUDE;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'E' || s[0] == 'e') {
    if (s[1] == 'X' || s[1] == 'x') {
      if (s[2] == 'C' || s[2] == 'c') {
        if (s[3] == 'L' || s[3] == 'l') {
          if (s[4] == 'U' || s[4] == 'u') {
            if (s[5] == 'S' || s[5] == 's') {
              if (s[6] == 'I' || s[6] == 'i') {
                if (s[7] == 'V' || s[7] == 'v') {
                  if (s[8] == 'E' || s[8] == 'e') {
                    return T_SQL_EXCLUSIVE;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'R' || s[0] == 'r') {
    if (s[1] == 'E' || s[1] == 'e') {
      if (s[2] == 'S' || s[2] == 's') {
        if (s[3] == 'E' || s[3] == 'e') {
          if (s[4] == 'R' || s[4] == 'r') {
            if (s[5] == 'V' || s[5] == 'v') {
              if (s[6] == 'I' || s[6] == 'i') {
                if (s[7] == 'N' || s[7] == 'n') {
                  if (s[8] == 'G' || s[8] == 'g') {
                    return T_SQL_RESERVING;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyExtraExecSql10(const char *s)
{
  if (s[0] == 'S' || s[0] == 's') {
    if (s[1] == 'Q' || s[1] == 'q') {
      if (s[2] == 'L' || s[2] == 'l') {
        if (s[3] == 'W' || s[3] == 'w') {
          if (s[4] == 'A' || s[4] == 'a') {
            if (s[5] == 'R' || s[5] == 'r') {
              if (s[6] == 'N' || s[6] == 'n') {
                if (s[7] == 'I' || s[7] == 'i') {
                  if (s[8] == 'N' || s[8] == 'n') {
                    if (s[9] == 'G' || s[9] == 'g') {
                      return T_SQL_SQLWARNING;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyExtraExecSql11(const char *s)
{
  if (s[0] == 'T' || s[0] == 't') {
    if (s[1] == 'R' || s[1] == 'r') {
      if (s[2] == 'A' || s[2] == 'a') {
        if (s[3] == 'N' || s[3] == 'n') {
          if (s[4] == 'S' || s[4] == 's') {
            if (s[5] == 'A' || s[5] == 'a') {
              if (s[6] == 'C' || s[6] == 'c') {
                if (s[7] == 'T' || s[7] == 't') {
                  if (s[8] == 'I' || s[8] == 'i') {
                    if (s[9] == 'O' || s[9] == 'o') {
                      if (s[10] == 'N' || s[10] == 'n') {
                        return T_SQL_TRANSACTION;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyExtraExecSql(const char *s, int n, int scState)
{
  switch ( scState ) {

  case Lexer::SqlState_StartDeclaration: // want EXEC
    if ( n == 4 ) {
      if ( s[0] == 'E' || s[0] == 'e' ) {
        if ( s[1] == 'X' || s[1] == 'x' ) {
          if ( s[2] == 'E' || s[2] == 'e' ) {
            if ( s[3] == 'C' || s[3] == 'c' ) {
              return T_SQL_EXEC;
            }
          }
        }
      }
    }
    return T_IDENTIFIER;

  case Lexer::SqlState_Exec: // want SQL
    if ( n == 3 ) {
      if ( s[0] == 'S' || s[0] == 's' ) {
        if ( s[1] == 'Q' || s[1] == 'q' ) {
          if ( s[2] == 'L' || s[2] == 'l' ) {
            return T_SQL_SQL;
          }
        }
      }
    }
    return T_IDENTIFIER;

  case Lexer::SqlState_ExecSql:
    // Go to next switch
    break;

  case Lexer::SqlState_Invalid:
  default:
    return T_IDENTIFIER;
  }

  switch (n) {
    case 2: return classifyExtraExecSql2(s);
    case 3: return classifyExtraExecSql3(s);
    case 4: return classifyExtraExecSql4(s);
    case 5: return classifyExtraExecSql5(s);
    case 6: return classifyExtraExecSql6(s);
    case 7: return classifyExtraExecSql7(s);
    case 8: return classifyExtraExecSql8(s);
    case 9: return classifyExtraExecSql9(s);
    case 10: return classifyExtraExecSql10(s);
    case 11: return classifyExtraExecSql11(s);
  }

  return T_IDENTIFIER;
}

//int Lexer::identifierToExecSqlKeyword( Token *tok )
//{
//    int nRes = 0;
//    int nKind = classifyExtraExecSql(tok->identifier->chars(), tok->identifier->size());

//    switch ( nKind ) {
//    case T_IDENTIFIER: return 0;
//    case T_SQL_EXEC: nRes = 1; break;
//    case T_SQL_SQL: nRes = 2; break;
//    }

//    tok->f.kind = nKind;
//    return nRes;
//}

int Lexer::classify(const char *s, int n, bool q, bool x, bool sc, int scState)
{
  int nRes = T_IDENTIFIER;

  switch (n) {
    case 2: nRes = classify2(s, q, x); break;
    case 3: nRes = classify3(s, q, x); break;
    case 4: nRes = classify4(s, q, x); break;
    case 5: nRes = classify5(s, q, x); break;
    case 6: nRes = classify6(s, q, x); break;
    case 7: nRes = classify7(s, q, x); break;
    case 8: nRes = classify8(s, q, x); break;
    case 9: nRes = classify9(s, q, x); break;
    case 10: nRes = classify10(s, q, x); break;
    case 11: nRes = classify11(s, q, x); break;
    case 12: nRes = classify12(s, q, x); break;
    case 13: nRes = classify13(s, q, x); break;
    case 14: nRes = classify14(s, q, x); break;
    case 16: nRes = classify16(s, q, x); break;
    case 18: nRes = classify18(s, q, x); break;
    case 19: nRes = classify19(s, q, x); break;
  } // switch

  if ( nRes == T_IDENTIFIER && sc )
      nRes = classifyExtraExecSql(s, n, scState);

  return nRes;
}

static inline int classifyOperator2(const char *s) {
  if (s[0] == 'o') {
    if (s[1] == 'r') {
      return T_OR;
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyOperator3(const char *s) {
  if (s[0] == 'a') {
    if (s[1] == 'n') {
      if (s[2] == 'd') {
        return T_AND;
      }
    }
  }
  else if (s[0] == 'n') {
    if (s[1] == 'o') {
      if (s[2] == 't') {
        return T_NOT;
      }
    }
  }
  else if (s[0] == 'x') {
    if (s[1] == 'o') {
      if (s[2] == 'r') {
        return T_XOR;
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyOperator5(const char *s) {
  if (s[0] == 'b') {
    if (s[1] == 'i') {
      if (s[2] == 't') {
        if (s[3] == 'o') {
          if (s[4] == 'r') {
            return T_BITOR;
          }
        }
      }
    }
  }
  else if (s[0] == 'c') {
    if (s[1] == 'o') {
      if (s[2] == 'm') {
        if (s[3] == 'p') {
          if (s[4] == 'l') {
            return T_COMPL;
          }
        }
      }
    }
  }
  else if (s[0] == 'o') {
    if (s[1] == 'r') {
      if (s[2] == '_') {
        if (s[3] == 'e') {
          if (s[4] == 'q') {
            return T_OR_EQ;
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

static inline int classifyOperator6(const char *s) {
  if (s[0] == 'a') {
    if (s[1] == 'n') {
      if (s[2] == 'd') {
        if (s[3] == '_') {
          if (s[4] == 'e') {
            if (s[5] == 'q') {
              return T_AND_EQ;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'b') {
    if (s[1] == 'i') {
      if (s[2] == 't') {
        if (s[3] == 'a') {
          if (s[4] == 'n') {
            if (s[5] == 'd') {
              return T_BITAND;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'n') {
    if (s[1] == 'o') {
      if (s[2] == 't') {
        if (s[3] == '_') {
          if (s[4] == 'e') {
            if (s[5] == 'q') {
              return T_NOT_EQ;
            }
          }
        }
      }
    }
  }
  else if (s[0] == 'x') {
    if (s[1] == 'o') {
      if (s[2] == 'r') {
        if (s[3] == '_') {
          if (s[4] == 'e') {
            if (s[5] == 'q') {
              return T_XOR_EQ;
            }
          }
        }
      }
    }
  }
  return T_IDENTIFIER;
}

int Lexer::classifyOperator(const char *s, int n) {
  switch (n) {
    case 2: return classifyOperator2(s);
    case 3: return classifyOperator3(s);
    case 5: return classifyOperator5(s);
    case 6: return classifyOperator6(s);
    default: return T_IDENTIFIER;
  } // switch
}



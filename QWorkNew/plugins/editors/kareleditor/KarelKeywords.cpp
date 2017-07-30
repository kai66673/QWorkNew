#include "KarelKeywords.h"

namespace Karel {

static inline void keywordClassify2( const char *s, Token &tk )
{
  if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'R' || s[1] == 'r' ) {
      tk.kf.keytype = 5;
      tk.kf.keyword5 = 1;
      tk.kf.keyid = T_CR;
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      tk.kf.keytype = 2;
      tk.kf.keyword2 = 1;
      tk.kf.keyid = T_DO;
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      tk.kf.keytype = 2;
      tk.kf.keyword2 = 1;
      tk.kf.keyid = T_GO;
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'F' || s[1] == 'f' ) {
      tk.kf.keytype = 2;
      tk.kf.keyword2 = 1;
      tk.kf.keyid = T_IF;
    }
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      tk.kf.keytype = 2;
      tk.kf.keyword2 = 1;
      tk.kf.keyid = T_IN;
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_LN;
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o' ) {
    if ( s[1] == 'F' || s[1] == 'f' ) {
      tk.kf.keytype = 2;
      tk.kf.keyword2 = 1;
      tk.kf.keyid = T_OF;
    }
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      tk.kf.keytype = 5;
      tk.kf.keyword5 = 1;
      tk.kf.keyid = T_ON;
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      tk.kf.keytype = 2;
      tk.kf.keyword2 = 1;
      tk.kf.keyid = T_OR;
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      tk.kf.keytype = 2;
      tk.kf.keyword2 = 1;
      tk.kf.keyid = T_TO;
    }
  }
}

static inline void keywordClassify3( const char *s, Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'B' || s[1] == 'b' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_ABS;
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        tk.kf.keytype = 5;
        tk.kf.keyword5 = 1;
        tk.kf.keyid = T_AIN;
      }
    }
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_AND;
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_CHR;
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_COS;
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        tk.kf.keytype = 5;
        tk.kf.keyword5 = 1;
        tk.kf.keyid = T_DIN;
      }
      else if ( s[2] == 'V' || s[2] == 'v' ) {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_DIV;
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_END;
      }
    }
    else if ( s[1] == 'X' || s[1] == 'x' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_EXP;
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_FOR;
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        tk.kf.keytype = 5;
        tk.kf.keyword5 = 1;
        tk.kf.keyid = T_GIN;
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'V' || s[2] == 'v' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_INV;
      }
    }
  }
  else if ( s[0] == 'K' || s[0] == 'k' ) {
    if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_KLC;
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_MOD;
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_NOT;
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o' ) {
    if ( s[1] == 'F' || s[1] == 'f' ) {
      if ( s[2] == 'F' || s[2] == 'f' ) {
        tk.kf.keytype = 5;
        tk.kf.keyword5 = 1;
        tk.kf.keyid = T_OFF;
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_ORD;
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_POS;
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        tk.kf.keytype = 5;
        tk.kf.keyword5 = 1;
        tk.kf.keyid = T_RDI;
      }
      else if ( s[2] == 'O' || s[2] == 'o' ) {
        tk.kf.keytype = 5;
        tk.kf.keyword5 = 1;
        tk.kf.keyid = T_RDO;
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_SIN;
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_TAN;
      }
    }
  }
  else if ( s[0] == 'V' || s[0] == 'v' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_VAR;
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_VIA;
      }
    }
  }
  else if ( s[0] == 'W' || s[0] == 'w' ) {
    if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        tk.kf.keytype = 5;
        tk.kf.keyword5 = 1;
        tk.kf.keyid = T_WDI;
      }
    }
  }
}

static inline void keywordClassify4( const char *s, Token &tk )
{
  if ( s[0] == '$' ) {
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          tk.kf.keytype = 5;
          tk.kf.keyword5 = 1;
          tk.kf.keyid = T_$MCR;
        }
      }
    }
  }
  else if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'C' || s[1] == 'c' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_ACOS;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          tk.kf.keytype = 5;
          tk.kf.keyword5 = 1;
          tk.kf.keyid = T_AOUT;
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_ASIN;
        }
      }
    }
    else if ( s[1] == 'W' || s[1] == 'w' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'Y' || s[3] == 'y' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_AWAY;
        }
      }
    }
    else if ( s[1] == 'X' || s[1] == 'x' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_AXIS;
        }
      }
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b' ) {
    if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = T_BYTE;
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_CASE;
        }
      }
    }
    else if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          tk.kf.keytype = 5;
          tk.kf.keyword5 = 1;
          tk.kf.keyid = T_CMOS;
        }
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          tk.kf.keytype = 5;
          tk.kf.keyword5 = 1;
          tk.kf.keyid = T_DOUT;
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          tk.kf.keytype = 5;
          tk.kf.keyword5 = 1;
          tk.kf.keyid = T_DRAM;
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e' ) {
    if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_ELSE;
        }
      }
    }
    else if ( s[1] == 'V' || s[1] == 'v' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_EVAL;
        }
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_FILE;
        }
      }
      else if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 5;
          tk.kf.keyword5 = 1;
          tk.kf.keyid = T_FINE;
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_FROM;
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          tk.kf.keytype = 5;
          tk.kf.keyword5 = 1;
          tk.kf.keyid = T_GOUT;
        }
      }
    }
  }
  else if ( s[0] == 'H' || s[0] == 'h' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_HAND;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_HOLD;
        }
      }
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_LOAD;
        }
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'V' || s[2] == 'v' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_MOVE;
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_NEAR;
        }
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o' ) {
    if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_OPEN;
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          tk.kf.keytype = 5;
          tk.kf.keyword5 = 1;
          tk.kf.keyid = T_OPIN;
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = T_PATH;
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_READ;
        }
        else if ( s[3] == 'L' || s[3] == 'l' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = T_REAL;
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'V' || s[2] == 'v' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_SAVE;
        }
      }
    }
    else if ( s[1] == 'Q' || s[1] == 'q' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_SQRT;
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_STOP;
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_THEN;
        }
      }
    }
    else if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          tk.kf.keytype = 5;
          tk.kf.keyword5 = 1;
          tk.kf.keyid = T_TPIN;
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 5;
          tk.kf.keyword5 = 1;
          tk.kf.keyid = T_TRUE;
        }
      }
    }
    else if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_TYPE;
        }
      }
    }
  }
  else if ( s[0] == 'W' || s[0] == 'w' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_WAIT;
        }
      }
    }
    else if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_WHEN;
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_WITH;
        }
      }
    }
  }
}

static inline void keywordClassify5( const char *s, Token &tk )
{
  if ( s[0] == '$' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            tk.kf.keytype = 5;
            tk.kf.keyword5 = 1;
            tk.kf.keyid = T_$NILP;
          }
        }
      }
    }
  }
  else if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'B' || s[1] == 'b' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_ABORT;
          }
        }
        else if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_ABOUT;
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'G' || s[4] == 'g' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_ALONG;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'Y' || s[4] == 'y' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_ARRAY;
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == '2' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_ATAN2;
          }
        }
      }
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'G' || s[2] == 'g' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_BEGIN;
          }
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_CLEAR;
          }
        }
      }
      else if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_CLOSE;
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_CONST;
          }
        }
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'Y' || s[4] == 'y' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_DELAY;
          }
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'F' || s[4] == 'f' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_ENDIF;
          }
        }
      }
    }
    else if ( s[1] == 'V' || s[1] == 'v' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_EVENT;
          }
        }
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 5;
            tk.kf.keyword5 = 1;
            tk.kf.keyid = T_FALSE;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_FRAME;
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'X' || s[4] == 'x' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_INDEX;
          }
        }
      }
      else if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 5;
            tk.kf.keyword5 = 1;
            tk.kf.keyid = T_INPUT;
          }
        }
      }
    }
  }
  else if ( s[0] == 'J' || s[0] == 'j' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 5;
            tk.kf.keyword5 = 1;
            tk.kf.keyid = T_JOINT;
          }
        }
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            tk.kf.keytype = 3;
            tk.kf.keyword3 = 1;
            tk.kf.keyid = T_MODEL;
          }
        }
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o' ) {
    if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 5;
            tk.kf.keyword5 = 1;
            tk.kf.keyid = T_OPOUT;
          }
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_PAUSE;
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_PULSE;
          }
        }
      }
      else if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'G' || s[3] == 'g' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_PURGE;
          }
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'X' || s[4] == 'x' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_RELAX;
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_RESET;
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_ROUND;
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 3;
            tk.kf.keyword3 = 1;
            tk.kf.keyid = T_SHORT;
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_TIMER;
          }
        }
      }
    }
    else if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 5;
            tk.kf.keyword5 = 1;
            tk.kf.keyid = T_TPOUT;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_TRUNC;
          }
        }
      }
    }
  }
  else if ( s[0] == 'U' || s[0] == 'u' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_UNPOS;
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_UNTIL;
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'G' || s[4] == 'g' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_USING;
          }
        }
      }
    }
  }
  else if ( s[0] == 'W' || s[0] == 'w' ) {
    if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 5;
            tk.kf.keyword5 = 1;
            tk.kf.keyid = T_WDOUT;
          }
        }
      }
    }
    else if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_WHILE;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_WRITE;
          }
        }
      }
    }
  }
}

static inline void keywordClassify6( const char *s, Token &tk )
{
  if ( s[0] == '$' ) {
    if ( s[1] == 'G' || s[1] == 'g' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              tk.kf.keytype = 5;
              tk.kf.keyword5 = 1;
              tk.kf.keyid = T_$GROUP;
            }
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              tk.kf.keytype = 5;
              tk.kf.keyword5 = 1;
              tk.kf.keyid = T_$SPEED;
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              tk.kf.keytype = 5;
              tk.kf.keyword5 = 1;
              tk.kf.keyid = T_$UTOOL;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == '%' ) {
    if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'Y' || s[5] == 'y' ) {
              tk.kf.keytype = 4;
              tk.kf.keyword4 = 1;
              tk.kf.keyid = T__DELAY;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'H' || s[5] == 'h' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_ATTACH;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b' ) {
    if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_BYNAME;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_CANCEL;
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 5;
              tk.kf.keyword5 = 1;
              tk.kf.keyid = T_COARSE;
            }
          }
        }
      }
      else if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'F' || s[3] == 'f' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'G' || s[5] == 'g' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = T_CONFIG;
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_CURPOS;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'W' || s[2] == 'w' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_DOWNTO;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'B' || s[3] == 'b' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_ENABLE;
            }
          }
        }
      }
      else if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'F' || s[3] == 'f' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_ENDFOR;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              tk.kf.keytype = 5;
              tk.kf.keyword5 = 1;
              tk.kf.keyid = T_LINEAR;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'X' || s[2] == 'x' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 5;
              tk.kf.keyword5 = 1;
              tk.kf.keyid = T_MAXINT;
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 5;
              tk.kf.keyword5 = 1;
              tk.kf.keyid = T_MININT;
            }
          }
        }
      }
      else if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_MIRROR;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'W' || s[2] == 'w' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_NOWAIT;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o' ) {
    if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_ORIENT;
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 5;
              tk.kf.keyword5 = 1;
              tk.kf.keyid = T_OUTPUT;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_REPEAT;
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_RESUME;
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_RETURN;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_SELECT;
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'G' || s[2] == 'g' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_SIGNAL;
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              tk.kf.keytype = 5;
              tk.kf.keyword5 = 1;
              tk.kf.keyid = T_STATUS;
            }
          }
        }
      }
      else if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'G' || s[5] == 'g' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = T_STRING;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'F' || s[2] == 'f' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              tk.kf.keytype = 5;
              tk.kf.keyword5 = 1;
              tk.kf.keyid = T_TPFUNC;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'U' || s[0] == 'u' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'H' || s[2] == 'h' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_UNHOLD;
            }
          }
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_UNINIT;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'V' || s[0] == 'v' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = T_VECTOR;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'X' || s[0] == 'x' ) {
    if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'Z' || s[2] == 'z' ) {
        if ( s[3] == 'W' || s[3] == 'w' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = T_XYZWPR;
            }
          }
        }
      }
    }
  }
}

static inline void keywordClassify7( const char *s, Token &tk )
{
  if ( s[0] == '$' ) {
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'Y' || s[4] == 'y' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 5;
                tk.kf.keyword5 = 1;
                tk.kf.keyid = T_$MOTYPE;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'F' || s[2] == 'f' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'M' || s[5] == 'm' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 5;
                tk.kf.keyword5 = 1;
                tk.kf.keyid = T_$UFRAME;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = T_BOOLEAN;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                tk.kf.keytype = 2;
                tk.kf.keyword2 = 1;
                tk.kf.keyid = T_CONNECT;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'F' || s[3] == 'f' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                tk.kf.keytype = 5;
                tk.kf.keyword5 = 1;
                tk.kf.keyid = T_CRTFUNC;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'J' || s[3] == 'j' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_CURJPOS;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'B' || s[4] == 'b' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 2;
                tk.kf.keyword2 = 1;
                tk.kf.keyid = T_DISABLE;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'G' || s[6] == 'g' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_GET_REG;
              }
            }
          }
          else if ( s[4] == 'V' || s[4] == 'v' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_GET_VAR;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'G' || s[4] == 'g' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = T_INTEGER;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'B' || s[3] == 'b' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                tk.kf.keytype = 2;
                tk.kf.keyword2 = 1;
                tk.kf.keyid = T_NOABORT;
              }
            }
          }
        }
      }
      else if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                tk.kf.keytype = 5;
                tk.kf.keyword5 = 1;
                tk.kf.keyid = T_NODECEL;
              }
            }
          }
        }
      }
      else if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 2;
                tk.kf.keyword2 = 1;
                tk.kf.keyid = T_NOPAUSE;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                tk.kf.keytype = 5;
                tk.kf.keyword5 = 1;
                tk.kf.keyid = T_PORT_ID;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'G' || s[3] == 'g' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'M' || s[6] == 'm' ) {
                tk.kf.keytype = 2;
                tk.kf.keyword2 = 1;
                tk.kf.keyid = T_PROGRAM;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'K' || s[5] == 'k' ) {
              if ( s[6] == 'B' || s[6] == 'b' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_READ_KB;
              }
            }
          }
        }
      }
      else if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 2;
                tk.kf.keyword2 = 1;
                tk.kf.keyid = T_RELEASE;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 2;
                tk.kf.keyword2 = 1;
                tk.kf.keyid = T_ROUTINE;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'W' || s[2] == 'w' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                tk.kf.keytype = 5;
                tk.kf.keyword5 = 1;
                tk.kf.keyid = T_RSWORLD;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'V' || s[4] == 'v' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_SET_VAR;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_STR_LEN;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'B' || s[2] == 'b' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_SUB_STR;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 5;
                tk.kf.keyword5 = 1;
                tk.kf.keyid = T_TPERROR;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'U' || s[0] == 'u' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 2;
                tk.kf.keyword2 = 1;
                tk.kf.keyid = T_UNPAUSE;
              }
            }
          }
        }
      }
    }
  }
}

static inline void keywordClassify8( const char *s, Token &tk )
{
  if ( s[0] == '$' ) {
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  tk.kf.keytype = 5;
                  tk.kf.keyword5 = 1;
                  tk.kf.keyid = T_$MNUTOOL;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'G' || s[5] == 'g' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'P' || s[7] == 'p' ) {
                  tk.kf.keytype = 5;
                  tk.kf.keyword5 = 1;
                  tk.kf.keyid = T_$MOR_GRP;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == '%' ) {
    if ( s[1] == 'C' || s[1] == 'c' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  tk.kf.keytype = 4;
                  tk.kf.keyword4 = 1;
                  tk.kf.keyid = T__COMMENT;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'U' || s[5] == 'u' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 4;
                  tk.kf.keyword4 = 1;
                  tk.kf.keyid = T__INCLUDE;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'B' || s[4] == 'b' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  tk.kf.keytype = 4;
                  tk.kf.keyword4 = 1;
                  tk.kf.keyid = T__NOABORT;
                }
              }
            }
          }
        }
        else if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'U' || s[5] == 'u' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 4;
                  tk.kf.keyword4 = 1;
                  tk.kf.keyid = T__NOPAUSE;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_ADD_DICT;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'W' || s[3] == 'w' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'D' || s[7] == 'd' ) {
                  tk.kf.keytype = 5;
                  tk.kf.keyword5 = 1;
                  tk.kf.keyid = T_AESWORLD;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                if ( s[7] == 'H' || s[7] == 'h' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_APPROACH;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  tk.kf.keytype = 5;
                  tk.kf.keyword5 = 1;
                  tk.kf.keyid = T_CIRCULAR;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 2;
                  tk.kf.keyword2 = 1;
                  tk.kf.keyid = T_CONTINUE;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'Y' || s[3] == 'y' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'P' || s[6] == 'p' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_COPY_TPE;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  tk.kf.keytype = 5;
                  tk.kf.keyword5 = 1;
                  tk.kf.keyid = T_CRTERROR;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'G' || s[7] == 'g' ) {
                  tk.kf.keytype = 2;
                  tk.kf.keyword2 = 1;
                  tk.kf.keyid = T_ENDUSING;
                }
              }
            }
          }
        }
        else if ( s[3] == 'W' || s[3] == 'w' ) {
          if ( s[4] == 'H' || s[4] == 'h' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 2;
                  tk.kf.keyword2 = 1;
                  tk.kf.keyid = T_ENDWHILE;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_ERR_DATA;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'M' || s[6] == 'm' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_GET_TIME;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == '_' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'G' || s[6] == 'g' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_IN_RANGE;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'J' || s[0] == 'j' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  tk.kf.keytype = 3;
                  tk.kf.keyword3 = 1;
                  tk.kf.keyid = T_JOINTPOS;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 5;
                  tk.kf.keyword5 = 1;
                  tk.kf.keyid = T_NOSETTLE;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o' ) {
    if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'P' || s[6] == 'p' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_OPEN_TPE;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_PATH_LEN;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  tk.kf.keytype = 3;
                  tk.kf.keyword3 = 1;
                  tk.kf.keyid = T_POSITION;
                }
              }
            }
          }
        }
        else if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_POST_ERR;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'V' || s[6] == 'v' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 2;
                  tk.kf.keyword2 = 1;
                  tk.kf.keyid = T_RELATIVE;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'K' || s[7] == 'k' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_RUN_TASK;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'G' || s[7] == 'g' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_SET_LANG;
                }
              }
            }
          }
          else if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'M' || s[6] == 'm' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_SET_TIME;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'M' || s[5] == 'm' ) {
              if ( s[6] == 'P' || s[6] == 'p' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  tk.kf.keytype = 5;
                  tk.kf.keyword5 = 1;
                  tk.kf.keyid = T_TPPROMPT;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  tk.kf.keytype = 5;
                  tk.kf.keyword5 = 1;
                  tk.kf.keyid = T_TPSTATUS;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'V' || s[0] == 'v' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  tk.kf.keytype = 5;
                  tk.kf.keyword5 = 1;
                  tk.kf.keyid = T_VARDECEL;
                }
              }
            }
          }
        }
        else if ( s[3] == '_' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'F' || s[6] == 'f' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_VAR_INFO;
                }
              }
            }
          }
          else if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_VAR_LIST;
                }
              }
            }
          }
        }
      }
    }
  }
}

static inline void keywordClassify9( const char *s, Token &tk )
{
  if ( s[0] == '$' ) {
    if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'L' || s[8] == 'l' ) {
                    tk.kf.keytype = 5;
                    tk.kf.keyword5 = 1;
                    tk.kf.keyid = T_$DECELTOL;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'F' || s[4] == 'f' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'M' || s[7] == 'm' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 5;
                    tk.kf.keyword5 = 1;
                    tk.kf.keyid = T_$MNUFRAME;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'D' || s[8] == 'd' ) {
                    tk.kf.keytype = 5;
                    tk.kf.keyword5 = 1;
                    tk.kf.keyid = T_$ROTSPEED;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'Y' || s[6] == 'y' ) {
                if ( s[7] == 'P' || s[7] == 'p' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 5;
                    tk.kf.keyword5 = 1;
                    tk.kf.keyid = T_$TERMTYPE;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == '%' ) {
    if ( s[1] == 'C' || s[1] == 'c' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'V' || s[5] == 'v' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    tk.kf.keytype = 4;
                    tk.kf.keyword4 = 1;
                    tk.kf.keyid = T__CMOSVARS;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'F' || s[3] == 'f' ) {
          if ( s[4] == 'G' || s[4] == 'g' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'U' || s[7] == 'u' ) {
                  if ( s[8] == 'P' || s[8] == 'p' ) {
                    tk.kf.keytype = 4;
                    tk.kf.keyword4 = 1;
                    tk.kf.keyid = T__DEFGROUP;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'Y' || s[8] == 'y' ) {
                    tk.kf.keytype = 4;
                    tk.kf.keyword4 = 1;
                    tk.kf.keyid = T__PRIORITY;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    tk.kf.keytype = 4;
                    tk.kf.keyword4 = 1;
                    tk.kf.keyid = T__TPMOTION;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'P' || s[7] == 'p' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_ADD_INTPC;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'Y' || s[4] == 'y' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_ARRAY_LEN;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'G' || s[8] == 'g' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_CALL_PROG;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'U' || s[7] == 'u' ) {
                  if ( s[8] == 'P' || s[8] == 'p' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_CAM_SETUP;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'P' || s[7] == 'p' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_CLOSE_TPE;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    tk.kf.keytype = 2;
                    tk.kf.keyword2 = 1;
                    tk.kf.keyid = T_CONDITION;
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  if ( s[8] == 'K' || s[8] == 'k' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_CONT_TASK;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'Y' || s[3] == 'y' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'F' || s[5] == 'f' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_COPY_FILE;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'M' || s[6] == 'm' ) {
                if ( s[7] == 'P' || s[7] == 'p' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    tk.kf.keytype = 5;
                    tk.kf.keyword5 = 1;
                    tk.kf.keyid = T_CRTPROMPT;
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'U' || s[7] == 'u' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    tk.kf.keytype = 5;
                    tk.kf.keyword5 = 1;
                    tk.kf.keyid = T_CRTSTATUS;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'G' || s[8] == 'g' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_CURR_PROG;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'C' || s[7] == 'c' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    tk.kf.keytype = 2;
                    tk.kf.keyword2 = 1;
                    tk.kf.keyid = T_ENDSELECT;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_FILE_LIST;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'Q' || s[4] == 'q' ) {
            if ( s[5] == 'U' || s[5] == 'u' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'U' || s[7] == 'u' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_GET_QUEUE;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == '_' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'U' || s[7] == 'u' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_IO_STATUS;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'J' || s[0] == 'j' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  if ( s[8] == '1' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_JOINTPOS1;
                  }
                  else if ( s[8] == '2' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_JOINTPOS2;
                  }
                  else if ( s[8] == '3' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_JOINTPOS3;
                  }
                  else if ( s[8] == '4' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_JOINTPOS4;
                  }
                  else if ( s[8] == '5' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_JOINTPOS5;
                  }
                  else if ( s[8] == '6' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_JOINTPOS6;
                  }
                  else if ( s[8] == '7' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_JOINTPOS7;
                  }
                  else if ( s[8] == '8' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_JOINTPOS8;
                  }
                  else if ( s[8] == '9' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_JOINTPOS9;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'V' || s[8] == 'v' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_MOUNT_DEV;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'V' || s[2] == 'v' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'F' || s[5] == 'f' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_MOVE_FILE;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'Z' || s[7] == 'z' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_NODE_SIZE;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'G' || s[7] == 'g' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 2;
                    tk.kf.keyword2 = 1;
                    tk.kf.keyid = T_NOMESSAGE;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'M' || s[7] == 'm' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_PEND_SEMA;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'M' || s[7] == 'm' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_POST_SEMA;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'G' || s[3] == 'g' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_PROG_LIST;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'G' || s[3] == 'g' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'V' || s[8] == 'v' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_PURGE_DEV;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'C' || s[7] == 'c' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_READ_DICT;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'H' || s[5] == 'h' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_SEMAPHORE;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'C' || s[7] == 'c' ) {
                  if ( s[8] == 'H' || s[8] == 'h' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_SET_PERCH;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 2;
                    tk.kf.keyword2 = 1;
                    tk.kf.keyid = T_STRUCTURE;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'Y' || s[8] == 'y' ) {
                    tk.kf.keytype = 5;
                    tk.kf.keyword5 = 1;
                    tk.kf.keyid = T_TPDISPLAY;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_TRANSLATE;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'V' || s[0] == 'v' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'C' || s[7] == 'c' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_VOL_SPACE;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'X' || s[0] == 'x' ) {
    if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'Z' || s[2] == 'z' ) {
        if ( s[3] == 'W' || s[3] == 'w' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'X' || s[7] == 'x' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_XYZWPREXT;
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

static inline void keywordClassify10( const char *s, Token &tk )
{
  if ( s[0] == '%' ) {
    if ( s[1] == 'C' || s[1] == 'c' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'V' || s[6] == 'v' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 4;
                      tk.kf.keyword4 = 1;
                      tk.kf.keyid = T__CRTDEVICE;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'K' || s[4] == 'k' ) {
            if ( s[5] == 'G' || s[5] == 'g' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'U' || s[8] == 'u' ) {
                    if ( s[9] == 'P' || s[9] == 'p' ) {
                      tk.kf.keytype = 4;
                      tk.kf.keyword4 = 1;
                      tk.kf.keyid = T__LOCKGROUP;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'K' || s[5] == 'k' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'Z' || s[8] == 'z' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 4;
                      tk.kf.keyword4 = 1;
                      tk.kf.keyid = T__STACKSIZE;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 4;
                      tk.kf.keyword4 = 1;
                      tk.kf.keyid = T__TIMESLICE;
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
  else if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'B' || s[1] == 'b' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    if ( s[9] == 'K' || s[9] == 'k' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_ABORT_TASK;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'C' || s[1] == 'c' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    if ( s[9] == 'N' || s[9] == 'n' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_ACT_SCREEN;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  if ( s[8] == 'P' || s[8] == 'p' ) {
                    if ( s[9] == 'C' || s[9] == 'c' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_ADD_REALPC;
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
  else if ( s[0] == 'B' || s[0] == 'b' ) {
    if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'F' || s[8] == 'f' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_BYTES_LEFT;
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
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'K' || s[4] == 'k' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_CHECK_DICT;
                    }
                  }
                }
              }
              else if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'P' || s[7] == 'p' ) {
                  if ( s[8] == 'O' || s[8] == 'o' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_CHECK_EPOS;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'M' || s[8] == 'm' ) {
                    if ( s[9] == 'A' || s[9] == 'a' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_CLEAR_SEMA;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'Y' || s[3] == 'y' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'Q' || s[5] == 'q' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'U' || s[8] == 'u' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_COPY_QUEUE;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'P' || s[8] == 'p' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_CREATE_TPE;
                    }
                  }
                }
                else if ( s[7] == 'V' || s[7] == 'v' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    if ( s[9] == 'R' || s[9] == 'r' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_CREATE_VAR;
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
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'F' || s[2] == 'f' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    if ( s[9] == 'N' || s[9] == 'n' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_DEF_SCREEN;
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'W' || s[4] == 'w' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'D' || s[7] == 'd' ) {
                  if ( s[8] == 'O' || s[8] == 'o' ) {
                    if ( s[9] == 'W' || s[9] == 'w' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_DEF_WINDOW;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'W' || s[4] == 'w' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'D' || s[7] == 'd' ) {
                  if ( s[8] == 'O' || s[8] == 'o' ) {
                    if ( s[9] == 'W' || s[9] == 'w' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_DET_WINDOW;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      tk.kf.keytype = 2;
                      tk.kf.keyword2 = 1;
                      tk.kf.keyid = T_DISCONNECT;
                    }
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      tk.kf.keytype = 3;
                      tk.kf.keyword3 = 1;
                      tk.kf.keyid = T_DISP_DAT_T;
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
  else if ( s[0] == 'F' || s[0] == 'f' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'D' || s[7] == 'd' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    if ( s[9] == 'V' || s[9] == 'v' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_FORMAT_DEV;
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
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'Q' || s[5] == 'q' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'U' || s[8] == 'u' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_INIT_QUEUE;
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
  else if ( s[0] == 'J' || s[0] == 'j' ) {
    if ( s[1] == '_' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'G' || s[8] == 'g' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_J_IN_RANGE;
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
  else if ( s[0] == 'K' || s[0] == 'k' ) {
    if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'U' || s[8] == 'u' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_KLC_STATUS;
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
  else if ( s[0] == 'L' || s[0] == 'l' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'K' || s[3] == 'k' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'G' || s[5] == 'g' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'U' || s[8] == 'u' ) {
                    if ( s[9] == 'P' || s[9] == 'p' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_LOCK_GROUP;
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
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'C' || s[7] == 'c' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'L' || s[9] == 'l' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_MOTION_CTL;
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
  else if ( s[0] == 'P' || s[0] == 'p' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    if ( s[9] == 'K' || s[9] == 'k' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_PAUSE_TASK;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'K' || s[4] == 'k' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'Y' || s[6] == 'y' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'D' || s[9] == 'd' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_POP_KEY_RD;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'F' || s[6] == 'f' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'L' || s[8] == 'l' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_PRINT_FILE;
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
  else if ( s[0] == 'Q' || s[0] == 'q' ) {
    if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'Y' || s[7] == 'y' ) {
                  if ( s[8] == 'P' || s[8] == 'p' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 3;
                      tk.kf.keyword3 = 1;
                      tk.kf.keyid = T_QUEUE_TYPE;
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
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'V' || s[7] == 'v' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    if ( s[9] == 'R' || s[9] == 'r' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_RENAME_VAR;
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
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'P' || s[8] == 'p' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_SELECT_TPE;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'U' || s[7] == 'u' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_SEMA_COUNT;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'U' || s[5] == 'u' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  if ( s[8] == 'O' || s[8] == 'o' ) {
                    if ( s[9] == 'R' || s[9] == 'r' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_SET_CURSOR;
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
  else if ( s[0] == 'W' || s[0] == 'w' ) {
    if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'J' || s[5] == 'j' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      tk.kf.keytype = 5;
                      tk.kf.keyword5 = 1;
                      tk.kf.keyid = T_WRISTJOINT;
                    }
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_WRITE_DICT;
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

static inline void keywordClassify11( const char *s, Token &tk )
{
  if ( s[0] == '$' ) {
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'U' || s[9] == 'u' ) {
                      if ( s[10] == 'M' || s[10] == 'm' ) {
                        tk.kf.keytype = 5;
                        tk.kf.keyword5 = 1;
                        tk.kf.keyid = T_$MNUTOOLNUM;
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
  else if ( s[0] == '%' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'B' || s[3] == 'b' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'Y' || s[6] == 'y' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    if ( s[9] == 'M' || s[9] == 'm' ) {
                      if ( s[10] == 'P' || s[10] == 'p' ) {
                        tk.kf.keytype = 4;
                        tk.kf.keyword4 = 1;
                        tk.kf.keyid = T__NOBUSYLAMP;
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
  else if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'O' || s[8] == 'o' ) {
                    if ( s[9] == 'D' || s[9] == 'd' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_APPEND_NODE;
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
    else if ( s[1] == 'V' || s[1] == 'v' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'U' || s[9] == 'u' ) {
                      if ( s[10] == 'M' || s[10] == 'm' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_AVL_POS_NUM;
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
  else if ( s[0] == 'B' || s[0] == 'b' ) {
    if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'H' || s[7] == 'h' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    if ( s[9] == 'A' || s[9] == 'a' ) {
                      if ( s[10] == 'D' || s[10] == 'd' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_BYTES_AHEAD;
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
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'A' || s[9] == 'a' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_CLR_IO_STAT;
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'G' || s[10] == 'g' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_CLR_POS_REG;
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
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'V' || s[2] == 'v' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == 'R' || s[10] == 'r' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_CNV_INT_STR;
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'I' || s[8] == 'i' ) {
                    if ( s[9] == 'N' || s[9] == 'n' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_CNV_STR_INT;
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
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'F' || s[7] == 'f' ) {
                  if ( s[8] == 'I' || s[8] == 'i' ) {
                    if ( s[9] == 'L' || s[9] == 'l' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_DELETE_FILE;
                      }
                    }
                  }
                }
                else if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'O' || s[8] == 'o' ) {
                    if ( s[9] == 'D' || s[9] == 'd' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_DELETE_NODE;
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
  else if ( s[0] == 'G' || s[0] == 'g' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'F' || s[8] == 'f' ) {
                    if ( s[9] == 'R' || s[9] == 'r' ) {
                      if ( s[10] == 'M' || s[10] == 'm' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_GET_POS_FRM;
                      }
                    }
                  }
                  else if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'G' || s[10] == 'g' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_GET_POS_REG;
                      }
                    }
                  }
                  else if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'P' || s[9] == 'p' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_GET_POS_TPE;
                      }
                    }
                    else if ( s[9] == 'Y' || s[9] == 'y' ) {
                      if ( s[10] == 'P' || s[10] == 'p' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_GET_POS_TYP;
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
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    if ( s[9] == 'O' || s[9] == 'o' ) {
                      if ( s[10] == 'C' || s[10] == 'c' ) {
                        tk.kf.keytype = 3;
                        tk.kf.keyword3 = 1;
                        tk.kf.keyid = T_GROUP_ASSOC;
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
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'O' || s[8] == 'o' ) {
                    if ( s[9] == 'D' || s[9] == 'd' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_INSERT_NODE;
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
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == '_' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'Y' || s[8] == 'y' ) {
                    if ( s[9] == 'P' || s[9] == 'p' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_IO_MOD_TYPE;
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
  else if ( s[0] == 'K' || s[0] == 'k' ) {
    if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'W' || s[7] == 'w' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    if ( s[9] == 'I' || s[9] == 'i' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_KLC_NO_WAIT;
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
  else if ( s[0] == 'L' || s[0] == 'l' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'U' || s[9] == 'u' ) {
                      if ( s[10] == 'S' || s[10] == 's' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_LOAD_STATUS;
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
  else if ( s[0] == 'P' || s[0] == 'p' ) {
    if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'K' || s[5] == 'k' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'Y' || s[7] == 'y' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'R' || s[9] == 'r' ) {
                      if ( s[10] == 'D' || s[10] == 'd' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_PUSH_KEY_RD;
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
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'C' || s[7] == 'c' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == '_' ) {
                      if ( s[10] == 'V' || s[10] == 'v' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_READ_DICT_V;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'V' || s[4] == 'v' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'D' || s[7] == 'd' ) {
                  if ( s[8] == 'I' || s[8] == 'i' ) {
                    if ( s[9] == 'C' || s[9] == 'c' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_REMOVE_DICT;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'F' || s[7] == 'f' ) {
                  if ( s[8] == 'I' || s[8] == 'i' ) {
                    if ( s[9] == 'L' || s[9] == 'l' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_RENAME_FILE;
                      }
                    }
                  }
                }
                else if ( s[7] == 'V' || s[7] == 'v' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    if ( s[9] == 'R' || s[9] == 'r' ) {
                      if ( s[10] == 'S' || s[10] == 's' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_RENAME_VARS;
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
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    if ( s[9] == 'P' || s[9] == 'p' ) {
                      if ( s[10] == 'C' || s[10] == 'c' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_SEND_DATAPC;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'G' || s[10] == 'g' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_SET_INT_REG;
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'G' || s[10] == 'g' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_SET_POS_REG;
                      }
                    }
                  }
                  else if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'P' || s[9] == 'p' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_SET_POS_TPE;
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
  else if ( s[0] == 'V' || s[0] == 'v' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'O' || s[9] == 'o' ) {
                      if ( s[10] == 'R' || s[10] == 'r' ) {
                        tk.kf.keytype = 5;
                        tk.kf.keyword5 = 1;
                        tk.kf.keyid = T_VIS_MONITOR;
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'C' || s[7] == 'c' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      if ( s[10] == 'S' || s[10] == 's' ) {
                        tk.kf.keytype = 3;
                        tk.kf.keyword3 = 1;
                        tk.kf.keyid = T_VIS_PROCESS;
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

static inline void keywordClassify12( const char *s, Token &tk )
{
  if ( s[0] == '$' ) {
    if ( s[1] == 'G' || s[1] == 'g' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'V' || s[5] == 'v' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'I' || s[9] == 'i' ) {
                      if ( s[10] == 'D' || s[10] == 'd' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 5;
                          tk.kf.keyword5 = 1;
                          tk.kf.keyid = T_$GENOVERRIDE;
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
    else if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'F' || s[4] == 'f' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'M' || s[7] == 'm' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    if ( s[9] == 'N' || s[9] == 'n' ) {
                      if ( s[10] == 'U' || s[10] == 'u' ) {
                        if ( s[11] == 'M' || s[11] == 'm' ) {
                          tk.kf.keytype = 5;
                          tk.kf.keyword5 = 1;
                          tk.kf.keyid = T_$MNUFRAMENUM;
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
  else if ( s[0] == '%' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'H' || s[4] == 'h' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'B' || s[6] == 'b' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'I' || s[9] == 'i' ) {
                      if ( s[10] == 'Z' || s[10] == 'z' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 4;
                          tk.kf.keyword4 = 1;
                          tk.kf.keyid = T__ALPHABETIZE;
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
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'V' || s[3] == 'v' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'M' || s[8] == 'm' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'N' || s[10] == 'n' ) {
                        if ( s[11] == 'T' || s[11] == 't' ) {
                          tk.kf.keytype = 4;
                          tk.kf.keyword4 = 1;
                          tk.kf.keyid = T__ENVIRONMENT;
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
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'K' || s[6] == 'k' ) {
                if ( s[7] == 'G' || s[7] == 'g' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'O' || s[9] == 'o' ) {
                      if ( s[10] == 'U' || s[10] == 'u' ) {
                        if ( s[11] == 'P' || s[11] == 'p' ) {
                          tk.kf.keytype = 4;
                          tk.kf.keyword4 = 1;
                          tk.kf.keyid = T__NOLOCKGROUP;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'U' || s[5] == 'u' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    if ( s[9] == 'H' || s[9] == 'h' ) {
                      if ( s[10] == 'F' || s[10] == 'f' ) {
                        if ( s[11] == 'T' || s[11] == 't' ) {
                          tk.kf.keytype = 4;
                          tk.kf.keyword4 = 1;
                          tk.kf.keyid = T__NOPAUSESHFT;
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
  else if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'B' || s[4] == 'b' ) {
            if ( s[5] == 'Y' || s[5] == 'y' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'M' || s[8] == 'm' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'P' || s[10] == 'p' ) {
                        if ( s[11] == 'C' || s[11] == 'c' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_ADD_BYNAMEPC;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'G' || s[9] == 'g' ) {
                      if ( s[10] == 'P' || s[10] == 'p' ) {
                        if ( s[11] == 'C' || s[11] == 'c' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_ADD_STRINGPC;
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
    else if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'Q' || s[7] == 'q' ) {
                  if ( s[8] == 'U' || s[8] == 'u' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'U' || s[10] == 'u' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_APPEND_QUEUE;
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
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'W' || s[4] == 'w' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'D' || s[7] == 'd' ) {
                  if ( s[8] == 'O' || s[8] == 'o' ) {
                    if ( s[9] == 'W' || s[9] == 'w' ) {
                      if ( s[10] == '_' ) {
                        if ( s[11] == 'D' || s[11] == 'd' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_ATT_WINDOW_D;
                        }
                        else if ( s[11] == 'S' || s[11] == 's' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_ATT_WINDOW_S;
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
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'G' || s[8] == 'g' ) {
                    if ( s[9] == 'L' || s[9] == 'l' ) {
                      if ( s[10] == 'I' || s[10] == 'i' ) {
                        if ( s[11] == 'N' || s[11] == 'n' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CALL_PROGLIN;
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
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      if ( s[10] == 'I' || s[10] == 'i' ) {
                        if ( s[11] == 'M' || s[11] == 'm' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CLR_PORT_SIM;
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
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'P' || s[7] == 'p' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'M' || s[9] == 'm' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        if ( s[11] == 'N' || s[11] == 'n' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNCL_STP_MTN;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == '_' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            if ( s[5] == 'Y' || s[5] == 'y' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'D' || s[8] == 'd' ) {
                    if ( s[9] == 'I' || s[9] == 'i' ) {
                      if ( s[10] == 'S' || s[10] == 's' ) {
                        if ( s[11] == 'B' || s[11] == 'b' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNC_DYN_DISB;
                        }
                        else if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNC_DYN_DISE;
                        }
                        else if ( s[11] == 'I' || s[11] == 'i' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNC_DYN_DISI;
                        }
                        else if ( s[11] == 'P' || s[11] == 'p' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNC_DYN_DISP;
                        }
                        else if ( s[11] == 'R' || s[11] == 'r' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNC_DYN_DISR;
                        }
                        else if ( s[11] == 'S' || s[11] == 's' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNC_DYN_DISS;
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
      else if ( s[2] == 'V' || s[2] == 'v' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'F' || s[7] == 'f' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        if ( s[11] == 'R' || s[11] == 'r' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNV_CONF_STR;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'J' || s[4] == 'j' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'R' || s[9] == 'r' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        if ( s[11] == 'L' || s[11] == 'l' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNV_JPOS_REL;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        if ( s[11] == 'R' || s[11] == 'r' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNV_REAL_STR;
                        }
                      }
                    }
                  }
                }
              }
              else if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'J' || s[8] == 'j' ) {
                    if ( s[9] == 'P' || s[9] == 'p' ) {
                      if ( s[10] == 'O' || s[10] == 'o' ) {
                        if ( s[11] == 'S' || s[11] == 's' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNV_REL_JPOS;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    if ( s[9] == 'O' || s[9] == 'o' ) {
                      if ( s[10] == 'N' || s[10] == 'n' ) {
                        if ( s[11] == 'F' || s[11] == 'f' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNV_STR_CONF;
                        }
                      }
                    }
                  }
                  else if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'A' || s[10] == 'a' ) {
                        if ( s[11] == 'L' || s[11] == 'l' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNV_STR_REAL;
                        }
                      }
                    }
                  }
                  else if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'I' || s[9] == 'i' ) {
                      if ( s[10] == 'M' || s[10] == 'm' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNV_STR_TIME;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'M' || s[6] == 'm' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        if ( s[11] == 'R' || s[11] == 'r' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_CNV_TIME_STR;
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
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      if ( s[10] == 'O' || s[10] == 'o' ) {
                        if ( s[11] == 'C' || s[11] == 'c' ) {
                          tk.kf.keytype = 3;
                          tk.kf.keyword3 = 1;
                          tk.kf.keyid = T_COMMON_ASSOC;
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
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'Q' || s[7] == 'q' ) {
                  if ( s[8] == 'U' || s[8] == 'u' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'U' || s[10] == 'u' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_DELETE_QUEUE;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == '_' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == 'P' || s[10] == 'p' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_DEL_INST_TPE;
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
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    if ( s[9] == 'L' || s[9] == 'l' ) {
                      if ( s[10] == 'P' || s[10] == 'p' ) {
                        if ( s[11] == 'H' || s[11] == 'h' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_DISCTRL_ALPH;
                        }
                      }
                    }
                  }
                  else if ( s[8] == 'F' || s[8] == 'f' ) {
                    if ( s[9] == 'O' || s[9] == 'o' ) {
                      if ( s[10] == 'R' || s[10] == 'r' ) {
                        if ( s[11] == 'M' || s[11] == 'm' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_DISCTRL_FORM;
                        }
                      }
                    }
                  }
                  else if ( s[8] == 'L' || s[8] == 'l' ) {
                    if ( s[9] == 'I' || s[9] == 'i' ) {
                      if ( s[10] == 'S' || s[10] == 's' ) {
                        if ( s[11] == 'T' || s[11] == 't' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_DISCTRL_LIST;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'U' || s[5] == 'u' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'D' || s[9] == 'd' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        if ( s[11] == 'V' || s[11] == 'v' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_DISMOUNT_DEV;
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
  else if ( s[0] == 'E' || s[0] == 'e' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'I' || s[9] == 'i' ) {
                      if ( s[10] == 'O' || s[10] == 'o' ) {
                        if ( s[11] == 'N' || s[11] == 'n' ) {
                          tk.kf.keytype = 2;
                          tk.kf.keyword2 = 1;
                          tk.kf.keyid = T_ENDCONDITION;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'C' || s[7] == 'c' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'U' || s[9] == 'u' ) {
                      if ( s[10] == 'R' || s[10] == 'r' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 2;
                          tk.kf.keyword2 = 1;
                          tk.kf.keyid = T_ENDSTRUCTURE;
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
  else if ( s[0] == 'F' || s[0] == 'f' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'P' || s[7] == 'p' ) {
                  if ( s[8] == 'M' || s[8] == 'm' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'N' || s[10] == 'n' ) {
                        if ( s[11] == 'U' || s[11] == 'u' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_FORCE_SPMENU;
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
  else if ( s[0] == 'G' || s[0] == 'g' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'P' || s[9] == 'p' ) {
                      if ( s[10] == 'R' || s[10] == 'r' ) {
                        if ( s[11] == 'G' || s[11] == 'g' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_GET_ATTR_PRG;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'F' || s[4] == 'f' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'P' || s[9] == 'p' ) {
                      if ( s[10] == 'O' || s[10] == 'o' ) {
                        if ( s[11] == 'S' || s[11] == 's' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_GET_FILE_POS;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'J' || s[4] == 'j' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'R' || s[9] == 'r' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        if ( s[11] == 'G' || s[11] == 'g' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_GET_JPOS_REG;
                        }
                      }
                    }
                    else if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == 'P' || s[10] == 'p' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_GET_JPOS_TPE;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'A' || s[9] == 'a' ) {
                      if ( s[10] == 'S' || s[10] == 's' ) {
                        if ( s[11] == 'G' || s[11] == 'g' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_GET_PORT_ASG;
                        }
                      }
                      else if ( s[10] == 'T' || s[10] == 't' ) {
                        if ( s[11] == 'R' || s[11] == 'r' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_GET_PORT_ATR;
                        }
                      }
                    }
                    else if ( s[9] == 'C' || s[9] == 'c' ) {
                      if ( s[10] == 'M' || s[10] == 'm' ) {
                        if ( s[11] == 'T' || s[11] == 't' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_GET_PORT_CMT;
                        }
                      }
                    }
                    else if ( s[9] == 'M' || s[9] == 'm' ) {
                      if ( s[10] == 'O' || s[10] == 'o' ) {
                        if ( s[11] == 'D' || s[11] == 'd' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_GET_PORT_MOD;
                        }
                      }
                    }
                    else if ( s[9] == 'S' || s[9] == 's' ) {
                      if ( s[10] == 'I' || s[10] == 'i' ) {
                        if ( s[11] == 'M' || s[11] == 'm' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_GET_PORT_SIM;
                        }
                      }
                    }
                    else if ( s[9] == 'V' || s[9] == 'v' ) {
                      if ( s[10] == 'A' || s[10] == 'a' ) {
                        if ( s[11] == 'L' || s[11] == 'l' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_GET_PORT_VAL;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'K' || s[6] == 'k' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'I' || s[8] == 'i' ) {
                    if ( s[9] == 'N' || s[9] == 'n' ) {
                      if ( s[10] == 'F' || s[10] == 'f' ) {
                        if ( s[11] == 'O' || s[11] == 'o' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_GET_TSK_INFO;
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
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            if ( s[5] == 'Y' || s[5] == 'y' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'D' || s[8] == 'd' ) {
                    if ( s[9] == 'I' || s[9] == 'i' ) {
                      if ( s[10] == 'S' || s[10] == 's' ) {
                        if ( s[11] == 'B' || s[11] == 'b' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_INI_DYN_DISB;
                        }
                        else if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_INI_DYN_DISE;
                        }
                        else if ( s[11] == 'I' || s[11] == 'i' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_INI_DYN_DISI;
                        }
                        else if ( s[11] == 'P' || s[11] == 'p' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_INI_DYN_DISP;
                        }
                        else if ( s[11] == 'R' || s[11] == 'r' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_INI_DYN_DISR;
                        }
                        else if ( s[11] == 'S' || s[11] == 's' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_INI_DYN_DISS;
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
      else if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'Q' || s[7] == 'q' ) {
                  if ( s[8] == 'U' || s[8] == 'u' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'U' || s[10] == 'u' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_INSERT_QUEUE;
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
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'F' || s[4] == 'f' ) {
            if ( s[5] == 'Y' || s[5] == 'y' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'Q' || s[7] == 'q' ) {
                  if ( s[8] == 'U' || s[8] == 'u' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'U' || s[10] == 'u' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_MODIFY_QUEUE;
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
  else if ( s[0] == 'P' || s[0] == 'p' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'G' || s[6] == 'g' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'Y' || s[9] == 'y' ) {
                      if ( s[10] == 'P' || s[10] == 'p' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_POS_REG_TYPE;
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
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == '_' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'V' || s[6] == 'v' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == 'P' || s[10] == 'p' ) {
                        if ( s[11] == 'C' || s[11] == 'c' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SEND_EVENTPC;
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
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'P' || s[9] == 'p' ) {
                      if ( s[10] == 'R' || s[10] == 'r' ) {
                        if ( s[11] == 'G' || s[11] == 'g' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_ATTR_PRG;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'R' || s[9] == 'r' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        if ( s[11] == 'G' || s[11] == 'g' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_EPOS_REG;
                        }
                      }
                    }
                    else if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == 'P' || s[10] == 'p' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_EPOS_TPE;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'F' || s[4] == 'f' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'A' || s[9] == 'a' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        if ( s[11] == 'R' || s[11] == 'r' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_FILE_ATR;
                        }
                      }
                    }
                    else if ( s[9] == 'P' || s[9] == 'p' ) {
                      if ( s[10] == 'O' || s[10] == 'o' ) {
                        if ( s[11] == 'S' || s[11] == 's' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_FILE_POS;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'J' || s[4] == 'j' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'R' || s[9] == 'r' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        if ( s[11] == 'G' || s[11] == 'g' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_JPOS_REG;
                        }
                      }
                    }
                    else if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == 'P' || s[10] == 'p' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_JPOS_TPE;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'A' || s[9] == 'a' ) {
                      if ( s[10] == 'S' || s[10] == 's' ) {
                        if ( s[11] == 'G' || s[11] == 'g' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_PORT_ASG;
                        }
                      }
                      else if ( s[10] == 'T' || s[10] == 't' ) {
                        if ( s[11] == 'R' || s[11] == 'r' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_PORT_ATR;
                        }
                      }
                    }
                    else if ( s[9] == 'C' || s[9] == 'c' ) {
                      if ( s[10] == 'M' || s[10] == 'm' ) {
                        if ( s[11] == 'T' || s[11] == 't' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_PORT_CMT;
                        }
                      }
                    }
                    else if ( s[9] == 'M' || s[9] == 'm' ) {
                      if ( s[10] == 'O' || s[10] == 'o' ) {
                        if ( s[11] == 'D' || s[11] == 'd' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_PORT_MOD;
                        }
                      }
                    }
                    else if ( s[9] == 'S' || s[9] == 's' ) {
                      if ( s[10] == 'I' || s[10] == 'i' ) {
                        if ( s[11] == 'M' || s[11] == 'm' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_PORT_SIM;
                        }
                      }
                    }
                    else if ( s[9] == 'V' || s[9] == 'v' ) {
                      if ( s[10] == 'A' || s[10] == 'a' ) {
                        if ( s[11] == 'L' || s[11] == 'l' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_PORT_VAL;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  if ( s[8] == '_' ) {
                    if ( s[9] == 'R' || s[9] == 'r' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        if ( s[11] == 'G' || s[11] == 'g' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_REAL_REG;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'K' || s[6] == 'k' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        if ( s[11] == 'R' || s[11] == 'r' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_TSK_ATTR;
                        }
                      }
                    }
                  }
                  else if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'A' || s[9] == 'a' ) {
                      if ( s[10] == 'M' || s[10] == 'm' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_SET_TSK_NAME;
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
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == '_' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'H' || s[6] == 'h' ) {
                if ( s[7] == '_' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'O' || s[9] == 'o' ) {
                      if ( s[10] == 'D' || s[10] == 'd' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 3;
                          tk.kf.keyword3 = 1;
                          tk.kf.keyid = T_STD_PTH_NODE;
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
  else if ( s[0] == 'U' || s[0] == 'u' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'K' || s[5] == 'k' ) {
              if ( s[6] == '_' ) {
                if ( s[7] == 'G' || s[7] == 'g' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'O' || s[9] == 'o' ) {
                      if ( s[10] == 'U' || s[10] == 'u' ) {
                        if ( s[11] == 'P' || s[11] == 'p' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_UNLOCK_GROUP;
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
  else if ( s[0] == 'W' || s[0] == 'w' ) {
    if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == '_' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == '_' ) {
                        if ( s[11] == 'V' || s[11] == 'v' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_WRITE_DICT_V;
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

void keywordClassify( const char *s, int l, Token &tk )
{
  switch (l) {
    case 2: keywordClassify2(s, tk); break;
    case 3: keywordClassify3(s, tk); break;
    case 4: keywordClassify4(s, tk); break;
    case 5: keywordClassify5(s, tk); break;
    case 6: keywordClassify6(s, tk); break;
    case 7: keywordClassify7(s, tk); break;
    case 8: keywordClassify8(s, tk); break;
    case 9: keywordClassify9(s, tk); break;
    case 10: keywordClassify10(s, tk); break;
    case 11: keywordClassify11(s, tk); break;
    case 12: keywordClassify12(s, tk); break;
  }
}

void Keywords::classify(const char *s, int l, Token &tk)
{ keywordClassify(s, l, tk); }

static const char *keywordNames[] = {
    "",
    "CR",
    "DO",
    "GO",
    "IF",
    "IN",
    "LN",
    "OF",
    "ON",
    "OR",
    "TO",
    "ABS",
    "AIN",
    "AND",
    "CHR",
    "COS",
    "DIN",
    "DIV",
    "END",
    "EXP",
    "FOR",
    "GIN",
    "INV",
    "KLC",
    "MOD",
    "NOT",
    "OFF",
    "ORD",
    "POS",
    "RDI",
    "RDO",
    "SIN",
    "TAN",
    "VAR",
    "VIA",
    "WDI",
    "$MCR",
    "ACOS",
    "AOUT",
    "ASIN",
    "AWAY",
    "AXIS",
    "BYTE",
    "CASE",
    "CMOS",
    "DOUT",
    "DRAM",
    "ELSE",
    "EVAL",
    "FILE",
    "FINE",
    "FROM",
    "GOUT",
    "HAND",
    "HOLD",
    "LOAD",
    "MOVE",
    "NEAR",
    "OPEN",
    "OPIN",
    "PATH",
    "READ",
    "REAL",
    "SAVE",
    "SQRT",
    "STOP",
    "THEN",
    "TPIN",
    "TRUE",
    "TYPE",
    "WAIT",
    "WHEN",
    "WITH",
    "$NILP",
    "ABORT",
    "ABOUT",
    "ALONG",
    "ARRAY",
    "ATAN2",
    "BEGIN",
    "CLEAR",
    "CLOSE",
    "CONST",
    "DELAY",
    "ENDIF",
    "EVENT",
    "FALSE",
    "FRAME",
    "INDEX",
    "INPUT",
    "JOINT",
    "MODEL",
    "OPOUT",
    "PAUSE",
    "PULSE",
    "PURGE",
    "RELAX",
    "RESET",
    "ROUND",
    "SHORT",
    "TIMER",
    "TPOUT",
    "TRUNC",
    "UNPOS",
    "UNTIL",
    "USING",
    "WDOUT",
    "WHILE",
    "WRITE",
    "$GROUP",
    "$SPEED",
    "$UTOOL",
    "_DELAY",
    "ATTACH",
    "BYNAME",
    "CANCEL",
    "COARSE",
    "CONFIG",
    "CURPOS",
    "DOWNTO",
    "ENABLE",
    "ENDFOR",
    "LINEAR",
    "MAXINT",
    "MININT",
    "MIRROR",
    "NOWAIT",
    "ORIENT",
    "OUTPUT",
    "REPEAT",
    "RESUME",
    "RETURN",
    "SELECT",
    "SIGNAL",
    "STATUS",
    "STRING",
    "TPFUNC",
    "UNHOLD",
    "UNINIT",
    "VECTOR",
    "XYZWPR",
    "$MOTYPE",
    "$UFRAME",
    "BOOLEAN",
    "CONNECT",
    "CRTFUNC",
    "CURJPOS",
    "DISABLE",
    "GET_REG",
    "GET_VAR",
    "INTEGER",
    "NOABORT",
    "NODECEL",
    "NOPAUSE",
    "PORT_ID",
    "PROGRAM",
    "READ_KB",
    "RELEASE",
    "ROUTINE",
    "RSWORLD",
    "SET_VAR",
    "STR_LEN",
    "SUB_STR",
    "TPERROR",
    "UNPAUSE",
    "$MNUTOOL",
    "$MOR_GRP",
    "_COMMENT",
    "_INCLUDE",
    "_NOABORT",
    "_NOPAUSE",
    "ADD_DICT",
    "AESWORLD",
    "APPROACH",
    "CIRCULAR",
    "CONTINUE",
    "COPY_TPE",
    "CRTERROR",
    "ENDUSING",
    "ENDWHILE",
    "ERR_DATA",
    "GET_TIME",
    "IN_RANGE",
    "JOINTPOS",
    "NOSETTLE",
    "OPEN_TPE",
    "PATH_LEN",
    "POSITION",
    "POST_ERR",
    "RELATIVE",
    "RUN_TASK",
    "SET_LANG",
    "SET_TIME",
    "TPPROMPT",
    "TPSTATUS",
    "VARDECEL",
    "VAR_INFO",
    "VAR_LIST",
    "$DECELTOL",
    "$MNUFRAME",
    "$ROTSPEED",
    "$TERMTYPE",
    "_CMOSVARS",
    "_DEFGROUP",
    "_PRIORITY",
    "_TPMOTION",
    "ADD_INTPC",
    "ARRAY_LEN",
    "CALL_PROG",
    "CAM_SETUP",
    "CLOSE_TPE",
    "CONDITION",
    "CONT_TASK",
    "COPY_FILE",
    "CRTPROMPT",
    "CRTSTATUS",
    "CURR_PROG",
    "ENDSELECT",
    "FILE_LIST",
    "GET_QUEUE",
    "IO_STATUS",
    "JOINTPOS1",
    "JOINTPOS2",
    "JOINTPOS3",
    "JOINTPOS4",
    "JOINTPOS5",
    "JOINTPOS6",
    "JOINTPOS7",
    "JOINTPOS8",
    "JOINTPOS9",
    "MOUNT_DEV",
    "MOVE_FILE",
    "NODE_SIZE",
    "NOMESSAGE",
    "PEND_SEMA",
    "POST_SEMA",
    "PROG_LIST",
    "PURGE_DEV",
    "READ_DICT",
    "SEMAPHORE",
    "SET_PERCH",
    "STRUCTURE",
    "TPDISPLAY",
    "TRANSLATE",
    "VOL_SPACE",
    "XYZWPREXT",
    "_CRTDEVICE",
    "_LOCKGROUP",
    "_STACKSIZE",
    "_TIMESLICE",
    "ABORT_TASK",
    "ACT_SCREEN",
    "ADD_REALPC",
    "BYTES_LEFT",
    "CHECK_DICT",
    "CHECK_EPOS",
    "CLEAR_SEMA",
    "COPY_QUEUE",
    "CREATE_TPE",
    "CREATE_VAR",
    "DEF_SCREEN",
    "DEF_WINDOW",
    "DET_WINDOW",
    "DISCONNECT",
    "DISP_DAT_T",
    "FORMAT_DEV",
    "INIT_QUEUE",
    "J_IN_RANGE",
    "KLC_STATUS",
    "LOCK_GROUP",
    "MOTION_CTL",
    "PAUSE_TASK",
    "POP_KEY_RD",
    "PRINT_FILE",
    "QUEUE_TYPE",
    "RENAME_VAR",
    "SELECT_TPE",
    "SEMA_COUNT",
    "SET_CURSOR",
    "WRISTJOINT",
    "WRITE_DICT",
    "$MNUTOOLNUM",
    "_NOBUSYLAMP",
    "APPEND_NODE",
    "AVL_POS_NUM",
    "BYTES_AHEAD",
    "CLR_IO_STAT",
    "CLR_POS_REG",
    "CNV_INT_STR",
    "CNV_STR_INT",
    "DELETE_FILE",
    "DELETE_NODE",
    "GET_POS_FRM",
    "GET_POS_REG",
    "GET_POS_TPE",
    "GET_POS_TYP",
    "GROUP_ASSOC",
    "INSERT_NODE",
    "IO_MOD_TYPE",
    "KLC_NO_WAIT",
    "LOAD_STATUS",
    "PUSH_KEY_RD",
    "READ_DICT_V",
    "REMOVE_DICT",
    "RENAME_FILE",
    "RENAME_VARS",
    "SEND_DATAPC",
    "SET_INT_REG",
    "SET_POS_REG",
    "SET_POS_TPE",
    "VIS_MONITOR",
    "VIS_PROCESS",
    "$GENOVERRIDE",
    "$MNUFRAMENUM",
    "_ALPHABETIZE",
    "_ENVIRONMENT",
    "_NOLOCKGROUP",
    "_NOPAUSESHFT",
    "ADD_BYNAMEPC",
    "ADD_STRINGPC",
    "APPEND_QUEUE",
    "ATT_WINDOW_D",
    "ATT_WINDOW_S",
    "CALL_PROGLIN",
    "CLR_PORT_SIM",
    "CNCL_STP_MTN",
    "CNC_DYN_DISB",
    "CNC_DYN_DISE",
    "CNC_DYN_DISI",
    "CNC_DYN_DISP",
    "CNC_DYN_DISR",
    "CNC_DYN_DISS",
    "CNV_CONF_STR",
    "CNV_JPOS_REL",
    "CNV_REAL_STR",
    "CNV_REL_JPOS",
    "CNV_STR_CONF",
    "CNV_STR_REAL",
    "CNV_STR_TIME",
    "CNV_TIME_STR",
    "COMMON_ASSOC",
    "DELETE_QUEUE",
    "DEL_INST_TPE",
    "DISCTRL_ALPH",
    "DISCTRL_FORM",
    "DISCTRL_LIST",
    "DISMOUNT_DEV",
    "ENDCONDITION",
    "ENDSTRUCTURE",
    "FORCE_SPMENU",
    "GET_ATTR_PRG",
    "GET_FILE_POS",
    "GET_JPOS_REG",
    "GET_JPOS_TPE",
    "GET_PORT_ASG",
    "GET_PORT_ATR",
    "GET_PORT_CMT",
    "GET_PORT_MOD",
    "GET_PORT_SIM",
    "GET_PORT_VAL",
    "GET_TSK_INFO",
    "INI_DYN_DISB",
    "INI_DYN_DISE",
    "INI_DYN_DISI",
    "INI_DYN_DISP",
    "INI_DYN_DISR",
    "INI_DYN_DISS",
    "INSERT_QUEUE",
    "MODIFY_QUEUE",
    "POS_REG_TYPE",
    "SEND_EVENTPC",
    "SET_ATTR_PRG",
    "SET_EPOS_REG",
    "SET_EPOS_TPE",
    "SET_FILE_ATR",
    "SET_FILE_POS",
    "SET_JPOS_REG",
    "SET_JPOS_TPE",
    "SET_PORT_ASG",
    "SET_PORT_ATR",
    "SET_PORT_CMT",
    "SET_PORT_MOD",
    "SET_PORT_SIM",
    "SET_PORT_VAL",
    "SET_REAL_REG",
    "SET_TSK_ATTR",
    "SET_TSK_NAME",
    "STD_PTH_NODE",
    "UNLOCK_GROUP",
    "WRITE_DICT_V"
};

const char *Keywords::name(int keyid)
{ return keywordNames[keyid]; }

} // Karel

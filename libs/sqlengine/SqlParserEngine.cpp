#include <QByteArray>
#include <QString>

#include "SqlParserEngine.h"
#include "SqlLexer.h"
#include "Parser.h"

namespace Sql {

static inline void sqlClassify2( const char *s, Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a') {
    if ( s[1] == 'S' || s[1] == 's') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_AS;
      return;
    }
    else if ( s[1] == 'T' || s[1] == 't') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_AT;
      return;
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b') {
    if ( s[1] == 'Y' || s[1] == 'y') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_BY;
      return;
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd') {
    if ( s[1] == 'O' || s[1] == 'o') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_DO;
      return;
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g') {
    if ( s[1] == 'O' || s[1] == 'o') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_GO;
      return;
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i') {
    if ( s[1] == 'F' || s[1] == 'f') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_IF;
      return;
    }
    else if ( s[1] == 'N' || s[1] == 'n') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_IN;
      return;
    }
    else if ( s[1] == 'S' || s[1] == 's') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_IS;
      return;
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n') {
    if ( s[1] == 'O' || s[1] == 'o') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_NO;
      return;
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o') {
    if ( s[1] == 'F' || s[1] == 'f') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_OF;
      return;
    }
    else if ( s[1] == 'N' || s[1] == 'n') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_ON;
      return;
    }
    else if ( s[1] == 'R' || s[1] == 'r') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_OR;
      return;
    }
  }
  else if ( s[0] == 'T' || s[0] == 't') {
    if ( s[1] == 'O' || s[1] == 'o') {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = T_2_TO;
      return;
    }
  }
}

static inline void sqlClassify3( const char *s, Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a') {
    if ( s[1] == 'D' || s[1] == 'd') {
      if ( s[2] == 'D' || s[2] == 'd') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_ADD;
        return;
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l') {
      if ( s[2] == 'L' || s[2] == 'l') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_ALL;
        return;
      }
    }
    else if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'D' || s[2] == 'd') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_AND;
        return;
      }
      else if ( s[2] == 'Y' || s[2] == 'y') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_ANY;
        return;
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'E' || s[2] == 'e') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_ARE;
        return;
      }
    }
    else if ( s[1] == 'S' || s[1] == 's') {
      if ( s[2] == 'C' || s[2] == 'c') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_ASC;
        return;
      }
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b') {
    if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'T' || s[2] == 't') {
        tk.kf.keytype = 3;
        tk.kf.keyword3 = 1;
        tk.kf.keyid = T_3_BIT;
        return;
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'Y' || s[2] == 'y') {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_3_DAY;
        return;
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'C' || s[2] == 'c') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_DEC;
        return;
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e') {
    if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'D' || s[2] == 'd') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_END;
        return;
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'R' || s[2] == 'r') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_FOR;
        return;
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'T' || s[2] == 't') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_GET;
        return;
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i') {
    if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'T' || s[2] == 't') {
        tk.kf.keytype = 3;
        tk.kf.keyword3 = 1;
        tk.kf.keyid = T_3_INT;
        return;
      }
    }
  }
  else if ( s[0] == 'K' || s[0] == 'k') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'Y' || s[2] == 'y') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_KEY;
        return;
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'P' || s[2] == 'p') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_MAP;
        return;
      }
      else if ( s[2] == 'X' || s[2] == 'x') {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_3_MAX;
        return;
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'N' || s[2] == 'n') {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_3_MIN;
        return;
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'W' || s[2] == 'w') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_NEW;
        return;
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'T' || s[2] == 't') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_NOT;
        return;
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o') {
    if ( s[1] == 'L' || s[1] == 'l') {
      if ( s[2] == 'D' || s[2] == 'd') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_OLD;
        return;
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'T' || s[2] == 't') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_OUT;
        return;
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'D' || s[2] == 'd') {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_3_PAD;
        return;
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'F' || s[2] == 'f') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_REF;
        return;
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'W' || s[2] == 'w') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_ROW;
        return;
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'T' || s[2] == 't') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_SET;
        return;
      }
    }
    else if ( s[1] == 'Q' || s[1] == 'q') {
      if ( s[2] == 'L' || s[2] == 'l') {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = T_3_SQL;
        return;
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'M' || s[2] == 'm') {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = T_3_SUM;
        return;
      }
    }
  }
}

static inline void sqlClassify4( const char *s, Token &tk )
{
  if ( s[0] == 'B' || s[0] == 'b') {
    if ( s[1] == 'L' || s[1] == 'l') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'B' || s[3] == 'b') {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = T_4_BLOB;
          return;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'H' || s[3] == 'h') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_BOTH;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'L' || s[3] == 'l') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_CALL;
          return;
        }
      }
      else if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_CASE;
          return;
        }
        else if ( s[3] == 'T' || s[3] == 't') {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_4_CAST;
          return;
        }
      }
    }
    else if ( s[1] == 'H' || s[1] == 'h') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'R' || s[3] == 'r') {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = T_4_CHAR;
          return;
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'B' || s[3] == 'b') {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = T_4_CLOB;
          return;
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'B' || s[2] == 'b') {
        if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_CUBE;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'A' || s[3] == 'a') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_DATA;
          return;
        }
        else if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = T_4_DATE;
          return;
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'C' || s[3] == 'c') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_DESC;
          return;
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'P' || s[3] == 'p') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_DROP;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'H' || s[3] == 'h') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_EACH;
          return;
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_ELSE;
          return;
        }
      }
    }
    else if ( s[1] == 'X' || s[1] == 'x') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'C' || s[3] == 'c') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_EXEC;
          return;
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'T' || s[3] == 't') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_EXIT;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f') {
    if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_FREE;
          return;
        }
      }
      else if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'M' || s[3] == 'm') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_FROM;
          return;
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'L' || s[3] == 'l') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_FULL;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'O' || s[3] == 'o') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_GOTO;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'H' || s[0] == 'h') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'D' || s[3] == 'd') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_HOLD;
          return;
        }
      }
      else if ( s[2] == 'U' || s[2] == 'u') {
        if ( s[3] == 'R' || s[3] == 'r') {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_4_HOUR;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i') {
    if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'O' || s[3] == 'o') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_INTO;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'J' || s[0] == 'j') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'N' || s[3] == 'n') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_JOIN;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'T' || s[3] == 't') {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_4_LAST;
          return;
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'F' || s[2] == 'f') {
        if ( s[3] == 'T' || s[3] == 't') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_LEFT;
          return;
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'K' || s[2] == 'k') {
        if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_LIKE;
          return;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'P' || s[3] == 'p') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_LOOP;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'X' || s[2] == 'x') {
        if ( s[3] == 'T' || s[3] == 't') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_NEXT;
          return;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_NONE;
          return;
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'L' || s[3] == 'l') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_NULL;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o') {
    if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'Y' || s[3] == 'y') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_ONLY;
          return;
        }
      }
    }
    else if ( s[1] == 'P' || s[1] == 'p') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'N' || s[3] == 'n') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_OPEN;
          return;
        }
      }
    }
    else if ( s[1] == 'V' || s[1] == 'v') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'R' || s[3] == 'r') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_OVER;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'H' || s[3] == 'h') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_PATH;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'D' || s[3] == 'd') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_READ;
          return;
        }
        else if ( s[3] == 'L' || s[3] == 'l') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_REAL;
          return;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_ROLE;
          return;
        }
      }
      else if ( s[2] == 'W' || s[2] == 'w') {
        if ( s[3] == 'S' || s[3] == 's') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_ROWS;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'S' || s[3] == 's') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_SETS;
          return;
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'Z' || s[2] == 'z') {
        if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = T_4_SIZE;
          return;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_SOME;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't') {
    if ( s[1] == 'H' || s[1] == 'h') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'N' || s[3] == 'n') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_THEN;
          return;
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_TIME;
          return;
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'U' || s[2] == 'u') {
        if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_TRUE;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'U' || s[0] == 'u') {
    if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'D' || s[2] == 'd') {
        if ( s[3] == 'O' || s[3] == 'o') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_UNDO;
          return;
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'R' || s[3] == 'r') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_USER;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'V' || s[0] == 'v') {
    if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'W' || s[3] == 'w') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_VIEW;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'W' || s[0] == 'w') {
    if ( s[1] == 'H' || s[1] == 'h') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'N' || s[3] == 'n') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_WHEN;
          return;
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'H' || s[3] == 'h') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_WITH;
          return;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'K' || s[3] == 'k') {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = T_4_WORK;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'Y' || s[0] == 'y') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'R' || s[3] == 'r') {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = T_4_YEAR;
          return;
        }
      }
    }
  }
  else if ( s[0] == 'Z' || s[0] == 'z') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'E' || s[3] == 'e') {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = T_4_ZONE;
          return;
        }
      }
    }
  }
}

static inline void sqlClassify5( const char *s, Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a') {
    if ( s[1] == 'F' || s[1] == 'f') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_AFTER;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_ALTER;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'Y' || s[4] == 'y') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_ARRAY;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'G' || s[2] == 'g') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'N' || s[4] == 'n') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_BEGIN;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c') {
    if ( s[1] == 'H' || s[1] == 'h') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'K' || s[4] == 'k') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_CHECK;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_CLOSE;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'U' || s[2] == 'u') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'T' || s[4] == 't') {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_5_COUNT;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'S' || s[4] == 's') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_CROSS;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'Y' || s[1] == 'y') {
      if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_CYCLE;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'P' || s[2] == 'p') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'H' || s[4] == 'h') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_DEPTH;
            return;
          }
        }
      }
      else if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'F' || s[4] == 'f') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_DEREF;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_FALSE;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'H' || s[4] == 'h') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_FETCH;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'T' || s[4] == 't') {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_5_FIRST;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'T' || s[4] == 't') {
            tk.kf.keytype = 3;
            tk.kf.keyword3 = 1;
            tk.kf.keyid = T_5_FLOAT;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'U' || s[2] == 'u') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'D' || s[4] == 'd') {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_5_FOUND;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g') {
    if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'T' || s[4] == 't') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_GRANT;
            return;
          }
        }
      }
      else if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'P' || s[4] == 'p') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_GROUP;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i') {
    if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'D' || s[2] == 'd') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'X' || s[4] == 'x') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_INDEX;
            return;
          }
        }
      }
      else if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_INNER;
            return;
          }
        }
      }
      else if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'T' || s[4] == 't') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_INOUT;
            return;
          }
        }
      }
      else if ( s[2] == 'P' || s[2] == 'p') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'T' || s[4] == 't') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_INPUT;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'G' || s[3] == 'g') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_LARGE;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'V' || s[3] == 'v') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_LEAVE;
            return;
          }
        }
      }
      else if ( s[2] == 'V' || s[2] == 'v') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'L' || s[4] == 'l') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_LEVEL;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'T' || s[4] == 't') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_LIMIT;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'L' || s[4] == 'l') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_LOCAL;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'H' || s[4] == 'h') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_MATCH;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'S' || s[4] == 's') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_MINUS;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'H' || s[4] == 'h') {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = T_5_MONTH;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'S' || s[4] == 's') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_NAMES;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'C' || s[1] == 'c') {
      if ( s[2] == 'H' || s[2] == 'h') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'R' || s[4] == 'r') {
            tk.kf.keytype = 3;
            tk.kf.keyword3 = 1;
            tk.kf.keyid = T_5_NCHAR;
            return;
          }
        }
      }
      else if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'O' || s[3] == 'o') {
          if ( s[4] == 'B' || s[4] == 'b') {
            tk.kf.keytype = 3;
            tk.kf.keyword3 = 1;
            tk.kf.keyid = T_5_NCLOB;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o') {
    if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'D' || s[2] == 'd') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_ORDER;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_OUTER;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p') {
    if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'O' || s[3] == 'o') {
          if ( s[4] == 'R' || s[4] == 'r') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_PRIOR;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'G' || s[3] == 'g') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_RANGE;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'D' || s[3] == 'd') {
          if ( s[4] == 'S' || s[4] == 's') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_READS;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'G' || s[2] == 'g') {
        if ( s[3] == 'H' || s[3] == 'h') {
          if ( s[4] == 'T' || s[4] == 't') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_RIGHT;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's') {
    if ( s[1] == 'C' || s[1] == 'c') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'P' || s[3] == 'p') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_SCOPE;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'P' || s[1] == 'p') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_SPACE;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'R' || s[3] == 'r') {
          if ( s[4] == 'T' || s[4] == 't') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_START;
            return;
          }
        }
        else if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_STATE;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'B' || s[2] == 'b') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_TABLE;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'T' || s[4] == 't') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_TREAT;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'U' || s[0] == 'u') {
    if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'D' || s[2] == 'd') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_UNDER;
            return;
          }
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'O' || s[3] == 'o') {
          if ( s[4] == 'N' || s[4] == 'n') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_UNION;
            return;
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'L' || s[4] == 'l') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_UNTIL;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'G' || s[3] == 'g') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_USAGE;
            return;
          }
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'G' || s[4] == 'g') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_USING;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'V' || s[0] == 'v') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_VALUE;
            return;
          }
        }
      }
    }
  }
  else if ( s[0] == 'W' || s[0] == 'w') {
    if ( s[1] == 'H' || s[1] == 'h') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'R' || s[3] == 'r') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_WHERE;
            return;
          }
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_WHILE;
            return;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'E' || s[4] == 'e') {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = T_5_WRITE;
            return;
          }
        }
      }
    }
  }
}

static inline void sqlClassify6( const char *s, Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a') {
    if ( s[1] == 'C' || s[1] == 'c') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'O' || s[4] == 'o') {
            if ( s[5] == 'N' || s[5] == 'n') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_ACTION;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'M' || s[3] == 'm') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'C' || s[5] == 'c') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_ATOMIC;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'F' || s[2] == 'f') {
        if ( s[3] == 'O' || s[3] == 'o') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'E' || s[5] == 'e') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_BEFORE;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'Y' || s[5] == 'y') {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = T_6_BINARY;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'M' || s[4] == 'm') {
            if ( s[5] == 'N' || s[5] == 'n') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_COLUMN;
              return;
            }
          }
        }
      }
      else if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'M' || s[3] == 'm') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'T' || s[5] == 't') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_COMMIT;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'E' || s[5] == 'e') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_CREATE;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'O' || s[4] == 'o') {
            if ( s[5] == 'R' || s[5] == 'r') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_CURSOR;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'E' || s[5] == 'e') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_DELETE;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'N' || s[5] == 'n') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_DOMAIN;
              return;
            }
          }
        }
      }
      else if ( s[2] == 'U' || s[2] == 'u') {
        if ( s[3] == 'B' || s[3] == 'b') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'E' || s[5] == 'e') {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = T_6_DOUBLE;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e') {
    if ( s[1] == 'L' || s[1] == 'l') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'F' || s[5] == 'f') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_ELSEIF;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'Q' || s[1] == 'q') {
      if ( s[2] == 'U' || s[2] == 'u') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'S' || s[5] == 's') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_EQUALS;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's') {
      if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'P' || s[4] == 'p') {
            if ( s[5] == 'E' || s[5] == 'e') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_ESCAPE;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'X' || s[1] == 'x') {
      if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'P' || s[4] == 'p') {
            if ( s[5] == 'T' || s[5] == 't') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_EXCEPT;
              return;
            }
          }
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'S' || s[5] == 's') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_EXISTS;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f') {
    if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'R' || s[5] == 'r') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_FILTER;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g') {
    if ( s[1] == 'L' || s[1] == 'l') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'B' || s[3] == 'b') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'L' || s[5] == 'l') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_GLOBAL;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'H' || s[0] == 'h') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'V' || s[2] == 'v') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'N' || s[4] == 'n') {
            if ( s[5] == 'G' || s[5] == 'g') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_HAVING;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i') {
    if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'T' || s[5] == 't') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_INSERT;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'H' || s[3] == 'h') {
          if ( s[4] == 'O' || s[4] == 'o') {
            if ( s[5] == 'D' || s[5] == 'd') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_METHOD;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'E' || s[5] == 'e') {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_6_MINUTE;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'D' || s[2] == 'd') {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'F' || s[4] == 'f' ) {
            if ( s[5] == 'Y' || s[5] == 'y' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_MODIFY;
              return;
            }
          }
        }
        else if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'E' || s[5] == 'e') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_MODULE;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o') {
    if ( s[1] == 'B' || s[1] == 'b') {
      if ( s[2] == 'J' || s[2] == 'j') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'C' || s[4] == 'c') {
            if ( s[5] == 'T' || s[5] == 't') {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = T_6_OBJECT;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'P' || s[1] == 'p') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'O' || s[4] == 'o') {
            if ( s[5] == 'N' || s[5] == 'n') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_OPTION;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'P' || s[3] == 'p') {
          if ( s[4] == 'U' || s[4] == 'u') {
            if ( s[5] == 'T' || s[5] == 't') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_OUTPUT;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p') {
    if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'B' || s[2] == 'b') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'C' || s[5] == 'c') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_PUBLIC;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'P' || s[2] == 'p') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'T' || s[5] == 't') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_REPEAT;
              return;
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'T' || s[5] == 't') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_RESULT;
              return;
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'N' || s[5] == 'n') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_RETURN;
              return;
            }
          }
        }
      }
      else if ( s[2] == 'V' || s[2] == 'v') {
        if ( s[3] == 'O' || s[3] == 'o') {
          if ( s[4] == 'K' || s[4] == 'k') {
            if ( s[5] == 'E' || s[5] == 'e') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_REVOKE;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'U' || s[4] == 'u') {
            if ( s[5] == 'P' || s[5] == 'p') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_ROLLUP;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's') {
    if ( s[1] == 'C' || s[1] == 'c') {
      if ( s[2] == 'H' || s[2] == 'h') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'M' || s[4] == 'm') {
            if ( s[5] == 'A' || s[5] == 'a') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_SCHEMA;
              return;
            }
          }
        }
      }
      else if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'O' || s[3] == 'o') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'L' || s[5] == 'l') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_SCROLL;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'R' || s[3] == 'r') {
          if ( s[4] == 'C' || s[4] == 'c') {
            if ( s[5] == 'H' || s[5] == 'h') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_SEARCH;
              return;
            }
          }
        }
      }
      else if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'O' || s[3] == 'o') {
          if ( s[4] == 'N' || s[4] == 'n') {
            if ( s[5] == 'D' || s[5] == 'd') {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = T_6_SECOND;
              return;
            }
          }
        }
      }
      else if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'C' || s[4] == 'c') {
            if ( s[5] == 'T' || s[5] == 't') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_SELECT;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'G' || s[2] == 'g') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'L' || s[5] == 'l') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_SIGNAL;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'C' || s[5] == 'c') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_STATIC;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'Y' || s[1] == 'y') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'M' || s[5] == 'm') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_SYSTEM;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'U' || s[0] == 'u') {
    if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'Q' || s[3] == 'q') {
          if ( s[4] == 'U' || s[4] == 'u') {
            if ( s[5] == 'E' || s[5] == 'e') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_UNIQUE;
              return;
            }
          }
        }
      }
      else if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'S' || s[4] == 's') {
            if ( s[5] == 'T' || s[5] == 't') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_UNNEST;
              return;
            }
          }
        }
      }
    }
    else if ( s[1] == 'P' || s[1] == 'p') {
      if ( s[2] == 'D' || s[2] == 'd') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'E' || s[5] == 'e') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_UPDATE;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'V' || s[0] == 'v') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'S' || s[5] == 's') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_VALUES;
              return;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'W' || s[0] == 'w') {
    if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'D' || s[3] == 'd') {
          if ( s[4] == 'O' || s[4] == 'o') {
            if ( s[5] == 'W' || s[5] == 'w') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_WINDOW;
              return;
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'H' || s[3] == 'h') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'N' || s[5] == 'n') {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = T_6_WITHIN;
              return;
            }
          }
        }
      }
    }
  }
}

static inline void sqlClassify7( const char *s, Token &tk )
{
  if ( s[0] == 'B' || s[0] == 'b') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'W' || s[3] == 'w') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'E' || s[5] == 'e') {
              if ( s[6] == 'N' || s[6] == 'n') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_BETWEEN;
                return;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'N' || s[6] == 'n') {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = T_7_BOOLEAN;
                return;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'D' || s[4] == 'd') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'H' || s[6] == 'h') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_BREADTH;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'D' || s[5] == 'd') {
              if ( s[6] == 'E' || s[6] == 'e') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_CASCADE;
                return;
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'O' || s[5] == 'o') {
              if ( s[6] == 'G' || s[6] == 'g') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_CATALOG;
                return;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'E' || s[6] == 'e') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_COLLATE;
                return;
              }
            }
          }
        }
      }
      else if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'M' || s[3] == 'm') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'T' || s[6] == 't') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_COMMENT;
                return;
              }
            }
          }
        }
      }
      else if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'C' || s[5] == 'c') {
              if ( s[6] == 'T' || s[6] == 't') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_CONNECT;
                return;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'R' || s[3] == 'r') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'T' || s[6] == 't') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_CURRENT;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'M' || s[4] == 'm') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'L' || s[6] == 'l') {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = T_7_DECIMAL;
                return;
              }
            }
          }
        }
        else if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'R' || s[5] == 'r') {
              if ( s[6] == 'E' || s[6] == 'e') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_DECLARE;
                return;
              }
            }
          }
        }
      }
      else if ( s[2] == 'F' || s[2] == 'f') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'U' || s[4] == 'u') {
            if ( s[5] == 'L' || s[5] == 'l') {
              if ( s[6] == 'T' || s[6] == 't') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_DEFAULT;
                return;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'Y' || s[1] == 'y') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'M' || s[4] == 'm') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'C' || s[6] == 'c') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_DYNAMIC;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e') {
    if ( s[1] == 'X' || s[1] == 'x') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'U' || s[4] == 'u') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'E' || s[6] == 'e') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_EXECUTE;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'G' || s[5] == 'g') {
              if ( s[6] == 'N' || s[6] == 'n') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_FOREIGN;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'L' || s[6] == 'l') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_GENERAL;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'H' || s[0] == 'h') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'D' || s[3] == 'd') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'E' || s[5] == 'e') {
              if ( s[6] == 'R' || s[6] == 'r') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_HANDLER;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i') {
    if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'G' || s[4] == 'g') {
            if ( s[5] == 'E' || s[5] == 'e') {
              if ( s[6] == 'R' || s[6] == 'r') {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = T_7_INTEGER;
                return;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'R' || s[3] == 'r') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'E' || s[6] == 'e') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_ITERATE;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'L' || s[6] == 'l') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_LATERAL;
                return;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'D' || s[3] == 'd') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'G' || s[6] == 'g') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_LEADING;
                return;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'O' || s[5] == 'o') {
              if ( s[6] == 'R' || s[6] == 'r') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_LOCATOR;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'L' || s[6] == 'l') {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = T_7_NATURAL;
                return;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'C' || s[6] == 'c') {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = T_7_NUMERIC;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'L' || s[6] == 'l') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_PARTIAL;
                return;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'P' || s[3] == 'p') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'R' || s[5] == 'r') {
              if ( s[6] == 'E' || s[6] == 'e') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_PREPARE;
                return;
              }
            }
          }
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'M' || s[3] == 'm') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'R' || s[5] == 'r') {
              if ( s[6] == 'Y' || s[6] == 'y') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_PRIMARY;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'S' || s[5] == 's') {
              if ( s[6] == 'E' || s[6] == 'e') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_RELEASE;
                return;
              }
            }
          }
        }
      }
      else if ( s[2] == 'P' || s[2] == 'p') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'C' || s[5] == 'c') {
              if ( s[6] == 'E' || s[6] == 'e') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_REPLACE;
                return;
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'S' || s[6] == 's') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_RETURNS;
                return;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'U' || s[2] == 'u') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'E' || s[6] == 'e') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_ROUTINE;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'O' || s[5] == 'o') {
              if ( s[6] == 'N' || s[6] == 'n') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_SECTION;
                return;
              }
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'O' || s[5] == 'o') {
              if ( s[6] == 'N' || s[6] == 'n') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_SESSION;
                return;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'R' || s[6] == 'r') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_SIMILAR;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't') {
    if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'G' || s[3] == 'g') {
          if ( s[4] == 'G' || s[4] == 'g') {
            if ( s[5] == 'E' || s[5] == 'e') {
              if ( s[6] == 'R' || s[6] == 'r') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_TRIGGER;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'U' || s[0] == 'u') {
    if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'K' || s[2] == 'k') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'O' || s[4] == 'o') {
            if ( s[5] == 'W' || s[5] == 'w') {
              if ( s[6] == 'N' || s[6] == 'n') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_UNKNOWN;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'V' || s[0] == 'v') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'H' || s[4] == 'h') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'R' || s[6] == 'r') {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = T_7_VARCHAR;
                return;
              }
            }
          }
        }
        else if ( s[3] == 'Y' || s[3] == 'y') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'G' || s[6] == 'g') {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = T_7_VARYING;
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'W' || s[0] == 'w') {
    if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'H' || s[3] == 'h') {
          if ( s[4] == 'O' || s[4] == 'o') {
            if ( s[5] == 'U' || s[5] == 'u') {
              if ( s[6] == 'T' || s[6] == 't') {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = T_7_WITHOUT;
                return;
              }
            }
          }
        }
      }
    }
  }
}

static inline void sqlClassify8( const char *s, Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a') {
    if ( s[1] == 'B' || s[1] == 'b') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'O' || s[3] == 'o') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'U' || s[5] == 'u') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_ABSOLUTE;
                  return;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'O' || s[3] == 'o') {
          if ( s[4] == 'C' || s[4] == 'c') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_ALLOCATE;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'D' || s[5] == 'd') {
              if ( s[6] == 'E' || s[6] == 'e') {
                if ( s[7] == 'D' || s[7] == 'd') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_CASCADED;
                  return;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'U' || s[6] == 'u') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_CONTINUE;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'F' || s[2] == 'f') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'R' || s[5] == 'r') {
              if ( s[6] == 'E' || s[6] == 'e') {
                if ( s[7] == 'D' || s[7] == 'd') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_DEFERRED;
                  return;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'B' || s[6] == 'b') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_DESCRIBE;
                  return;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'C' || s[6] == 'c') {
                if ( s[7] == 'T' || s[7] == 't') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_DISTINCT;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e') {
    if ( s[1] == 'X' || s[1] == 'x') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'A' || s[6] == 'a') {
                if ( s[7] == 'L' || s[7] == 'l') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_EXTERNAL;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f') {
    if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'O' || s[6] == 'o') {
                if ( s[7] == 'N' || s[7] == 'n') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_FUNCTION;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g') {
    if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'P' || s[4] == 'p') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'N' || s[6] == 'n') {
                if ( s[7] == 'G' || s[7] == 'g') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_GROUPING;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i') {
    if ( s[1] == 'D' || s[1] == 'd') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == 'Y' || s[7] == 'y') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_IDENTITY;
                  return;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'V' || s[5] == 'v') {
              if ( s[6] == 'A' || s[6] == 'a') {
                if ( s[7] == 'L' || s[7] == 'l') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_INTERVAL;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'G' || s[3] == 'g') {
          if ( s[4] == 'U' || s[4] == 'u') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'G' || s[6] == 'g') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_LANGUAGE;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'D' || s[2] == 'd') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'F' || s[4] == 'f') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'E' || s[6] == 'e') {
                if ( s[7] == 'S' || s[7] == 's') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_MODIFIES;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'O' || s[4] == 'o') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'A' || s[6] == 'a') {
                if ( s[7] == 'L' || s[7] == 'l') {
                  tk.kf.keytype = 3;
                  tk.kf.keyword3 = 1;
                  tk.kf.keyid = T_8_NATIONAL;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o') {
    if ( s[1] == 'V' || s[1] == 'v') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'R' || s[3] == 'r') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'P' || s[6] == 'p') {
                if ( s[7] == 'S' || s[7] == 's') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_OVERLAPS;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p') {
    if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'R' || s[5] == 'r') {
              if ( s[6] == 'V' || s[6] == 'v') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_PRESERVE;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'V' || s[6] == 'v') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_RELATIVE;
                  return;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'G' || s[4] == 'g') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'A' || s[6] == 'a') {
                if ( s[7] == 'L' || s[7] == 'l') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_RESIGNAL;
                  return;
                }
              }
            }
          }
        }
        else if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'C' || s[6] == 'c') {
                if ( s[7] == 'T' || s[7] == 't') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_RESTRICT;
                  return;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'B' || s[4] == 'b') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'C' || s[6] == 'c') {
                if ( s[7] == 'K' || s[7] == 'k') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_ROLLBACK;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's') {
    if ( s[1] == 'M' || s[1] == 'm') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'N' || s[6] == 'n') {
                if ( s[7] == 'T' || s[7] == 't') {
                  tk.kf.keytype = 3;
                  tk.kf.keyword3 = 1;
                  tk.kf.keyid = T_8_SMALLINT;
                  return;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'P' || s[1] == 'p') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'F' || s[5] == 'f') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'C' || s[7] == 'c') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_SPECIFIC;
                  return;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'Q' || s[1] == 'q') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_SQLSTATE;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't') {
    if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'N' || s[6] == 'n') {
                if ( s[7] == 'G' || s[7] == 'g') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_TRAILING;
                  return;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'U' || s[2] == 'u') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'C' || s[4] == 'c') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_TRUNCATE;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'W' || s[0] == 'w') {
    if ( s[1] == 'H' || s[1] == 'h') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'V' || s[5] == 'v') {
              if ( s[6] == 'E' || s[6] == 'e') {
                if ( s[7] == 'R' || s[7] == 'r') {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = T_8_WHENEVER;
                  return;
                }
              }
            }
          }
        }
      }
    }
  }
}

static inline void sqlClassify9( const char *s, Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a') {
    if ( s[1] == 'S' || s[1] == 's') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'O' || s[7] == 'o') {
                  if ( s[8] == 'N' || s[8] == 'n') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_ASSERTION;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c') {
    if ( s[1] == 'H' || s[1] == 'h') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'R' || s[3] == 'r') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'C' || s[5] == 'c') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  if ( s[8] == 'R' || s[8] == 'r') {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = T_9_CHARACTER;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'O' || s[7] == 'o') {
                  if ( s[8] == 'N' || s[8] == 'n') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_COLLATION;
                    return;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'D' || s[3] == 'd') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'O' || s[7] == 'o') {
                  if ( s[8] == 'N' || s[8] == 'n') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_CONDITION;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e') {
    if ( s[1] == 'X' || s[1] == 'x') {
      if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'P' || s[4] == 'p') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'O' || s[7] == 'o') {
                  if ( s[8] == 'N' || s[8] == 'n') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_EXCEPTION;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i') {
    if ( s[1] == 'M' || s[1] == 'm') {
      if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'D' || s[4] == 'd') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'A' || s[6] == 'a') {
                if ( s[7] == 'T' || s[7] == 't') {
                  if ( s[8] == 'E' || s[8] == 'e') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_IMMEDIATE;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'D' || s[2] == 'd') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'C' || s[4] == 'c') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == 'O' || s[7] == 'o') {
                  if ( s[8] == 'R' || s[8] == 'r') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_INDICATOR;
                    return;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'L' || s[6] == 'l') {
                if ( s[7] == 'L' || s[7] == 'l') {
                  if ( s[8] == 'Y' || s[8] == 'y') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_INITIALLY;
                    return;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'S' || s[5] == 's') {
              if ( s[6] == 'E' || s[6] == 'e') {
                if ( s[7] == 'C' || s[7] == 'c') {
                  if ( s[8] == 'T' || s[8] == 't') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_INTERSECT;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's') {
      if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'O' || s[7] == 'o') {
                  if ( s[8] == 'N' || s[8] == 'n') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_ISOLATION;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'M' || s[7] == 'm') {
                  if ( s[8] == 'E' || s[8] == 'e') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_LOCALTIME;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'M' || s[4] == 'm') {
            if ( s[5] == 'E' || s[5] == 'e') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  if ( s[8] == 'R' || s[8] == 'r') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_PARAMETER;
                    return;
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'O' || s[7] == 'o') {
                  if ( s[8] == 'N' || s[8] == 'n') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_PARTITION;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'S' || s[5] == 's') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'O' || s[7] == 'o') {
                  if ( s[8] == 'N' || s[8] == 'n') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_PRECISION;
                    return;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'O' || s[2] == 'o') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'D' || s[5] == 'd') {
              if ( s[6] == 'U' || s[6] == 'u') {
                if ( s[7] == 'R' || s[7] == 'r') {
                  if ( s[8] == 'E' || s[8] == 'e') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_PROCEDURE;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'U' || s[3] == 'u') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'S' || s[5] == 's') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'V' || s[7] == 'v') {
                  if ( s[8] == 'E' || s[8] == 'e') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_RECURSIVE;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's') {
    if ( s[1] == 'A' || s[1] == 'a') {
      if ( s[2] == 'V' || s[2] == 'v') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'P' || s[4] == 'p') {
            if ( s[5] == 'O' || s[5] == 'o') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'N' || s[7] == 'n') {
                  if ( s[8] == 'T' || s[8] == 't') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_SAVEPOINT;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'V' || s[7] == 'v') {
                  if ( s[8] == 'E' || s[8] == 'e') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_SENSITIVE;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'V' || s[7] == 'v') {
                  if ( s[8] == 'E' || s[8] == 'e') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_SENSITIVE;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'M' || s[5] == 'm') {
              if ( s[6] == 'E' || s[6] == 'e') {
                if ( s[7] == 'N' || s[7] == 'n') {
                  if ( s[8] == 'T' || s[8] == 't') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_STATEMENT;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'B' || s[2] == 'b') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'R' || s[5] == 'r') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'N' || s[7] == 'n') {
                  if ( s[8] == 'G' || s[8] == 'g') {
                    tk.kf.keytype = 2;
                    tk.kf.keyword2 = 1;
                    tk.kf.keyid = T_9_SUBSTRING;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'P' || s[3] == 'p') {
          if ( s[4] == 'O' || s[4] == 'o') {
            if ( s[5] == 'R' || s[5] == 'r') {
              if ( s[6] == 'A' || s[6] == 'a') {
                if ( s[7] == 'R' || s[7] == 'r') {
                  if ( s[8] == 'Y' || s[8] == 'y') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_TEMPORARY;
                    return;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'S' || s[4] == 's') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'A' || s[6] == 'a') {
                if ( s[7] == 'M' || s[7] == 'm') {
                  if ( s[8] == 'P' || s[8] == 'p') {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = T_9_TIMESTAMP;
                    return;
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

static inline void sqlClassify10( const char *s, Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a') {
    if ( s[1] == 'S' || s[1] == 's') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'S' || s[4] == 's') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == 'I' || s[7] == 'i') {
                  if ( s[8] == 'V' || s[8] == 'v') {
                    if ( s[9] == 'E' || s[9] == 'e') {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_10_ASENSITIVE;
                      return;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'Y' || s[2] == 'y') {
        if ( s[3] == 'M' || s[3] == 'm') {
          if ( s[4] == 'M' || s[4] == 'm') {
            if ( s[5] == 'E' || s[5] == 'e') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == 'R' || s[7] == 'r') {
                  if ( s[8] == 'I' || s[8] == 'i') {
                    if ( s[9] == 'C' || s[9] == 'c') {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_10_ASYMMETRIC;
                      return;
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
  else if ( s[0] == 'C' || s[0] == 'c') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'C' || s[5] == 'c') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == 'I' || s[7] == 'i') {
                  if ( s[8] == 'O' || s[8] == 'o') {
                    if ( s[9] == 'N' || s[9] == 'n') {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_10_CONNECTION;
                      return;
                    }
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'R' || s[5] == 'r') {
              if ( s[6] == 'A' || s[6] == 'a') {
                if ( s[7] == 'I' || s[7] == 'i') {
                  if ( s[8] == 'N' || s[8] == 'n') {
                    if ( s[9] == 'T' || s[9] == 't') {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_10_CONSTRAINT;
                      return;
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
  else if ( s[0] == 'D' || s[0] == 'd') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'L' || s[3] == 'l') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'O' || s[5] == 'o') {
              if ( s[6] == 'C' || s[6] == 'c') {
                if ( s[7] == 'A' || s[7] == 'a') {
                  if ( s[8] == 'T' || s[8] == 't') {
                    if ( s[9] == 'E' || s[9] == 'e') {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_10_DEALLOCATE;
                      return;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'F' || s[2] == 'f') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'R' || s[5] == 'r') {
              if ( s[6] == 'A' || s[6] == 'a') {
                if ( s[7] == 'B' || s[7] == 'b') {
                  if ( s[8] == 'L' || s[8] == 'l') {
                    if ( s[9] == 'E' || s[9] == 'e') {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_10_DEFERRABLE;
                      return;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'I' || s[5] == 'i') {
              if ( s[6] == 'P' || s[6] == 'p') {
                if ( s[7] == 'T' || s[7] == 't') {
                  if ( s[8] == 'O' || s[8] == 'o') {
                    if ( s[9] == 'R' || s[9] == 'r') {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_10_DESCRIPTOR;
                      return;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'O' || s[4] == 'o') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'N' || s[6] == 'n') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  if ( s[8] == 'C' || s[8] == 'c') {
                    if ( s[9] == 'T' || s[9] == 't') {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_10_DISCONNECT;
                      return;
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
  else if ( s[0] == 'O' || s[0] == 'o') {
    if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'D' || s[2] == 'd') {
        if ( s[3] == 'I' || s[3] == 'i') {
          if ( s[4] == 'N' || s[4] == 'n') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'L' || s[6] == 'l') {
                if ( s[7] == 'I' || s[7] == 'i') {
                  if ( s[8] == 'T' || s[8] == 't') {
                    if ( s[9] == 'Y' || s[9] == 'y') {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_10_ORDINALITY;
                      return;
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
  else if ( s[0] == 'P' || s[0] == 'p') {
    if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'I' || s[2] == 'i') {
        if ( s[3] == 'V' || s[3] == 'v') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'L' || s[5] == 'l') {
              if ( s[6] == 'E' || s[6] == 'e') {
                if ( s[7] == 'G' || s[7] == 'g') {
                  if ( s[8] == 'E' || s[8] == 'e') {
                    if ( s[9] == 'S' || s[9] == 's') {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_10_PRIVILEGES;
                      return;
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
  else if ( s[0] == 'R' || s[0] == 'r') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'F' || s[2] == 'f') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'E' || s[5] == 'e') {
              if ( s[6] == 'N' || s[6] == 'n') {
                if ( s[7] == 'C' || s[7] == 'c') {
                  if ( s[8] == 'E' || s[8] == 'e') {
                    if ( s[9] == 'S' || s[9] == 's') {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_10_REFERENCES;
                      return;
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
  else if ( s[0] == 'S' || s[0] == 's') {
    if ( s[1] == 'Q' || s[1] == 'q') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'W' || s[3] == 'w') {
          if ( s[4] == 'A' || s[4] == 'a') {
            if ( s[5] == 'R' || s[5] == 'r') {
              if ( s[6] == 'N' || s[6] == 'n') {
                if ( s[7] == 'I' || s[7] == 'i') {
                  if ( s[8] == 'N' || s[8] == 'n') {
                    if ( s[9] == 'G' || s[9] == 'g') {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = T_10_SQLWARNING;
                      return;
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

static inline void sqlClassify11( const char *s, Token &tk )
{
  if ( s[0] == 'C' || s[0] == 'c') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'N' || s[2] == 'n') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'T' || s[4] == 't') {
            if ( s[5] == 'R' || s[5] == 'r') {
              if ( s[6] == 'A' || s[6] == 'a') {
                if ( s[7] == 'I' || s[7] == 'i') {
                  if ( s[8] == 'N' || s[8] == 'n') {
                    if ( s[9] == 'T' || s[9] == 't') {
                      if ( s[10] == 'S' || s[10] == 's') {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_11_CONSTRAINTS;
                        return;
                      }
                    }
                  }
                }
              }
              else if ( s[6] == 'U' || s[6] == 'u') {
                if ( s[7] == 'C' || s[7] == 'c') {
                  if ( s[8] == 'T' || s[8] == 't') {
                    if ( s[9] == 'O' || s[9] == 'o') {
                      if ( s[10] == 'R' || s[10] == 'r') {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_11_CONSTRUCTOR;
                        return;
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
  else if ( s[0] == 'D' || s[0] == 'd') {
    if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'G' || s[3] == 'g') {
          if ( s[4] == 'N' || s[4] == 'n') {
            if ( s[5] == 'O' || s[5] == 'o') {
              if ( s[6] == 'S' || s[6] == 's') {
                if ( s[7] == 'T' || s[7] == 't') {
                  if ( s[8] == 'I' || s[8] == 'i') {
                    if ( s[9] == 'C' || s[9] == 'c') {
                      if ( s[10] == 'S' || s[10] == 's') {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_11_DIAGNOSTICS;
                        return;
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
  else if ( s[0] == 'I' || s[0] == 'i') {
    if ( s[1] == 'N' || s[1] == 'n') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'N' || s[4] == 'n') {
            if ( s[5] == 'S' || s[5] == 's') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'T' || s[7] == 't') {
                  if ( s[8] == 'I' || s[8] == 'i') {
                    if ( s[9] == 'V' || s[9] == 'v') {
                      if ( s[10] == 'E' || s[10] == 'e') {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_11_INSENSITIVE;
                        return;
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
  else if ( s[0] == 'R' || s[0] == 'r') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'F' || s[2] == 'f') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'E' || s[5] == 'e') {
              if ( s[6] == 'N' || s[6] == 'n') {
                if ( s[7] == 'C' || s[7] == 'c') {
                  if ( s[8] == 'I' || s[8] == 'i') {
                    if ( s[9] == 'N' || s[9] == 'n') {
                      if ( s[10] == 'G' || s[10] == 'g') {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_11_REFERENCING;
                        return;
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
  else if ( s[0] == 'S' || s[0] == 's') {
    if ( s[1] == 'Y' || s[1] == 'y') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'T' || s[3] == 't') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'M' || s[5] == 'm') {
              if ( s[6] == '_' || s[6] == '_') {
                if ( s[7] == 'U' || s[7] == 'u') {
                  if ( s[8] == 'S' || s[8] == 's') {
                    if ( s[9] == 'E' || s[9] == 'e') {
                      if ( s[10] == 'R' || s[10] == 'r') {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_11_SYSTEM_USER;
                        return;
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
  else if ( s[0] == 'T' || s[0] == 't') {
    if ( s[1] == 'R' || s[1] == 'r') {
      if ( s[2] == 'A' || s[2] == 'a') {
        if ( s[3] == 'N' || s[3] == 'n') {
          if ( s[4] == 'S' || s[4] == 's') {
            if ( s[5] == 'A' || s[5] == 'a') {
              if ( s[6] == 'C' || s[6] == 'c') {
                if ( s[7] == 'T' || s[7] == 't') {
                  if ( s[8] == 'I' || s[8] == 'i') {
                    if ( s[9] == 'O' || s[9] == 'o') {
                      if ( s[10] == 'N' || s[10] == 'n') {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_11_TRANSACTION;
                        return;
                      }
                    }
                  }
                }
              }
            }
            else if ( s[5] == 'L' || s[5] == 'l') {
              if ( s[6] == 'A' || s[6] == 'a') {
                if ( s[7] == 'T' || s[7] == 't') {
                  if ( s[8] == 'I' || s[8] == 'i') {
                    if ( s[9] == 'O' || s[9] == 'o') {
                      if ( s[10] == 'N' || s[10] == 'n') {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = T_11_TRANSLATION;
                        return;
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

static inline void sqlClassify12( const char *s, Token &tk )
{
  if ( s[0] == 'C' || s[0] == 'c') {
    if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'R' || s[3] == 'r') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == '_' || s[7] == '_') {
                  if ( s[8] == 'D' || s[8] == 'd') {
                    if ( s[9] == 'A' || s[9] == 'a') {
                      if ( s[10] == 'T' || s[10] == 't') {
                        if ( s[11] == 'E' || s[11] == 'e') {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_12_CURRENT_DATE;
                          return;
                        }
                      }
                    }
                  }
                  else if ( s[8] == 'P' || s[8] == 'p') {
                    if ( s[9] == 'A' || s[9] == 'a') {
                      if ( s[10] == 'T' || s[10] == 't') {
                        if ( s[11] == 'H' || s[11] == 'h') {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_12_CURRENT_PATH;
                          return;
                        }
                      }
                    }
                  }
                  else if ( s[8] == 'R' || s[8] == 'r') {
                    if ( s[9] == 'O' || s[9] == 'o') {
                      if ( s[10] == 'L' || s[10] == 'l') {
                        if ( s[11] == 'E' || s[11] == 'e') {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_12_CURRENT_ROLE;
                          return;
                        }
                      }
                    }
                  }
                  else if ( s[8] == 'T' || s[8] == 't') {
                    if ( s[9] == 'I' || s[9] == 'i') {
                      if ( s[10] == 'M' || s[10] == 'm') {
                        if ( s[11] == 'E' || s[11] == 'e') {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_12_CURRENT_TIME;
                          return;
                        }
                      }
                    }
                  }
                  else if ( s[8] == 'U' || s[8] == 'u') {
                    if ( s[9] == 'S' || s[9] == 's') {
                      if ( s[10] == 'E' || s[10] == 'e') {
                        if ( s[11] == 'R' || s[11] == 'r') {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_12_CURRENT_USER;
                          return;
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
  else if ( s[0] == 'S' || s[0] == 's') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'S' || s[2] == 's') {
        if ( s[3] == 'S' || s[3] == 's') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'O' || s[5] == 'o') {
              if ( s[6] == 'N' || s[6] == 'n') {
                if ( s[7] == '_' || s[7] == '_') {
                  if ( s[8] == 'U' || s[8] == 'u') {
                    if ( s[9] == 'S' || s[9] == 's') {
                      if ( s[10] == 'E' || s[10] == 'e') {
                        if ( s[11] == 'R' || s[11] == 'r') {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_12_SESSION_USER;
                          return;
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
    else if ( s[1] == 'P' || s[1] == 'p') {
      if ( s[2] == 'E' || s[2] == 'e') {
        if ( s[3] == 'C' || s[3] == 'c') {
          if ( s[4] == 'I' || s[4] == 'i') {
            if ( s[5] == 'F' || s[5] == 'f') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'C' || s[7] == 'c') {
                  if ( s[8] == 'T' || s[8] == 't') {
                    if ( s[9] == 'Y' || s[9] == 'y') {
                      if ( s[10] == 'P' || s[10] == 'p') {
                        if ( s[11] == 'E' || s[11] == 'e') {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_12_SPECIFICTYPE;
                          return;
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
    else if ( s[1] == 'Q' || s[1] == 'q') {
      if ( s[2] == 'L' || s[2] == 'l') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'X' || s[4] == 'x') {
            if ( s[5] == 'C' || s[5] == 'c') {
              if ( s[6] == 'E' || s[6] == 'e') {
                if ( s[7] == 'P' || s[7] == 'p') {
                  if ( s[8] == 'T' || s[8] == 't') {
                    if ( s[9] == 'I' || s[9] == 'i') {
                      if ( s[10] == 'O' || s[10] == 'o') {
                        if ( s[11] == 'N' || s[11] == 'n') {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = T_12_SQLEXCEPTION;
                          return;
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

static inline void sqlClassify13( const char *s, Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a') {
    if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'H' || s[3] == 'h') {
          if ( s[4] == 'O' || s[4] == 'o') {
            if ( s[5] == 'R' || s[5] == 'r') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'Z' || s[7] == 'z') {
                  if ( s[8] == 'A' || s[8] == 'a') {
                    if ( s[9] == 'T' || s[9] == 't') {
                      if ( s[10] == 'I' || s[10] == 'i') {
                        if ( s[11] == 'O' || s[11] == 'o') {
                          if ( s[12] == 'N' || s[12] == 'n') {
                            tk.kf.keytype = 1;
                            tk.kf.keyword1 = 1;
                            tk.kf.keyid = T_13_AUTHORIZATION;
                            return;
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
  else if ( s[0] == 'C' || s[0] == 'c') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'R' || s[3] == 'r') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'S' || s[5] == 's') {
              if ( s[6] == 'P' || s[6] == 'p') {
                if ( s[7] == 'O' || s[7] == 'o') {
                  if ( s[8] == 'N' || s[8] == 'n') {
                    if ( s[9] == 'D' || s[9] == 'd') {
                      if ( s[10] == 'I' || s[10] == 'i') {
                        if ( s[11] == 'N' || s[11] == 'n') {
                          if ( s[12] == 'G' || s[12] == 'g') {
                            tk.kf.keytype = 1;
                            tk.kf.keyword1 = 1;
                            tk.kf.keyid = T_13_CORRESPONDING;
                            return;
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
  else if ( s[0] == 'D' || s[0] == 'd') {
    if ( s[1] == 'E' || s[1] == 'e') {
      if ( s[2] == 'T' || s[2] == 't') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'R' || s[4] == 'r') {
            if ( s[5] == 'M' || s[5] == 'm') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'N' || s[7] == 'n') {
                  if ( s[8] == 'I' || s[8] == 'i') {
                    if ( s[9] == 'S' || s[9] == 's') {
                      if ( s[10] == 'T' || s[10] == 't') {
                        if ( s[11] == 'I' || s[11] == 'i') {
                          if ( s[12] == 'C' || s[12] == 'c') {
                            tk.kf.keytype = 1;
                            tk.kf.keyword1 = 1;
                            tk.kf.keyid = T_13_DETERMINISTIC;
                            return;
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
  else if ( s[0] == 'T' || s[0] == 't') {
    if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'Z' || s[4] == 'z') {
            if ( s[5] == 'O' || s[5] == 'o') {
              if ( s[6] == 'N' || s[6] == 'n') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  if ( s[8] == '_' || s[8] == '_') {
                    if ( s[9] == 'H' || s[9] == 'h') {
                      if ( s[10] == 'O' || s[10] == 'o') {
                        if ( s[11] == 'U' || s[11] == 'u') {
                          if ( s[12] == 'R' || s[12] == 'r') {
                            tk.kf.keytype = 1;
                            tk.kf.keyword1 = 1;
                            tk.kf.keyid = T_13_TIMEZONE_HOUR;
                            return;
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

static inline void sqlClassify14( const char *s, Token &tk )
{
  if ( s[0] == 'L' || s[0] == 'l') {
    if ( s[1] == 'O' || s[1] == 'o') {
      if ( s[2] == 'C' || s[2] == 'c') {
        if ( s[3] == 'A' || s[3] == 'a') {
          if ( s[4] == 'L' || s[4] == 'l') {
            if ( s[5] == 'T' || s[5] == 't') {
              if ( s[6] == 'I' || s[6] == 'i') {
                if ( s[7] == 'M' || s[7] == 'm') {
                  if ( s[8] == 'E' || s[8] == 'e') {
                    if ( s[9] == 'S' || s[9] == 's') {
                      if ( s[10] == 'T' || s[10] == 't') {
                        if ( s[11] == 'A' || s[11] == 'a') {
                          if ( s[12] == 'M' || s[12] == 'm') {
                            if ( s[13] == 'P' || s[13] == 'p') {
                              tk.kf.keytype = 1;
                              tk.kf.keyword1 = 1;
                              tk.kf.keyid = T_14_LOCALTIMESTAMP;
                              return;
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

static inline void sqlClassify15( const char *s, Token &tk )
{
  if ( s[0] == 'T' || s[0] == 't') {
    if ( s[1] == 'I' || s[1] == 'i') {
      if ( s[2] == 'M' || s[2] == 'm') {
        if ( s[3] == 'E' || s[3] == 'e') {
          if ( s[4] == 'Z' || s[4] == 'z') {
            if ( s[5] == 'O' || s[5] == 'o') {
              if ( s[6] == 'N' || s[6] == 'n') {
                if ( s[7] == 'E' || s[7] == 'e') {
                  if ( s[8] == '_' || s[8] == '_') {
                    if ( s[9] == 'M' || s[9] == 'm') {
                      if ( s[10] == 'I' || s[10] == 'i') {
                        if ( s[11] == 'N' || s[11] == 'n') {
                          if ( s[12] == 'U' || s[12] == 'u') {
                            if ( s[13] == 'T' || s[13] == 't') {
                              if ( s[14] == 'E' || s[14] == 'e') {
                                tk.kf.keytype = 1;
                                tk.kf.keyword1 = 1;
                                tk.kf.keyid = T_15_TIMEZONE_MINUTE;
                                return;
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

static inline void sqlClassify17( const char *s, Token &tk )
{
  if ( s[0] == 'C' || s[0] == 'c') {
    if ( s[1] == 'U' || s[1] == 'u') {
      if ( s[2] == 'R' || s[2] == 'r') {
        if ( s[3] == 'R' || s[3] == 'r') {
          if ( s[4] == 'E' || s[4] == 'e') {
            if ( s[5] == 'N' || s[5] == 'n') {
              if ( s[6] == 'T' || s[6] == 't') {
                if ( s[7] == '_' || s[7] == '_') {
                  if ( s[8] == 'T' || s[8] == 't') {
                    if ( s[9] == 'I' || s[9] == 'i') {
                      if ( s[10] == 'M' || s[10] == 'm') {
                        if ( s[11] == 'E' || s[11] == 'e') {
                          if ( s[12] == 'S' || s[12] == 's') {
                            if ( s[13] == 'T' || s[13] == 't') {
                              if ( s[14] == 'A' || s[14] == 'a') {
                                if ( s[15] == 'M' || s[15] == 'm') {
                                  if ( s[16] == 'P' || s[16] == 'p') {
                                    tk.kf.keytype = 1;
                                    tk.kf.keyword1 = 1;
                                    tk.kf.keyid = T_17_CURRENT_TIMESTAMP;
                                    return;
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

void sqlClassify( const char *s, int l, Token &tk )
{
  switch (l) {
    case 2: sqlClassify2(s, tk); break;
    case 3: sqlClassify3(s, tk); break;
    case 4: sqlClassify4(s, tk); break;
    case 5: sqlClassify5(s, tk); break;
    case 6: sqlClassify6(s, tk); break;
    case 7: sqlClassify7(s, tk); break;
    case 8: sqlClassify8(s, tk); break;
    case 9: sqlClassify9(s, tk); break;
    case 10: sqlClassify10(s, tk); break;
    case 11: sqlClassify11(s, tk); break;
    case 12: sqlClassify12(s, tk); break;
    case 13: sqlClassify13(s, tk); break;
    case 14: sqlClassify14(s, tk); break;
    case 15: sqlClassify15(s, tk); break;
    case 17: sqlClassify17(s, tk); break;
  }
}

SqlParserEngine::SqlParserEngine()
    : IParserEngine()
{
}

QList<Token> SqlParserEngine::parseTextLine( const QString &text, int state )
{
    QList<Token> tokens;

    const QByteArray bytes = text.toLatin1();
    const char *firstChar = bytes.constData();
    const char *lastChar = firstChar + bytes.size();

    Lexer lex(firstChar, lastChar, sqlClassify);
    lex.setScanKeywords(true);

    if (state != -1)
        lex.setState(state & 0xff);

    unsigned prevKind = T_EOF_SYMBOL;
    for (;;) {
        Token tk;
        lex(&tk, prevKind);
        if (tk.is(T_EOF_SYMBOL)) {
            break;
        }

        tokens.append(tk);
        prevKind = tk.kind();
    }

    _lastState = lex.state();

    return tokens;
}

const char *SqlParserEngine::name( const Token &tk ) const
{
    return tk.spell();
}

ISemanticParser *SqlParserEngine::createSemanticParser( TranslationUnit *unit ) const
{
    return new Parser(unit);
}

ClassifyFunc SqlParserEngine::keyworClassifyFunction()
{
    return sqlClassify;
}

bool SqlParserEngine::hasFutureNullIdent() const
{
    return false;
}

} // Sql

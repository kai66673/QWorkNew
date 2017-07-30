#include "PostgresParserEngine.h"
#include "SqlLexer.h"
#include "PostgresSemanticParser.h"
#include "PostgresParser.tab.hh"

namespace Postgres {

static inline void sqlClassify2( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'S' || s[1] == 's' ) {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = Postgres::PostgresParser::token::T_AS;
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = Postgres::PostgresParser::token::T_AT;
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b' ) {
    if ( s[1] == 'Y' || s[1] == 'y' ) {
      tk.kf.keytype = 3;
      tk.kf.keyword3 = 1;
      tk.kf.keyid = Postgres::PostgresParser::token::T_BY;
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = Postgres::PostgresParser::token::T_DO;
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'F' || s[1] == 'f' ) {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = Postgres::PostgresParser::token::T_IF;
    }
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = Postgres::PostgresParser::token::T_IN;
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = Postgres::PostgresParser::token::T_IS;
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = Postgres::PostgresParser::token::T_NO;
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o' ) {
    if ( s[1] == 'F' || s[1] == 'f' ) {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = Postgres::PostgresParser::token::T_OF;
    }
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = Postgres::PostgresParser::token::T_ON;
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = Postgres::PostgresParser::token::T_OR;
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      tk.kf.keytype = 1;
      tk.kf.keyword1 = 1;
      tk.kf.keyid = Postgres::PostgresParser::token::T_TO;
    }
  }
}

static inline void sqlClassify3( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_ADD;
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_ALL;
      }
    }
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_AND;
      }
      else if ( s[2] == 'Y' || s[2] == 'y' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_ANY;
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_ASC;
      }
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        tk.kf.keytype = 3;
        tk.kf.keyword3 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_BIT;
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'X' || s[2] == 'x' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_BOX;
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'V' || s[2] == 'v' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_CSV;
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'Y' || s[2] == 'y' ) {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_DAY;
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        tk.kf.keytype = 3;
        tk.kf.keyword3 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_DEC;
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_END;
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_FOR;
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        tk.kf.keytype = 3;
        tk.kf.keyword3 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_INT;
      }
    }
  }
  else if ( s[0] == 'K' || s[0] == 'k' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'Y' || s[2] == 'y' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_KEY;
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'X' || s[2] == 'x' ) {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_MAX;
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_MIN;
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_NOT;
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o' ) {
    if ( s[1] == 'F' || s[1] == 'f' ) {
      if ( s[2] == 'F' || s[2] == 'f' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_OFF;
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_OUT;
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'F' || s[2] == 'f' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_REF;
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'W' || s[2] == 'w' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_ROW;
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_SET;
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        tk.kf.keytype = 2;
        tk.kf.keyword2 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_SUM;
      }
    }
  }
  else if ( s[0] == 'X' || s[0] == 'x' ) {
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        tk.kf.keytype = 3;
        tk.kf.keyword3 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_XML;
      }
    }
  }
  else if ( s[0] == 'Y' || s[0] == 'y' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        tk.kf.keytype = 1;
        tk.kf.keyword1 = 1;
        tk.kf.keyid = Postgres::PostgresParser::token::T_YES;
      }
    }
  }
}

static inline void sqlClassify4( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_ALSO;
        }
      }
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_BOOL;
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_BOTH;
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_CASE;
        }
        else if ( s[3] == 'T' || s[3] == 't' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_CAST;
        }
      }
    }
    else if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_CHAR;
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_CIDR;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'Y' || s[3] == 'y' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_COPY;
        }
      }
      else if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_COST;
        }
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_DATA;
        }
        else if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_DATE;
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_DESC;
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_DROP;
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_EACH;
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_ELSE;
        }
      }
    }
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_ENUM;
        }
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f' ) {
    if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_FROM;
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_FULL;
        }
      }
    }
  }
  else if ( s[0] == 'H' || s[0] == 'h' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_HOLD;
        }
      }
      else if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_HOUR;
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_INET;
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == '2' || s[3] == '2' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_INT2;
        }
        else if ( s[3] == '4' || s[3] == '4' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_INT4;
        }
        else if ( s[3] == '8' || s[3] == '8' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_INT8;
        }
        else if ( s[3] == 'O' || s[3] == 'o' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_INTO;
        }
      }
    }
  }
  else if ( s[0] == 'J' || s[0] == 'j' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_JOIN;
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_JSON;
        }
      }
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          tk.kf.keytype = 2;
          tk.kf.keyword2 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_LAST;
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'F' || s[2] == 'f' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_LEFT;
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'K' || s[2] == 'k' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_LIKE;
        }
      }
      else if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_LINE;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_LOAD;
        }
      }
      else if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'K' || s[3] == 'k' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_LOCK;
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'G' || s[3] == 'g' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_LSEG;
        }
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_MODE;
        }
      }
      else if ( s[2] == 'V' || s[2] == 'v' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_MOVE;
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_NAME;
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'X' || s[2] == 'x' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_NEXT;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_NONE;
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_NULL;
        }
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_OIDS;
        }
      }
    }
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'Y' || s[3] == 'y' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_ONLY;
        }
      }
    }
    else if ( s[1] == 'V' || s[1] == 'v' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_OVER;
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
          tk.kf.keyid = Postgres::PostgresParser::token::T_PATH;
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_READ;
        }
        else if ( s[3] == 'L' || s[3] == 'l' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_REAL;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_ROLE;
        }
      }
      else if ( s[2] == 'W' || s[2] == 'w' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_ROWS;
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_RULE;
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'W' || s[3] == 'w' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_SHOW;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_SOME;
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_TEMP;
        }
      }
      else if ( s[2] == 'X' || s[2] == 'x' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_TEXT;
        }
      }
    }
    else if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_THEN;
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_TIME;
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_TRIM;
        }
      }
      else if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_TRUE;
        }
      }
    }
    else if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_TYPE;
        }
      }
    }
  }
  else if ( s[0] == 'U' || s[0] == 'u' ) {
    if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_USER;
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_UUID;
        }
      }
    }
  }
  else if ( s[0] == 'V' || s[0] == 'v' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'W' || s[3] == 'w' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_VIEW;
        }
      }
    }
  }
  else if ( s[0] == 'W' || s[0] == 'w' ) {
    if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_WHEN;
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_WITH;
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'K' || s[3] == 'k' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_WORK;
        }
      }
    }
  }
  else if ( s[0] == 'Y' || s[0] == 'y' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          tk.kf.keytype = 1;
          tk.kf.keyword1 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_YEAR;
        }
      }
    }
  }
  else if ( s[0] == 'Z' || s[0] == 'z' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          tk.kf.keytype = 3;
          tk.kf.keyword3 = 1;
          tk.kf.keyid = Postgres::PostgresParser::token::T_ZONE;
        }
      }
    }
  }
}

static inline void sqlClassify5( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'B' || s[1] == 'b' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_ABORT;
          }
        }
      }
    }
    else if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_ADMIN;
          }
        }
      }
    }
    else if ( s[1] == 'F' || s[1] == 'f' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_AFTER;
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_ALTER;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'Y' || s[4] == 'y' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_ARRAY;
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
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_BEGIN;
          }
        }
      }
    }
    else if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            tk.kf.keytype = 3;
            tk.kf.keyword3 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_BYTEA;
          }
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_CACHE;
          }
        }
      }
    }
    else if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_CHAIN;
          }
        }
      }
      else if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'K' || s[4] == 'k' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_CHECK;
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_CLASS;
          }
        }
      }
      else if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_CLOSE;
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_COUNT;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_CROSS;
          }
        }
      }
    }
    else if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_CYCLE;
          }
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e' ) {
    if ( s[1] == 'V' || s[1] == 'v' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_EVENT;
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
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_FALSE;
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'H' || s[4] == 'h' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_FETCH;
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_FIRST;
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 3;
            tk.kf.keyword3 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_FLOAT;
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_FORCE;
          }
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g' ) {
    if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_GRANT;
          }
        }
      }
      else if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_GROUP;
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'K' || s[3] == 'k' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_ILIKE;
          }
        }
      }
    }
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'X' || s[4] == 'x' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_INDEX;
          }
        }
      }
      else if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_INNER;
          }
        }
      }
      else if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_INOUT;
          }
        }
      }
      else if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_INPUT;
          }
        }
      }
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'B' || s[2] == 'b' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_LABEL;
          }
        }
      }
      else if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'G' || s[3] == 'g' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_LARGE;
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_LEAST;
          }
        }
      }
      else if ( s[2] == 'V' || s[2] == 'v' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_LEVEL;
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_LIMIT;
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_LOCAL;
          }
        }
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'H' || s[4] == 'h' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_MATCH;
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'Y' || s[4] == 'y' ) {
            tk.kf.keytype = 3;
            tk.kf.keyword3 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_MONEY;
          }
        }
        else if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'H' || s[4] == 'h' ) {
            tk.kf.keytype = 2;
            tk.kf.keyword2 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_MONTH;
          }
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_NAMES;
          }
        }
      }
    }
    else if ( s[1] == 'C' || s[1] == 'c' ) {
      if ( s[2] == 'H' || s[2] == 'h' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_NCHAR;
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_NULLS;
          }
        }
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o' ) {
    if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_ORDER;
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_OUTER;
          }
        }
      }
    }
    else if ( s[1] == 'W' || s[1] == 'w' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_OWNED;
          }
          else if ( s[4] == 'R' || s[4] == 'r' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_OWNER;
          }
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p' ) {
    if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_PLANS;
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 3;
            tk.kf.keyword3 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_POINT;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_PRIOR;
          }
        }
      }
    }
  }
  else if ( s[0] == 'Q' || s[0] == 'q' ) {
    if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_QUOTE;
          }
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'G' || s[3] == 'g' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_RANGE;
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_RESET;
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'G' || s[2] == 'g' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_RIGHT;
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'F' || s[4] == 'f' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_SETOF;
          }
        }
      }
    }
    else if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_SHARE;
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_START;
          }
        }
      }
      else if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_STDIN;
          }
        }
      }
      else if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_STRIP;
          }
        }
      }
    }
    else if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_SYSID;
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'B' || s[2] == 'b' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_TABLE;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_TREAT;
          }
        }
      }
    }
    else if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_TYPES;
          }
        }
      }
    }
  }
  else if ( s[0] == 'U' || s[0] == 'u' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_UNION;
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_UNTIL;
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'G' || s[4] == 'g' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_USING;
          }
        }
      }
    }
  }
  else if ( s[0] == 'V' || s[0] == 'v' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_VALID;
          }
        }
        else if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_VALUE;
          }
        }
      }
    }
  }
  else if ( s[0] == 'W' || s[0] == 'w' ) {
    if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_WHERE;
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_WRITE;
          }
        }
      }
    }
  }
  else if ( s[0] == 'X' || s[0] == 'x' ) {
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            tk.kf.keytype = 1;
            tk.kf.keyword1 = 1;
            tk.kf.keyid = Postgres::PostgresParser::token::T_XMLPI;
          }
        }
      }
    }
  }
}

static inline void sqlClassify6( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'C' || s[1] == 'c' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_ACCESS;
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_ACTION;
            }
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'W' || s[2] == 'w' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'Y' || s[4] == 'y' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_ALWAYS;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'F' || s[2] == 'f' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_BEFORE;
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'G' || s[2] == 'g' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_BIGINT;
            }
          }
        }
      }
      else if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'Y' || s[5] == 'y' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_BINARY;
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
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_CALLED;
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_CIRCLE;
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_COLUMN;
            }
          }
        }
      }
      else if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_COMMIT;
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
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_CREATE;
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_CURSOR;
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
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_DELETE;
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_DOMAIN;
            }
          }
        }
      }
      else if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'B' || s[3] == 'b' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_DOUBLE;
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
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_ENABLE;
            }
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_ESCAPE;
            }
          }
        }
      }
    }
    else if ( s[1] == 'X' || s[1] == 'x' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_EXCEPT;
            }
          }
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_EXISTS;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'Y' || s[5] == 'y' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_FAMILY;
            }
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == '4' || s[5] == '4' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_FLOAT4;
            }
            else if ( s[5] == '8' || s[5] == '8' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_FLOAT8;
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'Z' || s[4] == 'z' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_FREEZE;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g' ) {
    if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'B' || s[3] == 'b' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_GLOBAL;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'H' || s[0] == 'h' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'V' || s[2] == 'v' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'G' || s[5] == 'g' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_HAVING;
            }
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_HEADER;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_INLINE;
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_INSERT;
            }
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_ISNULL;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_LISTEN;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_MINUTE;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'F' || s[4] == 'f' ) {
            if ( s[5] == 'Y' || s[5] == 'y' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_NOTIFY;
            }
          }
        }
      }
      else if ( s[2] == 'W' || s[2] == 'w' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_NOWAIT;
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'F' || s[5] == 'f' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_NULLIF;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o' ) {
    if ( s[1] == 'B' || s[1] == 'b' ) {
      if ( s[2] == 'J' || s[2] == 'j' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_OBJECT;
            }
          }
        }
      }
    }
    else if ( s[1] == 'F' || s[1] == 'f' ) {
      if ( s[2] == 'F' || s[2] == 'f' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_OFFSET;
            }
          }
        }
      }
    }
    else if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_OPTION;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_PARSER;
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
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_RENAME;
            }
          }
        }
      }
      else if ( s[2] == 'V' || s[2] == 'v' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'K' || s[4] == 'k' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_REVOKE;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'C' || s[1] == 'c' ) {
      if ( s[2] == 'H' || s[2] == 'h' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_SCHEMA;
            }
          }
        }
      }
      else if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_SCROLL;
            }
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'H' || s[5] == 'h' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_SEARCH;
            }
          }
        }
      }
      else if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              tk.kf.keytype = 2;
              tk.kf.keyword2 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_SECOND;
            }
          }
        }
      }
      else if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_SELECT;
            }
          }
        }
      }
      else if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_SERIAL;
            }
          }
        }
        else if ( s[3] == 'V' || s[3] == 'v' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_SERVER;
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_SIMPLE;
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'B' || s[3] == 'b' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_STABLE;
            }
          }
        }
      }
      else if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_STDOUT;
            }
          }
        }
      }
      else if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_STRICT;
            }
          }
        }
      }
    }
    else if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'M' || s[5] == 'm' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_SYSTEM;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'B' || s[2] == 'b' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_TABLES;
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'Z' || s[5] == 'z' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_TIMETZ;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'U' || s[0] == 'u' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'Q' || s[3] == 'q' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_UNIQUE;
            }
          }
        }
      }
    }
    else if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'D' || s[2] == 'd' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_UPDATE;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'V' || s[0] == 'v' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'M' || s[5] == 'm' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_VACUUM;
            }
          }
        }
      }
      else if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_VALUES;
            }
          }
        }
      }
      else if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'B' || s[3] == 'b' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              tk.kf.keytype = 3;
              tk.kf.keyword3 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_VARBIT;
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'W' || s[0] == 'w' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'W' || s[5] == 'w' ) {
              tk.kf.keytype = 1;
              tk.kf.keyword1 = 1;
              tk.kf.keyid = Postgres::PostgresParser::token::T_WINDOW;
            }
          }
        }
      }
    }
  }
}

static inline void sqlClassify7( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'Y' || s[4] == 'y' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_ANALYSE;
              }
            }
            else if ( s[5] == 'Z' || s[5] == 'z' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_ANALYZE;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'W' || s[3] == 'w' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_BETWEEN;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_BOOLEAN;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_CASCADE;
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'G' || s[6] == 'g' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_CATALOG;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_CLUSTER;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_COLLATE;
              }
            }
          }
        }
      }
      else if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_COMMENT;
              }
            }
          }
        }
      }
      else if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_CONTENT;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_CURRENT;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_DECIMAL;
              }
            }
          }
        }
        else if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_DECLARE;
              }
            }
          }
        }
      }
      else if ( s[2] == 'F' || s[2] == 'f' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_DEFAULT;
              }
            }
          }
        }
        else if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_DEFINER;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'B' || s[4] == 'b' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_DISABLE;
              }
            }
          }
        }
        else if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_DISCARD;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'E' || s[0] == 'e' ) {
    if ( s[1] == 'X' || s[1] == 'x' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_EXCLUDE;
              }
            }
          }
        }
      }
      else if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_EXECUTE;
              }
            }
          }
        }
      }
      else if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_EXPLAIN;
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_EXTRACT;
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
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'G' || s[5] == 'g' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_FOREIGN;
              }
            }
          }
        }
        else if ( s[3] == 'W' || s[3] == 'w' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_FORWARD;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g' ) {
    if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_GRANTED;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'H' || s[0] == 'h' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_HANDLER;
              }
            }
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
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_INDEXES;
              }
            }
          }
        }
      }
      else if ( s[2] == 'H' || s[2] == 'h' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_INHERIT;
              }
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_INSTEAD;
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'G' || s[4] == 'g' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_INTEGER;
              }
            }
          }
        }
      }
      else if ( s[2] == 'V' || s[2] == 'v' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'K' || s[4] == 'k' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_INVOKER;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_LATERAL;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'G' || s[6] == 'g' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_LEADING;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_MACADDR;
              }
            }
          }
        }
      }
      else if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'G' || s[6] == 'g' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_MAPPING;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_NATURAL;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'G' || s[6] == 'g' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_NOTHING;
              }
            }
          }
        }
        else if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_NOTNULL;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_NUMERIC;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'O' || s[0] == 'o' ) {
    if ( s[1] == 'P' || s[1] == 'p' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_OPTIONS;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'V' || s[1] == 'v' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'Y' || s[6] == 'y' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_OVERLAY;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'P' || s[0] == 'p' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_PARTIAL;
              }
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'G' || s[6] == 'g' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_PASSING;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'L' || s[1] == 'l' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'G' || s[6] == 'g' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_PLACING;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'Y' || s[3] == 'y' ) {
          if ( s[4] == 'G' || s[4] == 'g' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_POLYGON;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_PREPARE;
              }
            }
          }
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'Y' || s[6] == 'y' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_PRIMARY;
              }
            }
          }
        }
      }
      else if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'G' || s[3] == 'g' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'M' || s[6] == 'm' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_PROGRAM;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'R' || s[0] == 'r' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'K' || s[6] == 'k' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_RECHECK;
              }
            }
          }
        }
      }
      else if ( s[2] == 'F' || s[2] == 'f' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'H' || s[6] == 'h' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_REFRESH;
              }
            }
          }
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'X' || s[6] == 'x' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_REINDEX;
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
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_RELEASE;
              }
            }
          }
        }
      }
      else if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_REPLACE;
              }
            }
          }
          else if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_REPLICA;
              }
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_RESTART;
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_RETURNS;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'S' || s[0] == 's' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == '2' || s[6] == '2' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_SERIAL2;
              }
              else if ( s[6] == '4' || s[6] == '4' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_SERIAL4;
              }
              else if ( s[6] == '8' || s[6] == '8' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_SERIAL8;
              }
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_SESSION;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_SIMILAR;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'G' || s[5] == 'g' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_STORAGE;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'G' || s[3] == 'g' ) {
          if ( s[4] == 'G' || s[4] == 'g' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_TRIGGER;
              }
            }
          }
        }
      }
      else if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_TRUSTED;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'Q' || s[2] == 'q' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'Y' || s[6] == 'y' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_TSQUERY;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'U' || s[0] == 'u' ) {
    if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'K' || s[2] == 'k' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'W' || s[5] == 'w' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_UNKNOWN;
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
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'H' || s[4] == 'h' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_VARCHAR;
              }
            }
          }
        }
        else if ( s[3] == 'Y' || s[3] == 'y' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'G' || s[6] == 'g' ) {
                tk.kf.keytype = 3;
                tk.kf.keyword3 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_VARYING;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'B' || s[3] == 'b' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_VERBOSE;
              }
            }
          }
        }
        else if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_VERSION;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'W' || s[0] == 'w' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'U' || s[5] == 'u' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_WITHOUT;
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_WRAPPER;
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'X' || s[0] == 'x' ) {
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                tk.kf.keytype = 1;
                tk.kf.keyword1 = 1;
                tk.kf.keyid = Postgres::PostgresParser::token::T_XMLROOT;
              }
            }
          }
        }
      }
    }
  }
}

static inline void sqlClassify8( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'B' || s[1] == 'b' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'U' || s[5] == 'u' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_ABSOLUTE;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'B' || s[0] == 'b' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'K' || s[3] == 'k' ) {
          if ( s[4] == 'W' || s[4] == 'w' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'D' || s[7] == 'd' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_BACKWARD;
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
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'D' || s[7] == 'd' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_CASCADED;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_COALESCE;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_COMMENTS;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_CONTINUE;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'D' || s[0] == 'd' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'B' || s[4] == 'b' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_DATABASE;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'F' || s[2] == 'f' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_DEFAULTS;
                }
              }
            }
          }
        }
        else if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'D' || s[7] == 'd' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_DEFERRED;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_DISTINCT;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_DOCUMENT;
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
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'G' || s[7] == 'g' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_ENCODING;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'X' || s[1] == 'x' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_EXTERNAL;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'F' || s[0] == 'f' ) {
    if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_FUNCTION;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'G' || s[0] == 'g' ) {
    if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_GREATEST;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'I' || s[0] == 'i' ) {
    if ( s[1] == 'D' || s[1] == 'd' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'Y' || s[7] == 'y' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_IDENTITY;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_IMPLICIT;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'H' || s[2] == 'h' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_INHERITS;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'V' || s[5] == 'v' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_INTERVAL;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'L' || s[0] == 'l' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'G' || s[3] == 'g' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'G' || s[6] == 'g' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_LANGUAGE;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'C' || s[1] == 'c' ) {
      if ( s[2] == '_' || s[2] == '_' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'Y' || s[5] == 'y' ) {
              if ( s[6] == 'P' || s[6] == 'p' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_LC_CTYPE;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_LOCATION;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'M' || s[0] == 'm' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'X' || s[2] == 'x' ) {
        if ( s[3] == 'V' || s[3] == 'v' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_MAXVALUE;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'V' || s[3] == 'v' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_MINVALUE;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'N' || s[0] == 'n' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_NATIONAL;
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
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_OPERATOR;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'V' || s[1] == 'v' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'P' || s[6] == 'p' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_OVERLAPS;
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
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'W' || s[4] == 'w' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'D' || s[7] == 'd' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_PASSWORD;
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
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_POSITION;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'D' || s[7] == 'd' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_PREPARED;
                }
              }
            }
          }
        }
        else if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'V' || s[6] == 'v' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_PRESERVE;
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
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'G' || s[6] == 'g' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_REASSIGN;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'V' || s[6] == 'v' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_RELATIVE;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_RESTRICT;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'B' || s[4] == 'b' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                if ( s[7] == 'K' || s[7] == 'k' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_ROLLBACK;
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
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'Y' || s[7] == 'y' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_SECURITY;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'Q' || s[2] == 'q' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_SEQUENCE;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  tk.kf.keytype = 3;
                  tk.kf.keyword3 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_SMALLINT;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'N' || s[1] == 'n' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'H' || s[5] == 'h' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_SNAPSHOT;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_TEMPLATE;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'G' || s[7] == 'g' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_TRAILING;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'U' || s[2] == 'u' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_TRUNCATE;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'V' || s[2] == 'v' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  tk.kf.keytype = 3;
                  tk.kf.keyword3 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_TSVECTOR;
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
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_UNLISTEN;
                }
              }
            }
          }
        }
        else if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'G' || s[4] == 'g' ) {
            if ( s[5] == 'G' || s[5] == 'g' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'D' || s[7] == 'd' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_UNLOGGED;
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
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_VALIDATE;
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'C' || s[7] == 'c' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_VARIADIC;
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_VOLATILE;
                }
              }
            }
          }
        }
      }
    }
  }
  else if ( s[0] == 'X' || s[0] == 'x' ) {
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  tk.kf.keytype = 1;
                  tk.kf.keyword1 = 1;
                  tk.kf.keyid = Postgres::PostgresParser::token::T_XMLPARSE;
                }
              }
            }
          }
        }
      }
    }
  }
}

static inline void sqlClassify9( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'G' || s[1] == 'g' ) {
      if ( s[2] == 'G' || s[2] == 'g' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'G' || s[5] == 'g' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_AGGREGATE;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_ASSERTION;
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
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'B' || s[5] == 'b' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_ATTRIBUTE;
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
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'G' || s[2] == 'g' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'L' || s[8] == 'l' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_BIGSERIAL;
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
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_CHARACTER;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_COLLATION;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'D' || s[8] == 'd' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_COMMITTED;
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
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_DELIMITER;
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
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'Y' || s[4] == 'y' ) {
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'D' || s[8] == 'd' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_ENCRYPTED;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'X' || s[1] == 'x' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'G' || s[8] == 'g' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_EXCLUDING;
                  }
                }
              }
            }
            else if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'V' || s[7] == 'v' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_EXCLUSIVE;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_EXTENSION;
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
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'W' || s[5] == 'w' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'G' || s[8] == 'g' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_FOLLOWING;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_FUNCTIONS;
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
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_IMMEDIATE;
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'B' || s[6] == 'b' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_IMMUTABLE;
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
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'G' || s[8] == 'g' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_INCLUDING;
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'M' || s[5] == 'm' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_INCREMENT;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  if ( s[8] == 'Y' || s[8] == 'y' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_INITIALLY;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'C' || s[7] == 'c' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_INTERSECT;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_ISOLATION;
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
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'K' || s[3] == 'k' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'F' || s[8] == 'f' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_LEAKPROOF;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'M' || s[7] == 'm' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_LOCALTIME;
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
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_PARTITION;
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
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'G' || s[8] == 'g' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_PRECEDING;
                  }
                }
              }
            }
          }
          else if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_PRECISION;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_PROCEDURE;
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
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'V' || s[7] == 'v' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_RECURSIVE;
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'G' || s[8] == 'g' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_RETURNING;
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
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'V' || s[2] == 'v' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'P' || s[4] == 'p' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_SAVEPOINT;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'Q' || s[2] == 'q' ) {
        if ( s[3] == 'U' || s[3] == 'u' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_SEQUENCES;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'M' || s[5] == 'm' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_STATEMENT;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'B' || s[2] == 'b' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'N' || s[7] == 'n' ) {
                  if ( s[8] == 'G' || s[8] == 'g' ) {
                    tk.kf.keytype = 2;
                    tk.kf.keyword2 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_SUBSTRING;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'Y' || s[1] == 'y' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_SYMMETRIC;
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
    if ( s[1] == 'E' || s[1] == 'e' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'P' || s[3] == 'p' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  if ( s[8] == 'Y' || s[8] == 'y' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_TEMPORARY;
                  }
                }
              }
            }
          }
        }
      }
    }
    else if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'M' || s[7] == 'm' ) {
                  if ( s[8] == 'P' || s[8] == 'p' ) {
                    tk.kf.keytype = 3;
                    tk.kf.keyword3 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_TIMESTAMP;
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
      if ( s[2] == 'B' || s[2] == 'b' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'D' || s[6] == 'd' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'D' || s[8] == 'd' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_UNBOUNDED;
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
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_VALIDATOR;
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
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_XMLCONCAT;
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'X' || s[4] == 'x' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_XMLEXISTS;
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'F' || s[3] == 'f' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'S' || s[7] == 's' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    tk.kf.keytype = 1;
                    tk.kf.keyword1 = 1;
                    tk.kf.keyid = Postgres::PostgresParser::token::T_XMLFOREST;
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

static inline void sqlClassify10( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'S' || s[1] == 's' ) {
      if ( s[2] == 'S' || s[2] == 's' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'G' || s[4] == 'g' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'M' || s[6] == 'm' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_ASSIGNMENT;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'Y' || s[2] == 'y' ) {
        if ( s[3] == 'M' || s[3] == 'm' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  if ( s[8] == 'I' || s[8] == 'i' ) {
                    if ( s[9] == 'C' || s[9] == 'c' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_ASYMMETRIC;
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
            if ( s[5] == 'P' || s[5] == 'p' ) {
              if ( s[6] == 'O' || s[6] == 'o' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_CHECKPOINT;
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
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'O' || s[8] == 'o' ) {
                    if ( s[9] == 'N' || s[9] == 'n' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_CONNECTION;
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
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_CONSTRAINT;
                    }
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'V' || s[3] == 'v' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'S' || s[6] == 's' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'O' || s[8] == 'o' ) {
                    if ( s[9] == 'N' || s[9] == 'n' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_CONVERSION;
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
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_DEALLOCATE;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'F' || s[2] == 'f' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'B' || s[7] == 'b' ) {
                  if ( s[8] == 'L' || s[8] == 'l' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_DEFERRABLE;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_DELIMITERS;
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
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'Y' || s[9] == 'y' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_DICTIONARY;
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
    if ( s[1] == 'C' || s[1] == 'c' ) {
      if ( s[2] == '_' || s[2] == '_' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_LC_COLLATE;
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
    if ( s[1] == 'R' || s[1] == 'r' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'V' || s[3] == 'v' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'G' || s[7] == 'g' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_PRIVILEGES;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'O' || s[2] == 'o' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'D' || s[5] == 'd' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    if ( s[9] == 'L' || s[9] == 'l' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_PROCEDURAL;
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
      if ( s[2] == 'F' || s[2] == 'f' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'C' || s[7] == 'c' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_REFERENCES;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'P' || s[2] == 'p' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'B' || s[7] == 'b' ) {
                  if ( s[8] == 'L' || s[8] == 'l' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_REPEATABLE;
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
    if ( s[1] == 'T' || s[1] == 't' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'D' || s[4] == 'd' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'L' || s[6] == 'l' ) {
                if ( s[7] == 'O' || s[7] == 'o' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_STANDALONE;
                    }
                  }
                }
              }
            }
          }
        }
        else if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_STATISTICS;
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
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'B' || s[2] == 'b' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'P' || s[6] == 'p' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_TABLESPACE;
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
    if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'T' || s[3] == 't' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'P' || s[6] == 'p' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_WHITESPACE;
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
  else if ( s[0] == 'X' || s[0] == 'x' ) {
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'E' || s[5] == 'e' ) {
              if ( s[6] == 'M' || s[6] == 'm' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      tk.kf.keytype = 1;
                      tk.kf.keyword1 = 1;
                      tk.kf.keyid = Postgres::PostgresParser::token::T_XMLELEMENT;
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

static inline void sqlClassify11( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == 'S' || s[10] == 's' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = Postgres::PostgresParser::token::T_CONSTRAINTS;
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
          if ( s[4] == 'N' || s[4] == 'n' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'I' || s[8] == 'i' ) {
                    if ( s[9] == 'V' || s[9] == 'v' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = Postgres::PostgresParser::token::T_INSENSITIVE;
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
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'L' || s[3] == 'l' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'E' || s[6] == 'e' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  if ( s[8] == 'I' || s[8] == 'i' ) {
                    if ( s[9] == 'A' || s[9] == 'a' ) {
                      if ( s[10] == 'L' || s[10] == 'l' ) {
                        tk.kf.keytype = 3;
                        tk.kf.keyword3 = 1;
                        tk.kf.keyid = Postgres::PostgresParser::token::T_SMALLSERIAL;
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
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'I' || s[1] == 'i' ) {
      if ( s[2] == 'M' || s[2] == 'm' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'M' || s[7] == 'm' ) {
                  if ( s[8] == 'P' || s[8] == 'p' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == 'Z' || s[10] == 'z' ) {
                        tk.kf.keytype = 3;
                        tk.kf.keyword3 = 1;
                        tk.kf.keyid = Postgres::PostgresParser::token::T_TIMESTAMPTZ;
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
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'S' || s[4] == 's' ) {
            if ( s[5] == 'A' || s[5] == 'a' ) {
              if ( s[6] == 'C' || s[6] == 'c' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'I' || s[8] == 'i' ) {
                    if ( s[9] == 'O' || s[9] == 'o' ) {
                      if ( s[10] == 'N' || s[10] == 'n' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = Postgres::PostgresParser::token::T_TRANSACTION;
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
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'O' || s[3] == 'o' ) {
          if ( s[4] == 'M' || s[4] == 'm' ) {
            if ( s[5] == 'M' || s[5] == 'm' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'T' || s[7] == 't' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'D' || s[10] == 'd' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = Postgres::PostgresParser::token::T_UNCOMMITTED;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      else if ( s[2] == 'E' || s[2] == 'e' ) {
        if ( s[3] == 'N' || s[3] == 'n' ) {
          if ( s[4] == 'C' || s[4] == 'c' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'Y' || s[6] == 'y' ) {
                if ( s[7] == 'P' || s[7] == 'p' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'E' || s[9] == 'e' ) {
                      if ( s[10] == 'D' || s[10] == 'd' ) {
                        tk.kf.keytype = 1;
                        tk.kf.keyword1 = 1;
                        tk.kf.keyid = Postgres::PostgresParser::token::T_UNENCRYPTED;
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

static inline void sqlClassify12( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'C' || s[3] == 'c' ) {
          if ( s[4] == 'U' || s[4] == 'u' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'N' || s[8] == 'n' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == 'L' || s[10] == 'l' ) {
                        if ( s[11] == 'Y' || s[11] == 'y' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = Postgres::PostgresParser::token::T_CONCURRENTLY;
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
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == '_' || s[7] == '_' ) {
                  if ( s[8] == 'D' || s[8] == 'd' ) {
                    if ( s[9] == 'A' || s[9] == 'a' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = Postgres::PostgresParser::token::T_CURRENT_DATE;
                        }
                      }
                    }
                  }
                  else if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'O' || s[9] == 'o' ) {
                      if ( s[10] == 'L' || s[10] == 'l' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = Postgres::PostgresParser::token::T_CURRENT_ROLE;
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
                          tk.kf.keyid = Postgres::PostgresParser::token::T_CURRENT_TIME;
                        }
                      }
                    }
                  }
                  else if ( s[8] == 'U' || s[8] == 'u' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        if ( s[11] == 'R' || s[11] == 'r' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = Postgres::PostgresParser::token::T_CURRENT_USER;
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
    if ( s[1] == 'A' || s[1] == 'a' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'E' || s[3] == 'e' ) {
          if ( s[4] == 'R' || s[4] == 'r' ) {
            if ( s[5] == 'I' || s[5] == 'i' ) {
              if ( s[6] == 'A' || s[6] == 'a' ) {
                if ( s[7] == 'L' || s[7] == 'l' ) {
                  if ( s[8] == 'I' || s[8] == 'i' ) {
                    if ( s[9] == 'Z' || s[9] == 'z' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        if ( s[11] == 'D' || s[11] == 'd' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = Postgres::PostgresParser::token::T_MATERIALIZED;
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
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'I' || s[3] == 'i' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'L' || s[5] == 'l' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'Z' || s[7] == 'z' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    if ( s[9] == 'B' || s[9] == 'b' ) {
                      if ( s[10] == 'L' || s[10] == 'l' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = Postgres::PostgresParser::token::T_SERIALIZABLE;
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
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'O' || s[5] == 'o' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == '_' || s[7] == '_' ) {
                  if ( s[8] == 'U' || s[8] == 'u' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      if ( s[10] == 'E' || s[10] == 'e' ) {
                        if ( s[11] == 'R' || s[11] == 'r' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = Postgres::PostgresParser::token::T_SESSION_USER;
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
  else if ( s[0] == 'X' || s[0] == 'x' ) {
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'S' || s[3] == 's' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'L' || s[8] == 'l' ) {
                    if ( s[9] == 'I' || s[9] == 'i' ) {
                      if ( s[10] == 'Z' || s[10] == 'z' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          tk.kf.keytype = 1;
                          tk.kf.keyword1 = 1;
                          tk.kf.keyid = Postgres::PostgresParser::token::T_XMLSERIALIZE;
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

static inline void sqlClassify13( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'A' || s[0] == 'a' ) {
    if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'T' || s[2] == 't' ) {
        if ( s[3] == 'H' || s[3] == 'h' ) {
          if ( s[4] == 'O' || s[4] == 'o' ) {
            if ( s[5] == 'R' || s[5] == 'r' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'Z' || s[7] == 'z' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == 'I' || s[10] == 'i' ) {
                        if ( s[11] == 'O' || s[11] == 'o' ) {
                          if ( s[12] == 'N' || s[12] == 'n' ) {
                            tk.kf.keytype = 1;
                            tk.kf.keyword1 = 1;
                            tk.kf.keyid = Postgres::PostgresParser::token::T_AUTHORIZATION;
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
  else if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'N' || s[2] == 'n' ) {
        if ( s[3] == 'F' || s[3] == 'f' ) {
          if ( s[4] == 'I' || s[4] == 'i' ) {
            if ( s[5] == 'G' || s[5] == 'g' ) {
              if ( s[6] == 'U' || s[6] == 'u' ) {
                if ( s[7] == 'R' || s[7] == 'r' ) {
                  if ( s[8] == 'A' || s[8] == 'a' ) {
                    if ( s[9] == 'T' || s[9] == 't' ) {
                      if ( s[10] == 'I' || s[10] == 'i' ) {
                        if ( s[11] == 'O' || s[11] == 'o' ) {
                          if ( s[12] == 'N' || s[12] == 'n' ) {
                            tk.kf.keytype = 1;
                            tk.kf.keyword1 = 1;
                            tk.kf.keyid = Postgres::PostgresParser::token::T_CONFIGURATION;
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
  else if ( s[0] == 'T' || s[0] == 't' ) {
    if ( s[1] == 'X' || s[1] == 'x' ) {
      if ( s[2] == 'I' || s[2] == 'i' ) {
        if ( s[3] == 'D' || s[3] == 'd' ) {
          if ( s[4] == '_' || s[4] == '_' ) {
            if ( s[5] == 'S' || s[5] == 's' ) {
              if ( s[6] == 'N' || s[6] == 'n' ) {
                if ( s[7] == 'A' || s[7] == 'a' ) {
                  if ( s[8] == 'P' || s[8] == 'p' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      if ( s[10] == 'H' || s[10] == 'h' ) {
                        if ( s[11] == 'O' || s[11] == 'o' ) {
                          if ( s[12] == 'T' || s[12] == 't' ) {
                            tk.kf.keytype = 3;
                            tk.kf.keyword3 = 1;
                            tk.kf.keyid = Postgres::PostgresParser::token::T_TXID_SNAPSHOT;
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
  else if ( s[0] == 'X' || s[0] == 'x' ) {
    if ( s[1] == 'M' || s[1] == 'm' ) {
      if ( s[2] == 'L' || s[2] == 'l' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'T' || s[4] == 't' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'R' || s[6] == 'r' ) {
                if ( s[7] == 'I' || s[7] == 'i' ) {
                  if ( s[8] == 'B' || s[8] == 'b' ) {
                    if ( s[9] == 'U' || s[9] == 'u' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          if ( s[12] == 'S' || s[12] == 's' ) {
                            tk.kf.keytype = 1;
                            tk.kf.keyword1 = 1;
                            tk.kf.keyid = Postgres::PostgresParser::token::T_XMLATTRIBUTES;
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

static inline void sqlClassify14( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == '_' || s[7] == '_' ) {
                  if ( s[8] == 'S' || s[8] == 's' ) {
                    if ( s[9] == 'C' || s[9] == 'c' ) {
                      if ( s[10] == 'H' || s[10] == 'h' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          if ( s[12] == 'M' || s[12] == 'm' ) {
                            if ( s[13] == 'A' || s[13] == 'a' ) {
                              tk.kf.keytype = 1;
                              tk.kf.keyword1 = 1;
                              tk.kf.keyid = Postgres::PostgresParser::token::T_CURRENT_SCHEMA;
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
  else if ( s[0] == 'L' || s[0] == 'l' ) {
    if ( s[1] == 'O' || s[1] == 'o' ) {
      if ( s[2] == 'C' || s[2] == 'c' ) {
        if ( s[3] == 'A' || s[3] == 'a' ) {
          if ( s[4] == 'L' || s[4] == 'l' ) {
            if ( s[5] == 'T' || s[5] == 't' ) {
              if ( s[6] == 'I' || s[6] == 'i' ) {
                if ( s[7] == 'M' || s[7] == 'm' ) {
                  if ( s[8] == 'E' || s[8] == 'e' ) {
                    if ( s[9] == 'S' || s[9] == 's' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        if ( s[11] == 'A' || s[11] == 'a' ) {
                          if ( s[12] == 'M' || s[12] == 'm' ) {
                            if ( s[13] == 'P' || s[13] == 'p' ) {
                              tk.kf.keytype = 1;
                              tk.kf.keyword1 = 1;
                              tk.kf.keyid = Postgres::PostgresParser::token::T_LOCALTIMESTAMP;
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

static inline void sqlClassify15( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'H' || s[1] == 'h' ) {
      if ( s[2] == 'A' || s[2] == 'a' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'A' || s[4] == 'a' ) {
            if ( s[5] == 'C' || s[5] == 'c' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == 'E' || s[7] == 'e' ) {
                  if ( s[8] == 'R' || s[8] == 'r' ) {
                    if ( s[9] == 'I' || s[9] == 'i' ) {
                      if ( s[10] == 'S' || s[10] == 's' ) {
                        if ( s[11] == 'T' || s[11] == 't' ) {
                          if ( s[12] == 'I' || s[12] == 'i' ) {
                            if ( s[13] == 'C' || s[13] == 'c' ) {
                              if ( s[14] == 'S' || s[14] == 's' ) {
                                tk.kf.keytype = 1;
                                tk.kf.keyword1 = 1;
                                tk.kf.keyid = Postgres::PostgresParser::token::T_CHARACTERISTICS;
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
    else if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == '_' || s[7] == '_' ) {
                  if ( s[8] == 'C' || s[8] == 'c' ) {
                    if ( s[9] == 'A' || s[9] == 'a' ) {
                      if ( s[10] == 'T' || s[10] == 't' ) {
                        if ( s[11] == 'A' || s[11] == 'a' ) {
                          if ( s[12] == 'L' || s[12] == 'l' ) {
                            if ( s[13] == 'O' || s[13] == 'o' ) {
                              if ( s[14] == 'G' || s[14] == 'g' ) {
                                tk.kf.keytype = 1;
                                tk.kf.keyword1 = 1;
                                tk.kf.keyid = Postgres::PostgresParser::token::T_CURRENT_CATALOG;
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

static inline void sqlClassify17( const char *s, Sql::Token &tk )
{
  if ( s[0] == 'C' || s[0] == 'c' ) {
    if ( s[1] == 'U' || s[1] == 'u' ) {
      if ( s[2] == 'R' || s[2] == 'r' ) {
        if ( s[3] == 'R' || s[3] == 'r' ) {
          if ( s[4] == 'E' || s[4] == 'e' ) {
            if ( s[5] == 'N' || s[5] == 'n' ) {
              if ( s[6] == 'T' || s[6] == 't' ) {
                if ( s[7] == '_' || s[7] == '_' ) {
                  if ( s[8] == 'T' || s[8] == 't' ) {
                    if ( s[9] == 'I' || s[9] == 'i' ) {
                      if ( s[10] == 'M' || s[10] == 'm' ) {
                        if ( s[11] == 'E' || s[11] == 'e' ) {
                          if ( s[12] == 'S' || s[12] == 's' ) {
                            if ( s[13] == 'T' || s[13] == 't' ) {
                              if ( s[14] == 'A' || s[14] == 'a' ) {
                                if ( s[15] == 'M' || s[15] == 'm' ) {
                                  if ( s[16] == 'P' || s[16] == 'p' ) {
                                    tk.kf.keytype = 1;
                                    tk.kf.keyword1 = 1;
                                    tk.kf.keyid = Postgres::PostgresParser::token::T_CURRENT_TIMESTAMP;
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

void sqlClassify( const char *s, int l, Sql::Token &tk )
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

PostgresParserEngine::PostgresParserEngine()
    : IParserEngine()
{
}

QList<Sql::Token> PostgresParserEngine::parseTextLine( const QString &text, int state )
{
    QList<Sql::Token> tokens;

    const QByteArray bytes = text.toLatin1();
    const char *firstChar = bytes.constData();
    const char *lastChar = firstChar + bytes.size();

    Sql::Lexer lex(firstChar, lastChar, sqlClassify);
    lex.setScanKeywords(true);

    if (state != -1)
        lex.setState(state & 0xff);

    unsigned prevKind = Sql::T_EOF_SYMBOL;
    for (;;) {
        Sql::Token tk;
        lex(&tk, prevKind);
        if (tk.is(Sql::T_EOF_SYMBOL)) {
            break;
        }

        tokens.append(tk);
        prevKind = tk.kind();
    }

    _lastState = lex.state();

    return tokens;
}

const char *PostgresParserEngine::name( const Sql::Token &tk ) const
{
    return tk.spell();
}

Sql::ISemanticParser *PostgresParserEngine::createSemanticParser( Sql::TranslationUnit *unit ) const
{
    return new PostgresSemanticParser(unit);
}

Sql::ClassifyFunc PostgresParserEngine::keyworClassifyFunction()
{
    return sqlClassify;
}

bool PostgresParserEngine::hasFutureNullIdent() const
{
    return false;
}

} // Postgres
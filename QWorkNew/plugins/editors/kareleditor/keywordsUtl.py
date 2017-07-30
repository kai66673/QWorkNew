import os
import shutil
from collections import defaultdict

namespace = 'Karel'

root_dir = 'C:/Alex/LexerGenerator'
if not os.path.exists(root_dir):
    os.makedirs(root_dir)

work_dir = root_dir + '/' + namespace
if os.path.exists(work_dir):
    shutil.rmtree(work_dir)
os.makedirs(work_dir)

# 1. builtin functions
# 2. keywords
# 3. builtin types
# 4. directives
# 5. system variables and constants
keywords = [
    ("ABORT_TASK", 1),
    ("ABS", 1),
    ("ACOS", 1),
    ("ACT_SCREEN", 1),
    ("ADD_BYNAMEPC", 1),
    ("ADD_DICT", 1),
    ("ADD_INTPC", 1),
    ("ADD_REALPC", 1),
    ("ADD_STRINGPC", 1),
    ("APPEND_NODE", 1),
    ("APPEND_QUEUE", 1),
    ("APPROACH", 1),
    ("ARRAY_LEN", 1),
    ("ASIN", 1),
    ("ATAN2", 1),
    ("ATT_WINDOW_D", 1),
    ("ATT_WINDOW_S", 1),
    ("AVL_POS_NUM", 1),
    ("BYNAME", 1),
    ("BYTES_AHEAD", 1),
    ("BYTES_LEFT", 1),
    ("CALL_PROG", 1),
    ("CALL_PROGLIN", 1),
    ("CHECK_DICT", 1),
    ("CHECK_EPOS", 1),
    ("CHR", 1),
    ("CLEAR", 1),
    ("CLEAR_SEMA", 1),
    ("CLOSE_TPE", 1),
    ("CLR_IO_STAT", 1),
    ("CLR_PORT_SIM", 1),
    ("CLR_POS_REG", 1),
    ("CNC_DYN_DISB", 1),
    ("CNC_DYN_DISE", 1),
    ("CNC_DYN_DISI", 1),
    ("CNC_DYN_DISP", 1),
    ("CNC_DYN_DISR", 1),
    ("CNC_DYN_DISS", 1),
    ("CNCL_STP_MTN", 1),
    ("CNV_CONF_STR", 1),
    ("CNV_INT_STR", 1),
    ("CNV_JPOS_REL", 1),
    ("CNV_REAL_STR", 1),
    ("CNV_REL_JPOS", 1),
    ("CNV_STR_CONF", 1),
    ("CNV_STR_INT", 1),
    ("CNV_STR_REAL", 1),
    ("CNV_STR_TIME", 1),
    ("CNV_TIME_STR", 1),
    ("CONT_TASK", 1),
    ("COPY_FILE", 1),
    ("COPY_QUEUE", 1),
    ("COPY_TPE", 1),
    ("COS", 1),
    ("CREATE_TPE", 1),
    ("CREATE_VAR", 1),
    ("CURJPOS", 1),
    ("CURPOS", 1),
    ("CURR_PROG", 1),
    ("DEF_SCREEN", 1),
    ("DEF_WINDOW", 1),
    ("DEL_INST_TPE", 1),
    ("DELETE_FILE", 1),
    ("DELETE_NODE", 1),
    ("DELETE_QUEUE", 1),
    ("DET_WINDOW", 1),
    ("DISCTRL_ALPH", 1),
    ("DISCTRL_FORM", 1),
    ("DISCTRL_LIST", 1),
    ("DISMOUNT_DEV", 1),
    ("ERR_DATA", 1),
    ("EXP", 1),
    ("FILE_LIST", 1),
    ("FORCE_SPMENU", 1),
    ("FORMAT_DEV", 1),
    ("FRAME", 1),
    ("GET_ATTR_PRG", 1),
    ("GET_FILE_POS", 1),
    ("GET_JPOS_REG", 1),
    ("GET_JPOS_TPE", 1),
    ("GET_PORT_ASG", 1),
    ("GET_PORT_ATR", 1),
    ("GET_PORT_CMT", 1),
    ("GET_PORT_MOD", 1),
    ("GET_PORT_SIM", 1),
    ("GET_PORT_VAL", 1),
    ("GET_POS_FRM", 1),
    ("GET_POS_REG", 1),
    ("GET_POS_TPE", 1),
    ("GET_POS_TYP", 1),
    ("GET_QUEUE", 1),
    ("GET_REG", 1),
    ("GET_TIME", 1),
    ("GET_TSK_INFO", 1),
    ("GET_VAR", 1),
    ("IN_RANGE", 1),
    ("INDEX", 1),
    ("INI_DYN_DISB", 1),
    ("INI_DYN_DISE", 1),
    ("INI_DYN_DISI", 1),
    ("INI_DYN_DISP", 1),
    ("INI_DYN_DISR", 1),
    ("INI_DYN_DISS", 1),
    ("INIT_QUEUE", 1),
    ("INSERT_NODE", 1),
    ("INSERT_QUEUE", 1),
    ("INV", 1),
    ("IO_MOD_TYPE", 1),
    ("IO_STATUS", 1),
    ("J_IN_RANGE", 1),
    ("KLC", 1),
    ("KLC_NO_WAIT", 1),
    ("KLC_STATUS", 1),
    ("LN", 1),
    ("LOAD", 1),
    ("LOAD_STATUS", 1),
    ("LOCK_GROUP", 1),
    ("MIRROR", 1),
    ("MODIFY_QUEUE", 1),
    ("MOTION_CTL", 1),
    ("MOUNT_DEV", 1),
    ("MOVE_FILE", 1),
    ("NODE_SIZE", 1),
    ("OPEN_TPE", 1),
    ("ORD", 1),
    ("ORIENT", 1),
    ("PATH_LEN", 1),
    ("PAUSE_TASK", 1),
    ("PEND_SEMA", 1),
    ("POP_KEY_RD", 1),
    ("POS", 1),
    ("POS_REG_TYPE", 1),
    ("POST_ERR", 1),
    ("POST_SEMA", 1),
    ("PRINT_FILE", 1),
    ("PROG_LIST", 1),
    ("PURGE_DEV", 1),
    ("PUSH_KEY_RD", 1),
    ("READ_DICT", 1),
    ("READ_DICT_V", 1),
    ("READ_KB", 1),
    ("REMOVE_DICT", 1),
    ("RENAME_FILE", 1),
    ("RENAME_VAR", 1),
    ("RENAME_VARS", 1),
    ("RESET", 1),
    ("ROUND", 1),
    ("RUN_TASK", 1),
    ("SAVE", 1),
    ("SELECT_TPE", 1),
    ("SEMA_COUNT", 1),
    ("SEND_DATAPC", 1),
    ("SEND_EVENTPC", 1),
    ("SET_ATTR_PRG", 1),
    ("SET_CURSOR", 1),
    ("SET_EPOS_REG", 1),
    ("SET_EPOS_TPE", 1),
    ("SET_FILE_ATR", 1),
    ("SET_FILE_POS", 1),
    ("SET_INT_REG", 1),
    ("SET_JPOS_REG", 1),
    ("SET_JPOS_TPE", 1),
    ("SET_LANG", 1),
    ("SET_PERCH", 1),
    ("SET_PORT_ASG", 1),
    ("SET_PORT_ATR", 1),
    ("SET_PORT_CMT", 1),
    ("SET_PORT_MOD", 1),
    ("SET_PORT_SIM", 1),
    ("SET_PORT_VAL", 1),
    ("SET_POS_REG", 1),
    ("SET_POS_TPE", 1),
    ("SET_REAL_REG", 1),
    ("SET_TIME", 1),
    ("SET_TSK_ATTR", 1),
    ("SET_TSK_NAME", 1),
    ("SET_VAR", 1),
    ("SIN", 1),
    ("SQRT", 1),
    ("STR_LEN", 1),
    ("SUB_STR", 1),
    ("TAN", 1),
    ("TRANSLATE", 1),
    ("TRUNC", 1),
    ("UNINIT", 1),
    ("UNLOCK_GROUP", 1),
    ("UNPOS", 1),
    ("VAR_INFO", 1),
    ("VAR_LIST", 1),
    ("VOL_SPACE", 1),
    ("WRITE_DICT", 1),
    ("WRITE_DICT_V", 1),
    ("ABORT", 2),
    ("ABOUT", 2),
    ("ALONG", 2),
    ("AND", 2),
    ("ARRAY", 2),
    ("ATTACH", 2),
    ("AWAY", 2),
    ("AXIS", 2),
    ("BEGIN", 2),
    ("CANCEL", 2),
    ("CASE", 2),
    ("CLOSE", 2),
    ("CONDITION", 2),
    ("CONNECT", 2),
    ("CONST", 2),
    ("CONTINUE", 2),
    ("DELAY", 2),
    ("DISABLE", 2),
    ("DISCONNECT", 2),
    ("DIV", 2),
    ("DO", 2),
    ("DOWNTO", 2),
    ("ELSE", 2),
    ("ENABLE", 2),
    ("END", 2),
    ("ENDCONDITION", 2),
    ("ENDFOR", 2),
    ("ENDFOR", 2),
    ("ENDIF", 2),
    ("ENDSELECT", 2),
    ("ENDSTRUCTURE", 2),
    ("ENDUSING", 2),
    ("ENDWHILE", 2),
    ("EVAL", 2),
    ("EVENT", 2),
    ("FILE", 2),
    ("FOR", 2),
    ("FROM", 2),
    ("GO", 2),
    ("HAND", 2),
    ("HOLD", 2),
    ("IF", 2),
    ("IN", 2),
    ("MOD", 2),
    ("MOVE", 2),
    ("NOT", 2),
    ("NEAR", 2),
    ("NOABORT", 2),
    ("NOMESSAGE", 2),
    ("NOPAUSE", 2),
    ("NOWAIT", 2),
    ("OF", 2),
    ("OPEN", 2),
    ("OR", 2),
    ("PAUSE", 2),
    ("PROGRAM", 2),
    ("PULSE", 2),
    ("PURGE", 2),
    ("READ", 2),
    ("RELATIVE", 2),
    ("RELAX", 2),
    ("RELEASE", 2),
    ("REPEAT", 2),
    ("RESUME", 2),
    ("RETURN", 2),
    ("ROUTINE", 2),
    ("SELECT", 2),
    ("SIGNAL", 2),
    ("STOP", 2),
    ("THEN", 2),
    ("TIMER", 2),
    ("TO", 2),
    ("UNHOLD", 2),
    ("UNPAUSE", 2),
    ("UNTIL", 2),
    ("USING", 2),
    ("VIA", 2),
    ("WAIT", 2),
    ("WHEN", 2),
    ("WHILE", 2),
    ("WITH", 2),
    ("WRITE", 2),
    ("ABORT", 2),
    ("ABOUT", 2),
    ("ALONG", 2),
    ("AND", 2),
    ("ARRAY", 2),
    ("ATTACH", 2),
    ("AWAY", 2),
    ("AXIS", 2),
    ("BEGIN", 2),
    ("CANCEL", 2),
    ("CLOSE", 2),
    ("CONDITION", 2),
    ("CONNECT", 2),
    ("CONTINUE", 2),
    ("DELAY", 2),
    ("DISABLE", 2),
    ("DISCONNECT", 2),
    ("DIV", 2),
    ("DO", 2),
    ("DOWNTO", 2),
    ("ENABLE", 2),
    ("END", 2),
    ("ENDCONDITION", 2),
    ("ENDFOR", 2),
    ("ENDFOR", 2),
    ("ENDIF", 2),
    ("ENDSELECT", 2),
    ("ENDUSING", 2),
    ("ENDWHILE", 2),
    ("EVAL", 2),
    ("EVENT", 2),
    ("FILE", 2),
    ("FOR", 2),
    ("FROM", 2),
    ("GO", 2),
    ("HAND", 2),
    ("HOLD", 2),
    ("IF", 2),
    ("IN", 2),
    ("MOD", 2),
    ("MOVE", 2),
    ("NOT", 2),
    ("NEAR", 2),
    ("NOABORT", 2),
    ("NOMESSAGE", 2),
    ("NOPAUSE", 2),
    ("NOWAIT", 2),
    ("OPEN", 2),
    ("OR", 2),
    ("PAUSE", 2),
    ("PROGRAM", 2),
    ("PULSE", 2),
    ("PURGE", 2),
    ("READ", 2),
    ("RELATIVE", 2),
    ("RELAX", 2),
    ("RELEASE", 2),
    ("REPEAT", 2),
    ("RESUME", 2),
    ("RETURN", 2),
    ("ROUTINE", 2),
    ("SELECT", 2),
    ("SIGNAL", 2),
    ("STOP", 2),
    ("STRUCTURE", 2),
    ("TIMER", 2),
    ("TO", 2),
    ("TYPE", 2),
    ("UNHOLD", 2),
    ("UNPAUSE", 2),
    ("UNTIL", 2),
    ("USING", 2),
    ("VAR", 2),
    ("VIA", 2),
    ("WAIT", 2),
    ("WHEN", 2),
    ("WHILE", 2),
    ("WITH", 2),
    ("WRITE", 2),
    ("BOOLEAN", 3),
    ("BYTE", 3),
    ("COMMON_ASSOC", 3),
    ("CONFIG", 3),
    ("DISP_DAT_T", 3),
    ("GROUP_ASSOC", 3),
    ("INTEGER", 3),
    ("JOINTPOS", 3),
    ("JOINTPOS1", 3),
    ("JOINTPOS2", 3),
    ("JOINTPOS3", 3),
    ("JOINTPOS4", 3),
    ("JOINTPOS5", 3),
    ("JOINTPOS6", 3),
    ("JOINTPOS7", 3),
    ("JOINTPOS8", 3),
    ("JOINTPOS9", 3),
    ("PATH", 3),
    ("POSITION", 3),
    ("QUEUE_TYPE", 3),
    ("REAL", 3),
    ("SEMAPHORE", 3),
    ("SHORT", 3),
    ("STD_PTH_NODE", 3),
    ("STRING", 3),
    ("VECTOR", 3),
    ("VIS_PROCESS", 3),
    ("CAM_SETUP", 3),
    ("MODEL", 3),
    ("XYZWPR", 3),
    ("XYZWPREXT", 3),
    ("%ALPHABETIZE", 4),
    ("%CMOSVARS", 4),
    ("%COMMENT", 4),
    ("%CRTDEVICE", 4),
    ("%DEFGROUP", 4),
    ("%DELAY", 4),
    ("%ENVIRONMENT", 4),
    ("%INCLUDE", 4),
    ("%LOCKGROUP", 4),
    ("%NOABORT", 4),
    ("%NOBUSYLAMP", 4),
    ("%NOLOCKGROUP", 4),
    ("%NOPAUSE", 4),
    ("%NOPAUSESHFT", 4),
    ("%PRIORITY", 4),
    ("%STACKSIZE", 4),
    ("%TIMESLICE", 4),
    ("%TPMOTION", 4),
    ("$DECELTOL", 5),
    ("$GENOVERRIDE", 5),
    ("$GROUP", 5),
    ("$MCR", 5),
    ("$MNUFRAME", 5),
    ("$MNUFRAMENUM", 5),
    ("$MNUTOOL", 5),
    ("$MNUTOOLNUM", 5),
    ("$MOR_GRP", 5),
    ("$MOTYPE", 5),
    ("$NILP", 5),
    ("$ROTSPEED", 5),
    ("$SPEED", 5),
    ("$TERMTYPE", 5),
    ("$UFRAME", 5),
    ("$UTOOL", 5),
    ("AESWORLD", 5),
    ("AIN", 5),
    ("AOUT", 5),
    ("CIRCULAR", 5),
    ("COARSE", 5),
    ("CR", 5),
    ("CRTERROR", 5),
    ("CRTFUNC", 5),
    ("CRTPROMPT", 5),
    ("CRTSTATUS", 5),
    ("CMOS", 5),
    ("DRAM", 5),
    ("DIN", 5),
    ("DOUT", 5),
    ("FALSE", 5),
    ("FINE", 5),
    ("GIN", 5),
    ("GOUT", 5),
    ("INPUT", 5),
    ("JOINT", 5),
    ("LINEAR", 5),
    ("MAXINT", 5),
    ("MININT", 5),
    ("NODECEL", 5),
    ("NOSETTLE", 5),
    ("OFF", 5),
    ("ON", 5),
    ("OPIN", 5),
    ("OPOUT", 5),
    ("OUTPUT", 5),
    ("PORT_ID", 5),
    ("RDI", 5),
    ("RDO", 5),
    ("RSWORLD", 5),
    ("STATUS", 5),
    ("TPDISPLAY", 5),
    ("TPERROR", 5),
    ("TPFUNC", 5),
    ("TPIN", 5),
    ("TPOUT", 5),
    ("TPPROMPT", 5),
    ("TPSTATUS", 5),
    ("TRUE", 5),
    ("VARDECEL", 5),
    ("VIS_MONITOR", 5),
    ("WDI", 5),
    ("WDOUT", 5),
    ("WRISTJOINT", 5)
]

def writeBuiltings():
    with open(work_dir + '/' + namespace + 'CompletionFragment.cpp', 'w') as fc:
        for k in keywords:
            if k[1] == 5:
                fc.write('addCompletionItem(QStringLiteral("{0}"), KarelCompletionIcons::icon(KarelCompletionIcons::Keyword));\n'.format(k[0]))
            elif k[1] in (1, 2):
                fc.write('addCompletionItem(QStringLiteral("{0}"), KarelCompletionIcons::icon(KarelCompletionIcons::Function));\n'.format(k[0]))

def writeEngineSource():
    keywords.sort(key=lambda k: k[0])
    keywords.sort(key=lambda k: len(k[0]))

    keywords_map = defaultdict(list)
    for k in keywords:
        keywords_map[len(k[0])].append(k)

    te_list = []
    # Write Parser-Engine Header
    # with open(work_dir + '/' + namespace + 'ParserEngine.h', 'w') as feh:
    #    feh.write(engine_header_content)

    # Write Keywords Source
    with open(work_dir + '/' + namespace + 'Keywords.cpp', 'w') as fes:
      fes.write('#include "{0}Keywords.h"\n\nnamespace {0} {{\n\n'.format(namespace))

      for l in keywords_map.keys():
          fes.write('static inline void keywordClassify' + str(l) + '( const char *s, Token &tk )\n')
          fes.write('{\n')

          s = keywords_map[l][0][0]
          level = 1
          lines = []
          spaces = ''
          for c in s:
              spaces = ' ' * (level * 2)
              if c.lower() != c:
                  lines.append(spaces + "if ( s[{0}] == '{1}' || s[{0}] == '{2}' ) {{\n".format(level - 1, c, c.lower()))
              else:
                  lines.append(spaces + "if ( s[{0}] == '{1}' ) {{\n".format(level - 1, c))
              level += 1
          spaces = ' ' * (level * 2)
          strTypeId = str(keywords_map[l][0][1])
          lines.append(spaces + 'tk.kf.keytype = ' + strTypeId + ';\n')
          lines.append(spaces + 'tk.kf.keyword' + strTypeId + ' = 1;\n')
          te = 'T_'
          if s[0] == '%':
              te += '_' + s[1:]
          else:
              te += s
          lines.append(spaces + 'tk.kf.keyid = ' + te + ';\n')
          te_list.append(te)
          while level > 1:
              level -= 1
              lines.append(' ' * (level * 2) + "}\n")

          sprev = s
          wi = 1
          while len(keywords_map[l]) > wi:
              s = keywords_map[l][wi][0]
              sl = 1
              if s == sprev:
                  wi += 1
                  continue
              while s[sl - 1] == sprev[sl - 1]:
                  sl += 1
              needElse = True
              ind = len(lines) - sl + 1
              level = sl
              while len(s) >= level:
                  c = s[level - 1]
                  elsestr = ""
                  if needElse:
                      elsestr = "else "
                      needElse = False
                  spaces = ' ' * (level * 2)
                  if c.lower() != c:
                      lines.insert(ind, spaces + elsestr + "if ( s[{0}] == '{1}' || s[{0}] == '{2}' ) {{\n".format(level - 1, c, c.lower()))
                  else:
                      lines.insert(ind, spaces + elsestr + "if ( s[{0}] == '{1}' ) {{\n".format(level - 1, c))
                  ind += 1
                  level += 1
              strTypeId = str(keywords_map[l][wi][1])
              spaces = ' ' * (level * 2)
              lines.insert(ind, spaces + 'tk.kf.keytype = ' + strTypeId + ';\n')
              ind += 1
              lines.insert(ind, spaces + 'tk.kf.keyword' + strTypeId + ' = 1;\n')
              te = 'T_'
              if s[0] == '%':
                  te += '_' + s[1:]
              else:
                  te += s
              ind += 1
              lines.insert(ind, spaces + 'tk.kf.keyid = ' + te + ';\n')
              ind += 1
              te_list.append(te)
              while level > sl:
                  level -= 1
                  lines.insert(ind, ' ' * (level * 2) + "}\n")
                  ind += 1
              wi += 1
              sprev = s

          for line in lines:
              fes.write(line)
          fes.write('}\n\n')

      fes.write('void keywordClassify( const char *s, int l, Token &tk )\n{\n')
      fes.write('  switch (l) {\n')
      for l in keywords_map.keys():
          fes.write('    case {0}: keywordClassify{0}(s, tk); break;\n'.format(l))
      fes.write('  }\n}\n\n')

      fes.write('void Keywords::classify(const char *s, int l, Token &tk)\n{ keywordClassify(s, l, tk); }\n\n')

      fes.write('static char *keywordNames[] = {\n    "",\n')
      for te in te_list[:-1]:
          fes.write('    "{0}",\n'.format(te[2:]))
      fes.write('    "{0}"\n'.format(te_list[-1][2:]))
      fes.write('};\n\n')
      fes.write('char *Keywords::name(int keyid)\n{ return keywordNames[keyid]; }\n\n')

      fes.write('} // ' + namespace + '\n')

    # Write Keywords Header
    with open(work_dir + '/' + namespace + 'Keywords.h', 'w') as feh:
        feh.write('#ifndef {0}_KEYWORDS_H\n'.format(namespace.upper()))
        feh.write('#define {0}_KEYWORDS_H\n\n'.format(namespace.upper()))

        feh.write('#include "{0}Token.h"\n\n'.format(namespace))

        feh.write('namespace {0} {{\n\n'.format(namespace))

        feh.write('enum KeywordKind {\n')
        feh.write('    {0} = 1,\n'.format(te_list[0]))
        for te in te_list[1:-1]:
            feh.write('    {0},\n'.format(te))
        feh.write('    {0}\n'.format(te_list[-1]))
        feh.write('};\n\n')

        feh.write('class Keywords {\n')
        feh.write('public:\n')
        feh.write('    static void classify(const char *s, int l, Token &tk);\n')
        feh.write('    static char *name(int keyid);\n')
        feh.write('};\n\n')

        feh.write('}} // namespace {0}\n\n'.format(namespace))

        feh.write('#endif // {0}_KEYWORDS_H\n'.format(namespace.upper()))

writeBuiltings()
writeEngineSource()

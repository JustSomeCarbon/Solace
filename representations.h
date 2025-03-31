#ifndef REP_H
#define REP_H

enum tokenCode {
  INTLIT=1001,
  CHARLIT,
  FLOATLIT,
  STRINGLIT,
  BOOLLIT,
  USERWORD
};

char reserved_words[][10] =
{
  "mod",
  "Main",
  "do",
  "end",
  "main",
  "int",
  "float",
  "char",
  "string",
  "bool"
};


#endif

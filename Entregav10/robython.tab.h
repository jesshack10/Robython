/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_ROBYTHON_TAB_H_INCLUDED
# define YY_YY_ROBYTHON_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    START = 258,
    FUNCTION = 259,
    VAR = 260,
    BOOL = 261,
    TEXT = 262,
    NUM = 263,
    TRUE = 264,
    FALSE = 265,
    PRINT = 266,
    IF = 267,
    ELSE = 268,
    FOR = 269,
    WHILE = 270,
    TEMPERATURE = 271,
    DISTANCE = 272,
    R_LIGHT = 273,
    L_LIGHT = 274,
    ON_LED = 275,
    OFF_LED = 276,
    FORWARD = 277,
    BACK = 278,
    LEFT = 279,
    RIGHT = 280,
    BUZZER = 281,
    RGB = 282,
    DO = 283,
    DO_P = 284,
    RE = 285,
    RE_P = 286,
    MI = 287,
    MI_P = 288,
    FA = 289,
    FA_P = 290,
    SOL = 291,
    SOL_P = 292,
    LA = 293,
    LA_P = 294,
    SI = 295,
    SI_P = 296,
    SAME = 297,
    EQUALS = 298,
    NO_EQUAL = 299,
    PCOMA = 300,
    AND = 301,
    OR = 302,
    ID = 303,
    STRING = 304,
    NUMBER = 305
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 347 "robython.y" /* yacc.c:1909  */

	float fval;
	bool bval;
	char *sval;

#line 111 "robython.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ROBYTHON_TAB_H_INCLUDED  */

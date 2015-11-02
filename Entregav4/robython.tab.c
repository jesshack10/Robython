/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 5 "robython.y" /* yacc.c:339  */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <queue>
#include <vector>
#include <list>
#include "Variable.hpp"
#include <iostream>

using namespace std; 

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin; 
extern int line_num; 
extern char* yytext; 

//Vector that contains elements of the class Variable
vector<Variable> tabVars; 

string nameID; //Guarda el nombre de variable encontrado
string nameFunct; //Guarda el nombre de funcion encontrada 

//Valores scope variables globales. Empieza en 1000 hasta 1999
int contGlobalNum = 1000; //Solo se podrán declarar 500 globales Num 
int contGlobalText = 1500; //Solo se podrán declarar 250 globales Text
int contGlobalBool = 1725; //Solo se podrán declarar 250 globales Bool

//Valores scope variables locales. Empieza en 2000 hasta 2999
int contLocalNum = 2000;	
int contLocalText = 2500;
int contLocalBool = 2725; 

//Valores de scope para parametros locales a funciones. Empiezan en 200 hasta 230. 
//En cada funcion solo se permiten como maximo 10 parametros de tipo num, 10 de tipo text y 10 de tipo bool. 
int contParNum=200;
int contParText=210;
int contParBool=220; 

//Banderas para inserción de variables
bool bnum=false;
bool btext=false;
bool bbool=false;
bool globalOrLocal;//Booleano para saber si estoy en global o local
//Bandera de ERROR
bool error=false;

//Bandera de Error por Maximo Parametros 
bool maxParError=false; 
//Bandera de Error por Maximo Globales y bandera de error por maximo locales.
bool maxGlobalError=false
bool maxLocalError=false; 

//Lista para ayudar a guardar las variables de cada función o globales
list<Variable> listVariables;
 
//Estructura para contener el nombre de función y su tabla de variables asociada. 
struct DirObject{
	string id;
	list<Variable> listaVar;
};

//Directorio de procedimientos
vector<DirObject> DirProc; 

Variable var; //Objeto auxiliar para crear las variable encontradas

int SemanticCube[3][3][12] = { {{0,0,0,0,0,2,2,2,2,0,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
					     				 {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, {-1,-1,-1,1,-1,2,2,2,2,1,-1,-1}, {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}},
					      				 {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,2,2,2,2,2,2,2}} };

//Aumenta el contador dependiendo del tipo de variables encontradas
//true global, false local
void addCounterOfKeyType(bool scope)
{
	if(scope)
	{
		if(bnum && (contGlobalNum<1500)) var.setKey(contGlobalNum++);
		else if(btext && (contGlobalText<1725)) var.setKey(contGlobalText++);
		else if(bbool && (contGlobalNum<2000)) var.setKey(contGlobalBool++);
		else {cout<<"¡Muchas variables declaradas!"<<endl; error=true; maxGlobalError=true;}
	}else{
		if(bnum && (contLocalNum<2500)) var.setKey(contLocalNum++);
		else if(btext && (contLocalText<2725)) var.setKey(contLocalText++);
		else if(bbool && (contLocalNum<3000)) var.setKey(contLocalBool++);
		else {cout<<"¡Muchas variables declaradas!"<<endl; error=true;maxLocalError=true;}
	}
}

void addCounterOfParameter()
{
	if (bnum && (contParNum<210))var.setKey(contParNum++);
	else if(btext && (contParText<220))var.setKey(contParText++);
	else if (bbool && (contParBool<230))var.setKey(contParBool++);
	else {cout << "¡Muchos parámetros declarados!" << endl; error=true; maxParError=true;}
}

//Funcion que valida que no exista ya el nombre de la funcion dentro del Directorio Procedimientos.
bool validateInsertFunc(string name_fun)
{
	bool insertarFunc = true; 
	{
		for(int c=0; c<DirProc.size(); c++)
		{
			if (name_fun == DirProc[c].id)
				insertarFunc = false; 
		}
	}
	return insertarFunc; 
}


//Funcion que valida que no exista el nombre de la variable dentro de la misma funcion. 
bool validateInsertVar(string name_var)
{	
	bool poderInsertar = true; 
	for (list<Variable>:: iterator it= listVariables.begin();it!=listVariables.end();it++)
	{
		if (name_var == (*it).getId())	
			poderInsertar = false; 
	}
	return poderInsertar; 
}

void yyerror (const char *s);

#line 195 "robython.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "robython.tab.h".  */
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
#line 134 "robython.y" /* yacc.c:355  */

	float fval;
	bool bval;
	char *sval;

#line 292 "robython.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ROBYTHON_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 307 "robython.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   195

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  99
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  201

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   305

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    55,     2,     2,
      57,    58,    53,    51,    61,    52,     2,    54,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    56,     2,
      63,     2,    62,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    59,     2,    60,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   200,   200,   201,   200,   224,   224,   224,   247,   251,
     255,   255,   271,   275,   276,   280,   298,   298,   317,   318,
     319,   323,   327,   328,   329,   330,   331,   332,   336,   340,
     344,   345,   346,   347,   348,   352,   356,   357,   358,   362,
     366,   367,   368,   372,   377,   378,   379,   383,   384,   389,
     390,   391,   392,   396,   397,   398,   399,   403,   407,   408,
     412,   416,   417,   421,   422,   426,   427,   428,   429,   430,
     431,   432,   433,   434,   435,   436,   437,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   458,   459,   463,   467,   471,   475,   476,   477,   478
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "START", "FUNCTION", "VAR", "BOOL",
  "TEXT", "NUM", "TRUE", "FALSE", "PRINT", "IF", "ELSE", "FOR", "WHILE",
  "TEMPERATURE", "DISTANCE", "R_LIGHT", "L_LIGHT", "ON_LED", "OFF_LED",
  "FORWARD", "BACK", "LEFT", "RIGHT", "BUZZER", "RGB", "DO", "DO_P", "RE",
  "RE_P", "MI", "MI_P", "FA", "FA_P", "SOL", "SOL_P", "LA", "LA_P", "SI",
  "SI_P", "SAME", "EQUALS", "NO_EQUAL", "PCOMA", "AND", "OR", "ID",
  "STRING", "NUMBER", "'+'", "'-'", "'*'", "'/'", "'%'", "':'", "'('",
  "')'", "'{'", "'}'", "','", "'>'", "'<'", "$accept", "programa", "$@1",
  "$@2", "functions", "$@3", "$@4", "vars_func", "par_func", "$@5", "vars",
  "def_names", "$@6", "tipo", "bloque", "estatuto", "asignacion",
  "expresion", "expr_1", "expb", "e_bool", "exp", "e_sum", "termino",
  "factor", "fact", "var_cte", "e_mult", "condicion", "con_else",
  "escritura", "esc_2", "esc_3", "robot", "note", "ciclos", "for1p",
  "for2p", "for3p", "op_for", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,    43,    45,    42,    47,    37,    58,    40,    41,   123,
     125,    44,    62,    60
};
# endif

#define YYPACT_NINF -122

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-122)))

#define YYTABLE_NINF -17

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      42,     9,    63,  -122,  -122,    11,    59,    33,  -122,  -122,
    -122,  -122,    18,    64,    28,    25,    23,    45,    44,    59,
    -122,    61,  -122,    18,  -122,    49,    50,    51,    53,    54,
      67,    69,    70,    71,    60,    65,    73,    74,    75,    76,
      66,    68,    81,    72,    61,    61,    61,    61,    61,  -122,
      33,    -2,     8,    78,     8,  -122,  -122,  -122,  -122,    77,
      79,  -122,  -122,  -122,  -122,    80,    83,     8,  -122,  -122,
    -122,  -122,  -122,  -122,    82,    86,  -122,  -122,  -122,  -122,
    -122,  -122,  -122,     8,  -122,   -38,   -32,    10,   -44,     3,
    -122,    84,    85,    87,    88,    90,    89,    91,    92,    93,
      94,    96,    97,  -122,    95,     8,     8,     8,     8,  -122,
       8,     8,  -122,     8,     8,  -122,     8,     8,     8,  -122,
    -122,    98,    -2,    45,   101,     8,    45,    99,   103,    62,
     102,  -122,    59,  -122,   112,  -122,  -122,  -122,  -122,  -122,
    -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,   115,
    -122,  -122,   113,  -122,  -122,  -122,  -122,  -122,  -122,  -122,
    -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,
     104,   100,   105,    45,    33,    45,  -122,   111,   118,   110,
    -122,    64,  -122,  -122,   121,   108,  -122,   109,  -122,   120,
      45,   124,   -16,  -122,  -122,  -122,  -122,  -122,  -122,   122,
    -122
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     1,     0,    14,     0,     3,    20,
      18,    19,     0,     8,    15,     0,     0,     0,     0,    14,
       5,    27,     4,     0,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,    27,    27,    27,    27,    17,
      12,     0,     0,     0,     0,    65,    66,    67,    68,     0,
       0,    71,    72,    73,    74,     0,     0,     0,    21,    23,
      24,    25,    26,    22,     0,     0,    51,    52,    49,    64,
      50,    47,    48,     0,    63,    34,    38,    42,    56,     0,
      46,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    10,     0,     0,     0,     0,     0,    29,
       0,     0,    35,     0,     0,    39,     0,     0,     0,    43,
      45,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    14,     6,     0,    44,    33,    32,    30,    31,
      36,    37,    40,    41,    53,    54,    55,    60,    62,    59,
      93,    94,     0,    91,    69,    70,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    12,     0,    57,     0,     0,     0,
       9,     8,    11,    58,     0,     0,    75,     0,     7,     0,
       0,     0,     0,    92,    76,    96,    97,    98,    99,     0,
      95
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -122,  -122,  -122,  -122,   -45,  -122,  -122,  -122,   -37,  -122,
     -19,   116,  -122,   131,  -121,   -25,  -122,   -51,  -122,   -77,
    -122,   -87,  -122,   -74,  -122,  -122,   106,  -122,  -122,  -122,
    -122,    48,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     5,    13,    17,    25,   173,   133,    74,   134,
       8,    15,    18,    75,    22,    43,    44,    84,   109,    85,
     112,    86,   115,    87,    88,    89,    90,   119,    45,   176,
      46,    91,    92,    47,   170,    48,    95,   152,   185,   199
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      24,    93,   149,    96,   105,   153,   106,    76,    77,   116,
     117,   118,    76,    77,   110,   111,   101,    76,    77,    69,
      70,    71,    72,    73,   107,   108,   142,   143,   136,   137,
     138,   139,   104,   140,   141,   195,   196,   197,   198,     9,
      10,    11,   144,   145,   146,     1,    78,    79,    80,    81,
      82,    78,   181,    80,   183,    83,    78,     3,    80,    81,
      82,   113,   114,     4,     7,    83,    14,     6,    16,   193,
      19,    20,    26,    27,   151,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,   -16,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,    21,    23,    50,    51,    52,    42,
      53,    54,    55,   172,    56,    57,    58,    59,    61,    62,
      63,    64,    60,    65,    67,    66,    94,    97,   175,    98,
      99,   124,    68,   100,   103,   125,   188,   182,    12,    49,
     102,   131,   121,   147,   154,   123,   122,   126,   155,   127,
     128,   150,   171,   135,   129,   130,   132,   174,   177,   184,
     187,   179,   178,   186,   189,   180,   190,   191,   192,   194,
     148,     0,   200,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   120
};

static const yytype_int16 yycheck[] =
{
      19,    52,   123,    54,    42,   126,    44,     9,    10,    53,
      54,    55,     9,    10,    46,    47,    67,     9,    10,    44,
      45,    46,    47,    48,    62,    63,   113,   114,   105,   106,
     107,   108,    83,   110,   111,    51,    52,    53,    54,     6,
       7,     8,   116,   117,   118,     3,    48,    49,    50,    51,
      52,    48,   173,    50,   175,    57,    48,    48,    50,    51,
      52,    51,    52,     0,     5,    57,    48,    56,     4,   190,
      45,    48,    11,    12,   125,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    61,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    59,    61,    57,    57,    57,    48,
      57,    57,    45,   132,    45,    45,    45,    57,    45,    45,
      45,    45,    57,    57,    43,    57,    48,    50,    13,    50,
      50,    43,    60,    50,    48,    45,   181,   174,     7,    23,
      58,    45,    58,    45,    45,    58,    61,    58,    45,    58,
      58,    50,    50,    58,    61,    61,    59,    45,    45,    48,
      50,    61,    58,    45,    43,    60,    58,    58,    48,    45,
     122,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    65,    48,     0,    66,    56,     5,    74,     6,
       7,     8,    77,    67,    48,    75,     4,    68,    76,    45,
      48,    59,    78,    61,    74,    69,    11,    12,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    48,    79,    80,    92,    94,    97,    99,    75,
      57,    57,    57,    57,    57,    45,    45,    45,    45,    57,
      57,    45,    45,    45,    45,    57,    57,    43,    60,    79,
      79,    79,    79,    79,    72,    77,     9,    10,    48,    49,
      50,    51,    52,    57,    81,    83,    85,    87,    88,    89,
      90,    95,    96,    81,    48,   100,    81,    50,    50,    50,
      50,    81,    58,    48,    81,    42,    44,    62,    63,    82,
      46,    47,    84,    51,    52,    86,    53,    54,    55,    91,
      90,    58,    61,    58,    43,    45,    58,    58,    58,    61,
      61,    45,    59,    71,    73,    58,    83,    83,    83,    83,
      83,    83,    85,    85,    87,    87,    87,    45,    95,    78,
      50,    81,   101,    78,    45,    45,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      98,    50,    74,    70,    45,    13,    93,    45,    58,    61,
      60,    78,    72,    78,    48,   102,    45,    50,    68,    43,
      58,    58,    48,    78,    45,    51,    52,    53,    54,   103,
      50
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    64,    66,    67,    65,    69,    70,    68,    68,    71,
      73,    72,    72,    74,    74,    75,    76,    75,    77,    77,
      77,    78,    79,    79,    79,    79,    79,    79,    80,    81,
      82,    82,    82,    82,    82,    83,    84,    84,    84,    85,
      86,    86,    86,    87,    88,    88,    88,    89,    89,    90,
      90,    90,    90,    91,    91,    91,    91,    92,    93,    93,
      94,    95,    95,    96,    96,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      98,    99,    99,   100,   101,   102,   103,   103,   103,   103
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     8,     0,     0,    10,     0,     3,
       0,     5,     0,     5,     0,     1,     0,     4,     1,     1,
       1,     3,     2,     2,     2,     2,     2,     0,     4,     2,
       2,     2,     2,     2,     0,     2,     2,     2,     0,     2,
       2,     2,     0,     2,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     0,     6,     2,     0,
       5,     1,     3,     1,     1,     2,     2,     2,     2,     5,
       5,     2,     2,     2,     2,     7,     9,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     9,     3,     1,     5,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 200 "robython.y" /* yacc.c:1646  */
    {globalOrLocal = true;}
#line 1526 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 201 "robython.y" /* yacc.c:1646  */
    {DirObject myDirObject;
							myDirObject.id = (yyvsp[-3].sval);
							myDirObject.listaVar = listVariables;
							DirProc.push_back(myDirObject);
							listVariables.clear();}
#line 1536 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 206 "robython.y" /* yacc.c:1646  */
    {
					//Imprimir resultados finales
					cout<<"PROGRAM WORKS PROPERLY"<<endl;
					for(int i=0; i<DirProc.size(); i++)
					{
						cout<<DirProc[i].id<<endl;
						while(!DirProc[i].listaVar.empty())
						{
							cout<<"\t";
							DirProc[i].listaVar.front().muestra();
							DirProc[i].listaVar.pop_front();
							cout<<endl;
						}
					}
				}
#line 1556 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 224 "robython.y" /* yacc.c:1646  */
    {globalOrLocal = false;}
#line 1562 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 224 "robython.y" /* yacc.c:1646  */
    {
																			nameFunct=(yyvsp[-5].sval);
																			bool insertF = true;
																			insertF=validateInsertFunc(nameFunct);
																			if (insertF==true)
																			{
																				DirObject myDirObject;
																				myDirObject.id = (yyvsp[-5].sval);
																				myDirObject.listaVar = listVariables;
																				DirProc.push_back(myDirObject);
																				listVariables.clear();
																			}																		else 
																			{
																				listVariables.clear(); 
																				cout << "ERROR FATAL: NO PUEDEN EXISTIR DOS FUNCIONES CON EL MISMO NOMBRE" << endl; 
																			}
																				contLocalNum = 2000;
																				contLocalText = 2500;
																				contLocalBool = 2725;
																				contParNum=200;
																				contParText=210;
																				contParBool=220;
												}
#line 1590 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 255 "robython.y" /* yacc.c:1646  */
    { 	//Llamada a funcion para verificar si el nombre de variable ya existe 
			nameID = (yyvsp[0].sval); 
			bool insert=true; 
			insert = validateInsertVar(nameID);
			if (insert == true )
			{
				var.setId((yyvsp[0].sval));
		 		addCounterOfParameter();
		 		if (maxParError==false)
		 			listVariables.push_back(var); cout<<"push"<<endl;
		 	}
		 	else 
		 	{	cout << "ERROR FATAL: NO ES POSIBLE DECLARAR VARIABLES CON EL MISMO NOMBRE DENTRO DE UNA FUNCION" << endl;
		 		error=true;
		 	}			 	 
		 	}
#line 1611 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 280 "robython.y" /* yacc.c:1646  */
    { 	//Llamada a funcion para verificar si el nombre de variable ya existe 
				nameID = (yyvsp[0].sval); 
				bool insert=true; 
				insert = validateInsertVar(nameID);
				if (insert == true )
				{
					var.setId((yyvsp[0].sval));
			 		addCounterOfKeyType(globalOrLocal);
			 		if (maxGlobalError==false && maxLocalError==false)
			 			listVariables.push_back(var), cout<<"push"<<endl;
			 	}
			 	else 
			 		{	cout << "ERROR FATAL: NO ES POSIBLE DECLARAR VARIABLES CON EL MISMO NOMBRE DENTRO DE UNA FUNCION" << endl;
			 			error=true;
			 		}
		 	}
#line 1632 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 298 "robython.y" /* yacc.c:1646  */
    {
				nameID = (yyvsp[0].sval); 
				bool insert=true; 
				insert = validateInsertVar(nameID);
				if (insert == true)
				{
					var.setId((yyvsp[0].sval));
			   		addCounterOfKeyType(globalOrLocal);
			   		if (maxGlobalError==false && maxLocalError==false)
			   			listVariables.push_back(var), cout<<"push"<<endl;
				}
				else 
					{	cout << "ERROR FATAL: NO ES POSIBLE DECLARAR VARIABLES CON EL MISMO NOMBRE DENTRO DE UNA FUNCION" << endl;
			 			error=true;
			 		}		
			}
#line 1653 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 317 "robython.y" /* yacc.c:1646  */
    {btext = true; bnum = false; bbool = false;}
#line 1659 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 318 "robython.y" /* yacc.c:1646  */
    {bnum = true; btext = false; bbool = false;}
#line 1665 "robython.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 319 "robython.y" /* yacc.c:1646  */
    {bbool = true; bnum = false; btext = false;}
#line 1671 "robython.tab.c" /* yacc.c:1646  */
    break;


#line 1675 "robython.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 481 "robython.y" /* yacc.c:1906  */


void yyerror (const char *s)
{
	printf ("Error en el parser: %s", s); 
}

int main()
{
	FILE *myfile = fopen("pruebaE", "r");
	if (!myfile)
	{
		printf("No se puede abrir el archivo\n");
		return -1;
	}
	yyin = myfile;
	do 
	{
		yyparse();

	} while (!feof(yyin)); 
	cout<<"Error:"<<error<<endl;
}

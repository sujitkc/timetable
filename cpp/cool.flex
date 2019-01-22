/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in {% %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

static int comment_level = 0;

enum CommentType
{
	eLineComment,
	eGenComment
};

static CommentType comment_type;

/*
 *  Add Your own definitions here
 */

%}

/*
 * Define names for regular expressions here.
 */
%s	STRING
%s	INVALIDSTRING
%s	COMMENT
%s	NORMAL

BEGN	[b|B][e|E][g|G][i|N][n|N]
CLASS	[c|C][l|L][a|A][s|S][s|S]
ELSE	[e|E][l|L][s|S][e|E]
END	[e|E][n|N][d|D]
FI	[f|F][i|I]
IF	[i|I][f|F]
IN	[i|I][n|N]
INHERITS	[i|I][n|N][h|H][e|E][r|R][i|I][t|T][s|S]
IS	[i|I][s|S]
LET	[l|L][e|E][t|T]
LOOP	[l|L][o|O][o|O][p|P]
POOL	[p|P][o|O][o|O][l|L]
THEN	[t|T][h|H][e|E][n|N]
WHILE	[w|W][h|H][i|I][l|L][e|E]
CASE	[c|C][a|A][s|S][e|E]
ESAC	[e|E][s|S][a|A][c|C]
OF	[o|O][f|F]
NEW	[n|N][e|E][w|W]

NOT	[n|N][o|O][t|T]
ISVOID	[i|I][s|S][v|V][o|O][i|I][d|D]

ASSIGN	<-
LE	<=
DARROW          =>
LINECOMMENT	--
STARTCOMMENT	\(\*
ENDCOMMENT	\*\)

WS       [ \t\f\r\v]
NL	[\n]
DIGIT    [0-9]
CAPLETTER	[A-Z]
SMALLLETTER	[a-z]
INT_CONST ({DIGIT}+)
BOOL_CONST	true|false
LETTER 	{SMALLLETTER}|{CAPLETTER} 
OBJECTID      {SMALLLETTER}(({DIGIT}|{LETTER})*(_)*)*
TYPEID      {CAPLETTER}(({DIGIT}|{LETTER})*(_)*)*
ERROR	.

%%

<INITIAL>{WS}    		{}
<INITIAL>"+"	{	return	'+';	}
<INITIAL>"/"	{	return 	'/';	}
<INITIAL>"-"	{	return	'-';	}
<INITIAL>"*"	{	return	'*';	}
<INITIAL>"="	{	return	'=';	}
<INITIAL>"<"	{	return	'<';	}
<INITIAL>"."	{	return	'.';	}
<INITIAL>"~"	{	return	'~';	}
<INITIAL>","	{	return	',';	}
<INITIAL>";"	{	return	';';	}
<INITIAL>":"	{	return	':';	}
<INITIAL>"("	{	return	'(';	}
<INITIAL>")"	{	return	')';	}
<INITIAL>"@"	{	return	'@';	}
<INITIAL>"\""	{	
			BEGIN(STRING);
			strcpy(string_buf, "");
			string_buf_ptr = string_buf;
		}

<INITIAL>{BEGN}		{ return (BEGN); }
<INITIAL>{CLASS}	{ return (CLASS); }
<INITIAL>{ELSE}		{ return (ELSE); }
<INITIAL>{END}		{ return (END); }
<INITIAL>{FI}		{ return (FI); }
<INITIAL>{IF}		{ return (IF); }
<INITIAL>{IN}		{ return (IN); }
<INITIAL>{INHERITS}	{ return (INHERITS); }
<INITIAL>{IS}		{ return (IS); }
<INITIAL>{LET}		{ return (LET); }
<INITIAL>{LOOP}		{ return (LOOP); }
<INITIAL>{POOL}		{ return (POOL); }
<INITIAL>{THEN}		{ return (THEN); }
<INITIAL>{WHILE}		{ return (WHILE); }
<INITIAL>{ASSIGN}	{ return (ASSIGN); }
<INITIAL>{CASE}		{ return (CASE); }
<INITIAL>{ESAC}		{ return (ESAC); }
<INITIAL>{OF}		{ return (OF); }
<INITIAL>{NEW}		{ return (NEW); }

<INITIAL>{TYPEID}	{
			cool_yylval.symbol = idtable.add_string(yytext);
			return (TYPEID); 
		}

<INITIAL>{BOOL_CONST}	{ 
			if(strcmp(yytext, "true") == 0)
			{
				cool_yylval.boolean = true;
			}
			else
			{
				cool_yylval.boolean = false;
			}
			return (BOOL_CONST); 
		}

<INITIAL>{INT_CONST}	{ 
			cool_yylval.symbol = inttable.add_string(yytext);
			return (INT_CONST); 
		}

<INITIAL>{OBJECTID}	{ 
			cool_yylval.symbol =	idtable.add_string(yytext);
			return (OBJECTID); 
		}

<INITIAL>{ERROR}		{ return (ERROR); }

<INITIAL>{NL}			{ curr_lineno++; }	

<INITIAL>{DARROW}		{ return (DARROW); }

<INITIAL>{LINECOMMENT}	{
				comment_type = eLineComment;
				BEGIN(COMMENT);
		}
				
<INITIAL>{STARTCOMMENT}	{
				comment_type = eGenComment;
				BEGIN(COMMENT);	
				comment_level ++;
			}

<INITIAL>{ENDCOMMENT}	{
				char * err_msg = "Unexpected end of comment!";
				cool_yylval.error_msg = err_msg;
				return ERROR;
			}
<INITIAL><<EOF>>		{ return 0; }

<STRING>"\\n"		{
				if(strlen(string_buf) + strlen(yytext) 
				> MAX_STR_CONST)
				{
					char * err_msg = "String constant too long";
					cool_yylval.error_msg = err_msg;
			BEGIN(INVALIDSTRING);
					return ERROR;

				}
				else
				{
					strcat(string_buf, "\n" );
				}

			}

<STRING>"\""	{	

			BEGIN(INITIAL);
			cool_yylval.symbol = stringtable.add_string(string_buf);
			string_buf_ptr = string_buf + strlen(string_buf);
			*string_buf_ptr = '\0';
			return (STR_CONST);
		}

<STRING>\n	{
			char * err_msg = "unescaped newline inside string";
			cool_yylval.error_msg = err_msg;
			BEGIN(INVALIDSTRING);
			return ERROR;
		}


<STRING>{ERROR}		{
				if(strlen(string_buf) + strlen(yytext) 
				> MAX_STR_CONST)
				{
					char * err_msg = "String constant too long";
					cool_yylval.error_msg = err_msg;
			BEGIN(INVALIDSTRING);
					return ERROR;

				}
				else
				{
					strcat(string_buf, yytext);
					//cout << yytext << endl;
				}

			}

<STRING><<EOF>>		{
				
				char * err_msg = "End of file within string";
				cool_yylval.error_msg = err_msg;
				BEGIN(INITIAL); /* otherwise goes into inf. loop	*/
				return ERROR;
			}
<INVALIDSTRING>"\""	{	

			BEGIN(INITIAL);
			cool_yylval.symbol = stringtable.add_string(string_buf);
			string_buf_ptr = string_buf + strlen(string_buf);
			*string_buf_ptr = '\0';
			return (STR_CONST);
		}

<INVALIDSTRING>{ERROR}	{
			}

<COMMENT>{NL}		{ 
				curr_lineno++; 
				if(comment_type == eLineComment)
				{
					BEGIN(INITIAL);
				}
			}
	
<COMMENT>{ENDCOMMENT}	{
				comment_level --;
				if(comment_level == 0)
				{
					BEGIN(INITIAL);
				}
			}


<COMMENT><<EOF>>		{
				
				char * err_msg = "End of file within comment";
				cool_yylval.error_msg = err_msg;
				if(comment_type == eLineComment)
				{
					BEGIN(INITIAL); /* otherwise goes into inf. loop	*/
					return 0;
				}
				BEGIN(INITIAL);
				return ERROR;
			}

<COMMENT>{ERROR}	{}
 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */


 /*
  *  String constants (C syntax, taken from lexdoc(1) )
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  *
  */


%%

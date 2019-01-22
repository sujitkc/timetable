/*
 *  The scanner definition for Scheduler.
 */

%{
#include <iostream>
#include <stdio.h>
#include "Global.h"

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

int curr_lineno;
int verbose_flag;
string input_string;
float input_number;
static int comment_level = 0;

enum CommentType
{
	eLineComment,
	eGenComment
};

static CommentType comment_type;

%}

/*
 * Define names for regular expressions here.
 */
%s	COMMENT
%s	NORMAL

LINECOMMENT	--
STARTCOMMENT	\(\*
ENDCOMMENT	\*\)

WS       [ \t\f\r\v\n]
NL	[\n]
DIGIT    [0-9]
LETTER	[a-zA-Z]
INT_CONST ({DIGIT}+)
FLOAT	({DIGIT}+)\.({DIGIT}+)
ID      (({DIGIT}|{LETTER})*(_)*)*
ERROR	.

%%

<INITIAL>{WS}    		{}
<INITIAL>"mon"	{	return MON;	}
<INITIAL>"tue"	{	return TUE;	}
<INITIAL>"wed"	{	return WED;	}
<INITIAL>"thu"	{	return THU;	}
<INITIAL>"fri"	{	return FRI;	}
<INITIAL>";"	{	return	';';	}
<INITIAL>":"	{	return	':';	}
<INITIAL>"-"	{	return '-';	}
<INITIAL>{FLOAT}	{	
				sscanf(yytext, "%f", &input_number);
				return NUMBER;	
			}
<INITIAL>{INT_CONST}	{	
				sscanf(yytext, "%f", &input_number);
				return NUMBER;	
			}
<INITIAL>{ID}	{	
			input_string = yytext;
			return ID;	
		}
<INITIAL>{ERROR}		{ return (ERROR); }

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
				return ERROR;
			}
<INITIAL><<EOF>>		{ return 0; }

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
				if(comment_type == eLineComment)
				{
					BEGIN(INITIAL); /* otherwise goes into inf. loop	*/
					return 0;
				}
				BEGIN(INITIAL);
				return ERROR;
			}

<COMMENT>{ERROR}	{}

%%

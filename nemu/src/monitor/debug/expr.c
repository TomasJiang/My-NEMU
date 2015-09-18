#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

typedef struct token {
	int type;
	char str[32];
} Token;

typedef struct IStack
{
	int topStack;
	uint32_t vals[100];
} IStack;

typedef struct TStack
{
	int topStack;
	Token tokens[100];
} TStack;

int prec(Token t);
int in2post(Token *postTokens);
uint32_t eval();
uint32_t regval(char *reg);
void eval_biop(IStack *pistack, int op);

enum {
	NOTYPE = 256, REG, HEX, INT, DEREF, EQ, NEQ, AND, OR, NOT
};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {
	{" +",	NOTYPE},				// spaces
	{"\\$[a-z]{3}", REG}, 			// register
	{"0x[a-fA-F0-9]+", HEX},		// hexadecimal
	{"[0-9]+", INT},				// integer
	{"\\+", '+'},					// plus
	{"\\-", '-'},					// minus
	{"\\*", '*'},					// mul
	{"\\/", '/'},					// divide
	{"\\(", '('},					// lparen
	{"\\)", ')'},					// rparen
	{"==", EQ},						// equal
	{"!=", NEQ},					// not equal
	{"&&", AND},					// and
	{"\\|\\|", OR},					// or
	{"!",  NOT}						// not
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				// Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;
				switch(rules[i].token_type) {
					case NOTYPE:
						break;
					case REG:
					case HEX:
					case INT:
						sprintf(tokens[nr_token].str, "%.*s", substr_len, substr_start);
					case '+':
					case '-':
					case '/':
					case EQ:
					case NEQ:
					case AND:
					case OR:
					case NOT:
					case '(':
					case ')':
						tokens[nr_token++].type = rules[i].token_type;
						break;
					case '*':
						{
							if(nr_token == 0)
								tokens[nr_token++].type = DEREF;
							else {
								int fop = tokens[nr_token-1].type;
								if(fop == REG || fop == HEX || fop == INT)
									tokens[nr_token++].type = '*';
								else
									tokens[nr_token++].type = DEREF;
							}
							break;
						}
					default:
						panic("please implement me");
						break;
				}
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	uint32_t result = eval();
	*success = true;
	printf("0x%08x\n", result);

	return 0;
}

void pushi(IStack *pistack, uint32_t t)
{
	pistack->vals[pistack->topStack++] = t;
}

uint32_t popi(IStack *pistack)
{
	return pistack->vals[--pistack->topStack];
}

uint32_t topi(IStack *pistack)
{
	return pistack->vals[pistack->topStack-1];
}

uint32_t eval()
{
    Token post[100];
	int len = in2post(post);
	
	IStack *pistack = (IStack *)malloc(sizeof(IStack));
	pistack->topStack = 0;
	uint32_t val = 0;
	int i;
	for(i = 0; i < len; ++i)
	{
		switch(post[i].type)
		{
			case REG:
				val = regval(post[i].str + 1);
				pushi(pistack, val);
				break;
			case HEX:
				sscanf(post[i].str, "%x", &val);
				pushi(pistack, val);
				break;
			case INT:
				sscanf(post[i].str, "%d", &val);
				pushi(pistack, val);
				break;
			case DEREF:
				val = swaddr_read(popi(pistack), 4);
				pushi(pistack, val);
				break;
			case '+':
			case '-':
			case '*':
			case '/':
			case EQ:
			case NEQ:
			case AND:
			case OR:
				eval_biop(pistack, post[i].type);
				break;
			case NOT:
				val = popi(pistack);
				pushi(pistack, !val);
				break;
		}	
	}
	uint32_t result = popi(pistack);
	if(pistack->topStack)
	{
		panic("Stack is not empty!");
		exit(-1);
	}
	free(pistack);
	return result;
}

uint32_t regval(char *reg)
{
	int i;
	for(i = 0; i < 8; ++i)
	{
		if(!strcmp(reg, regsl[i]))
		{
			return reg_l(i);
		}
		if(!strcmp(reg, regsw[i]))
		{
			return reg_w(i);
		}
		if(!strcmp(reg, regsb[i]))
		{
			return reg_b(i);
		}
	}

	panic("No such register!");
	return -1;
}

void eval_biop(IStack *pistack, int op)
{
	uint32_t rval = popi(pistack);
	uint32_t lval = popi(pistack);
	switch(op)
	{
		case '*':
			pushi(pistack, lval * rval);
			break;
		case '/':
			pushi(pistack, lval / rval);
			break;
		case '+':
			pushi(pistack, lval + rval);
			break;
		case '-':
			pushi(pistack, lval - rval);
			break;
		case EQ:
			pushi(pistack, lval == rval);
			break;
		case NEQ:
			pushi(pistack, lval != rval);
			break;
		case AND:
			pushi(pistack, lval && rval);
			break;
		case OR:
			pushi(pistack, lval || rval);
			break;
		default:
			panic("Wrong Type!");
			exit(-1);
	}
	return;
}
			
void pusht(TStack *ptstack, Token t)
{
	ptstack->tokens[ptstack->topStack++] = t;
}

Token popt(TStack *ptstack)
{
	return ptstack->tokens[--ptstack->topStack];
}

Token topt(TStack *ptstack)
{
	return ptstack->tokens[ptstack->topStack-1];
}

int in2post(Token *postTokens)
{
	TStack *ptstack = (TStack *)malloc(sizeof(TStack));
	ptstack->topStack = 0;

	int k = 0, i = 0;
	while(i < nr_token)
	{
		if(tokens[i].type == REG ||	tokens[i].type == HEX || 
				tokens[i].type == INT)
		{
			postTokens[k++] = tokens[i++];
		}
		else 
		{
			if(!ptstack->topStack) {
				pusht(ptstack, tokens[i++]);
			}
			else if(topt(ptstack).type == '(' && tokens[i].type == ')') {
				popt(ptstack);
				i++;
			}
			else if(prec(tokens[i]) > prec(topt(ptstack))) {
				pusht(ptstack, tokens[i++]);
			}
			else {
				postTokens[k++] = popt(ptstack);
			}
		}
	}

	while(ptstack->topStack)
	{
		postTokens[k++] = popt(ptstack);
	}

	free(ptstack);
	return k;
}

int prec(Token t)
{
	switch(t.type)
	{
		case '(':
			return 20;
		case DEREF:
			return 11;
		case '*':
		case '/':
			return 10;
		case '+':
		case '-':
			return 9;
		case NOT:
			return 8;
		case AND:
			return 7;
		case EQ:
		case NEQ:
			return 6;
		case OR:
			return 5;
		case ')':
			return -1;
		default:
			panic("Wrong Type!");
			exit(0);
	}
}

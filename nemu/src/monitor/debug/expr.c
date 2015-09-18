#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

// uint32_t eval(int p, int q);
typedef struct token {
	int type;
	char str[32];
} Token;

typedef struct IStack
{
	int topStack;
	int tokens[100];
} IStack;

typedef struct TStack
{
	int topStack;
	Token tokens[100];
} TStack;

bool check_parentheses(int p, int q);
int op_pos(int p, int q);
int prec(Token t);
int in2post(Token *postTokens);
uint32_t eval();
int regval(char *reg);
void eval_biop(IStack *pistack, int op);

enum {
	NOTYPE = 256, REG, HEX, INT, EQ, NEQ, AND, OR, NOT
};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

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

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case NOTYPE:
						break;
					case REG:
					case HEX:
					case INT:
						sprintf(tokens[nr_token].str, "%.*s", substr_len, substr_start);
					case '+':
					case '-':
					case '*':
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

	/* TODO: Insert codes to evaluate the expression. */

	uint32_t result = eval();
	*success = true;
	printf("%d\n", result);

	return 0;
}

/*
uint32_t eval(int p, int q)
{
	Log("p = %d, q = %d", p, q);
	if(p > q) {
		Assert(0, "function eval: p > q!");
	}
	else if (p == q) {
		Assert(tokens[p].type == INT, "function eval: not a INT!\n");
		return atoi(tokens[p].str);
	}
	else if(check_parentheses(p, q) == true) {
		return eval(p + 1, q - 1);
	}
	else {
		int op = op_pos(p, q);
		Log("op = %d", op);
		uint32_t val1 = eval(p, op - 1);
		uint32_t val2 = eval(op + 1, q);

		switch(tokens[op].type) {
			case '+':
				return val1 + val2;
			case '-':
				return val1 - val2;
			case '*':
				return val1 * val2;
			case '/':
				return val1 / val2;
			default:
				assert(0);
		}
	}
}
*/

void pushi(IStack *pistack, int t)
{
	pistack->tokens[pistack->topStack++] = t;
}

int popi(IStack *pistack)
{
	return pistack->tokens[--pistack->topStack];
}

int topi(IStack *pistack)
{
	return pistack->tokens[pistack->topStack-1];
}

uint32_t eval()
{
    Token post[100];
	int len = in2post(post);
	
	IStack *pistack = (IStack *)malloc(sizeof(IStack));
	int val = 0, i;
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
	int result = popi(pistack);
	if(pistack->topStack)
	{
		panic("Stack is not empty!");
		exit(-1);
	}
	free(pistack);
	return result;
}

int regval(char *reg)
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
	int rval = popi(pistack);
	int lval = popi(pistack);
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

int in2post(Token *postTokens) // tokens[0, nr_token-1]
{
	TStack *ptstack = (TStack *)malloc(sizeof(TStack));

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

bool check_parentheses(int p, int q)
{
	bool result = false;
	int i, parentNum = 0;
	if(tokens[p].type == '(')
		result = true;
	for(i = p; i <= q; ++i)
	{
		if(tokens[i].type == '(')
			++parentNum;
		if(tokens[i].type == ')')
			--parentNum;
		if(parentNum == 0 && i != q)
			result = false;
		Assert(parentNum >= 0, "The expression is not surrounded by a matched pair of parentheses\n");
	}

	Assert(parentNum == 0, "The expression is not surrounded by a matched pair of parentheses\n");
	return result;
}

int op_pos(int p, int q)
{
	bool hasCandidate = false;
	int candidate = -1, parentNum = 0, i;
	for(i = q; i >= p; --i) {
		switch(tokens[i].type)
		{
			case EQ:
			case NEQ:
			case AND:
			case OR:
			case '+':
			case '-':
				if(parentNum)
					break;
				return i;
			case '*':
			case '/':
				if(parentNum)
					break;
				if(!hasCandidate)
				{
					hasCandidate = true;
					candidate = i;
				}
				break;
			case '(':
				--parentNum;
				break;
			case ')':
				++parentNum;
				break;
			case INT:
				break;
			default:
				assert(0);
		}
	}
	if(hasCandidate)
		return candidate;
	else
		return -1;
}

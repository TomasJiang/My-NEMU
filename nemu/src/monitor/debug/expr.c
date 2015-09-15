#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

uint32_t eval(int p, int q);
bool check_parentheses(int p, int q);
int op_pos(int p, int q);

enum {
	NOTYPE = 256, EQ,

	/* TODO: Add more token types */
	INT,

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"[0-9]+", INT},				// integer
	{"\\+", '+'},					// plus
	{"\\-", '-'},					// minus
	{"\\*", '*'},					// mul
	{"\\/", '/'},					// divide
	{"\\(", '('},					// lparen
	{"\\)", ')'},					// rparen
	{"==", EQ}						// equal
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

typedef struct token {
	int type;
	char str[32];
} Token;

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
					case INT:
						Assert(substr_len < 32, "integer is too big!\n");
						sprintf(tokens[nr_token].str, "%.*s", substr_len, substr_start);
					case '+':
					case '-':
					case '*':
					case '/':
					case '(':
					case ')':
					case EQ:
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

	Log("nr_token = %d", nr_token);
	uint32_t result = eval(0, nr_token-1);
	*success = true;
	printf("%d\n", result);

	return 0;
}

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

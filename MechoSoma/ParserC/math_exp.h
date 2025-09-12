
#ifndef __MATHEXP_H__
#define __MATHEXP_H__

// Return values...
#define MT_EXP_OK			0
#define MT_EXP_PARSE_FAILED		1
#define MT_EXP_EVALUATION_FAILED	2
#define MT_EXP_BAD_OPERAND		3

int i_parseMathExpr(const char* expr,int& out);
int d_parseMathExpr(const char* expr,double& out);

#endif /* __MATHEXP_H__ */

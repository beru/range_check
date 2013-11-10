
#include "parser.h"
#include "util.h"
// ((1+3)*4)

struct Parser
{
private:
	template <typename T>
	T* newNode() {
		AnyNode* nd = nodes++;
		nd->type = T::NodeType;
		return (T*)nd;
	}
	BinaryNode* newBinaryNode(
		const Node* lhs,
		const Token* op,
		const Node* rhs
	) {
		BinaryNode* bn = newNode<BinaryNode>();
		bn->lhs = lhs;
		bn->op = op;
		bn->rhs = rhs;
		return bn;
	}
	TokenNode* newTokenNode(
		const Token* tk
	) {
		TokenNode* nd = newNode<TokenNode>();
		nd->token = tk;
		return nd;
	}
public:
	AnyNode* nodes;

	Parser() {}

	Node* expr(const Token*& ts) {
		Node* pRet = term(ts);
		while (in(ts->type, TokenType::Add, TokenType::Sub)) {
			pRet = newBinaryNode(pRet, ts, term(++ts));
		}
		return pRet;
	}
	Node* term(const Token*& ts) {
		Node* pRet = fact(ts);
		while (in(ts->type, TokenType::Mul, TokenType::Div)) {
			pRet = newBinaryNode(pRet, ts, fact(++ts));
		}
		return pRet;
	}
	Node* fact(const Token*& ts) {
		switch (ts->type) {
		case TokenType::IntegerConstant:
			return newTokenNode(ts++);
			break;
		case TokenType::LeftParentthesis:
			{
				++ts;
				Node* exp = expr(ts);
				if (ts->type != TokenType::RightParenthesis) {
					break;
				}
				++ts;
				return exp;
			}
			break;
		default:
			
			break;
		}
	}
};


size_t ParseTokens(
	const Token* ts, const Token* te,
	AnyNode* nodes,
	Node** expressions
	)
{
	Parser p;
	p.nodes = nodes;
	expressions[0] = p.expr(ts);
	return 1;
}



#include "parse.h"
#include "node.h"
#include "token.h"

#include "util.h"

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

	Node* stmt(const Token*& ts) {
		switch (ts->type) {
		case TokenType::S1:
		case TokenType::S2:
		case TokenType::S4:
		case TokenType::S8:
		case TokenType::U1:
		case TokenType::U2:
		case TokenType::U4:
		case TokenType::U8:
			return declare(ts);
			break;
		case TokenType::Identifier:
			switch ((ts+1)->type) {
			case TokenType::Increment:
			case TokenType::Decrement:
				return expr_stmt(ts);
			default:
				return assign(ts);
			}
			break;
		default:
			return expr_stmt(ts);
			break;
		case TokenType::EndOfString:
			return nullptr;
//			throw "";
			break;
		}
	}
	Node* expr_stmt(const Token*& ts) {
		while (ts->type == TokenType::Semicolon) {
			++ts;
		}
		Node* ret = expr(ts);
		if (ts->type != TokenType::Semicolon) {
			throw "semicolon needed";
		}
		++ts;
		return ret;
	}
	Node* expr(const Token*& ts) {
		return conditional(ts);
	}
	Node* declare(const Token*& ts) {
		DeclareNode* pRet = newNode<DeclareNode>();
		pRet->type = ts++;
		if (ts->type != TokenType::Identifier) {
			throw "identifier not found";
		}
		pRet->id = ts++;
		switch (ts->type) {
		case TokenType::Assign:
			pRet->rhs = expr_stmt(++ts);
			break;
		case TokenType::Semicolon:
			pRet->rhs = nullptr;
			break;
		default:
			throw "";
			break;
		}
		return pRet;
	}
	Node* assign(const Token*& ts) {
		AssignNode* pRet = newNode<AssignNode>();
		pRet->id = ts++;
		switch (ts->type) {
		case TokenType::Assign:
		case TokenType::AddAssign:
		case TokenType::SubAssign:
		case TokenType::MulAssign:
		case TokenType::DivAssign:
		case TokenType::ModAssign:
		case TokenType::LeftShiftAssign:
		case TokenType::RightShiftAssign:
		case TokenType::AndAssign:
		case TokenType::XorAssign:
		case TokenType::OrAssign:
			pRet->op = ts++;
			pRet->rhs = expr_stmt(ts);
			return pRet;
			break;
		default:
			throw "";
			break;
		}
	}
	Node* conditional(const Token*& ts) {
		Node* pLogicalOr = logicalOr(ts);
		if (ts->type == TokenType::Question) {
			++ts;
			Node* pTrue = expr(ts);
			if (ts->type == TokenType::Colon) {
				++ts;
				Node* pFalse = conditional(ts);
				ConditionalNode* pCon = newNode<ConditionalNode>();
				pCon->condition = pLogicalOr;
				pCon->trueNode = pTrue;
				pCon->falseNode = pFalse;
				return pCon;
			}else {
				throw "";
			}
		}else {
			return pLogicalOr;
		}
	}

	template <typename... Types>
	Node* makeNode(
		Node* (Parser::*func)(const Token*&),
		const Token*& ts,
		Types... tokenTypes
	) {
		Node* pRet = (this->*func)(ts);
		while (in(ts->type, tokenTypes...)) {
			Node* lhs = pRet;
			const Token* op = ts;
			Node* rhs = (this->*func)(++ts);
			pRet = newBinaryNode(lhs, op, rhs);
		}
		return pRet;
	}
	Node* logicalAnd(const Token*& ts) {
		return makeNode(&Parser::bitwiseOr, ts,
						TokenType::LogicalAnd);
	}
	Node* logicalOr(const Token*& ts) {
		return makeNode(&Parser::logicalAnd, ts,
						TokenType::LogicalOr);
	}
	Node* bitwiseOr(const Token*& ts) {
		return makeNode(&Parser::bitwiseXor, ts,
						TokenType::BitwiseOr);
	}
	Node* bitwiseXor(const Token*& ts) {
		return makeNode(&Parser::bitwiseAnd, ts,
						TokenType::BitwiseXor);
	}
	Node* bitwiseAnd(const Token*& ts) {
		return makeNode(&Parser::equality, ts,
						TokenType::BitwiseAnd);
	}
	Node* equality(const Token*& ts) {
		return makeNode(&Parser::relational, ts,
						TokenType::Equal,
						TokenType::NotEqual);
	}
	Node* relational(const Token*& ts) {
		return makeNode(&Parser::shift, ts,
						TokenType::GreaterThan,
						TokenType::GreaterThanOrEqual,
						TokenType::LessThan,
						TokenType::LessThanOrEqual
						);
	}
	Node* shift(const Token*& ts) {
		return makeNode(&Parser::additive, ts,
						TokenType::LeftShift,
						TokenType::RightShift
						);
	}
	Node* additive(const Token*& ts) {
		return makeNode(&Parser::mult, ts,
						TokenType::Add,
						TokenType::Sub
						);
	}
	Node* mult(const Token*& ts) {
		return makeNode(&Parser::unary, ts,
						TokenType::Mul,
						TokenType::Div,
						TokenType::Mod
						);
	}
	Node* unary(const Token*& ts) {
		switch (ts->type) {
		case TokenType::Add:
		case TokenType::Sub:
		case TokenType::Increment:
		case TokenType::Decrement:
			{
				UnaryNode* u = newNode<UnaryNode>();
				u->lhs = newTokenNode(ts++);
				u->rhs = unary(ts);
				return u;
			}
			break;
		default:
			return postfix(ts);
		}
	}

	Node* postfix(const Token*& ts) {
		Node* nd = primary(ts);
		if (in(ts->type, TokenType::Increment, TokenType::Decrement)) {
			UnaryNode* u = newNode<UnaryNode>();
			u->lhs = nd;
			u->rhs = newTokenNode(ts++);
			return u;
		}else {
			return nd;
		}
	}

	Node* primary(const Token*& ts) {
		switch (ts->type) {
		case TokenType::IntegerConstant:
		case TokenType::Identifier:
			return newTokenNode(ts++);
			break;
		case TokenType::LeftParentthesis:
			{
				Node* exp = expr(++ts);
				if (ts->type != TokenType::RightParenthesis) {
					throw "";
					break;
				}
				++ts;
				return exp;
			}
			break;
		default:
			throw "";
			break;
		}
	}
};

size_t Parse(
	const Token* ts, const Token* te,
	AnyNode* nodes,
	Node** expressions
	)
{
	Parser p;
	p.nodes = nodes;

	Node** start = expressions;
	while (Node* node = p.stmt(ts)) {
		*expressions++ = node;
	}
	return expressions - start;
}


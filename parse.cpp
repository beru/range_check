
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
			return declare_stmt(ts);
			break;
		case TokenType::Identifier:
			return assign_stmt(ts);
			break;
		default:
			throw "";
			break;
		}
	}
	Node* declare_stmt(const Token*& ts) {
		DeclareNode* pRet = newNode<DeclareNode>();
		pRet->type = ts++;
		if (ts->type != TokenType::Identifier) {
			throw "";
		}
		pRet->id = ts++;
		switch (ts->type) {
		case TokenType::Assign:
			pRet->rhs = expr(++ts);
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
	Node* assign_stmt(const Token*& ts) {
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
			pRet->rhs = expr(ts);
			return pRet;
			break;
		default:
			throw "";
			break;
		}
	}
	Node* expr(const Token*& ts) {
		return conditional(ts);
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

	template <TokenType tt>
	Node* makeNode(
		Node* (Parser::*func)(const Token*&),
		const Token*& ts
	) {
		Node* pRet = (this->*func)(ts);
		while (ts->type == tt) {
			Node* lhs = pRet;
			const Token* op = ts;
			Node* rhs = (this->*func)(++ts);
			pRet = newBinaryNode(lhs, op, rhs);
		}
		return pRet;
	}
	template <TokenType tt0, TokenType tt1>
	Node* makeNode(
		Node* (Parser::*func)(const Token*&),
		const Token*& ts
	) {
		Node* pRet = (this->*func)(ts);
		while (in(ts->type, tt0, tt1)) {
			Node* lhs = pRet;
			const Token* op = ts;
			Node* rhs = (this->*func)(++ts);
			pRet = newBinaryNode(lhs, op, rhs);
		}
		return pRet;
	}
	Node* logicalAnd(const Token*& ts) {
		return makeNode<TokenType::LogicalAnd>(&Parser::bitwiseOr, ts);
	}
	Node* logicalOr(const Token*& ts) {
		return makeNode<TokenType::LogicalOr>(&Parser::logicalAnd, ts);
	}
	Node* bitwiseOr(const Token*& ts) {
		return makeNode<TokenType::BitwiseOr>(&Parser::bitwiseXor, ts);
	}
	Node* bitwiseXor(const Token*& ts) {
		return makeNode<TokenType::BitwiseXor>(&Parser::bitwiseAnd, ts);
	}
	Node* bitwiseAnd(const Token*& ts) {
		return makeNode<TokenType::BitwiseAnd>(&Parser::equality, ts);
	}
	Node* equality(const Token*& ts) {
		return makeNode<TokenType::Equal, TokenType::NotEqual>(&Parser::relational, ts);
	}
	Node* relational(const Token*& ts) {
		Node* pRet = shift(ts);
		while (
			in(
				ts->type,
				TokenType::GreaterThan,
				TokenType::GreaterThanOrEqual,
				TokenType::LessThan,
				TokenType::LessThanOrEqual
			)
		) {
			Node* lhs = pRet;
			const Token* op = ts;
			Node* rhs = shift(++ts);
			pRet = newBinaryNode(lhs, op, rhs);
		}
		return pRet;
	}
	Node* shift(const Token*& ts) {
		return makeNode<TokenType::LeftShift, TokenType::RightShift>(&Parser::additive, ts);
	}
	Node* additive(const Token*& ts) {
		return makeNode<TokenType::Add, TokenType::Sub>(&Parser::mult, ts);
	}
	Node* mult(const Token*& ts) {
		Node* pRet = fact(ts);
		while (in(ts->type, TokenType::Mul, TokenType::Div, TokenType::Mod)) {
			Node* lhs = pRet;
			const Token* op = ts;
			Node* rhs = fact(++ts);
			pRet = newBinaryNode(lhs, op, rhs);
		}
		return pRet;
	}
	Node* fact(const Token*& ts) {
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
	expressions[0] = p.stmt(ts);
	return 1;
}


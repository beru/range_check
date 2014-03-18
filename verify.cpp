#include "verify.h"

#include <map>
#include <vector>
#include <string>
#include <stdint.h>

#include "node.h"
#include "token.h"
#include "value.h"

#include <boost/multiprecision/cpp_int.hpp>
#include "integer_literal_scan.h"
#include <limits>

struct Identifier
{
	bool isConstant;
	bool isPointer;
	bool isArray;
	bool isSigned;
	bool isFunction;
	unsigned char byteSize;

	Value value;

	const Token* typeToken;
	const Token* idToken;
};

template <typename... Args>
struct castInt;

template <typename First>
struct castInt<First>
{
	castInt(Value& v, boost::multiprecision::cpp_int& ival) {
		if (ival <= std::numeric_limits<First>::max()) {
			v = static_cast<First>(ival);
		}else {
			throw "integer literal too big.";
		}
	}
};

template <typename First, typename... Rest>
struct castInt<First, Rest...>
{
	castInt(Value& v, boost::multiprecision::cpp_int& ival) {
		if (ival <= std::numeric_limits<First>::max()) {
			v = static_cast<First>(ival);
		}else {
			castInt<Rest...> caster(v, ival);
		}
	}
};

Value::Type TokenTypeToValueType(TokenType tt)
{
	switch (tt) {
	case TokenType::S1:
		return Value::Type::sc;
	case TokenType::S2:
		return Value::Type::ss;
	case TokenType::S4:
		return Value::Type::si;
	case TokenType::S8:
		return Value::Type::sl;
	case TokenType::U1:
		return Value::Type::uc;
	case TokenType::U2:
		return Value::Type::us;
	case TokenType::U4:
		return Value::Type::ui;
	case TokenType::U8:
		return Value::Type::ul;
	default:
		return Value::Type::unknown;
	}
}

struct Verifier
{
	bool findId(const char* id) {
		if (ids.find(id) != ids.end()) {
			return true;
		}
		return false;
	}

	bool findId(const Token* id) {
		char buff[256];
		ExtractString(id, buff);
		return findId(buff);
	}



	Value Verify(const DeclareNode* node) {

		Identifier id;
		id.isConstant = false;
		id.isPointer = false;
		id.byteSize = 0;
		id.typeToken = node->type;
		id.idToken = node->id;
		id.value.ull = 0;
		// TODO: to support typedef
		id.value.type = TokenTypeToValueType(id.typeToken->type);
		char buff[256];
		ExtractString(node->id, buff);
		if (!findId(buff)) {
			ids[buff] = id;
		}else {
			printf("id overlap\n");
		}
		Value& lhs = getValue(node->id);
		if (node->rhs) {
			Value rhs = Verify(node->rhs);
			lhs.assign(rhs);
		}
		return lhs;
	}

	Value Verify(const AssignNode* node) {
		if (!findId(node->id)) {
			char buff[256];
			ExtractString(node->id, buff);
			printf("id %s not found\n", buff);
		}
		Value& lhs = getValue(node->id);
		Value rhs = Verify(node->rhs);
		switch (node->op->type) {
		case TokenType::Assign:				return lhs.assign(rhs);
		case TokenType::AddAssign:			return lhs += rhs;
		case TokenType::SubAssign:			return lhs -= rhs;
		case TokenType::MulAssign:			return lhs *= rhs;
		case TokenType::DivAssign:			return lhs /= rhs;
		case TokenType::RightShiftAssign:	return lhs >>= rhs;
		case TokenType::LeftShiftAssign:	return lhs <<= rhs;
		}
	}

	Value Verify(const BinaryNode* node) {
		Value lhs = Verify(node->lhs);
		Value rhs = Verify(node->rhs);
		switch (node->op->type) {
		case TokenType::Add:			return lhs + rhs;
		case TokenType::Sub:			return lhs - rhs;
		case TokenType::Mul:			return lhs * rhs;
		case TokenType::Div:			return lhs / rhs;
		case TokenType::RightShift:		return lhs >> rhs;
		case TokenType::LeftShift:		return lhs << rhs;
		}
	}

	Value Verify(const UnaryNode* node) {
		Value lhs = Verify(node->lhs);
		Value rhs = Verify(node->rhs);
		// TODO: rewrite..
		return lhs;
	}

	Value Verify(const CastNode* node) {
		Value ret = Verify(node->rhs);
		ret.Cast(TokenTypeToValueType(node->typeToken->type));
		return ret;
	}

	Value Verify(const TokenNode* node) {
		switch (node->token->type) {
		case TokenType::Identifier:
			if (!findId(node->token)) {
				char buff[256];
				ExtractString(node->token, buff);
				printf("id %s not found\n", buff);
			}
			return getValue(node->token);
			break;
		case TokenType::IntegerConstant:
			{
				char buff[256];
				const char* pstr = ExtractString(node->token, buff);
				const char* pstrOrg = pstr;
				BaseType base = ScanBase(pstr);
				int suffix = ScanSuffix(pstr);
				buff[pstr - pstrOrg] = 0;
				using namespace boost::multiprecision;
				cpp_int num(pstrOrg);
				Value v;
				switch (base) {
				case BaseType_Decimal:
					switch (suffix) {
					case Suffix_None:
						castInt<
							int,
							long,
							long long
						>(v, num);
						break;
					case Suffix_Unsigned:
						castInt<
							unsigned int,
							unsigned long,
							unsigned long long
						>(v, num);
						break;
					case Suffix_Long:
						castInt<
							long,
							long long
						>(v, num);
						break;
					case (Suffix_Unsigned | Suffix_Long):
						castInt<
							unsigned long,
							unsigned long long
						>(v, num);
						break;
					case Suffix_LongLong:
						castInt<
							long long
						>(v, num);
						break;
					case (Suffix_Unsigned | Suffix_LongLong):
						castInt<
							unsigned long long
						>(v, num);
						break;
					}
					break;
				case BaseType_Hexadecimal:
				case BaseType_Octal:
					switch (suffix) {
					case Suffix_None:
						castInt<
							int, unsigned int,
							long, unsigned long,
							long long, unsigned long long
						>(v, num);
						break;
					case Suffix_Unsigned:
						castInt<
							unsigned int,
							unsigned long,
							unsigned long long
						>(v, num);
						break;
					case Suffix_Long:
						castInt<
							long, unsigned long,
							long long, unsigned long long
						>(v, num);
						break;
					case (Suffix_Unsigned | Suffix_Long):
					case Suffix_LongLong:
						castInt<
							unsigned long,
							unsigned long long
						>(v, num);
						break;
					case (Suffix_Unsigned | Suffix_LongLong):
						castInt<
							unsigned long long
						>(v, num);
						break;
					}
					break;
				}
				return v;
			}
			break;
		}
	}

	Value Verify(const Node* node) {
		switch (node->nodeType) {
		case NodeType::Declare:	return Verify((DeclareNode*)node);
		case NodeType::Assign:	return Verify((AssignNode*)node);
		case NodeType::Binary:	return Verify((BinaryNode*)node);
		case NodeType::Unary:	return Verify((UnaryNode*)node);
		case NodeType::Token:	return Verify((TokenNode*)node);
		case NodeType::Cast:	return Verify((CastNode*)node);
		}
	}

	Value& getValue(const Token* id) {
		char buff[256];
		ExtractString(id, buff);
		Identifier& identifier = ids[buff];
		return identifier.value;
	}

	std::map<std::string, Identifier> ids;
};

void Verify(Node** nb, size_t cnt)
{
	Verifier v;
	for (size_t i=0; i<cnt; ++i) {
		Node* nd = nb[i];
		if (!nd) {
			break;
		}
		v.Verify(nd);
	}
	
}


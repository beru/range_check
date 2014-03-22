#!/usr/bin/env python

import sys
from pycparser.c_ast import *
from pycparser.c_parser import CParser, Coord, ParseError
from pycparser.c_lexer import CLexer

parser = CParser()

f = open('sample.c')
src = f.read()
ast = parser.parse(src)
#ast.show()

class Verifier(NodeVisitor):
	
	class Variable:
		def __init__(self, name, value = None):
			self.name = name
			self.value = value
		def __repr__(self):
			return "%s %s" % (self.name, self.value)
	
	# interval arithmetic
	class Value:
		def __init__(self, type, value = None):
			self.type = type
			self.upper = value
			self.lower = value
		def __repr__(self):
			return "%s [%s:%s]" % (self.type, self.lower, self.upper)
		
		def __add__(self, rhs):
			ret = Verifier.Value(self.type)
			ret.upper = self.upper - rhs.upper
			ret.lower = self.lower - rhs.lower
			return ret
		
		def __sub__(self, rhs):
			ret = Verifier.Value(self.type)
			ret.upper = self.upper - rhs.lower
			ret.lower = self.lower - rhs.upper
			return ret
		
		def __mul__(self, rhs):
			ret = Verifier.Value(self.type)
			lu = self.upper
			ll = self.lower
			ru = rhs.upper
			rl = rhs.lower
			luru = lu * ru
			lurl = lu * rl
			llru = ll * ru
			llrl = ll * rl
			ret.upper = max(luru, lurl, llru, llrl)
			ret.lower = min(luru, lurl, llru, llrl)
			return ret
		
		def __div__(self, rhs):
			ret = Verifier.Value(self.type)
			lu = self.upper
			ll = self.lower
			ru = rhs.upper
			rl = rhs.lower
			luru = lu / ru
			lurl = lu / rl
			llru = ll / ru
			llrl = ll / rl
			ret.upper = max(luru, lurl, llru, llrl)
			ret.lower = min(luru, lurl, llru, llrl)
			return ret
		
		def __eq__(self, rhs):
			ret = Verifier.Value(self.type)
			ret.lower = ret.upper = (self.upper == rhs.upper)
			return ret
		
		def __ne__(self, rhs):
			ret = Verifier.Value(self.type)
			ret.lower = ret.upper = (self.upper != rhs.upper)
			return ret
			
		
	def __init__(self):
		self.vars = {}
		self.stack = []
	
	def push(self, v):
		self.stack.append(v)
		
	def pop(self):
		if not len(self.stack):
			return None
		return self.stack.pop()
		
	def eval(self, node):
		if not node:
			return None
		else:
			self.visit(node)
			
	def ope(self, lhs, op, rhs):
		print(lhs, op, rhs)
		
		if lhs is None:
			print("lhs None\n")
			sys.exit()
		
		if rhs is None:
			print("rhs None\n")
			sys.exit()
		
		if op == "+":
			return lhs + rhs
		elif op == "-":
			return lhs - rhs
		elif op == "*":
			return lhs * rhs
		elif op == "/":
			return lhs / rhs
		elif op == "%":
			return lhs % rhs
		elif op == "==":
			return lhs == rhs
		elif op == "!=":
			return lhs != rhs
		else:
			print "%s not supported\n" % op
	
	def visit_Decl(self, node):
		type = node.type
		name = type.declname
		typeName = type.type.names[0]
		self.eval(node.init)
		print(name)
		self.vars[name] = self.Variable(name, self.pop())
		
	def visit_Constant(self, node):
		v = self.Value(node.type, int(node.value))
		self.push(v)
	
	def visit_ID(self, node):
		name = node.name
		if not name in self.vars:
			print("at %s %s not found\n" % (node.coord, name))
			sys.exit()
		else:
			self.push(self.vars[name])
	
	def visit_Cast(self, node):
		self.visit(node.expr)
		
	def visit_Assignment(self, node):
		self.visit(node.lvalue)
		self.visit(node.rvalue)
		r = self.pop()
		l = self.pop()
		rv = r.value if isinstance(r, self.Variable) else r
		if node.op == "=":
			l.value = rv
		else:
			lv = l.value if isinstance(l, self.Variable) else l
			l.value = self.ope(lv, node.op[0], rv)
		
	def visit_BinaryOp(self, node):
		self.visit(node.left)
		self.visit(node.right)
		r = self.pop()
		l = self.pop()
		lv = l.value if isinstance(l, self.Variable) else l
		rv = r.value if isinstance(r, self.Variable) else r
		result = self.ope(lv, node.op, rv)
		self.push(result)
	
	def visit_If(self, node):
		a = 0
		
	
class FuncDefVisitor(NodeVisitor):
    def visit_FuncDef(self, node):
#        print('%s at %s' % (node.decl.name, node.decl.coord))
#        node.show()
        v = Verifier()
        v.visit(node.body)

v = FuncDefVisitor()
v.visit(ast)


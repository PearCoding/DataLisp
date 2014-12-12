/*
 Copyright (c) 2014, �mercan Yazici <pearcoding AT gmail.com>
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

 3. Neither the name of the copyright owner may be used
 to endorse or promote products derived from this software without
 specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */
#include "Parser.h"
#include <sstream>

namespace DL
{
	Parser::Parser(const string_t& str, SourceLogger* logger) :
		mLexer(str, logger), mLogger(logger)
	{
	}

	Parser::~Parser()
	{
	}

	SyntaxTree* Parser::parse()
	{
		return gr_tr_unit();
	}

	Token Parser::match(TokenType type)
	{
		Token token = mLexer.next();

		if (token.Type != type)
		{
			std::stringstream stream;
			stream << "Expected '" << tokenToString(type) << "' but got '" << tokenToString(token.Type) << "'";
			mLogger->log(mLexer.currentLine(), mLexer.currentColumn(), L_Error, stream.str());
		}
		return token;
	}

	bool Parser::lookahead(TokenType type)
	{
		Token token = mLexer.look();

		if (token.Type == type)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	const char* Parser::tokenToString(TokenType type)
	{
		switch (type)
		{
		case T_Identifier:
			return "IDENTIFIER";
		case T_String:
			return "STRING";
		case T_Integer:
			return "INTEGER";
		case T_Float:
			return "FLOAT";
		case T_True:
			return "true";
		case T_False:
			return "false";
		case T_OpenParanthese:
			return "(";
		case T_CloseParanthese:
			return ")";
		case T_OpenSquareBracket:
			return "[";
		case T_CloseSquareBracket:
			return "]";
		case T_Comma:
			return ",";
		case T_Colon:
			return ":";
		case T_ExpressionParanthese:
			return "$(";
		case T_EOF:
			return "EOF";
		default:
			return "Unknown";
		}
	}

	SyntaxTree* Parser::gr_tr_unit()
	{
		SyntaxTree* unit = new SyntaxTree;
		while (lookahead(T_OpenParanthese))
		{
			match(T_OpenParanthese);
			unit->Nodes.push_back(gr_statement());
			match(T_CloseParanthese);
		};
		return unit;
	}

	StatementNode* Parser::gr_statement()
	{
		StatementNode* node = new StatementNode;

		node->Name = match(T_Identifier).Value;
		node->Nodes = gr_data_list();
		return node;
	}

	ExpressionNode* Parser::gr_expression()
	{
		ExpressionNode* node = new ExpressionNode;

		node->Name = match(T_Identifier).Value;
		node->Nodes = gr_data_list();
		return node;
	}

	list_t<DataNode*>::type Parser::gr_data_list()
	{
		list_t<DataNode*>::type list;
		while (lookahead(T_Colon) ||
			lookahead(T_OpenParanthese) ||
			lookahead(T_OpenSquareBracket) ||
			lookahead(T_ExpressionParanthese) ||
			lookahead(T_Integer) ||
			lookahead(T_Float) ||
			lookahead(T_String) ||
			lookahead(T_True) ||
			lookahead(T_False))
		{
			list.push_back(gr_data());
		}

		return list;
	}

	DataNode* Parser::gr_data()
	{
		DataNode* node = new DataNode;

		if (lookahead(T_Colon))
		{
			match(T_Colon);
			Token str = match(T_Identifier);

			node->Key = str.Value;
		}
		else
		{
			node->Key = "";
		}

		node->Value = gr_value();
		return node;
	}

	ValueNode* Parser::gr_value()
	{
		ValueNode* node = new ValueNode;

		if (lookahead(T_OpenParanthese))
		{
			match(T_OpenParanthese);

			node->Type = 0;
			node->Statement = gr_statement();

			match(T_CloseParanthese);
		}
		else if (lookahead(T_OpenSquareBracket))
		{
			match(T_OpenSquareBracket);

			node->Type = 5;
			node->Array = gr_array();

			match(T_CloseSquareBracket);
		}
		else if (lookahead(T_ExpressionParanthese))
		{
			match(T_ExpressionParanthese);

			node->Type = 6;
			node->Expression = gr_expression();

			match(T_CloseParanthese);
		}
		else if (lookahead(T_Integer))
		{
			Token str = match(T_Integer);

			node->Type = 1;
			node->Integer = atoi(str.Value.c_str());
		}
		else if (lookahead(T_Float))
		{
			Token str = match(T_Float);

			node->Type = 2;
			node->Float = static_cast<float>(atof(str.Value.c_str()));
		}
		else if (lookahead(T_String))
		{
			Token str = match(T_String);

			node->Type = 3;
			node->String = str.Value;
		}
		else if (lookahead(T_True))
		{
			match(T_True);

			node->Type = 4;
			node->Boolean = true;
		}
		else if (lookahead(T_False))
		{
			match(T_False);

			node->Type = 4;
			node->Boolean = false;
		}
		else
		{
			node->Type = -1;
		}

		return node;
	}

	ArrayNode* Parser::gr_array()
	{
		ArrayNode* node = new ArrayNode;
		node->Nodes = gr_array_list();

		return node;
	}

	list_t<ValueNode*>::type Parser::gr_array_list()
	{
		list_t<ValueNode*>::type nodes;
		while (lookahead(T_OpenParanthese) ||
			lookahead(T_OpenSquareBracket) ||
			lookahead(T_ExpressionParanthese) ||
			lookahead(T_Integer) ||
			lookahead(T_Float) ||
			lookahead(T_String) ||
			lookahead(T_True) ||
			lookahead(T_False))
		{
			nodes.push_back(gr_value());

			if (lookahead(T_Comma))
			{
				match(T_Comma);
			}
			else
			{
				break;
			}
		}

		return nodes;
	}
}
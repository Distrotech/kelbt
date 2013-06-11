/*
 *  Copyright 2004, 2005 Adrian Thurston <thurston@complang.org>
 */

/*  This file is part of C++ Pre-Parser.
 *
 *  C++ Pre-Parser is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 * 
 *  C++ Pre-Parser is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with C++ Pre-Parser; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include "names.h"
#include "parser.h"
#include <iostream>

using namespace std;

ostream &indent( int in )
{
	while ( in-- > 0 )
		cerr << "  ";
	return cerr;
}

void ChildIter::operator++(int)
{
	child = frame->nsChildren; 
	while ( child.lte() ) {
		/* A node may have an non Object type as a base type. */
		if ( *child != 0 ) {
			stack.push( child );
			frame = *child;
			return;
		}
again:
		child.increment();
	}

	if ( stack.stack.length() > 0 ) {
		child = stack.pop();
		goto again;
	}

	atEnd = true;
}

void Buffer::empty()
{
	if ( data != 0 ) {
		free( data );

		data = 0;
		length = 0;
		allocated = 0;
	}
}

void Buffer::upAllocate( int len )
{
	if ( data == 0 )
		data = (char*) malloc( len );
	else
		data = (char*) realloc( data, len );
	allocated = len;
}

#define SA_BLOCK_SIZE 262144

/* Length does not include null terminator. */
char *StrAlloc::allocate( const char *data, int len )
{
	if ( block == 0 || pos+len+1 > SA_BLOCK_SIZE ) {
		block = new char [SA_BLOCK_SIZE];
		pos = 0;
	}
	
	/* Copy the data in, null terminate it, get a ptr to the return val and
	 * then advance the position over the data. */
	memcpy( block+pos, data, len );
	block[pos+len] = 0;
	char *rtnVal = block + pos;
	pos += len + 1;
	return rtnVal;
}

std::ostream &Type::print( std::ostream &out )
{
	switch ( type ) {
	case BuiltinType:
		if ( sign == Unsigned )
			out << "un";
		out << "signed ";
		switch ( keyType ) {
			case Long: out << "long"; break;
			case Int: out << "int"; break;
			case Short: out << "short"; break;
			case Char: out << "char"; break;
			case Wchar_t: out << "wchar_t"; break;
			case Bool: out << "bool"; break;
			case Float: out << "float"; break;
			case Double: out << "double"; break;
			case Void: out << "void"; break;
			default: break;
		}
		break;
	case ObjectType:
		out << "object: " << object << " ";
		switch ( object->type ) {
			case Object::ClassType: out << "class "; break;
			case Object::TemplateClassType: out << "template class "; break;
			case Object::EnumType: out << "enum "; break;
			default: out << "OTHER "; break;
		}
		out << (object->data != 0 ? object->data : "(NULL)" );
		break;
	case TemplParamType:
		out << "template param";
		break;
	case UnknownType:
		out << "unknown type";
		break;
	}
	return out;
}

std::ostream &TypeList::print( std::ostream &out )
{
	for ( Iter pt = first(); pt.lte(); pt++ ) {
		if ( *pt == 0 )
			out << "  0" << endl;
		else {
			out << "  ";
			(*pt)->print( out ) << endl;
		}
	}
	return out;
}

bool Parser::typeListsMatch( Object *baseNs, TypeList *specList, TypeList *argList )
{
	if ( specList->length() != argList->length() )
		return false;
	for ( TypeList::Iter sli = *specList, ali = *argList; sli.lte(); sli++, ali++ ) {
		if ( *sli != templParamType && *sli != *ali )
			return false;
	}
	return true;
}

int Parser::numTemplParams( TypeList *typeList )
{
	int numTemplParams = 0;
	for ( TypeList::Iter tli = *typeList; tli.lte(); tli++ ) {
		if ( *tli == templParamType )
			numTemplParams += 1;
	}
	return numTemplParams;
}

void Parser::clearLessSpecific( Vector<TypeListMapEl*> &matched, TypeList *typeList )
{
	for ( int ri = 0; ri < matched.length(); ) {
		if ( numTemplParams( typeList ) < numTemplParams( &matched[ri]->key ) )
			matched.remove( ri );
		else
			ri += 1;
	}
}

Object *Parser::findSpecCovers( Object *baseNs, TypeList *typeList )
{
	Vector<TypeListMapEl*> matched;
	for ( TypeListMap::Iter tli = baseNs->typeListMap; tli.lte(); tli++ ) {
		if ( typeListsMatch( baseNs, &tli->key, typeList ) ) {
			clearLessSpecific( matched, &tli->key );
			matched.append( tli );
		}
	}

	/* FIXME: if results contains more than one item we simply return the
	 * first. Really our callers should be dealing with the error. */
	if ( matched.length() > 0 )
		return matched[0]->value;
	return 0;
}


Type *Parser::reduceType( Type *type )
{
	if ( type->type == Type::TemplParamType )
		return templParamType;
	else if ( type->type == Type::UnknownType )
		return unknownType;
	else {
		TypeMapEl *result = typeMap.find( type );
		if ( result != 0 )
			return result->key;
		else {
			Type *newType = new Type( *type );
			typeMap.insert( newType );
			return newType;
		}
	}
}

Type *Parser::newType( Type::Property prop )
{
	Type newType;
	newType.type = Type::BuiltinType;
	if ( Type::isSign( prop ) ) {
		newType.sign = prop;
		newType.keyType = Type::Int;
	}
	else {
		newType.sign = Type::Signed;
		newType.keyType = prop;
	}
	newType.object = 0;
	return reduceType( &newType );
}

Type *Parser::addInProp( Type *type, Type::Property prop )
{
	Type newType = *type;
	if ( Type::isSign( prop ) )
		newType.sign = prop;
	else
		newType.keyType = prop;
	return reduceType( &newType );
}



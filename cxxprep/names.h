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

#ifndef _NAMES_H
#define _NAMES_H

#include "dlist.h"
#include "compare.h"
#include "vector.h"
#include "avlmap.h"
#include "avltree.h"
#include "avlset.h"
#include <iostream>

#define LOG_TOKENS
#define LOG_ACTIONS
#define LOG_REDUCE
#define LOG_LOOKUP
#define LOG_BACKTRACK

using std::ostream;

struct Type;
struct Object;

/* This structure is used to keep track of information necessary to make a
 * declaration. While parsing a declaration it records the declaration's
 * attributes. */
struct DeclarationData
{
	void init()
	{
		isTypedef = false;
		isFriend = false;
		isTemplate = false;
		type = 0;
	}

	bool isTypedef;
	bool isFriend;
	bool isTemplate;
	Type *type;
};

/* This structure is used to keep track of information necessary to make a
 * declarator. */
struct DeclaratorData
{
	void init()
	{
		qualObj = 0;
		declScope = 0;
	}

	Object *qualObj;
	Object *declScope;
};


struct Parser_LangEl;
typedef Parser_LangEl LangEl;

struct Token
{
	int outType;
	char *data;
	char *leader;
	bool qual;
	Object *object;
};

struct NameData
{
	char *id;
	Object *object;
	Object *qualNs;
	LangEl *langEl;
};

struct Object;

typedef AvlMap<char*, Object*, CmpStr> BaseNsMap;
typedef AvlMapEl<char*, Object*> BaseNsMapEl;
typedef DList<Object> ObjectList;

struct MapEl : public AvlTreeEl<MapEl>
{
	MapEl(char *data) :
		data(data)
	{ }

	char *getKey() const { return data; }

	char *data;
	ObjectList objectList;
};

typedef AvlTree<MapEl, char*, CmpStr> Map;
typedef Vector<Object*> BaseNsVect;

template <class T> struct Stack
{
	void push( T obj );
	void set( T obj );
	T pop();
	T &top();

	typedef typename Vector<T>::Iter Iter;

	Vector<T> stack;
};

template <class T> void Stack<T>::push( T obj )
{
	stack.prepend( obj );
}

template <class T> void Stack<T>::set( T obj )
{
	stack.data[0] = obj;
}

template <class T> T &Stack<T>::top()
{
	return stack.data[0];
}

template <class T> T Stack<T>::pop()
{
	T rtnVal = stack.data[0];
	stack.remove( 0, 1 );
	return rtnVal;
}


/* A growable buffer for collecting headers. */
struct Buffer
{
	Buffer() : data(0), allocated(0), length(0) { }
	Buffer( const Buffer &other ) {
		data = (char*)malloc( other.allocated );
		memcpy( data, other.data, other.length );
		allocated = other.allocated;
		length = other.length;
	}
	~Buffer() { empty(); }

	void append( const char p ) {
		if ( ++length > allocated )
			upAllocate( length*2 );
		data[length-1] = p;
	}
	void append( const char *str, int len ) {
		if ( (length += len) > allocated )
			upAllocate( length*2 );
		memcpy( data+length-len, str, len );
	}
		
	void clear() { length = 0; }
	void upAllocate( int len );
	void empty();

	char *data;
	int allocated;
	int length;
};

struct Type 
{
	enum TypeType { 
		BuiltinType, ObjectType, TemplParamType, UnknownType
	};

	/* For builtin types. */
	enum Property { 
		Signed, Unsigned,
		Long, Int, Short, Char, 
		Wchar_t, Bool, 
		Float, Double, Void 
	};

	Type() {}

	Type( Object *object ) :
		type(ObjectType), object(object) {}

	TypeType type;
	Property sign;
	Property keyType;
	Object *object;

	static bool isSign( Property prop ) 
		{ return prop == Signed || prop == Unsigned; }
	
	Object *getObject() 
		{ return type == ObjectType ? object : 0; }

	ostream &print( ostream &out );
};

struct CmpType
{
	static int compare( const Type *type1, const Type *type2 )
	{
		if ( type1->type < type2->type )
			return -1;
		else if ( type1->type > type2->type )
			return 1;
		else if ( type1->type == Type::BuiltinType ) {
			if ( type1->sign < type2->sign )
				return -1;
			else if ( type1->sign > type2->sign )
				return 1;
			else if ( type1->keyType < type2->keyType )
				return -1;
			else if ( type1->keyType > type2->keyType )
				return 1;
			return 0;
		}
		else if ( type1->type == Type::ObjectType ) {
			if ( type1->object < type2->object )
				return -1;
			else if ( type1->object > type2->object )
				return 1;
			return 0;
		}
		return 0;
	}
};

typedef AvlSet<Type*, CmpType> TypeMap;
typedef AvlSetEl<Type*> TypeMapEl;

struct TypeList : public Vector<Type*>
{
	ostream &print( ostream &out );
};

typedef CmpTable< Type*, CmpOrd<Type*> > CmpTypeList;
typedef AvlMap<TypeList, Object*, CmpTypeList> TypeListMap;
typedef AvlMapEl<TypeList, Object*> TypeListMapEl;

struct Object
{
	enum Type { 
		NamespaceType,
		ClassType, 
		TemplateClassType,
		TypedefType, 
		EnumType,
		IdType,
		TemplateIdType,
		UnknownType
	};

	Object( ) : 
		type(UnknownType), data(0), objType(0) {}

	Object( Type type, char *data ) : 
		type(type), data(data), objType(0) {}

	Object( char *data ) : 
		type(UnknownType), data(data), objType(0) {}

	Object( Type type, const Stack<Object*> &lookupNs ) : 
		type(type), data(0), objType(0), lookupNs(lookupNs) {}

	Object( Type type, char *data, const Stack<Object*> &lookupNs ) : 
		type(type), data(data), objType(0), lookupNs(lookupNs) 
		{ this->Object::lookupNs.push( this ); }

	Type type;
	char *data;
	::Type *objType;

	Map map;
	TypeListMap typeListMap;

	Stack<Object*> lookupNs;
	BaseNsVect nsChildren;

	inline Object *findObject( char *data );
	inline void insertObject( char *data, Object *object );
	inline void removeObject( Object *object );

	inline Object *findNamespace( char *data );
	inline Object *findClass( char *data );
	inline Object *findTemplateClass( char *data );
	inline Object *findTypedef( char *data );
	inline Object *findEnum( char *data );
	inline Object *findIdentifier( char *data );
	inline Object *findTemplateId( char *data );
	inline Object *findSpecExact( const TypeList &typeList );

	Object *prev, *next;
};

struct StrAlloc
{
	StrAlloc() : block(0) {}

	/* Length does not include null terminator. */
	char *allocate( const char *data, int len );

	char *block;
	int pos;
};

Object *Object::findObject( char *data )
{
	MapEl *mapEl = map.find( data );
	return mapEl != 0 ? mapEl->objectList.head : 0;
}

void Object::insertObject( char *data, Object *object )
{
	MapEl *inMap;
	map.insert( data, &inMap );
	inMap->objectList.prepend( object );
}

void Object::removeObject( Object *object )
{
	MapEl *mapEl = map.find( object->data );
	mapEl->objectList.detach( object );
}

Object *Object::findNamespace( char *data )
{
	MapEl *mapEl = map.find( data );
	if ( mapEl != 0 ) {
		for ( ObjectList::Iter obj = mapEl->objectList; obj.lte(); obj++ ) {
			if ( obj->type == NamespaceType )
				return obj;
		}
	}
	return 0;
}

Object *Object::findClass( char *data )
{
	MapEl *mapEl = map.find( data );
	if ( mapEl != 0 ) {
		for ( ObjectList::Iter obj = mapEl->objectList; obj.lte(); obj++ ) {
			if ( obj->type == ClassType )
				return obj;
		}
	}
	return 0;
}

Object *Object::findTemplateClass( char *data )
{
	MapEl *mapEl = map.find( data );
	if ( mapEl != 0 ) {
		for ( ObjectList::Iter obj = mapEl->objectList; obj.lte(); obj++ ) {
			if ( obj->type == TemplateClassType )
				return obj;
		}
	}
	return 0;
}


Object *Object::findTypedef( char *data )
{
	MapEl *mapEl = map.find( data );
	if ( mapEl != 0 ) {
		for ( ObjectList::Iter obj = mapEl->objectList; obj.lte(); obj++ ) {
			if ( obj->type == TypedefType )
				return obj;
		}
	}
	return 0;
}

Object *Object::findEnum( char *data )
{
	MapEl *mapEl = map.find( data );
	if ( mapEl != 0 ) {
		for ( ObjectList::Iter obj = mapEl->objectList; obj.lte(); obj++ ) {
			if ( obj->type == EnumType )
				return obj;
		}
	}
	return 0;
}

Object *Object::findIdentifier( char *data )
{
	MapEl *mapEl = map.find( data );
	if ( mapEl != 0 ) {
		for ( ObjectList::Iter obj = mapEl->objectList; obj.lte(); obj++ ) {
			if ( obj->type == IdType )
				return obj;
		}
	}
	return 0;
}

Object *Object::findTemplateId( char *data )
{
	MapEl *mapEl = map.find( data );
	if ( mapEl != 0 ) {
		for ( ObjectList::Iter obj = mapEl->objectList; obj.lte(); obj++ ) {
			if ( obj->type == TemplateIdType )
				return obj;
		}
	}
	return 0;
}

Object *Object::findSpecExact( const TypeList &typeList )
{
	TypeListMapEl *mapEl = typeListMap.find( typeList );
	return mapEl != 0 ? mapEl->value : 0;
}

#endif /* _NAMES_H */

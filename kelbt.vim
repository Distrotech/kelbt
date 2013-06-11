" Vim syntax file
"
" Language: Kelbt
" Author: Adrian Thurston

syntax clear

"
" Outside code
"

" Comments
syntax region ocComment start="\/\*" end="\*\/"
syntax match ocComment "\/\/.*$"

" Anything preprocessor
syntax match ocPreproc "#.*$"

" Strings
syntax match ocLiteral "'\(\\.\|[^'\\]\)*'"
syntax match ocLiteral "\"\(\\.\|[^\"\\]\)*\""

" Keywords
syntax keyword ocType unsigned signed void char short int long float double bool
syntax keyword ocType inline static extern register const volatile auto
syntax keyword ocType union enum struct class typedef
syntax keyword ocType namespace template typename mutable
syntax keyword ocKeyword break continue default do else for
syntax keyword ocKeyword goto if return switch while
syntax keyword ocKeyword new delete this using friend public private protected sizeof
syntax keyword ocKeyword throw try catch operator typeid
syntax keyword ocKeyword and bitor xor compl bitand and_eq or_eq xor_eq not not_eq
syntax keyword ocKeyword static_cast dynamic_cast
syntax match ocObjCDirective "@public\|@private\|@protected"
syntax match ocObjCDirective "@interface\|@implementation"
syntax match ocObjCDirective "@class\|@end\|@defs"
syntax match ocObjCDirective "@encode\|@protocol\|@selector"


" Numbers
syntax match ocNumber "[0-9][0-9]*"
syntax match ocNumber "0x[0-9a-fA-F][0-9a-fA-F]*"

" Booleans
syntax keyword ocBoolean true false

" Identifiers
syntax match anyId "[a-zA-Z_][a-zA-Z_0-9]*"

" Labels
syntax match ocLabelColon "^[\t ]*[a-zA-Z_][a-zA-Z_0-9]*[ \t]*:$" contains=ocLabel
syntax match ocLabelColon "^[\t ]*[a-zA-Z_][a-zA-Z_0-9]*[ \t]*:[^=:]"me=e-1 contains=ocLabel
syntax match ocLabel "[a-zA-Z_][a-zA-Z_0-9]*" contained

" Case Labels
syntax keyword ocCaseLabelKeyword case contained
syntax cluster ocCaseLabelItems contains=ocComment,ocPreproc,ocLiteral,ocType,ocKeyword,ocCaseLabelKeyword,ocNumber,ocBoolean,anyId
syntax match ocCaseLabelColon "case" contains=@ocCaseLabelItems
syntax match ocCaseLabelColon "case[\t ]\+.*:$" contains=@ocCaseLabelItems
syntax match ocCaseLabelColon "case[\t ]\+.*:[^=:]"me=e-1 contains=@ocCaseLabelItems


"
" Kelbt sections
"

" Parser Specification
syntax region parserSpec1 matchgroup=beginParser start="%%{" end="}%%" contains=@klItems
syntax region parserSpec2 matchgroup=beginParser start="%%[^{]"rs=e-1 end="$" keepend contains=@klItems
syntax region parserSpec3 matchgroup=beginParser start="%%$" end="$" keepend contains=@klItems

" Items in kelbt sections
syntax cluster klItems contains=klComment,klLiteral,klKeywords,klCodeCurly,klCodeSemi,klNumber,anyId,klNonTerm,klWrite

" Non-terminal
syntax match klNonTerm "[a-zA-Z_][a-zA-Z_0-9]*[ \t]*:" contained

" Comments
syntax match klComment "#.*$" contained

" Literals
syntax match klLiteral "'\(\\.\|[^'\\]\)*'" contained
syntax match klLiteral "\"\(\\.\|[^\"\\]\)*\"" contained
syntax match klLiteral /\/\(\\.\|[^\/\\]\)*\// contained
syntax match klLiteral "\[\(\\.\|[^\]\\]\)*\]" contained

" Numbers
syntax match klNumber "[0-9][0-9]*" contained
syntax match klNumber "0x[0-9a-fA-F][0-9a-fA-F]*" contained

" Keywords
syntax keyword klKeywords parser include token nonterm uses type translate try undo final commit pri class shortest contained

" Write statements
syntax region klWrite matchgroup=Type start="\<write\>" matchgroup=NONE end=";" contained contains=klWriteWhat
syntax keyword klWriteWhat instance_data token_defs types data init exec finish contained

" Inline items
syntax cluster inlineItems contains=klCodeCurly,ocComment,ocPreproc,ocLiteral,ocType,ocKeyword,ocNumber,ocBoolean,ocLabelColon,anyId,ocCaseLabelColon,ilAccess

" RHS Accessors in inline code
syntax match ilAccess "[\$@]\([\$@]\|[0-9]\+\)" contained

" Blocks of code. klCodeCurly is recursive.
syntax region klCodeCurly matchgroup=NONE start="{" end="}" contained contains=@inlineItems
syntax region klCodeSemi matchgroup=Type start="\<access\>" matchgroup=NONE end=";" contained contains=@inlineItems

"
" Sync at the start and end of parser specifications.
"
syntax sync match kelbtSyncPat grouphere NONE "%%{&"
syntax sync match kelbtSyncPat grouphere NONE "}%%"

"
" Specifying Groups
"
hi link ocComment Comment
hi link ocPreproc Macro
hi link ocLiteral String
hi link ocType Type
hi link ocKeyword Keyword
hi link ocObjCDirective Keyword
hi link ocNumber Number
hi link ocBoolean Boolean
hi link ocCaseLabelKeyword Keyword
hi link ocLabel Label
hi link klComment Comment
hi link klNumber Number
hi link klLiteral String
hi link klKeywords Type
hi link klNonTerm Function
hi link klWriteWhat Keyword
hi link ilAccess Keyword
hi link beginParser Type
 
let b:current_syntax = "kelbt"

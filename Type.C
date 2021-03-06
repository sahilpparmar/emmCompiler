#include "Type.h"
#include "SymTabEntry.h"

#include <stdlib.h>

const char* typeName[] = {
    "error", "unknown", "void", 
    "bool", 
    "string",
    "byte",
    "unsigned int", "int", 
    "double",
    "class", 
    "event", "function",
    "numeric",
    "integral",
    "signed",
    "unsigned",
    "scalar",
    "primitive",
    "native",
    "data",
    "any"
};

const string Type::name(TypeTag t) {
    if((t >= ERROR) && (t <= ANY))
        return string(typeName[t]);
    else return string();
}

const Type
Type::errorType(ERROR), Type::unkType(UNKNOWN), Type::voidType(VOID),
    Type::boolType(BOOL),
    Type::stringType(STRING),
    Type::byteType(BYTE), 
    Type::uintType(UINT), Type::intType(INT), 
    Type::doubleType(DOUBLE);

const Type* Type::type[] = {
    &Type::errorType, &Type::unkType, &Type::voidType, 
    &Type::boolType, 
    &Type::stringType,
    &Type::byteType, 
    &Type::uintType, &Type::intType,
    &Type::doubleType,
    NULL, NULL, NULL,
    NULL, NULL,
    NULL, NULL, NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

/****************************************************************
  Type Class 
 ****************************************************************/

Type::Type(TypeTag tag) {
    tag_ = tag; 
    argTypes_ = NULL; 
    retType_ = NULL;
    size_ = 4;  // All primitive types have size 4 bytes
};     

Type::Type(SymTabEntry* td, TypeTag t) {   // Must be enum, class or struct
    tag_ = t; 
    typeDesc_ = td;
    size_ = 4;  // Size of an empty class is 4 bytes
};

Type::Type(vector<Type*>* tupleType, TypeTag t) {// For tuples, modules, events
    tag_ = t; 
    argTypes(tupleType); 
    size_ = 0;
};

Type::Type(vector<Type*>* argt, Type* rt) {      // For functions
    tag_ = FUNCTION; 
    retType_ = rt; 
    argTypes_ = argt; 
    size_ = 0;
};

string 
Type::fullName() const {
    string info;
    switch (tag_) {
        case CLASS:
            return info + name() + " " + typeDesc_->name() ;

        case EVENT:
        case FUNCTION:
            info += name(tag_) + " (";
            if (argTypes_ != NULL) {
                for (unsigned int i=0; i< argTypes_->size(); i++) {
                    if ((*argTypes_)[i] != NULL)
                        info += (*argTypes_)[i]->fullName() ;  
                    if (i < argTypes_->size() - 1)
                        info += ", ";
                }
            }
            if (tag_ == FUNCTION) {
                info += "): ";
                info += retType_->fullName() + " ";
            }
            else info += ") ";
            return info;

        default:
            break;
    }
    return info+name();
}  

const Type&
Type::operator=(const Type& t) {
    tag_ = t.tag_;
    switch (tag_) {
        case CLASS: 
            typeDesc_ = t.typeDesc_;
            break;

        case EVENT:
        case FUNCTION: 
            if (t.argTypes_ != NULL) {
                argTypes_ = new vector<Type*>(t.argTypes_->size());
                for (unsigned int i=0; i<t.argTypes_->size(); i++) 
                    (*argTypes_)[i] = new Type(*((*t.argTypes_)[i]));
            } 
            else argTypes_ = NULL;
            if (tag_ == FUNCTION) {
                if (t.retType_ != NULL)
                    retType_ = new Type(*(t.retType_));
                else retType_ = new Type(VOID);
            }
            break;

        default:
            break;
    }
    return *this;
}

void 
Type::print(ostream& os, int indent) const {
    os << fullName();
}

bool
Type::isSubType(TypeTag t2, const Type* param_type) const {
    TypeTag t1 = tag();

    //cout << " isSubType() ";

    // Both t1 and t2 should be bool
    if (isBool(t1) && isBool(t2)) return true;
    else if (isBool(t1)) return false;
    else if (isBool(t2)) return false;

    //cout << "string";
    // Both t1 and t2 should be string
    if (isString(t1) && isString(t2)) return true;
    else if (isString(t1)) return false;
    else if (isString(t2)) return false;

    //cout << "class";
    // Both t1 and t2 should be class
    if (isClass(t1) && isClass(t2)) {

        if(param_type->typeDesc()->name().compare(this->typeDesc()->name()) != 0)
            return false; 
        
        return true;
    }
    else if (isClass(t1)) return false;
    else if (isClass(t2)) return false;

    //cout << "byte";
    if (t1 == BYTE && t2 == BYTE) return true;
    else if (t1 == BYTE) return false;

    //cout << "float;
    if (isFloat(t1)) {
        if (isNumeric(t2)) return true;
        else return false;
    } else if (isFloat(t2)) return false;

    //cout << "int";
    if (isUnsigned(t1)) {
        if (isUnsigned(t2)) return true;
        else return false;
    } else if (isSigned(t1)) {
        if (isSigned(t2) || isUnsigned(t2)) return true;
        else return false;
    }

    return true;
}


#ifndef C_DSPECS_H
#define C_DSPECS_H

enum DeclBaseType_t
{
  DT_NONE = 0,

  DT_VOID,          

  DT_BOOL,          
  DT_CHAR,          
  DT_INT,           
  DT_EXT_INT,       

  DT_FLOAT,         
  DT_DOUBLE,        
  DT_EXT_FLOAT,     

  DT_TYPENAME, // typedef, 
  DT_TAG,      // enum, struct, union

  DT_LAST,
};

struct TDeclSpecs
{
  enum DeclBaseType_t baseType;   

  uchar isComplex  : 2; // 0 - no, 1 - imaginary, 2 - complex
  uchar isSigned   : 2; // 0 - no, 1 - unsigned, 2 - signed
  uchar isLong     : 2; // 0 - no, 1 - short, 2 - long, 3 - long long
  uchar isConst    : 1;
  uchar isVolatile : 1;
  uchar isRestrict : 1;
  uchar isInline   : 1;

  union
  {
    IntegerType_t  extInt;    // DT_EXT_INT: index of the type
    RealType_t     extFloat;  // DT_EXT_FLOAT: index of the type
    const TQualNode  * typeName;  // DT_TYPENAME
    const TTypeNode  * tag;       // DT_TAG
  } u;

  ExtQualAttr_t   extQualAttr; 
  StorageClass_t  sclass;
  TExtDeclSpec    * extDeclSpec;

  const TQualNode * type; // set by DSpecs_GenType()
};
void TDeclSpecs_GcMark ( const TDeclSpecs * ds );

TDeclSpecs * New_DSpecs ( int token );
TDeclSpecs * New_DSpecs_ExtInt ( const TAst * astNum );
TDeclSpecs * New_DSpecs_ExtFloat ( const TAst * astNum );
TDeclSpecs * New_DSpecs_Typename ( const TQualNode * type );
TDeclSpecs * New_DSpecs_Tag ( const TTypeNode * tag );
TDeclSpecs * New_DSpecs_ExtDeclSpec ( TExtDeclSpec * extDeclSpec ); 
TDeclSpecs * New_DSpecs_ExtQualAttr ( ExtQualAttr_t extQualAttr );
TDeclSpecs * DSpecs_Add ( TDeclSpecs * a, TDeclSpecs * b );
TQualNode  * DSpecs_GenQual ( TDeclSpecs * ds );
TDeclSpecs * DSpecs_GenType ( TDeclSpecs * ds );

#endif // C_DSPECS_H
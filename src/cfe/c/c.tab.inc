

#ifndef YYST_AST

#define YYST_AST  1
#define YYST_STR  2
#define YYST_TOKEN  3
#define YYST_SYM  4
#define YYST_SCOPE  5
#define YYST_TYP  6
#define YYST_DSPECS  7
#define YYST_TNODE  8
#define YYST_EDESC  9
#define YYST_M_TYP  10
#define YYST__LAST  11

#endif

#ifndef YYTOKTYP_DEFINED
#define YYTOKTYP_DEFINED

static const unsigned char yytoktyp[] = 
{
  0, 0, 0, YYST_TOKEN, YYST_TOKEN, 
  YYST_TOKEN, YYST_TOKEN, YYST_TOKEN, YYST_TOKEN, YYST_TOKEN, 
  YYST_TOKEN, YYST_TOKEN, YYST_TOKEN, YYST_TOKEN, YYST_TOKEN, 
  0, 0, YYST_TOKEN, YYST_TOKEN, YYST_TOKEN, 
  YYST_TOKEN, YYST_TOKEN, YYST_TOKEN, YYST_TOKEN, YYST_TOKEN, 
  YYST_TOKEN, YYST_TOKEN, YYST_TOKEN, YYST_TOKEN, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, YYST_STR, YYST_SYM, YYST_AST, 
  YYST_AST, YYST_AST, YYST_AST, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  YYST_AST, YYST_AST, YYST_STR, YYST_STR, YYST_AST, 
  YYST_AST, 0, 0, YYST_AST, 0, 
  YYST_AST, YYST_AST, YYST_AST, YYST_AST, YYST_AST, 
  YYST_AST, YYST_AST, YYST_AST, YYST_AST, YYST_AST, 
  YYST_AST, YYST_AST, YYST_AST, YYST_AST, 0, 
  YYST_AST, YYST_AST, YYST_AST, YYST_AST, 0, 
  YYST_DSPECS, YYST_DSPECS, YYST_DSPECS, YYST_DSPECS, YYST_DSPECS, 
  0, 0, 0, YYST_TYP, 0, 
  YYST_DSPECS, YYST_TOKEN, YYST_DSPECS, YYST_DSPECS, YYST_TOKEN, 
  YYST_DSPECS, YYST_DSPECS, YYST_DSPECS, YYST_STR, YYST_TNODE, 
  YYST_SCOPE, 0, YYST_TOKEN, 0, 0, 
  YYST_DSPECS, YYST_DSPECS, YYST_DSPECS, YYST_DSPECS, 0, 
  0, 0, YYST_DSPECS, 0, 0, 
  YYST_EDESC, 0, 0, 0, YYST_STR, 
  YYST_DSPECS, YYST_TOKEN, YYST_DSPECS, YYST_SYM, YYST_TYP, 
  0, 0, 0, 0, 0, 
  0, 0, YYST_SCOPE, 0, YYST_SCOPE, 
  0, YYST_TYP, YYST_SYM, 0, 0, 
  YYST_SYM, YYST_SYM, 0, 0, YYST_SYM, 
  YYST_SYM, 0, 0, YYST_TYP, YYST_TYP, 
  YYST_TYP, YYST_DSPECS, YYST_M_TYP, YYST_M_TYP, 0, 
  0, 0, 0, 0, 0, 
  YYST_SYM, YYST_TYP, 0, 0, 0, 
  0, YYST_TYP, YYST_SYM, 0, YYST_DSPECS, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, YYST_SCOPE, 0, 0
};

#endif

#ifndef YYTOKDESC_DEFINED
#define YYTOKDESC_DEFINED

static const char * const yytokdesc[] = 
{
  0, 0, 0, "void", "char", 
  "short", "int", "long", "signed", "unsigned", 
  "float", "double", "_Bool", "_Complex", "_Imaginary", 
  0, 0, "const", "volatile", "restrict", 
  "extern", "auto", "static", "register", "typedef", 
  "inline", "struct", "union", "enum", "switch", 
  "continue", "case", "sizeof", "default", "else", 
  "while", "do", "goto", "break", "if", 
  "for", "return", "identifier", "type name", "integer number", 
  "real number", "string constant", "wide string constant", "||", "&&", 
  "++", "--", "->", "...", "'*=', '/=', '%=', '+=', '-=', '&=', '|=', '^=', '<<=' or '>>='", 
  "'==' or '!='", "'<=', '>=', '<' or '>'", "'<<' or '>>'", "__declspec", "__attr", 
  "__typeof__", "__label__", "__alignof__", "__attribute__", 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 
  0, 0, 0, 0
};

#endif



#ifndef YYSTATETYP_DEFINED
#define YYSTATETYP_DEFINED

static const unsigned char yystatetyp[] =
{
  0,
  0, 	/*   1:error */
  YYST_TOKEN, 	/*   2:C_VOID */
  YYST_TOKEN, 	/*   3:C_CHAR */
  YYST_TOKEN, 	/*   4:C_SHORT */
  YYST_TOKEN, 	/*   5:C_INT */
  YYST_TOKEN, 	/*   6:C_LONG */
  YYST_TOKEN, 	/*   7:C_SIGNED */
  YYST_TOKEN, 	/*   8:C_UNSIGNED */
  YYST_TOKEN, 	/*   9:C_FLOAT */
  YYST_TOKEN, 	/*  10:C_DOUBLE */
  YYST_TOKEN, 	/*  11:C__BOOL */
  YYST_TOKEN, 	/*  12:C__COMPLEX */
  YYST_TOKEN, 	/*  13:C__IMAGINARY */
  0, 	/*  14:C_EXT_INT */
  0, 	/*  15:C_EXT_FLOAT */
  YYST_TOKEN, 	/*  16:C_CONST */
  YYST_TOKEN, 	/*  17:C_VOLATILE */
  YYST_TOKEN, 	/*  18:C_RESTRICT */
  YYST_TOKEN, 	/*  19:C_EXTERN */
  YYST_TOKEN, 	/*  20:C_AUTO */
  YYST_TOKEN, 	/*  21:C_STATIC */
  YYST_TOKEN, 	/*  22:C_REGISTER */
  YYST_TOKEN, 	/*  23:C_TYPEDEF */
  YYST_TOKEN, 	/*  24:C_INLINE */
  YYST_TOKEN, 	/*  25:STRUCT */
  YYST_TOKEN, 	/*  26:UNION */
  YYST_TOKEN, 	/*  27:ENUM */
  YYST_SYM, 	/*  28:TYPENAME */
  0, 	/*  29:DECLSPEC */
  0, 	/*  30:ATTR */
  0, 	/*  31:TYPEOF */
  0, 	/*  32:';' */
  0, 	/*  33:declaration */
  YYST_DSPECS, 	/*  34:GenType_declaration_specifiers */
  YYST_DSPECS, 	/*  35:declaration_specifiers */
  YYST_DSPECS, 	/*  36:storage_class_specifier */
  YYST_TOKEN, 	/*  37:sclass */
  YYST_DSPECS, 	/*  38:declspec */
  YYST_DSPECS, 	/*  39:type_specifier */
  YYST_TOKEN, 	/*  40:typespec */
  YYST_DSPECS, 	/*  41:extended_int_specifier */
  YYST_DSPECS, 	/*  42:extended_float_specifier */
  YYST_DSPECS, 	/*  43:struct_or_union_specifier */
  YYST_TOKEN, 	/*  44:struct_or_union */
  YYST_DSPECS, 	/*  45:enum_specifier */
  YYST_DSPECS, 	/*  46:type_qualifier */
  YYST_TOKEN, 	/*  47:qual */
  YYST_DSPECS, 	/*  48:function_specifier */
  YYST_DSPECS, 	/*  49:typedef_name */
  0, 	/*  50:translation_unit */
  0, 	/*  51:external_declaration */
  0, 	/*  52:function_definition */
  0, 	/*  53:'(' */
  0, 	/*  54:'(' */
  YYST_STR, 	/*  55:IDENT */
  YYST_SYM, 	/*  56:TYPENAME */
  YYST_STR, 	/*  57:any_identifier */
  YYST_STR, 	/*  58:null_str */
  0, 	/*  59:'(' */
  0, 	/*  60:'(' */
  0, 	/*  61:'(' */
  0, 	/*  62:init_declarator_list */
  0, 	/*  63:init_declarator_list_opt */
  YYST_TYP, 	/*  64:GetDSpecsTyp */
  0, 	/*  65:init_declarator */
  YYST_DSPECS, 	/*  66:declaration_specifiers */
  YYST_DSPECS, 	/*  67:declaration_specifiers_opt */
  YYST_DSPECS, 	/*  68:declaration_specifiers1 */
  YYST_DSPECS, 	/*  69:storage_class_specifier */
  YYST_DSPECS, 	/*  70:type_specifier */
  YYST_DSPECS, 	/*  71:type_qualifier */
  YYST_DSPECS, 	/*  72:function_specifier */
  YYST_STR, 	/*  73:any_identifier */
  YYST_STR, 	/*  74:null_str */
  YYST_DSPECS, 	/*  75:declaration_specifiers_opt */
  YYST_DSPECS, 	/*  76:declaration_specifiers_opt */
  YYST_DSPECS, 	/*  77:declaration_specifiers2 */
  YYST_DSPECS, 	/*  78:storage_class_specifier */
  YYST_DSPECS, 	/*  79:type_qualifier */
  YYST_DSPECS, 	/*  80:function_specifier */
  0, 	/*  81:external_declaration */
  0, 	/*  82:ensure_file_scope */
  YYST_AST, 	/*  83:NUMBER */
  YYST_AST, 	/*  84:NUMBER */
  YYST_TNODE, 	/*  85:declare_tag */
  YYST_EDESC, 	/*  86:declare_enum */
  YYST_EDESC, 	/*  87:declare_enum */
  YYST_STR, 	/*  88:any_identifier */
  YYST_STR, 	/*  89:any_identifier */
  0, 	/*  90:SIZEOF */
  YYST_STR, 	/*  91:IDENT */
  YYST_AST, 	/*  92:NUMBER */
  YYST_AST, 	/*  93:F_NUMBER */
  YYST_AST, 	/*  94:STRING_CONST */
  YYST_AST, 	/*  95:LSTRING_CONST */
  0, 	/*  96:LOG_AND */
  0, 	/*  97:INC */
  0, 	/*  98:DEC */
  0, 	/*  99:ALIGNOF */
  0, 	/* 100:'(' */
  0, 	/* 101:'*' */
  0, 	/* 102:'&' */
  0, 	/* 103:'+' */
  0, 	/* 104:'-' */
  0, 	/* 105:'~' */
  0, 	/* 106:'!' */
  YYST_AST, 	/* 107:constant */
  YYST_AST, 	/* 108:string_literal */
  YYST_STR, 	/* 109:identifier */
  YYST_AST, 	/* 110:primary_expression */
  YYST_AST, 	/* 111:postfix_expression */
  YYST_AST, 	/* 112:unary_expression */
  0, 	/* 113:unary_operator */
  YYST_AST, 	/* 114:cast_expression */
  YYST_AST, 	/* 115:multiplicative_expression */
  YYST_AST, 	/* 116:additive_expression */
  YYST_AST, 	/* 117:shift_expression */
  YYST_AST, 	/* 118:relational_expression */
  YYST_AST, 	/* 119:equality_expression */
  YYST_AST, 	/* 120:and_expression */
  YYST_AST, 	/* 121:exclusive_or_expression */
  YYST_AST, 	/* 122:inclusive_or_expression */
  YYST_AST, 	/* 123:logical_and_expression */
  YYST_AST, 	/* 124:logical_or_expression */
  YYST_AST, 	/* 125:conditional_expression */
  YYST_AST, 	/* 126:assignment_expression */
  YYST_AST, 	/* 127:expression1 */
  YYST_AST, 	/* 128:expression */
  YYST_DSPECS, 	/* 129:type_specifier */
  YYST_DSPECS, 	/* 130:GenType_specifier_qualifier_list */
  YYST_DSPECS, 	/* 131:specifier_qualifier_list */
  YYST_DSPECS, 	/* 132:type_qualifier */
  0, 	/* 133:type_name */
  YYST_DSPECS, 	/* 134:typedef_name */
  0, 	/* 135:',' */
  0, 	/* 136:';' */
  0, 	/* 137:'*' */
  YYST_SYM, 	/* 138:declarator */
  YYST_TYP, 	/* 139:qual_pointer */
  YYST_TYP, 	/* 140:pointer */
  YYST_TYP, 	/* 141:pointer_opt */
  YYST_DSPECS, 	/* 142:declaration_specifiers1 */
  YYST_DSPECS, 	/* 143:declaration_specifiers1 */
  YYST_DSPECS, 	/* 144:declaration_specifiers1 */
  YYST_DSPECS, 	/* 145:declaration_specifiers1 */
  YYST_TNODE, 	/* 146:declare_tag */
  YYST_TNODE, 	/* 147:declare_tag */
  YYST_DSPECS, 	/* 148:declaration_specifiers2 */
  YYST_DSPECS, 	/* 149:declaration_specifiers2 */
  YYST_DSPECS, 	/* 150:declaration_specifiers2 */
  0, 	/* 151:ensure_file_scope */
  0, 	/* 152:garbage_collect */
  0, 	/* 153:')' */
  0, 	/* 154:')' */
  0, 	/* 155:'{' */
  0, 	/* 156:'{' */
  0, 	/* 157:')' */
  0, 	/* 158:')' */
  0, 	/* 159:'(' */
  YYST_AST, 	/* 160:unary_expression */
  YYST_STR, 	/* 161:any_identifier */
  0, 	/* 162:'(' */
  YYST_AST, 	/* 163:unary_expression */
  YYST_AST, 	/* 164:unary_expression */
  0, 	/* 165:'(' */
  YYST_AST, 	/* 166:unary_expression */
  0, 	/* 167:'{' */
  YYST_AST, 	/* 168:expression */
  0, 	/* 169:type_name */
  0, 	/* 170:compound_statement */
  YYST_AST, 	/* 171:unary_expression */
  YYST_AST, 	/* 172:cast_expression */
  YYST_AST, 	/* 173:cast_expression */
  YYST_AST, 	/* 174:STRING_CONST */
  YYST_AST, 	/* 175:LSTRING_CONST */
  0, 	/* 176:INC */
  0, 	/* 177:DEC */
  0, 	/* 178:PTR */
  0, 	/* 179:'(' */
  0, 	/* 180:'[' */
  0, 	/* 181:'.' */
  0, 	/* 182:ASSIGN */
  0, 	/* 183:'=' */
  0, 	/* 184:assignment_operator */
  YYST_AST, 	/* 185:cast_expression */
  0, 	/* 186:'*' */
  0, 	/* 187:'/' */
  0, 	/* 188:'%' */
  0, 	/* 189:'+' */
  0, 	/* 190:'-' */
  0, 	/* 191:SHIFT */
  0, 	/* 192:ARITHCOMPARE */
  0, 	/* 193:EQCOMPARE */
  0, 	/* 194:'&' */
  0, 	/* 195:'^' */
  0, 	/* 196:'|' */
  0, 	/* 197:LOG_AND */
  0, 	/* 198:LOG_OR */
  0, 	/* 199:'?' */
  0, 	/* 200:',' */
  0, 	/* 201:')' */
  YYST_DSPECS, 	/* 202:type_specifier */
  YYST_DSPECS, 	/* 203:specifier_qualifier_list1 */
  YYST_DSPECS, 	/* 204:type_qualifier */
  YYST_TYP, 	/* 205:GetDSpecsTyp */
  YYST_DSPECS, 	/* 206:specifier_qualifier_list */
  0, 	/* 207:')' */
  YYST_DSPECS, 	/* 208:specifier_qualifier_list2 */
  YYST_DSPECS, 	/* 209:type_qualifier */
  YYST_DSPECS, 	/* 210:@1 */
  YYST_DSPECS, 	/* 211:type_qualifier */
  YYST_DSPECS, 	/* 212:type_qualifier_list */
  YYST_M_TYP, 	/* 213:GenQual_type_qualifier_list */
  YYST_M_TYP, 	/* 214:type_qualifier_list_opt */
  0, 	/* 215:'=' */
  0, 	/* 216:@28 */
  YYST_SCOPE, 	/* 217:declaration_list_scope */
  0, 	/* 218:@30 */
  YYST_TYP, 	/* 219:qual_pointer */
  0, 	/* 220:'(' */
  YYST_STR, 	/* 221:any_identifier */
  YYST_TYP, 	/* 222:left_bra_NullTyp */
  YYST_SYM, 	/* 223:direct_declarator */
  0, 	/* 224:'{' */
  YYST_SCOPE, 	/* 225:struct_scope */
  YYST_SCOPE, 	/* 226:struct_scope */
  0, 	/* 227:garbage_collect */
  YYST_EDESC, 	/* 228:@4 */
  YYST_EDESC, 	/* 229:@5 */
  0, 	/* 230:type_name */
  0, 	/* 231:type_name */
  0, 	/* 232:type_name */
  0, 	/* 233:@27 */
  0, 	/* 234:')' */
  0, 	/* 235:')' */
  0, 	/* 236:')' */
  YYST_STR, 	/* 237:any_identifier */
  0, 	/* 238:argument_expression_list */
  0, 	/* 239:argument_expression_list_opt */
  YYST_AST, 	/* 240:assignment_expression */
  YYST_AST, 	/* 241:expression */
  YYST_STR, 	/* 242:any_identifier */
  YYST_AST, 	/* 243:assignment_expression */
  YYST_AST, 	/* 244:cast_expression */
  YYST_AST, 	/* 245:cast_expression */
  YYST_AST, 	/* 246:cast_expression */
  YYST_AST, 	/* 247:multiplicative_expression */
  YYST_AST, 	/* 248:multiplicative_expression */
  YYST_AST, 	/* 249:additive_expression */
  YYST_AST, 	/* 250:shift_expression */
  YYST_AST, 	/* 251:relational_expression */
  YYST_AST, 	/* 252:equality_expression */
  YYST_AST, 	/* 253:and_expression */
  YYST_AST, 	/* 254:exclusive_or_expression */
  YYST_AST, 	/* 255:inclusive_or_expression */
  YYST_AST, 	/* 256:logical_and_expression */
  YYST_AST, 	/* 257:expression */
  YYST_AST, 	/* 258:expression_opt */
  YYST_AST, 	/* 259:assignment_expression */
  YYST_DSPECS, 	/* 260:specifier_qualifier_list1 */
  YYST_DSPECS, 	/* 261:specifier_qualifier_list1 */
  YYST_TYP, 	/* 262:pointer */
  YYST_TYP, 	/* 263:pointer_opt */
  YYST_SYM, 	/* 264:abstract_declarator */
  YYST_DSPECS, 	/* 265:specifier_qualifier_list2 */
  YYST_TYP, 	/* 266:GetDSpecsTyp */
  0, 	/* 267:init_declarator */
  YYST_DSPECS, 	/* 268:type_qualifier */
  0, 	/* 269:'{' */
  YYST_AST, 	/* 270:assignment_expression */
  0, 	/* 271:initializer */
  0, 	/* 272:'{' */
  0, 	/* 273:function_compound_statement */
  0, 	/* 274:@29 */
  0, 	/* 275:error */
  0, 	/* 276:declaration */
  YYST_DSPECS, 	/* 277:GenType_declaration_specifiers */
  0, 	/* 278:declaration_list */
  YYST_TYP, 	/* 279:@16 */
  YYST_SYM, 	/* 280:declarator */
  0, 	/* 281:@2 */
  0, 	/* 282:error */
  YYST_STR, 	/* 283:any_identifier */
  0, 	/* 284:enumerator_list */
  0, 	/* 285:enumerator */
  YYST_STR, 	/* 286:enumeration_constant */
  0, 	/* 287:enumerator_list */
  0, 	/* 288:')' */
  0, 	/* 289:')' */
  0, 	/* 290:')' */
  0, 	/* 291:LABEL */
  0, 	/* 292:declare_labels_list */
  0, 	/* 293:declare_labels_list_opt */
  0, 	/* 294:declare_labels */
  0, 	/* 295:'{' */
  YYST_AST, 	/* 296:cast_expression */
  0, 	/* 297:',' */
  0, 	/* 298:')' */
  0, 	/* 299:']' */
  0, 	/* 300:':' */
  0, 	/* 301:'(' */
  0, 	/* 302:'[' */
  YYST_SCOPE, 	/* 303:param_scope */
  YYST_TYP, 	/* 304:left_bra_NullTyp */
  YYST_TYP, 	/* 305:direct_abstract_declarator_tail */
  YYST_SYM, 	/* 306:direct_abstract_declarator */
  YYST_SYM, 	/* 307:declarator */
  0, 	/* 308:'[' */
  0, 	/* 309:'.' */
  0, 	/* 310:initializer_list */
  0, 	/* 311:designation */
  0, 	/* 312:designation_opt */
  0, 	/* 313:designator_list */
  0, 	/* 314:designator */
  0, 	/* 315:declare_labels_list_opt */
  0, 	/* 316:function_compound_statement */
  YYST_TYP, 	/* 317:GetDSpecsTyp */
  0, 	/* 318:declaration */
  0, 	/* 319:'(' */
  0, 	/* 320:'[' */
  YYST_TYP, 	/* 321:direct_declarator_tail */
  YYST_SCOPE, 	/* 322:param_scope */
  YYST_SCOPE, 	/* 323:identifier_list_scope */
  0, 	/* 324:')' */
  0, 	/* 325:error */
  YYST_DSPECS, 	/* 326:GenType_declaration_specifiers */
  0, 	/* 327:struct_declaration_list */
  0, 	/* 328:struct_declaration */
  0, 	/* 329:',' */
  0, 	/* 330:comma_opt */
  0, 	/* 331:'=' */
  0, 	/* 332:comma_opt */
  YYST_STR, 	/* 333:any_identifier */
  0, 	/* 334:declare_labels */
  0, 	/* 335:SWITCH */
  0, 	/* 336:CONTINUE */
  0, 	/* 337:CASE */
  0, 	/* 338:DEFAULT */
  0, 	/* 339:WHILE */
  0, 	/* 340:DO */
  0, 	/* 341:GOTO */
  0, 	/* 342:BREAK */
  0, 	/* 343:IF */
  0, 	/* 344:FOR */
  0, 	/* 345:RETURN */
  YYST_STR, 	/* 346:IDENT */
  YYST_SYM, 	/* 347:TYPENAME */
  YYST_STR, 	/* 348:any_identifier */
  YYST_AST, 	/* 349:expression_opt */
  0, 	/* 350:declaration */
  0, 	/* 351:statement */
  0, 	/* 352:labeled_statement */
  0, 	/* 353:compound_statement */
  0, 	/* 354:block_item_list */
  0, 	/* 355:block_item_list_opt */
  0, 	/* 356:block_item */
  0, 	/* 357:expression_statement */
  0, 	/* 358:selection_statement */
  0, 	/* 359:iteration_statement */
  0, 	/* 360:jump_statement */
  0, 	/* 361:',' */
  0, 	/* 362:';' */
  0, 	/* 363:initializer_list */
  YYST_AST, 	/* 364:assignment_expression */
  YYST_AST, 	/* 365:conditional_expression */
  0, 	/* 366:')' */
  0, 	/* 367:@14 */
  0, 	/* 368:'*' */
  YYST_AST, 	/* 369:assignment_expression */
  YYST_AST, 	/* 370:assignment_expression_opt */
  YYST_TYP, 	/* 371:@25 */
  YYST_SYM, 	/* 372:abstract_declarator */
  YYST_AST, 	/* 373:conditional_expression */
  YYST_AST, 	/* 374:constant_expression */
  YYST_STR, 	/* 375:any_identifier */
  0, 	/* 376:',' */
  0, 	/* 377:comma_opt */
  0, 	/* 378:initializer */
  0, 	/* 379:'=' */
  0, 	/* 380:designator */
  0, 	/* 381:block_item_list_opt */
  0, 	/* 382:')' */
  0, 	/* 383:@15 */
  YYST_TOKEN, 	/* 384:C_STATIC */
  YYST_M_TYP, 	/* 385:GenQual_type_qualifier_list */
  YYST_M_TYP, 	/* 386:type_qualifier_list_opt */
  YYST_TYP, 	/* 387:@11 */
  YYST_TYP, 	/* 388:@13 */
  YYST_TYP, 	/* 389:@17 */
  YYST_TYP, 	/* 390:GetDSpecsTyp */
  0, 	/* 391:struct_declarator_list */
  0, 	/* 392:struct_declarator */
  0, 	/* 393:'}' */
  0, 	/* 394:struct_declaration */
  YYST_EDESC, 	/* 395:@6 */
  0, 	/* 396:'}' */
  YYST_AST, 	/* 397:constant_expression */
  0, 	/* 398:'}' */
  0, 	/* 399:';' */
  0, 	/* 400:'(' */
  0, 	/* 401:';' */
  YYST_AST, 	/* 402:constant_expression */
  0, 	/* 403:':' */
  0, 	/* 404:'(' */
  0, 	/* 405:statement */
  0, 	/* 406:'*' */
  YYST_STR, 	/* 407:any_identifier */
  0, 	/* 408:';' */
  0, 	/* 409:'(' */
  0, 	/* 410:'(' */
  YYST_AST, 	/* 411:expression_opt */
  0, 	/* 412:':' */
  0, 	/* 413:';' */
  0, 	/* 414:block_item */
  0, 	/* 415:'}' */
  YYST_STR, 	/* 416:any_identifier */
  0, 	/* 417:comma_opt */
  YYST_TYP, 	/* 418:@24 */
  0, 	/* 419:error */
  0, 	/* 420:forward_parameter_decl */
  0, 	/* 421:parameter_type_list */
  0, 	/* 422:']' */
  0, 	/* 423:']' */
  0, 	/* 424:'(' */
  YYST_TYP, 	/* 425:direct_abstract_declarator_tail */
  YYST_TYP, 	/* 426:direct_abstract_declarator_tail_opt */
  0, 	/* 427:')' */
  0, 	/* 428:ELLIPSIS */
  0, 	/* 429:']' */
  0, 	/* 430:designation_opt */
  0, 	/* 431:'}' */
  0, 	/* 432:'}' */
  YYST_TYP, 	/* 433:@12 */
  YYST_STR, 	/* 434:identifier */
  0, 	/* 435:identifier_list */
  YYST_M_TYP, 	/* 436:type_qualifier_list_opt */
  YYST_TOKEN, 	/* 437:C_STATIC */
  0, 	/* 438:'*' */
  YYST_AST, 	/* 439:assignment_expression_opt */
  YYST_TYP, 	/* 440:direct_declarator_tail */
  YYST_TYP, 	/* 441:direct_declarator_tail */
  YYST_TYP, 	/* 442:direct_declarator_tail */
  0, 	/* 443:':' */
  YYST_SYM, 	/* 444:declarator */
  0, 	/* 445:',' */
  0, 	/* 446:';' */
  0, 	/* 447:enumerator */
  YYST_AST, 	/* 448:expression */
  0, 	/* 449:ELLIPSIS */
  0, 	/* 450:':' */
  0, 	/* 451:statement */
  YYST_AST, 	/* 452:expression */
  0, 	/* 453:WHILE */
  YYST_AST, 	/* 454:cast_expression */
  0, 	/* 455:';' */
  YYST_AST, 	/* 456:expression */
  YYST_AST, 	/* 457:expression_opt */
  0, 	/* 458:declaration */
  0, 	/* 459:';' */
  0, 	/* 460:statement */
  0, 	/* 461:'}' */
  YYST_TYP, 	/* 462:direct_abstract_declarator_tail_opt */
  0, 	/* 463:ELLIPSIS */
  YYST_DSPECS, 	/* 464:GenType_declaration_specifiers */
  0, 	/* 465:parameter_list */
  0, 	/* 466:parameter_declaration */
  0, 	/* 467:')' */
  YYST_TYP, 	/* 468:@23 */
  YYST_TYP, 	/* 469:@22 */
  YYST_TYP, 	/* 470:@26 */
  YYST_AST, 	/* 471:constant_expression */
  0, 	/* 472:initializer */
  YYST_TYP, 	/* 473:direct_declarator_tail */
  0, 	/* 474:',' */
  0, 	/* 475:')' */
  YYST_AST, 	/* 476:assignment_expression */
  YYST_AST, 	/* 477:assignment_expression */
  0, 	/* 478:']' */
  0, 	/* 479:']' */
  YYST_AST, 	/* 480:constant_expression */
  0, 	/* 481:':' */
  YYST_DSPECS, 	/* 482:@3 */
  0, 	/* 483:')' */
  YYST_AST, 	/* 484:constant_expression */
  0, 	/* 485:statement */
  0, 	/* 486:')' */
  0, 	/* 487:'(' */
  0, 	/* 488:';' */
  0, 	/* 489:')' */
  0, 	/* 490:';' */
  YYST_AST, 	/* 491:expression_opt */
  YYST_TYP, 	/* 492:GetDSpecsTyp */
  0, 	/* 493:',' */
  YYST_TYP, 	/* 494:direct_abstract_declarator_tail_opt */
  YYST_TYP, 	/* 495:direct_abstract_declarator_tail_opt */
  YYST_TYP, 	/* 496:direct_abstract_declarator_tail_opt */
  0, 	/* 497:']' */
  YYST_STR, 	/* 498:any_identifier */
  0, 	/* 499:']' */
  0, 	/* 500:']' */
  YYST_TYP, 	/* 501:@10 */
  YYST_TYP, 	/* 502:@7 */
  YYST_AST, 	/* 503:constant_expression */
  0, 	/* 504:struct_declarator */
  0, 	/* 505:statement */
  0, 	/* 506:':' */
  0, 	/* 507:statement */
  YYST_AST, 	/* 508:expression */
  0, 	/* 509:statement */
  YYST_AST, 	/* 510:expression_opt */
  0, 	/* 511:';' */
  YYST_SYM, 	/* 512:declarator1 */
  YYST_TYP, 	/* 513:pointer_opt */
  YYST_SYM, 	/* 514:abstract_declarator */
  0, 	/* 515:parameter_declaration */
  YYST_TYP, 	/* 516:@8 */
  YYST_TYP, 	/* 517:@9 */
  YYST_TYP, 	/* 518:direct_declarator_tail */
  YYST_TYP, 	/* 519:direct_declarator_tail */
  0, 	/* 520:statement */
  0, 	/* 521:')' */
  0, 	/* 522:ELSE */
  0, 	/* 523:';' */
  YYST_AST, 	/* 524:expression_opt */
  YYST_STR, 	/* 525:any_identifier */
  YYST_TYP, 	/* 526:left_bra_NullTyp */
  YYST_SYM, 	/* 527:direct_declarator1 */
  YYST_TYP, 	/* 528:direct_declarator_tail */
  YYST_TYP, 	/* 529:direct_declarator_tail */
  0, 	/* 530:';' */
  0, 	/* 531:statement */
  YYST_AST, 	/* 532:expression_opt */
  0, 	/* 533:')' */
  YYST_TYP, 	/* 534:@18 */
  YYST_SYM, 	/* 535:declarator2 */
  YYST_TYP, 	/* 536:pointer_opt */
  0, 	/* 537:')' */
  0, 	/* 538:statement */
  YYST_TYP, 	/* 539:direct_declarator_tail */
  0, 	/* 540:')' */
  YYST_STR, 	/* 541:identifier */
  YYST_TYP, 	/* 542:left_bra_NullTyp */
  YYST_SYM, 	/* 543:direct_declarator2 */
  0, 	/* 544:statement */
  YYST_TYP, 	/* 545:@19 */
  YYST_TYP, 	/* 546:@20 */
  YYST_SYM, 	/* 547:declarator2 */
  YYST_TYP, 	/* 548:direct_declarator_tail */
  YYST_TYP, 	/* 549:direct_declarator_tail */
  0, 	/* 550:')' */
  YYST_TYP, 	/* 551:@21 */
  YYST_TYP, 	/* 552:direct_declarator_tail */
  0, 	/* 553:start */
  0, 	/* 554:$ */
  0, 	/* 555:$ */
};

#endif


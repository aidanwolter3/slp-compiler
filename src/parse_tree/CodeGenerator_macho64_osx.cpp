#include "../ult.h"
#include "CodeGenerator_macho64_osx.h"

const char* CodeGenerator_macho64_osx::head =
                           "global start\n"
                           "section .text\n";

CodeGenerator_macho64_osx::CodeGenerator_macho64_osx(SymbolTable *symbolTable, char *output) {

  //grab the symbol table and calculate variable locations
  this->symbolTable = symbolTable;
  this->symbolTable->calculate_locations();

  //copy the output location
  strcpy(this->output, output);

  //initialize the code
  code = (char*)malloc(2000*sizeof(char));
  len = 0;

  //set that no registers are in use
  regs.eax = 0;
  regs.ebx = 0;
  regs.ecx = 0;
  regs.edx = 0;

  len += sprintf(code+len, "%s", head);

  len += sprintf(code+len, "\nstart:\n"
                           "mov rbp,rsp\n"
                           "sub rsp,%d\n", symbolTable->data_size());
}
const char* CodeGenerator_macho64_osx::next_reg() {
  if(regs.eax == 0) {
    const char *eax = "eax";
    regs.eax = 1;
    return eax;
  }
  if(regs.ebx == 0) {
    const char *ebx = "ebx";
    regs.ebx = 1;
    return ebx;
  }
  if(regs.ecx == 0) {
    const char *ecx = "ecx";
    regs.ecx = 1;
    return ecx;
  }
  if(regs.edx == 0) {
    const char *edx = "edx";
    regs.edx = 1;
    return edx;
  }
  const char *noreg = "";
  return noreg;
}
void CodeGenerator_macho64_osx::release_reg(const char *reg) {
  const char *eax = "eax";
  const char *ebx = "ebx";
  const char *ecx = "ecx";
  const char *edx = "edx";
  if(strcmp(reg, eax) == 0) {
    regs.eax = 0;
  }
  else if(strcmp(reg, ebx) == 0) {
    regs.ebx = 0;
  }
  else if(strcmp(reg, ecx) == 0) {
    regs.ecx = 0;
  }
  else if(strcmp(reg, edx) == 0) {
    regs.edx = 0;
  }
}
void CodeGenerator_macho64_osx::write_exit() {
  len += sprintf(code+len, "add rsp,%d      ; move stack past local vars\n"
                           "\n;exit\n"
                           "mov rax,0x2000001\n"
                           "mov rdi,0\n"
                           "syscall\n", symbolTable->data_size());
}
void CodeGenerator_macho64_osx::print_code() {
  printf("%s", code);
}
void CodeGenerator_macho64_osx::write_code() {
  FILE *f = fopen(output, "w");
  fprintf(f, "%s", code);
  fclose(f);
}
void* CodeGenerator_macho64_osx::visit(CompoundStatement *s) {
  s->stm1->accept(this);
  s->stm2->accept(this);
  return new CodeReturn(0, STATEMENT_PAIR_PROD);
}
void* CodeGenerator_macho64_osx::visit(AssignStatement *stm) {
  CodeReturn *c1 = (CodeReturn*)stm->id->accept(this);
  CodeReturn *c2 = (CodeReturn*)stm->exp->accept(this);

  if(c2->type == EXPRESSION_STR_PROD) {
    char *str = c2->tmp;
    Symbol *sym = symbolTable->get(stm->id->lexem);
    for(int i = 0; i < strlen(str); i++) {
      len += sprintf(code+len, "mov dword [rbp-%d],%d ; str assign\n", (sym->loc)-(i*4), str[i]);
    }
  }
  else {
    len += sprintf(code+len, "mov dword %s,%s ; assign\n", c1->tmp, c2->tmp);
    release_reg(c2->tmp);
  }

  return new CodeReturn(0, STATEMENT_ASGN_PROD);
}
void* CodeGenerator_macho64_osx::visit(PrintStatement *stm) {
  CodeReturn *c = (CodeReturn*)stm->exp->accept(this);

  //get the symbol so that we know how long to print
  Symbol *sym = symbolTable->get(((IdExpression*)(stm->exp))->lexem);

  len += sprintf(code+len, "mov rax,0x2000004 ; huck\n"
                           "mov rdi,1\n"
                           "lea rsi,%s\n"
                           "mov rdx,%d\n"
                           "syscall\n", c->tmp, sym->size);

  release_reg(c->tmp);

  return new CodeReturn(0, STATEMENT_HUCK_PROD);
}
void* CodeGenerator_macho64_osx::visit(IdExpression *exp) {
  CodeReturn *c = new CodeReturn(0, EXPRESSION_ID_PROD);
  Symbol *sym = symbolTable->get(exp->lexem);
  sprintf(c->tmp, "[rbp-%d]", sym->loc);
  return c;
}
void* CodeGenerator_macho64_osx::visit(NumExpression *exp) {
  const char *reg = next_reg();
  len += sprintf(code+len, "mov %s,%d\n", reg, exp->val);

  CodeReturn *c = new CodeReturn(4, EXPRESSION_NUM_PROD);
  strcpy(c->tmp, reg);
  return c;
}
void* CodeGenerator_macho64_osx::visit(StrExpression *exp) {
  CodeReturn *c = new CodeReturn(0, EXPRESSION_STR_PROD);
  strcpy(c->tmp, exp->val);
  return c;
}
void* CodeGenerator_macho64_osx::visit(OperationExpression *exp) {
  CodeReturn *c1 = (CodeReturn*)exp->exp1->accept(this);
  CodeReturn *c2 = (CodeReturn*)exp->op->accept(this);
  CodeReturn *c3 = (CodeReturn*)exp->exp2->accept(this);

  const char *reg = next_reg();

  if(c2->type == OPERATION_PLUS_PROD) {
    len += sprintf(code+len, "mov %s,%s     ; +\n"
                             "add %s,%s\n", reg, c1->tmp, reg, c3->tmp);
  }
  else if(c2->type == OPERATION_MINUS_PROD) {
    len += sprintf(code+len, "mov %s,%s     ; -\n"
                             "sub %s,%s\n", reg, c1->tmp, reg, c3->tmp);
  }
  else if(c2->type == OPERATION_DIV_PROD) {
    len += sprintf(code+len, "push rax      ; /\n"
                             "push rdx\n"
                             "mov rax,%s\n"
                             "div qword %s\n"
                             "push %s\n"
                             "add rsp,4\n"
                             "pop rdx\n"
                             "pop rax\n"
                             "mov %s,[rsp-12]\n", c1->tmp, c3->tmp, c1->tmp, reg);
  }
  else if (c2->type == OPERATION_MULT_PROD) {
    len += sprintf(code+len, "push rax      ; *\n"
                             "push rdx\n"
                             "mov rax,%s\n"
                             "mul qword %s\n"
                             "push %s\n"
                             "add rsp,4\n"
                             "pop rdx\n"
                             "pop rax\n"
                             "mov %s,[rsp-12]\n", c1->tmp, c3->tmp, c1->tmp, reg);
  }

  release_reg(c1->tmp);
  release_reg(c3->tmp);

  CodeReturn *c = new CodeReturn(0, EXPRESSION_OPR_PROD);
  strcpy(c->tmp, reg);
  return c;
}
void* CodeGenerator_macho64_osx::visit(PairExpressionList *exp) {
  exp->exp->accept(this);
  CodeReturn *c2 = (CodeReturn*)exp->list->accept(this);
  CodeReturn *c = new CodeReturn(0, LIST_PAIR_PROD);
  strcpy(c->tmp, c2->tmp);
  return c;
}
void* CodeGenerator_macho64_osx::visit(LastExpressionList *exp) {
  CodeReturn *c1 = (CodeReturn*)exp->exp->accept(this);
  CodeReturn *c = new CodeReturn(0, LIST_END_PROD);
  strcpy(c->tmp, c1->tmp);
  return c;
}
void* CodeGenerator_macho64_osx::visit(Plus *op) {
  return new CodeReturn(0, OPERATION_PLUS_PROD);
}
void* CodeGenerator_macho64_osx::visit(Minus *op) {
  return new CodeReturn(0, OPERATION_MINUS_PROD);
}
void* CodeGenerator_macho64_osx::visit(Divide *op) {
  return new CodeReturn(0, OPERATION_DIV_PROD);
}
void* CodeGenerator_macho64_osx::visit(Multiply *op) {
  return new CodeReturn(0, OPERATION_MULT_PROD);
}

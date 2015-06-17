#include "CodeGenerator_x86.h"

const char *head_mac =     "global start\n"
                           "section .text\n";
const char *head_ubu =     "global _start\n"
                           "section .text\n";
const char *putint =       "\nputint:\n"
                           "push eax\n"
                           "push ebx\n"
                           "push ecx\n"
                           "push edx\n"
                           "mov eax,dword[esp+20]\n"
                           "mov ecx,0\n"
                           "push 0\n"
                           "push 0x0A\n"
                           "mov ebx,10\n"
                           ".putint0:\n"
                           "add ecx,1\n"
                           "mov dx,0\n"
                           "div bx\n"
                           "add edx,0x30\n"
                           "push edx\n"
                           "cmp ax,0\n"
                           "jz .putint1\n"
                           "jmp .putint0\n"
                           ".putint1:\n"
                           "push dword esp\n"
                           "call putstr\n"
                           "add esp,4\n"
                           "lea ecx,[8+4*ecx]\n"
                           "add esp,ecx\n"
                           "pop edx\n"
                           "pop ecx\n"
                           "pop ebx\n"
                           "pop eax\n"
                           "ret\n";
const char *putstr =       "\nputstr:\n"
                           "push eax\n"
                           "mov esi,dword[esp+8]\n"
                           ".putstr0:\n"
                           "mov byte al,[esi]\n"
                           "cmp al,0\n"
                           "je .putstr1\n"
                           "push eax\n"
                           "call putchar\n"
                           "add esp,4\n"
                           "add esi,4\n"
                           "jmp .putstr0\n"
                           ".putstr1:\n"
                           "pop eax\n"
                           "ret\n";
const char *putchar_mac =  "\nputchar:\n"
                           "push eax\n"
                           "push ebx\n"
                           "push ecx\n"
                           "push edx\n"
                           "push dword [esp+20]\n"
                           "mov ebx,1\n"
                           "mov edx,esp\n"
                           "push dword 1\n"
                           "push dword edx\n"
                           "push dword 1\n"
                           "mov eax,4\n"
                           "sub esp,4\n"
                           "int 0x80\n"
                           "add esp,20\n"
                           "pop edx\n"
                           "pop ecx\n"
                           "pop ebx\n"
                           "pop eax\n"
                           "ret\n";
const char *putchar_ubu =  "\nputchar:\n"
                           "push eax\n"
                           "push ebx\n"
                           "push ecx\n"
                           "push edx\n"
                           "push dword [esp+20]\n"
                           "mov ebx,1\n"
                           "mov ecx,esp\n"
                           "mov edx,1\n"
                           "mov eax,4\n"
                           "int 0x80\n"
                           "add esp,4\n"
                           "pop edx\n"
                           "pop ecx\n"
                           "pop ebx\n"
                           "pop eax\n"
                           "ret\n";

CodeGenerator_x86::CodeGenerator_x86(const char *target, SymbolTable *symbolTable) {

  //grab the symbol table
  this->symbolTable = symbolTable;

  //initialize the code
  code = (char*)malloc(2000*sizeof(char));
  len = 0;

  //set that no registers are in use
  regs.eax = 0;
  regs.ebx = 0;
  regs.ecx = 0;
  regs.edx = 0;

  const char *target_mac = "mac";
  const char *target_ubu = "ubu";

  //add the necessary functions for printing an integer
  if(strcmp(target, target_mac) == 0) {
    len += sprintf(code+len, "%s", head_mac);
  }
  else if(strcmp(target, target_ubu) == 0) {
    len += sprintf(code+len, "%s", head_ubu);
  }

  len += sprintf(code+len, "%s", putint);
  len += sprintf(code+len, "%s", putstr);
  
  if(strcmp(target, target_mac) == 0) {
    len += sprintf(code+len, "%s", putchar_mac);
    len += sprintf(code+len, "\nstart:\n"
                             "mov ebp,esp\n"
                             "sub esp,%d\n", symbolTable->size*4);
  }
  else if (strcmp(target, target_ubu) == 0) {
    len += sprintf(code+len, "%s", putchar_ubu);
    len += sprintf(code+len, "\n_start:\n"
                             "mov ebp,esp\n"
                             "sub esp,%d\n", symbolTable->size*4);
  }
}
const char* CodeGenerator_x86::next_reg() {
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
void CodeGenerator_x86::release_reg(const char *reg) {
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
void CodeGenerator_x86::write_exit() {
  len += sprintf(code+len, "add esp,%d\n"
                           "\n;exit\n"
                           "push dword 0\n"
                           "mov eax,1\n"
                           "sub esp,12\n"
                           "int 0x80\n"
                           "add esp,4\n", symbolTable->size*4);
}
void CodeGenerator_x86::print_code() {
  printf("%s", code);
}
void CodeGenerator_x86::write_code() {
  FILE *f = fopen("output.asm", "w");
  fprintf(f, "%s", code);
  fclose(f);
}
void* CodeGenerator_x86::visit(CompoundStatement *s) {
  s->stm1->accept(this);
  s->stm2->accept(this);
  return new CodeReturn(0, 0);
}
void* CodeGenerator_x86::visit(AssignStatement *stm) {
  CodeReturn *c1 = (CodeReturn*)stm->id->accept(this);
  CodeReturn *c2 = (CodeReturn*)stm->exp->accept(this);

  len += sprintf(code+len, "mov %s,%s\n", c1->tmp, c2->tmp);

  release_reg(c2->tmp);

  return new CodeReturn(0, 1);
}
void* CodeGenerator_x86::visit(PrintStatement *stm) {
  CodeReturn *c = (CodeReturn*)stm->exp->accept(this);

  len += sprintf(code+len, "push dword %s\n", c->tmp);
  len += sprintf(code+len, "call putint\n");

  release_reg(c->tmp);

  return new CodeReturn(0, 2);
}
void* CodeGenerator_x86::visit(IdExpression *exp) {
  CodeReturn *c = new CodeReturn(0, 3);
  Symbol *sym = symbolTable->get(exp->lexem);
  sprintf(c->tmp, "[ebp-%d]", sym->loc);
  return c;
}
void* CodeGenerator_x86::visit(NumExpression *exp) {
  const char *reg = next_reg();
  len += sprintf(code+len, "mov %s,%d\n", reg, exp->val);

  CodeReturn *c = new CodeReturn(4, 4);
  strcpy(c->tmp, reg);
  return c;
}
void* CodeGenerator_x86::visit(OperationExpression *exp) {
  CodeReturn *c1 = (CodeReturn*)exp->exp1->accept(this);
  CodeReturn *c2 = (CodeReturn*)exp->op->accept(this);
  CodeReturn *c3 = (CodeReturn*)exp->exp2->accept(this);

  const char *reg = next_reg();

  if(c2->type == 9) {
    len += sprintf(code+len, "mov %s,%s\n"
                             "add %s,%s\n", reg, c1->tmp, reg, c3->tmp);
  }
  else if(c2->type == 10) {
    len += sprintf(code+len, "mov %s,%s\n"
                             "sub %s,%s\n", reg, c1->tmp, reg, c3->tmp);
  }
  else if(c2->type == 11) {
    len += sprintf(code+len, "push eax\n"
                             "push edx\n"
                             "mov eax,%s\n"
                             "div dword %s\n"
                             "push %s\n"
                             "add esp,4\n"
                             "pop edx\n"
                             "pop eax\n"
                             "mov %s,[esp-12]\n", c1->tmp, c3->tmp, c1->tmp, reg);
  }
  else if (c2->type == 12) {
    len += sprintf(code+len, "push eax\n"
                             "push edx\n"
                             "mov eax,%s\n"
                             "mul dword %s\n"
                             "push %s\n"
                             "add esp,4\n"
                             "pop edx\n"
                             "pop eax\n"
                             "mov %s,[esp-12]\n", c1->tmp, c3->tmp, c1->tmp, reg);
  }

  release_reg(c1->tmp);
  release_reg(c3->tmp);

  CodeReturn *c = new CodeReturn(0, 5);
  strcpy(c->tmp, reg);
  return c;
}
void* CodeGenerator_x86::visit(PairExpressionList *exp) {
  exp->exp->accept(this);
  CodeReturn *c2 = (CodeReturn*)exp->list->accept(this);
  CodeReturn *c = new CodeReturn(0, 7);
  strcpy(c->tmp, c2->tmp);
  return c;
}
void* CodeGenerator_x86::visit(LastExpressionList *exp) {
  CodeReturn *c1 = (CodeReturn*)exp->exp->accept(this);
  CodeReturn *c = new CodeReturn(0, 8);
  strcpy(c->tmp, c1->tmp);
  return c;
}
void* CodeGenerator_x86::visit(Plus *op) {
  return new CodeReturn(0, 9);
}
void* CodeGenerator_x86::visit(Minus *op) {
  return new CodeReturn(0, 10);
}
void* CodeGenerator_x86::visit(Divide *op) {
  return new CodeReturn(0, 11);
}
void* CodeGenerator_x86::visit(Multiply *op) {
  return new CodeReturn(0, 12);
}

#ifndef __CODEOPT_H__
#define __CODEOPT_H__

/**** Code Optimization Phase for E-- Compiler ****/

class InterCodesClass;

class CodeOpt {
    
public:
    CodeOpt()   {}
    //InterCodesClass* codeOptimization(InterCodesClass* org);

    InterCodesClass* peephole_optimization(InterCodesClass* org, bool* optStatus);
    //InterCodesClass* constant_folding(InterCodesClass* org);
    
    void ifElseJumpOptimization ( vector<InterCode*>* in, bool* optStatus = NULL);

};
#endif
    

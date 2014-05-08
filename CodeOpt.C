#include "InterCode.h"
#include "CodeOpt.h"
#include "all.h"
#include <sstream>

InterCodesClass* CodeOpt::codeOptimization(InterCodesClass* org) {

    cout << "\n Basic Block Code Optimization \n";

    bool optStatus = true;

    InterCodesClass* opt;
    opt = constant_folding(org);
    //opt = peephole_optimization(org, &(optStatus));
    return opt;
}

InterCodesClass* CodeOpt::constant_folding(InterCodesClass* org) {

    cout<< "\n Constant Folding Optimization \n";
    int result = 0;
    unsigned int i;
    
    //InterCodesClass* out = new InterCodesClass();
    vector<InterCode*> *tempICodeVector, *dupICodeVector;

    dupICodeVector = org->getICodeVector();
    tempICodeVector = new vector<InterCode*> ();
    
    //ExprNode** tempOperands;
    
    for ( i = 0; i < dupICodeVector->size(); i++)
    {
            tempICodeVector->push_back(dupICodeVector->at(i)); 

        if (dupICodeVector->at(i)->getOPNType() != InterCode::OPNTYPE::EXPR)
        {
            continue;
        }
        ExprNode** operands = (ExprNode**)dupICodeVector->at(i)->get3Operands();
     
        if (operands[0] && operands[1] && operands[2]) {
            ExprNode* new1 = operands[1]; 
            ExprNode* new2 = operands[2]; 

            if ((new1->exprNodeType() == ExprNode::ExprNodeType::VALUE_NODE) && (new2->exprNodeType() == ExprNode::ExprNodeType::VALUE_NODE))   {
                cout << operands[0]->getRefName() << " = ";
                switch(dupICodeVector->at(i)->getsubCode()) {
                    case OpNode::OpCode::PLUS :
                                        result = atoi(operands[1]->getRefName().c_str()) + atoi(operands[2]->getRefName().c_str());
                                        break;
                    case OpNode::OpCode::MINUS :
                                        result = atoi(operands[1]->getRefName().c_str()) - atoi(operands[2]->getRefName().c_str());
                                        break;
                    case OpNode::OpCode::MULT :
                                        result = atoi(operands[1]->getRefName().c_str()) * atoi(operands[2]->getRefName().c_str());
                                        break;
                    case OpNode::OpCode::DIV :
                                        result = atoi(operands[1]->getRefName().c_str()) / atoi(operands[2]->getRefName().c_str());
                                        break;
                    case OpNode::OpCode::MOD :
                                        result = atoi(operands[1]->getRefName().c_str()) % atoi(operands[2]->getRefName().c_str());
                                        break;
                    case OpNode::OpCode::BITOR :
                                        result = atoi(operands[1]->getRefName().c_str()) | atoi(operands[2]->getRefName().c_str());
                                        break;
                    case OpNode::OpCode::BITAND :
                                        result = atoi(operands[1]->getRefName().c_str()) & atoi(operands[2]->getRefName().c_str());
                                        break;
                    case OpNode::OpCode::BITXOR :
                                        result = atoi(operands[1]->getRefName().c_str()) ^ atoi(operands[2]->getRefName().c_str());
                                        break;
                    case OpNode::OpCode::SHL :
                                        result = atoi(operands[1]->getRefName().c_str()) << atoi(operands[2]->getRefName().c_str());
                                        break;
                    case OpNode::OpCode::SHR :
                                        result = atoi(operands[1]->getRefName().c_str()) >> atoi(operands[2]->getRefName().c_str());
                                        break;
                    default : 
                            cout << "\nUnhandled OpCode \n";
                            break;
                }
                cout << result << "\n";
                Value *temp = new Value(result, Type::UINT);
                new1->exprVal(temp);
                Value *temp1 = new Value(0, Type::UINT);
                new2->exprVal(temp1);
                operands[1] = new1;
                operands[2] = new2;
                tempICodeVector->at(i)->setSubCode(OpNode::OpCode::ASSIGN);
                 
            }
        }
    }

    org->setICodeVector(tempICodeVector);
    cout << "\nReturned \n";
    return org;

}

InterCodesClass* CodeOpt::peephole_optimization(InterCodesClass* org, bool* optStatus)    {

    cout << "\n Peep Hole Optimization \n";
    
    unsigned int i;
    InterCodesClass* opt = new InterCodesClass();
    vector<InterCode*> *tempICodeVector, *dupICodeVector;
    
    *optStatus = false;
    
    tempICodeVector = new vector<InterCode*> ();
    dupICodeVector = org->getICodeVector();

    tempICodeVector->push_back(dupICodeVector->at(0));
    tempICodeVector->push_back(dupICodeVector->at(1));
    
    for (i = 2; i < dupICodeVector->size(); i++) {
        tempICodeVector->push_back(dupICodeVector->at(i));
        ifElseJumpOptimization(tempICodeVector);
        if(tempICodeVector->size() == 3) {
            opt->addCode(tempICodeVector->at(0));
            tempICodeVector->erase(tempICodeVector->begin());
        }
        else
            *optStatus = true;
    }

    for( i = 0; i < tempICodeVector->size(); i++)
            opt->addCode(tempICodeVector->at(i));

    return opt;

}

void CodeOpt::ifElseJumpOptimization ( vector<InterCode*>* in, bool* optStatus) {
	/*Optimize for
	 *
	 Modify:
	 if a goto L1
	 goto L2
	 L1:

	 To:
	 if a == 0 goto L2
	 L6:

	 */
        cout << "\n If Then Else  Optimization \n";

	InterCode *if_node, *middle_node, *label_node, *if_node_new;

	if (in->size() != 3)
		return;

	if ((in->at(0)->getOPNType() == InterCode::IFREL) && (in->at(2)->getOPNType() == InterCode::LABEL))  {
		if_node = in->at(0);
		middle_node = in->at(1);
		label_node = in->at(2);

		void** operands = if_node->get3Operands();
		string if_label = ((InterCode*) (operands[0]))->getLabel();
		InterCode* new_label;

		int id = (long) (label_node->get3Operands()[0]);
		string *name = (string*) (label_node->get3Operands()[1]);
		ostringstream os;
		if (name && name->length())
			os << *name;
		else
			os << "L" << id;

		if ((middle_node->getOPNType() != InterCode::GOTO) || (if_label != os.str())) {
			return;
		}

		void** opnd1 = middle_node->get3Operands();
		new_label = ((InterCode*) (opnd1[0]));

		in->clear();

		/*if (if_node->getType() == ImmCode::IF)
			if_node_new = new ImmCode(ImmCode::IF_REL, OpNode::EQ, new_label,
					opnd[1], (void*) new ValueNode(new Value(0, Type::INT)));
		else {*/
		if_node_new = if_node;
		operands[0] = new_label;
		if_node_new->xchgSubcode();
		//}

		in->push_back(if_node_new);
		in->push_back(label_node);

	}

	return;

}


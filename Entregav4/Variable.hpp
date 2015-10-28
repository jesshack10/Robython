//
//  Variable.h
//  Tabla_Variables
//
//  Created by Joan Lauren Eugenio Silva on 25/10/15.
//  Copyright © 2015 Joan Lauren Eugenio Silva. All rights reserved.
//

//#ifndef Variable_h
//#define Variable_h
#include <iostream>
#include <string>
using namespace std; 
class Variable
{
public:
    
    //Setters
    void setId (string name);
    void setKey (int numScope);

    
    //Getters
    string getId();
    int getKey();
    
    //Constructors
    Variable();
    Variable(string name, int numScope);
    
    //Extra Functions
    void muestra();
    
private:
    string id;
    int key;
};

Variable::Variable()
{
    id = "";
    key = 1000;
}

Variable::Variable(string name, int numScope)
{
    id = name;
    key = numScope;

}

void Variable::setId(string name)
{
    id = name;
}

void Variable::setKey(int numScope)
{
    key=numScope;
}

string Variable::getId()
{
    return id;
}

int Variable::getKey()
{
    return key;
}


void Variable::muestra()
{
    cout << "Var Name: " << id << "," << " Key Scope: " << key << endl;
}
//#endif /* Variable_h */

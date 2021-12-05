#ifndef LIST_H
#define LIST_H
#include "Entity.h"

class List {
public:
    byte length;
    
    Entity data[16];

    List();

    Entity getItem(byte);
    
    void append(Entity);
    
    void remove(byte);
};

#endif

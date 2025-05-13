#ifndef BARGP_H
#define BARGP_H


enum ArgType {
    CHAR = 0,
    LONG,
    DOUBLE,
    STRING
};


struct ArgumentDefinition {
    char* name;
    enum ArgType type;
};


#endif  /* BARGP_H */

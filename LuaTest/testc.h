#pragma once

typedef unsigned char lu_byte;

typedef struct Abc Abc;

#define CommonHeader	Abc *next; lu_byte tt; lu_byte marked

struct Abc {
    CommonHeader;
};

#pragma once

#define TABLE_SIZE   (200)

class Sine : public Node
{
public:
    Sine();

protected:
    Frame tick(Frame input);

    Sample sine[TABLE_SIZE];
    int phase;
};


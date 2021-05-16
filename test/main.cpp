#include <iostream>
using namespace std;

class KlasaBazowa
{
public:
    KlasaBazowa(){}
};

class KlasaPochodna : public KlasaBazowa 
{
public:
    KlasaPochodna(){}
};

void f(KlasaBazowa a)
{
    
}

int main()
{
    KlasaPochodna a;
    f(a);
    return 0;
}
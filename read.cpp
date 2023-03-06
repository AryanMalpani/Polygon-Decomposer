#include <iostream>
#include <fstream>
int main(int argc, char * argv[])
{
    std::fstream myfile("edges.txt", std::ios_base::in);

    int a;
    while (myfile >> a)
    {
        printf("%d ", a);
    }


    return 0;
}
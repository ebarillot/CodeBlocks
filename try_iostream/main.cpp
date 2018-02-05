#include <fstream>
#include <iostream>

using namespace std;

int main()
{
    fstream in( "Readme.txt", ios::in );
    if( !in.is_open() )
    {
        cout << "Fichier non trouve !" << endl;
        return 1;
    }

    char c;
    in >> c;
    cout << c;

    return 0;
}

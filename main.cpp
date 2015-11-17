#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>


using namespace std;

void ReadBMP()
{
    FILE* f = fopen("jesien.bmp", "rb");

    FILE* w = fopen("zapis.bmp", "wb");

    if(f == NULL)
        throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f);
    fwrite(info, sizeof(unsigned char), 54, w);

    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    cout << endl;
    cout << "Width: " << width << endl;
    cout << "Height: " << height << endl;


 //   int realwidth = width+(width%4);
    int realwidth =width + (width/8) %2;
    int volume = 3 * height * realwidth;
    unsigned char* data = new unsigned char[volume];

    fwrite(info, sizeof(unsigned char), 54, w);

    fread(data, sizeof(unsigned char), volume, f);
    unsigned char kolor = 0;

    for(int i = 0; i < volume; i+=3)
    {
        kolor=(0.299*data[i]+0.587*data[i+1]+0.114*data[i+2])/3;
        data[i] = kolor;
        data[i+1] = kolor;
        data[i+2] = kolor;
    }


    fwrite(data, sizeof(unsigned char), volume, w);



    fclose(f);
    fclose(w);
    delete(data);
}

int main()
{
    ReadBMP();
    return 0;
}

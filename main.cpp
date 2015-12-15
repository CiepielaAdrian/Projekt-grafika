#include <iostream>
#include <fstream>
#include <conio.h>

using namespace std;

void dodanieBitu(unsigned char *we, unsigned char *wy, int rozmiar)
{
    int i = 0, j = 0, k = 0;
    unsigned char temp = 0;
    while(i < rozmiar)
    {
        switch(j)
        {
            case 0:
                wy[k] |= we[i] >> 1;
                i++;
                j++;
                break;
            case 1:
                wy[k] |= we[i-1] << 7;
                wy[k] = wy[k] >> 1;
                temp = we[i] >> 2;
                wy[k] |= temp;
                i++;
                j++;
                break;
            case 2:
                wy[k] |= we[i-1] << 6;
                wy[k] = wy[k] >> 1;
                temp = we[i] >> 3;
                wy[k] |= temp;
                i++;
                j++;
                break;
            case 3:
                wy[k] |= we[i-1] << 5;
                wy[k] = wy[k] >> 1;
                temp = we[i] >> 4;
                wy[k] |= temp;
                i++;
                j++;
                break;
            case 4:
                wy[k] |= we[i-1] << 4;
                wy[k] = wy[k] >> 1;
                temp = we[i] >> 5;
                wy[k] |= temp;
                i++;
                j++;
                break;
            case 5:
                wy[k] |= we[i-1] << 3;
                wy[k] = wy[k] >> 1;
                temp = we[i] >> 6;
                wy[k] |= temp;
                i++;
                j++;
                break;
            case 6:
                wy[k] |= we[i-1] << 2;
                wy[k] = wy[k] >> 1;
                temp = we[i] >> 7;
                wy[k] |= temp;
                i++;
                j++;
                break;
            case 7:
                wy[k] |= we[i-1] << 1;
                wy[k] = wy[k] >> 1;
                j = 0;
                break;
        }
        k++;
    }
}

void obciecieBitu(unsigned char *we, unsigned char wy[], int volume)
{
	int j = 0, k = 0;
	for (int i = 0; i < volume; i++)
	{
		switch (j)
		{
		case 0:
			wy[k] |= (we[i] << 1);
			j++;
			k++;
			break;
		case 1:
			wy[k - 1] |= (we[i] >> 6);
			wy[k] |= (we[i] << 2);
			j++;
			k++;
			break;
		case 2:
			wy[k - 1] |= (we[i] >> 5);
			wy[k] |= (we[i] << 3);
			j++;
			k++;
			break;
		case 3:
			wy[k - 1] |= (we[i] >> 4);
			wy[k] |= (we[i] << 4);
			j++;
			k++;
			break;
		case 4:
			wy[k - 1] |= (we[i] >> 3);
			wy[k] |= (we[i] << 5);
			j++;
			k++;
			break;
		case 5:
			wy[k - 1] |= (we[i] >> 2);
			wy[k] |= (we[i] << 6);
			j++;
			k++;
			break;
		case 6:
			wy[k - 1] |= (we[i] >> 1);
			wy[k] |= (we[i] << 7);
			j++;
			k++;
			break;
		case 7:
			wy[k - 1] |= (we[i]);
			j = 0;
			break;
		}
	}
}

int zaokraglij(int a, int b)
{
	if (a%b == 0)
		return a / b;
	else
		return a / b + 1;
}

void konwersjaBMPdoNV()
{
	FILE* f = fopen("test2.bmp", "rb");
	FILE* w = fopen("zapis.nv", "wb");

	if (f == NULL)
		throw "Argument Exception";

	unsigned char info[54];

	fread(info, sizeof(unsigned char), 54, f);
	fwrite(info, sizeof(*info), 54, w);

	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	cout << endl;

	cout << "Width: " << width << endl;
	cout << "Height: " << height << endl;


	int realwidth = 3 * width + (4 - ((3 * width) % 4));
	int volume = height * realwidth;
	unsigned char* data = new unsigned char[volume];

	fread(data, sizeof(*data), volume, f);

	for (int i = 0; i < volume; i++)
	{
		data[i] = data[i] >> 1;
	}


	int rozmiar = zaokraglij(volume, 8) * 7;
	unsigned char* a = new unsigned char[rozmiar];

	for (int i = 0; i < rozmiar; i++)
	{
		a[i] = 0;
	}

	obciecieBitu(data, a, volume);

	fwrite(a, sizeof(*a), rozmiar, w);
	fclose(f);
	fclose(w);
	delete(data);
	delete(a);
	delete(info);
}

void konwersjaNVdoBMP()
{
	FILE* f = fopen("zapis.nv", "rb");
	FILE* w = fopen("nowy.bmp", "wb");

	if (f == NULL)
		throw "Argument Exception";

	unsigned char info[54];

	fread(info, sizeof(*info), 54, f);
	fwrite(info, sizeof(*info), 54, w);

	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	cout << endl;

	cout << "Width: " << width << endl;
	cout << "Height: " << height << endl;


	int realwidth = 3 * width + (4 - ((3 * width) % 4));
	int volume = height * realwidth;
	int rozmiar = zaokraglij(volume, 8) * 7;
	unsigned char* data = new unsigned char[rozmiar];

	fread(data, sizeof(*data), rozmiar, f);


	unsigned char* a = new unsigned char[volume];

	for (int i = 0; i < volume; i++)
	{
		a[i] = 0;
	}


	dodanieBitu(data, a, rozmiar);

	for (int i = 0; i < volume; i++)
	{
		if(a[i] == 127)
            a[i] = 255;
        else
            a[i] *= 2;
	}

	fwrite(a, sizeof(*a), volume, w);


	fclose(f);
	fclose(w);
	delete(a);
	delete(data);
	delete(info);
}

int main()
{

	konwersjaBMPdoNV();
	konwersjaNVdoBMP();
	return 0;

}



#include "stdafx.h"
#include "conio.h"
#include "fstream"
#include "iostream"
#include "EasyBMP.h"
#include "EasyBMP_DataStructures.h"

using namespace std;

const int riba = 100;

int n;
// gretimumo matrica
int a[riba][riba];
// saugo virðûniø spalva, jei nëra tada 0
int spalvumasyvas[riba];
// saugo kiek kiekviena ið virðûniu turi gretimø virðûniø
int laipsniumasyvas[riba];
//masyvas virðûniø kurios nëra gretimos esamai virðûnei
int NN[riba];
// counteris
int NNCount;
// neliestø virðûniø skaièius
int neliestosvirsunes;

/*
					RLF spalvinimas
*/

// skaitymo funkcija
void Nuskaitymas()
{
	string file = "input.txt";
	ifstream in(file);
	in >> n;
	for (int i=0; i < n; i++)
		for (int j=0; j < n; j++)
			in >> a[i][j];
	in.close();
		
	
}

// priskiriamos nulinës reikðmës masyvam ir kintamiesiems
void pradiniaiveiksmai()
{
	for (int i=0; i<n; i++)
	{
		spalvumasyvas[i] = 0;
		laipsniumasyvas[i] = 0;
		for (int j = 0; j < n; j++)
			if (a[i][j] == 1)
				laipsniumasyvas[i] ++;
	}
	NNCount = 0;
	neliestosvirsunes = n;
}
// funkcija surandanti neliesta virsunia su didziausiu laipsniu
int DIDlaipsniovirsune()
{
	int max = -1;
	int max_i;
	for (int i =0; i<n; i++)
		if (spalvumasyvas[i] == 0)
			if (laipsniumasyvas[i]>max)
			{
				max = laipsniumasyvas[i];
				max_i = i;
			}
	return max_i;
}
// this function is for UpdateNN function
// it reset the value of scanned array
// resetina nuskanuota areju
void scannedInit(int scanned[riba])
{
	for (int i=0; i<n; i++)
		scanned[i] = 0;
}
// funkcija atnaujinanti masyva, mazinanti ji po kiekvieno ciklo kol pasiekiamas 0
void UpdateNN(int ColorNumber)
{
	NNCount = 0;
	for (int i=0; i<n; i++)
		if (spalvumasyvas[i] == 0)
		{
			NN[NNCount] = i;
			NNCount ++;
		}
	for (int i=0; i<n; i++)
		if (spalvumasyvas[i] == ColorNumber)
			for (int j=0; j<NNCount; j++)
				while (a[i][NN[j]] == 1)
				{
					NN[j] = NN[NNCount - 1];
					NNCount --;
				}
}
// funkcija surandanti tinkama virsune is NN
int Paieska(int ColorNumber, int& VerticesInCommon)
{
	int temp,tmp_y,y;
	int scanned[riba];
	VerticesInCommon = 0;
	for (int i=0; i<NNCount; i++)
	{
		tmp_y = NN[i];
		temp = 0;
		scannedInit(scanned);
		for (int x=0; x<n; x++)
			if (spalvumasyvas[x] == ColorNumber)
				for (int k=0; k<n; k++)
					if (spalvumasyvas[k] == 0 && scanned[k] == 0)
						if (a[x][k] == 1 && a[tmp_y][k] == 1)
						{
							temp ++;
							scanned[k] = 1;
						}
		if (temp > VerticesInCommon)
		{
			VerticesInCommon = temp;
			y = tmp_y;
		}
	}
	return y;
}
// suranda virsune su daugiausiai gretimu virsuniu
int VirsuinesumaxlaipsniuInNN()
{
	int tmp_y;
	int temp, max = 0;
	for (int i=0; i<NNCount; i++)
	{
		temp = 0;
		for (int j=0; j<n; j++)
			if (spalvumasyvas[NN[j]] == 0 && a[i][NN[j]] == 1)
				temp ++;
		if (temp > max)
		{
			max = temp;
			tmp_y = NN[i];
		}
	}
	return tmp_y;
}
// funkcija atliekanti spalvinima
void Spalvinimas()
{

	//BMP grafas;
	//grafas.SetSize(640, 480);
	//grafas.SetBitDepth(32);

	int x,y;
	int spalvosnr = 0;
	int bendrosvirsunes = 0;
	while (neliestosvirsunes>0)
	{
		x = DIDlaipsniovirsune();
		spalvosnr ++;
		spalvumasyvas[x] = spalvosnr;
		neliestosvirsunes --;
		UpdateNN(spalvosnr);
		while (NNCount>0)
		{
			y = Paieska(spalvosnr, bendrosvirsunes);
			if (bendrosvirsunes == 0)
				y = VirsuinesumaxlaipsniuInNN();
			spalvumasyvas[y] = spalvosnr;
			neliestosvirsunes --;
			UpdateNN(spalvosnr);
		//	grafas(spalvosnr, bendrosvirsunes)->Green = 0;
		}

	//	grafas.WriteToFile("bandymas.bmp");
	}
}
// atspausdina nuspalvintas virsunes
void Spausdinimas()
{
	for (int i = 0; i < n; i++)
		
		cout << "Virsune: " << i+1 << " Nuspalvinta spalva nr: " << spalvumasyvas[i] << endl;

}
// main funkcija
void main()
{
	
	Nuskaitymas();
	pradiniaiveiksmai();
	Spalvinimas();
	Spausdinimas();
	getch();
	
    //http://www.imada.sdu.dk/~marco/Publications/Files/MIC2011-ChiGalGua.pdf
	//http://www.tandfonline.com/doi/abs/10.1080/00207160701419114?journalCode=gcom20
}
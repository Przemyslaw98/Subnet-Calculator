#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <regex>

using namespace std;

int main(int argc, char** argv) {

	unsigned int adresIP[4] = { 0,0,0,0 };
	string adresIP_B[4] = { "","","","" };
	unsigned int maskaPodsieci[4] = { 0,0,0,0 };
	string maskaPodsieci_B[4] = { "","","","" };

	unsigned int adresSieci[4] = { 0,0,0,0 };
	string adresSieci_B[4] = { "","","","" };
	unsigned int adresBroadcast[4] = { 0,0,0,0 };
	string adresBroadcast_B[4] = { "","","","" };
	unsigned int adresHostaPierwszy[4] = { 0,0,0,0 };
	string adresHostaPierwszy_B[4] = { "","","","" };
	unsigned int adresHostaOstatni[4] = { 0,0,0,0 };
	string adresHostaOstatni_B[4] = { "","","","" };

	void toBinary(unsigned int*, string*);
	void toDecimal(string*, unsigned int*);
	void wypisz(unsigned int*, string*, string&);







	//Wersja z argumentem
	if (argc >= 2) {
		unsigned int dane[5] = { 0,0,0,0,0 }; //Tablica na integery
		string argument = argv[1];
		int t = 4;
		int p = 0;
		bool slash = 1;
		//Sprawdzanie poprawnoœci argumentu
		for (int i = argument.length() - 1; i >= 0; i--) {
			if ((argument[i] > 57 || argument[i] < 48) && argument[i] != '.'&&argument[i] != '/') {
				cout << "Nieprawidlowy argument!\n"; //Znak niebêd¹cy cyfr¹, kropk¹ ani slashem
				system("pause");
				return 1;
			}
			else if (argument[i] <= 57 && argument[i] >= 48) {
				dane[t] += ((int)argument[i] - 48) * pow(10, p);
				p++;
			}
			else if ((argument[i] == '.'&&slash == 0) || (argument[i] == '/'&&slash == 1)) {
				slash = 0;
				if (argument[i - 1] == '.' || argument[i - 1] == '/') {
					cout << "Nieprawidlowy argument!\n"; //Dwie kropki b¹dŸ slashe obok siebie
					system("pause");
					return 1;
				}
				p = 0;
				t--;
			}
			else if ((argument[i] == '.'&&slash == 1) || (argument[i] == '/'&&slash == 0)) {
				cout << "Nieprawidlowy argument!\n"; //Kropka b¹dŸ slash w miejcu, gdzie na pewno nie powinno ich byæ
				system("pause");
				return 1;
			}
		}
		for (int i = 0; i < 4; i++)
			if (dane[i] > 255) {
				cout << "Nieprawidlowy argument!\n"; //Liczba w adresie wiêksza od 255
				system("pause");
				return 1;
			}
		if (dane[4] > 32) {
			cout << "Nieprawidlowy argument!\n"; //Maska wiêksza od 32
			system("pause");
			return 1;
		}
		for (int i = 0; i < 4; i++) {
			adresIP[i] = dane[i];
		}
		for (int i = 0; i < 32; i++)
			if (i < dane[4])
				maskaPodsieci_B[i / 8] += '1';
			else maskaPodsieci_B[i / 8] += '0';

			toBinary(adresIP, adresIP_B); //Konwersja na system binarny
			toDecimal(maskaPodsieci_B, maskaPodsieci); //I na dziesiêtny
	}
	//Wersja bez argumentu
	else {
		system("ipconfig>ip.txt"); //Zapisuje wynik komendy do pliku
		ifstream ip;
		ip.open("ip.txt");
		string wyciag;
		if (ip.good() == false) {
			cout << "Niewystarczaj¹ce uprawnienia!\n";
			system("pause");
			return 1;
		}
		while (ip.eof() == false) {
			regex check(".*IPv4.*"); //Szuka linijki z adresem IP
			getline(ip, wyciag);
			if (regex_search(wyciag, check) == 1)
				break;
		}
		string adres = wyciag.substr(wyciag.find(':') + 2); //Zapisuje ci¹g od dwukropka do koñca linijki
		while (ip.eof() == false) {
			regex check(".*Mask.*"); //To samo z mask¹
			getline(ip, wyciag);
			if (regex_search(wyciag, check) == 1)
				break;
		}
		string maska = wyciag.substr(wyciag.find(':') + 2);
		ip.close();
		int t = 4;
		int p = 0;
		for (int i = adres.length() - 1; i >= 0; i--) { //Przepisujemy liczby z ci¹gu do tablicy integerów
			if (adres[i] <= 57 && adres[i] >= 48) {
				adresIP[t] += ((int)adres[i] - 48) * pow(10, p);
				p++;
			}
			else if (adres[i] == '.') {
				p = 0;
				t--;
			}
		}
		t = 4; p = 0;
		for (int i = maska.length() - 1; i >= 0; i--) {
			if (maska[i] <= 57 && maska[i] >= 48) {
				maskaPodsieci[t] += ((int)maska[i] - 48) * pow(10, p);
				p++;
			}
			else if (adres[i] == '.') {
				p = 0;
				t--;
			}
		}
		toBinary(adresIP, adresIP_B); //Konwersja na system binarny
		toBinary(maskaPodsieci, maskaPodsieci_B);
	}






	for (int i = 0; i < 4; i++) { //Kalkulacja na zapisie binarnym
		for (int j = 0; j < 8; j++)
			if (maskaPodsieci_B[i][j] == '1')
				adresSieci_B[i] += adresIP_B[i][j];
			else adresSieci_B[i] += '0';
	}
	toDecimal(adresSieci_B, adresSieci); //I przepisanie do dziesiêtnego

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++)
			if (maskaPodsieci_B[i][j] == '1')
				adresBroadcast_B[i] += adresIP_B[i][j];
			else adresBroadcast_B[i] += '1';
	}
	toDecimal(adresBroadcast_B, adresBroadcast);

	int tmp = 1;
	for (int i = 3; i >= 0; i--) {
		adresHostaPierwszy[i] = adresSieci[i]; //Do ostatniej liczby dodajemy jeden
		if (tmp == 1) {
			if (adresHostaPierwszy[i] < 255) { //Chyba ¿e jest równa 255
				adresHostaPierwszy[i]++;
				tmp--;
			}
			else adresHostaPierwszy[i] = 0; //Wtedy zerujemy i próbujemy na kolejnej liczbie
		}
	}
	toBinary(adresHostaPierwszy, adresHostaPierwszy_B);
	tmp = 1;
	for (int i = 3; i >= 0; i--) {
		adresHostaOstatni[i] = adresBroadcast[i]; //Tu analogicznie tylko w drug¹ stronê
		if (tmp == 1) {
			if (adresHostaOstatni[i] > 0) {
				adresHostaOstatni[i]--;
				tmp--;
			}
			else adresHostaOstatni[i] = 255;
		}
	}
	toBinary(adresHostaOstatni, adresHostaOstatni_B);

	ofstream plik;
	plik.open("wynik.txt"); //Tworzymy (jeœli nie ma) i otwieramy plik do zapisu
	string wynik = "";
	if (plik.good() == false) {
		cout << "Niewystarczaj¹ce uprawnienia!\n";
		return 1;
	}


	wynik += "         Klasa sieci:\t"; //Przepisujemy wszystko po kolei do ci¹gu.
	if (adresIP_B[0][0] == '0')
		wynik += "A\n";
	else if (adresIP_B[0][1] == '0')
		wynik += "B\n";
	else if (adresIP_B[0][2] == '0')
		wynik += "C\n";
	else if (adresIP_B[0][3] == '0')
		wynik += "D\n";
	else wynik += "E\n";
	wynik += "          Typ adresu:\t";
	if (adresIP[0] == 10 || (adresIP[0] == 172 && adresIP[1] > 15 && adresIP[1] < 32) || (adresIP[0] == 192 && adresIP[1] == 168))
		wynik += "Prywatny\n";
	else wynik += "Publiczny\n";

	wynik += "            Adres IP:\t";
	wypisz(adresIP, adresIP_B, wynik);
	wynik += "      Maska podsieci:\t";
	wypisz(maskaPodsieci, maskaPodsieci_B, wynik);
	wynik += "         Adres sieci:\t";
	wypisz(adresSieci, adresSieci_B, wynik);
	wynik += "     Adres broadcast:\t";
	wypisz(adresBroadcast, adresBroadcast_B, wynik);
	wynik += "Pierwszy adres hosta:\t";
	wypisz(adresHostaPierwszy, adresHostaPierwszy_B, wynik);
	wynik += " Ostatni adres hosta:\t";
	wypisz(adresHostaOstatni, adresHostaOstatni_B, wynik);
	wynik += " Maks. liczba hostow:\t";
	wynik += to_string((adresHostaOstatni[0] - adresSieci[0]) * 16777216 + (adresHostaOstatni[1] - adresSieci[1]) * 65536 + (adresHostaOstatni[2] - adresSieci[2]) * 256 + adresHostaOstatni[3] - adresSieci[3]);
	wynik += "\n";
	plik << wynik;
	cout << wynik;
	if ((adresIP[0]!=adresSieci[0] || adresIP[1] != adresSieci[1] || adresIP[2] != adresSieci[2] || adresIP[3] != adresSieci[3] )&& (adresIP[0] != adresBroadcast[0] || adresIP[1] != adresBroadcast[1] || adresIP[2] != adresBroadcast[2] || adresIP[3] != adresBroadcast[3])) {
		cout << "Czy wykonac ping?(Y/N)";
		char reply = '0';
		while (reply != 'y'&&reply != 'Y'&&reply != 'n'&&reply != 'N')
			cin >> reply;
		if (reply == 'y' || reply == 'Y') {
			string command = "ping " + to_string(adresIP[0]) + '.' + to_string(adresIP[1]) + '.' + to_string(adresIP[2]) + '.' + to_string(adresIP[3]);
			system(command.c_str());
		}
	}
	system("pause");
	return 0;
}

void toBinary(unsigned int* dec, string* bin) {
	for (int i = 0; i < 4; i++) {
		int tmp = dec[i];			//Kopiujemy liczbê z adresu
		for (int j = 7; j >= 0; j--)
			if (tmp >= pow(2, j)) { //I sprawdzamy czy jest wiêksza od kolejnych potêg dwójki zaczynaj¹c od siódmej, koñcz¹c na zerowej
				tmp -= pow(2, j);
				bin[i] += '1';
			}
			else bin[i] += '0';
	}
}
void toDecimal(string* bin, unsigned int* dec) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++)
			if (bin[i][j] == '1') {
				dec[i] += pow(2, 7 - j); //Tam gdzie jedynka, dodajemy do liczby odpowiedni¹ potêgê dwójki
			}
	}
}
void wypisz(unsigned int* dec, string* bin, string& wynik) {
	wynik += to_string(dec[0]) + '.' + to_string(dec[1]) + '.' + to_string(dec[2]) + '.' + to_string(dec[3]) + '\t' + bin[0] + ' ' + bin[1] + ' ' + bin[2] + ' ' + bin[3] + "\n";
}
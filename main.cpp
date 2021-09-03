#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#include <sstream>
#include <limits>
#include <conio.h>

using namespace std;

string nazwaPlikuZUzytkownikami = "Uzytkownicy.txt";
string nazwaPlikuZAdresatami = "Adresaci.txt";
string nazwaPlikuTymczasowego = "Adresaci_tymczasowy.txt";

struct Uzytkownik {
	int idUzytkownika = 0;
	string nazwa = "";
	string haslo = "";
};

struct Adresat {
	int idAdresata = 0;
	int idUzytkownika = 0;
	string imie = "";
	string nazwisko = "";
	string numerTelefonu = "";
	string email = "";
	string adres = "";
};

void wczytajUzytkownikowZPliku(vector<Uzytkownik>&);
Uzytkownik pobierzDaneUzytkownika(string);
void zarejestrujUzytkownika(vector<Uzytkownik>&);
string wczytajLinie();
void dopiszUzytkownikaDoPliku(Uzytkownik);
int zalogujUzytkownika(vector<Uzytkownik>&);
void zmienHaslo(int, vector<Uzytkownik>&);
void zapiszZmienioneHasloDoPliku(vector<Uzytkownik>&);
string konwersjaIntNaString(int);
void dodajAdresata(int, int, vector<Adresat>&);
void dopiszAdresataDoPliku(Adresat);
void wczytajAdresatowZPliku(int, vector<Adresat>&);
Adresat pobierzDaneAdresata(string);
void wyszukajAdresatowPoImieniu(vector<Adresat>&);
void wypiszWszystkichAdresatow(vector<Adresat>&);
int znajdzIdUzytkownika(string);
void wyszukajAdresatowPoNazwisku(vector<Adresat>&);
void usunAdresata(vector<Adresat>&);
int wczytajLiczbeCalkowita();
void usunAdresataZPliku(int);
int wczytajIdOstatniegoAdresata(vector<Adresat>&);
void edytujAdresata(vector<Adresat>&);
void edytujAdresataWPliku(vector<Adresat>&);

int main() {
	vector<Adresat> adresaci;
	vector<Uzytkownik> uzytkownicy;
	int idOstatniegoAdresata = 0;
	int idZalogowanegoUzytkownika = 0;
	char wybor;
	wczytajUzytkownikowZPliku(uzytkownicy);
	while (true) {
		if (idZalogowanegoUzytkownika == 0) {
			system("cls");
			cout << ">>> MENU GLOWNE <<<\n";
			cout << "1. Logowanie\n";
			cout << "2. Rejestracja\n";
			cout << "9. Zamknij program\n";
			cout << "Wybierz opcje z menu: ";
			cin >> wybor;
			switch (wybor) {
			case '1':
				idZalogowanegoUzytkownika = zalogujUzytkownika(uzytkownicy);
				adresaci.clear();
				wczytajAdresatowZPliku(idZalogowanegoUzytkownika, adresaci);
				break;
			case '2':
				zarejestrujUzytkownika(uzytkownicy);
				break;
			case '9':
				exit(0);
			}
		}
		else {
			system("cls");
			cout << " >>> KSIAZKA ADRESOWA <<< \n\n";
			cout << "1. Dodaj adresata\n";
			cout << "2. Wyszukaj po imieniu\n";
			cout << "3. Wyszukaj po nazwisku\n";
			cout << "4. Wyswietl wszystkich adresatow\n";
			cout << "5. Usun adresata\n";
			cout << "6. Edytuj adresata\n";
			cout << "7. Zmien haslo\n";
			cout << "9. Wyloguj sie\n";
			cout << "Wybierz opcje z menu: ";
			cin >> wybor;
			switch (wybor) {
			case '1':
				idOstatniegoAdresata = wczytajIdOstatniegoAdresata(adresaci);
				dodajAdresata(idZalogowanegoUzytkownika, idOstatniegoAdresata, adresaci);
				break;
			case '2':
				wyszukajAdresatowPoImieniu(adresaci);
				break;
			case '3':
				wyszukajAdresatowPoNazwisku(adresaci);
				break;
			case '4':
				wypiszWszystkichAdresatow(adresaci);
				break;
			case '5':
				usunAdresata(adresaci);
				break;
			case '6':
				edytujAdresata(adresaci);
				break;
			case '7':
				zmienHaslo(idZalogowanegoUzytkownika, uzytkownicy);
				break;
			case '9':
				idZalogowanegoUzytkownika = 0;
			}
		}
	}

	return 0;
}

void wczytajUzytkownikowZPliku(vector<Uzytkownik>& uzytkownicy) {
	Uzytkownik uzytkownik;
	string daneUzytkownika = "";
	fstream plik;
	plik.open(nazwaPlikuZUzytkownikami.c_str(), ios::in);
	if (plik.good() == true) {
		while (getline(plik, daneUzytkownika)) {
			uzytkownik = pobierzDaneUzytkownika(daneUzytkownika);
			uzytkownicy.push_back(uzytkownik);
		}
		plik.close();
	}
	else {
		cout << "Nie udalo sie wczytac danych z pliku.\n";
	}
}

Uzytkownik pobierzDaneUzytkownika(string daneUzytkownika) {
	Uzytkownik uzytkownik;
	string pojedynczaDana = "";
	int numerDanej = 1;
	for (unsigned int i = 0; i < daneUzytkownika.length(); i++) {
		if (daneUzytkownika[i] != '|') {
			pojedynczaDana += daneUzytkownika[i];
		}
		else {
			switch (numerDanej) {
			case 1:
				uzytkownik.idUzytkownika = atoi(pojedynczaDana.c_str());
				break;
			case 2:
				uzytkownik.nazwa = pojedynczaDana;
				break;
			case 3:
				uzytkownik.haslo = pojedynczaDana;
				break;
			}
			pojedynczaDana = "";
			numerDanej++;
		}
	}
	return uzytkownik;
}

string wczytajLinie() {
	string wejscie = "";
	getline(cin, wejscie);
	return wejscie;
}

void zarejestrujUzytkownika(vector<Uzytkownik>& uzytkownicy) {
	Uzytkownik uzytkownik;
	string nazwa = "";
	system("cls");
	cout << ">>> DODAWANIE NOWEGO UZYTKOWNIKA <<<\n\n";
	if (uzytkownicy.empty() == true) {
		uzytkownik.idUzytkownika = 1;
	}
	else {
		uzytkownik.idUzytkownika = uzytkownicy.back().idUzytkownika + 1;
	}
	cout << "Podaj nazwe uzytkownika: ";
	cin >> nazwa;
	for (auto it = uzytkownicy.begin(); it != uzytkownicy.end(); it++) {
		if (it->nazwa == nazwa) {
			cout << "Taki uzytkownik juz istnieje. Podaj inna nazwe uzytkownika: ";
			cin >> nazwa;
			it = uzytkownicy.begin();
		}
	}
	uzytkownik.nazwa = nazwa;
	cout << "Podaj haslo: ";
	cin >> uzytkownik.haslo;
	uzytkownicy.push_back(uzytkownik);
	dopiszUzytkownikaDoPliku(uzytkownik);
}

void dopiszUzytkownikaDoPliku(Uzytkownik uzytkownik) {
	fstream plik;
	plik.open(nazwaPlikuZUzytkownikami.c_str(), ios::out | ios::app);
	if (plik.good() == true) {
		plik << uzytkownik.idUzytkownika << '|';
		plik << uzytkownik.nazwa << '|';
		plik << uzytkownik.haslo << '|' << '\n';
		plik.close();
		cout << "Uzytkownik zostal dodany\n";
	}
	else {
		cout << "Nie udalo sie otworzyc pliku i zapisac w nim danych\n";
	}
	system("pause");
}

int zalogujUzytkownika(vector<Uzytkownik>& uzytkownicy) {
	Uzytkownik uzytkownik;
	string nazwa = "";
	string haslo = "";
	cout << "Podaj nazwe uzytkownika: ";
	cin >> nazwa;
	for (auto it = uzytkownicy.begin(); it != uzytkownicy.end(); it++) {
		if (it->nazwa == nazwa) {
			for (int i = 0; i < 3; i++) {
				cout << "Podaj haslo. Pozostaly " << 3 - i << " proby: ";
				cin >> haslo;
				if (it->haslo == haslo) {
					cout << "Logowanie przebieglo pomyslnie.\n";
					system("pause");
					return it->idUzytkownika;
				}
			}
			cout << "Podales bledne haslo. Nacisnij dowolny klawisz by wrocic do menu glownego.\n";
			system("pause");
			return 0;
		}
	}
	cout << "Nie ma uzytkownikow z takim loginem.\n";
	system("pause");
	return 0;
}

void zmienHaslo(int idZalogowanegoUzytkownika, vector<Uzytkownik>& uzytkownicy) {
	string haslo;

	for (auto it = uzytkownicy.begin(); it != uzytkownicy.end(); it++) {
		if (it->idUzytkownika == idZalogowanegoUzytkownika) {
			cout << "Podaj nowe haslo: ";
			cin >> haslo;
			it->haslo = haslo;
			cout << "Haslo zostalo zmienione.\n";
			zapiszZmienioneHasloDoPliku(uzytkownicy);
		}
	}
	system("pause");
}

void zapiszZmienioneHasloDoPliku(vector<Uzytkownik>& uzytkownicy) {
	fstream plik;
	string linia = "";
	plik.open(nazwaPlikuZUzytkownikami.c_str(), ios::out);
	if (plik.good()) {
		for (auto it = uzytkownicy.begin(); it != uzytkownicy.end(); it++) {
			linia += konwersjaIntNaString(it->idUzytkownika) + '|';
			linia += it->nazwa + '|';
			linia += it->haslo + '|';
			plik << linia << '\n';
			linia = "";
		}
		plik.close();
	}
	else {
		cout << "Nie mozna otworzyc pliku " << nazwaPlikuZUzytkownikami << '\n';
	}
}

string konwersjaIntNaString(int liczba) {
	ostringstream ss;
	ss << liczba;
	string str = ss.str();
	return str;
}

void dodajAdresata(int idZalogowanegoUzytkownika, int idOstatniegoAdresata, vector<Adresat>& adresaci) {
	Adresat adresat;
	system("cls");
	cout << " >>> DODAWANIE NOWEGO ADRESATA <<< \n\n";
	if (adresaci.empty() == true) {
		adresat.idAdresata = 1;
	}
	else {
		adresat.idAdresata = idOstatniegoAdresata + 1;
	}
	adresat.idUzytkownika = idZalogowanegoUzytkownika;
	cin.clear();
	cin.ignore();
	cout << "Podaj imie: ";
	adresat.imie = wczytajLinie();
	cout << "Podaj nazwisko: ";
	adresat.nazwisko = wczytajLinie();
	cout << "Podaj numer telefonu: ";
	adresat.numerTelefonu = wczytajLinie();
	cout << "Podaj email: ";
	adresat.email = wczytajLinie();
	cout << "Podaj adres: ";
	adresat.adres = wczytajLinie();
	adresaci.push_back(adresat);
	dopiszAdresataDoPliku(adresat);
}

void dopiszAdresataDoPliku(Adresat adresat) {
	fstream plik;
	plik.open(nazwaPlikuZAdresatami.c_str(), ios::out | ios::app);
	if (plik.good() == true) {
		plik << adresat.idAdresata << '|';
		plik << adresat.idUzytkownika << '|';
		plik << adresat.imie << '|';
		plik << adresat.nazwisko << '|';
		plik << adresat.numerTelefonu << '|';
		plik << adresat.email << '|';
		plik << adresat.adres << '|' << '\n';
		plik.close();
		cout << "\nAdresat zostal dodany\n";
	}
	else {
		cout << "Nie udalo sie otworzyc pliku i zapisac w nim danych\n";
	}
	system("pause");
}

void wczytajAdresatowZPliku(int idZalogowanegoUzytkownika, vector<Adresat>& adresaci) {
	Adresat adresat;
	string daneAdresata = "";
	fstream plik;
	plik.open(nazwaPlikuZAdresatami.c_str(), ios::in);
	if (plik.good() == true) {
		while (getline(plik, daneAdresata)) {
			adresat = pobierzDaneAdresata(daneAdresata);
			if (idZalogowanegoUzytkownika == adresat.idUzytkownika) {
				adresaci.push_back(adresat);
			}
		}
		plik.close();
	}
}

int znajdzIdUzytkownika(string linia) {
	int poczatek, koniec, liczba;
	poczatek = linia.find('|') + 1;
	koniec = linia.find('|', poczatek);
	liczba = koniec - poczatek;
	return atoi(linia.substr(poczatek, liczba).c_str());
}

Adresat pobierzDaneAdresata(string daneAdresata) {
	Adresat adresat;
	string pojedynczaDana = "";
	int numerDanej = 1;
	for (unsigned int i = 0; i < daneAdresata.length(); i++) {
		if (daneAdresata[i] != '|') {
			pojedynczaDana += daneAdresata[i];
		}
		else {
			switch (numerDanej) {
			case 1:
				adresat.idAdresata = atoi(pojedynczaDana.c_str());
				break;
			case 2:
				adresat.idUzytkownika = atoi(pojedynczaDana.c_str());
				break;
			case 3:
				adresat.imie = pojedynczaDana;
				break;
			case 4:
				adresat.nazwisko = pojedynczaDana;
				break;
			case 5:
				adresat.numerTelefonu = pojedynczaDana;
				break;
			case 6:
				adresat.email = pojedynczaDana;
				break;
			case 7:
				adresat.adres = pojedynczaDana;
				break;
			}
			pojedynczaDana = "";
			numerDanej++;
		}
	}
	return adresat;
}

void wyszukajAdresatowPoImieniu(vector<Adresat>& adresaci) {
	string imie = "";
	int iloscAdresatow = 0;
	system("cls");
	if (!adresaci.empty()) {
		cout << " >>> WYSZUKIWANIE ADRESATOW PO IMIENIU <<< \n\n";
		cout << "Podaj imie adresata: ";
		cin.ignore();
		imie = wczytajLinie();
		for (auto it = adresaci.begin(); it != adresaci.end(); it++) {
			if (it->imie == imie) {
				cout << "\nId: " << it->idAdresata;
				cout << "\nImie: " << it->imie;
				cout << "\nNazwisko: " << it->nazwisko;
				cout << "\nNumer telefonu: " << it->numerTelefonu;
				cout << "\nEmail: " << it->email;
				cout << "\nAdres: " << it->adres;
				cout << endl;
				iloscAdresatow++;
			}
		}
		if (iloscAdresatow == 0) {
			cout << "\n\nNie ma adresatow o podanym imieniu\n";
		}
		else {
			cout << "\nIlosc adresatow z imieniem " << imie << " w ksiazce adresowej wynosi " << iloscAdresatow << endl << endl;
		}
	}
	else {
		cout << "\nKsiazka adresowa jest pusta\n";
	}
	cout << endl;
	system("pause");
}

void wyszukajAdresatowPoNazwisku(vector<Adresat>& adresaci) {
	string nazwisko = "";
	int iloscAdresatow = 0;
	system("cls");
	if (!adresaci.empty()) {
		cout << " >>> WYSZUKIWANIE ADRESATOW PO NAZWISKU <<< \n\n";
		cout << "Podaj nazwisko adresata: ";
		cin.ignore();
		nazwisko = wczytajLinie();
		for (auto it = adresaci.begin(); it != adresaci.end(); it++) {
			if (it->nazwisko == nazwisko) {
				cout << "\nId: " << it->idAdresata;
				cout << "\nImie: " << it->imie;
				cout << "\nNazwisko: " << it->nazwisko;
				cout << "\nNumer telefonu: " << it->numerTelefonu;
				cout << "\nEmail: " << it->email;
				cout << "\nAdres: " << it->adres;
				cout << endl;
				iloscAdresatow++;
			}
		}
		if (iloscAdresatow == 0) {
			cout << "\n\nNie ma adresatow o podanym nazwisku\n";
		}
		else {
			cout << "\nIlosc adresatow z nazwiskiem " << nazwisko << " w ksiazce adresowej wynosi " << iloscAdresatow << endl << endl;
		}
	}
	else {
		cout << "\nKsiazka adresowa jest pusta\n";
	}
	cout << endl;
	system("pause");
}

void wypiszWszystkichAdresatow(vector<Adresat>& adresaci) {
	system("cls");
	cout << "Wyswietl Wszystkich Adresatow" << endl;
	int liczbaAdresatow = adresaci.size();
	for (int i = 0; i < liczbaAdresatow; i++) {
		cout << "Id: " << adresaci[i].idAdresata << endl;
		cout << "Imie: " << adresaci[i].imie << endl;
		cout << "Nazwisko: " << adresaci[i].nazwisko << endl;
		cout << "Numer Telefonu: " << adresaci[i].numerTelefonu << endl;
		cout << "email: " << adresaci[i].email << endl;
		cout << "Adres: " << adresaci[i].adres << endl << endl;
	}
	/*if (!adresaci.empty()) {
		for (auto it = adresaci.begin(); it != adresaci.end(); it++) {
			cout << "\nId: " << it->idAdresata;
			cout << "\nImie: " << it->imie;
			cout << "\nNazwisko: " << it->nazwisko;
			cout << "\nEmail: " << it->email;
			cout << "\nAdres: " << it->adres;
			cout << endl << endl;
		}
	}
	else {
		cout << "\nKsiazka adresowa jest pusta.\n";
	}*/
	system("pause");
}

void usunAdresata(vector<Adresat>& adresaci) {
	int idUsuwanegoAdresata = 0;
	char znak;
	bool czyIstniejeAdresat = false;
	system("cls");
	if (!adresaci.empty()) {
		cout << " >>> USUWANIE WYBRANEJ OSOBY <<< \n\n";
		cout << "Podaj numer ID adresata, ktorego chcesz usunac: ";
		cin.ignore();
		idUsuwanegoAdresata = wczytajLiczbeCalkowita();
		for (auto it = begin(adresaci); it != end(adresaci); it++) {
			if (it->idAdresata == idUsuwanegoAdresata) {
				czyIstniejeAdresat = true;
				cout << "\nPotwierdz naciskajac klawisz t: ";
				cin.clear();
				cin.ignore();
				cin >> znak;
				if (znak == 't' || znak == 'T') {
					adresaci.erase(it);
					cout << "\nSzukany adresat zostal usuniety\n";
					usunAdresataZPliku(idUsuwanegoAdresata);
					break;
				}
				else {
					cout << "\nWybrany adresat nie zostal usuniety\n";
					break;
				}
			}
		}
		if (czyIstniejeAdresat == false) {
			cout << "\nNie ma takiego adresata w ksiazce adresowej\n";
		}
	}
	else {
		cout << "\nKsiazka adresowa jest pusta\n";
	}
	system("pause");
}

int wczytajLiczbeCalkowita() {
	string wejscie;
	int liczba = 0;
	while (true) {
		getline(cin, wejscie);
		stringstream myStream(wejscie);
		if (myStream >> liczba) {
			break;
		}
		cout << "To nie jest liczba. Wpisz ponownie.\n";
	}
	return liczba;
}

void usunAdresataZPliku(int idUsuwanegoAdresata) {
	fstream plik, plikTymczasowy;
	string linia = "";
	plik.open(nazwaPlikuZAdresatami.c_str(), ios::in);
	plikTymczasowy.open(nazwaPlikuTymczasowego.c_str(), ios::out);
	do {
		string numerId = "";
		getline(plik, linia);
		for (int i = 0; i < linia.length(); i++) {
			if (linia[i] != '|') {
				numerId += linia[i];
			}
			else {
				break;
			}
		}
		int numer = atoi(numerId.c_str());
		if (numer != idUsuwanegoAdresata && linia != "") {
			plikTymczasowy << linia << endl;
		}
	} while (linia != "");
	plik.close();
	plikTymczasowy.close();
	remove(nazwaPlikuZAdresatami.c_str());
	rename(nazwaPlikuTymczasowego.c_str(), nazwaPlikuZAdresatami.c_str());
}

int wczytajIdOstatniegoAdresata(vector<Adresat>& adresaci) {
	Adresat adresat;
	string daneAdresata = "";
	int idOstatniegoAdresata = 0;
	fstream plik;
	plik.open(nazwaPlikuZAdresatami.c_str(), ios::in);
	if (plik.good() == true) {
		while (getline(plik, daneAdresata)) {
			adresat = pobierzDaneAdresata(daneAdresata);
			idOstatniegoAdresata = adresat.idAdresata;
		}
		plik.close();
	}
	return idOstatniegoAdresata;
}

void edytujAdresata(vector<Adresat>& adresaci) {
	int idWybranegoAdresata = 0;
	char wybor;
	bool czyAdresatIstnieje = false;
	system("cls");
	if (!adresaci.empty()) {
		cout << "\n >>> EDYCJA WYBRANEGO ADRESATA <<< \n";
		cout << "\nPodaj numer id adresata, ktorego dane chcesz edytowac: ";
		idWybranegoAdresata = wczytajLiczbeCalkowita();
		for (auto it = begin(adresaci); it != end(adresaci); it++) {
			if (it->idAdresata == idWybranegoAdresata) {
				czyAdresatIstnieje = true;
				cout << "\nKtore dane zaktualizowac: \n";
				cout << "\n1 - Imie";
				cout << "\n2 - Nazwisko";
				cout << "\n3 - Numer telefonu";
				cout << "\n4 - Email";
				cout << "\n5 - Adres";
				cout << "\n6 - Powrot";
				cout << "\n\nWybierz opcje z menu: ";
				cin >> wybor;
				cin.ignore();
				switch (wybor) {
				case '1':
					cout << "Podaj nowe imie: ";
					it->imie = wczytajLinie();
					cout << "\nImie zostalo zmienione\n";
					edytujAdresataWPliku(adresaci);
					break;
				case'2':
					cout << "Podaj nowe nazwisko: ";
					it->nazwisko = wczytajLinie();
					cout << "\nNazwisko zostalo zmienione\n";
					edytujAdresataWPliku(adresaci);
					break;
				case '3':
					cout << "Podaj nowy numer telefonu: ";
					it->numerTelefonu = wczytajLinie();
					cout << "\nNumer telefonu zostal zmieniony:\n";
					edytujAdresataWPliku(adresaci);
					break;
				case '4':
					cout << "Podaj nowy email: ";
					it->email = wczytajLinie();
					cout << "\nEmail zostal zmieniony\n";
					edytujAdresataWPliku(adresaci);
					break;
				case '5':
					cout << "Podaj nowy adres zamieszkania: ";
					it->adres = wczytajLinie();
					cout << "\nAdres zostal zmieniony: ";
					edytujAdresataWPliku(adresaci);
					break;
				default:
					cout << "\nNie ma takiej opcji w menu\n";
					break;
				}
			}
		}
		if (czyAdresatIstnieje == false) {
			cout << "\nNie ma takiego adresata w ksiazce adresowej\n";
		}
	}
	else {
		cout << "\Ksiazka adresowa jest pusta\n";
	}
	system("pause");
}

void edytujAdresataWPliku(vector<Adresat>& adresaci) {
	Adresat adresat;
	int licznik = 0;
	string linia = "";
	fstream plik, plikTymczasowy;
	plik.open(nazwaPlikuZAdresatami.c_str(), ios::in);
	plik.open(nazwaPlikuTymczasowego.c_str(), ios::out);
	do {
		string numerId = "";
		getline(plik, linia);
		for (int i = 0; i < linia.length(); i++) {
			if (linia[i] != '|') {
				numerId += linia[i];
			}
			else {
				break;
			}
		}
		int numer = atoi(numerId.c_str());
		if (linia == "") {
			break;
		}
		else if (numer != adresaci[licznik].idAdresata) {
			plikTymczasowy << linia << endl;
		}
		else {
			linia += konwersjaIntNaString(adresaci[licznik].idAdresata) + '|';
			linia += konwersjaIntNaString(adresaci[licznik].idUzytkownika) + '|';
			linia += adresaci[licznik].imie + '|';
			linia += adresaci[licznik].nazwisko + '|';
			linia += adresaci[licznik].numerTelefonu + '|';
			linia += adresaci[licznik].email + '|';
			linia += adresaci[licznik].adres + '|';
			plikTymczasowy << linia << endl;
			licznik++;
		}
	} while (linia != "");
	plik.close();
	plikTymczasowy.close();

	remove("Adresaci.txt");
	rename("Tymczasowy.txt", "Adresaci.txt");
}

#include <iostream>
#include <random>
#include <time.h>
#include <Windows.h>
#include <process.h>

using namespace std;

const int DIMENSIONE = 10;
const char VUOTO = ' ';

struct nave
{
    string tipo;
    int lunghezza;
};

void inizializza(char matrice[][DIMENSIONE])
{
    for (int i = 0; i < DIMENSIONE; i++)
    {
        for (int j = 0; j < DIMENSIONE; j++)
        {
            matrice[i][j] = VUOTO;
        }
    }
}

void stampaConsole(char matriceGiocatore[][DIMENSIONE], char matricePC[][DIMENSIONE])
{
    cout << "   |";
    for (int i = 'A'; i < 'A' + DIMENSIONE; i++)
    {
        cout << " " << (char)i << " ";
    }
    cout << "        |";
    for (int i = 'A'; i < 'A' + DIMENSIONE; i++)
    {
        cout << " " << (char)i << " ";
    }
    cout << endl;
    int trattini = DIMENSIONE * 3 + 3;
    for (int i = 0; i < trattini; i++)
    {
        cout << '-';
    }
    cout << "      ";
    for (int i = 0; i < trattini; i++)
    {
        cout << '-';
    }
    cout << endl;
    for (int i = 0; i < DIMENSIONE; i++)
    {
        cout << i + 1 << ((i < 9) ? "  |" : " |");
        for (int j = 0; j < DIMENSIONE; j++)
        {
            cout << " " << matriceGiocatore[i][j] << " ";
        }
        cout << "     " << i + 1 << ((i < 9) ? "  |" : " |");
        for (int j = 0; j < DIMENSIONE; j++)
        {
            cout << " " << matricePC[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool posiziona(char matrice[][DIMENSIONE], nave ship, int x, int y, int v)
{
    srand(time(NULL));
    bool check = true;
    switch (v)
    {
    case 0: // ORIZZONTALE
    {
        if (x >= DIMENSIONE - ship.lunghezza)
        {
            return false;
        }
        else
        {
            int index = 0;
            int i = x;
            while (index < ship.lunghezza)
            {
                if (matrice[y][i] != VUOTO)
                {
                    return false;
                }
                index++;
                i++;
            }
            index = 0;
            i = x;
            while (index < ship.lunghezza)
            {
                matrice[y][i] = ship.tipo[0];
                i++;
                index++;
            }
        }
    }
    break;
    case 1: // VERTICALE
    {
        if (y >= DIMENSIONE - ship.lunghezza)
        {
            return false;
        }
        else
        {
            int index = 0;
            int i = y;
            while (index < ship.lunghezza)
            {
                if (matrice[i][x] != VUOTO)
                {
                    return false;
                }
                i++;
                index++;
            }
            index = 0;
            i = y;
            while (index < ship.lunghezza)
            {
                matrice[i][x] = ship.tipo[0];
                index++;
                i++;
            }
        }
    }
    break;
    }
    return check;
}

void impostaNavi(char matrice[][DIMENSIONE])
{
    nave navi[7];
    int n = 0;
    while (n < 2)
    {
        navi[n].lunghezza = 5;
        navi[n].tipo = "corazzata";
        n++;
    }
    while (n < 4)
    {
        navi[n].lunghezza = 3;
        navi[n].tipo = "incrociatore";
        n++;
    }
    while (n < 7)
    {
        navi[n].lunghezza = 2;
        navi[n].tipo = "sottomarino";
        n++;
    }
    int coordX = rand() % DIMENSIONE;
    int coordY = rand() % DIMENSIONE;
    int verso = rand() % 2;

    for (int i = 0; i < 7; i++)
    {
        while (!posiziona(matrice, navi[i], coordX, coordY, verso))
        {
            coordX = rand() % DIMENSIONE;
            coordY = rand() % DIMENSIONE;
            verso = rand() % 2;
        }
    }
}

void stampaConDelay(string frase)
{
    for (int i = 0; i < frase.length(); i++)
    {
        cout << frase[i];
        Sleep(10);
    }
}
bool checkCoordinate(char let, int num)
{
    if (((int)let < 'A') or ((int)let > 'A' + DIMENSIONE) or (num < 1) or (num > 10))
    {
        return false;
    }
    else
    {
        return true;
    }
}

char rendiMaiuscola(char c)
{
    if (((int)c >= 'a') and ((int)c <= 'z'))
    {
        return (int)c - 32;
    }
    else
    {
        return c;
    }
}

void turnoGiocatore(char matriceNascosta[][DIMENSIONE], char matriceVisibile[][DIMENSIONE])
{
    char lettera;
    int numero;
    stampaConDelay("Dammi le coordinate per colpire, prima la lettera, poi il numero...\n");
    cin >> lettera;
    cin >> numero;
    lettera = rendiMaiuscola(lettera);
    while (!checkCoordinate(lettera, numero))
    {
        cout << "Coordinate non valide, riprova. Prima la lettera, poi il numero..." << endl;
        cin >> lettera;
        lettera = rendiMaiuscola(lettera);
        cin >> numero;
    }
    int coordinata = (int)lettera - 65;
    if (matriceNascosta[numero-1][coordinata] != VUOTO)
    {
        cout << "Colpito..." << endl;
        matriceVisibile[numero-1][coordinata] = 'O';
    }
    else
    {
        cout << "Acqua..." << endl;
        matriceVisibile[numero-1][coordinata] = 'X';
    }
}

int main()
{
    string giocatore = "Riccardo";
    char grigliaGiocatore[DIMENSIONE][DIMENSIONE];
    char grigliaPC[DIMENSIONE][DIMENSIONE];
    char grigliaAvversario[DIMENSIONE][DIMENSIONE];
    inizializza(grigliaGiocatore);
    inizializza(grigliaPC);
    inizializza(grigliaAvversario);
    // stampaConDelay("STO POSIZIONANDO LE NAVI..........");
    impostaNavi(grigliaGiocatore);
    impostaNavi(grigliaPC);
    // stampaConDelay("FLOTTE PRONTE ALLA BATTAGLIA!");
    cout << endl
         << endl;
    stampaConsole(grigliaGiocatore, grigliaAvversario);
    // stampaConsole(grigliaGiocatore, grigliaPC);
    system("pause");
    system("cls");
    while (true)
    {
        turnoGiocatore(grigliaPC, grigliaAvversario);
        stampaConsole(grigliaGiocatore, grigliaAvversario);
    }
    
}
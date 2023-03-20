#include <iostream>
#include <random>
#include <time.h>
#include <Windows.h>
#include <process.h>
#include <string>

using namespace std;
const int DIMENSIONE = 10;
const char VUOTO = ' ';
const int NUMERONAVI = 7;

struct nave
{
    string tipo;
    int lunghezza;
};

nave *creaFlotta()
{
    nave *navi = new nave[NUMERONAVI];
    int n = 0;
    while (n < 2)
    {
        navi[n].lunghezza = 5;
        navi[n].tipo = "corazzata" + to_string(n);
        n++;
    }
    while (n < 4)
    {
        navi[n].lunghezza = 3;
        navi[n].tipo = "incrociatore" + to_string(n);
        n++;
    }
    while (n < NUMERONAVI)
    {
        navi[n].lunghezza = 2;
        navi[n].tipo = "sottomarino" + to_string(n);
        n++;
    }
    return navi;
}

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

                matrice[y][i] = ship.tipo[ship.tipo.length() - 1];
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
                matrice[i][x] = ship.tipo[ship.tipo.length() - 1];
                index++;
                i++;
            }
        }
    }
    break;
    }
    return check;
}

void impostaNavi(char matrice[][DIMENSIONE], nave navi[])
{
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
        Sleep(15);
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

bool turnoGiocatore(char matriceNascosta[][DIMENSIONE], char matriceVisibile[][DIMENSIONE])
{
    char lettera;
    int numero;
    stampaConDelay("Dammi le coordinate dell'obiettivo da colpire, prima la lettera e poi il numero...\n");
    cin >> lettera;
    cin >> numero;
    lettera = rendiMaiuscola(lettera);
    while (!checkCoordinate(lettera, numero))
    {
        stampaConDelay("Coordinate non valide, riprova. Prima la lettera, poi il numero...\n");
        cin >> lettera;
        lettera = rendiMaiuscola(lettera);
        cin >> numero;
    }
    int coordinata = (int)lettera - 65;
    if (matriceNascosta[numero - 1][coordinata] != VUOTO)
    {
        matriceVisibile[numero - 1][coordinata] = 'X';
        return true;
    }
    else
    {
        matriceVisibile[numero - 1][coordinata] = 'O';
        return false;
    }
}

bool finePartita(char matrice[][DIMENSIONE])
{
    int count = 0;
    for (int i = 0; i < DIMENSIONE; i++)
    {
        for (int j = 0; j < DIMENSIONE; j++)
        {
            if ((matrice[i][j] == 'c') or (matrice[i][j] == 's') or (matrice[i][j] == 'i') or (matrice[i][j] == 'X'))
            {
                count++;
            }
        }
    }
    cout << count << endl;
    return (count == 22);
}

int *checkBersagli(char matriceNascosta[][DIMENSIONE], char matriceVisibile[][DIMENSIONE])
{
    int *numeri = new int[NUMERONAVI];
    int indiceNavi = 0;
    for (int i = 0; i < NUMERONAVI; i++)
    {
        numeri[i] = 0;
    }
    for (int i = 0; i < DIMENSIONE; i++)
    {
        for (int j = 0; j < DIMENSIONE; j++)
        {
            if (matriceVisibile[i][j] == 'X')
            {
                indiceNavi = (int)matriceNascosta[i][j] - 48;
                numeri[indiceNavi]++;
            }
        }
    }
    return numeri;
}

void impostaGiocatoreVisibile(char matriceNascosta[][DIMENSIONE], char matriceVisibile[][DIMENSIONE])
{
    for (int i = 0; i < DIMENSIONE; i++)
    {
        for (int j = 0; j < DIMENSIONE; j++)
        {
            if (matriceNascosta[i][j] != VUOTO)
            {
                matriceVisibile[i][j] = matriceNascosta[i][j];
            }
        }
    }
}

void mostraNaveAbbattuta(char matriceNascosta[][DIMENSIONE], char matriceVisibile[][DIMENSIONE], int numeroNave)
{
    char car;
    if ((numeroNave == 0) or (numeroNave == 1))
    {
        car = 'c';
    }
    else if ((numeroNave == 2) or (numeroNave == 3))
    {
        car = 'i';
    }
    else
    {
        car = 's';
    }

    for (int i = 0; i < DIMENSIONE; i++)
    {
        for (int j = 0; j < DIMENSIONE; j++)
        {
            if (matriceNascosta[i][j] == (char)(numeroNave + 48))
            {
                matriceVisibile[i][j] = car;
            }
        }
    }
}

int isColpitoEAffondato(int bersagli[])
{
    int n = 0;
    int indice = -1;
    while (n < 2)
    {
        //cout << n << "      " << indice;
        if (bersagli[n] == 5)
        {
            indice = n;

        }
        n++;
    }
    while (n < 4)
    {
        //cout << n << "      " << indice;
        if (bersagli[n] == 3)
        {
            indice = n;
        }
        n++;
    }
    while (n < NUMERONAVI)
    {
        //cout << n << "      " << indice;
        if (bersagli[n] == 2)
        {
            indice = n;
        }
        n++;
    }
    return indice;
}

int main()
{
    bool turno = true;
    string giocatore = "Riccardo";
    nave *flotta = creaFlotta();
    char grigliaGiocatoreNascosta[DIMENSIONE][DIMENSIONE];
    char grigliaPCNascosta[DIMENSIONE][DIMENSIONE];
    char grigliaPCVisibile[DIMENSIONE][DIMENSIONE];
    char grigliaGiocatoreVisibile[DIMENSIONE][DIMENSIONE];
    inizializza(grigliaGiocatoreNascosta);
    inizializza(grigliaPCNascosta);
    inizializza(grigliaPCVisibile);
    inizializza(grigliaGiocatoreVisibile);
    stampaConDelay("STO POSIZIONANDO LE NAVI..........");
    impostaNavi(grigliaGiocatoreNascosta, flotta);
    impostaNavi(grigliaPCNascosta, flotta);
    /*
    grigliaGiocatoreNascosta[2][2]='6';
    grigliaGiocatoreNascosta[2][3]='6';

    grigliaGiocatoreNascosta[4][8]='0';
    grigliaGiocatoreNascosta[5][8]='0';
    grigliaGiocatoreNascosta[6][8]='0';
    grigliaGiocatoreNascosta[7][8]='0';
    grigliaGiocatoreNascosta[8][8]='0';

    grigliaGiocatoreNascosta[2][7]='4';
    grigliaGiocatoreNascosta[3][7]='4';
    grigliaGiocatoreNascosta[4][7]='4';

    grigliaPCNascosta[2][2]='6';
    grigliaPCNascosta[2][3]='6';

    grigliaPCNascosta[4][8]='0';
    grigliaPCNascosta[5][8]='0';
    grigliaPCNascosta[6][8]='0';
    grigliaPCNascosta[7][8]='0';
    grigliaPCNascosta[8][8]='0';

    grigliaPCNascosta[2][7]='2';
    grigliaPCNascosta[3][7]='2';
    grigliaPCNascosta[4][7]='2';
*/
    stampaConsole(grigliaGiocatoreNascosta, grigliaPCNascosta);
    impostaGiocatoreVisibile(grigliaGiocatoreNascosta, grigliaGiocatoreVisibile);
    stampaConDelay("FLOTTE PRONTE ALLA BATTAGLIA!");
    cout << endl
         << endl;
    Sleep(1500);

    stampaConsole(grigliaGiocatoreVisibile, grigliaPCVisibile);
    // stampaConsole(grigliaGiocatoreNascosta, grigliaPCNascosta);
    int *bersagliColpiti = new int[NUMERONAVI];
    do
    {
        if (turno)
        {
            Sleep(1500);
            while (turnoGiocatore(grigliaPCNascosta, grigliaPCVisibile) and (!finePartita(grigliaPCVisibile)))
            {
                //cout << "entro" << endl;
                Sleep(1500);
                bersagliColpiti = checkBersagli(grigliaPCNascosta, grigliaPCVisibile);

                /*for (int i =0; i<NUMERONAVI; i++)
                {
                    cout << bersagliColpiti[i];
                }*/

                int affondato = isColpitoEAffondato(bersagliColpiti);
                // cout << affondato << endl;
                if (affondato != -1)
                {
                    mostraNaveAbbattuta(grigliaPCNascosta, grigliaPCVisibile, affondato);
                    stampaConDelay("Colpito e affondato!\n");
                }
                stampaConsole(grigliaGiocatoreVisibile, grigliaPCVisibile);
            }
            stampaConsole(grigliaGiocatoreVisibile, grigliaPCVisibile);
            turno = !turno;
        }
        else
        {
            stampaConDelay("Tocca di nuovo a te...");
            turno = !turno;
        }
    } while (!finePartita(grigliaPCVisibile));
    system("pause");
}
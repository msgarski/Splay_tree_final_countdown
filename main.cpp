#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <ctime>

using namespace std;

int licznik=0;
int k;
bool tab[29976]{};
struct Node
{
    int klucz;
    char ch[16]{};
    Node *right;
    Node *left;

};

///*******************************************************************************************************************

///     BLOK FUNKCJI WYSWIETLAJACYCH SKLADNIKI DRZEWA

///*******************************************************************************************************************
void preorder(Node *root)       ///     OK
{
    if(root)
    {
        cout<<root->klucz<<" "<<"zawartosc jego tablicy: ";
            for(int i=0; i<16; i++)
            {
                if(root->ch[i])
                cout<<root->ch[i];
            }
            cout<<endl;
            licznik++;
        preorder(root->left);
        preorder(root->right);
    }
    return;
}
void preorder_licz(Node *root)  ///     OK
{
    licznik=0;
    cout<<"Wyswietlanie w trybie preorder: "<<endl;
    preorder(root);
    cout<<endl<<"licznik preorder = "<<licznik<<endl;
    if(!licznik)
        cout<<"Drzewo nie zawiera zadnych elementow!"<<endl<<endl;
}
///zwykla funkcja inorder
void inorder(Node *root)        ///     OK
{
         if(root)
        {
            inorder(root->left);
            cout<<root->klucz<<" "<<"zawartosc jego tablicy: ";
            for(int i=0; i<16; i++)
            {
                if(root->ch[i])
                cout<<root->ch[i];
            }
            cout<<endl;
            licznik++;
            inorder(root->right);
        }
    return;
}

///funkcja inorder do zliczania odwiedzonych wezlow
void inorder_licz(Node *root)   ///     OK
{
    licznik=0;
    cout<<"Wyswietlanie w trybie inorder: "<<endl;
    inorder(root);
    cout<<endl<<"licznik inorder = "<<licznik<<endl;
    if(!licznik&&!root) ///dodalem dodatkowy warunek sprawdzajacy
        cout<<"Drzewo nie zawiera zadnych elementow!"<<endl<<endl;
}
///funkcja do zaznaczania w tablicy juz istniejacych wezlow
///jako pomoc dla funkcji wstawiajacej X elementow
void inorder_stan(Node *root, bool *a)///   OK
{
         if(root)
        {
            inorder_stan(root->left, a);        ///rekurencyjnie w porzadku poprzecznym przechodzi drzewo
                                                ///i wracajac zaznacza w przekazanej tablicy wystapienia
            a[root->klucz-25]=true;             ///kluczy w odwiedzonych wezlach
            inorder_stan(root->right, a);
        }
    return;
}
///**************************************************************************************************************

///     BLOK FUNKCJI ROTUJACYCH

///**************************************************************************************************************
void rotacja_R(Node ** parent)  ///     OK
{

    Node * x=(*parent)->left;
    (*parent)->left=x->right;
    x->right=*parent;
    *parent=x;
}

void rotacja_LR(Node ** grand, Node ** parent, Node ** p)///    OK
{
    Node *x=(*grand)->left;
    Node *y=x->right;
    x->right=y->left;
    y->left=x;
    (*grand)->left=y->right;
    y->right=*grand;
    *grand=y;
}

void rotacja_L(Node ** parent)  ///     OK
{
    Node * x=(*parent)->right;
    (*parent)->right=x->left;
    x->left=*parent;
    *parent=x;
}

void rotacja_RL(Node ** grand, Node ** parent, Node ** p)///      OK
{
    Node *x=(*grand)->right;
    Node *y=x->left;
    x->left=y->right;
    y->right=x;
    (*grand)->right=y->left;
    y->left=*grand;
    *grand=y;


}

///********************************************************************************************************

///     BLOK FUNKCJI SZUKAJACYCH

///********************************************************************************************************
void szukanie_pre_grand(Node * grand, Node **pre_grand, Node * root)    ///     OK
{
*pre_grand=nullptr;
Node *dziad=root;
int k=grand->klucz;
//cout<<"Rozpoczynam szukanie wezla o kluczu dziadka: "<<grand->klucz<<endl;

    while(k!=dziad->klucz)      ///rob to, co w petli, az bedzie sukces lub sciana
    {   ///zakladam ze klucz istnieje na 100% w drzewie, bo to szukanie jest gdy rotujemy wezel
        /// lub po udanym wstawieniu wezla
        *pre_grand=dziad;
        if(k>dziad->klucz)
            dziad=dziad->right;
        else
            dziad=dziad->left;
    }
}
void szukaj(Node **root, int x)///      OK
{
    if(!(*root))
    {
        cout<<"Drzewo nie zawiera zadnych wezlow!, brak szukanego wezla!"<<endl;
        return;
    }

///szukanie p i jednoczesne podciaganie za soba dziadka, oraz rodzica:
Node *p=nullptr;
///duza petla while: szuka, znajduje, rotuje az do korzenia
while(x!=(*root)->klucz) ///lub p!=*root
{
///szukanie wezla o kluczu x, wraz z ustaleniem jego rodzica i dziadka
    p=*root;
    Node *grandpa_p=nullptr;
    Node *parent_p=p;

///mala petla while - tylko szuka wezla o kluczu x i pociaga za soba ojca i dziadka
    while(p!=nullptr&&x!=p->klucz)
    {

///      Gdy szukany wezel jest w lewym poddrzewie:
        if(x<p->klucz)
        {
            if(p->left)     /// czy lewy wezel istnieje
            {
                grandpa_p=parent_p;
                parent_p=p;
                p=p->left;  ///przesuwam p krok dalej, wiec inne wskazniki tez musza nadazac

                if(x==p->klucz)     ///klucz znaleziony, wiec IDZIEMY DO DZIALU ROTACJI
                {
                    break;
                }
                ///skoro to nie ten wezel, to patrzymy, w ktora strone teraz...
                else if(x<p->klucz) /// czy w lewo...
                {
                    grandpa_p=parent_p;
                    parent_p=p;
                    p=p->left;
                }
                else                /// czy w prawo
                {
                    grandpa_p=parent_p;
                    parent_p=p;
                    p=p->right;
                }
            }
            else    ///nie ma lewego wezla, wiec nie ma x w drzewie
            {
                cout<<"BLAD!!! Wezla o kluczu "<<x<<" , nie ma w drzewie."<<endl;
                p=nullptr;
                return;
            }
        }
        else ///czyli szukany x jest po prawej stronie drzewa
        {
            if(p->right)     /// czy lewy wezel istnieje
            {
                grandpa_p=parent_p;
                parent_p=p;
                p=p->right;  ///przesuwam p krok dalej, wiec inne wskazniki tez musza nadazac

                if(x==p->klucz)     ///klucz znaleziony, wiec idziemy do dzialu rotacji
                {
                    break;
                }
                ///skoro to nie ten wezel, to patrzymy, w ktora strone teraz...
                else if(x<p->klucz) /// czy w lewo...
                {
                    grandpa_p=parent_p;
                    parent_p=p;
                    p=p->left;
                }
                else                /// czy w prawo
                {
                    grandpa_p=parent_p;
                    parent_p=p;
                    p=p->right;
                }

            }
            else    ///nie ma lewego wezla, wiec nie ma x w drzewie
            {
                cout<<"BLAD!!! Wezla o kluczu "<<x<<" , nie ma w drzewie."<<endl;
                p=nullptr;
                return;
            }
        }
    }
    ///zakonczyl szukanie, ale czy znalazl???:
    if (!p)     ///jednak nie znalazl  p, wychodzimy z funkcji
    {
        cout<<"Nie udalo sie znalezc wezla o tym kluczu"<<endl<<endl<<endl;
        return;
    }

///********************************************************************************************************
/// Obsluga wszystkich rotacji dla lewego i prawego drzewa:
///********************************************************************************************************

///*****************************************************************************************************************
///obsluga pojedynczych rotacji:
///*****************************************************************************************************************
    if(p==*root)
        return;     ///rotacja niepotrzebna, nasz wezel jest juz rootem, wychodzimy z calej funkcji
    else if(parent_p==*root)   /// rodzic=dziadek to root, wiec wystarczy pojedyncza rotacja w lewo lub w prawo
    {
        if(x<(*root)->klucz)    ///rotacja pojedyncza w prawo zrobiona do końca
        {
            rotacja_R(&parent_p);
            *root=parent_p;
            cout<<"root po rotacji "<<(*root)->klucz<<endl;
            return;     ///rotacja zrobiona, wychodzimy z calej funkcji
        }
        else                    /// rotacja pojedyncza w lewo zrobiona
        {
            rotacja_L(&parent_p);
            *root=parent_p; ///a nie czasem root==p?
            cout<<"root po rotacji "<<(*root)->klucz<<endl;
            return;     ///rotacja zrobiona, wychodzimy z calej funkcji
        }
    }

///***************************************************************************************************************
/// gdy po podwojnej rotacji zmieni sie root
///***************************************************************************************************************
       else if(grandpa_p==*root)
        {
            if(grandpa_p->left==parent_p)
            {
                if(parent_p->left==p) /// jednorodna
                {
                    ///rotacja podwojna w prawo rodzic w prawo p
                    rotacja_R(&grandpa_p);
                    rotacja_R(&grandpa_p);
                    *root=grandpa_p;
                    cout<<"root po rotacji "<<(*root)->klucz<<endl;

                }
                else                  /// niejednorodna w dwoch wersjach
                {
                ///rotacja podwojna w lewo p i w prawo p skomasowana...
                    //rotacja_LR(&grandpa_p, &parent_p, &p);
                ///rotacja podwojna w lewo p i w prawo p rozdzielona na pojedyncze rotacje:
                    rotacja_L(&parent_p);
                    grandpa_p->left=parent_p;
                    rotacja_R(&grandpa_p);
                ///czesc wspolna - zmiana roota:
                    *root=grandpa_p;
                    cout<<"root po rotacji "<<(*root)->klucz<<endl;
                }
            }
            else    /// jesli grandpa_p->right==p
            {
                if(parent_p->right==p) ///jednorodna
                {
                    ///rotacja podwojna w lewo rodzic w lewo p
                     //cout<<"Rotacja podwojna w lewo"<<endl;
                    rotacja_L(&grandpa_p);
                    rotacja_L(&grandpa_p);
                    *root=grandpa_p;
                    cout<<"root po rotacji "<<(*root)->klucz<<endl;

                }
                else    ///     niejednorodna
                {
                ///rotacja podwojna w prawo p i w lewo p skomasowana...
                  //rotacja_RL(&grandpa_p, &parent_p, &p);
                ///rotacja podwojna w prawo p i w lewo p rozdzielona na pojedyncze rotacje:
                    rotacja_R(&parent_p);
                    grandpa_p->right=parent_p;
                    rotacja_L(&grandpa_p);
                ///czesc wspolna - zmiana roota:
                    *root=grandpa_p;
                    cout<<"root po rotacji "<<(*root)->klucz<<endl;
                }
            }
        }

///***********************************************************************************************************************
///przypadek, gdy wskutek rotacji nie zmieni sie root:
///***********************************************************************************************************************
        else    ///tutaj sa tylko podwojne rotacje
        {
               ///no to musimy szukac pradziadka:
            Node *pre_grandpa_p=nullptr;
            szukanie_pre_grand(grandpa_p, &pre_grandpa_p, *root);

            if(grandpa_p->left==parent_p)
            {
                if(parent_p->left==p)   /// jednorodna
                {///rotacja podwojna w prawo rodzic w prawo p
                    rotacja_R(&grandpa_p);
                    rotacja_R(&grandpa_p);
                }
                else    /// niejednorodna
                {
                ///rotacja podwojna w lewo p i w prawo p skomasowana...
                    //rotacja_LR(&grandpa_p, &parent_p, &p);
                ///lub rotacja podwojna w lewo p i w prawo p rozdzielona na pojedyncze rotacje:
                    rotacja_L(&parent_p);
                    grandpa_p->left=parent_p;
                    rotacja_R(&grandpa_p);
                }
            }
            else    /// jesli grandpa_p->right==p
            {
                if(parent_p->right==p) ///jednorodna
                {
                    ///rotacja podwojna w lewo rodzic w lewo p
                    rotacja_L(&grandpa_p);
                    rotacja_L(&grandpa_p);
                }
                else    ///     niejednorodna
                {
                ///rotacja podwojna w prawo p i w lewo p skomasowana...
                   // rotacja_RL(&grandpa_p, &parent_p, &p);
                ///lub rotacja podwojna w prawo p i w lewo p rozdzielona na pojedyncze rotacje:
                    rotacja_R(&parent_p);
                    grandpa_p->right=parent_p;
                    rotacja_L(&grandpa_p);
                }
            }

    ///     a teraz odtworzenie dowiazania do reszty drzewa
    ///     po wykonaniu podwojnej rotacji
                    if(grandpa_p->klucz<pre_grandpa_p->klucz)
                        pre_grandpa_p->left=grandpa_p;
                    else
                        pre_grandpa_p->right=grandpa_p;
        }
}
    return;
}


///************************************************************************************************************

///     BLOK FUNKCJI WSTAWIAJACYCH

///*************************************************************************************************************

int wstawienie(Node **root, int klucz)/// brak dziadka do rotacji
{
///zaczynamy szukac miejsca na nowy wezel
cout<<"rozpoczynam wstawianie wezla o kluczu "<<klucz<<endl<<endl;
        Node *p=*root;
        Node *rodzic=nullptr;
        if(*root)
        {
            while(p!=nullptr)
            {
                if(p->klucz==klucz) /// obsluga przypadku powtorzenia wstawianego klucza
                {
                    cout<<"BLAD! W drzewie jest juz wezel o kluczu = "<<klucz<<endl<<endl;
                    return 1;
                }
                rodzic=p;
                if(p->klucz>klucz)
                    p=p->left;
                else
                    p=p->right;        ///znajdujemy miejsce na stworzenie naszego wezla-liscia
            }
        }

///alokacja pamieci na nowy wezel listy
        Node *nowy= new Node;
        if(!nowy)
        {
            cout<<"Nie udalo sie utworzyc nowego wezla drzewa!\n Brak pamieci!"<<std::endl;
            return -1;                  ///gdy brak pamieci na wezel
        }

///inicjalizacja liczbowych zmiennych wezla:
        nowy->klucz=klucz;

///potrzeba jeszcze wypelnic skladowa tablice wezla....
        for(int i=0; i<16; i++)
        {
            nowy->ch[i]=(rand()%23)+65;
        }
        nowy->right=nullptr;
        nowy->left=nullptr;

/// jesli to bedzie pierwszy wezel w drzewie, to robimy go rootem
       if(!*root)
       {
          *root=nowy;
            return 0;
        }
/// jesli drzewo ma juz roota, to: podczepienie naszego wezla pod rodzica:
        else if(rodzic->klucz>klucz)
            rodzic->left=nowy;
        else
            rodzic->right=nowy;

///tutaj wstawie funkcje rotacji:
        szukaj(root, klucz);
        return 0;
}

///***********************************************************************************************************
void wstawienie_X_elementow(Node **root, int X)///gotowa
{
    int los;
    bool tabl_losowych[29976];
    srand(time(0));

    ///przejscie przez drzewo aby zaktualizowac tablice wystepujacych juz w drzewie kluczzy
    inorder_stan(*root, tabl_losowych);

    ///proces wstawiania wezla:
        for(int i=0; i<X; i++)
        {
            ///losowanie unikalnej liczby i przeslanie jej do wezla listy
            do
            {
                los=(rand()%29976);
            }
            while(tabl_losowych[los]);
            tabl_losowych[los]=true;        ///zapamietanie wylosowania danej liczb

            ///wywolanie funkcji wstawiajacej dla wylosowanego klucza
            wstawienie(root, los+25);
        }
        return;
}

///****************************************************************************************************************

///         USUWANIE WEZLA

///********************************************************************************************************************
void usuwanie(Node **root, int x)
{
///tymczasowo wprowadzam wskaznik:
Node *p=nullptr;

/// tu po wyszukaniu i rotacji, powinienem miec p==root
szukaj(root, x);
p=*root;
cout<<"Usuwanie wezla "<<x<<endl;
///szukanie jego poprzednika:
    ///  czy p,  vel root, ma potomstwo?
    if(x!=(*root)->klucz)
        cout<<"naprawde nie ma takiego wezla"<<endl;
    else    /// (p==*root)
    {
        /// Gdy root nie ma potomkow:
        if(p->left==nullptr&&p->right==nullptr)
        {
            *root=nullptr;          ///zerujemy roota
            delete p;               ///zwalniamy pamiec po p
            cout<<"Wezel usuniety..."<<endl<<endl;
            return;
        }
        ///ma tylko lewe poddrzewo:
        else if(!p->right)
        {
            *root=p->left;
            delete p;
        }
        ///ma tylko prawe potomstwo:
        else if(!p->left)
        {
            *root=p->right;
            delete p;
            return;
        }
    ///***********************************************************************************************
    /// A wiec ma obu potomków i potrzeba nam poprzednika:
    ///podciagamy za soba rodzica szukanego poprzednika
    ///***********************************************************************************************
    Node* poprzednik=p->left;
    Node *rodzic_poprzednika=p;
    while(poprzednik->right)
    {
        rodzic_poprzednika=poprzednik;
        poprzednik=poprzednik->right;
    }
cout<<"poprzednik "<<poprzednik->klucz<<endl;
    ///mamy poprzednika, teraz go obejrzymy i wytniemy...

    if(poprzednik==p->left) ///gdy poprzednik jest tuz obok p, po lewej stronie,
    {
        *root=p->left;
        (*root)->right=p->right;
        delete p;
        return;
    }
    else
    {
    ///gdy poprzednik jest gdzies na koncu...

    ///wyluskanie poprzednika:
        rodzic_poprzednika->right=poprzednik->left;
        poprzednik->left=nullptr;

    /// wstawienie poprzednika w miejsce usuwanego wezla p
        poprzednik->left=(*root)->left;
        poprzednik->right=(*root)->right;
        *root=poprzednik;
        delete p;

    return;
    }
    }
}

///*****************************************************************************************************

///         WLASCIWA FUNKCJA MAIN

///*****************************************************************************************************


int main()
{
    srand(time(0));
    //wczytanie z pliku************************************************************************8
        int n, flaga=1;
        int k1, k2, k3;

        FILE* fp=fopen("lab4.txt", "r");
            if(fp==NULL)
            return -1;
        fscanf(fp, "%d %d %d %d", &n, &k1, &k2, &k3);
        fclose(fp);

    ///czas start;*************************************************************************************8
        clock_t begin, end;
        double time_spent;
        begin = clock();

///zainicjuj „puste” drzewo;
Node *drzewo=nullptr;

///wstaw X elementow do drzewa;
wstawienie_X_elementow(&drzewo, n);

///wyswietl klucze wszystkich wezlow drzewa w porzadku inorder;
inorder_licz(drzewo);

///wyswietl klucze wszystkich wezlow drzewa w porzadku preorder;
preorder_licz(drzewo);

///wstaw do drzewa wezel o kluczu k1 lub zwiekszaj o 1 dotychczasowy klucz do skutku
    while(flaga)
    {
        flaga=wstawienie(&drzewo, k1);
        k1++;
    }

///wyswietl klucze wszystkich wezlow drzewa w porzadku preorder;
preorder_licz(drzewo);

///wstaw do drzewa wezel o kluczu k2 lub zwiekszaj o 1 dotychczasowy klucz do skutku
    flaga=1;
    while(flaga)
        {
            flaga=wstawienie(&drzewo, k2);
            k1++;
        }

///wyswietl klucze wszystkich wezlow drzewa w porzadku preorder;
preorder_licz(drzewo);

///wyszukaj w drzewie węzeł o kluczu k3;
cout<<"szukam wezla "<<k3<<endl<<endl;
szukaj(&drzewo, k3);

///wyswietl klucze wszystkich wezlow drzewa w porzadku preorder;
preorder_licz(drzewo);

///usun z drzewa węzeł o kluczu k2;
usuwanie(&drzewo, k2);

///wyswietl klucze wszystkich wezlow drzewa w porzadku preorder;
preorder_licz(drzewo);

///wyswietl klucze wszystkich wezlow drzewa w porzadku inorder;
inorder_licz(drzewo);

///czas stop;
        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

///wypisz czas wykonania.
cout<<"Czas wykonania: "<<time_spent<<endl;

        return 0;
}











/*
while(flaga==-1)
{
    flaga=wstawienie(&drzewo, k1);
    cout<<"zwiekszyc o 1"<<endl;
    k1++;
    cout<<"k1 = "<<k1<<endl;
}while(flaga==-1);
*/


/*
void szukaj(Node **root, int x)///gotowa
{
    cout<<"Rozpoczynam szukanie wezla o kluczu: "<<x<<endl;
    if(!(*root))
    {
        cout<<"Drzewo nie zawiera zadnych wezlow!, brak szukanego wezla!"<<endl;
        return;
    }

///szukanie p i jednoczesne podciaganie za soba dziadka, oraz rodzica:
Node *p=nullptr;
///duza petla while: szuka, znajduje, rotuje az do korzenia
while(x!=(*root)->klucz) ///lub p!=*root
{
///szukanie wezla o kluczu x, wraz z ustaleniem jego rodzica i dziadka
    p=*root;
    Node *grandpa_p=nullptr;
    Node *parent_p=p;
    int licznik_while=0;

///mala petla while - tylko szuka wezla o kluczu x i pociaga za soba ojca i dziadka
    while(p!=nullptr&&x!=p->klucz)
    {
        licznik_while++;
        //cout<<"wchodzimy do while "<<licznik_while<<" raz"<<endl;

///      Gdy szukany wezel jest w lewym poddrzewie:
        if(x<p->klucz)
        {
            //cout<<"jestem w lewym poddrzewie"<<endl;
            if(p->left)     /// czy lewy wezel istnieje
            {
                //cout<<"lewy wezel istnieje"<<endl;
                grandpa_p=parent_p;
                parent_p=p;
                p=p->left;  ///przesuwam p krok dalej, wiec inne wskazniki tez musza nadazac

                if(x==p->klucz)     ///klucz znaleziony, wiec jaka rotacja jest potrzebna?
                {
                    //cout<<"pierwszy lewy wezel i jest klucz"<<endl;
                    break;
                }
                ///skoro to nie ten wezel, to patrzymy, w ktora strone teraz...
                else if(x<p->klucz) /// czy w lewo...
                {
                    //cout<<"wybor lewej drogi do drugiego wezla"<<endl;
                    grandpa_p=parent_p;
                    parent_p=p;
                    p=p->left;
                }
                else                /// czy w prawo
                {
                    //cout<<"wybor prawej drogi do drugiego wezla"<<endl;
                    grandpa_p=parent_p;
                    parent_p=p;
                    p=p->right;
                }
            }
            else    ///nie ma lewego wezla, wiec nie ma x w drzewie
            {
                cout<<"BLAD!!! Wezla o kluczu "<<x<<" , nie ma w drzewie."<<endl;
                p=nullptr;
                return;
            }
        }
        else ///czyli szukany x jest po prawej stronie drzewa
        {
            //cout<<"wejscie w prawe niedokonczone poddrzewo"<<endl;

           // cout<<"jestem w prawym poddrzewie"<<endl;
            if(p->right)     /// czy lewy wezel istnieje
            {
                //cout<<"prawy wezel istnieje"<<endl;
                grandpa_p=parent_p;
                parent_p=p;
                p=p->right;  ///przesuwam p krok dalej, wiec inne wskazniki tez musza nadazac

                if(x==p->klucz)     ///klucz znaleziony, wiec jaka rotacja jest potrzebna?
                {
                    //cout<<"pierwszy lewy wezel i jest klucz"<<endl;
                    break;
                }
                ///skoro to nie ten wezel, to patrzymy, w ktora strone teraz...
                else if(x<p->klucz) /// czy w lewo...
                {
                  //  cout<<"wybor lewej drogi do drugiego wezla"<<endl;
                    grandpa_p=parent_p;
                    parent_p=p;
                    p=p->left;
                }
                else                /// czy w prawo
                {
                   // cout<<"wybor prawej drogi do drugiego wezla"<<endl;
                    grandpa_p=parent_p;
                    parent_p=p;
                    p=p->right;
                }

            }
            else    ///nie ma lewego wezla, wiec nie ma x w drzewie
            {
                cout<<"BLAD!!! Wezla o kluczu "<<x<<" , nie ma w drzewie."<<endl;
                p=nullptr;
                return;
            }
        }
    }
    ///zakonczyl szukanie, ale czy znalazl???:
    if (!p)     ///jednak nie znalazl  p, wychodzimy z funkcji
    {
        cout<<"Nie udalo sie znalezc wezla o tym kluczu"<<endl<<endl<<endl;
        return;
    }

///********************************************************************************************************
/// Obsluga wszystkich rotacji dla lewego i prawego drzewa:
///********************************************************************************************************

///*****************************************************************************************************************
///obsluga pojedynczych rotacji:
///*****************************************************************************************************************
    if(p==*root)
        return;     ///rotacja niepotrzebna, wychodzimy z calej funkcji
    else if(parent_p==*root)   /// rodzic=dziadek to root, wiec wystarczy pojedyncza rotacja w lewo lub w prawo
    {
        if(x<(*root)->klucz)    ///rotacja pojedyncza w prawo zrobiona do końca
        {
            //cout<<"Rotacja pojedyncza w prawo"<<endl;
            rotacja_R(&parent_p);
            *root=parent_p;
            cout<<"root po rotacji "<<(*root)->klucz<<p->klucz<<endl;
            preorder(*root);///do usuniecia
            return;     ///rotacja zrobiona, wychodzimy z calej funkcji
        }
        else                    /// rotacja pojedyncza w lewo zrobiona
        {
            //cout<<"Rotacja pojedyncza w lewo"<<endl;
            rotacja_L(&parent_p);
            *root=parent_p; ///a nie czasem root==p?
            cout<<"root po rotacji "<<(*root)->klucz<<p->klucz<<endl;
            preorder(*root);///do usuniecia
            return;     ///rotacja zrobiona, wychodzimy z calej funkcji
        }
    }
///***************************************************************************************************************
/// gdy po podwojnej rotacji zmieni sie root
///***************************************************************************************************************
       else if(grandpa_p==*root)
        {
            if(grandpa_p->left==parent_p)
            {
                if(parent_p->left==p) /// jednorodna
                {
                    ///rotacja podwojna w prawo rodzic w prawo p
                    rotacja_R(&grandpa_p);
                    rotacja_R(&grandpa_p);
                    *root=grandpa_p;
                }
                else                  /// niejednorodna
                {
                    ///rotacja podwojna w lewo p i w prawo p
                    rotacja_LR(&grandpa_p, &parent_p, &p);
                    *root=grandpa_p;
                }
            }
            else    /// jesli grandpa_p->right==p
            {
                if(parent_p->right==p) ///jednorodna
                {
                    ///rotacja podwojna w lewo rodzic w lewo p
                     //cout<<"Rotacja podwojna w lewo"<<endl;
                    rotacja_L(&grandpa_p);
                    rotacja_L(&grandpa_p);
                    *root=grandpa_p;
                }
                else    ///     niejednorodna
                {
                    ///rotacja podwojna w prawo p w lewo p
                    rotacja_RL(&grandpa_p, &parent_p, &p);
                    *root=grandpa_p;
                }
            }
        }
///***********************************************************************************************************************
///przypadek, gdy wskutek rotacji nie zmieni sie root:
///***********************************************************************************************************************
        else    ///tutaj sa tylko podwojne rotacje
        {       ///no to musimy szukac pradziadka:
            Node *pre_grandpa_p=nullptr;
            szukanie_pre_grand(grandpa_p, &pre_grandpa_p, *root);

            if(grandpa_p->left==parent_p)
            {
                if(parent_p->left==p)   /// jednorodna
                {///rotacja podwojna w prawo rodzic w prawo p
                    //cout<<"Rotacja podwojna w prawo"<<endl;
                    rotacja_R(&grandpa_p);
                    rotacja_R(&grandpa_p);
                }
                else    /// niejednorodna
                {
                    ///rotacja podwojna w lewo p i w prawo p
                    rotacja_LR(&grandpa_p, &parent_p, &p);
                }
            }
            else    /// jesli grandpa_p->right==p
            {
                if(parent_p->right==p) ///jednorodna
                {
                    ///rotacja podwojna w lewo rodzic w lewo p
                    rotacja_L(&grandpa_p);
                    rotacja_L(&grandpa_p);
                }
                else    ///     niejednorodna
                {
                    ///rotacja podwojna w prawo p w lewo p
                    rotacja_RL(&grandpa_p, &parent_p, &p);
                }
            }

    ///     a teraz odtworzenie dowiazania do reszty drzewa
    ///     po wykonaniu podwojnej rotacji
                    if(grandpa_p->klucz<pre_grandpa_p->klucz)
                        pre_grandpa_p->left=grandpa_p;
                    else
                        pre_grandpa_p->right=grandpa_p;
        }

           // cout<<"root po rotacji "<<(*root)->klucz<<endl;
            //preorder_licz(*root);///do usuniecia
}
    return;
}

*/







#include "jeu.h"
#include <iostream>
#include <assert.h>

Fantome::Fantome()
{
    posX = 0; posY = 0;
    dir = BAS;
}

int Fantome::getPosX() const
{
    return posX;
}

int Fantome::getPosY() const
{
    return posY;
}

Jeu::Jeu()
{
    terrain = NULL;
    largeur = 0; hauteur = 0;
    posPacmanX = 0; posPacmanY = 0;
    PointVie = 0;
    NbGomme = 0;
    niveau = EASY;
}

Jeu::~Jeu()
{
    if (terrain!=NULL)
        delete[] terrain;
}

void Jeu::set_Largeur(const int l)
{
    largeur = l;
}

void Jeu::set_Hauteur(const int h)
{
    hauteur = h;
}

bool Jeu::init()             // Niveau 1 (EASY Mode)
{
	int x, y;
	Set_PointVie(3);
	list<Fantome>::iterator itFantome;
	Set_Niveau(EASY);

        const char terrain_defaut[17][21] = {
            "....................",
            "....................",
            "####################",
            "#...../..##........#",
            "#.#####..##...####.#",
            "#........##........#",
            "#..................|",
            "#......#....#......#",
            "#....*.#...##......#",
            "#####..#....#..#####",
            "#......##...#......#",
            "#......#....#......#",
            "#..............*...#",
            "#..................#",
            "#.....#......#.....#",
            "#.....#......#.....#",
            "####################"
        };

        largeur = 20;
        hauteur = 17;

        terrain = new Case[largeur*hauteur];

        for(y=0;y<hauteur;++y)
            for(x=0;x<largeur;++x)
                if (terrain_defaut[y][x]=='#')
                    terrain[y*largeur+x] = MUR;
                else if (terrain_defaut[y][x]=='.')
                    terrain[y*largeur+x] = VIDE;
                else if (terrain_defaut[y][x]=='/')
                    terrain[y*largeur+x] = GOMME;
                else if (terrain_defaut[y][x]=='|')
                    terrain[y*largeur+x] = PORTE;
                else terrain[y*largeur+x] = BONUS;

        fantomes.resize(3);

        for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
        {
            do {
                x = rand()%largeur;
                y = rand()%hauteur + 3;
            } while (terrain[y*largeur+x]!=VIDE);

            itFantome->posX = x;
            itFantome->posY = y;
            itFantome->dir = (Direction)(rand()%4);
        }

        do {
            x = rand()%largeur;
            y = rand()%hauteur;
        } while (terrain[y*largeur+x]!=VIDE);

        posPacmanX = 3;         // Position initial du PacMan
        posPacmanY = 3;

        return true;
}

bool Jeu::niveau2()             // Niveau 2 (Pour passer au niveau 2, il faut terminer le niveau 1)
{
    int x, y;
	Set_PointVie(3);
	list<Fantome>::iterator itFantome;
	Set_Niveau(HARD);
         const char terrain_defaut[17][21] = {
            "....................",
            "....................",
            "####################",
            "#...../..##.....#/.#",
            "#.###....##...###..#",
            "#...#....##.../....#",
            "#...../...//.......|",
            "#.//...##...#..//..#",
            "#../#*.#....#/..//.#",
            "#####..#....#...####",
            "#......#..###...#..#",
            "#......#....#.../..#",
            "#..//....*.....*...#",
            "#...//../..../../..#",
            "#....##......##..//#",
            "#.../.#.../..#./../#",
            "####################"
        };

        largeur = 20;
        hauteur = 17;

        terrain = new Case[largeur*hauteur];

        for(y=0;y<hauteur;++y)
            for(x=0;x<largeur;++x)
                if (terrain_defaut[y][x]=='#')
                    terrain[y*largeur+x] = MUR;
                else if (terrain_defaut[y][x]=='.')
                    terrain[y*largeur+x] = VIDE;
                else if (terrain_defaut[y][x]=='/')
                    terrain[y*largeur+x] = GOMME;
                else if (terrain_defaut[y][x]=='|')
                    terrain[y*largeur+x] = PORTE;
                else terrain[y*largeur+x] = BONUS;

        fantomes.resize(7);

        for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
        {
            do {
                x = rand()%largeur;
                y = rand()%hauteur + 3;
            } while (terrain[y*largeur+x]!=VIDE);

            itFantome->posX = x;
            itFantome->posY = y;
            itFantome->dir = (Direction)(rand()%4);
        }

        do {
            x = rand()%largeur;
            y = rand()%hauteur;
        } while (terrain[y*largeur+x]!=VIDE);

        posPacmanX = 3;         // Position initial du PacMan pour l'instant
        posPacmanY = 3;

        return true;
}
void Jeu::evolue()
{
    int testX, testY;
	list<Fantome>::iterator itFantome;
	Set_NBGommes();

    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};

    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        testX = itFantome->posX + depX[itFantome->dir];
        testY = itFantome->posY + depY[itFantome->dir];

        if ((terrain[testY*largeur+testX] !=MUR) && (terrain[testY*largeur+testX] !=PORTE))
        {
            itFantome->posX = testX;
            itFantome->posY = testY;
        }
        else
            // Changement de direction
            itFantome->dir = (Direction)(rand()%4);
    }
}

int Jeu::getNbCasesX() const
{
    return largeur;
}

int Jeu::getNbCasesY() const
{
    return hauteur;
}

int Jeu::getPacmanX() const
{
    return posPacmanX;
}

int Jeu::getPacmanY() const
{
    return posPacmanY;
}

Case Jeu::getCase(int x, int y) const
{
    assert(x>=0 && x<largeur && y>=0 && y<hauteur);
    return terrain[y*largeur+x];
}

bool Jeu::posValide(int x, int y) const
{
    return (x>=0 && x<largeur && y>=0 && y<hauteur && terrain[y*largeur+x] !=MUR);
}

bool Jeu::deplacePacman(Direction dir)
{
    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};
    int testX, testY;

    testX = posPacmanX + depX[dir];
    testY = posPacmanY + depY[dir];

    if (posValide(testX, testY))
    {
        posPacmanX = testX;
        posPacmanY = testY;
        return true;
    }
    else
        return false;
}

void Jeu ::Ajouter_Fantome()
{
    Fantome f;
    f.posX = 10;
    f.posY = 10;
    f.dir =  (Direction)(rand()%4);
    fantomes.push_back(f);
}

void Jeu ::Supprimer_Fantome()
{
    fantomes.pop_back();
}

bool Jeu :: Verifier_collisions()
{
    list<Fantome>::iterator itFantome;
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        if ((getPacmanX() == itFantome->posX) && (getPacmanY() == itFantome->posY))
        {
            return true;
        }
    }
    return false;
}

void Jeu::Handle_collisions()
{
    if (Verifier_collisions() == true)
    {
        // retourner au milieu du jeu
        posPacmanX = 10;
        posPacmanY = 10;
        Set_PointVie(Get_PointVie()-1);
        cout<<"Vous avez perdu un vie"<<endl;
        cout<<PointVie<<endl;
    }
}
void Jeu::Set_PointVie(const int a)
{
    PointVie = a;
}

int Jeu::Get_PointVie() const
{
    return PointVie;
}

void Jeu::Set_NBGommes()
{
    int x, y;
    int compte = -1;
    for(y=0;y<hauteur;++y)
    {
		for(x=0;x<largeur;++x)
        {
            if (getCase(x,y) == GOMME)
                compte = compte + 1;
        }
    }
    NbGomme = compte;
}
int Jeu::Get_NBGommes() const
{
    return NbGomme;
}

void Jeu :: Handle_Gomme()
{
    if (getCase(getPacmanX(),getPacmanY()) == GOMME)
    {
        terrain[getPacmanY()*largeur+getPacmanX()] = VIDE;
        cout<< "Nombre de Gommes reste :"<<Get_NBGommes()<<endl;
    }
}

void Jeu :: Handle_Bonus()
{
    if (getCase(getPacmanX(),getPacmanY()) == BONUS)
    {
        terrain[getPacmanY()*largeur+getPacmanX()] = VIDE;
        PointVie = PointVie + 1;
    }
}

 bool Jeu::Verifier_Gagner()
 {
     if (Get_NBGommes() == -1)
     {
         if (getCase(getPacmanX(),getPacmanY()) == PORTE)
            return true;
     }
     else
        return false;
 }

 void Jeu::Set_Niveau(NiveauDif n)
 {
     niveau = n;
 }

 NiveauDif Jeu::Get_Niveau()
 {
     return niveau;
 }


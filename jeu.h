#ifndef JEU_H
#define JEU_H

#include <list>
using namespace std;

typedef enum {VIDE, MUR, GOMME, PORTE, BONUS} Case;
typedef enum {GAUCHE, DROITE, HAUT, BAS} Direction;
typedef enum {EASY,HARD} NiveauDif;

class Jeu;

class Fantome
{
    friend class Jeu;

  protected:
    int posX, posY;
    Direction dir;

  public:
    Fantome();
    int getPosX() const;
    int getPosY() const;
};

class Jeu
{
  protected:
    Case *terrain;
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
    int posPacmanX, posPacmanY;
    int NbGomme;          // Nombre de gommes dans la terrain
    int PointVie;         // Point de Vie
    NiveauDif niveau;     // Niveau du jeu

  public:
    list<Fantome> fantomes;

  public:
    Jeu();
    Jeu(const Jeu &)=delete;
    ~Jeu();

    Jeu &operator=(const Jeu &)=delete;

    bool init();
    void evolue();
    bool niveau2();      // Augmenter le niveau du jeu

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;
    int getNbCasesY() const;

    // Set Largeur et Hauteur du jeu
    void set_Largeur(const int);
    void set_Hauteur(const int);

    // Retourne la position du Pacman
    int getPacmanX() const;
    int getPacmanY() const;

    // Retourne la case à une position donnée
    Case getCase(int, int) const;

    // Indique si la case à une position donnée existe et est vide
    // (Pacman ou les fantomes peuvent l'occuper)
    bool posValide(int, int) const;

    // Déplace Pacman dans une direction (si la case à atteindre est valide)
    bool deplacePacman(Direction);

    // Ajouter un fatome
    void Ajouter_Fantome();

    // Supprimer un fatome
    void Supprimer_Fantome();

    // Verifier les Collisions entre pacman et fantomes
    bool Verifier_collisions();

    // Collisions entre pacman et fantomes
    void Handle_collisions();

    // Get Point de Vie
    int Get_PointVie() const;

    // Set Point de Vie
    void Set_PointVie(const int);

    // Bonus point de Vie
    void Handle_Bonus();

    // Verifier Gagner
    bool Verifier_Gagner();

    // Set Nombre de gommes
    void Set_NBGommes();

    // Get Nombre de gommes;
    int Get_NBGommes() const;

    // Handle Gomme
    void Handle_Gomme();

    // Set niveau du jeu
    void Set_Niveau(NiveauDif);

    // Get niveau du jeu
    NiveauDif Get_Niveau();

};

#endif

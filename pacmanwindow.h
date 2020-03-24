#ifndef PACMANWINDOW_H
#define PACMANWINDOW_H

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.h"

class PacmanWindow : public QFrame
{
  friend class Fantome;
  protected:
    Jeu jeu;
    QPixmap pixmapPacman, pixmapFantome, pixmapMur, pixmapGomme, pixmapPorte, pixmapPointVie;

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=0);
    void handleTimer();
    void Ajouter_Button();
    void Supprimer_Button();
    void Handle_perdu_MSG();
    void Handle_Gagner_MSG();
    void Button_Rejouer();
    void Button_Sortir();
    void Niveau_Jeu();
    void Button_Easy();
    void Button_Hard();

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

};

class PacmanButton : public QPushButton
{
  protected:
    void keyPressEvent(QKeyEvent *);
  public:
    PacmanButton(const QString &,QWidget *pParent=0);
};

#endif

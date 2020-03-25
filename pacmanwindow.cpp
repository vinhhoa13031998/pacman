#include <iostream>
#include "pacmanwindow.h"

using namespace std;
PacmanButton::PacmanButton(const QString& str, QWidget *pParent):QPushButton(str, pParent){}

void PacmanButton::keyPressEvent(QKeyEvent *e)
{
    if(parent() != NULL)
        QCoreApplication::sendEvent(parent(), e);
}

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    if (pixmapPacman.load("./data/pacman.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacman.png"<<endl;
        exit(-1);
    }

    if (pixmapFantome.load("./data/fantome.bmp")==false)
    {
        cout<<"Impossible d'ouvrir fantome.bmp"<<endl;
        exit(-1);
    }

    if (pixmapMur.load("./data/mur.bmp")==false)
    {
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }

    if (pixmapGomme.load("./data/gomme.bmp")==false)
    {
        cout<<"Impossible d'ouvrir gomme.bmp"<<endl;
        exit(-1);
    }

    if (pixmapPorte.load("./data/porte.bmp")==false)
    {
        cout<<"Impossible d'ouvrir porte.bmp"<<endl;
        exit(-1);
    }
    if (pixmapPointVie.load("./data/coeur.bmp")==false)
    {
        cout<<"Impossible d'ouvrir coeur.bmp"<<endl;
        exit(-1);
    }
    if (pixmapBonus.load("./data/bonus.bmp")==false)
    {
        cout<<"Impossible d'ouvrir bonus.bmp"<<endl;
        exit(-1);
    }

    // Ajouter le button "AJOUTER FANTOME"
    PacmanButton *btA = new PacmanButton("Ajouter fantome",this);
    btA->setGeometry(10,10,130,40);
    connect(btA,PacmanButton::clicked, this, PacmanWindow::Ajouter_Button);

    // Ajouter le button "Supprimer FANTOME"
    PacmanButton *btS = new PacmanButton("Supprimer fantome",this);
    connect(btS,PacmanButton::clicked, this, PacmanWindow::Supprimer_Button);
    btS->setGeometry(150,10,130,40);

    jeu.init();

    QTimer *timer = new QTimer(this);
    connect(timer, QTimer::timeout, this, PacmanWindow::handleTimer);
    timer->start(120);

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*hauteurCase);
}

void PacmanWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    list<Fantome>::const_iterator itFantome;
    int x, y;

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    // Dessine les cases
    for (y=0;y<jeu.getNbCasesY();y++)
        for (x=0;x<jeu.getNbCasesX();x++)
			if (jeu.getCase(x,y)==MUR)
                painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapMur);
            else if(jeu.getCase(x,y) == GOMME)
                painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapGomme);
            else if(jeu.getCase(x,y) == PORTE)
                painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapPorte);
            else if(jeu.getCase(x,y) == BONUS)
                painter.drawPixmap(x*largeurCase, y*hauteurCase, pixmapBonus);
    // Dessine les fantomes
    for (itFantome=jeu.fantomes.begin(); itFantome!=jeu.fantomes.end(); itFantome++)
        {painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantome);}

	// Dessine Pacman
	painter.drawPixmap(jeu.getPacmanX()*largeurCase, jeu.getPacmanY()*hauteurCase, pixmapPacman);

	// Dessine PointdeVie
	for (int i =1; i <= jeu.Get_PointVie();i++)
    {
        painter.drawPixmap(350+(i-1)*40,10,40,35,pixmapPointVie);
    }

}

void PacmanWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Left)
        jeu.deplacePacman(GAUCHE);
    else if (event->key()==Qt::Key_Right)
        jeu.deplacePacman(DROITE);
    else if (event->key()==Qt::Key_Up)
        jeu.deplacePacman(HAUT);
    else if (event->key()==Qt::Key_Down)
        jeu.deplacePacman(BAS);
    update();
}

void PacmanWindow::handleTimer()
{
    jeu.evolue();
    Handle_perdu_MSG();
    Handle_Gagner_MSG();
    Handle_Continuer_MSG();
    jeu.Handle_collisions();
    jeu.Handle_Gomme();
    jeu.Handle_Bonus();
    update();
}

//// Fonction ajouter fantome
void PacmanWindow :: Ajouter_Button()
{
    jeu.Ajouter_Fantome();
}

//// Fonction supprimer fantome
void PacmanWindow :: Supprimer_Button()
{
    jeu.Supprimer_Fantome();
}

//// Button rejouer
void PacmanWindow:: Button_Rejouer()
{
    jeu.init();
}

//// Button Sortir
void PacmanWindow:: Button_Sortir()
{
    this->close();
}

//// Button Hard
void PacmanWindow:: Button_Continuer()
{
    jeu.niveau2();
}

//// Message perdu
void PacmanWindow :: Handle_perdu_MSG()
{
    QString ABC;
    string text;
    QMessageBox msg;
    text = "Vinh Hoa";
    ABC = QString::fromLocal8Bit(text.c_str());
    if(jeu.Get_PointVie() == 0)
    {
        msg.setText("Vous avez perdu, essayez-vous ?");
        msg.setInformativeText(ABC);

        QPushButton *btn_rejouer = msg.addButton("Rejouer", QMessageBox::AcceptRole);
        connect(btn_rejouer, QPushButton::clicked, this, PacmanWindow::Button_Rejouer);

        QPushButton *btn_sortir = msg.addButton("Sortir", QMessageBox::AcceptRole);
        connect(btn_sortir, QPushButton::clicked, this, PacmanWindow::Button_Sortir);

        msg.exec();
    }
}
void PacmanWindow :: Handle_Gagner_MSG()
{
    QMessageBox msg;
    if(jeu.Verifier_Gagner() == true && jeu.Get_Niveau() == EASY)
    {
        msg.setText("Bien Joue, Continuez 'HARD-MODE' ?");

        QPushButton *btn_continuer = msg.addButton("OK", QMessageBox::AcceptRole);
        connect(btn_continuer, QPushButton::clicked, this, PacmanWindow::Button_Continuer);

        QPushButton *btn_sortir = msg.addButton("Sortir", QMessageBox::AcceptRole);
        connect(btn_sortir, QPushButton::clicked, this, PacmanWindow::Button_Sortir);

        msg.exec();
    }
}
void PacmanWindow :: Handle_Continuer_MSG()
{
    QMessageBox msg;
    if(jeu.Verifier_Gagner() == true && jeu.Get_Niveau() == HARD)
    {
        msg.setText("Felicitation, Vous avez gagner ");

        QPushButton *btn_rejouer = msg.addButton("Rejouer", QMessageBox::AcceptRole);
        connect(btn_rejouer, QPushButton::clicked, this, PacmanWindow::Button_Rejouer);

        QPushButton *btn_sortir = msg.addButton("Sortir", QMessageBox::AcceptRole);
        connect(btn_sortir, QPushButton::clicked, this, PacmanWindow::Button_Sortir);

        msg.exec();
    }
}


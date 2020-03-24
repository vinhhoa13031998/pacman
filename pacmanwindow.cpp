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

    // Ajouter le button "AJOUTER FANTOME"
    PacmanButton *btA = new PacmanButton("Ajouter fantome",this);
    btA->setGeometry(10,10,130,40);
    connect(btA,PacmanButton::clicked, this, PacmanWindow::Ajouter_Button);

    // Ajouter le button "Supprimer FANTOME"
    PacmanButton *btS = new PacmanButton("Supprimer fantome",this);
    connect(btS,PacmanButton::clicked, this, PacmanWindow::Supprimer_Button);
    btS->setGeometry(200,10,130,40);

    Niveau_Jeu();

    jeu.init();

    QTimer *timer = new QTimer(this);
    connect(timer, QTimer::timeout, this, PacmanWindow::handleTimer);
    if (jeu.Get_Niveau() == EASY)
        {timer->start(120);}
    else {timer->start(80);}

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

    // Dessine les fantomes
    for (itFantome=jeu.fantomes.begin(); itFantome!=jeu.fantomes.end(); itFantome++)
        {painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase, pixmapFantome);}

	// Dessine Pacman
	painter.drawPixmap(jeu.getPacmanX()*largeurCase, jeu.getPacmanY()*hauteurCase, pixmapPacman);

	// Dessine PointdeVie
	for (int i =1; i <= jeu.Get_PointVie();i++)
    {
        painter.drawPixmap(500+(i-1)*40,10,40,35,pixmapPointVie);
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
    jeu.Handle_collisions();
    jeu.Handle_Gomme();
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
void PacmanWindow:: Button_Hard()
{
    jeu.Set_Niveau(HARD);
}

//// Button Easy
void PacmanWindow::Button_Easy()
{
    jeu.Set_Niveau(EASY);
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
    QString ABC;
    string text;
    QMessageBox msg;
    text = "Vinh Hoa";
    ABC = QString::fromLocal8Bit(text.c_str());
    if(jeu.Verifier_Gagner() == true)
    {
        msg.setText("Vous avez gagner, essayez-vous ?");
        msg.setInformativeText(ABC);

        QPushButton *btn_rejouer = msg.addButton("Rejouer", QMessageBox::AcceptRole);
        connect(btn_rejouer, QPushButton::clicked, this, PacmanWindow::Button_Rejouer);

        QPushButton *btn_sortir = msg.addButton("Sortir", QMessageBox::AcceptRole);
        connect(btn_sortir, QPushButton::clicked, this, PacmanWindow::Button_Sortir);

        msg.exec();
    }
}

void PacmanWindow :: Niveau_Jeu()
{
    QMessageBox msg;
    msg.setText(" Choisissez-vous le niveau du jeu");

    QPushButton *btn_easy = msg.addButton("Easy", QMessageBox::AcceptRole);
    connect(btn_easy, QPushButton::clicked, this, PacmanWindow::Button_Easy);

    QPushButton *btn_hard = msg.addButton("Hard", QMessageBox::AcceptRole);
    connect(btn_hard, QPushButton::clicked, this, PacmanWindow::Button_Hard);

    msg.exec();
}

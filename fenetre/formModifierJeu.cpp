#include <qDebug>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QPixMap>

#include "fenetre/formModifierJeu.h"
#include "ui_formModifierJeu.h"

#include "BD/collections.h"

formModifierJeu::formModifierJeu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formModifierJeu)
{
    ui->setupUi(this);
    qDebug() << "jeu:";
    jeuSelect->afficher();
    ui->nomJeu->setText(jeuSelect->getNom());
    ui->theme->setText(jeuSelect->getTheme());
}

formModifierJeu::~formModifierJeu()
{
    delete ui;
}

void formModifierJeu::on_supprimerJeuButton_clicked()
{
    // yes no
    deleteJeu(jeuSelect);
    emit listJeuChanged();
    this->close();
}

void formModifierJeu::on_modifierJeuButton_clicked()
{
    jeuSelect->setNom(ui->nomJeu->text());
    QString fileName=ui->theme->text();
    jeuSelect->setTheme(fileName);
    emit listJeuChanged();
    this->close();
}

void formModifierJeu::on_parcourirButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Images (*.png *.bmp *.jpg)"));
    ui->theme->setText(fileName);
    ui->themeImage->setPixmap(fileName);
    ui->themeImage->adjustSize();
    ui->themeImage->setScaledContents(true);
}

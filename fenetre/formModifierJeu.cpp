#include <qDebug>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QPixMap>
#include <QMessageBox>

#include "fenetre/formModifierJeu.h"
#include "ui_formModifierJeu.h"
#include "fenetre/formNouveauAttribut.h"
#include "fenetre/formModifierAttribut.h"

#include "BD/collections.h"

#define ATTRIBUT 0
#define VALEUR 1
#define COMPETENCE 2
#define INFORMATION 3

formModifierJeu::formModifierJeu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formModifierJeu)
{
    ui->setupUi(this);
    // ajout des informations de l'objet sur le formulaire
    ui->nomJeu->setText(listJeu[jeuSelect].getNom());
    ui->theme->setText(listJeu[jeuSelect].getTheme());
    ui->listAttributDisp->clear();
    ui->listAttributSelect->clear();
    ui->listCompetenceDisp->clear();
    ui->listCompetenceSelect->clear();
    ui->listValeurDisp->clear();
    ui->listValeurSelect->clear();
    ui->listInformationDisp->clear();
    ui->listInformationSelect->clear();


    remplirListAttribut();


    // ajout des attributs séléctionnés de l'objet sur le formulaire
    QVector<Attribut> listAtt = listJeu[jeuSelect].getListAttribut();
    for(int i=0;i<listAtt.size();i++)
    {
        Attribut a=listAtt[i];
        QListWidgetItem *newItem = new QListWidgetItem;
        // on met le titre de l'objet comme donnée
        newItem->setData(Qt::UserRole,a.getTitre());
        // le nom de l'objet comme text affiché
        newItem->setText(a.getNom());

        if(a.getType()==ATTRIBUT) ui->listAttributSelect->addItem(newItem);
        else if (a.getType()==VALEUR) ui->listValeurSelect->addItem(newItem);
        else if (a.getType()==COMPETENCE) ui->listCompetenceSelect->addItem(newItem);
        else if (a.getType()==INFORMATION) ui->listInformationSelect->addItem(newItem);
        //qDebug() << "taille" << ui->listInformationSelect->count();
    }
    Load();
}

void formModifierJeu::Load()
{
    // ajout des attributs non séléctionnés dans la liste des attributs dispnibles
    bool test=true;
    for(int i=0;i<listAttribut.size();i++)
    {
        //listAttribut[i].afficher();
        test=true;
        QString titre;
        if(listAttribut[i].getType()==ATTRIBUT)
        {
            for(int j=0;j<ui->listAttributSelect->count();j++)
            {
                titre=ui->listAttributSelect->item(j)->data(Qt::UserRole).toString();
                // teste si l'attribut regardé de la liste est déja selectionné
                if(titre==listAttribut[i].getTitre())
                {
                    test=false;
                    break;
                }
            }
        }
        else if(listAttribut[i].getType()==VALEUR)
        {
            for(int j=0;j<ui->listValeurSelect->count();j++)
            {
                titre=ui->listValeurSelect->item(j)->data(Qt::UserRole).toString();
                // teste si l'attribut regardé de la liste est déja selectionné
                if(titre==listAttribut[i].getTitre())
                {
                    test=false;
                    break;
                }
            }
        }
        else if(listAttribut[i].getType()==COMPETENCE)
        {
            for(int j=0;j<ui->listCompetenceSelect->count();j++)
            {
                titre=ui->listCompetenceSelect->item(j)->data(Qt::UserRole).toString();
                // teste si l'attribut regardé de la liste est déja selectionné
                if(titre==listAttribut[i].getTitre())
                {
                    test=false;
                    break;
                }
            }
        }
        else if(listAttribut[i].getType()==INFORMATION)
        {
            for(int j=0;j<ui->listInformationSelect->count();j++)
            {
                titre=ui->listInformationSelect->item(j)->data(Qt::UserRole).toString();
                // teste si l'attribut regardé de la liste est déja selectionné
                if(titre==listAttribut[i].getTitre())
                {
                    test=false;
                    break;
                }
            }
        }
        if(test) // si l'attribut regardé n'est pas déja selectionné on l'ajoute dans la liste des disponibles
        {
            QListWidgetItem *newItem = new QListWidgetItem;
            // on met le titre de l'objet comme donnée
            newItem->setData(Qt::UserRole,listAttribut[i].getTitre());
            // le nom de l'objet comme text affiché
            newItem->setText(listAttribut[i].getNom());
            if(listAttribut[i].getType()==ATTRIBUT)ui->listAttributDisp->addItem(newItem);
            else if(listAttribut[i].getType()==VALEUR)ui->listValeurDisp->addItem(newItem);
            else if(listAttribut[i].getType()==COMPETENCE)ui->listCompetenceDisp->addItem(newItem);
            else if(listAttribut[i].getType()==INFORMATION)ui->listInformationDisp->addItem(newItem);
        }
    }
    //qDebug() << "fin";
    QPixmap p(listJeu[jeuSelect].getTheme());
    int w = ui->themeImage->width();
    int h = ui->themeImage->height();
    ui->themeImage->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));
}

formModifierJeu::~formModifierJeu()
{
    delete ui;
}

void formModifierJeu::changementAttribut()
{
    ui->listAttributDisp->clear();
    ui->listCompetenceDisp->clear();
    ui->listValeurDisp->clear();
    ui->listInformationDisp->clear();

    remplirListAttribut();

    Load();
}

void formModifierJeu::on_supprimerJeuButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Suppression", "Etes vous sur de vouloir supprimer ce jeu ?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      deleteJeu(jeuSelect);
          emit listJeuChanged();
          this->close();
    }
}

void formModifierJeu::on_modifierJeuButton_clicked()
{
    // on récupère les données
    Jeu prec=listJeu[jeuSelect];
    QString fileName=ui->theme->text();
    if(listJeu[jeuSelect].getTheme()!=fileName) listJeu[jeuSelect].setTheme(fileName);
    // on liste les attributs selectionnés
    QVector<Attribut> list;
    for(int i=0;i<ui->listAttributSelect->count();i++)
    {
        QString titre=ui->listAttributSelect->item(i)->data(Qt::UserRole).toString();
        Attribut a(titre);
        list.append(a);
    }
    for(int i=0;i<ui->listValeurSelect->count();i++)
    {
        QString titre=ui->listValeurSelect->item(i)->data(Qt::UserRole).toString();
        Attribut a(titre);
        list.append(a);
    }
    for(int i=0;i<ui->listCompetenceSelect->count();i++)
    {
        QString titre=ui->listCompetenceSelect->item(i)->data(Qt::UserRole).toString();
        Attribut a(titre);
        list.append(a);
    }
    for(int i=0;i<ui->listInformationSelect->count();i++)
    {
        QString titre=ui->listInformationSelect->item(i)->data(Qt::UserRole).toString();
        Attribut a(titre);
        list.append(a);
    }
    listJeu[jeuSelect].setListAttribut(list);
    remplirListCampagne();
    remplirListPlaylist();
    remplirListPartie();
    remplirListPersonnage();
    listJeu[jeuSelect].setNom(ui->nomJeu->text());
    // on signale une modification
    //prec.afficher();
    //listJeu[jeuSelect].afficher();
    updateJeu(prec,listJeu[jeuSelect]);
    emit listJeuChanged();
    // on ferme le formulaire
    this->close();
}

void formModifierJeu::on_parcourirButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Images (*.png *.bmp *.jpg)"));
    ui->theme->setText(fileName);

    QPixmap p(fileName);
    int w = ui->themeImage->width();
    int h = ui->themeImage->height();
    ui->themeImage->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));
}

void formModifierJeu::on_nouveauAttribut_clicked()
{
    formNouveauAttribut formNouveauAttribut;
    formNouveauAttribut.setModal(true);
    QObject::connect(&formNouveauAttribut, SIGNAL(listAttributChanged()),this, SLOT(changementAttribut()));
    formNouveauAttribut.exec();
}

void formModifierJeu::on_ajouterButton_clicked()
{
    if(ui->listAttributDisp->currentItem()!=NULL)
    {
        // on vérifie qu'un item est selectionné
        // on met l'attribut dispo seclectionné dans la liste des attributs select
        // et on le supprime de la liste dispo
        QListWidgetItem* item = ui->listAttributDisp->currentItem();
        QListWidgetItem *newItem = new QListWidgetItem;
        // on met le titre de l'objet comme donnée
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        // le nom de l'objet comme text affiché
        newItem->setText(item->text());
        ui->listAttributSelect->addItem(newItem);
        delete item;
    }
    else if(ui->listValeurDisp->currentItem()!=NULL)
    {
        QListWidgetItem* item = ui->listValeurDisp->currentItem();
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        newItem->setText(item->text());
        ui->listValeurSelect->addItem(newItem);
        delete item;
    }
    else if(ui->listCompetenceDisp->currentItem()!=NULL)
    {
        QListWidgetItem* item = ui->listCompetenceDisp->currentItem();
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        newItem->setText(item->text());
        ui->listCompetenceSelect->addItem(newItem);
        delete item;
    }
    else if(ui->listInformationDisp->currentItem()!=NULL)
    {
        QListWidgetItem* item = ui->listInformationDisp->currentItem();
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        newItem->setText(item->text());
        ui->listInformationSelect->addItem(newItem);
        delete item;
    }
}

void formModifierJeu::on_supprimerButton_clicked()
{
    // on vérifie qu'un item est selectionné
    if(ui->listAttributSelect->currentItem()!=NULL)
    {
        // on met l'attribut dispo seclectionné dans la liste des attributs select
        // et on le supprime de la liste dispo
        QListWidgetItem* item = ui->listAttributSelect->currentItem();
        QListWidgetItem *newItem = new QListWidgetItem;
        // on met le titre de l'objet comme donnée
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        // le nom de l'objet comme text affiché
        newItem->setText(item->text());
        ui->listAttributDisp->addItem(newItem);
        delete item;
    }
    else if(ui->listValeurSelect->currentItem()!=NULL)
    {
        QListWidgetItem* item = ui->listValeurSelect->currentItem();
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        newItem->setText(item->text());
        ui->listValeurDisp->addItem(newItem);
        delete item;
    }
    else if(ui->listCompetenceSelect->currentItem()!=NULL)
    {
        QListWidgetItem* item = ui->listCompetenceSelect->currentItem();
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        newItem->setText(item->text());
        ui->listCompetenceDisp->addItem(newItem);
        delete item;
    }
    else if(ui->listInformationSelect->currentItem()!=NULL)
    {
        QListWidgetItem* item = ui->listInformationSelect->currentItem();
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        newItem->setText(item->text());
        ui->listInformationDisp->addItem(newItem);
        delete item;
    }
}

void formModifierJeu::on_ajouterAttributButton_clicked()
{
    QListWidgetItem* item;
    QListWidgetItem *newItem = new QListWidgetItem;
    if(ui->listAttributDisp->currentItem()!=NULL)
    {
        // on vérifie qu'un item est selectionné
        // on met l'attribut dispo seclectionné dans la liste des attributs select
        // et on le supprime de la liste dispo
        item = ui->listAttributDisp->currentItem();
        // on met le titre de l'objet comme donnée
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        // le nom de l'objet comme text affiché
        newItem->setText(item->text());
        ui->listAttributSelect->addItem(newItem);
        delete item;
    }
}

void formModifierJeu::on_ajouterValeurButton_clicked()
{
    QListWidgetItem* item;
    QListWidgetItem *newItem = new QListWidgetItem;
    if(ui->listValeurDisp->currentItem()!=NULL)
    {
        item = ui->listValeurDisp->currentItem();
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        newItem->setText(item->text());
        ui->listValeurSelect->addItem(newItem);
        delete item;
    }
}

void formModifierJeu::on_supprimerValeurButton_clicked()
{
    QListWidgetItem* item;
    QListWidgetItem *newItem = new QListWidgetItem;
    // on vérifie qu'un item est selectionné
    if(ui->listValeurSelect->currentItem()!=NULL)
    {
        item = ui->listValeurSelect->currentItem();
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        newItem->setText(item->text());
        ui->listValeurDisp->addItem(newItem);
        delete item;
    }
}

void formModifierJeu::on_ajouterCompetenceButton_clicked()
{
    QListWidgetItem* item;
    QListWidgetItem *newItem = new QListWidgetItem;
    if(ui->listCompetenceDisp->currentItem()!=NULL)
    {
        item = ui->listCompetenceDisp->currentItem();
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        newItem->setText(item->text());
        ui->listCompetenceSelect->addItem(newItem);
        delete item;
    }
}

void formModifierJeu::on_supprimerCompetenceButton_clicked()
{
    QListWidgetItem* item;
    QListWidgetItem *newItem = new QListWidgetItem;
    if(ui->listCompetenceSelect->currentItem()!=NULL)
    {
        item = ui->listCompetenceSelect->currentItem();
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        newItem->setText(item->text());
        ui->listCompetenceDisp->addItem(newItem);
        delete item;
    }
}

void formModifierJeu::on_ajouterInformationButton_clicked()
{
    QListWidgetItem* item;
    QListWidgetItem *newItem = new QListWidgetItem;
    if(ui->listInformationDisp->currentItem()!=NULL)
    {
        item = ui->listInformationDisp->currentItem();
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        newItem->setText(item->text());
        ui->listInformationSelect->addItem(newItem);
        delete item;
    }
}

void formModifierJeu::on_supprimerInformationButton_clicked()
{
    QListWidgetItem* item;
    QListWidgetItem *newItem = new QListWidgetItem;

    if(ui->listInformationSelect->currentItem()!=NULL)
    {
        item = ui->listInformationSelect->takeItem(ui->listInformationSelect->currentRow());//currentItem();
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        newItem->setText(item->text());
        ui->listInformationDisp->addItem(newItem);
        //if(item!=NULL)delete item;
        qDebug() << "test3";
    }
}

void formModifierJeu::on_supprimerAttributButton_clicked()
{
    QListWidgetItem* item;
    QListWidgetItem *newItem = new QListWidgetItem;
    if(ui->listAttributSelect->currentItem()!=NULL)
    {
        // on met l'attribut dispo seclectionné dans la liste des attributs select
        // et on le supprime de la liste dispo
        item = ui->listAttributSelect->currentItem();
        // on met le titre de l'objet comme donnée
        newItem->setData(Qt::UserRole,item->data(Qt::UserRole));
        // le nom de l'objet comme text affiché
        newItem->setText(item->text());
        ui->listAttributDisp->addItem(newItem);
        delete item;
    }
}

void formModifierJeu::on_modifierInformationButton_clicked()
{
    if(ui->listInformationDisp->currentItem()!=NULL)
    {
        attributSelect=chercheTitreAttribut(ui->listInformationDisp->currentItem()->data(Qt::UserRole).toString());
        formModifierAttribut formModifierAttribut;
        formModifierAttribut.setModal(true);
        QObject::connect(&formModifierAttribut, SIGNAL(listAttributChanged()),this, SLOT(changementAttribut()));
        formModifierAttribut.exec();
    }
}

void formModifierJeu::on_modifierCompetenceButton_clicked()
{
    if(ui->listCompetenceDisp->currentItem()!=NULL)
    {
        attributSelect=chercheTitreAttribut(ui->listCompetenceDisp->currentItem()->data(Qt::UserRole).toString());
        formModifierAttribut formModifierAttribut;
        formModifierAttribut.setModal(true);
        QObject::connect(&formModifierAttribut, SIGNAL(listAttributChanged()),this, SLOT(changementAttribut()));
        formModifierAttribut.exec();
    }
}

void formModifierJeu::on_modifierValeurButton_clicked()
{
    if(ui->listValeurDisp->currentItem()!=NULL)
    {
        attributSelect=chercheTitreAttribut(ui->listValeurDisp->currentItem()->data(Qt::UserRole).toString());
        formModifierAttribut formModifierAttribut;
        formModifierAttribut.setModal(true);
        QObject::connect(&formModifierAttribut, SIGNAL(listAttributChanged()),this, SLOT(changementAttribut()));
        formModifierAttribut.exec();
    }
}

void formModifierJeu::on_modifierAttributButton_clicked()
{
    if(ui->listAttributDisp->currentItem()!=NULL)
    {
        attributSelect=chercheTitreAttribut(ui->listAttributDisp->currentItem()->data(Qt::UserRole).toString());
        formModifierAttribut formModifierAttribut;
        formModifierAttribut.setModal(true);
        QObject::connect(&formModifierAttribut, SIGNAL(listAttributChanged()),this, SLOT(changementAttribut()));
        formModifierAttribut.exec();
    }
}

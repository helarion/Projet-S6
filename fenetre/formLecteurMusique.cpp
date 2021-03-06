#include "formLecteurMusique.h"
#include "ui_formLecteurMusique.h"

#include <QSound>
#include <QMediaPlayer.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QMediaPlaylist>
#include <random>
#include <QTest>

#include "BD/playlist.h"
#include "BD/jeu.h"
#include "BD/collections.h"

formLecteurMusique::formLecteurMusique(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formLecteurMusique)
{
    ui->setupUi(this);
    ui->listPlaylist->clear();
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist;
    playlist->setPlaybackMode(QMediaPlaylist::QMediaPlaylist::Random);
    connect(player,&QMediaPlayer::positionChanged,this, &formLecteurMusique::on_positionChanged);
    connect(player,&QMediaPlayer::positionChanged,this, &formLecteurMusique::on_DurationChanged);
    Load();
    connect(playlist,&QMediaPlaylist::currentMediaChanged,this,&formLecteurMusique::on_SongChanged);

    compteur=0;
}

formLecteurMusique::~formLecteurMusique()
{
    delete ui;
}

void formLecteurMusique::Load()
{
    Jeu j=listJeu[jeuSelect];
    ui->listPlaylist->clear();
    remplirListPlaylist();
    for(int i=0;i<listPlaylist.size();i++)
    {
        //listPlaylist[i].afficher();
        //qDebug() << "titre boucle" << listPlaylist[i].getTitreJeu();
        if(listPlaylist[i].getTitreJeu()==j.getTitre())
        {
            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setData(Qt::UserRole,i);
            // le nom de l'objet comme text affiché
            newItem->setText(listPlaylist[i].getNom());
            ui->listPlaylist->addItem(newItem);
        }
    }
}

void formLecteurMusique::on_PauseButton_clicked()
{
    player->pause();
}

void formLecteurMusique::on_PlayButton_clicked()
{
    player->play();
}

void formLecteurMusique::on_StopButton_clicked()
{
    player->stop();
    playlist->setCurrentIndex(0);
}

void formLecteurMusique::on_ProgressSlider_sliderMoved(int position)
{
    player->setPosition(position);
}

void formLecteurMusique::on_DurationChanged()
{
  ui->ProgressSlider->setMaximum(player->duration());
}

void formLecteurMusique::on_positionChanged(qint64 position)
{
    ui->ProgressSlider->setValue(position);
}

void formLecteurMusique::on_SongChanged()
{
    if(ui->RandomCheckBox->isChecked()){
        compteur++;
        if(compteur%2==0){
        int rand=(int)(qrand()%(playlist->mediaCount()));
        //qDebug() << rand;
        playlist->setCurrentIndex(rand);
        }
     }
}

void formLecteurMusique::on_verticalSlider_sliderMoved(int position)
{
    player->setVolume(position);
}

void formLecteurMusique::on_PreviousButton_clicked()
{
    playlist->previous();
    player->stop();
    player->play();
}

void formLecteurMusique::on_NextButton_clicked()
{
    playlist->next();
    player->stop();
    player->play();
}

void formLecteurMusique::on_listPlaylist_itemDoubleClicked(QListWidgetItem *item)
{
    player->stop();
    int index=item->data(Qt::UserRole).toInt();
    playlist->clear();
    QVector<QString> l=listPlaylist[index].getListe();
    for(int i=0;i<l.size();i++)
    {
        playlist->addMedia(QUrl(l[i]));
    }
    playlist->setPlaybackMode(QMediaPlaylist::QMediaPlaylist::Random);
    player->setMedia(playlist);
    player->play();
}

void formLecteurMusique::on_ajouterPlaylistButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this,tr("Selectionner un dossier :"));
    if(directory.isEmpty()){
        return;}

    Jeu j=listJeu[jeuSelect];

    QDir dir(directory);
    QStringList splitString = dir.absolutePath().split("/");
    QString nom = splitString[splitString.size()-1];
    QStringList files = dir.entryList(QStringList() << "*.mp3",QDir::Files);

    QVector<QString> liste;
    QDir d;
    QString path="data/Playlist/"+j.getTitre()+"_"+nom;
    d.mkdir(path);
    foreach(QString itm, files)
    {
        QFile f;
        f.copy(dir.path()+"/"+itm,path+"/"+itm);
        liste.append(path+"/"+itm);
    }

   // qDebug() << "gettitre de j" <<j.getTitre();

    Playlist p(nom,liste,j.getTitre());
    p.Save();
    Load();
}

void formLecteurMusique::on_supprimerPlaylistButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Suppression", "Etes vous sur de vouloir supprimer cette playlist ?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        playlistSelect=ui->listPlaylist->currentItem()->data(Qt::UserRole).toInt();
        deletePlaylist(playlistSelect);
        Load();
    }
}

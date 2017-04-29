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
#include "BD/collections.h"

formLecteurMusique::formLecteurMusique(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formLecteurMusique)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist;
    connect(player,&QMediaPlayer::positionChanged,this, &formLecteurMusique::on_positionChanged);
    connect(player,&QMediaPlayer::positionChanged,this, &formLecteurMusique::on_DurationChanged);

    Load();
    connect(playlist,&QMediaPlaylist::currentMediaChanged,this,&formLecteurMusique::on_SongChanged);
}

formLecteurMusique::~formLecteurMusique()
{
    delete ui;
}

void formLecteurMusique::Load()
{
    remplirListPlaylist();
    for(int i=0;i<listPlaylist.size();i++)
    {
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setData(Qt::UserRole,i);
        // le nom de l'objet comme text affiché
        newItem->setText(listPlaylist[i].getNom());
        ui->listPlaylist->addItem(newItem);
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
    //qDebug() << "chanson changée";

  if(ui->RandomCheckBox->isChecked()){
      int rand=(int)(qrand()%(playlist->mediaCount()));
      qDebug() << rand;
      QTest::qSleep(100);
      qDebug() << "pause";
      player->stop();
      playlist->setCurrentIndex(rand);
      player->play();

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
    qDebug() << "test";
    playlist->clear();
    QVector<QString> l=listPlaylist[index].getListe();
    for(int i=0;i<l.size();i++)
    {
        playlist->addMedia(QUrl(l[i]));
    }
    player->setMedia(playlist);
    player->play();
}

void formLecteurMusique::on_ajouterPlaylistButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this,tr("Selectionner un dossier :"));
    if(directory.isEmpty()){
        return;}

    QDir dir(directory);
    QStringList splitString = dir.absolutePath().split("/");
    QString nom = splitString[splitString.size()-1];
    QStringList files = dir.entryList(QStringList() << "*.mp3",QDir::Files);

    QVector<QString> liste;
    foreach(QString itm, files)
    {
        liste.append(dir.path()+"/"+itm);
    }
    Playlist p(nom,liste);
    p.Save();
}

#ifndef FORMMODIFIERJEU_H
#define FORMMODIFIERJEU_H

#include <QDialog>

namespace Ui {
class formModifierJeu;
}

class formModifierJeu : public QDialog
{
    Q_OBJECT

public:
    explicit formModifierJeu(QWidget *parent = 0);
    ~formModifierJeu();

private slots:
    void on_supprimerJeuButton_clicked();

    void on_modifierJeuButton_clicked();

    void on_parcourirButton_clicked();

    void on_nouveauAttribut_clicked();

    void changementAttribut();

    void on_ajouterButton_clicked();

    void on_supprimerButton_clicked();

    void on_ajouterAttributButton_clicked();

    void on_supprimerAttributButton_clicked();

    void on_ajouterValeurButton_clicked();

    void on_supprimerValeurButton_clicked();

    void on_ajouterCompetenceButton_clicked();

    void on_supprimerCompetenceButton_clicked();

    void on_ajouterInformationButton_clicked();

    void on_supprimerInformationButton_clicked();

    void on_modifierAttributButton_clicked();

    void on_modifierValeurButton_clicked();

    void on_modifierCompetenceButton_clicked();

    void on_modifierInformationButton_clicked();

signals:
    void listJeuChanged();

private:
    Ui::formModifierJeu *ui;

    void Load();
};

#endif // FORMMODIFIERJEU_H

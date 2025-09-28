#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "backend.h"
#include <QProgressBar>
#include <QMainWindow>
#include <bits/stdc++.h>
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QTextEdit>
#include <QRandomGenerator>
#include <QSpinBox>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setupLobby();
    }

private slots:
    void startGame() {
        QString name = nameInput->text();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a name for your hero!");
            return;
        }

        hero = new Hero(name.toStdString(), 100, 15);

        setupGameScreen();
    }
    void animateAttack(QLabel *attacker, bool is_hero) {
        if (!attacker) return;

        QPoint startPos = attacker->pos();
        QPoint endPos ;
        if(is_hero){
            endPos = startPos + QPoint(30, 0);
        }else{
            endPos = startPos + QPoint(-30, 0);
        }
        QPropertyAnimation *animation = new QPropertyAnimation(attacker, "pos");
        animation->setDuration(200);
        animation->setStartValue(startPos);
        animation->setEndValue(endPos);
        animation->setEasingCurve(QEasingCurve::OutQuad);

        connect(animation, &QPropertyAnimation::finished, [=]() {
            QPropertyAnimation *back = new QPropertyAnimation(attacker, "pos");
            back->setDuration(200);
            back->setStartValue(endPos);
            back->setEndValue(startPos);
            back->setEasingCurve(QEasingCurve::InQuad);
            back->start(QAbstractAnimation::DeleteWhenStopped);
        });

        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void spawnEnemy() {
        if (currentEnemy) {
            delete currentEnemy;
            currentEnemy = nullptr;
        }

        int enemyType = QRandomGenerator::global()->bounded(6);

        switch (enemyType) {
        case 0:
            currentEnemy = new GreenDragon(60,10);
            break;
        case 1:
            currentEnemy = new RedDragon(60,15);
            break;
        case 2:
            currentEnemy = new BlueDragon(50,20);
            break;
        case 3:
            currentEnemy = new MountainTroll(30, 10);
            break;
        case 4:
            currentEnemy = new IceTroll(70, 12);
            break;
        case 5:
            currentEnemy = new DesertTroll(90, 8);
            break;
        }

        currentEnemy->set_answer();

        updateGameScreen();
    }

    void submitAnswer() {
        if (!currentEnemy) return;
        int answer = answerInput->text().toInt();
        bool correct = currentEnemy->check_answer(answer);

        if (correct) {
            animateAttack(heroImageLabel,1);
            hero->attack(currentEnemy);
            logText->append("Your answer was correct! You attacked the enemy.");
            currentEnemy->set_answer();
        } else {
            animateAttack(enemyImageLabel,0);
            currentEnemy->attack(hero);
            logText->append("Your answer was wrong! The enemy attacked you.");
        }
        if (!currentEnemy->is_live()) {
            logText->append("You defeated the enemy!");
            hero->gain_exp(25);
            spawnEnemy();
        }
        if (!hero->is_live()) {
            logText->append("You have been defeated! Game over.");
            QMessageBox::information(this, "Game Over", "You have been defeated!");
            resetGame();
        }

        updateGameScreen();
    }
private:

    void setupLobby() {
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        QLabel *titleLabel = new QLabel("Hero Adventure Game", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        QFont titleFont = titleLabel->font();
        titleFont.setPointSize(16);
        titleLabel->setFont(titleFont);

        QLabel *nameLabel = new QLabel("Enter your hero's name:", this);
        nameInput = new QLineEdit(this);

        QPushButton *startButton = new QPushButton("Start Game", this);
        connect(nameInput, &QLineEdit::returnPressed, this, &GameWindow::startGame);
        connect(startButton, &QPushButton::clicked, this, &GameWindow::startGame);

        layout->addWidget(titleLabel);
        layout->addWidget(nameLabel);
        layout->addWidget(nameInput);
        layout->addWidget(startButton);

        setCentralWidget(centralWidget);
        resize(400, 300);
    }
    void setupGameScreen() {
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        QHBoxLayout *heroLayout = new QHBoxLayout();
        QVBoxLayout *heroInfoLayout = new QVBoxLayout();
        heroNameLabel = new QLabel(this);
        heroHealthBar = new QProgressBar(this);
        heroHealthBar->setRange(0, hero->get_max_health());
        heroHealthBar->setValue(hero->get_hp());
        heroHealthBar->setTextVisible(1);
        heroHealthBar->setFixedSize(120, 15);

        heroLevelLabel = new QLabel(this);
        heroExpLabel = new QLabel(this);
        heroDamageLabel = new QLabel(this);


        heroInfoLayout->addWidget(heroNameLabel);
        heroInfoLayout->addWidget(heroHealthBar);
        heroLayout->addLayout(heroInfoLayout);
        heroLayout->addWidget(heroDamageLabel);
        heroLayout->addWidget(heroLevelLabel);
        heroLayout->addWidget(heroExpLabel);

        enemyTypeLabel = new QLabel(this);
        enemyHealthBar = new QProgressBar(this);
        enemyHealthBar->setRange(0, 100);
        enemyHealthBar->setValue(100);
        enemyHealthBar->setTextVisible(1);
        enemyHealthBar->setFixedSize(120, 15);
        enemyQuestionLabel = new QLabel(this);
        QVBoxLayout *enemyInfoLayout = new QVBoxLayout();
        enemyInfoLayout->addWidget(enemyTypeLabel);
        enemyInfoLayout->addWidget(enemyHealthBar);

        QHBoxLayout *enemyLayout = new QHBoxLayout();
        enemyLayout->addLayout(enemyInfoLayout);
        enemyLayout->addWidget(enemyQuestionLabel);

        QHBoxLayout *answerLayout = new QHBoxLayout();
        QLabel *answerPrompt = new QLabel("Your answer:", this);
        answerInput = new QLineEdit(this);
        QPushButton *submitButton = new QPushButton("Submit", this);

        connect(submitButton, &QPushButton::clicked, this, &GameWindow::submitAnswer);
        connect(answerInput, &QLineEdit::returnPressed, this, &GameWindow::submitAnswer);

        answerLayout->addWidget(answerPrompt);
        answerLayout->addWidget(answerInput);
        answerLayout->addWidget(submitButton);

        logText = new QTextEdit(this);
        logText->setReadOnly(true);

        QHBoxLayout *imagesLayout = new QHBoxLayout();
        heroImageLabel = new QLabel(this);
        heroImageLabel->setFixedSize(300, 300);

        enemyImageLabel = new QLabel(this);
        enemyImageLabel->setFixedSize(300, 300);
        heroImageLabel->setStyleSheet("background: transparent;");
        enemyImageLabel->setStyleSheet("background: transparent;");

        imagesLayout->addStretch();
        imagesLayout->addWidget(heroImageLabel);
        imagesLayout->addStretch();
        imagesLayout->addWidget(enemyImageLabel);
        imagesLayout->addStretch();

        mainLayout->addLayout(heroLayout);
        mainLayout->addLayout(enemyLayout);
        mainLayout->addLayout(answerLayout);
        mainLayout->addWidget(logText);
        mainLayout->addLayout(imagesLayout);

        setCentralWidget(centralWidget);
        this->setStyleSheet("background-color: #f1e8d7;");
        resize(600, 600);

        spawnEnemy();
    }




    void updateGameScreen() {
        if (!hero) return;

        heroNameLabel->setText(QString("Name: %1").arg(QString::fromStdString(hero->get_name())));
        heroLevelLabel->setText(QString("Level: %1").arg(hero->get_level()));
        heroExpLabel->setText(QString("EXP: %1/100").arg(hero->get_exp()));
        heroHealthBar->setMaximum(hero->get_max_health());
        heroHealthBar->setValue(hero->is_live() ? hero->get_hp() : 0);

        heroDamageLabel->setText(QString("Damage: %1").arg(hero->get_damage()));

        QPixmap heroPix(":/images/hero.png");
        heroImageLabel->setPixmap(heroPix.scaled(heroImageLabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

        if (currentEnemy) {
            QString enemyType;
            QString imagePath;

            if (auto dragon = dynamic_cast<EnemiesDragon*>(currentEnemy)) {
                enemyType = dragon->get_type();
                if (enemyType == "RedDragon") imagePath = ":/images/red_dragon.png";
                else if (enemyType == "BlueDragon") imagePath = ":/images/blue_dragon.png";
                else imagePath = ":/images/green_dragon.png";
            } else if (auto troll = dynamic_cast<Troll*>(currentEnemy)) {
                enemyType = troll->get_type();
                if (enemyType == "MountainTroll") imagePath = ":/images/mountain_troll.png";
                else if (enemyType == "IceTroll") imagePath = ":/images/ice_troll.png";
                else if (enemyType == "DesertTroll") imagePath = ":/images/desert_troll.png";
            }

            enemyTypeLabel->setText(QString("Enemy: %1").arg(enemyType));
            enemyQuestionLabel->setText(QString::fromStdString(currentEnemy->question()));
            enemyHealthBar->setMaximum(currentEnemy->get_max_health());
            enemyHealthBar->setValue(currentEnemy->is_live() ? currentEnemy->get_hp() : 0);
            if (!imagePath.isEmpty()) {
                QPixmap enemyPix(imagePath);
                enemyImageLabel->setPixmap(enemyPix.scaled(enemyImageLabel->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    }



    void resetGame() {
        delete hero;
        hero = nullptr;

        if (currentEnemy) {
            delete currentEnemy;
            currentEnemy = nullptr;
        }
        heroImageLabel = nullptr;
        enemyImageLabel = nullptr;
        setupLobby();
    }

    QLineEdit *nameInput;
    QLabel *heroNameLabel;
    QProgressBar *heroHealthBar;
    QLabel *heroLevelLabel;
    QLabel *heroExpLabel;
    QLabel *enemyTypeLabel;
    QProgressBar *enemyHealthBar;
    QLabel *enemyQuestionLabel;
    QLabel *heroDamageLabel;
    QLabel *heroImageLabel = nullptr;
    QLabel *enemyImageLabel = nullptr;
    QLineEdit *answerInput;
    QTextEdit *logText;

    Hero *hero = nullptr;
    Enemies *currentEnemy = nullptr;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif

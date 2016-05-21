#include "competitionwindow.h"
#include "ui_competitionwindow.h"
#include "QDebug"
#include "Exceptions/wrongid.h"
#include "qdebug.h"

CompetitionWindow::CompetitionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CompetitionWindow),
    comp(new Competition),
    isGenerated(false)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    connect(ui->btn_GS16_GroupA_Save, SIGNAL(clicked(bool)), this, SLOT(saveResults()));
    connect(ui->btn_GS16_GroupB_Save, SIGNAL(clicked(bool)), this, SLOT(saveResults()));
    connect(ui->btn_GS16_GroupC_Save, SIGNAL(clicked(bool)), this, SLOT(saveResults()));
    connect(ui->btn_GS16_GroupD_Save, SIGNAL(clicked(bool)), this, SLOT(saveResults()));
    connect(ui->btn_CG4_Create, SIGNAL(clicked(bool)), this, SLOT(createGroup()));
    connect(ui->btn_CG8_Create, SIGNAL(clicked(bool)), this, SLOT(createGroup()));
    connect(ui->btn_CG4_Generate, SIGNAL(clicked(bool)), this, SLOT(generateGroups()));
    connect(ui->btn_CG8_Generate, SIGNAL(clicked(bool)), this, SLOT(generateGroups()));
    connect(ui->btn_CG4_Start, SIGNAL(clicked(bool)), this, SLOT(startGroupStage()));
    connect(ui->btn_CG8_Start, SIGNAL(clicked(bool)), this, SLOT(startGroupStage()));
}

CompetitionWindow::~CompetitionWindow()
{
    delete ui;
}

void CompetitionWindow::setSettings(QString title, QList<QListWidgetItem *> teams)
{
    comp->setTitle(title.toStdString());
    for( QListWidgetItem * lsWidget: teams )
        this->teams.push_back(lsWidget->text());
    if( teams.size() == 16 ) {
        teamNum = EURO;
        ui->grbTeams->setMode(false);
        ui->stackedWidget->setCurrentWidget(ui->pgCreateGroups4);
    }
    else {
        teamNum = WORLDCUP;
        ui->grbTeams_2->setMode(true);
        ui->stackedWidget->setCurrentWidget(ui->pgCreateGroups8);
    }
    drawCreateGroups();
}

void CompetitionWindow::startGroupStage()
{
    if( (int)comp->getNumberOfTeams() == teams.size() ) {
        if( teamNum == EURO )
            ui->stackedWidget->setCurrentWidget(ui->pgGroupStage16);
        else
            ui->stackedWidget->setCurrentWidget(ui->pgGroupStage32);
        if( isGenerated )
            comp->startGroupStage();
        else
            comp->createGroups();
        drawGroupStage();
    }
    else {
        WarningDialog *needCreateFourGroups = new WarningDialog(this, "You need to create four groups!");
        needCreateFourGroups->exec();
        delete needCreateFourGroups;
    }
}

void CompetitionWindow::drawCreateGroups()
{
    TeamCheckBoxes *grb = ui->grbTeams;
    if( teamNum == WORLDCUP )
        grb = ui->grbTeams_2;
    for( int i = 0; i < teams.size(); i++ ) {
        QCheckBox *item = grb->item(i+1);
        item->setText(teams[i]);
        item->setIcon(QPixmap(":/Flags/" + teams[i] + ".png"));
        if( item->icon().isNull() ) item->setIcon(QPixmap(":/Flags/Unknown.png"));
        connect(item, SIGNAL(clicked()), ui->grbTeams, SLOT(checkMax()));
    }
}

void CompetitionWindow::drawGroupStage()
{
    sizeTableGroupStage();
    drawTablesGroupStage();
    drawMatchesGroupStage();
}

void CompetitionWindow::sizeTableGroupStage()
{
    for( Group group: comp->getGroupStage().getGroups() )
    {
        QFrame *frame = ui->stackedWidget->findChild<QFrame*>(QString("frame_GS")+QString::number(teamNum));
        QTreeWidget *table = frame->findChild<QTreeWidget*>(QString("tree_GS")+QString::number(teamNum)+"_Group"+group.getId());
        table->header()->resizeSection(0, 30);
        table->header()->resizeSection(1, 275);
        table->header()->resizeSection(2, 40);
        table->header()->resizeSection(3, 40);
        table->header()->resizeSection(4, 40);
        table->header()->resizeSection(5, 40);
        table->header()->resizeSection(6, 40);
        table->header()->resizeSection(7, 40);
        table->header()->resizeSection(8, 40);
        table->header()->resizeSection(9, 40);
        table->header()->resizeSection(10, 40);
    }
}

void CompetitionWindow::drawTablesGroupStage()
{
    for( Group &group : comp->getGroupStage().getGroups() )
    {
        QTreeWidget *table = ui->stackedWidget->findChild<QTreeWidget*>(QString("tree_GS")+QString::number(teamNum)+"_Group"+group.getId(), Qt::FindChildrenRecursively);
        for( TeamInGroup &team : group.getTeams() ) {
            QTreeWidgetItem *item = new QTreeWidgetItem(table);
            item->setText(1, QString::fromStdString(team.getName()));
            item->setIcon(1, QPixmap(":/Flags/" + item->text(1) + ".png"));
            if( item->icon(1).isNull() ) item->setIcon(1, QPixmap(":/Flags/Unknown.png"));
            item->setText(2, QString::number(team.getGames()));
            item->setText(3, QString::number(team.getWins()));
            item->setText(4, QString::number(team.getDraws()));
            item->setText(5, QString::number(team.getLoses()));
            item->setText(6, QString::number(team.getGoalsFor()));
            item->setText(7, QString::number(team.getGoalsAgainst()));
            item->setText(8, QString::number(team.getGoalsDiffrence()));
            item->setText(9, QString::number(team.getPoints()));
        }
    }
}

void CompetitionWindow::drawMatchesGroupStage()
{
    for( Group &group : comp->getGroupStage().getGroups() )
    {
        QWidget *tabMatches = ui->stackedWidget->findChild<QWidget*>(QString("tab_GS"+QString::number(teamNum)+"_Group"+group.getId()+"_Matches"), Qt::FindChildrenRecursively);
        int i = 1;
        for( MatchInGroup &match : group.getMatches() ) {
            QLabel *left = tabMatches->findChild<QLabel*>(QString("lbl_GS")+QString::number(teamNum)+"_Group"+group.getId()+"_left_m"+QString::number(i));
            left->setText(QString::fromStdString(match.getFirstTeam().getName()));
            left = new QLabel(tabMatches);
            left->setPixmap(QPixmap(":/Flags/" + QString::fromStdString(match.getFirstTeam().getName()) + ".png"));
            left->setGeometry(265, -10+40*i, 16, 16);
            left->show();


            QLabel *right = tabMatches->findChild<QLabel*>(QString("lbl_GS")+QString::number(teamNum)+"_Group"+group.getId()+"_right_m"+QString::number(i));
            right->setText(QString::fromStdString(match.getSecondTeam().getName()));
            right = new QLabel(tabMatches);
            right->setPixmap(QPixmap(":/Flags/" + QString::fromStdString(match.getSecondTeam().getName()) + ".png"));
            right->setGeometry(395, -10+40*i, 16, 16);
            right->show();
            i++;
        }
    }
    ui->tab_GS16_Groups->setCurrentIndex(0);
    ui->tab_GS16_GroupA->setCurrentIndex(0);
}

void CompetitionWindow::redrawTableGroupStage(char groupId)
{
    QTreeWidget *table = ui->stackedWidget->findChild<QTreeWidget*>(QString("tree_GS")+QString::number(teamNum)+"_Group"+groupId, Qt::FindChildrenRecursively);
    int i = 0;
    for( TeamInGroup &team : comp->getGroupStage().getGroup(groupId).getTeams() ) {
        QTreeWidgetItem *item = table->topLevelItem(i);
        item->setText(1, QString::fromStdString(team.getName()));
        item->setIcon(1, QPixmap(":/Flags/" + item->text(1) + ".png"));
        if( item->icon(1).isNull() ) item->setIcon(1, QPixmap(":/Flags/Unknown.png"));
        item->setText(2, QString::number(team.getGames()));
        item->setText(3, QString::number(team.getWins()));
        item->setText(4, QString::number(team.getDraws()));
        item->setText(5, QString::number(team.getLoses()));
        item->setText(6, QString::number(team.getGoalsFor()));
        item->setText(7, QString::number(team.getGoalsAgainst()));
        item->setText(8, QString::number(team.getGoalsDiffrence()));
        item->setText(9, QString::number(team.getPoints()));
        i++;
    }
}

void CompetitionWindow::saveResults()
{
    char groupId = sender()->objectName()[14].toLatin1();
    int i = 1;
    for( MatchInGroup &match : comp->getGroupStage().getGroup(groupId).getMatches() ) {
        QSpinBox *left = ui->frame_GS16->findChild<QSpinBox*>(QString("spb_GS16_GroupA_left_m")+QString::number(i));
        QSpinBox *right = ui->frame_GS16->findChild<QSpinBox*>(QString("spb_GS16_GroupA_right_m")+QString::number(i));
        match.setResult(left->value(), right->value());
        i++;
    }
    redrawTableGroupStage(groupId);
}

void CompetitionWindow::createGroup()
{
    TeamCheckBoxes *grb = ui->grbTeams;
    if( teamNum == WORLDCUP )
        grb = ui->grbTeams_2;
    if( grb->selectedItems().size() == 4 ) {
        char groupId = 'A' + comp->getTeams().size()/4;
        for( QCheckBox *item : grb->selectedItems() ) {
            comp->addTeam(item->text().toStdString());
            item->setStyleSheet("QCheckBox {"
                                "color: #6c6c6c"
                                "}"
                                "QCheckBox::indicator::checked{ "
                                "border: 2px solid #555555;"
                                "background-color: #6c6c6c;"
                                "}");
            item->setEnabled(false);
            item->setText(item->text() + " (" + groupId + ")");
        }
    }
    else {
        WarningDialog *needSelecetFourTeams = new WarningDialog(this, "You need to selecet four teams!");
        needSelecetFourTeams->exec();
        delete needSelecetFourTeams;
    }
}

void CompetitionWindow::generateGroups()
{
    isGenerated = true;
    TeamCheckBoxes *grb = ui->grbTeams;
    if( teamNum == WORLDCUP )
        grb = ui->grbTeams_2;
    for( int i = 0; i < teams.size(); i++ ) {
        char groupId = 'A' + i % (teamNum/4);
        QCheckBox *item = grb->item(i+1);
        comp->addTeam(item->text().toStdString());
        item->setStyleSheet("QCheckBox {"
                            "color: #6c6c6c"
                            "}"
                            "QCheckBox::indicator::checked{ "
                            "border: 2px solid #555555;"
                            "background-color: #6c6c6c;"
                            "}");
        item->setEnabled(false);
        item->setText(item->text() + " (" + groupId + ")");
    }
}

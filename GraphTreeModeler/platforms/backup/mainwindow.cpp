#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRadioButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDir>
#include <QFileInfo>
#include <iostream>
#include <QDateTime>
#include <QFileDialog>
#include "dialognextsketchguide.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    this->setWindowIcon(QIcon(":/res/res/logo.png"));
    this->setWindowTitle("GraphTreeModeler (UI)  @Zhihao");

    m_tcpHelper = new TcpHelper();
    m_tcpHelper->connectToServer(this);

    m_paintWidget = new PaintWidget(ui->centralWidget);

    ui->lblAdd->raise();
    ui->cbxAddMode->raise();

    this->initializeData();  // 加载一些资源数据（纹理等）;

    connect(ui->btnGenerateNew, SIGNAL(clicked(bool)),this, SLOT(slot_btnGenerateNew()));
    connect(ui->btnMakeScene, SIGNAL(clicked(bool)),this, SLOT(slot_btnMakeScene()));
    connect(ui->btnRandSeed, SIGNAL(clicked(bool)), this, SLOT(slot_btnRandSeed()));

    connect(ui->cbxAddMode, SIGNAL(currentIndexChanged(int)),this, SLOT(slot_setAddLevelType()));
    connect(this->m_paintWidget, SIGNAL(sig_selectedChanged()), this, SLOT(slot_updateSelectedUI()));
    connect(ui->btnUpdateParams, SIGNAL(clicked(bool)), this, SLOT(slot_updateCurNode_by_UI()));
    connect(ui->btnBranch_SplineLenFactor,SIGNAL(clicked(bool)), this, SLOT(slot_SpineChartPanel()));
    connect(ui->btnTrunk_SplineGravityFactor,SIGNAL(clicked(bool)), this, SLOT(slot_SpineChartPanel()));
    connect(ui->btnBranch_SplineGravityFactor,SIGNAL(clicked(bool)), this, SLOT(slot_SpineChartPanel()));
    connect(ui->actionLoadGraph, SIGNAL(triggered(bool)), this, SLOT(slot_loadGraph()));
    connect(ui->actionSaveGraph, SIGNAL(triggered(bool)), this, SLOT(slot_saveGraph()));

    setFocusPolicy(Qt::StrongFocus);

    this->slot_updateUI_by_CurNode();
}

void MainWindow::initializeData()
{
    // 初始化树叶纹理区域
    {
        QDir dir;
        dir.setPath(GlobalSetting::m_path_leaf_textures);
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        dir.setSorting(QDir::Name);

        QFileInfoList list = dir.entryInfoList();
        qDebug()<<"纹理数目:"<<list.size();

        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            if(fileInfo.suffix() == "png" || fileInfo.suffix() == "PNG" )
                ui->cbxLeafTextures->addItem(QIcon(GlobalSetting::m_path_leaf_textures+fileInfo.fileName()), fileInfo.fileName());
        }

        ui->cbxLeafTextures->setCurrentText(GlobalSetting::m_default_leaf_texture);
    }
    //    {
    //        QDir dir;
    //        dir.setPath(GlobalSetting::m_path_bark_textures);
    //        dir.setFilter(QDir::Files | QDir::NoSymLinks);
    //        dir.setSorting(QDir::Name);

    //        QFileInfoList list = dir.entryInfoList();
    //        for (int i = 0; i < list.size(); ++i) {
    //            QFileInfo fileInfo = list.at(i);
    //            if(fileInfo.suffix() == "png" || fileInfo.suffix() == "PNG" )
    //                ui->cbxBarkTextures->addItem(QIcon(GlobalSetting::m_path_bark_textures+fileInfo.fileName()), fileInfo.fileName());
    //        }

    //        ui->cbxBarkTextures->setCurrentText(GlobalSetting::m_default_bark_texture);
    //    }
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    int w = ui->centralWidget->width();
    int h = ui->centralWidget->height();

    // 横向
    m_paintWidget->move(0, 0);
    m_paintWidget->resize((w-ui->groupBox->width()), h);   // 画板

    ui->cbxAddMode->move(m_paintWidget->width()-ui->cbxAddMode->width()-5, 10);
    ui->lblAdd->move(ui->cbxAddMode->x()-ui->lblAdd->width(), ui->cbxAddMode->y());

    ui->groupBox->move(m_paintWidget->width(), 0);  // GroupBox
    m_paintWidget->update();
}


void MainWindow::slot_SpineChartPanel()
{
    m_lineChartView = new LineChartView(this);
    if(sender() == (QObject*)ui->btnBranch_SplineLenFactor)   // 如果点击的是branchLevel的长度渐变控制器
    {
        BranchLevel* curNode = (BranchLevel*)m_paintWidget->m_curPtr;
        this->m_lineChartView->SetControlPoints(curNode->lenGradient_ctrlPoints);
        this->m_lineChartView->exec();
        curNode->lenGradient_ctrlPoints = this->m_lineChartView->GetControlPoints();
        curNode->lenGradient_splinePoints = this->m_lineChartView->GetSplinePoints();
    }
    // 如果点击的是branchLevel的长度渐变控制器
    if(sender() == (QObject*)ui->btnTrunk_SplineGravityFactor || sender() == (QObject*)ui->btnBranch_SplineGravityFactor)
    {
        TrunkLevel* curNode = (TrunkLevel*)m_paintWidget->m_curPtr;
        this->m_lineChartView->SetControlPoints(curNode->gravityGradient_ctrlPoints);
        this->m_lineChartView->exec();
        curNode->gravityGradient_ctrlPoints = this->m_lineChartView->GetControlPoints();
        curNode->gravityGradient_splinePoints = this->m_lineChartView->GetSplinePoints();
    }
    delete m_lineChartView;
}


void MainWindow::slot_updateSelectedUI()
{
    if(this->m_paintWidget->m_curPtr == nullptr)
    {
        ui->tabWidget->setCurrentWidget(ui->tab_1);
        ui->lblName->setText( "None");
        ui->tabWidget->setEnabled(false);
    }
    else
    {
        ui->tabWidget->setEnabled(true);

        ui->lblName->setText(this->m_paintWidget->m_curPtr->name);
        if(this->m_paintWidget->m_curPtr->type == "BaseLevel")
        {
            ui->tabWidget->setCurrentWidget(ui->tab_1);
            ui->tab_1->setEnabled(true); ui->tab_2->setEnabled(false); ui->tab_3->setEnabled(false); ui->tab_4->setEnabled(false);
        }
        if(this->m_paintWidget->m_curPtr->type == "TrunkLevel")
        {
            ui->tabWidget->setCurrentWidget(ui->tab_2);
            ui->tab_1->setEnabled(false); ui->tab_2->setEnabled(true); ui->tab_3->setEnabled(false); ui->tab_4->setEnabled(false);

        }
        if(this->m_paintWidget->m_curPtr->type == "BranchLevel")
        {
            ui->tabWidget->setCurrentWidget(ui->tab_3);
            ui->tab_1->setEnabled(false); ui->tab_2->setEnabled(false); ui->tab_3->setEnabled(true); ui->tab_4->setEnabled(false);

        }
        if(this->m_paintWidget->m_curPtr->type == "LeafLevel")
        {
            ui->tabWidget->setCurrentWidget(ui->tab_4);
            ui->tab_1->setEnabled(false); ui->tab_2->setEnabled(false); ui->tab_3->setEnabled(false); ui->tab_4->setEnabled(true);
        }
    }

    this->slot_updateUI_by_CurNode();
}

void MainWindow::slot_updateUI_by_CurNode()
{
    auto curNode = m_paintWidget->m_curPtr;

    if(curNode == nullptr)
    {
        ui->tabWidget->setEnabled(false);
        return;
    }

    if(curNode->type == "BaseLevel")
    {
        ui->spinRoot_Seed->setValue(curNode->seed);  // seed
    }

    if(curNode->type == "TrunkLevel")
    {
        auto curTrunkNode = (TrunkLevel*)curNode;
        ui->spinTrunk_Seed->setValue(curTrunkNode->seed);
        ui->spinTrunk_LengthTotal->setValue(curTrunkNode->totalLen);
        ui->spinTrunk_RadiusMax->setValue(curTrunkNode->radiusMax);
        ui->spinTrunk_RadiusFactor->setValue(curTrunkNode->radiusExpandFactor);
        ui->spinTrunk_AngleTangential->setValue(curTrunkNode->angleTangential);
        ui->spinTrunk_Gravity->setValue(curTrunkNode->gravitySpinAngle);
        ui->spinTrunk_TwistNum->setValue(curTrunkNode->twistNodeNum);
        ui->spinTrunk_TwistDegree->setValue(curTrunkNode->twistNodeDegree);
    }

    if(curNode->type == "BranchLevel")
    {
        auto curBranchNode = (BranchLevel*)curNode;
        ui->spinBranch_Seed->setValue(curBranchNode->seed);
        ui->spinBranch_TotalLen->setValue(curBranchNode->totalLen);
        ui->spinBranch_RadiusMax->setValue(curBranchNode->radiusMax);
        ui->spinBranch_RadiusFactor->setValue(curBranchNode->radiusExpandFactor);
        ui->spinBranch_AngleTangential->setValue(curBranchNode->angleTangential);

        ui->spinBranch_subBranchNum->setValue(curBranchNode->subBranchNum);
        ui->spinBranch_subStart->setValue(curBranchNode->subStart);
        ui->spinBranch_subEnd->setValue(curBranchNode->subEnd);
        ui->spinBranch_Gravity->setValue(curBranchNode->gravitySpinAngle);
        ui->spinBranch_TwistNum->setValue(curBranchNode->twistNodeNum);
        ui->spinBranch_TwistDegree->setValue(curBranchNode->twistNodeDegree);
    }

    if(curNode->type == "LeafLevel")
    {
        auto curLeafNode = (LeafLevel*)curNode;
        ui->cbxLeafTextures->setCurrentText(curLeafNode->leafTextures);
        ui->spinLeafSeed->setValue(curLeafNode->seed);
        ui->spinLeaf_Num->setValue(curLeafNode->leafNum);
        ui->spinLeaf_Size->setValue(curLeafNode->leafSize);
        ui->spinLeafGravity->setValue(curLeafNode->gravityFactor);
    }
}

void MainWindow::slot_updateCurNode_by_UI()
{
    auto curNode = m_paintWidget->m_curPtr;

    if(curNode == nullptr)
        return;

    if(curNode->type == "BaseLevel")
    {
        curNode->seed = ui->spinRoot_Seed->value();
    }

    if(curNode->type == "TrunkLevel")
    {
        auto curTrunkNode = (TrunkLevel*)curNode;
        curTrunkNode->seed = ui->spinTrunk_Seed->value();
        curTrunkNode->totalLen = ui->spinTrunk_LengthTotal->value();
        curTrunkNode->radiusMax = ui->spinTrunk_RadiusMax->value();
        curTrunkNode->radiusExpandFactor = ui->spinTrunk_RadiusFactor->value();
        curTrunkNode->angleTangential = ui->spinTrunk_AngleTangential->value();
        curTrunkNode->gravitySpinAngle = ui->spinTrunk_Gravity->value();
        curTrunkNode->twistNodeNum = ui->spinTrunk_TwistNum->value();
        curTrunkNode->twistNodeDegree = ui->spinTrunk_TwistDegree->value();
    }

    if(curNode->type == "BranchLevel")
    {
        auto curBranchLevel = (BranchLevel*)curNode;
        curBranchLevel->seed = ui->spinBranch_Seed->value();
        curBranchLevel->totalLen = ui->spinBranch_TotalLen->value();
        curBranchLevel->radiusMax = ui->spinBranch_RadiusMax->value();
        curBranchLevel->radiusExpandFactor = ui->spinBranch_RadiusFactor->value();
        curBranchLevel->angleTangential = ui->spinBranch_AngleTangential->value();

        curBranchLevel->subBranchNum = ui->spinBranch_subBranchNum->value();
        curBranchLevel->subStart = ui->spinBranch_subStart->value();
        curBranchLevel->subEnd = ui->spinBranch_subEnd->value();
        curBranchLevel->gravitySpinAngle = ui->spinBranch_Gravity->value();
        curBranchLevel->twistNodeNum = ui->spinBranch_TwistNum->value();
        curBranchLevel->twistNodeDegree = ui->spinBranch_TwistDegree->value();
    }

    if(curNode->type == "LeafLevel")
    {
        auto curLeafLevel = (LeafLevel*)curNode;
        curLeafLevel->leafTextures = ui->cbxLeafTextures->currentText();
        curLeafLevel->seed = ui->spinLeafSeed->value();
        curLeafLevel->leafNum = ui->spinLeaf_Num->value();
        curLeafLevel->leafSize = ui->spinLeaf_Size->value();
        curLeafLevel->gravityFactor = ui->spinLeafGravity->value();
    }
}

void MainWindow::slot_setAddLevelType()
{
    if(ui->cbxAddMode->currentText() == "Trunk")
        this->m_paintWidget->currentAddNodeType = "TrunkLevel";
    if(ui->cbxAddMode->currentText() == "Branch")
        this->m_paintWidget->currentAddNodeType = "BranchLevel";
    if(ui->cbxAddMode->currentText() == "Leaf")
        this->m_paintWidget->currentAddNodeType = "LeafLevel";
}

void MainWindow::slot_btnGenerateNew()
{
    this->slot_updateCurNode_by_UI();
    this->m_paintWidget->SaveConstructedTreeStructure( GlobalSetting::m_buffer_jsonNode_path, GlobalSetting::m_buffer_jsonEdge_path);

    m_tcpHelper->SendMsg(GlobalSetting::command_header_GenerateNew + GlobalSetting::m_buffer_jsonNode_path + " " + GlobalSetting::m_buffer_jsonEdge_path);
}

void MainWindow::slot_btnRandSeed()
{
    this->m_paintWidget->SaveConstructedTreeStructure( GlobalSetting::m_buffer_jsonNode_path, GlobalSetting::m_buffer_jsonEdge_path);

    m_tcpHelper->SendMsg(GlobalSetting::command_header_RandSeed);
}

void MainWindow::slot_btnMakeScene()
{
    this->m_paintWidget->SaveConstructedTreeStructure( GlobalSetting::m_buffer_jsonNode_path, GlobalSetting::m_buffer_jsonEdge_path);

    m_tcpHelper->SendMsg(GlobalSetting::command_header_GenerateForest + GlobalSetting::m_buffer_jsonNode_path + " " + GlobalSetting::m_buffer_jsonEdge_path
                         + " " + QString::number(70));
}

void MainWindow::slot_saveGraph()
{
    {
        QDir dir(GlobalSetting::m_path_savedGraph);   // 如果不存在则创建。
        if (!dir.exists()) {
            dir.mkpath(".");
        }
    }

    QString filename = QFileDialog::getSaveFileName(this, "Save", GlobalSetting::m_path_savedGraph, tr("Tree_Files (*.graph)"));

    if(filename == "")
        return;


    auto edgeFilename = filename;
    auto nodeFilename = filename.replace(filename.lastIndexOf(".graph"), QString(".graph").length(), ".nodes");


    this->m_paintWidget->SaveConstructedTreeStructure(nodeFilename, edgeFilename);
}

void MainWindow::slot_loadGraph()
{
    QString filename = QFileDialog::getOpenFileName(this, "Load", GlobalSetting::m_path_savedGraph, tr("Tree_Files (*.graph )"));

    if(filename == "")
        return;

    auto edgeFilename = filename;
    auto nodeFilename = filename.replace(filename.lastIndexOf(".graph"), QString(".graph").length(), ".nodes");

    this->m_paintWidget->LoadConstructedTreeStructure(nodeFilename, edgeFilename);
}

void MainWindow::slot_sendBotanicalParameters()
{

}


MainWindow::~MainWindow()
{
    delete ui;
}

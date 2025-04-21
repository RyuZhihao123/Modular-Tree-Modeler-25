#include "dialognextsketchguide.h"
#include "ui_dialognextsketchguide.h"


QString imagePath = "C:/Users/liuzh/PycharmProjects/DeepTreeSketch/DeepTreeSketch/DeepTreeDrawing_Qt_Client/res/trees/";



DialogNextSketchGuide::DialogNextSketchGuide(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNextSketchGuide)
{
    ui->setupUi(this);

    this->setWindowTitle("Next-sketch Guidance");


    ui->comboBox->addItem(QIcon(imagePath+"Not yet selected"),"Not yet selected");


    ui->comboBox->addItem(QIcon(imagePath+"Birch"),"Birch");
    ui->comboBox->addItem(QIcon(imagePath+"Pine"),"Pine");
    ui->comboBox->addItem(QIcon(imagePath+"Prunus"),"Prunus");
    ui->comboBox->addItem(QIcon(imagePath+"Maple"),"Maple");
    ui->comboBox->addItem(QIcon(imagePath+"Beech"),"Beech");


    ui->comboBox->addItem(QIcon(imagePath+"Acer"),"Acer");
    ui->comboBox->addItem(QIcon(imagePath+"Apple"),"Apple");
    ui->comboBox->addItem(QIcon(imagePath+"Millingtonia"),"Millingtonia");
    ui->comboBox->addItem(QIcon(imagePath+"Oak"),"Oak");
    ui->comboBox->addItem(QIcon(imagePath+"Pterocarpus"),"Pterocarpus");

    ui->comboBox->addItem(QIcon(imagePath+"Tulip"),"Tulip");
    ui->comboBox->addItem(QIcon(imagePath+"Willow"),"Willow");


    connect(ui->comboBox, SIGNAL(editTextChanged(QString)), this,SLOT(slot_ComboBox()));
    connect(ui->buttonOk, SIGNAL(clicked(bool)), this, SLOT(slot_ButtonOk()));
    connect(ui->buttonCancel, SIGNAL(clicked(bool)), this, SLOT(slot_ButtonCancel()));

    slot_ComboBox();


}

void DialogNextSketchGuide::slot_ComboBox()
{
    QPixmap pixmap(imagePath+ui->comboBox->currentText());
    pixmap = pixmap.scaled(ui->labelImage->width(), ui->labelImage->height(), Qt::KeepAspectRatio);

    ui->labelImage->setPixmap(pixmap );
}

void DialogNextSketchGuide::slot_ButtonOk()
{
    this->is_ok = true;
    this->close();
}

void DialogNextSketchGuide::slot_ButtonCancel()
{
    this->is_ok = false;
    this->close();
}

DialogNextSketchGuide::~DialogNextSketchGuide()
{
    delete ui;
}

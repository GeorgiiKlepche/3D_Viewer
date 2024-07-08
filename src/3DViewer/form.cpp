#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: rgb(51, 51, 51);");
}

Form::~Form()
{
    delete ui;
}

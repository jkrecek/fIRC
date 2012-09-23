#include "firctab.h"
#include "ui_firctab.h"

fIRCtab::fIRCtab(QString tabName) :
    tabName_m(tabName),
    ui(new Ui::fIRCtab)
{
    ui->setupUi(this);
}

fIRCtab::~fIRCtab()
{
    delete ui;
}

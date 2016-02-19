#include "MainWindow.h"
#include "ui_MainWindow.h"
using namespace std;
extern string fileName;


extern double alpha[4];		// start configuration
extern double beta[4];		// goal configuration
extern double epsilon;			// resolution parameter


extern std::vector<int> expansions;


extern double L1;
extern double L2;
extern double R0;
extern int seed;
extern int QType;
extern bool runAnim;

extern bool hideBoxBoundary;
extern int numberForDisplay;


extern int renderSteps;
extern bool step;
int incr(1);



int animationSpeed(50);

void run();



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->inputFile->setText(QString::fromStdString(fileName.substr(0,fileName.length()-4)));
    ui->aX->setValue(alpha[0]);
    ui->aY->setValue(alpha[1]);
    ui->aT1->setValue(alpha[2]);
    ui->bX->setValue(beta[0]);
    ui->bY->setValue(beta[1]);
    ui->bT1->setValue(beta[2]);
    ui->l1->setValue(R0);
    ui->eps->setValue(epsilon);
    ui->random->setValue(seed);
    ui->greedy->setChecked(true);
    ui->boundary->setChecked(false);
    ui->steplabel->hide();
    ui->inc->setEnabled(false);
    ui->left->setEnabled(false);
    ui->right->setEnabled(false);





}

MainWindow::~MainWindow()
{
    delete ui;
}



//================================//
//     Display Text to Window     //
//================================//
/* Scrolls the text screen to the bottom and prints text */
MainWindow& MainWindow::operator<< (const std::string& str) {
ui->textOutput->moveCursor (QTextCursor::End);
ui->textOutput->insertPlainText(str.c_str());
ui->textOutput->moveCursor (QTextCursor::End);
return *this;
}

MainWindow& MainWindow::operator<< (const char* text) {
ui->textOutput->moveCursor (QTextCursor::End);
ui->textOutput->insertPlainText(text);
ui->textOutput->moveCursor (QTextCursor::End);
return *this;
}

MainWindow& MainWindow::operator<< (int i) {
ui->textOutput->moveCursor (QTextCursor::End);
ui->textOutput->insertPlainText(QString::number(i));
ui->textOutput->moveCursor (QTextCursor::End);
return *this;
}

MainWindow& MainWindow::operator<< (long l) {
ui->textOutput->moveCursor (QTextCursor::End);
ui->textOutput->insertPlainText(QString::number(l));
ui->textOutput->moveCursor (QTextCursor::End);
return *this;
}

MainWindow& MainWindow::operator<< (float f) {
ui->textOutput->moveCursor (QTextCursor::End);
ui->textOutput->insertPlainText(QString::number(f));
ui->textOutput->moveCursor (QTextCursor::End);
return *this;
}

MainWindow& MainWindow::operator<< (double d) {
ui->textOutput->moveCursor (QTextCursor::End);
ui->textOutput->insertPlainText(QString::number(d));
ui->textOutput->moveCursor (QTextCursor::End);
return *this;
}





void MainWindow::on_run_clicked()
{
    fileName=ui->inputFile->text().toStdString()+".txt";
    alpha[0]=ui->aX->value();
    alpha[1]=ui->aY->value();
    alpha[2]=ui->aT1->value();
    beta[0]=ui->bX->value();
    beta[1]=ui->bY->value();
    beta[2]=ui->bT1->value();
    R0=ui->l1->value();
    epsilon=ui->eps->value();
    seed=ui->random->value();

    hideBoxBoundary=ui->boundary->isChecked();




    run();

    ui->openGLWidget->genScene();
    ui->openGLWidget->update();



}


void MainWindow::on_randomButton_clicked()
{
    QType=0;
}

void MainWindow::on_bfs_clicked()
{
    QType=1;
}

void MainWindow::on_greedy_clicked()
{
    QType=2;
}

void MainWindow::on_dist_clicked()
{
    QType=3;
}

void MainWindow::on_vor_clicked()
{
    QType=4;
}

void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::on_anim_clicked()
{
    runAnim=true;
    this->update();

}

void MainWindow::on_boundary_clicked()
{
    hideBoxBoundary=ui->boundary->isChecked();
    this->update();
}



void MainWindow::on_pushButton_clicked()
{
    if(step){
        step=false;
        ui->steplabel->hide();
        renderSteps=1;
        incr=1;
        ui->inc->setValue(1);
        ui->inc->setEnabled(false);
        ui->left->setEnabled(false);
        ui->right->setEnabled(false);
        this->update();
    }else{
        step=true;
        incr=1;
        ui->steplabel->show();
        ui->steplabel->setText("Step: 1");
        ui->inc->setEnabled(true);
        ui->left->setEnabled(true);
        ui->right->setEnabled(true);
    }
}

void MainWindow::on_inc_valueChanged(int arg1)
{
    incr=arg1;
}

void MainWindow::on_left_clicked()
{
    if(renderSteps-incr<1) return;
    renderSteps-=incr;

    ui->steplabel->setText("Step: "+QString::number(renderSteps));

    ui->openGLWidget->genScene();
    ui->openGLWidget->update();
}

void MainWindow::on_right_clicked()
{

    renderSteps+=incr;
    if(renderSteps>expansions.size()-1) renderSteps=expansions.size()-1;
    ui->steplabel->setText("Step: "+QString::number(renderSteps));

    ui->openGLWidget->genScene();
    ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    animationSpeed=value;
}

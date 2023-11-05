#include "calc.h"

#include "graph.h"
#include "ui_calc.h"

namespace s21 {
Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Calculator) {
  ui->setupUi(this);
  credit_w_ = new Credit(this);
  credit_w_->setAttribute(Qt::WA_DeleteOnClose);
  connect(credit_w_, &Credit::ShowMain, this, &Calculator::show);

  deposit_w_ = new Deposit(this);
  deposit_w_->setAttribute(Qt::WA_DeleteOnClose);
  connect(deposit_w_, &Deposit::ShowMain, this, &Calculator::show);

  connect(deposit_w_, &Deposit::ShowSecondWindow, credit_w_, &Credit::show);
  connect(credit_w_, &Credit::ShowSecondWindow, deposit_w_, &Deposit::show);
}

Calculator::~Calculator() { delete ui; }

void Calculator::on_Appended_buttonClicked(QAbstractButton *button) {
  ClearIfNeeded();
  ComplementInsertion(button->text());
}

void Calculator::ClearIfNeeded() {
  if (clear_screen_) {
    ui->lineEdit->clear();
    clear_screen_ = false;
  }
}

void Calculator::on_Button_c_clicked() {
  ui->lineEdit->clear();
  clear_screen_ = true;
  ui->lineEdit->setText("0");
}

void Calculator::on_Button_backspace_clicked() {
  ClearIfNeeded();
  DeleteOneToken();
}

void Calculator::on_Button_eq_clicked() {
  if (!Evaluate(x_)) {
    if (std::isnan(result_) || std::isinf(result_)) clear_screen_ = true;
    ui->lineEdit->setText(ConvertExponentToDec());
  }
}

void Calculator::on_Button_xc_clicked() {
  x_ = 0.0;
  ui->label_x->setText("0");
}

void Calculator::on_Button_xs_clicked() {
  if (!Evaluate(x_)) {
    x_ = result_;
    ui->label_x->setText(QString::number(x_));
  }
}

void Calculator::on_Button_graph_clicked() {
  if (!Evaluate()) {
    auto *w = new Graph(ui->lineEdit->text());
    connect(w, &Graph::destroyed, this, &Calculator::show);
    emit GraphCreated(*w);
    hide();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
  }
}

void Calculator::on_actionCredit_triggered() {
  hide();
  credit_w_->show();
}

void Calculator::on_actionDeposit_triggered() {
  hide();
  deposit_w_->show();
}

void Calculator::Close() {
  show();
  close();
}

QString Calculator::ConvertExponentToDec() const {
  QString ret = QString::number(result_);
  auto pos = ret.indexOf('e');

  if (pos >= 0) {
    ret[pos] = '*';
    ret.insert(pos + 1, "10^(");
    ret.append(')');
  }
  return ret;
}

void Calculator::SetResult(double &result) { result_ = result; }

bool Calculator::Evaluate(const double &x) {
  SetStatusOK();
  emit EvaluateExpression(ui->lineEdit->text(), x);

  return error;
}

void Calculator::SetError(const QString &message) {
  ui->label_status->setText(message);
  error = true;
}

void Calculator::SetStatusOK() {
  error = false;
  ui->label_status->setText("OK");
}

Credit *Calculator::GetCredit() { return credit_w_; }

Deposit *Calculator::GetDeposit() { return deposit_w_; }
}  // namespace s21
//
// Created by ruslan on 25.02.23.
//

#include "graph.h"

#include "qcustomplot.h"
#include "ui_graph.h"

namespace s21 {

Graph::Graph(QString const &func, QWidget *parent)
    : QWidget(parent), ui(new Ui::Graph) {
  ui->setupUi(this);
  ui->Title->setText(QString("Graph: %1").arg(func));
}

Graph::~Graph() { delete ui; }

void Graph::showEvent(QShowEvent *event) {
  Plot();
  QWidget::showEvent(event);
}

void Graph::Plot() {
  PollSpins();
  ui->plot->addGraph();
  QVector<double> x(1000), y(1000);
  double step = (x_max_ - x_min_) / 999.0;

  x[0] = x_min_;
  emit SetPrecision(step);
  for (int i = 0; i <= 999; ++i) {
    if (i) x[i] = x[i - 1] + step;
    RequestCalculation(std::abs(x[i]) < step ? 0.0 : x[i]);
    y[i] = result_;
  }
  emit SetPrecision();
  ui->plot->graph(0)->setData(x, y);
  ui->plot->xAxis->setRange(x_min_, x_max_);
  ui->plot->xAxis->setLabel("X");
  ui->plot->yAxis->setRange(y_min_, y_max_);
  ui->plot->yAxis->setLabel("Y");
}

void Graph::on_pushButton_redraw_clicked() {
  ui->plot->removeGraph(0);
  Plot();
  ui->plot->replot();
}

void Graph::PollSpins() {
  x_max_ = ui->domain_max_spin->value();
  x_min_ = ui->domain_min_spin->value();
  y_max_ = ui->codomain_max_spin->value();
  y_min_ = ui->codomain_min_spin->value();
}

void Graph::on_domain_check_stateChanged() {
  if (ui->domain_check->isChecked()) {
    double vl = std::abs(ui->domain_max_spin->value());
    if (vl < 0.001) vl += 1;
    ui->domain_max_spin->setValue(vl);
    ui->domain_max_spin->setMinimum(0.001);
    on_domain_max_spin_valueChanged();
    ui->domain_min_spin->setDisabled(true);
  } else {
    ui->domain_min_spin->setDisabled(false);
    on_domain_min_spin_valueChanged();
  }
}

void Graph::on_codomain_check_stateChanged() {
  if (ui->codomain_check->isChecked()) {
    double vl = std::abs(ui->codomain_max_spin->value());
    if (vl < 0.001) vl += 1;
    ui->codomain_max_spin->setValue(vl);
    ui->codomain_max_spin->setMinimum(0.001);
    on_codomain_max_spin_valueChanged();
    ui->codomain_min_spin->setDisabled(true);
  } else {
    ui->codomain_min_spin->setDisabled(false);
    on_codomain_min_spin_valueChanged();
  }
}

void Graph::on_domain_max_spin_valueChanged() {
  double vl = ui->domain_max_spin->value();
  if (ui->domain_check->isChecked()) {
    ui->domain_min_spin->setValue(-vl);
  }
  ui->domain_min_spin->setMaximum(vl - 0.001);
}

void Graph::on_codomain_max_spin_valueChanged() {
  double vl = ui->codomain_max_spin->value();
  if (ui->codomain_check->isChecked()) {
    ui->codomain_min_spin->setValue(-vl);
  }
  ui->codomain_min_spin->setMaximum(vl - 0.001);
}

void Graph::on_codomain_min_spin_valueChanged() {
  ui->codomain_max_spin->setMinimum(ui->codomain_min_spin->value() + 0.001);
}

void Graph::on_domain_min_spin_valueChanged() {
  ui->domain_max_spin->setMinimum(ui->domain_min_spin->value() + 0.001);
}

void Graph::SetResult(const double &result) { result_ = result; }
}  // namespace s21
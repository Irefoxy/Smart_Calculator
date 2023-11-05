#include <QApplication>

#include "calc.h"
#include "controller.h"
#include "model.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::Calculator view;
  s21::model model;
  s21::controller controller(&model, &view);
  view.show();
  return a.exec();
}

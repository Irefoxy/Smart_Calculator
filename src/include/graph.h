//
// Created by ruslan on 25.02.23.
//

#ifndef CALC_SRC_QT_GRAPH_H_
#define CALC_SRC_QT_GRAPH_H_

#include <QAbstractButton>
#include <QCheckBox>
#include <QMainWindow>

namespace Ui {
class Graph;
}

namespace s21 {
/**
 * @file graph.cc - graph class definitions
 */

/**
 * @class Graph
 * @brief Class for graph window widget
 */
class Graph : public QWidget {
  Q_OBJECT

 public:
  /**
   * Constructor. Set ui and connects. Get expression to display
   * @param func
   * @param parent
   */
  explicit Graph(const QString &func, QWidget *parent = nullptr);

  /**
   * Destructor
   */
  ~Graph() override;

  /**
   * Public method for controller to set result
   * @param result
   */
  void SetResult(const double &result);

 signals:

  void RequestCalculation(const double &x);

  void SetPrecision(const double &precision = 1e-7);

 private slots:

  void on_pushButton_redraw_clicked();

  void on_domain_min_spin_valueChanged();

  void on_domain_max_spin_valueChanged();

  void on_codomain_max_spin_valueChanged();

  void on_codomain_min_spin_valueChanged();

  void on_domain_check_stateChanged();

  void on_codomain_check_stateChanged();

 private:
  /**
   * Method to plot the expression
   */
  void Plot();

  /**
   * Polls spins values from gui
   */
  void PollSpins();

  /**
   * Overrides show method to plot expression before show
   * @param event
   */
  void showEvent(QShowEvent *event) override;

 private:
  double result_ = NAN;
  Ui::Graph *ui;
  double x_min_ = 0;
  double x_max_ = 0;
  double y_min_ = 0;
  double y_max_ = 0;
};
}  // namespace s21

#endif  // CALC_SRC_QT_GRAPH_H_

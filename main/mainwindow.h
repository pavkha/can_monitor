#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../device/device.h"
#include <QApplication>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QMetaType>
#include <QPushButton>
#include <QSpinBox>
#include <QStatusBar>
#include <QTextBrowser>
#include <QTime>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>
#include <QVector>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  void init_ui();
  void init_device();

  enum {
    can_data0,
    can_data1,
    can_data2,
    can_data3,
    can_data4,
    can_data5,
    can_data6,
    can_data7,
    can_data_count
  };

  static int const message_browser_max_lines{1000};

private slots:
  void device_settings_dialog();
  void device_connect();
  void device_disconnect();

  void device_msg_read(VSCAN_MSG);
  void device_send_msg();
  void device_error_read_thread(int);

  void send_message_timer_stop();

private slots:
  void about();
  void clear_browsers();

private:
  QLabel *status_bar_state_label{nullptr};
  QLabel *status_bar_device_label{nullptr};
  QLabel *receive_label{nullptr};
  QLabel *receive_count_label{nullptr};
  QLabel *transmit_label{nullptr};
  QLabel *transmit_count_label{nullptr};
  QLabel *can_id_label{nullptr};
  QLabel *dlc_label{nullptr};
  QLabel *data_label{nullptr};

  QMenu *file_menu{nullptr};
  QMenu *device_menu{nullptr};
  QMenu *help_menu{nullptr};

  QToolBar *device_settings_toolbar{nullptr};
  QToolBar *connect_toolbar{nullptr};
  QToolBar *disconnect_toolbar{nullptr};
  QToolBar *exit_toolbar{nullptr};
  QToolBar *clear_browser_toolbar{nullptr};

  QAction *exit_action{nullptr};
  QAction *device_settings_action{nullptr};
  QAction *device_connect_action{nullptr};
  QAction *device_disconnect_action{nullptr};
  QAction *about_action{nullptr};
  QAction *clear_browser_action{nullptr};

  QTextBrowser *receive_message_browser{nullptr};
  QTextBrowser *transmit_message_browser{nullptr};

  QStatusBar *status_bar{nullptr};

  QLineEdit *can_id_line_edit{nullptr};
  QVector<QLineEdit *> *transmit_data_ctrl{nullptr};

  QPushButton *send_message_btn{nullptr};

  QCheckBox *repeat_checkbox{nullptr};
  QCheckBox *extended_checkbox{nullptr};
  QCheckBox *rtr_checkbox{nullptr};

  QSpinBox *dlc_spinbox{nullptr};
  QSpinBox *repeat_spinbox{nullptr};

  QTimer *send_message_timer{nullptr};

private:
  device::device_ptr device_{nullptr};
};

#endif // MAINWINDOW_H

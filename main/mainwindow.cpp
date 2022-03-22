#include "mainwindow.h"
#include "version.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  init_ui();
  init_device();
}

MainWindow::~MainWindow() { device_disconnect(); }

void MainWindow::init_ui() {
  setWindowTitle("CAN monitor VScom");
  resize(1250, 850);
  setMinimumWidth(1250);

  file_menu = menuBar()->addMenu("&File");
  device_menu = menuBar()->addMenu("&Device");
  help_menu = menuBar()->addMenu("&Help");

  device_connect_action =
      new QAction(QIcon(":/icons/connect.png"), "Connect", this);
  device_disconnect_action =
      new QAction(QIcon(":/icons/disconnect.png"), "Disconnect", this);
  device_settings_action =
      new QAction(QIcon(":/icons/settings.png"), "Settings...", this);
  exit_action = new QAction(QIcon(":/icons/exit.png"), "Quit", this);
  about_action = new QAction("About...", this);
  clear_browser_action = new QAction(QIcon(":/icons/clear.png"), "Clear", this);

  connect_toolbar = addToolBar("Connect");
  connect_toolbar->setMovable(false);
  disconnect_toolbar = addToolBar("Disonnect");
  disconnect_toolbar->setMovable(false);
  device_settings_toolbar = addToolBar("Settings");
  device_settings_toolbar->setMovable(false);
  clear_browser_toolbar = addToolBar("Clear");
  clear_browser_toolbar->setMovable(false);
  exit_toolbar = addToolBar("Quit");
  exit_toolbar->setMovable(false);

  file_menu->addSeparator();
  file_menu->addAction(exit_action);

  device_menu->addAction(device_settings_action);
  device_menu->addSeparator();
  device_menu->addAction(device_connect_action);
  device_menu->addAction(device_disconnect_action);

  help_menu->addAction(about_action);

  connect_toolbar->addAction(device_connect_action);
  disconnect_toolbar->addAction(device_disconnect_action);
  device_settings_toolbar->addAction(device_settings_action);
  exit_toolbar->addAction(exit_action);
  clear_browser_toolbar->addAction(clear_browser_action);

  receive_message_browser = new QTextBrowser(this);
  transmit_message_browser = new QTextBrowser(this);

  receive_label = new QLabel("Received:");
  receive_count_label = new QLabel("0");
  transmit_label = new QLabel("Transmitted:");
  transmit_count_label = new QLabel("0");
  can_id_label = new QLabel("CAN ID <hex>");
  dlc_label = new QLabel("DLC");
  data_label = new QLabel("Data 0 ... Data 7 <hex>");

  can_id_line_edit = new QLineEdit;
  transmit_data_ctrl = new QVector<QLineEdit *>;

  send_message_btn = new QPushButton("Send");

  extended_checkbox = new QCheckBox;
  extended_checkbox->setText("Extended");
  rtr_checkbox = new QCheckBox;
  rtr_checkbox->setText("RTR");
  repeat_checkbox = new QCheckBox;
  repeat_checkbox->setText("Repeat, ms");

  dlc_spinbox = new QSpinBox;
  repeat_spinbox = new QSpinBox;

  QRegularExpression can_id_reg_exp("([0-9A-Fa-f]{1,8})");
  QRegularExpressionValidator *can_id_validator =
      new QRegularExpressionValidator(can_id_reg_exp, this);

  can_id_line_edit->setValidator(can_id_validator);
  can_id_line_edit->setFixedWidth(100);
  can_id_line_edit->setAlignment(Qt::AlignRight);
  can_id_line_edit->setText("0");

  dlc_spinbox->setFixedWidth(31);
  dlc_spinbox->setAlignment(Qt::AlignTop);
  dlc_spinbox->setMinimum(0);
  dlc_spinbox->setMaximum(8);
  dlc_spinbox->setValue(8);

  repeat_spinbox->setFixedWidth(80);
  repeat_spinbox->setAlignment(Qt::AlignRight);
  repeat_spinbox->setMinimum(10);
  repeat_spinbox->setMaximum(1000000);
  repeat_spinbox->setValue(1000);

  QRegularExpression can_data_reg_exp("([0-9A-Fa-f]{1,2})");
  QRegularExpressionValidator *can_data_validator =
      new QRegularExpressionValidator(can_data_reg_exp, this);

  for (auto i{0}; i < can_data_count; ++i) {
    transmit_data_ctrl->append(new QLineEdit);
    transmit_data_ctrl->at(i)->setValidator(can_data_validator);
    transmit_data_ctrl->at(i)->setFixedWidth(25);
    transmit_data_ctrl->at(i)->setAlignment(Qt::AlignRight);
    transmit_data_ctrl->at(i)->setText(QString("0%1").arg(i));
  }

  QHBoxLayout *transmit_ctr_label_layout = new QHBoxLayout;
  transmit_ctr_label_layout->addWidget(can_id_label);
  transmit_ctr_label_layout->addSpacing(20);
  transmit_ctr_label_layout->addWidget(dlc_label);
  transmit_ctr_label_layout->addSpacing(60);
  transmit_ctr_label_layout->addWidget(data_label);
  transmit_ctr_label_layout->addStretch();

  QHBoxLayout *transmit_ctr_data_layout = new QHBoxLayout;
  transmit_ctr_data_layout->addWidget(can_id_line_edit);
  transmit_ctr_data_layout->addWidget(dlc_spinbox);
  for (auto i{0}; i < can_data_count; ++i) {
    transmit_ctr_data_layout->addWidget(transmit_data_ctrl->at(i));
  }
  transmit_ctr_data_layout->addWidget(send_message_btn);

  QHBoxLayout *transmit_ctr_ext_layout = new QHBoxLayout;
  transmit_ctr_ext_layout->addWidget(extended_checkbox);
  transmit_ctr_ext_layout->addWidget(rtr_checkbox);
  transmit_ctr_ext_layout->addStretch();
  transmit_ctr_ext_layout->addWidget(repeat_checkbox);
  transmit_ctr_ext_layout->addWidget(repeat_spinbox);

  QVBoxLayout *transmit_message_ctrl_layout = new QVBoxLayout;
  transmit_message_ctrl_layout->addLayout(transmit_ctr_label_layout);
  transmit_message_ctrl_layout->addLayout(transmit_ctr_data_layout);
  transmit_message_ctrl_layout->addLayout(transmit_ctr_ext_layout);

  QHBoxLayout *transmit_info_layout = new QHBoxLayout;
  transmit_info_layout->addWidget(transmit_label);
  transmit_info_layout->addWidget(transmit_count_label);
  transmit_info_layout->addStretch();

  QVBoxLayout *transmit_message_browser_layout = new QVBoxLayout;
  transmit_message_browser_layout->addLayout(transmit_info_layout);
  transmit_message_browser_layout->addWidget(transmit_message_browser);
  transmit_message_browser_layout->addSpacing(10);
  transmit_message_browser_layout->addLayout(transmit_message_ctrl_layout);
  transmit_message_browser_layout->addSpacing(3);

  QHBoxLayout *receive_info_layout = new QHBoxLayout;
  receive_info_layout->addWidget(receive_label);
  receive_info_layout->addWidget(receive_count_label);
  receive_info_layout->addStretch();

  QVBoxLayout *receive_message_browser_layout = new QVBoxLayout;
  receive_message_browser_layout->addLayout(receive_info_layout);
  receive_message_browser_layout->addWidget(receive_message_browser);

  QHBoxLayout *receive_transmit_layout = new QHBoxLayout;
  receive_transmit_layout->addLayout(receive_message_browser_layout);
  receive_transmit_layout->addLayout(transmit_message_browser_layout);

  QVBoxLayout *main_layout = new QVBoxLayout;
  main_layout->addLayout(receive_transmit_layout);
  main_layout->addSpacing(5);

  QWidget *widget = new QWidget;
  widget->setLayout(main_layout);
  setCentralWidget(widget);

  status_bar = statusBar();
  status_bar_state_label = new QLabel(status_bar);
  status_bar_state_label->setText(QString("Status: ") +
                                  QString("disconnected"));
  status_bar_device_label = new QLabel(status_bar);
  status_bar_device_label->setText(QString("Device: ") + QString("non"));

  status_bar->addWidget(status_bar_device_label);
  status_bar->addPermanentWidget(status_bar_state_label);

  send_message_timer = new QTimer;

  qRegisterMetaType<VSCAN_MSG>("VSCAN_MSG");

  connect(device_settings_action, SIGNAL(triggered()),
          SLOT(device_settings_dialog()));
  connect(device_connect_action, SIGNAL(triggered()), SLOT(device_connect()));
  connect(device_disconnect_action, SIGNAL(triggered()),
          SLOT(device_disconnect()));
  connect(clear_browser_action, SIGNAL(triggered()), SLOT(clear_browsers()));
  connect(exit_action, SIGNAL(triggered()), QApplication::instance(),
          SLOT(quit()));
  connect(about_action, SIGNAL(triggered()), SLOT(about()));

  connect(send_message_btn, SIGNAL(clicked()), SLOT(device_send_msg()));
  connect(send_message_timer, SIGNAL(timeout()), SLOT(device_send_msg()));
  connect(repeat_checkbox, SIGNAL(toggled(bool)),
          SLOT(send_message_timer_stop()));

  device_connect_action->setDisabled(true);
  device_disconnect_action->setDisabled(true);
  send_message_btn->setDisabled(true);
}

void MainWindow::about() {
  QMessageBox::about(this, "About",
                     "<b>CANmonitor VScom </b>" CAN_MONITOR_VERSION);
  QMessageBox::aboutQt(this);
}

void MainWindow::init_device() {
  device_ = new device::vscom_usb_can_plus{};
  connect(device_, SIGNAL(msg_read(VSCAN_MSG)),
          SLOT(device_msg_read(VSCAN_MSG)));
  connect(device_, SIGNAL(error_read_thread(int)),
          SLOT(device_error_read_thread(int)));
}

void MainWindow::device_settings_dialog() {
  device::settings_data edit_settings = *device_->get_settings();
  device::vscom_info edit_info = *device_->get_cached_info();
  device::settings_dialog *device_dialog =
      new device::settings_dialog(&edit_settings, &edit_info, this);

  if (device_dialog->exec() == QDialog::Accepted) {
    device_dialog->get_device_settings(&edit_settings);
    if (edit_settings.port_name != "") {
      device_->change_settings(&edit_settings);
      device_dialog->get_device_info(&edit_info);
      device_->set_cached_info(&edit_info);
      status_bar_device_label->setText(
          "Device: " + edit_settings.port_name + ' ' +
          device::settings_data::get_mode_string(edit_settings.mode) + ' ' +
          device::settings_data::get_can_br_string(edit_settings.br));
      device_connect_action->setEnabled(true);
    } else {
      edit_settings.set_default();
      device_->change_settings(&edit_settings);
      status_bar_device_label->setText("Device: non");
      device_connect_action->setDisabled(true);
    }
  }

  delete device_dialog;
}

void MainWindow::device_connect() {
  device::settings_data device_settings = *device_->get_settings();

  VSCAN_HANDLE handle =
      VSCAN_Open((char *)(device_settings.port_name.toStdString().c_str()),
                 device_settings.mode);

  if (handle < 0) {
    QMessageBox::critical(this, "Error", "Can`t open device!");
    return;
  }

  VSCAN_STATUS status = VSCAN_Ioctl(
      handle, VSCAN_IOCTL_SET_SPEED,
      (void *)device::settings_data::get_can_br_dec(device_settings.br));
  if (status != VSCAN_ERR_OK) {
    QMessageBox::critical(this, "Error", "Can`t set speed!");
    return;
  }

  if (device_settings.dual_filter) {
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_FILTER_MODE,
                         VSCAN_FILTER_MODE_DUAL);
    if (status != VSCAN_ERR_OK) {
      QMessageBox::critical(this, "Error", "Can`t set filter mode!");
      return;
    }
  }

  status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_ACC_CODE_MASK,
                       &device_settings.code_mask);
  if (status != VSCAN_ERR_OK) {
    QMessageBox::critical(this, "Error", "Can`t set acc code mask!");
    return;
  }

  device_->set_handle(handle);

  device_disconnect_action->setEnabled(true);
  device_connect_action->setDisabled(true);
  device_settings_action->setDisabled(true);
  if (device_settings.mode != device::settings_data::mode_listen) {
    send_message_btn->setEnabled(true);
  }
  status_bar_state_label->setText("Status: connected");

  device_->start_read_thread();
}

void MainWindow::device_disconnect() {
  VSCAN_HANDLE handle = device_->get_handle();

  if (handle > 0) {
    VSCAN_Close(handle);
    device_->set_handle(0);
  }

  repeat_checkbox->setChecked(false);
  device_connect_action->setEnabled(true);
  device_disconnect_action->setDisabled(true);
  device_settings_action->setEnabled(true);
  send_message_btn->setDisabled(true);
  status_bar_state_label->setText("Status: disconnected");

  device_->stop_read_thread();
}

void MainWindow::device_msg_read(VSCAN_MSG msg) {
  QString curr_time = QTime::currentTime().toString("HH:mm:ss:zzz");
  QString in_marker{" -> "};
  QString read_msg = device::vscan_msg_to_qstring(msg);

  auto count = receive_count_label->text().toInt();
  if (count >= message_browser_max_lines) {
    receive_message_browser->clear();
    receive_count_label->setText("0");
    count = 0;
  }

  receive_message_browser->append(curr_time + in_marker + read_msg);
  receive_count_label->setText(QString::number(++count));
}

void MainWindow::device_send_msg() {
  QString curr_time = QTime::currentTime().toString("HH:mm:ss:zzz");

  if (repeat_checkbox->isChecked()) {
    auto period = repeat_spinbox->text().toInt();
    if (period < 10) {
      period = 10;
    }
    send_message_timer->start(period);
  }

  VSCAN_MSG msg;

  msg.Id = can_id_line_edit->text().toUInt(nullptr, 16);
  msg.Size = dlc_spinbox->text().toUShort();
  msg.Flags = ((extended_checkbox->isChecked() << (VSCAN_FLAGS_EXTENDED >> 1)) |
               (rtr_checkbox->isChecked() << (VSCAN_FLAGS_REMOTE >> 1)));
  msg.Timestamp = 0;

  for (auto i = 0; i < msg.Size; ++i) {
    msg.Data[i] = transmit_data_ctrl->at(i)->text().toUShort(nullptr, 16);
  }

  QString out_marker{" <- "};

  if (device_->send_msg(msg)) {
    QString send_msg = device::vscan_msg_to_qstring(msg);

    auto count = transmit_count_label->text().toInt();
    if (count >= message_browser_max_lines) {
      transmit_message_browser->clear();
      transmit_count_label->setText("0");
      count = 0;
    }
    transmit_message_browser->append(curr_time + out_marker + send_msg);
    transmit_count_label->setText(QString::number(++count));
  } else {
    QString error_string = "[!] Send message error";
    transmit_message_browser->append(curr_time + out_marker + error_string);

    device_disconnect();
  }
}

void MainWindow::device_error_read_thread(int e) {
  QString curr_time = QTime::currentTime().toString("HH:mm:ss:zzz");
  QString in_marker{" -> "};
  QString error_string = QString("[!] Read thread error: %1").arg(e);

  receive_message_browser->append(curr_time + in_marker + error_string);

  device_disconnect();
}

void MainWindow::clear_browsers() {
  receive_message_browser->clear();
  receive_count_label->setText("0");
  transmit_message_browser->clear();
  transmit_count_label->setText("0");
}

void MainWindow::send_message_timer_stop() { send_message_timer->stop(); }

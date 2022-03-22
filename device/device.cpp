#include "device.h"

namespace device {

settings_dialog::settings_dialog(settings_data const *sdata,
                                 device_info const *info, QWidget *w)
    : QDialog{w} {
  setWindowTitle("Settings");
  resize(350, 420);

  btn_box = new QDialogButtonBox(Qt::Horizontal, this);
  port_group = new QGroupBox("Device", this);
  port_combobox = new QComboBox(this);
  search_btn = new QPushButton("Search", this);

  config_group = new QGroupBox("Config", this);
  mode_group = new QGroupBox("Mode", this);

  radio_btn_mode_group = new QButtonGroup(this);

  normal_mode_radio_btn = new QRadioButton("Normal", this);
  listen_mode_radio_btn = new QRadioButton("Listen only", this);
  self_mode_radio_btn = new QRadioButton("Self reception", this);

  debug_checkbox = new QCheckBox(this);
  debug_checkbox->setText("Debug mode");
  hw_timestamps_checkbox = new QCheckBox(this);
  hw_timestamps_checkbox->setText("HW timestamps");

  baudrate_combobox = new QComboBox(this);

  device_api_label = new QLabel("API version:", this);
  device_api_info_label = new QLabel("", this);
  device_type_label = new QLabel("Device type:", this);
  device_type_info_label = new QLabel("", this);
  device_sn_label = new QLabel("Serial number:", this);
  device_sn_info_label = new QLabel("", this);
  device_hw_label = new QLabel("HW version:", this);
  device_hw_info_label = new QLabel("", this);
  device_sw_label = new QLabel("SW version:", this);
  device_sw_info_label = new QLabel("", this);
  baudrate_label = new QLabel("Baudrate, bit", this);

  main_layout = new QVBoxLayout(this);
  port_main_layout = new QVBoxLayout(port_group);
  port_act_layout = new QHBoxLayout();
  port_device_info_layout = new QGridLayout();
  mode_layout = new QVBoxLayout(mode_group);
  config_main_layout = new QGridLayout(config_group);
  baudrate_layout = new QVBoxLayout();

  port_device_info_layout->addWidget(device_api_label, 0, 0);
  port_device_info_layout->addWidget(device_api_info_label, 0, 1);

  port_device_info_layout->addWidget(device_sn_label, 1, 0);
  port_device_info_layout->addWidget(device_sn_info_label, 1, 1);
  port_device_info_layout->addWidget(device_hw_label, 1, 2);
  port_device_info_layout->addWidget(device_hw_info_label, 1, 3);

  port_device_info_layout->addWidget(device_type_label, 2, 0);
  port_device_info_layout->addWidget(device_type_info_label, 2, 1);
  port_device_info_layout->addWidget(device_sw_label, 2, 2);
  port_device_info_layout->addWidget(device_sw_info_label, 2, 3);

  port_device_info_layout->setColumnStretch(1, 100);

  btn_box->addButton("Ok", QDialogButtonBox::AcceptRole);
  btn_box->addButton("Cancel", QDialogButtonBox::RejectRole);

  port_group->setStyleSheet("QGroupBox::title {subcontrol-origin: margin; \
                                                 left: 7; \
                                                 top: 12;}");
  config_group->setStyleSheet("QGroupBox::title {subcontrol-origin: margin; \
                                                 left: 7; \
                                                 top: 12;}");
  mode_group->setStyleSheet("QGroupBox::title {subcontrol-origin: margin; \
                                                 left: 7; \
                                                 top: 12;}");

  port_act_layout->addWidget(port_combobox);
  port_act_layout->addWidget(search_btn);

  port_main_layout->addLayout(port_act_layout);
  port_main_layout->addSpacing(10);
  port_main_layout->addLayout(port_device_info_layout);

  port_group->setLayout(port_main_layout);

  radio_btn_mode_group->setExclusive(true);
  radio_btn_mode_group->addButton(normal_mode_radio_btn, 0);
  radio_btn_mode_group->addButton(listen_mode_radio_btn, 1);
  radio_btn_mode_group->addButton(self_mode_radio_btn, 2);
  radio_btn_mode_group->button(1)->setChecked(true);

  mode_layout->addWidget(normal_mode_radio_btn);
  mode_layout->addWidget(listen_mode_radio_btn);
  mode_layout->addWidget(self_mode_radio_btn);
  mode_group->setLayout(mode_layout);

  config_main_layout->addWidget(mode_group, 0, 0, 15, 1);
  config_main_layout->addWidget(debug_checkbox, 3, 1);
  config_main_layout->addWidget(hw_timestamps_checkbox, 4, 1);

  config_group->setLayout(config_main_layout);

  baudrate_combobox->setFixedWidth(100);

  baudrate_combobox->addItem("100000");
  baudrate_combobox->addItem("125000");
  baudrate_combobox->addItem("250000");
  baudrate_combobox->addItem("500000");
  baudrate_combobox->addItem("1000000");

  baudrate_layout->addWidget(baudrate_label);
  baudrate_layout->addWidget(baudrate_combobox);

  main_layout->addWidget(port_group);
  main_layout->addWidget(config_group);
  main_layout->addSpacing(10);
  main_layout->addLayout(baudrate_layout);
  main_layout->addSpacing(10);
  main_layout->addStretch();
  main_layout->addWidget(btn_box);

  port_combobox->addItem(sdata->port_name);
  radio_btn_mode_group->buttons().at(sdata->mode)->setChecked(true);
  baudrate_combobox->setCurrentIndex(sdata->br);

  device_info_ = *info;

  if (port_combobox->currentText() != "") {
    device_api_info_label->setText(QString("%1.%2.%3")
                                       .arg(device_info_.api_ver.Major)
                                       .arg(device_info_.api_ver.Minor)
                                       .arg(device_info_.api_ver.SubMinor));
    device_type_info_label->setText(
        vscom_info::get_type_string(device_info_.hw_param.HwType));
    device_sn_info_label->setText(
        QString::number(device_info_.hw_param.SerialNr));
    device_hw_info_label->setText(
        QString("%1.%2")
            .arg((device_info_.hw_param.HwVersion >> 4) & 0x0f)
            .arg((device_info_.hw_param.HwVersion) & 0x0f));
    device_sw_info_label->setText(
        QString("%1.%2")
            .arg((device_info_.hw_param.SwVersion >> 4) & 0x0f)
            .arg((device_info_.hw_param.SwVersion) & 0x0f));
  }

  connect(btn_box, SIGNAL(accepted()), SLOT(accept()));
  connect(btn_box, SIGNAL(rejected()), SLOT(reject()));
  connect(search_btn, SIGNAL(released()), SLOT(search_device()));
  connect(port_combobox, SIGNAL(currentIndexChanged(int)),
          SLOT(change_device()));
}

void settings_dialog::search_device() {
  port_combobox->clear();

  QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

  for (auto &port : ports) {
    VSCAN_HANDLE handle = VSCAN_Open(
        (char *)port.portName().toStdString().c_str(), VSCAN_MODE_LISTEN_ONLY);

    if (handle < 0) {
      continue;
    } else {
      VSCAN_STATUS status = VSCAN_Close(handle);
      if (status == VSCAN_ERR_OK) {
        port_combobox->addItem(port.portName());
      }
    }
  }

  if (port_combobox->count() == 0) {
    reset_controls();
    QMessageBox::warning(this, "Search", "Device not found");
  }
}

void settings_dialog::reset_controls() {
  port_combobox->clear();
  device_api_info_label->clear();
  device_type_info_label->clear();
  device_sn_info_label->clear();
  device_hw_info_label->clear();
  device_sw_info_label->clear();
  radio_btn_mode_group->buttons().at(0)->setChecked(true);
  baudrate_combobox->setCurrentIndex(0);
}

void settings_dialog::change_device() { read_device_info(); }

void settings_dialog::get_device_settings(settings_data *sdata) {
  sdata->port_name = port_combobox->currentText();
  sdata->mode = radio_btn_mode_group->checkedId();
  sdata->br = baudrate_combobox->currentIndex();
}

void settings_dialog::get_device_info(device_info *info) {
  *info = device_info_;
}

void settings_dialog::read_device_info() {
  if (port_combobox->currentText() == "") {
    return;
  }

  VSCAN_HANDLE handle =
      VSCAN_Open((char *)port_combobox->currentText().toStdString().c_str(),
                 VSCAN_MODE_LISTEN_ONLY);

  VSCAN_STATUS status =
      VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_API_VERSION, &device_info_.api_ver);
  if (status != VSCAN_ERR_OK) {
    QMessageBox::critical(this, "Error", "Can`t read api version!");
  } else {
    device_api_info_label->setText(QString("%1.%2.%3")
                                       .arg(device_info_.api_ver.Major)
                                       .arg(device_info_.api_ver.Minor)
                                       .arg(device_info_.api_ver.SubMinor));
  }

  status = VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_HWPARAM, &device_info_.hw_param);
  if (status != VSCAN_ERR_OK) {
    reset_controls();
    QMessageBox::critical(this, "Error", "Can`t read device info!");
  } else {
    device_type_info_label->setText(
        vscom_info::get_type_string(device_info_.hw_param.HwType));
    device_sn_info_label->setText(
        QString::number(device_info_.hw_param.SerialNr));
    device_hw_info_label->setText(
        QString("%1.%2")
            .arg((device_info_.hw_param.HwVersion >> 4) & 0x0f)
            .arg((device_info_.hw_param.HwVersion) & 0x0f));
    device_sw_info_label->setText(
        QString("%1.%2")
            .arg((device_info_.hw_param.SwVersion >> 4) & 0x0f)
            .arg((device_info_.hw_param.SwVersion) & 0x0f));
  }

  VSCAN_Close(handle);
}

auto settings_data::get_can_br_string(uint8_t br) -> const char * {
  if (br >= can_br_100000 && br < can_br_unknown) {
    return can_br_strings[br];
  } else {
    return can_br_strings[can_br_unknown];
  }
}

auto settings_data::get_can_br_dec(uint8_t br) -> uint64_t {
  if (br >= can_br_100000 && br < can_br_unknown) {
    return can_br_dec[br];
  } else {
    return can_br_dec[can_br_unknown];
  }
}

auto settings_data::get_mode_string(uint8_t mode) -> const char * {
  if (mode >= mode_normal && mode < mode_unknown) {
    return mode_strings[mode];
  } else {
    return mode_strings[mode_unknown];
  }
}

void settings_data::set_default() {
  port_name = "";
  mode = mode_normal;
  br = can_br_100000;
}

auto vscom_info::get_type_string(uint8_t type) -> const char * {
  if (type > vscom_unknown && type <= vscom_bus) {
    return type_strings[type];
  } else {
    return type_strings[vscom_unknown];
  }
}

auto vscom_usb_can_plus::get_settings() -> settings_data const * {
  return &settings_;
}

auto vscom_usb_can_plus::get_cached_info() -> vscom_info const * {
  return &cache_info_;
}

void vscom_usb_can_plus::set_cached_info(vscom_info const *info) {
  cache_info_ = *info;
}

void vscom_usb_can_plus::change_settings(settings_data const *new_settings) {
  settings_ = *new_settings;
}

auto vscom_usb_can_plus::get_handle() -> VSCAN_HANDLE { return handle_; }

void vscom_usb_can_plus::set_handle(VSCAN_HANDLE h) { handle_ = h; }

auto vscom_usb_can_plus::get_msg(VSCAN_MSG &msg) -> bool {
  DWORD nr;

  VSCAN_STATUS status = VSCAN_Read(handle_, &msg, 1, &nr);
  if (status != VSCAN_ERR_OK) {
    return false;
  }

  return true;
}

void vscom_usb_can_plus::run() {
  sem_t sem;
  timespec ts;
  int ret;

  const auto nsec_timeout = 100'000'000l;
  const auto nsec_limit = 999'999'999l;

  sem_init(&sem, 0, 0);

  VSCAN_STATUS status = VSCAN_SetRcvEvent(handle_, &sem);
  if (status != VSCAN_ERR_OK) {
    emit error_read_thread(error_read_api);
  }

  while (!QThread::currentThread()->isInterruptionRequested()) {
    clock_gettime(CLOCK_REALTIME, &ts);

    ts.tv_nsec += nsec_timeout;
    if (ts.tv_nsec > nsec_limit) {
      ++ts.tv_sec;
      ts.tv_nsec -= nsec_limit;
    }

    ret = sem_timedwait(&sem, &ts);
    if (ret == -1) {
      if (errno == ETIMEDOUT) {
        continue;
      } else {
        emit error_read_thread(error_read_wait_msg);
        while (!QThread::currentThread()->isInterruptionRequested()) {
        }
      }
    } else {
      VSCAN_MSG msg{};

      if (get_msg(msg)) {
        emit msg_read(msg);
      } else {
        emit error_read_thread(error_read_msg);
      }
    }
  }

  sem_close(&sem);
  sem_destroy(&sem);
}

void vscom_usb_can_plus::start_read_thread() {
  if (isRunning()) {
    return;
  }

  start();
}

void vscom_usb_can_plus::stop_read_thread() {
  if (!isRunning()) {
    return;
  }

  quit();
  requestInterruption();

  while (isRunning()) {
  }
}

bool vscom_usb_can_plus::send_msg(VSCAN_MSG msg) {
  DWORD nw;

  VSCAN_STATUS status = VSCAN_Write(handle_, &msg, 1, &nw);
  if (status != VSCAN_ERR_OK) {
    return false;
  }

  status = VSCAN_Flush(handle_);
  if (status != VSCAN_ERR_OK) {
    return false;
  }

  return true;
}

auto vscan_msg_to_qstring(VSCAN_MSG msg) -> QString {
  QString qstring_msg;
  QTextStream qstring_stream(&qstring_msg);
  QString delim{" | "};

  qstring_stream << Qt::uppercasedigits << Qt::hex << "ID:" << msg.Id << delim
                 << Qt::dec
                 << "Ext:" << ((msg.Flags & VSCAN_FLAGS_EXTENDED) ? 1 : 0)
                 << delim
                 << "RTR:" << ((msg.Flags & VSCAN_FLAGS_REMOTE) ? 1 : 0)
                 << delim << "DLC:" << msg.Size << delim;

  for (auto i = 0; i < msg.Size; ++i) {
    qstring_stream
        << QString("%1 ").arg(msg.Data[i], 2, 16, QChar('0')).toUpper();
  }

  return qstring_msg;
}

} // namespace device

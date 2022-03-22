#pragma once

#include "vs_can_api.h"
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTextStream>
#include <QThread>
#include <QVBoxLayout>

#include <cstdint>

namespace device {

class vscom_usb_can_plus;

class settings_data {
public:
  enum : uint8_t {
    can_br_100000,
    can_br_125000,
    can_br_250000,
    can_br_500000,
    can_br_1000000,
    can_br_unknown
  };

  static constexpr const char *can_br_strings[]{"100000", "125000",  "250000",
                                                "500000", "1000000", "Unknown"};

  static constexpr uint64_t can_br_dec[]{100000, 125000,  250000,
                                         500000, 1000000, 100000};

  enum : uint8_t { mode_normal, mode_listen, mode_self_test, mode_unknown };

  static constexpr const char *mode_strings[]{"Normal", "Listen only",
                                              "Self test", "Unknown"};

  static auto get_can_br_string(uint8_t) -> const char *;
  static auto get_mode_string(uint8_t) -> const char *;
  static auto get_can_br_dec(uint8_t) -> uint64_t;

  void set_default();

  QString port_name{""};
  uint8_t mode{mode_normal};
  uint64_t br{can_br_100000};
  VSCAN_CODE_MASK code_mask{0x00000000, 0xFFFFFFFF};
  bool dual_filter{false};
};

class vscom_info {
public:
  VSCAN_HWPARAM hw_param;
  VSCAN_API_VERSION api_ver;

  enum : uint8_t {
    vscom_unknown,
    vscom_serial,
    vscom_usb,
    vscom_net,
    vscom_bus
  };

  static constexpr const char *type_strings[]{"Unknown", "Serial", "USB", "Net",
                                              "Bus"};

  static auto get_type_string(uint8_t) -> const char *;
};

using device_type = vscom_usb_can_plus;
using device_ptr = device_type *;
using device_info = vscom_info;

class settings_dialog : public QDialog {
  Q_OBJECT

public:
  settings_dialog(settings_data const *, device_info const *,
                  QWidget * = nullptr);
  settings_dialog(settings_dialog const &) = delete;
  settings_dialog &operator=(settings_dialog const &) = delete;
  settings_dialog(settings_dialog &&) = delete;
  settings_dialog &operator=(settings_dialog &&) = delete;
  ~settings_dialog() = default;

  void get_device_settings(settings_data *);
  void get_device_info(device_info *);

private slots:
  void search_device();
  void read_device_info();
  void change_device();

private:
  device_info device_info_;

private:
  void reset_controls();

private:
  QVBoxLayout *main_layout{nullptr};
  QVBoxLayout *port_main_layout{nullptr};
  QHBoxLayout *port_act_layout{nullptr};
  QGridLayout *port_device_info_layout{nullptr};
  QVBoxLayout *mode_layout{nullptr};
  QGridLayout *config_main_layout{nullptr};
  QVBoxLayout *baudrate_layout{nullptr};

  QGroupBox *port_group{nullptr};
  QGroupBox *config_group{nullptr};
  QGroupBox *mode_group{nullptr};

  QButtonGroup *radio_btn_mode_group{nullptr};

  QRadioButton *normal_mode_radio_btn{nullptr};
  QRadioButton *listen_mode_radio_btn{nullptr};
  QRadioButton *self_mode_radio_btn{nullptr};

  QCheckBox *debug_checkbox{nullptr};
  QCheckBox *hw_timestamps_checkbox{nullptr};

  QComboBox *port_combobox{nullptr};
  QComboBox *baudrate_combobox{nullptr};

  QPushButton *search_btn{nullptr};
  QDialogButtonBox *btn_box{nullptr};

  QLabel *device_api_label{nullptr};
  QLabel *device_api_info_label{nullptr};
  QLabel *device_type_label{nullptr};
  QLabel *device_type_info_label{nullptr};
  QLabel *device_sn_label{nullptr};
  QLabel *device_sn_info_label{nullptr};
  QLabel *device_sw_label{nullptr};
  QLabel *device_sw_info_label{nullptr};
  QLabel *device_hw_label{nullptr};
  QLabel *device_hw_info_label{nullptr};
  QLabel *baudrate_label{nullptr};
};

class vscom_usb_can_plus : public QThread {
  Q_OBJECT

public:
  vscom_usb_can_plus() = default;
  vscom_usb_can_plus(vscom_usb_can_plus const &) = delete;
  vscom_usb_can_plus &operator=(vscom_usb_can_plus const &) = delete;
  vscom_usb_can_plus(vscom_usb_can_plus &&) = delete;
  vscom_usb_can_plus &operator=(vscom_usb_can_plus &&) = delete;

  enum { error_read_api = 1, error_read_wait_msg, error_read_msg };

  void change_settings(settings_data const *);
  auto get_settings() -> settings_data const *;
  auto get_cached_info() -> vscom_info const *;
  void set_cached_info(vscom_info const *);
  void set_handle(VSCAN_HANDLE);
  auto get_handle() -> VSCAN_HANDLE;

  bool send_msg(VSCAN_MSG);

  virtual void run() override;

public slots:
  void start_read_thread();
  void stop_read_thread();

signals:
  void msg_read(VSCAN_MSG);
  void error_read_thread(int);

private:
  auto get_msg(VSCAN_MSG &) -> bool;

private:
  settings_data settings_{};
  VSCAN_HANDLE handle_{0};
  vscom_info cache_info_{};
};

auto vscan_msg_to_qstring(VSCAN_MSG) -> QString;

} // namespace device

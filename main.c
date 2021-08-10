import time
from threading import Thread
from tkinter.ttk import *
import serial
import tkinter as tk
import openpyxl
from openpyxl import load_workbook
import copy
from tkinter import messagebox
# import visa
from tkinter import *
from tkinter import ttk
from tkinter import scrolledtext
from tables import *
# from MT1000A_Class import *
from MTS5800_Class import *
import telnetlib

usb_serial = "COM4"#Takis
eth_serial = "COM1"#Takis

# usb_serial = "COM3"  #hackeroni
# eth_serial = "COM8"  #hackeroni

# usb_serial = "COM1"  #hackeroni
# eth_serial = "COM8"  #hackeroni

ip_mts5800 = "192.168.200.2"
ip_mt1000a = "192.168.200.6"
ip_33220a = "192.168.200.3"
ip_fse = "192.168.1.10"
red_color = '#fc2626'
green_color = '#00ff04'
yellow_color = '#e5f307'

serbuffer = ""

fpga_result_list = []
led_list = []
sync_in_out_results = []

#variables
diag_result = 0
diag_login_output = 0
diag_OK_list = 0
diag_PASSED_list = 0
label_choice = ""
fse_login_output = 0
fse_error_outout = 0
fse_loop_count = 0
fse_pass_output = 0

default_mac_address = "00:00:33:33:44:44"
mac_addresses_path = r"C:\G5DualBS_Files\G5DualBS_Mac_Addresses\G5DualBS_MAC_ADDRESSES.txt"
mac_addresses_per_card = 10
ycode_list = ["Y00-750/13.L2", "Y00-751/02.L2", "3", "4", "5"]


g5dualbs_fse_version = r"7.4.0_2400_A.6.1.0.build_11"
ulgx80_fse_version = r"8.0.99_4231_R.3.0.0.build_06x"
xpic_bitfile = "gigas_test_xpic.crbf"
traffic_test_bitfile = "gigas_test.crbf"


#paths
table_path = r'C:\G5DualBS_Files\G5DualBS_Table'
ulgx80_fse_path = r'C:\G5DualBS_Files\ULGX80_FSE'
g5dualbs_fse_path = r'C:\G5DualBS_Files\G5DualBS_FSE'
results_path = r'C:\G5DualBS_Files\G5DualBS_Results'
results_file = "G5DualBS_Results.xlsx"
xpic_path = r'C:\G5DualBS_Files\ULGX80_FSE'
traffic_test_bitfile_path = r'C:\G5DualBS_Files\ULGX80_FSE'

ulgx80_license = "000559FFFFFF-04022020_131800000.license"
g5dualbs_license = "000559FFFFFF-12042021-135300000.license"


maxTemp = 60

datetime_string = ['-']


datetime_output_results = datetime.now().strftime('%d.%m.%Y , %H:%M:%S')
datetime_RTC = datetime.now().strftime('%H:%M:%S')
exceltime = datetime.now().strftime('%H:%M:%S')
date = datetime.now().strftime('%d.%m.%Y')

GroupTests = ['TestGroup A', 'TestGroup B', 'TestGroup C', 'TestGroup D']


def timeResult():
    exceltime = datetime.now().strftime('%H:%M:%S')
    date = datetime.now().strftime('%d.%m.%Y')
    resultList[0] = date
    resultList[1] = exceltime


def timeResult_end():
    endtime = datetime.now().strftime('%H:%M:%S')
    resultList[2] = endtime


testList = ['SERIAL NUMBER', 'YCODE NUMBER', 'MAC ADDRESS', 'DIAG TEST' 'TEMPERATURE', 'RTC', 'ALARM63', 'ALARM64',
            'ALARM65', 'ALARM66',
            'ALARM67', 'ALARM68', 'ALARM63_SHORTED', 'ALARM64_SHORTED', 'ALARM65_SHORTED', 'ALARM66_SHORTED',
            'ALARM67_SHORTED', 'ALARM68_SHORTED']

resultList = ['NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL',
              'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL',
              'NULL', 'NULL', 'NULL', 'NULL', 'NULL',
              'NULL', 'NULL', 'NULL', 'NULL', 'NULL',
              'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL']

resultList2 = ['NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL',
               'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL',
               'NULL', 'NULL', 'NULL', 'NULL', 'NULL',
               'NULL', 'NULL', 'NULL', 'NULL', 'NULL',
               'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL', 'NULL']



# variables
sncode = StringVar
ycode = StringVar
testerIDcode = StringVar
userIDcode = StringVar
ImgResultID = 2


def deselect_all():
    checkbox_Diag.deselect()
    checkbox_HWID.deselect()
    checkbox_fpga.deselect()
    checkbox_MAC.deselect()
    checkbox_RTC.deselect()
    checkbox_PLL.deselect()
    checkbox_telnet.deselect()
    checkbox_YCode_test.deselect()
    checkbox_YSerial_test.deselect()
    checkbox_Temp.deselect()
    checkbox_traffic_test.deselect()
    checkbox_license.deselect()
    checkbox_serial_eth_interface.deselect()
    checkbox_sync_in_out.deselect()
    checkbox_check_system.deselect()
    checkbox_fse.deselect()
    checkbox_dPLL.deselect()
    checkbox_set_normal_mode.deselect()
    checkbox_set_test_mode.deselect()
    checkbox_xpic_test.deselect()
    checkbox_sync_in_out.deselect()
    checkbox_table_test.deselect()
    checkbox_fse_swap.deselect()
    checkbox_fse_check.deselect()
    checkbox_norfu.deselect()
    checkbox_license_rtc_g5dualbs.deselect()
    checkbox_rm_norfu.deselect()


def select_all():
    checkbox_Diag.select()
    checkbox_HWID.select()
    checkbox_fpga.select()
    checkbox_MAC.select()
    checkbox_RTC.select()
    checkbox_PLL.select()
    checkbox_telnet.select()
    checkbox_YCode_test.select()
    checkbox_YSerial_test.select()
    checkbox_Temp.select()
    checkbox_traffic_test.select()
    checkbox_license.select()
    checkbox_serial_eth_interface.select()
    checkbox_sync_in_out.select()
    checkbox_check_system.select()
    checkbox_dPLL.select()
    checkbox_set_normal_mode.select()
    checkbox_set_test_mode.select()
    checkbox_xpic_test.select()
    checkbox_sync_in_out.select()
    checkbox_table_test.select()
    checkbox_fse_swap.select()
    checkbox_fse_check.select()
    checkbox_norfu.select()
    checkbox_license_rtc_g5dualbs.select()
    checkbox_rm_norfu.select()


def warning_message(title, message):
    messagebox.showwarning(title, message)


def warning_reboot():
    messagebox.showwarning("Warning", "Press ok first, then reset the BBU!")


def tftp_path_select():
    messagebox.showwarning("Warning",
                           r"On Tftpd32 choose directory C:\Omnibas_10P\10P_fse\omnibas_10p_5.8.0_2431_R.1.0.0.build_25 and IP: " + ip_fse)


def warning_second_com():
    messagebox.showerror("Error", "Could not open port 'COM1', please reset the BBU!")


def reset_gui():
    print(Indicator_label.config(text=f"{''}", bg='white'))
    print(MAC_label.config(text=f"{''}", bg='white'))
    print(mac_info_output_label.config(text=f"{''}"))
    print(Diag_label.config(text=f"{''}", bg='white'))
    print(fse_label.config(text=f"{''}", bg='white'))
    print(license_label.config(text=f"{''}", bg='white'))
    print(License_info_output_label.config(text=f"{''}"))
    print(management_port_label.config(text=f"{''}", bg='white'))
    # print(serial_eth_interface_label.config(text=f"{''}", bg='white'))
    print(Yserial_test_label.config(text=f"{''}", bg='white'))
    print(Ycode_output_label.config(text=f"{''}"))
    print(ycode_test_label.config(text=f"{''}", bg='white'))
    print(Yserial_output_label.config(text=f"{''}"))
    print(HWID_label.config(text=f"{''}", bg='white'))
    print(Temp_label.config(text=f"{''}", bg='white'))
    print(RTC_label.config(text=f"{''}", bg='white'))
    print(PLL_label.config(text=f"{''}", bg='white'))
    print(p110_telnet_label.config(text=f"{''}", bg='white'))
    print(fpga_program_label.config(text=f"{''}", bg='white'))
    print(Traffic_label.config(text=f"{''}", bg='white'))
    print(fpga_test_label.config(text=f"{''}", bg='white'))
    print(mac_info_output_label.config(text=f"{''}"))
    print(fse_version_info_output_label.config(text=f"{''}"))
    print(remaining_mac_info_output_label.config(text=f"{''}"))
    print(jesd_label.config(text=f"{''}", bg='white'))
    print(check_system_label.config(text=f"{''}", bg='white'))
    print(RTC_info_output_label.config(text=f"{''}"))
    print(sync_in_out_label.config(text=f"{''}", bg='white'))
    print(License_info_output_label.config(text=f"{''}"))
    print(TENG_info_output_label.config(text=f"{''}"))
    print(fpga_test_label.config(text=f"{''}", bg='white'))
    print(dPLL_label.config(text=f"{''}", bg='white'))
    # print(led_serial_eth_interface_label.config(text=f"{''}", bg='white'))
    print(set_normal_mode_label.config(text=f"{''}", bg='white'))
    print(set_test_mode_label.config(text=f"{''}", bg='white'))
    print(table_label.config(text=f"{''}", bg='white'))
    print(xpic_label_program.config(text=f"{''}", bg='white'))
    print(g5dualbs_RTC_label.config(text=f"{''}", bg='white'))
    print(g5dualbs_license_label.config(text=f"{''}", bg='white'))
    print(fse_swap_label.config(text=f"{''}", bg='white'))
    print(xpic_label_test.config(text=f"{''}", bg='white'))
    print(fse_check_label.config(text=f"{''}", bg='white'))
    print(rm_norfu_label.config(text=f"{''}", bg='white'))
    print(norfu_label.config(text=f"{''}", bg='white'))
    cb.set("")


def ulgx80_tests_result():
    ulgx80_tests = resultList[0:33]
    print(ulgx80_tests)
    if 'FAIL' in ulgx80_tests:
        print(Indicator_label.config(text=f"{'FAIL'}", bg=red_color))
        resultList[4] = 'FAIL'
        resultList[4] = 'FAIL'
        return 'FAIL'
    else:
        print(Indicator_label.config(text=f"{'PASS'}", bg=green_color))
        resultList[4] = 'PASS'
        return 'PASS'


def ulgx80_testmode_tests_result():
    ulgx80_tests = resultList[0:29]
    print(ulgx80_tests)
    if 'FAIL' in ulgx80_tests:
        print(Indicator_label.config(text=f"{'FAIL'}", bg=red_color))
        resultList[4] = 'FAIL'
        resultList[4] = 'FAIL'
        return 'FAIL'
    else:
        print(Indicator_label.config(text=f"{'PASS'}", bg=green_color))
        resultList[4] = 'PASS'
        return 'PASS'

def final_result():
    print(resultList)
    for result in resultList:
        result = re.findall(r'FAIL', result)
        if result:
            resultList[4] = 'FAIL'
            print(Indicator_label.config(text=f"{'Test Failed'}", bg=red_color))
            break
        else:
            print(Indicator_label.config(text=f"{'Test Passed'}", bg=green_color))
            resultList[4] = 'PASS'



def createFolder(directory):
    try:
        if not os.path.exists(directory):
            os.makedirs(directory)
    except OSError:
        print('Error: Creating directory ' + directory)


def save_results():
    # createFolder('C:\Omnibas_10P\\10P_Results')
    os.chdir(results_path)
    while True:
        try:
            wb = load_workbook(results_file)
            ws = wb.active
            ws.append(resultList)
            wb.save(results_file)
            reset_list()
            print(resultList)
            break
        except FileNotFoundError:
            wb = openpyxl.Workbook()
            sheet = wb['Sheet']
            ws = wb.active
            for col, val in enumerate(testList, start=1):
                sheet.cell(row=1, column=col).value = val
            ws.append(resultList)
            wb.save(results_file)
            reset_list()


def save_txt_results():
    os.chdir(results_path + r'\TXT Results')
    with open(resultList[5] + '.TXT', "a") as f:
        f.write("""Test Start : {date} {start_time} 	
Serial Number : {serial_number_head}
Y-Code : {ycode_number_head}
Model : G5 Dual BS
Tester ID : 
Data Source : 
Operator ID : {user_id}


Name ; Measurement ; Unit ; Low Limit ; High Limit ; Result
YSerial ; {serial_number} ; NULL ; NULL ; NULL ; {yserial_test}
Ycode ; {ycode_number} ; NULL ; NULL ; NULL ; {ycode_test}
Mac Address ; {mac_address} ; NULL ; NULL ; PASS
Diag Test ; NULL ; NULL ; NULL ; NULL ; {diag_test}
Management Port ; NULL ; NULL ; NULL ; NULL ; {mng_port_test}
RTC ; NULL ; NULL ; NULL ; NULL ; PASS {rtc_test}
Hardware ID ; NULL ; NULL ; NULL ; NULL ; {hwid_test}
PLL ; NULL ; 0x1 ; NULL ; NULL ; {pll_test}
Temperature ; NULL ; NULL ; NULL ; NULL ; {temp_test}
DPLL Reset ; NULL ; NULL ; NULL ; NULL ; {dpll_reset}
FPGA Test ; NULL ; NULL ; NULL ; NULL ; {fpga_test}
Sync Acquired Port 1 ; NULL ; NULL ; NULL ; NULL ; {sync_acq_p1}
Link Active Port 1 ; NULL ; NULL ; NULL ; NULL ; {link_active_p1}
Frame Detect Port 1 ; NULL ; NULL ; NULL ; NULL ; {frame_detect_p1}
Pattern Sync Port 1 ; NULL ; NULL ; NULL ; NULL ; {pattern_sync_p1}
Sync Loss Port 1 ; ; NULL ; NULL ; NULL ; NULL ; {sync_loss_p1}
Link Loss Port 1 ; NULL ; NULL ; NULL ; NULL ; {link_loss_p1}
FCS Errored Frames Port 1 ; NULL ; NULL ; NULL ; NULL ; {fcs_p1}
Bit Errors Port 1 ; NULL ; NULL ; NULL ; NULL ; {bit_errors_p1}
Jabbers Port 1 ; NULL ; NULL ; NULL ; NULL ; {jabbers_p1}
XPIC Test ; NULL ; NULL ; NULL ; NULL ; {xpic_test}
Table Upload ; NULL ; NULL ; NULL ; NULL ; {table_upload}
P110 Telnet Test ; NULL ; NULL ; NULL ; NULL ; {p110_telnet_test}
Sync In/Out Test ; NULL ; NULL ; NULL ; NULL ; {sync_in_out_test}
License ULGX80 ; NULL ; NULL ; NULL ; NULL ; {set_license_ulgx80}
Check System ; NULL ; NULL ; NULL ; NULL ; {check_system}
FSE G5DualBS ; NULL ; NULL ; NULL ; NULL ; {fse_g5dualbs}
Create Norfu  ; NULL ; NULL ; NULL ; NULL ; {create_norfu}
License - RTC G5DualBS ; NULL ; NULL ; NULL ; NULL ; {rtc_license_g5}
Remove Norfu  ; NULL ; NULL ; NULL ; NULL ; {rm_norfu}

Test End : {date} {end_time}
FINAL RESULT : {final_result}

------------------------------------------------------------------

""".format(date=resultList[0],
           start_time=resultList[1],
           end_time=resultList[2],
           user_id=resultList[3],
           final_result=resultList[4],
           serial_number_head=resultList[5],
           serial_number=resultList[5],
           ycode_number=resultList[6],
           ycode_number_head=resultList[6],
           mac_address=resultList[7],
           diag_test=resultList[8],
           mng_port_test=resultList[9],
           yserial_test=resultList[10],
           ycode_test=resultList[11],
           hwid_test=resultList[12],
           temp_test=resultList[13],
           rtc_test=resultList[14],
           pll_test=resultList[15],
           dpll_reset=resultList[16],
           fpga_test=resultList[17],
           sync_acq_p1=resultList[18],
           link_active_p1=resultList[19],
           frame_detect_p1=resultList[20],
           pattern_sync_p1=resultList[21],
           sync_loss_p1=resultList[22],
           link_loss_p1=resultList[23],
           fcs_p1=resultList[24],
           bit_errors_p1=resultList[25],
           jabbers_p1=resultList[26],
           xpic_test=resultList[27],
           table_upload=resultList[28],
           p110_telnet_test=resultList[29],
           sync_in_out_test=resultList[30],
           set_license_ulgx80=resultList[31],
           check_system=resultList[32],
           fse_g5dualbs=resultList[33],
           rtc_license_g5=resultList[34],
           create_norfu=resultList[35],
           rm_norfu=resultList[36]))
        f.close()



class SerialClass:
    def __init__(self, com, baudrate):
        try:
            self.com = com
            self.baudrate = baudrate
            self.ser = serial.Serial(com, timeout=1)
            self.ser.baudrate = baudrate
        except Exception as e:
            print("Error: " + str(e))

    def send_ser_cmd(self, cmd):
        cmd = cmd + '\r'
        self.ser.write(cmd.encode())

    def winmon_login(self):
        try:
            # port_reset()
            self.ser.write(b'\r')
            while True:
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                time.sleep(0.1)
                abort_command = '\x03'.encode()
                if "Hit CTRL-C to abort" in output:
                    self.ser.write(abort_command)
                    # print(fse_label.config(text=f"{'Running'}", bg=green_color))
                if "WinMon>" in output:
                    # print(Indicator_label.config(text=f"{'WinMon Entered'}"))
                    break
                if "OmniBAS>" in output:
                    self.send_ser_cmd('reboot system')
                    time.sleep(0.5)
                    self.send_ser_cmd('y')
                    continue
                # if "omnibas login" in output:
                #     admin_login()
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))
            print(Indicator_label.config(text=f"{'No serial connection, please connect the BBU!'}", bg=red_color))

    def send_cmd(self, command):
        try:
            command = command + '\r'
            self.ser.write(command.encode())
            # output = self.ser.read(1000).decode()
            # print(output)
        except Exception as e:
            print("type error: " + str(e))

    def port_reset(self):
        try:
            self.ser.close()
            time.sleep(0.5)
            self.ser.open()
        except Exception as e:
            print("type error: " + str(e))
            write_serial_output(str(e))

    def admin_login(self, special_exception='help please'):
        try:
            self.ser.write(b'\r')
            while True:
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                if "ul-gx80 login:" in output:
                    time.sleep(1)
                    self.ser.write("admin\r".encode())
                if "[root@omnibas:~]#" in output:
                    time.sleep(0.1)
                    self.ser.write('exit\r'.encode())
                if 'WinMon>' in output:
                    self.ser.write('boot\r'.encode())
                if "omnibas login:" in output:
                    time.sleep(10)
                    self.ser.write(b"admin\r")
                if "osdr-g5-dual-fdd-pmp login:" in output:
                    time.sleep(17)
                    self.ser.write(b"admin\r")
                elif "Username:" in output:
                    time.sleep(10)
                    self.ser.write(b"admin\r")
                elif 'Password:' in output:
                    time.sleep(7)
                    self.ser.write(b"admin\r")
                elif 'UL-GX80>' in output:
                    break
                elif 'OSDR-G5-DUAL-FDD-PMP>' in output:
                    return "admin"
                elif special_exception in output:
                    break
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def root_login(self, special_exception='help please'):
        try:
            self.ser.write(b'\r')
            while True:
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                if "press [Enter] for test mode" in output:
                    self.ser.write("\r\n".encode("ascii", "ignore"))
                if "ul-gx80 login:" in output:
                    time.sleep(1)
                    self.ser.write("root\r".encode())
                if 'WinMon>' in output:
                    self.ser.write('boot\r'.encode())
                elif "osdr-g5-dual-fdd-pmp login:" in output:
                    return 1
                elif "OSDR-G5-DUAL-FDD-PMP>" in output:
                    return 1
                if "omnibas login:" in output:
                    time.sleep(10)
                    self.ser.write(b"admin\r")
                elif "Username:" in output:
                    time.sleep(10)
                    self.ser.write(b"root\r")
                elif 'Password:' in output:
                    time.sleep(2)
                    self.ser.write(b"p\r")
                elif 'UL-GX80>' in output:
                    break
                elif '[root@ul-gx80:~]#' in output:
                    return 'root'
                elif '[root@ul-gx80:/intracom/' in output:
                    time.sleep(0.5)
                    self.send_ser_cmd('cd')
                elif special_exception in output:
                    time.sleep(0.5)
                    self.send_ser_cmd('cd')
                    break
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def root_login_g5(self, special_exception='help please'):
        try:
            self.ser.write(b'\r')
            while True:
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                # if "press [Enter] for test mode" in output:
                #     self.ser.write("\r\n".encode("ascii", "ignore"))
                if "Login incorrect" in output:
                    self.admin_login()
                    return "admin"
                if "ul-gx80 login:" in output:
                    time.sleep(1)
                    self.ser.write("root\r".encode())
                if 'WinMon>' in output:
                    self.ser.write('boot\r'.encode())
                elif "osdr-g5-dual-fdd-pmp login:" in output:
                    time.sleep(15)
                    self.send_ser_cmd('root')
                elif "OSDR-G5-DUAL-FDD-PMP>" in output:
                    return "admin"
                elif "[root@osdr-g5-dual-fdd-pmp:~]#" in output:
                    break
                if "omnibas login:" in output:
                    time.sleep(10)
                    self.ser.write(b"admin\r")
                elif "Username:" in output:
                    time.sleep(10)
                    self.ser.write(b"root\r")
                elif 'Password:' in output:
                    time.sleep(2)
                    self.ser.write(b"p\r")
                elif 'UL-GX80>' in output:
                    break
                elif '[root@ul-gx80:~]#' in output:
                    return 'root'
                elif '[root@ul-gx80:/intracom/' in output:
                    time.sleep(0.5)
                    self.send_ser_cmd('cd')
                elif special_exception in output:
                    time.sleep(0.5)
                    self.send_ser_cmd('cd')
                    break
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def mac_addresses(self):
        with open(mac_addresses_path, "r") as f:
            lines = open(mac_addresses_path).readlines()
            content = [x.strip() for x in lines] # ka8arizei to string apo newline
            print(content)
            if len(content) < mac_addresses_per_card:
                print('Need more Mac Addresses')
                return 'Need more Mac Addresses'
            elif len(content) >= mac_addresses_per_card:
                first_line = f.readline()
                remaining_mac = str(int(((len(content)) / mac_addresses_per_card)))
                write_serial_output('Remaining Mac Addresses:' + remaining_mac + '\n')
                print(remaining_mac_info_output_label.config(text=f"{remaining_mac}"))
                mac_formatted = ""
                for i in range(0, 12, 2):
                    mac_formatted += first_line[i:i + 2] + ":"
                mac_formatted = mac_formatted[:-1]
                print("s=", mac_formatted)
                print('MAC Address:' + mac_formatted)
                open(mac_addresses_path, 'w').writelines(lines[mac_addresses_per_card:])
                return mac_formatted

    def check_mac(self):
        self.winmon_login()
        try:
            self.ser.write(b'env\r')
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
            mac_regex = re.findall(r'(?:[0-9a-fA-F]:?){12}', output)
            # warning_message("Warning", 'MAC Address set : ' + mac_regex[0] + '\n')
            if mac_regex[0] != default_mac_address:
                result = messagebox.askquestion("Warning", 'There is another MAC Address set : ' + mac_regex[0] + '. Do You want to ovewrite it?')
                if result == 'yes':
                    self.set_mac()
                elif result == 'no':
                    print(mac_info_output_label.config(text=f"{mac_regex[0]}"))
                    print(MAC_label.config(text=f"{'MAC Set'} ", bg=green_color))
                    pass
            else:
                self.set_mac()
        except IndexError:
            mac_return = self.set_mac()
            return mac_return
        except Exception as e:
            print("type error: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))
            print(Indicator_label.config(text=f"{str(e)}", bg=red_color))
            print(MAC_label.config(text=f"{'FAIL'}", bg=red_color))

    def set_mac(self):
        try:
            self.ser.write(('setenv BOARD_MAC_ETHX ' + ' ' + self.mac_addresses() + '\r').encode())
            time.sleep(0.7)
            self.ser.write(b'env\r')
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
            mac_regex = re.findall(r'(?:[0-9a-fA-F]:?){12}', output)
            write_serial_output('MAC Address set : ' + mac_regex[0] + '\n')
            if mac_regex:
                print('PASS')
                print(mac_info_output_label.config(text=f"{mac_regex[0]}"))
                print(MAC_label.config(text=f"{'MAC Set'} ", bg=green_color))
                return mac_regex[0]
            else:
                print(MAC_label.config(text=f"{'FAIL'} ", bg='##fc2626'))
                return 'FAIL'
        except Exception as e:
            print("type error: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))
            print(Indicator_label.config(text=f"{str(e)}", bg=red_color))
            print(MAC_label.config(text=f"{'FAIL'}", bg=red_color))

    def mac_addresses_result(self):
        state = mac_address_state.get()
        if state == 1:
            print(MAC_label.config(text=f"{'Running...'}", bg=yellow_color))
            resultList[7] = self.check_mac()
        else:
            pass

    def set_test_mode(self, test=1):
        if test == 1:
            state = set_test_mode_state.get()
            if state == 1:
                print(set_test_mode_label.config(text=f"{'Running...'}", bg=yellow_color))
                login = self.root_login()
                if login == 'root':
                    self.send_ser_cmd('ll /intracom/app/test_mode')
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    write_serial_output(output)
                    print(output)
                    if 'No such file or directory' in output:
                        commandlist = ['cd /intracom/app', 'echo > test_mode', 'cd', 'reboot']
                        for command in commandlist:
                            self.send_ser_cmd(command)
                            time.sleep(1)
                            output = self.ser.read(1000).decode('UTF-8', 'ignore')
                            write_serial_output(output)
                        i = 0
                        while i < 200:
                            i += 0.2
                            time.sleep(0.2)
                            output = self.ser.read(1000).decode('UTF-8', 'ignore')
                            write_serial_output(output)
                            if 'TERM signal' in output:
                                break
                            elif i >= 199:
                                break

                        time.sleep(1)
                        self.root_login()
                        time.sleep(1)
                        self.send_ser_cmd('ll /intracom/app/test_mode')
                        time.sleep(1)
                        output = self.ser.read(1000).decode('UTF-8', 'ignore')
                        if 'No such file or directory' in output:
                            print(set_test_mode_label.config(text=f"{'Not Set'}", bg=red_color))
                        else:
                            print(set_test_mode_label.config(text=f"{'Set'} ", bg=green_color))
                    elif '-rw-r--r-- 1 root root' in output:
                        print(set_test_mode_label.config(text=f"{'Set'} ", bg=green_color))
                else:
                    pass
            else:
                pass
        elif test == 0:
            self.root_login()
            self.send_ser_cmd('ll /intracom/app/test_mode')
            time.sleep(1)
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            write_serial_output(output)
            print(output)
            if 'No such file or directory' in output:
                commandlist = ['cd /intracom/app', 'echo > test_mode', 'cd', 'reboot']
                for command in commandlist:
                    self.send_ser_cmd(command)
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    write_serial_output(output)
                i = 0
                while i < 200:
                    i += 0.2
                    time.sleep(0.2)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    write_serial_output(output)
                    if 'TERM signal' in output:
                        break
                    elif i >= 199:
                        break

                time.sleep(1)
                self.root_login()
                time.sleep(1)
                self.send_ser_cmd('ll /intracom/app/test_mode')
                time.sleep(1)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                if 'No such file or directory' in output:

                    print("No such file or directory")
                else:
                   pass
            elif '-rw-r--r-- 1 root root' in output:
                pass

    def prompt_check(self, prompt):
        try:
            self.send_ser_cmd("")
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
            if prompt in output:
                return 1
            else:
                return 0
        except Exception as E:
            print(E)

    def remove_test_mode(self):
        # self.root_login()
        self.send_ser_cmd('ll /intracom/app/test_mode')
        output = self.ser.read(1000).decode('UTF-8', 'ignore')
        write_serial_output(output)
        if '-rw-r--r-- 1 root root' in output:
            self.send_ser_cmd('rm /intracom/app/test_mode')
            time.sleep(1)
            # self.send_ser_cmd('reboot')
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            write_serial_output(output)
            self.send_ser_cmd('ll /intracom/app/test_mode')
            if '-rw-r--r-- 1 root root' in output:
                return 0
            else:
                warning_message('Warning', 'Hardware Reboot the card and then press OK!')
                print(set_normal_mode_label.config(text=f"{'Running...'}", bg=yellow_color))
                self.admin_login("Not starting Web Server.")
                graphics_delay(set_normal_mode_label, 31)
                return 1

    def set_normal_mode(self):
            state = set_normal_mode_state.get()
            if state == 1:
                print(set_normal_mode_label.config(text=f"{'Running...'}", bg=yellow_color))
                self.send_ser_cmd('ll /intracom/app/test_mode')
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                write_serial_output(output)
                if '-rw-r--r-- 1 root root' in output:
                        self.send_ser_cmd('rm /intracom/app/test_mode')
                        time.sleep(1)
                        # self.send_ser_cmd('reboot')
                        output = self.ser.read(1000).decode('UTF-8', 'ignore')
                        write_serial_output(output)
                        self.send_ser_cmd('ll /intracom/app/test_mode')
                        if '-rw-r--r-- 1 root root' in output:
                            print("HELP GOD")
                        else:
                            warning_message('Warning', 'Hardware Reboot the card and then press OK!')
                            self.admin_login("Not starting Web Server.")
                            # warning_message("Warning", "Wait 30 seconds!")
                            graphics_delay(set_normal_mode_label, 31)
                            telnet = TelnetClass('192.168.1.100', 23, 3)
                            login = telnet.admin_login_check()
                            if login == "Set":
                                print(set_normal_mode_label.config(text=f"{'Set'} ", bg=green_color))
                                return "PASS"
                elif 'No such file or directory' in output:
                    telnet = TelnetClass('192.168.1.100', 23, 3)
                    login = telnet.admin_login_check()
                    if login == "Set":
                        print(set_normal_mode_label.config(text=f"{'Set'} ", bg=green_color))
                        return "PASS"
                    else:
                        print(set_normal_mode_label.config(text=f"{'Not set'} ", bg=red_color))
                        warning_message('Warning', 'Hardware Reboot the card and then press OK!')
                        self.admin_login("Not starting Web Server.")
                        login = telnet.admin_login_check()
                        if login == "Set":
                            print(set_normal_mode_label.config(text=f"{'Set'} ", bg=green_color))
                            return "PASS"
                else:
                    # self.admin_login("Not starting Web Server.")
                    telnet = TelnetClass('192.168.1.100', 23, 3)
                    login = telnet.admin_login_check()
                    if login == "Set":
                        print(set_normal_mode_label.config(text=f"{'Set'} ", bg=green_color))
                        return "PASS"
                    else:
                        print(set_normal_mode_label.config(text=f"{'Not set'} ", bg=red_color))
                        warning_message('Warning', 'Hardware Reboot the card and then press OK!')
                        self.root_login()
                        test_mode = self.remove_test_mode()
                        if test_mode == 0:
                            print(set_normal_mode_label.config(text=f"{'Not set'} ", bg=red_color))
                        elif test_mode == 1:

                            telnet = TelnetClass('192.168.1.100', 23, 3)
                            login = telnet.admin_login_check()
                            if login == "Set":
                                print(set_normal_mode_label.config(text=f"{'Set'} ", bg=green_color))
                                return "PASS"
                        else:
                            print(set_normal_mode_label.config(text=f"{'Not set'} ", bg=red_color))

    def diag_test(self):
        state = diag_state.get()
        if state == 1:
            self.winmon_login()
            time.sleep(2)
            self.ser.write('diag\r'.encode())
            output = self.ser.read(100).decode('UTF-8', 'ignore')
            write_serial_output(output)
            self.diag_test_v2()
        else:
            pass

    def diag_test_v2(self):
        # self.winmon_login()
        # time.sleep(1)
        DDRA_list = []
        DDRB_list = []
        DDRC_list = []
        DDRInternal_list = []
        global diag_OK_list
        global diag_PASSED_list
        global diag_login_output
        global diag_result
        try:

            while True:
                #output = self.ser.read().decode('UTF-8', 'ignore')
                output = self.ser.read()
                print(len(output))
                if len(output) == 0:
                    break

                global serbuffer

                if output == b'\r':
                    output = b''

                if output == b'\n':
                    serbuffer += "\n"
                    print(serbuffer)
                    write_serial_output(serbuffer)
                    serbuffer = ""
                else:
                    serbuffer += output.decode('UTF-8', 'ignore')
                    # print(serbuffer)
                    # write_serial_output()(serbuffer)
                if "WinMon>" in serbuffer:
                    diag_login_output = 1
                    write_serial_output("WinMon>")
                    break
                # if "OK" in serbuffer:
                #     diag_OK_list.append("OK")
                #     print(diag_OK_list)
                # if "PASSED" in serbuffer:
                #     diag_PASSED_list.append("PASSED")
                #     print(diag_PASSED_list)
                if "FAILED" in serbuffer:
                    diag_result = 1
            if diag_login_output == 0:
                master.after(10, self.diag_test_v2())
            elif diag_login_output == 1:
                if diag_result == 0:
                    print(Diag_label.config(text=f"{'PASS'}", bg=green_color))
                    resultList[8] = 'PASS'
                elif diag_result == 1:
                    print(Diag_label.config(text=f"{'FAIL'}", bg=red_color))
                    resultList[8] = 'FAIL'
                pass
        except Exception as e:
            print(e)

    def diag_thread(self):
        state = diag_state.get()
        if state == 1:
            print(Diag_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.winmon_login()
            self.diag_test()
        else:
            pass

    def fse(self, fse_path):
        global serbuffer
        try:
            self.winmon_login()
            os.chdir(fse_path)
            os.startfile(fse_path + r"\tftpd32.EXE")
            time.sleep(3)
            self.ser.write(b'fse 192.168.1.10 !nodiag \r')
            output = self.ser.read(1000).decode()
            print(output)
            write_serial_output(output)
            time.sleep(2)
            self.send_ser_cmd('1')
            output = self.ser.read(1000).decode()
            print(output)
            write_serial_output(output)
            time.sleep(0.5)
            serbuffer = ""
            fse_result = self.fse_loop()
            print("FSE RESULT :" + str(fse_result))
            os.system('cmd /c TASKKILL /IM tftpd32.EXE')
        except Exception as e:
            print(e)

    def fse_ULGX80(self):
        reset_variables()
        global serbuffer
        global label_choice
        global fse_pass_output
        label_choice = "ULGX80"
        state = fse_state.get()
        if state == 1:
            print(fse_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.fse(ulgx80_fse_path)
            if fse_pass_output == 1:
                print(fse_label.config(text=f"{'PASS'}", bg=green_color))
            else:
                print(fse_label.config(text=f"{'FAIL'}", bg=red_color))

        else:
            pass

    def fse_G5DualBS(self):
        global serbuffer
        global label_choice
        reset_variables()
        label_choice = "G5DualBS"
        print(fse_swap_label.config(text=f"{'Running...'}", bg=yellow_color))
        self.winmon_login()
        self.fse(g5dualbs_fse_path)
        if fse_pass_output == 1:
            print(fse_swap_label.config(text=f"{'PASS'}", bg=green_color))
            resultList[33] = "PASS"
        else:
            print(fse_swap_label.config(text=f"{'FAIL'}", bg=red_color))
            resultList[33] = "FAIL"

    def fse_loop(self):
        global fse_loop_count
        global fse_pass_output
        global fse_error_outout
        global serbuffer
        try:
            while True:
                fse_loop_count += 1
                output = self.ser.read()
                byte_count = len(output)
                print(str(fse_loop_count) + ": " + str(byte_count))
                if len(output) == 0:
                    break

                if output == b'\r':
                    output = b''

                if output == b'\n':
                    serbuffer += "\n"
                    print(serbuffer)
                    write_serial_output(serbuffer)
                    serbuffer = ""
                else:
                    serbuffer += output.decode('UTF-8', 'ignore')
                if "WinMon>" in serbuffer:
                    fse_error_outout = 1
                    break
                # if "Is this an eon chip?" in serbuffer:
                #     fse_pass_output = 1
                #     break
                # if 'Networking error' in serbuffer:
                #     fse_error_outout = 1
                #     break
                if 'Neither one of the kernels is valid' in serbuffer:
                    print(fse_label.config(text=f"{'Networking Error'}", bg=red_color))
                    fse_error_outout = 1
                    write_serial_output("No vailid Kernel!")
                    break
                if "Kernel panic - not syncing" in serbuffer:
                    fse_error_outout = 1
                    write_serial_output("KERNEL PANIC")
                    break
                if 'Binary file downloaded, size: 0x01f00000' in serbuffer:
                    if label_choice == 'ULGX80':
                        print(fse_label.config(text=f"{'Installing..'}", bg=yellow_color))
                    elif label_choice == 'G5DualBS':
                        print(fse_swap_label.config(text=f"{'Installing..'}", bg=yellow_color))
                if 'Not starting Web Server.' in serbuffer:
                    if label_choice == 'ULGX80':
                        write_serial_output("NOT STARTING WEB SERVER")
                        print(fse_label.config(text=f"{'PASS'}", bg=green_color))
                        fse_pass_output = 1
                        break
                    elif label_choice == "G5DualBS":
                        print(fse_swap_label.config(text=f"{'PASS'}", bg=green_color))
                        fse_pass_output = 1
                        break
                if 'Test Mode active, aborting application launching...' in serbuffer:
                    if label_choice == 'ULGX80':
                        write_serial_output("NOT STARTING WEB SERVER")
                        print(fse_label.config(text=f"{'PASS'}", bg=green_color))
                        fse_pass_output = 1
                        break
                    elif label_choice == "G5DualBS":
                        print(fse_swap_label.config(text=f"{'PASS'}", bg=green_color))
                        fse_pass_output = 1
                        break
                # if 'Intracom OmniBAS8W booting' in serbuffer:
                #     fse_pass_output = 1
                #     break
            if fse_pass_output == 1:
                pass
            elif fse_error_outout == 1:
                # print(fse_label.config(text=f"{'Error'}", bg=red_color))
                pass
            elif fse_error_outout == 0 and fse_pass_output == 0:
                master.after(10, self.fse_loop())

        except Exception as e:
            print(e)

    def fse_check(self):
        state = fse_check_state.get()
        if state == 1:
            try:
                print(fse_check_label.config(text=f"{'Running...'}", bg=yellow_color))
                system_version = self.root_login()
                # system_version = 1
                # self.set_normal_mode(0)
                # telnet = TelnetClass('192.168.1.100', 23, 3)
                # system_version = telnet.check_system_version(ulgx80_fse_version)
                if system_version == 1:
                    print("WRONG VERSION")
                    print(fse_check_label.config(text=f"{'FAIL'}", bg=red_color))
                    warning_message("Warning", "Manually reboot the card!")
                    print(fse_check_label.config(text=f"{''}", bg='white'))
                    self.fse(ulgx80_fse_path)

                    if fse_pass_output == 1:
                        print(fse_label.config(text=f"{'PASS'}", bg=green_color))
                        print(fse_check_label.config(text=f"{'PASS'}", bg=green_color))
                    else:
                        print(fse_label.config(text=f"{'FAIL'}", bg=red_color))
                        print(fse_check_label.config(text=f"{'FAIL'}", bg=red_color))
                else:
                    print("CORRECT VERSION")
                    print(fse_check_label.config(text=f"{'PASS'}", bg=green_color))


            except Exception as E:
                write_serial_output(E)
        else:
            pass

    def mng_port_test(self):
        state = management_port_state.get()
        if state == 1:
            print(management_port_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.root_login()
            try:
                self.send_ser_cmd("ping -i 0.5 -c 5 192.168.1.100")
                time.sleep(5)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                write_serial_output(output)
                print(output)
                ping_regex = re.findall(r'ttl=64', output)
                if len(ping_regex) == 5:
                    print("PASS")
                    print(management_port_label.config(text=f"{'PASS'}", bg=green_color))
                    resultList[9] = "PASS"
                else:
                    print("FAIL")
                    print(management_port_label.config(text=f"{'FAIL'}", bg=red_color))
                    resultList[9] = "FAIL"
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
                resultList[9] = "FAIL"
        else:
            pass

    def get_yserial(self):
        state = yserial_state.get()
        if state == 1:
            print(Yserial_test_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.root_login()
            try:
                self.send_ser_cmd('wf_test prod_set ctrl 1 Yserial ' + resultList[5])
                time.sleep(1)
                self.send_ser_cmd('wf_test prod_get ctrl 1 Yserial')
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                regex_Yserial = re.findall(r'\d\d\d+', output)

                try:
                    if resultList[5] == regex_Yserial[0]:
                        print(Yserial_test_label.config(text=f"{'PASS'}", bg=green_color))
                        print(Yserial_output_label.config(text=f"{regex_Yserial[0]}"))
                        write_serial_output('Yserial Test PASS : ' + regex_Yserial[0] + '\n')
                        resultList[10] = 'PASS'
                except IndexError:
                    print('FAIL')
                    print(Yserial_test_label.config(text=f"{'FAIL'}", bg=red_color))
                    write_serial_output('Yserial Test FAIL : Wrong Serial entered\n')
                    resultList[10] = 'FAIL'
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
                print(Yserial_test_label.config(text=f"{'FAIL'}", bg=red_color))
                resultList[10] = 'FAIL'
        else:
            pass

    def set_ycode(self):
        try:
            self.send_ser_cmd('wf_test prod_set ctrl 1 Ycode ' + resultList[6])
            time.sleep(1)
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            write_serial_output(output)
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))


    def set_ycode_yserial_rfu(self):
        try:
            Ycode_command = "!!shell sudo wf_test prod_set trcvr 1 Ycode Y00-000/00.L0"
            self.send_ser_cmd(Ycode_command)
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            write_serial_output(output)
            self.send_ser_cmd( "!!shell sudo wf_test prod_set trcvr 1 Yserial Y00-000/00.L0")
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def ycode_test(self):
        state = ycode_state.get()
        if state == 1:
            print(ycode_test_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.root_login()
            self.set_ycode()
            try:
                ycode_get_command = 'wf_test prod_get ctrl 1 Ycode'
                self.send_ser_cmd(ycode_get_command)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                write_serial_output(output)
                regex_ycode = re.findall(r'\w\d\d-\d\d\w[/]\d\d[.]\w\d', output)
                try:
                    if regex_ycode[0] == resultList[6]:
                        print(Ycode_output_label.config(text=f"{regex_ycode[0]}"))
                        print(ycode_test_label.config(text=f"{'PASS'}", bg=green_color))
                        resultList[11] = 'PASS'
                except IndexError:
                    print('FAIL')
                    print(ycode_test_label.config(text=f"{'FAIL'}", bg=red_color))
                    write_serial_output('Ycode Test FAIL : Wrong Ycode Entered\n')
                    resultList[11] = 'FAIL'
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
                resultList[11] = 'FAIL'
        else:
            pass

    def temp_test(self):
        state = temp_state.get()
        if state == 1:
            print(Temp_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.root_login()
            try:
                self.send_ser_cmd('wf_test m_temp')
                time.sleep(0.5)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                temp_string = re.findall(r'\((\d+(?:\.\d+)?) oC\)', output)
                temps2 = float(temp_string[0])
                print(str(temps2) + ' oC')
                if temps2 < maxTemp:
                    print(Temp_label.config(text=f"{str(temps2) + ' oC'}", bg=green_color))
                    resultList[13] = "PASS"
                    return 'PASS'
                elif temps2 > maxTemp:
                    print(Temp_label.config(text=f"{str(temps2) + ' oC'}", bg=red_color))
                    write_serial_output(output)
                    resultList[13] = "FAIL"
                    return 'FAIL'
                elif 'Error in ioctl WF_I2C_GET_TEMP' in output:
                    print(Temp_label.config(text=f"{'FAIL'}", bg=red_color))
                    write_serial_output('Temperature Test FAIL: Error in ioctl WF_I2C_GET_TEMP\n')
                    write_serial_output(output)
                    resultList[13] = "FAIL"
                    return 'FAIL'
                else:
                    pass
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))

    def hwid_test(self):
        state = hwid_state.get()
        if state == 1:
            print(HWID_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.root_login()
            try:
                hwid_command = "wf_test g_sig 10"
                self.send_ser_cmd(hwid_command)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                hwid_regex = re.findall(r'\d\w\d', output)
                print(hwid_regex)
                if hwid_regex:
                    print(HWID_label.config(text=f"{hwid_regex[0]}", bg=green_color))
                    resultList[12] = 'PASS: {}'.format(hwid_regex[0])
                else:
                    print(HWID_label.config(text=f"{'FAIL'}", bg=red_color))
                    write_serial_output(output)
                    resultList[12] = 'FAIL:' + hwid_regex[0]
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
                print(HWID_label.config(text=f"{'FAIL'}", bg=red_color))
        else:
            pass

    def rtc_write(self):
        try:

            # datetime_output = datetime.now().strftime('%a %b %H:%M:%S %Y')
            datetime_output = datetime.now().strftime('%d/%m/%Y %H:%M:%S')
            set_command = 'wf_test s_rtc'
            datetime_string[0] = datetime_output
            self.send_ser_cmd(set_command + " " + datetime_output)
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            write_serial_output(output)
            print(output)
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def rtc_read(self):
        state = rtc_state.get()
        if state == 1:
            print(RTC_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.root_login()
            self.rtc_write()
            try:
                rtc = "wf_test g_rtc"
                self.send_ser_cmd(rtc)
                datetime_output_results = datetime.now().strftime('%a %b %d %H:%M:%S %Y')
                result = self.ser.read(1000).decode('UTF-8', 'ignore')
                rtc_string3 = re.findall(r'\d\d:', result)
                rtc_string4 = re.findall(r'\d\d:', datetime_string[0])
                print(rtc_string3)
                print(rtc_string4)
                print('RTC: ' + rtc_string3[0])
                print('Datetime:' + rtc_string4[0])
                write_serial_output(result)
                if rtc_string3[0] == rtc_string4[0]:
                    print(RTC_label.config(text=f"{'PASS'}", bg=green_color))
                    write_serial_output(result)
                    resultList[14] = 'PASS'
                else:
                    print(RTC_label.config(text=f"{'FAIL'}", bg=red_color))
                    write_serial_output(result)
                    resultList[14] = 'FAIL'
            except IndexError:
                print(RTC_label.config(text=f"{'FAIL'}", bg=red_color))
        else:
            pass

    def rtc_readV2(self):
        self.rtc_write()
        try:
            rtc = "wf_test g_rtc"
            self.send_ser_cmd(rtc)
            datetime_output_results = datetime.now().strftime('%a %b %d %H:%M:%S %Y')
            result = self.ser.read(1000).decode('UTF-8', 'ignore')
            rtc_string3 = re.findall(r'\d\d:', result)
            rtc_string4 = re.findall(r'\d\d:', datetime_string[0])
            print(rtc_string3)
            print(rtc_string4)
            print('RTC: ' + rtc_string3[0])
            print('Datetime:' + rtc_string4[0])
            write_serial_output(result)
            if rtc_string3[0] == rtc_string4[0]:
                # print(RTC_label.config(text=f"{'PASS'}", bg=green_color))
                write_serial_output(result)

            else:
                # print(RTC_label.config(text=f"{'FAIL'}", bg=red_color))
                write_serial_output(result)

        except IndexError:
            # print(RTC_label.config(text=f"{'FAIL'}", bg=red_color))
            print('Fail')

    def pll_test(self):
        state = pll_state.get()
        if state == 1:
            print(PLL_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.root_login()
            time.sleep(0.1)
            try:
                pll_command = "wf_test s_pll_1588 r r8 0x1"
                self.send_ser_cmd(pll_command)
                time.sleep(0.5)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                output = output.strip("\r\n")
                write_serial_output(output)
                print(output)
                if "pll reg=0x1 val=0xe" in output:
                    print(PLL_label.config(text=f"{'val=0xe'}", bg=green_color))
                    resultList[15] = 'PASS'
                else:
                    print(PLL_label.config(text=f"{'FAIL'}", bg=red_color))
                    write_serial_output('PLL Test FAIL\n')
                    write_serial_output(output)
                    resultList[15] = "FAIL"
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
        else:
            pass

    def reset_program_dpll(self):
        state = dpll_state.get()
        if state == 1:
            print(dPLL_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.root_login()
            time.sleep(0.1)
            try:
                reset_program_commands = [" wf_test s_sig 8 0", " wf_test s_sig 8 1", 'cd /intracom/per']
                for command in reset_program_commands:
                    self.send_ser_cmd(command)
                    time.sleep(2)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    write_serial_output(output)
                time.sleep(0.5)
                self.send_ser_cmd("wf_test pll_prog pll.cfg")
                time.sleep(9)
                self.send_ser_cmd('cd')
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                write_serial_output(output)
                if "programming OK" in output:
                    print("PASS")
                    print(dPLL_label.config(text=f"{'PASS'}", bg=green_color))
                    resultList[16] = 'PASS'
                elif "Error while programming" in output:
                    print("FAIL")
                    print(dPLL_label.config(text=f"{'FAIL'}", bg=red_color))
                    resultList[16] = 'FAIL'
                else:
                    print("ERROR")
                    print(dPLL_label.config(text=f"{'FAIL'}", bg=red_color))
                    resultList[16] = 'FAIL'
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
                print(dPLL_label.config(text=f"{'FAIL'}", bg=red_color))
                resultList[16] = 'FAIL'
        else:
            pass

    def fpga_program(self):
        # reset_program_commands = ['cd /intracom/per']
        # for command in reset_program_commands:
        #     self.send_ser_cmd(command)
        #     time.sleep(1)
        #     output = self.ser.read(1000).decode('UTF-8', 'ignore')
        #     write_serial_output(output)
        print(fpga_program_label.config(text=f"{'Running...'}", bg=yellow_color))
        fpga_command = "wf_test f_prog 0 /intracom/per/gigas.crbf"
        self.send_ser_cmd(fpga_command)
        time.sleep(1)
        self.fpga_program_read_loop()

    def fpga_program_read_loop(self):
        serbuffer = " "
        result_output = 0
        try:
            while True:
                # output = self.ser.read().decode('UTF-8', 'ignore')
                output = self.ser.read()
                print(len(output))
                if len(output) == 0:
                    break

                if output == b'\r':
                    output = b''

                if output == b'\n':
                    serbuffer += "\n"
                    print(serbuffer)
                    write_serial_output(serbuffer)
                    serbuffer = " "
                else:
                    serbuffer += output.decode('UTF-8', 'ignore')
                if "FPGA programming DONE" in serbuffer:
                    result_output = 1
                    print(fpga_program_label.config(text=f"{'Program'}", bg=green_color))
                    print("PASS")

                if 'FPGA programming FAILED' in serbuffer:
                    result_output = 1
                    print("FAIL")
                    print(fpga_program_label.config(text=f"{'Program'}", bg=red_color))

            if result_output == 0:
                master.after(10, self.fpga_program_read_loop())
            else:
                pass
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def fpga_test(self):
        state = fpga_state.get()
        print("STATE = " + str(state))
        if state == 1:
            self.root_login()
            self.fpga_program()
            print(fpga_test_label.config(text=f"{'Running...'}", bg=yellow_color))
            time.sleep(1)
            # self.send_ser_cmd("cd")
            # time.sleep(1)
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
            try:
                self.send_ser_cmd("wf_test f_test 0 100000")
                time.sleep(7)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                if "FPGA test PASSED" in output:
                    print("PASS")
                    print(fpga_test_label.config(text=f"{'Test'}", bg=green_color))
                    resultList[17] = 'PASS'
                else:
                    print(fpga_test_label.config(text=f"{'Test'}", bg=red_color))
                    print("FAIL")
                    resultList[17] = 'FAIL'
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
        else:
            pass

    def insert_traffic_test_bitfile(self):
        try:
            os.chdir(traffic_test_bitfile_path)
            os.startfile(traffic_test_bitfile_path + r"\tftpd32.EXE")
            command_list = ["cd /intracom/per", 'tftp 192.168.1.10',"bin", 'get {}'.format(traffic_test_bitfile)]
            for commands in command_list:
                self.send_ser_cmd(commands)
                time.sleep(2)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
            time.sleep(85)
            os.system('cmd /c TASKKILL /IM tftpd32.EXE')
            self.send_ser_cmd("quit")
            time.sleep(1)
            self.send_ser_cmd("cd")
            output = self.ser.read(10000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
        except Exception as e:
            print(e)
            write_serial_output(e)

    def check_traffic_bitfile(self):
        try:
            self.send_ser_cmd('ll /intracom/per/{}'.format(traffic_test_bitfile))
            time.sleep(1)
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            if 'No such file or directory' in output:
                self.insert_traffic_test_bitfile()
                time.sleep(1)
                self.send_ser_cmd('ll /intracom/per/{}'.format(traffic_test_bitfile))
                output = self.ser.read(10000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                if 'No such file or directory' in output:
                    return "FAIL"
                elif "root root" in output:
                    return "PASS"
            elif "root root" in output:
                return "PASS"
        except Exception as e:
            print(e)
            write_errors_output(e)
            return "ERROR"


    def set_traffic_test(self):
        try:
            check_gigas_test_bitfile = self.check_traffic_bitfile()
            if check_gigas_test_bitfile == "PASS":
                self.send_ser_cmd('cd /intracom/per')
                time.sleep(0.5)
                self.send_ser_cmd('wf_test f_prog 0 gigas_test.crbf')
                for i in range(100):
                    i += 0.1
                    # self.ser.read_until('[root@ul-gx80:~]#').decode('UTF-8', 'ignore')
                    time.sleep(0.1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    write_serial_output(output)
                    if 'FPGA programming DONE' in output:
                        break
                    if 'FPGA programming FAILED' in output:
                        break
                time.sleep(1)
                self.send_ser_cmd('wf_test f_set 0 r16 0x3 0x440')
                time.sleep(2)
            elif check_gigas_test_bitfile == "FAIL":
                pass

        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    # def jesd_test_set(self):
    #     try:
    #         self.send_ser_cmd("wf_test f_set 0 r16 0x50 0x0001")
    #         time.sleep(3)
    #         output = self.ser.read(1000).decode('UTF-8', 'ignore')
    #         write_serial_output(output)
    #         if "REG 0x51, DATA 0x0" in output:
    #             print("PASS")
    #         else:
    #             print("FAIL")
    #     except Exception as e:
    #         print("ERROR: " + str(e))
    #         write_serial_output(str(e))
    #         warning_message('Warning', str(e))
    #
    # def jesd_test_result(self):
    #     try:
    #         time.sleep(0.5)
    #         self.send_ser_cmd("wf_test f_get 0 r16 0x51")
    #         time.sleep(3)
    #         output = self.ser.read(1000).decode('UTF-8', 'ignore')
    #         write_serial_output(output)
    #         if "REG 0x51, DATA 0x0" in output:
    #             print(jesd_label.config(text=f"{'JESD'}", bg=green_color))
    #         else:
    #             print("FAIL")
    #             print(jesd_label.config(text=f"{'JESD'}", bg=red_color))
    #     except Exception as e:
    #         print("ERROR: " + str(e))
    #         write_serial_output(str(e))
    #         warning_message('Warning', str(e))

    def jesd_test_set(self):
        try:
            command_set_list = ["wf_test f_set 0 r16 0x3 0x440", "wf_test f_set 0 r16 0x50 0x0001"]
            for command in command_set_list:
                self.send_ser_cmd(command)
                time.sleep(1)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                write_serial_output(output)

        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def jesd_test_result(self):
        try:
            command_get_list = ["wf_test f_get 0 r16 0x53",
                                "wf_test f_get 0 r16 0x54", "wf_test f_get 0 r16 0x54"]
            expected_output = ["REG 0x53, DATA 0x0", "REG 0x54, DATA 0x0",
                               "REG 0x54, DATA 0x0"]
            jesd_result_list = []
            for command in command_get_list:
                self.send_ser_cmd(command)
                time.sleep(1)
                for command_output in expected_output:
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    write_serial_output(output)
                    if command_output in output:
                        print("FAIL")
                        jesd_result_list.append("FAIL")
                    else:
                        print("PASS")
                        jesd_result_list.append("PASS")
            print(jesd_result_list)
            if "FAIL" in jesd_result_list:
                print(jesd_label.config(text=f"{'JESD'}", bg=red_color))
            else:
                print("PASS")
                print(jesd_label.config(text=f"{'JESD'}", bg=green_color))
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def serdes_loopbacks(self):
        try:
            self.send_ser_cmd("cd /intracom/util")
            time.sleep(1)
            self.send_ser_cmd("/intracom/util/serdes_loopbacks")
            time.sleep(0.1)
            i = 1
            for i in range(14):
                i += 0.1
                time.sleep(0.1)
                output2 = self.ser.read(1000).decode('UTF-8', 'ignore')
                write_serial_output(output2)
                regex_prompt = re.findall(r'ERROR', output2)
                if regex_prompt:
                    break
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def gigas_test_sh(self):
        try:
            self.send_ser_cmd('cd /intracom/util')
            time.sleep(1)
            self.send_ser_cmd('chmod +x gigas_test.sh')
            time.sleep(1)
            self.send_ser_cmd("gigas_test.sh")
            time.sleep(4)
            output = self.ser.read(10000).decode('UTF-8', 'ignore')
            write_serial_output(output)
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def start_10g_phy(self):
        try:
            self.send_ser_cmd('recalibration.scr')
            time.sleep(6)
            output = self.ser.read(10000).decode('UTF-8', 'ignore')
            write_serial_output(output)
            self.send_ser_cmd('cd')
            time.sleep(1)
            output = self.ser.read(10000).decode('UTF-8', 'ignore')
            write_serial_output(output)
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def traffic_test(self):
        state = traffic_state.get()
        if state == 1:
            self.root_login('[root@ul-gx80:/intracom/per]#')
            print(jesd_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.set_traffic_test()
            self.jesd_test_set()
            self.serdes_loopbacks()
            self.gigas_test_sh()
            # set_traffic_mt1000a_thread() #mt1000a
            # mt1000a_traffic_test() #mt1000a
            # leds_on_eth_test()
            self.start_10g_phy()
            trafficON_port1()
            self.jesd_test_result()
            # time.sleep(0.5)
            mts5800_traffic_test()
            mts5800_traffic_test_show_result()
        else:
            pass

    def xpic_check(self):
        state = xpic_state.get()
        if state == 1:
            try:
                print(xpic_label_program.config(text=f"{'Running...'}", bg=yellow_color))
                self.root_login()
                self.send_ser_cmd("ll /intracom/per/{}".format(xpic_bitfile))
                time.sleep(1)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                write_serial_output(output)
                if '-rw-r--r-- 1 root root' in output:
                    print('pass')
                    self.xpic_test()
                elif 'No such file or directory':
                    # warning_message('Warning', "Pick the bitfile  that you can find in C:\\G5DualBS_Files\\ULGX80_FSE path, and place it to /intracom/per/ path via filezilla. Then Press OK!")
                    self.insert_xpic_test_file()
                    time.sleep(5)
                    self.root_login()
                    self.send_ser_cmd("ll /intracom/per/{}".format(xpic_bitfile))
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    write_serial_output(output)
                    if 'No such file or directory' in output:
                        print(xpic_label_program.config(text=f"{'Upload'}", bg=red_color))
                    else:
                        print(xpic_label_program.config(text=f"{'Upload'}", bg=green_color))
                        time.sleep(1)
                        self.xpic_test()

            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
        else:
            pass

    def insert_xpic_test_file(self):
        try:
            os.chdir(xpic_path)
            os.startfile(xpic_path + r"\tftpd32.EXE")
            command_list = ["cd /intracom/per", 'tftp 192.168.1.10', "bin", 'get {}'.format(xpic_bitfile)]
            for commands in command_list:
                self.send_ser_cmd(commands)
                time.sleep(2)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
            time.sleep(85)
            os.system('cmd /c TASKKILL /IM tftpd32.EXE')
            self.send_ser_cmd("quit")
            time.sleep(1)
            self.send_ser_cmd("cd")
            output = self.ser.read(10000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
        except Exception as e:
            print(e)
            write_serial_output(e)

    def xpic_program(self):
        program_result = ''
        try:
            print(xpic_label_program.config(text=f"{'Running...'}", bg=yellow_color))
            self.send_ser_cmd('wf_test f_prog 0 /intracom/per/{}'.format(xpic_bitfile))
            for i in range(130):
                i += 0.1
                # self.ser.read_until('[root@ul-gx80:~]#').decode('UTF-8', 'ignore')
                time.sleep(0.1)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                write_serial_output(output)
                if 'FPGA programming DONE' in output:
                    print(xpic_label_program.config(text=f"{'Program'}", bg=green_color))
                    program_result = "PASS"
                    break
                elif 'FPGA programming FAILED' in output:
                    print(xpic_label_program.config(text=f"{'Program'}", bg=red_color))
                    program_result = "FAIL"
                    break
                # else:
                #     print(xpic_label_program.config(text=f"{'Program'}", bg=red_color))
            command_list = ['wf_test f_set 0 r16 0x3 0x440', 'ntest set 3:90[2] 0', 'ntest set 3:90[2] 1', 'ntest set 3:90[2] 0']
            for command in command_list:
                self.send_ser_cmd(command)
                time.sleep(1.5)
                output = self.ser.read(10000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                return program_result
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def xpic_test(self):
            try:

                program_result = self.xpic_program()
                if program_result == "PASS":
                    print(xpic_label_test.config(text=f"{'Running...'}", bg=yellow_color))
                    regex_results = []
                    for i in range(10):
                        self.send_ser_cmd('ntest get 3:105[1-0]')
                        time.sleep(1.5)
                        output = self.ser.read(1000).decode('UTF-8', 'ignore')
                        print(output)
                        write_serial_output(output)
                        result_regex = re.findall(r'00000000', output)
                        if result_regex:
                            regex_results.append("PASS")
                            print(regex_results)
                            # break
                        else:
                            pass
                    if len(regex_results) == 10:
                        print(xpic_label_test.config(text=f"{'Test'}", bg=green_color))
                        resultList[27] = "PASS"
                    else:
                        print("FAIL")
                        print(xpic_label_test.config(text=f"{'Test'}", bg=red_color))
                        resultList[27] = "FAIL"
                else:
                    print("FAIL")
                    print(xpic_label_test.config(text=f"{'Test'}", bg=red_color))
                    resultList[27] = "FAIL"
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))

    def table_upload(self):
        state = table_state.get()
        if state == 1:
            try:
                print(table_label.config(text=f"{'Running...'}", bg=yellow_color))
                date = datetime.now().strftime('%d%m%Y')
                print(date)
                self.root_login_g5()
                self.send_ser_cmd('!!shell sudo wf_test prod_set ctrl 1 Yserial ' + resultList[5])
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                time.sleep(1)
                self.send_ser_cmd('!!shell sudo wf_test prod_get ctrl 1 Yserial')
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                regex_Yserial = re.findall(r'\d\d\d+', output)
                edit_bitfile(regex_Yserial[0], date, table_path)
                self.insert_table()
                print(table_label.config(text=f"{'PASS'}", bg=green_color))
                resultList[28] = "PASS"
            except Exception as e:
                write_serial_output("Error in table upload: {}".format(e))
                write_errors_output("Error in table upload: {}".format(e))
                print(table_label.config(text=f"{'FAIL'}", bg=red_color))
                resultList[28] = "FAIL"
        else:
            pass

    def insert_table(self):
        try:
            os.chdir(table_path)
            os.startfile(table_path + r"\tftpd32.EXE")
            command_list = ["!!shell sudo su", "cd /intracom/per", 'tftp 192.168.1.10', 'bin', 'get idu_tbl.bin']
            for commands in command_list:
                self.send_ser_cmd(commands)
                time.sleep(2)
                output = self.ser.read(10000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
            time.sleep(3)
            os.system('cmd /c TASKKILL /IM tftpd32.EXE')
            self.send_ser_cmd("quit")
            time.sleep(1)
            self.send_ser_cmd("cd")
            output = self.ser.read(10000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
            self.send_ser_cmd("exit")
            output = self.ser.read(10000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
        except IndexError:
            warning_message('warning', 'Yserial not found, please enter it and rerun the test!')
        except Exception as E:
            print(E)
            write_serial_output(E)

    def check_license_g5dualbs(self):
        try:
            self.send_cmd('get license info')
            time.sleep(0.5)
            output = self.ser.read(10000).decode('UTF-8', 'ignore')
            write_serial_output(output)
            license_regex = re.findall(r'SK1D', output)
            print(license_regex)
            time.sleep(0.5)
            if license_regex:
                return 'PASS'
            else:
                return 'FAIL'
        except Exception as e:
            print("type error: " + str(e))
            write_serial_output(str(e))
            return 'ERROR'

    def check_license_file(self):
        try:
            command_list = ["!!shell sudo su", "cd /intracom/conf"]
            for commands in command_list:
                self.send_cmd(commands)
                time.sleep(2)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
            self.send_cmd('ll "{}"'.format(g5dualbs_license))
            time.sleep(1)
            output = self.ser.read(10000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
            if "root root" in output:
                self.send_cmd("exit")
                time.sleep(1)
                output = self.ser.read(10000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
                print("pass")
                return 'PASS'
            elif "No such file or directory" in output:
                return 'FAIL'
        except Exception as e:
            print("type error: " + str(e))
            return 'ERROR'

    def activate_license(self):
        try:
            self.send_cmd('!!shell sudo touch /intracom/conf/insecure')
            time.sleep(0.5)
            self.send_cmd('modify license cfg local "{}"'.format(g5dualbs_license))
            time.sleep(2)
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            write_serial_output(output)
            print(output)
            if 'New license requires reset' in output:
                print(g5dualbs_license_label.config(text=f"{'Reseting...'}", fg='black', bg=yellow_color))
            if 'for working card not found!' in output:
                print(g5dualbs_license_label.config(text=f"{'FAIL'}", bg=red_color))
                return 'not found'
            if 'Extended error #10320: Invalid license' in output:
                return 'Invalid License'
            time.sleep(13)
            self.admin_login()
            self.ser.write(b'get license info\r')
            time.sleep(0.5)
            output = self.ser.read(10000).decode('UTF-8', 'ignore')
            write_serial_output(output)
            license_regex = re.findall(r'SK1D', output)
            print(license_regex)
            time.sleep(0.5)
            if license_regex:
                print(g5dualbs_license_label.config(text=f"{'License set'}", fg='black', bg=green_color))
                return 'PASS'
            if not license_regex:
                print(g5dualbs_license_label.config(text=f"{'FAIL'}", fg='black', bg=red_color))
                return 'FAIL'
        except Exception as e:
            print(e)
            write_serial_output(e)

    def set_license_g5dualbs(self):
        try:
            self.admin_login()
            check_license = self.check_license_g5dualbs()
            if check_license == 'PASS':
                print(g5dualbs_license_label.config(text=f"{'License set'}", fg='black', bg=green_color))
            elif check_license == 'FAIL':
                self.send_cmd('!!shell sudo touch /intracom/conf/insecure')
                output = self.ser.read(1000).decode()
                print(output)
                write_serial_output(output)
                check_license_file = self.check_license_file()
                if check_license_file == "PASS":
                    activate_license = self.activate_license()
                    if activate_license == 'PASS':
                        pass
                    elif activate_license == 'Invalid License':
                        self.reset_license()
                        activate_license = self.activate_license()
                        if activate_license == 'PASS':
                            pass
                        if activate_license == 'Invalid License':
                            print(license_label.config(text=f"{'FAIL'}", bg=red_color))
                elif check_license_file == 'FAIL':
                    self.insert_license_file()
                    check_license_file = self.check_license_file()
                    if check_license_file == 'PASS':
                        print("PASS")
                        self.activate_license()
                    elif check_license_file == 'FAIL':
                        print('FAIL')
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))
            print(Indicator_label.config(text=f"{str(e)}", bg=red_color))
            print(license_label.config(text=f"{'FAIL'}", bg=red_color))

    def insert_license_file(self):
        os.chdir(g5dualbs_fse_path)
        os.startfile(g5dualbs_fse_path + r"\tftpd32.EXE")
        command_list = ['!!shell sudo su', 'tftp 192.168.1.10', "get " + g5dualbs_license, 'quit', 'exit']
        for commands in command_list:
            self.send_cmd(commands)
            time.sleep(2)
            output = self.ser.read(10000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
        os.system('cmd /c TASKKILL /IM tftpd32.EXE')

    def reset_license(self):
        try:
            command_list = ['!!shell sudo su', 'cd', 'rm /intracom/conf/{license}'.format(license=g5dualbs_license), 'engcli', 'lman force default', 'exit', 'exit']
            for commands in command_list:
                self.send_cmd(commands)
                time.sleep(1)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
        except Exception as e:
            print(e)
            write_serial_output(e)

    def check_system_version(self, version):
        output = ''
        try:
            self.send_cmd('get system info')
            time.sleep(1)
            output2 = self.ser.read(1000).decode('UTF-8', 'ignore')
            output += output2
            write_serial_output(output)
            regex_fse_version = re.findall(version, output)
            print("REGEX fse_VERSION " + regex_fse_version[0])
            if regex_fse_version:
                return regex_fse_version[0]
            else:
                return 0
        except Exception as e:
            write_serial_output(e)

    def rtc_write_g5dualbs(self):
        try:
            # datetime_output = datetime.now().strftime('%a %b %H:%M:%S %Y')
            datetime_output = datetime.now().strftime('%d/%m/%Y %H:%M:%S')
            set_command = '!!shell sudo wf_test s_rtc'
            datetime_string[0] = datetime_output
            self.send_ser_cmd(set_command + " " + datetime_output)
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            write_serial_output(output)
            print(output)
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def rtc_read_g5dualbs(self):
        self.rtc_write_g5dualbs()
        try:
            rtc = "!!shell sudo wf_test g_rtc"
            self.send_cmd(rtc)
            datetime_output_results = datetime.now().strftime('%a %b %d %H:%M:%S %Y')
            result = self.ser.read(1000).decode('UTF-8', 'ignore')
            rtc_string3 = re.findall(r'\d\d:', result)
            rtc_string4 = re.findall(r'\d\d:', datetime_string[0])
            print(rtc_string3)
            print(rtc_string4)
            print('RTC: ' + rtc_string3[0])
            print('Datetime:' + rtc_string4[0])
            write_serial_output(result)
            if rtc_string3[0] == rtc_string4[0]:
                # print(RTC_label.config(text=f"{'PASS'}", bg=green_color))
                write_serial_output(result)
                return 'PASS'
            else:
                # print(RTC_label.config(text=f"{'FAIL'}", bg=red_color))
                write_serial_output(result)
                return 'FAIL'
        except IndexError:
            # print(RTC_label.config(text=f"{'FAIL'}", bg=red_color))
            return 'ERROR'

    def fse_swap(self):
        state = fse_swap_state.get()
        if state == 1:
            warning_message('Warning', 'Manually reboot the card and then press ok, in order to swap to G5DualBS FSE!')
            self.winmon_login()
            self.fse_G5DualBS()
        else:
            pass

    def license_rtc_g5dualbs(self):
        state = license_rtc_g5dualbs_state.get()
        if state == 1:
            self.set_license_g5dualbs()
            rtc = self.rtc_read_g5dualbs()
            self.set_ycode_yserial_rfu()
            fse_version = self.check_system_version(g5dualbs_fse_version)
            if fse_version != 0:
                print(fse_version_info_output_label.config(text=f"{fse_version}"))
            elif fse_version == 0:
                print(fse_version_info_output_label.config(text=f"{'Wrong FSE'}", bg=red_color))
            if rtc == "PASS":
                print("PASS")
                print(g5dualbs_RTC_label.config(text=f"{'RTC'}", bg=green_color))
                resultList[34] = "PASS"
            else:
                print("FAIL")
                print(g5dualbs_RTC_label.config(text=f"{'RTC'}", bg=red_color))
                resultList[34] = "FAIL"
        else:
            pass

    def check_norfu(self, add_or_remove):
        login = self.root_login_g5()
        if login == "admin":
            command_list = ["!!shell sudo su", 'cd /intracom/per']
            for commands in command_list:
                self.send_cmd(commands)
                time.sleep(1)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
            self.send_ser_cmd("ll norfu")
            time.sleep(1)
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
            if 'No such file or directory' in output:
                if add_or_remove == "add":
                    self.send_ser_cmd("touch norfu")
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    print(output)
                    write_serial_output(output)
                    self.send_ser_cmd("rm /mnt/metadata/lmx_cal")
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    print(output)
                    write_serial_output(output)
                    self.send_ser_cmd("exit")
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    print(output)
                    write_serial_output(output)
                    warning_message("Warning", "Hardware reboot the card!")
                    return "added reboot"
                elif add_or_remove == "remove":
                    self.send_ser_cmd("exit")
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    print(output)
                    write_serial_output(output)
                    return "removed"
            else:
                if add_or_remove == "add":
                    self.send_ser_cmd("exit")
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    print(output)
                    write_serial_output(output)
                    return "added"
                elif add_or_remove == "remove":
                    self.send_ser_cmd("rm norfu")
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    print(output)
                    write_serial_output(output)
                    self.send_ser_cmd("exit")
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    print(output)
                    write_serial_output(output)
                    return "removed"
        else:
            command_list = ['cd /intracom/per']
            for commands in command_list:
                self.send_cmd(commands)
                time.sleep(1)
                output = self.ser.read(1000).decode('UTF-8', 'ignore')
                print(output)
                write_serial_output(output)
            self.send_ser_cmd("ll norfu")
            time.sleep(1)
            output = self.ser.read(1000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
            if 'No such file or directory' in output:
                if add_or_remove == "add":
                    self.send_ser_cmd("touch norfu")
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    print(output)
                    write_serial_output(output)
                    self.send_ser_cmd("rm /mnt/metadata/lmx_cal")
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    print(output)
                    write_serial_output(output)
                    warning_message("Warning", "Hardware reboot the card!")
                    return "added reboot"
                elif add_or_remove == "remove":

                    return "removed"
            else:
                if add_or_remove == "add":
                    return "added"
                elif add_or_remove == "remove":
                    self.send_ser_cmd("rm norfu")
                    time.sleep(1)
                    output = self.ser.read(1000).decode('UTF-8', 'ignore')
                    print(output)
                    write_serial_output(output)
                    return "removed"


    def create_norfu(self):
        state = norfu_state.get()
        if state == 1:
            try:
                print(norfu_label.config(text=f"{'Running...'}", bg=yellow_color))
                norfu_check = self.check_norfu("add")
                if norfu_check == "added":
                    print(norfu_label.config(text=f"{'PASS'}", bg=green_color))
                elif norfu_check == "added reboot":
                    self.admin_login()
                    print(norfu_label.config(text=f"{'PASS'}", bg=green_color))
                    resultList[35] = "PASS"
                else:
                    print(norfu_label.config(text=f"{'FAIL'}", bg=red_color))
                    resultList[35] = "FAIL"
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
        else:
            pass

    def rm_norfu(self):
        state = rm_norfu_state.get()
        if state == 1:
            print(rm_norfu_label.config(text=f"{'Running...'}", bg=yellow_color))
            try:
                norfu_check = self.check_norfu("remove")
                if norfu_check == "removed":
                    print(rm_norfu_label.config(text=f"{'PASS'}", bg=green_color))
                    resultList[36] = "PASS"

                else:
                    print(rm_norfu_label.config(text=f"{'FAIL'}", bg=red_color))
                    resultList[36] = "FAIL"
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
        else:
            pass

    def check_system(self):
        state = check_system_state.get()
        print(state)
        if state == 1:
            print(check_system_label.config(text=f"{'Running'}", fg='black', bg=yellow_color))
            resultList[32] = 'PASS'  # Ξεκινάω με pass, και το αλλάζω στο τέλος με fail αν υπάρχει
            result_list = []
            self.admin_login()
            try:
                self.send_ser_cmd('get system info')
                time.sleep(0.5)
                self.send_ser_cmd("!!shell sudo wf_test g_rtc")
                time.sleep(0.5)
                self.send_ser_cmd('get license info')
                time.sleep(1)
                # self.send_ser_cmd('get ethernet state')
                # time.sleep(0.5)
                output = self.ser.read(10000).decode('UTF-8', 'ignore')
                write_serial_output(output)
                rtc_string = re.findall(r'\w\w\w\s?\s\d?\d?\d?\s\s?\d\d:\d\d:\d\d\s\d\d\d\d', output)
                license_regex = re.findall(r'SK\dd-', output)
                print(license_regex)
                mac_regex2 = re.findall(r'Outband MAC Address : (?:[0-9a-fA-F]:?){12}', output)
                mac_regex = re.findall(r'(?:[0-9a-fA-F]:?){12}', mac_regex2[0])
                print(mac_regex2)
                print(mac_regex)
                regex_Ycode = re.findall(r'Y00-\d\d\w[/]\d\d.\w\d\d?\d?', output)
                regex_Yserial = re.findall(r'[0]?3[1-2]\d\d\d\d\d\d\d\d\d\d\d?', output)
                regex_FSE_version = re.findall(r'\d.\d.\d_\d\d\d\d_\w.\d.\d.\d.\w\w\w\w\w_\d\d', output)
                regex_10gb_status = re.findall(r'10000mbps', output)
                print(regex_10gb_status)
                print(check_system_label.config(text=f"{'PASS'}", bg=green_color))
                if mac_regex:
                    print(mac_info_output_label.config(text=f"{mac_regex[0]}"))
                    resultList[7] = mac_regex[0]
                    print(mac_regex)
                if not mac_regex:
                    result_list.append('FAIL')
                if regex_Ycode:
                    print(Ycode_output_label.config(text=f"{regex_Ycode[0]}"))
                    resultList[6] = regex_Ycode[0]
                    print(regex_Ycode)
                if not regex_Ycode:
                    result_list.append('FAIL')
                if regex_Yserial:
                    print(Yserial_output_label.config(text=f"{regex_Yserial[0]}"))
                    resultList[5] = regex_Yserial[0]
                    print(regex_Yserial)
                if not regex_Yserial:
                    result_list.append('FAIL')
                if regex_FSE_version:
                    print(fse_version_info_output_label.config(text=f"{regex_FSE_version[0]}"))
                if rtc_string:
                    print(RTC_info_output_label.config(text=f"{rtc_string[0]}"))
                    print(rtc_string[0])
                license_check = self.check_license_g5dualbs()
                if license_check == "PASS":
                    print(License_info_output_label.config(text=f"{'Active License'}", fg='black'))
                if license_check != "PASS":
                    print(License_info_output_label.config(text=f"{'No Active License'}", fg='black', bg=red_color))
                    result_list.append('FAIL')
                # if not license_regex:
                #     print(License_info_output_label.config(text=f"{'No Active License'}", fg='black', bg=red_color))
                #     result_list.append('FAIL')
                # if license_regex:
                #     print(License_info_output_label.config(text=f"{'Active License'}", fg='black'))
                # if regex_10gb_status:
                #     print(TENG_info_output_label.config(text=f"{'10Gbit Active'}", fg='black'))
                # if not regex_10gb_status:
                #     print(TENG_info_output_label.config(text=f"{'10Gbit Not Active'}", fg='black', bg=red_color))
                #     result_list.append('FAIL')
                if 'FAIL' in result_list:
                    print(check_system_label.config(text=f"{'FAIL'}", fg='black', bg=red_color))
                    print(result_list)
                    resultList[32] = 'FAIL'
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
        else:
            pass



class TelnetClass:
    def __init__(self, ip, port, timeout):
        try:
            self.ip = ip
            self.timeout = timeout
            self.port = 23
            self.tn = telnetlib.Telnet(ip, port)
        except Exception as e:
            print('Error: ' + str(e))

    def send_cmd(self, cmd):
        try:
            cmd = cmd + '\r'
            self.tn.write(cmd.encode())
            write_serial_output(cmd)
            time.sleep(0.5)
        except Exception as e:
            print('Error: ' + str(e))

    def req_data(self, cmd):
        try:
            self.send_cmd(cmd)
            print(cmd)
            time.sleep(0.5)
            data = self.tn.read_very_eager().decode('UTF-8', 'ignore')
            data = data.strip('\r\n')
            return data
        except Exception as e:
            print(e)



    def admin_login(self):
        try:
            self.send_cmd('\r')
            timeout = 0
            while True:
                # global output
                time.sleep(0.5)
                timeout += 1
                output = self.tn.read_very_eager().decode('ascii')
                print(output, end='\r\n', flush=True)
                write_serial_output(output)
                if "System starting up, please try later" in output:
                    time.sleep(17)
                if 'Username: ' in output:
                    self.send_cmd('admin')
                    time.sleep(1)
                if 'Password: ' in output:
                    self.send_cmd('admin')
                if 'UL-GX80>' in output:
                    return 1
                if 'OmniBAS>' in output:
                    return 1
                if 'OSDR-G5-DUAL-FDD-PMP>' in output:
                    return 1
                if timeout >= 20:
                    warning_message('ERROR', 'Restart the card!')
                    # SerialClass.
                    return 2
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def admin_login_check(self):
        try:
            # self.send_cmd('\r')
            i = 0
            while i <= 50:
                i += 1
                # global output
                time.sleep(0.5)
                output = self.tn.read_very_eager().decode('ascii')
                print(output, end='\r\n', flush=True)
                write_serial_output(output)
                if "System starting up, please try later" in output:
                    time.sleep(10)
                if 'Username: ' in output:
                    self.send_cmd('admin')
                    time.sleep(1)
                if 'Password: ' in output:
                    self.send_cmd('admin')
                if 'UL-GX80>' in output:
                    print(set_normal_mode_label.config(text=f"{'Set'}", bg=green_color))
                    return "Set"
                if i >= 49:
                    print(set_normal_mode_label.config(text=f"{'Not Set'}", bg=red_color))
                    return "Not set"
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def check_license(self):
        try:
            self.send_cmd('get license info')
            time.sleep(0.5)
            output = self.tn.read_very_eager().decode()
            write_serial_output(output)
            license_regex = re.findall(r'SK1D', output)
            print('LICENSE REGEX' + license_regex[0])
            time.sleep(0.5)
            if license_regex:
                print(license_label.config(text=f"{'License set'}", fg='black', bg=green_color))
                print(License_info_output_label.config(text=f"{'License set'}", fg='black'))
                return 1
            else:
                # print(license_label.config(text=f"{'FAIL'}", fg='black', bg=red_color))
                # print(License_info_output_label.config(text=f"{'License set'}", fg='black', bg=red_color))
                return 0
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            # warning_message('Warning', str(e))

    def rtc_readV2(self):
        self.rtc_write()
        try:
            rtc = "wf_test g_rtc"
            self.send_cmd(rtc)
            datetime_output_results = datetime.now().strftime('%a %b %d %H:%M:%S %Y')
            result = self.tn.read_very_eager().decode()
            rtc_string3 = re.findall(r'\d\d:', result)
            rtc_string4 = re.findall(r'\d\d:', datetime_string[0])
            print(rtc_string3)
            print(rtc_string4)
            print('RTC: ' + rtc_string3[0])
            print('Datetime:' + rtc_string4[0])
            write_serial_output(result)
            if rtc_string3[0] == rtc_string4[0]:
                # print(RTC_label.config(text=f"{'PASS'}", bg=green_color))
                write_serial_output(result)

            else:
                # print(RTC_label.config(text=f"{'FAIL'}", bg=red_color))
                write_serial_output(result)

        except IndexError:
            # print(RTC_label.config(text=f"{'FAIL'}", bg=red_color))
            print('Fail')


    def set_license_ulgx80(self):
        state = license_state.get()
        if state == 1:
            print(license_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.admin_login()
            time.sleep(0.5)
            self.rtc_readV2()
            time.sleep(2)
            try:
                set_license = self.set_license(ulgx80_license)
                if set_license == 1:
                    print(license_label.config(text=f"{'License set'}", fg='black', bg=green_color))
                    resultList[31] = "PASS"
                else:
                    print(license_label.config(text=f"{'FAIL'}", fg='black', bg=red_color))
                    resultList[31] = "FAIL"
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
                print(Indicator_label.config(text=f"{str(e)}", bg=red_color))
                print(license_label.config(text=f"{'FAIL'}", bg=red_color))
                resultList[31] = "FAIL"
        else:
            pass

    def set_license(self, license_name):
        try:
            license_status = self.check_license()
            if license_status == 1:
                return 1
            else:
                self.send_cmd('!!shell sudo touch /intracom/conf/insecure')
                time.sleep(3)
                self.send_cmd('modify license cfg file "{}"'.format(license_name)) #to license prepei na einai mesa se ""
                time.sleep(0.5)
                output = self.tn.read_very_eager().decode()
                write_serial_output(output)
                write_serial_output(output)
                print(output)
                if 'New license requires reset' in output:
                    if license_name == ulgx80_license:
                        print(license_label.config(text=f"{'Reseting...'}", fg='black', bg=yellow_color))
                    elif license_name == g5dualbs_license:
                        print(g5dualbs_license_label.config(text=f"{'Reseting...'}", fg='black', bg=yellow_color))
                time.sleep(13)
                self.admin_login()
                self.send_cmd('get license info')
                time.sleep(0.5)
                output = self.tn.read_very_eager().decode()
                write_serial_output(output)
                license_regex = re.findall(r'SK1D', output)
                print(license_regex)
                time.sleep(0.5)
                if license_regex:
                    if license_name == ulgx80_license:
                        print(license_label.config(text=f"{'License set'}", fg='black', bg=green_color))
                        return 1
                    elif license_name == g5dualbs_license:
                        print(g5dualbs_license_label.config(text=f"{'License set'}", fg='black', bg=green_color))
                        return 1
                if not license_regex:
                    if license_name == ulgx80_license:
                        print(license_label.config(text=f"{'FAIL'}", fg='black', bg=red_color))
                        return 0
                    elif license_name == g5dualbs_license:
                        print(g5dualbs_license_label.config(text=f"{'FAIL'}", fg='black', bg=red_color))
                        return 0
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))
            print(Indicator_label.config(text=f"{str(e)}", bg=red_color))
            print(license_label.config(text=f"{'FAIL'}", bg=red_color))

    def rtc_write(self):
        try:
            # datetime_output = datetime.now().strftime('%a %b %H:%M:%S %Y')
            datetime_output = datetime.now().strftime('%d/%m/%Y %H:%M:%S')
            set_command = '!!shell sudo wf_test s_rtc'
            datetime_string[0] = datetime_output
            self.send_cmd(set_command + " " + datetime_output)
            output = self.tn.read_very_eager().decode()
            write_serial_output(output)
            print(output)
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def rtc_read_g5dualbs(self):
        self.rtc_write()
        try:
            rtc = "!!shell sudo wf_test g_rtc"
            self.send_cmd(rtc)
            datetime_output_results = datetime.now().strftime('%a %b %d %H:%M:%S %Y')
            result = self.tn.read_very_eager().decode()
            rtc_string3 = re.findall(r'\d\d:', result)
            rtc_string4 = re.findall(r'\d\d:', datetime_string[0])
            print(rtc_string3)
            print(rtc_string4)
            print('RTC: ' + rtc_string3[0])
            print('Datetime:' + rtc_string4[0])
            write_serial_output(result)
            if rtc_string3[0] == rtc_string4[0]:
                # print(RTC_label.config(text=f"{'PASS'}", bg=green_color))
                write_serial_output(result)
                return 'PASS'
            else:
                # print(RTC_label.config(text=f"{'FAIL'}", bg=red_color))
                write_serial_output(result)
                return 'FAIL'
        except IndexError:
            # print(RTC_label.config(text=f"{'FAIL'}", bg=red_color))
            return 'ERROR'

    # def check_system(self):
    #     state = check_system_state.get()
    #     print(state)
    #     if state == 1:
    #         output = ''
    #         command_list = ['get system info', '!!shell sudo wf_test g_rtc', 'get license info', 'get ethernet state']
    #         state = check_system_state.get()
    #         print(state)
    #         if state == 1:
    #             print(check_system_label.config(text=f"{'Running...'}", bg=yellow_color))
    #             resultList[32] = 'PASS'
    #             result_list = []
    #             self.admin_login()
    #             self.rtc_readV2()
    #             time.sleep(0.5)
    #             try:
    #                 for command in command_list:
    #                     self.send_cmd(command)
    #                     time.sleep(0.5)
    #                     output2 = self.tn.read_very_eager().decode()
    #                     output += output2
    #                     write_serial_output(output)
    #                 rtc_string = re.findall(r'\w\w\w\s?\s\d?\d?\d?\s\s?\d\d:\d\d:\d\d\s\d\d\d\d', output)
    #                 license_regex = re.findall(r'SK1D-', output)
    #                 mac_regex2 = re.findall(r'Outband MAC Address : (?:[0-9a-fA-F]:?){12}', output)
    #
    #                 mac_regex = re.findall(r'(?:[0-9a-fA-F]:?){12}', mac_regex2[0])
    #                 print(mac_regex2)
    #                 print(mac_regex)
    #                 regex_Ycode = re.findall(r'Y00-\d\d\w[/]\d\d.\w\d\d?\d?', output)
    #                 regex_Yserial = re.findall(r'[0]?3[1-2]\d\d\d\d\d\d\d\d\d\d\d?', output)
    #                 regex_fse_version = re.findall(ulgx80_fse_version, output)
    #                 print("REGEX fse_VERSION " + regex_fse_version[0])
    #                 regex_10gb_status = re.findall(r'10000mbps', output)
    #                 print(regex_10gb_status)
    #                 print(check_system_label.config(text=f"{'PASS'}", bg=green_color))
    #                 if mac_regex:
    #                     print(mac_info_output_label.config(text=f"{mac_regex[0]}"))
    #                     resultList[7] = mac_regex[0]
    #                     print(mac_regex)
    #                 if not mac_regex:
    #                     result_list.append('FAIL')
    #                 if regex_Ycode:
    #                     print(Ycode_output_label.config(text=f"{regex_Ycode[0]}"))
    #                     resultList[6] = regex_Ycode[0]
    #                     print(regex_Ycode)
    #                 if not regex_Ycode:
    #                     result_list.append('FAIL')
    #                 if regex_Yserial:
    #                     print(Yserial_output_label.config(text=f"{regex_Yserial[0]}"))
    #                     resultList[5] = regex_Yserial[0]
    #                     print(regex_Yserial)
    #                 if not regex_Yserial:
    #                     result_list.append('FAIL')
    #                 if regex_fse_version:
    #                     print(fse_version_info_output_label.config(text=f"{regex_fse_version[0]}"))
    #                 if rtc_string:
    #                     print(RTC_info_output_label.config(text=f"{rtc_string[0]}"))
    #                     print(rtc_string[0])
    #                 if not license_regex:
    #                     print(License_info_output_label.config(text=f"{'No Active License'}", fg='black', bg=red_color))
    #                     result_list.append('FAIL')
    #                 if license_regex:
    #                     print(License_info_output_label.config(text=f"{'Active License'}", fg='black'))
    #                 if regex_10gb_status:
    #                     print(TENG_info_output_label.config(text=f"{'10Gbit Active'}", fg='black'))
    #                 if not regex_10gb_status:
    #                     print(TENG_info_output_label.config(text=f"{'10Gbit Not Active'}", fg='black', bg=red_color))
    #                     result_list.append('FAIL')
    #                 if 'FAIL' in result_list:
    #                     print(check_system_label.config(text=f"{'FAIL'}", fg='black', bg=red_color))
    #                     print(result_list)
    #                     resultList[32] = 'FAIL'
    #             except Exception as e:
    #                 print("ERROR: " + str(e))
    #                 write_serial_output(str(e))
    #                 warning_message('Warning', str(e))
    #     else:
    #         pass

    def check_system_version(self, version):
        output = ''
        try:
            login = self.admin_login()
            if login == 1:
                self.send_cmd('get system info')
                time.sleep(0.5)
                output2 = self.tn.read_very_eager().decode()
                output += output2
                write_serial_output(output)
                regex_fse_version = re.findall(version, output)
                print("REGEX fse_VERSION " + regex_fse_version[0])
                if regex_fse_version:
                    return regex_fse_version[0]
                else:
                    return 0
            elif login == 2:
                print(fse_check_label.config(text=f"{'FAIL'}", fg='black', bg=red_color))
        except Exception as e:
            write_serial_output(e)

    def p110_telnet_test(self):
        state = telnet_state.get()
        if state == 1:
            try:
                print(p110_telnet_label.config(text=f"{'Running...'}", bg=yellow_color))
                self.admin_login()
                self.send_cmd("ping ip 10.10.10.100 repeat 3")
                time.sleep(4)
                output = self.tn.read_very_eager().decode()
                write_serial_output(output)
                if "3 packets transmitted, 3 received, 0% packet loss" in output:
                    print(p110_telnet_label.config(text=f"{'PASS'}", bg=green_color))
                    resultList[29] = "PASS"
                else:
                    print("FAIL")
                    print(p110_telnet_label.config(text=f"{'FAIL'}", bg=red_color))
                    resultList[29] = "FAIL"
            except Exception as e:
                print("ERROR: " + str(e))
                write_serial_output(str(e))
                warning_message('Warning', str(e))
        else:
            pass

    def change_pll_locked_source(self):
        command_list = ['modify system clockinfo operationmode manual', 'modify system clockinfo clocksource combo1']
        try:
            for command in command_list:
                self.send_cmd(command)
                time.sleep(2)
                output = self.tn.read_very_eager().decode()
                print(output)
                write_serial_output(output)
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def get_clockinfo(self, result_string):
        try:
            synclist = []
            for i in range(10):
                clock_sync_info = 'get system clockinfo'
                self.send_cmd(clock_sync_info)
                output = self.tn.read_very_eager().decode()
                print(output)
                write_serial_output(output)
                time.sleep(6)
                if result_string in output:
                    synclist.append("PASS")
                    break
                else:
                    synclist.append("FAIL")
            if 'PASS' in synclist:
                sync_in_out_results.append("PASS")
            else:
                sync_in_out_results.append("FAIL")
            print(synclist)
        except Exception as e:
            print("ERROR: " + str(e))
            write_serial_output(str(e))
            warning_message('Warning', str(e))

    def sync_in_out_test(self):
        state = sync_in_out_state.get()
        if state == 1:
            print(sync_in_out_label.config(text=f"{'Running...'}", bg=yellow_color))
            self.admin_login()
            time.sleep(1)
            # set_freq_ampl()
            # time.sleep(2)
            # check_enable_signals()
            trafficON_port1()
            time.sleep(2)
            self.change_pll_locked_source()
            time.sleep(1)
            self.get_clockinfo("DPLL  Status : locked")
            time.sleep(1)
            traffic_test_OFF()
            # turn_off()
            sync_eth_source_results()
            print(sync_in_out_results)
        else:
            pass


def graphics_delay(graphics, time_delay):
    time_delay = time_delay
    for i in range(time_delay):
        time_delay = time_delay - 1
        time.sleep(1)
        print(time_delay)
        print(graphics.config(text=f"{'Wait {}s'.format(time_delay)}", bg=yellow_color))




def reset_variables():
    global diag_result
    global diag_login_output
    global diag_OK_list
    global diag_PASSED_list
    global label_choice
    global fse_login_output
    global fse_error_outout
    global fse_loop_count
    global fse_pass_output
    diag_result = 0
    diag_login_output = 0
    diag_OK_list = 0
    diag_PASSED_list = 0
    label_choice = ""
    fse_login_output = 0
    fse_error_outout = 0
    fse_loop_count = 0
    fse_pass_output = 0


def reset_list():
    global resultList
    resultList.clear()
    resultList = copy.deepcopy(resultList2)


def UserID_result():
    resultList[3] = User_ID_input()
    print(resultList)


def alltests_bar():
    for i in range(101):
        time.sleep(0.2)
        AllTests_progressbar['value'] = i
        master.update_idletasks()


def clear_entry(event, entry):
    entry.delete(0, END)


def admin_login():
    try:
        ser = serial.Serial(usb_serial, timeout=1)
        ser.baudrate = 115200
        ser.write(b'\r')
        while True:
            output = ser.read(1000).decode('UTF-8', 'ignore')
            print(output)
            write_serial_output(output)
            if "ul-gx80 login:" in output:
                time.sleep(1)
                ser.write("admin\r".encode())
            if "[root@omnibas:~]#" in output:
                time.sleep(0.1)
                ser.write('exit\r'.encode())
            if 'WinMon>' in output:
                ser.write('boot\r'.encode())
            if "omnibas login:" in output:
                time.sleep(10)
                ser.write(b"admin")
                time.sleep(0.5)
                ser.write('\r'.encode())
            elif "Username:" in output:
                time.sleep(10)
                ser.write(b"admin")
                time.sleep(0.1)
                ser.write('\r'.encode())
            elif 'Password:' in output:
                time.sleep(7)
                ser.write(b"admin")
                time.sleep(0.1)
                ser.write('\r'.encode())
            elif 'UL-GX80>' in output:
                break
    except Exception as e:
        print("ERROR: " + str(e))
        write_serial_output(str(e))
        warning_message('Warning', str(e))


def enable10gb_check():
    try:
        ser = serial.Serial(usb_serial, timeout=1)
        ser.baudrate = 115200
        ser.write(b"modify ethernet state ifname tengbe1 autoneg disabled speed 10000mbps duplex full\r ")
        output = ser.read(1000).decode('UTF-8', 'ignore')
        if 'Extended error #0183: Operation is not licensed' in output:
            print(Traffic_label.config(text=f"{'FAIL'}", bg='#ff0000'))
            write_serial_output('Extended error #0183: Operation is not licensed, please enable the license! ')
        if 'Speed setting of a tengbe port is applied to all tengbe ports' in output:
            output = ser.read(1000).decode('UTF-8', 'ignore')
            print(output)
    except Exception as e:
        print("ERROR: " + str(e))
        write_serial_output(str(e))
        warning_message('Warning', str(e))


def enable10gb_set():
    try:
        ser = serial.Serial(usb_serial, timeout=1)
        ser.baudrate = 115200
        ser.write(b"modify ethernet state ifname tengbe1 autoneg disabled speed 10000mbps duplex full\r ")
        output = ser.read(1000).decode('UTF-8', 'ignore')
        write_serial_output(output)
        if 'Proceed to modify?' in output:
            ser.write(b'y\r')
            output = ser.read(1000).decode('UTF-8', 'ignore')
            write_serial_output(output)
            print(Traffic_label.config(text=f"{'Rebooting...'}", bg=yellow_color))
        if 'Extended error #0183: Operation is not licensed' in output:
            print(Traffic_label.config(text=f"{'FAIL'}", bg='#ff0000'))
            write_serial_output('Extended error #0183: Operation is not licensed, please enable the license! ')
    except Exception as e:
        print("ERROR: " + str(e))
        write_serial_output(str(e))
        warning_message('Warning', str(e))


def enable10gb():
    enable10gb_set()
    time.sleep(2)
    enable10gb_check()






# def traffic_test():
#     state = traffic_state.get()
#     if state == 1:
#         admin_login()
#         enable10gb()
#         time.sleep(10)
#         admin_login()
#         set_traffic_test()
#         # warning_message("Warning", "Check if the orange LEDs on the ethernet ports from GbE8 to GbE5 are turned on and if the green LEDs turn on and blink, in the next 60 seconds.")
#         # set_traffic_mt1000a_thread() #mt1000a
#         # mt1000a_traffic_test() #mt1000a
#         # leds_on_eth_test()
#         jesd_test_set()
#         mts5800_traffic_test()
#         jesd_test_result()
#         mts5800_traffic_test_show_result()
#     else:
#         pass


#
# def alltests_bar():
#     for i in range(101):
#         time.sleep(0.2)
#         AllTests_progressbar['value'] = i
#         master.update_idletasks()


def traffic_test_OFF():
    mts.selectApp(mts.app1)
    mts.optic_off()
    # mts.selectApp(mts.app2)
    # mts.toggleTrafficOFF()


def trafficON_port1():
    mts.selectApp(mts.app1)
    mts.optic_on()

#
# def trafficON_port2():
#     mts.selectApp(mts.app2)
#     mts.toggleTrafficON()

# s5800r.send(b':SENS:TEST:DUR 120\n')

def mts5800_main_tests(data, resultlist_number, test_name):
    try:
        if data == "1":
            print(test_name + ' : PASS')
            write_serial_output(test_name + ' : PASS')
            write_traffic_tester_output(test_name + ' : PASS')
            resultList[resultlist_number] = 'PASS'
        elif data == '0':
            print(test_name + ' : FAIL')
            write_serial_output(test_name + ' : FAIL')
            write_traffic_tester_output(test_name + ' : FAIL')
            resultList[resultlist_number] = 'FAIL'
        else:
            print(test_name + ' : Error')
            write_serial_output(test_name + ' : Error')
            write_serial_output(str(data))
            write_traffic_tester_output(test_name + ' : Error')
            write_traffic_tester_output(str(data))
            resultList[resultlist_number] = 'Error'
    except TimeoutError:
        print('MTS 5800 Not connected')
        print(Indicator_label.config(text=f"{'Connect MTS 5800 and restart the test!'}", bg=red_color))
        warning_message("Warning", "MTS5800 not connected! Please connect it and restart the program!")
        write_traffic_tester_output("MTS5800 not connected! Please connect it and restart the program!")
        write_errors_output("MTS5800 not connected! Please connect it and restart the program!")


def mts5800_secondary_tests(data,  resultlist_number, test_name):
    try:
        if data != '0':
            print(test_name + ' : FAIL')
            resultList[resultlist_number] = 'FAIL'
            write_serial_output(test_name + ' : FAIL ' + "(Number of errors: " + str(data) + ")")
        else:
            print(test_name + ' : PASS')
            resultList[resultlist_number] = 'PASS'
            write_serial_output(test_name + ' : PASS')
    except Exception as e:
        print("type error: " + str(e))
        write_serial_output(str(e))
        warning_message('Warning', str(e))
        print(Indicator_label.config(text=f"{str(e)}", bg=red_color))
        print(Traffic_label.config(text=f"{'FAIL'}", bg=red_color))




def run_tests_port1():
    sync_active_port1()
    time.sleep(0.3)
    link_active_port1()
    time.sleep(0.3)
    frame_detect_port1()
    time.sleep(0.3)
    pattern_sync_port1()
    time.sleep(0.3)
    sync_loss_port1()
    time.sleep(0.3)
    link_loss_port1()
    time.sleep(0.3)
    fcs_errored_frames_port1()
    time.sleep(0.3)
    bit_errors_port1()
    time.sleep(0.3)
    jabbers_port1()



def sync_active_port1():
    sync_active_data = mts.reqData(":SENSE:DATA? CSTatus:PCS:PHY:SYNC:ACTive")
    mts5800_main_tests(sync_active_data, 18, "Sync Acquired")


def link_active_port1():
    link_active_data = mts.reqData(":SENSE:DATA? CSTatus:PCS:PHY:LINK:ACTive")
    mts5800_main_tests(link_active_data, 19, "Link Active")


def frame_detect_port1():
    frame_detect_data = mts.reqData(":SENSe:DATA? CSTatus:MAC:ETH:FRAMe:DETect")
    mts5800_main_tests(frame_detect_data, 20, "Frame Detect")


def pattern_sync_port1():
    pattern_sync_data = mts.reqData(":SENSe:DATA? CSTatus:MAC:L2:PATTern:SYNC")
    mts5800_main_tests(pattern_sync_data, 21, "Pattern Sync")


def sync_loss_port1():
    sync_loss_data = mts.reqData(":SENSe:DATA? ESECOND:PCS:PHY:SYNC:LOSS")
    mts5800_secondary_tests(sync_loss_data, 22, "Sync Loss")


def link_loss_port1():
    link_loss_data = mts.reqData(":SENSe:DATA? ESECOND:PCS:PHY:LINK:LOSS")
    mts5800_secondary_tests(link_loss_data, 23, "Link Loss")


def fcs_errored_frames_port1():
    fcs_errored_frames_data = mts.reqData(":SENSe:DATA? ECOUNT:MAC:ETH:FCS:FRAME")
    mts5800_secondary_tests(fcs_errored_frames_data, 24, "FCS Errored Frames")


def bit_errors_port1():
    bit_errors_data = mts.reqData(":SENSe:DATA? ECOUNT:MAC:L2:TSE")
    mts5800_secondary_tests(bit_errors_data, 25, "Bit Errors ")


def jabbers_port1():
    jabbers_data = mts.reqData(":SENSe:DATA? ECOUNT:MAC:ETH:JABBER")
    mts5800_secondary_tests(jabbers_data, 26, "Jabbers")





def mts5800_traffic_test():
    print(Traffic_label.config(text=f"{'Running...'}", bg=yellow_color))
    # mts.set_mac_port2()
    mts.set_mac_port1()
    trafficON_port1()
    time.sleep(1)
    mts.selectApp(mts.app1)
    trafficON_port1()
    time.sleep(15)
    mts.restartPort()
    time.sleep(60)
    mts.selectApp(mts.app1)
    write_serial_output('\nPort 1 results:\n')
    write_traffic_tester_output('\nPort 1 results:\n')
    run_tests_port1()
    time.sleep(1)
    # mts.selectApp(mts.app2)
    # write_serial_output('\nPort 2 results:\n')
    # run_tests_port2()
    traffic_test_OFF()
    # mts5800_traffic_test_show_result()


def mts5800_traffic_test_show_result():
    trafficList = resultList[18:27]
    print(trafficList)
    if 'FAIL' in trafficList:
        print(Traffic_label.config(text=f"{'FAIL'}", bg=red_color))
    else:
        print(Traffic_label.config(text=f"{'PASS'}", bg=green_color))


# def set_traffic_mt1000a_thread():
#     if __name__ == '__main__':
#         t1 = Thread(target=set_traffic_test)
#         t2 = Thread(target=mt1000a_traffic_set)
#         t1.start()
#         t2.start()
#         t1.join()
#         t2.join()
#
#
# def mt1000a_tests(command, resultlist_number, test_name):
#     try:
#         output = command
#         if output == "0":
#             print(test_name + ' : PASS')
#             write_serial_output(test_name + ' : PASS')
#             resultList[resultlist_number] = 'PASS'
#         elif output != 0:
#             print(test_name + ' : FAIL')
#             write_serial_output(test_name + ' : FAIL')
#             write_serial_output(str(output))
#             resultList[resultlist_number] = 'FAIL'
#         else:
#             print(test_name + ' : Error')
#             write_serial_output(test_name + ' : Error')
#             write_serial_output(output)
#             resultList[resultlist_number] = 'Error'
#     except Exception as e:
#         print("ERROR: " + str(e))
#         write_serial_output(str(e))
#         print('MTS1000A Not connected')
#         print(Indicator_label.config(text=f"{'Connect MT1000A and restart the test!'}", bg=red_color))
#         warning_message("Warning","'MT1000A' not connected, you will not be able to run the traffic test. After you connect it restart the program!")
#
#
# def run_tests_port1():
#     mt1000a_tests(mt1000a.reqData("ETH:PORT1:IFET? (BPE)"), 18, "Pattern Errors")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT1:IFET? (BSE)"), 19, "Sequence Errors")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT1:IFET? (BSSL)"), 20, "Sequence Sync")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT1:IFET? (BFL)"), 21, "Frame Loss")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT1:IFET? (BFLS)"), 22, "Frame Loss Secs")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT1:IFET? (LOS)"), 23, "Loss of signal")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT1:IFET? (NLS)"), 24, "No Link Seconds")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT1:IFET? (FFR)"), 25, "Fragmented Frames")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT1:IFET? (FEFR)"), 26, "FCS Errored Frames")
#
#
# def run_tests_port2():
#     mt1000a_tests(mt1000a.reqData("ETH:PORT2:IFET? (BPE)"), 32, "Pattern Errors")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT2:IFET? (BSE)"), 33, "Sequence Errors")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT2:IFET? (BSSL)"), 34, "Sequence Sync")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT2:IFET? (BFL)"), 35, "Frame Loss")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT2:IFET? (BFLS)"), 36, "Frame Loss Secs")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT2:IFET? (LOS)"), 37, "Loss of signal")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT1:IFET? (NLS)"), 38, "No Link Seconds")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT2:IFET? (FFR)"), 39, "Fragmented Frames")
#     mt1000a_tests(mt1000a.reqData("ETH:PORT2:IFET? (FEFR)"), 40, "FCS Errored Frames")
#
#
# def mt1000a_traffic_set():
#     print(Traffic_label.config(text=f"{'Loading...'}", bg=yellow_color))
#     mt1000a.bert_check_sfp_eth_1gb()
#     time.sleep(10)
#
#
# def mt1000a_traffic_test():
#     print(Traffic_label.config(text=f"{'Running...'}", bg=yellow_color))
#     mt1000a.toggleTrafficON()
#     time.sleep(60)
#     mt1000a.toggleTrafficOFF()
#     write_serial_output('\nPort 1 results:\n')
#     run_tests_port1()
#     write_serial_output('\nPort 2 results:\n')
#     run_tests_port2()
#     mt1000a_traffic_test_show_result()
#
#
# def mt1000a_traffic_test_show_result():
#     trafficList = resultList[23:41]
#     print(trafficList)
#     if 'FAIL' in trafficList:
#         print(Traffic_label.config(text=f"{'FAIL'}", bg=red_color))
#     else:
#         print(Traffic_label.config(text=f"{'PASS'}", bg=green_color))


def alltests_thread():
    if __name__ == '__main__':
        Thread(target=alltests_bar).start()
        Thread(target=main).start()


def check_for_reset():
    state1 = mac_address_state.get()
    state2 = fse_state.get()
    state3 = diag_state.get()
    if state1 == 1 or state2 == 1 or state3 == 1:
        warning_reboot()
    else:
        pass

def check_for_g5_fse():
    state1 = fse_swap_state.get()
    state2 = license_rtc_g5dualbs_state.get()
    state3 = norfu_state.get()
    state4 = table_state.get()
    state5 = rm_norfu_state.get()
    state6 = check_system_state.get()
    if state1 == 1 or state2 == 1 or state3 == 1 or state4 == 1 or state5 == 1 or state6 == 1:
        return 1
    else:
        return 0

def sync_eth_source_results():
    if 'FAIL' in sync_in_out_results:
        print(sync_in_out_label.config(text=f"{'FAIL'}", bg=red_color))
        resultList[30] = "FAIL"
    else:
        print(sync_in_out_label.config(text=f"{'PASS'}", bg=green_color))
        resultList[30] = 'PASS'

def telnet_check():
    state1 = set_normal_mode_state.get()
    state2 = telnet_state.get()
    state3 = sync_in_out_state.get()
    state4 = license_state.get()
    # state5 = check_system_state.get()
    if state1 == 1 or state2 == 1 or state3 == 1 or state4 == 1:
        return 1
    else:
        return 0

# def test_mode_tests_check():
#     state1 = set_normal_mode_state.get()
#     state2 = telnet_state.get()
#     state3 = sync_in_out_state.get()
#     state4 = license_state.get()
#     state5 = check_system_state.get()
#     if state1 or state2 or state3 or state4 or state5 == 1:
#         return 1
#     else:
#         return 0



def test_finalization():
    final_result()
    timeResult_end()
    save_txt_results()
    save_results()
    warning_message('Warning',
                    "Test finished, to test a new card enter a new Ycode and Serial, and press 'Run Tests'! ")




def main():
    serial = SerialClass(eth_serial, 115200)
    check_for_reset()
    reset_gui()
    print(Indicator_label.config(text=f"{'Test running, please wait...'}", fg='black', bg=yellow_color))
    timeResult()
    serial.mac_addresses_result()
    serial.diag_thread()
    serial.fse_ULGX80()
    serial.fse_check()
    serial.set_test_mode(1)
    serial.mng_port_test()
    serial.get_yserial()
    serial.ycode_test()
    serial.hwid_test()
    serial.temp_test()
    serial.rtc_read()
    serial.pll_test()
    serial.reset_program_dpll()
    serial.fpga_test()
    serial.traffic_test()
    serial.xpic_check()
    test_mode_tests_result = ulgx80_testmode_tests_result()
    if test_mode_tests_result == "PASS":
        print(Indicator_label.config(text=f"{'Test running, please wait...'}", fg='black', bg=yellow_color))
        normal_mode_result = serial.set_normal_mode()
        telnet_tests_check = telnet_check()
        if normal_mode_result == "PASS":
            if telnet_tests_check == 1:
                telnet = TelnetClass('192.168.1.100', 23, 3)
                telnet.p110_telnet_test()
                telnet.sync_in_out_test()
                telnet.set_license_ulgx80()
            else:
                pass
        else:
            pass
        ulgx80_test_results = ulgx80_tests_result()
        g5_check = check_for_g5_fse()
        if g5_check == 1:
            if ulgx80_test_results == 'PASS':
                print(Indicator_label.config(text=f"{'Test running, please wait...'}", fg='black', bg=yellow_color))
                serial.fse_swap()
                serial.create_norfu()
                serial.license_rtc_g5dualbs()
                serial.table_upload()
                serial.rm_norfu()
                serial.check_system()
                test_finalization()
            elif ulgx80_test_results == "FAIL":
                result = messagebox.askquestion("Warning", "One or more of the tests failed, press YES if you want to continue with the G5 FSE, or NO if you want to rerun the tests")
                if result == "yes":
                    serial.fse_swap()
                    serial.create_norfu()
                    serial.license_rtc_g5dualbs()
                    serial.table_upload()
                    serial.rm_norfu()
                    serial.check_system()
                    test_finalization()
                elif result == "no":
                    test_finalization()
            else:
                test_finalization()
    elif test_mode_tests_result == "FAIL":
            telnet_tests_check = telnet_check()
            g5_check = check_for_g5_fse()
            if g5_check == 1 or telnet_tests_check == 1:
                result = messagebox.askquestion("Warning",
                                            "One or more of the Test Mode Tests, press YES if you want to continue with the Normal Mode Tests, or NO if you want to rerun the tests!")

                if result == "yes":
                    normal_mode_result = serial.set_normal_mode()
                    telnet_tests_check = telnet_check()
                    if normal_mode_result == "PASS":
                        if telnet_tests_check == 1:
                            telnet = TelnetClass('192.168.1.100', 23, 3)
                            telnet.p110_telnet_test()
                            telnet.sync_in_out_test()
                            telnet.set_license_ulgx80()
                        else:
                            pass
                    serial.fse_swap()
                    serial.create_norfu()
                    serial.license_rtc_g5dualbs()
                    serial.table_upload()
                    serial.rm_norfu()
                    serial.check_system()
                    test_finalization()
                elif result == "no":
                    test_finalization()
            else:
                test_finalization()


master = Tk()

GUI_WIDTH = 1160
GUI_HEIGHT = 740

gui_resolution = str(GUI_WIDTH) + 'x' + str(GUI_HEIGHT) + '+0+0'
master.geometry(gui_resolution)
master.resizable(FALSE, FALSE)
master.title("G5DualBS BBU Functional Test")


testsFrame = LabelFrame(master, width=GUI_WIDTH/4, height=GUI_HEIGHT*.95, text='Tests')
testsFrame.grid(column=0, row=0, sticky=N)


# test_normalFrame = LabelFrame(master, width=GUI_WIDTH / 4, height=GUI_HEIGHT * .45, text='Normal Mode Tests')
# test_normalFrame.grid(column=1, row=0, sticky=N)



tests_tab = []
rb = []

tests_nb = ttk.Notebook(testsFrame)


test_mode_tests = Frame(tests_nb)
tests_nb.add(test_mode_tests, text="Test Mode Tests")
tests_tab.append(test_mode_tests)

normal_mode_tests = Frame(tests_nb)
tests_nb.add(normal_mode_tests, text="Normal Mode Tests")
tests_tab.append(normal_mode_tests)

G5DualBS_tests = Frame(tests_nb)
tests_nb.add(G5DualBS_tests, text="G5DualBS Tests")
tests_tab.append(G5DualBS_tests)

tests_nb.grid(column=0, row=0, columnspan=4)


monFrame = LabelFrame(master, width=GUI_WIDTH * 2, height=GUI_HEIGHT * .55, text='Serial Output')
monFrame.grid(column=2, row=0, sticky=N)



col = 0
mon_nb = ttk.Notebook(monFrame)
mon_nb_tab1 = Frame(mon_nb)
mon_nb_tab2 = Frame(mon_nb)
mon_nb_tab3 = Frame(mon_nb)
mon_nb.add(mon_nb_tab1, text="UUT Serial Output")
mon_nb.add(mon_nb_tab2, text="Traffic Tester Output")
mon_nb.add(mon_nb_tab3, text="Errors Output")

mon_nb.grid(column=4, row=0, columnspan=4)
results = scrolledtext.ScrolledText(mon_nb_tab1, width=60, height=30, bg='lightcyan', fg='black')
results.grid(column=(col + len(GroupTests)), row=0, padx=30, pady=30)
results.insert(END, 'Waiting to start ...\n')
port1 = scrolledtext.ScrolledText(mon_nb_tab2, width=60, height=30, bg='lightblue', fg='black')
port1.grid(column=(col + len(GroupTests)), row=0, padx=30, pady=30)
port1.insert(END, 'Waiting to connect ...\n')
port2 = scrolledtext.ScrolledText(mon_nb_tab3, width=60, height=30, bg='#afd3a3',
                                          fg='black')  # lightblue was lightgray
port2.grid(column=(col + len(GroupTests)), row=0, padx=30, pady=30)
port2.insert(END, 'Waiting to connect ...\n')


def write_serial_output(string):
    results.config(state=tk.NORMAL)
    results.insert("end", string + "\n")
    results.see("end")
    results.config(state=tk.DISABLED)


def write_traffic_tester_output(string):
    port1.config(state=tk.NORMAL)
    port1.insert("end", string + "\n")
    port1.see("end")
    port1.config(state=tk.DISABLED)


def write_errors_output(string):
    port2.config(state=tk.NORMAL)
    port2.insert("end", string + "\n")
    port2.see("end")
    port2.config(state=tk.DISABLED)


configFrame = LabelFrame(master, width=GUI_WIDTH / 4, height=GUI_HEIGHT * .95, text='Configuration')
configFrame.grid(column=3, row=0, sticky=N)
sysinfoFrame = LabelFrame(master, width=GUI_WIDTH / 5, height=GUI_HEIGHT * .30, text='System Info')
sysinfoFrame.grid(column=3, row=0, sticky=SW)
# indicatorFrame = LabelFrame(master, width=GUI_WIDTH/5, height=GUI_HEIGHT*.30, text='System Info')
# indicatorFrame.grid(column=2, row=0, sticky=N)

Indicator_label = Label(master, width=35, height=3, relief=SUNKEN, text='Test Status: IDLE')
Indicator_label.grid(column=0, row=2, sticky=SW, padx=5, pady=5, columnspan=3)

print(Indicator_label.config(text=f"{'Waiting to start...'}", fg='black'))




def on_select(event=None):
    print('----------------------------')

    if event:  # <-- this works only with bind because `command=` doesn't send event
        print("event.widget:", event.widget.get())
        resultList[6] = event.widget.get()

    for i, x in enumerate(all_comboboxes):
        print("all_comboboxes[%d]: %s" % (i, x.get()))
        resultList[6] = event.widget.get()
        print(resultList[6])
        write_serial_output('Ycode: ' + resultList[6] + '\n')

    # entrytext_Ycode = tk.StringVar()
    # entry_Ycode = tk.Entry(configFrame, textvariable=entrytext_Ycode)
    # entry_Ycode.grid(column=1, row=1, sticky=W, pady=5)
    # entry_Ycode.bind("<Button-1>", lambda event: clear_entry(event, entry_Ycode))
    # entry_Ycode.bind('<Return>', lambda _: Ycode_input())

all_comboboxes = []

cb = ttk.Combobox(configFrame, values=(ycode_list))
cb.set("")
cb.grid(column=1, row=0, sticky=W, pady=5)
cb.bind('<<ComboboxSelected>>', on_select)

all_comboboxes.append(cb)

Ycode_entry_label = Label(configFrame, text="Select YCode")
Ycode_entry_label.grid(column=0, row=0, sticky=W, pady=5)


def Ycode_input():
    entry_Ycode = entrytext_Ycode.get()
    regex_Ycode = re.findall(r'Y00-\d\d\w[/]\d\d.\w\d\d?\d?', entry_Ycode)
    if regex_Ycode:
        Ycode = regex_Ycode[0]
        write_serial_output('Ycode: ' + Ycode + '\n')
        resultList[6] = Ycode
    elif not regex_Ycode:
        resultList[6] = '-'
        warning_message("Warning", "Wrong Ycode entered!")


Ycode_entry_label = Label(configFrame, text="Enter YCode")
Ycode_entry_label.grid(column=0, row=1, sticky=W, pady=5)


entrytext_Ycode = tk.StringVar()
entry_Ycode = tk.Entry(configFrame, textvariable=entrytext_Ycode)
entry_Ycode.grid(column=1, row=1, sticky=W, pady=5)
entry_Ycode.bind("<Button-1>", lambda event: clear_entry(event, entry_Ycode))
entry_Ycode.bind('<Return>', lambda _: Ycode_input())


def Yserial_input():
    entry_serial = entrytext_YSerial.get()
    regex_Yserial = re.findall(r'[0]?3[1-2]\d\d\d\d\d\d\d\d\d\d\d?', entry_serial)
    if regex_Yserial:
        Yserial = regex_Yserial[0]
        if len(regex_Yserial[0]) == 12:
            Yserial = regex_Yserial[0]
            print(Yserial)
            write_serial_output('Yserial Entered: ' + Yserial + '\n')
            resultList[5] = Yserial
            print(resultList)
        if len(regex_Yserial[0]) == 13:
            warning_message("Warning", "Wrong serial number!")
        if len(regex_Yserial[0]) == 14:
            Yserial = Yserial[1:13]
            write_serial_output('Yserial Entered: ' + Yserial + '\n')
            resultList[5] = Yserial
    elif not regex_Yserial:
        resultList[5] = '-'
        warning_message("Warning", "Wrong serial number!")


Yserial_entry_label = Label(configFrame, text="Enter YSerial")
Yserial_entry_label.grid(column=0, row=2, sticky=W, pady=5)


entrytext_YSerial = tk.StringVar()
entry_Yserial = tk.Entry(configFrame, textvariable=entrytext_YSerial)
entry_Yserial.grid(column=1, row=2, sticky=W, pady=5)
entry_Yserial.bind("<Button-1>", lambda event: clear_entry(event, entry_Yserial))
entry_Yserial.bind('<Return>', lambda _: Yserial_input())


def User_ID_input():
    entry_UserID = entrytext_UserID.get()
    write_serial_output('User ID: ' + entry_UserID)
    resultList[3] = entry_UserID
    print(entry_UserID)
    return entry_UserID


UserID_entry_label = Label(configFrame, text="Enter User ID")
UserID_entry_label.grid(column=0, row=3, sticky=W, pady=5)


entrytext_UserID = tk.StringVar()
entry_UserID = tk.Entry(configFrame, textvariable=entrytext_UserID)
entry_UserID.grid(column=1, row=3, sticky=W, pady=5)
entry_UserID.bind("<Button-1>", lambda event: clear_entry(event, entry_UserID))
entry_UserID.bind('<Return>', lambda _: User_ID_input())


button_Runalltests = tk.Button(configFrame, text="RUN TESTS", bg='#E1C699', command=alltests_thread)
button_Runalltests.grid(column=0, row=4, sticky=W, pady=10)


def select_deselect():
    if btn_rf1['text'] == "Deselect All Tests":
        btn_rf1.configure(text="Select All Tests", bg='#E1C699')
        deselect_all()
    else:
        btn_rf1.configure(text="Deselect All Tests", bg='#E1C699')
        select_all()


# btn_rf1 = Button(normal_mode_tests, text="Deselect All Tests", bg='#E1C699', command=select_deselect)
# btn_rf1.grid(column=0, row=17, sticky=W, pady=10)
btn_rf1 = Button(master, text="Deselect All Tests", bg='#E1C699', command=select_deselect)
btn_rf1.grid(column=0, row=1, sticky=SW, padx=5, pady=5, columnspan=3)

# Indicator_label.grid(column=0, row=2, sticky=SW, padx=5, pady=5, columnspan=3)


# text_box = tk.Text(monFrame, state=tk.DISABLED, width=60, height=35)
# text_box.grid(row=0, column=0, padx=20, pady=20)


mac_address_state = IntVar()
checkbox_MAC = tk.Checkbutton(test_mode_tests, text="Set MAC Address", variable=mac_address_state)
checkbox_MAC.select()
checkbox_MAC.grid(row=0, column=0, sticky='W', padx=5, pady=5)


diag_state = IntVar()
checkbox_Diag = tk.Checkbutton(test_mode_tests, text="Diag Test", variable=diag_state)
checkbox_Diag.select()
checkbox_Diag.grid(row=1, column=0, sticky='W', padx=5, pady=5)


fse_state = IntVar()
checkbox_fse = tk.Checkbutton(test_mode_tests, text="FSE GX80", variable=fse_state)
checkbox_fse.grid(row=2, column=0, sticky='W', padx=5, pady=5)

fse_check_state = IntVar()
checkbox_fse_check = tk.Checkbutton(test_mode_tests, text="FSE Check", variable=fse_check_state)
checkbox_fse_check.select()
checkbox_fse_check.grid(row=3, column=0, sticky='W', padx=5, pady=5)


set_test_mode_state = IntVar()
checkbox_set_test_mode = tk.Checkbutton(test_mode_tests, text="Set Test Mode", variable=set_test_mode_state)
checkbox_set_test_mode.select()
checkbox_set_test_mode.grid(row=4, column=0, sticky='W', padx=5, pady=5)


management_port_state = IntVar()
checkbox_serial_eth_interface = tk.Checkbutton(test_mode_tests, text="Management Port", variable=management_port_state)
checkbox_serial_eth_interface.select()
checkbox_serial_eth_interface.grid(row=5, column=0, sticky='W', padx=5, pady=5)


yserial_state = IntVar()
checkbox_YSerial_test = tk.Checkbutton(test_mode_tests, text="Yserial Test", variable=yserial_state)
checkbox_YSerial_test.select()
checkbox_YSerial_test.grid(row=6, column=0, sticky='W', padx=5, pady=5)


ycode_state = IntVar()
checkbox_YCode_test = tk.Checkbutton(test_mode_tests, text="Ycode Test", variable=ycode_state)
checkbox_YCode_test.select()
checkbox_YCode_test.grid(row=7, column=0, sticky='W', padx=5, pady=5)


hwid_state = IntVar()
checkbox_HWID = tk.Checkbutton(test_mode_tests, text="Hardware ID", variable=hwid_state)
checkbox_HWID.select()
checkbox_HWID.grid(row=8, column=0, sticky='W', padx=5, pady=5)


temp_state = IntVar()
checkbox_Temp = tk.Checkbutton(test_mode_tests, text="Temp Test", variable=temp_state)
checkbox_Temp.select()
checkbox_Temp.grid(row=9, column=0, sticky='W', padx=5, pady=5)


rtc_state = IntVar()
checkbox_RTC = tk.Checkbutton(test_mode_tests, text="RTC Test", variable=rtc_state)
checkbox_RTC.select()
checkbox_RTC.grid(row=10, column=0, sticky='W', padx=5, pady=5)


pll_state = IntVar()
checkbox_PLL = tk.Checkbutton(test_mode_tests, text="PLL Test", variable=pll_state)
checkbox_PLL.select()
checkbox_PLL.grid(row=11, column=0, sticky='W', padx=5, pady=5)


dpll_state = IntVar()
checkbox_dPLL = tk.Checkbutton(test_mode_tests, text="DPLL Reset/Program", variable=dpll_state)
checkbox_dPLL.select()
checkbox_dPLL.grid(row=12, column=0, sticky='W', padx=5, pady=5)


fpga_state = IntVar()
checkbox_fpga = tk.Checkbutton(test_mode_tests, text="FPGA Test", variable=fpga_state)
checkbox_fpga.select()
checkbox_fpga.grid(row=13, column=0, sticky='W', padx=5, pady=5)


traffic_state = IntVar()
checkbox_traffic_test = tk.Checkbutton(test_mode_tests, text="Traffic Test", variable=traffic_state)
checkbox_traffic_test.select()
checkbox_traffic_test.grid(row=14, column=0, sticky='W', padx=5, pady=5)

xpic_state = IntVar()
checkbox_xpic_test = tk.Checkbutton(test_mode_tests, text="XPIC Test", variable=xpic_state)
checkbox_xpic_test.select()
checkbox_xpic_test.grid(row=15, column=0, sticky='W', padx=5, pady=5)




set_normal_mode_state = IntVar()
checkbox_set_normal_mode = tk.Checkbutton(normal_mode_tests, text="Set Normal Mode", variable=set_normal_mode_state)
checkbox_set_normal_mode.select()
checkbox_set_normal_mode.grid(row=0, column=0, sticky='W', padx=5, pady=5)

telnet_state = IntVar()
checkbox_telnet = tk.Checkbutton(normal_mode_tests, text="P110 Telnet Test", variable=telnet_state)
checkbox_telnet.select()
checkbox_telnet.grid(row=1, column=0, sticky='W', padx=5, pady=5)

sync_in_out_state = IntVar()
checkbox_sync_in_out = tk.Checkbutton(normal_mode_tests, text="Sync In/Out Test", variable=sync_in_out_state)
checkbox_sync_in_out.select()
checkbox_sync_in_out.grid(row=2, column=0, sticky='W', padx=5, pady=5)

license_state = IntVar()
checkbox_license = tk.Checkbutton(normal_mode_tests, text="Set License", variable=license_state)
checkbox_license.select()
checkbox_license.grid(row=3, column=0, sticky='W', padx=5, pady=5)
# bt_enable_state = IntVar()
# checkbox_bt_enable = tk.Checkbutton(test_normalFrame, text="BT Enable", variable=bt_enable_state)
# checkbox_bt_enable.select()
# checkbox_bt_enable.grid(row=4, column=0, sticky='W', padx=5, pady=5)




fse_swap_state = IntVar()
checkbox_fse_swap = tk.Checkbutton(G5DualBS_tests, text="FSE G5DualBS", variable=fse_swap_state)
checkbox_fse_swap.select()
checkbox_fse_swap.grid(row=0, column=0, sticky='W', padx=5, pady=5)

norfu_state = IntVar()
checkbox_norfu = tk.Checkbutton(G5DualBS_tests, text="Create norfu", variable=norfu_state)
checkbox_norfu.select()
checkbox_norfu.grid(row=1, column=0, sticky='W', padx=5, pady=5)

license_rtc_g5dualbs_state = IntVar()
checkbox_license_rtc_g5dualbs = tk.Checkbutton(G5DualBS_tests, text="RTC/License G5", variable=license_rtc_g5dualbs_state)
checkbox_license_rtc_g5dualbs.select()
checkbox_license_rtc_g5dualbs.grid(row=2, column=0, sticky='W', padx=5, pady=5)


table_state = IntVar()
checkbox_table_test = tk.Checkbutton(G5DualBS_tests, text="Upload Table", variable=table_state)
checkbox_table_test.select()
checkbox_table_test.grid(row=3, column=0, sticky='W', padx=5, pady=5)

rm_norfu_state = IntVar()
checkbox_rm_norfu = tk.Checkbutton(G5DualBS_tests, text="Remove norfu", variable=rm_norfu_state)
checkbox_rm_norfu.select()
checkbox_rm_norfu.grid(row=4, column=0, sticky='W', padx=5, pady=5)

check_system_state = IntVar()
checkbox_check_system = tk.Checkbutton(G5DualBS_tests, text="Check System", variable=check_system_state)
checkbox_check_system.select()
checkbox_check_system.grid(row=5, column=0, sticky='W', padx=5, pady=5)

write_serial_output("Steps before you start the test :\n")
write_serial_output('1.Check that the time of the PC is set correctly!\n')
write_serial_output('2.Set the IP of MTS5800 traffic tester to ' + ip_mts5800 + ', if not set already.\n')
# write_serial_output('3.Set the IP of 33220A Function Generator to ' + ip_33220a + ', if not set already.\n')
write_serial_output('3.Power on the BBU\n')
write_serial_output("4.Select or enter Y-code and press enter\n")
write_serial_output("5.Enter Serial Number and press enter\n")
write_serial_output("6.Enter User ID and press enter\n")
write_serial_output('7.Choose the tests by clicking the checkboxes\n')
write_serial_output("8.Press 'Run Tests'\n")
write_serial_output("9.A message will show up, press 'OK' first,\n then reset the BBU.\n")
#
#
# statusBar = Label(master, width=10 * len(GroupTests), relief=SUNKEN, text='Test Status: IDLE')
#
# statusBar.grid(column=0, row=2, sticky=SW, padx=5, pady=20, columnspan=3)  # len(GroupTests)) #3


MAC_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
MAC_label.grid(row=0, column=1, sticky='W', padx=5, pady=5)


Diag_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
Diag_label.grid(row=1, column=1, sticky='W', padx=5, pady=5)


fse_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
fse_label.grid(row=2, column=1, sticky='W', padx=5, pady=5)

fse_check_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
fse_check_label.grid(row=3, column=1, sticky='W', padx=5, pady=5)


set_test_mode_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
set_test_mode_label.grid(row=4, column=1, sticky='W', padx=5, pady=5)




# serial_eth_interface_label = tk.Label(testsFrame, width=10, relief=SUNKEN)
# serial_eth_interface_label.grid(row=4, column=2, sticky='W', padx=5, pady=5)
#
#
# led_serial_eth_interface_label = tk.Label(testsFrame, width=10, relief=SUNKEN)
# led_serial_eth_interface_label.grid(row=4, column=3, sticky='W', padx=5, pady=5)


management_port_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
management_port_label.grid(row=5, column=1, sticky='W', padx=5, pady=5)


Yserial_test_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
Yserial_test_label.grid(row=6, column=1, sticky='W', padx=5, pady=5)


ycode_test_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
ycode_test_label.grid(row=7, column=1, sticky='W', padx=5, pady=5)


HWID_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
HWID_label.grid(row=8, column=1, sticky='W', padx=5, pady=5)


Temp_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
Temp_label.grid(row=9, column=1, sticky='W', padx=5, pady=5)


RTC_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
RTC_label.grid(row=10, column=1, sticky='W', padx=5, pady=5)


PLL_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
PLL_label.grid(row=11, column=1, sticky='W', padx=5, pady=5)


dPLL_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
dPLL_label.grid(row=12, column=1, sticky='W', padx=5, pady=5)


fpga_program_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
fpga_program_label.grid(row=13, column=1, sticky='W', padx=5, pady=5)


fpga_test_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
fpga_test_label.grid(row=13, column=2, sticky='W', padx=5, pady=5)

jesd_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
jesd_label.grid(row=14, column=1, sticky='W', padx=5, pady=5)


Traffic_label = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
Traffic_label.grid(row=14, column=2, sticky='W', padx=5, pady=5)


xpic_label_program = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
xpic_label_program.grid(row=15, column=1, sticky='W', padx=5, pady=5)


xpic_label_test = tk.Label(test_mode_tests, width=10, relief=SUNKEN)
xpic_label_test.grid(row=15, column=2, sticky='W', padx=5, pady=5)




set_normal_mode_label = tk.Label(normal_mode_tests, width=10, relief=SUNKEN)
set_normal_mode_label.grid(row=0, column=1, sticky='W', padx=5, pady=5)

p110_telnet_label = tk.Label(normal_mode_tests, width=10, relief=SUNKEN)
p110_telnet_label.grid(row=1, column=1, sticky='W', padx=5, pady=5)

sync_in_out_label = tk.Label(normal_mode_tests, width=10, relief=SUNKEN)
sync_in_out_label.grid(row=2, column=1, sticky='W', padx=5, pady=5)

license_label = tk.Label(normal_mode_tests, width=10, relief=SUNKEN)
license_label.grid(row=3, column=1, sticky='W', padx=5, pady=5)
# bt_enable_label = tk.Label(test_normalFrame, width=10, relief=SUNKEN)
# bt_enable_label.grid(row=4, column=1, sticky='W', padx=5, pady=5)




fse_swap_label = tk.Label(G5DualBS_tests, width=10, relief=SUNKEN)
fse_swap_label.grid(row=0, column=1, sticky='W', padx=5, pady=5)

norfu_label = tk.Label(G5DualBS_tests, width=10, relief=SUNKEN)
norfu_label.grid(row=1, column=1, sticky='W', padx=5, pady=5)

g5dualbs_license_label = tk.Label(G5DualBS_tests, width=10, relief=SUNKEN)
g5dualbs_license_label.grid(row=2, column=1, sticky='W', padx=5, pady=5)

g5dualbs_RTC_label = tk.Label(G5DualBS_tests, width=10, relief=SUNKEN)
g5dualbs_RTC_label.grid(row=2, column=2, sticky='W', padx=5, pady=5)

table_label = tk.Label(G5DualBS_tests, width=10, relief=SUNKEN)
table_label.grid(row=3, column=1, sticky='W', padx=5, pady=5)

rm_norfu_label = tk.Label(G5DualBS_tests, width=10, relief=SUNKEN)
rm_norfu_label.grid(row=4, column=1, sticky='W', padx=5, pady=5)

check_system_label = tk.Label(G5DualBS_tests, width=10, relief=SUNKEN)
check_system_label.grid(row=5, column=1, sticky='W', padx=5, pady=5)

remaining_mac_info_label = tk.Label(sysinfoFrame, text='Remaining MAC : ')
remaining_mac_info_label.grid(row=0, column=0, sticky='W', pady=5)


mac_info_label = tk.Label(sysinfoFrame, text='MAC Address: ')
mac_info_label.grid(row=1, column=0, sticky='W', pady=5)


Ycode_label = tk.Label(sysinfoFrame, text="Set YCode: ")
Ycode_label.grid(row=2, column=0, sticky='W', pady=5)


Yserial_label = tk.Label(sysinfoFrame, text="Set YSerial: ")
Yserial_label.grid(row=3, column=0, sticky='W', pady=5)


remaining_mac_info_output_label = tk.Label(sysinfoFrame, text='')
remaining_mac_info_output_label.grid(row=0, column=1, sticky='W', pady=5)


mac_info_output_label = tk.Label(sysinfoFrame, text='')
mac_info_output_label.grid(row=1, column=1, sticky='W', pady=5)


Ycode_output_label = tk.Label(sysinfoFrame, text="")
Ycode_output_label.grid(row=2, column=1, sticky='W', pady=5)


Yserial_output_label = tk.Label(sysinfoFrame, text="")
Yserial_output_label.grid(row=3, column=1, sticky='W', pady=5)


fse_version_info_label = tk.Label(sysinfoFrame, text='fse Version:')
fse_version_info_label.grid(row=4, column=0, sticky='W', pady=5)


fse_version_info_output_label = tk.Label(sysinfoFrame, text='')
fse_version_info_output_label.grid(row=4, column=1, sticky='W', pady=5)


RTC_info_label = tk.Label(sysinfoFrame, text='RTC:')
RTC_info_label.grid(row=6, column=0, sticky='W', pady=5)


RTC_info_output_label = tk.Label(sysinfoFrame, text='')
RTC_info_output_label.grid(row=6, column=1, sticky='W', pady=5)


License_info_label = tk.Label(sysinfoFrame, text='License:')
License_info_label.grid(row=7, column=0, sticky='W', pady=5)


License_info_output_label = tk.Label(sysinfoFrame, text='')
License_info_output_label.grid(row=7, column=1, sticky='W', pady=5)


TENG_info_label = tk.Label(sysinfoFrame, text='10GBit:')
TENG_info_label.grid(row=8, column=0, sticky='W', pady=5)


TENG_info_output_label = tk.Label(sysinfoFrame, text='')
TENG_info_output_label.grid(row=8, column=1, sticky='W', pady=5)



# mt1000a = mt1000a(ip_mt1000a)
# mt1000a_connection_test = mt1000a.reqData("*IDN?")
# if "Anritsu" not in mt1000a_connection_test:
#     print('MT1000A Not connected')
#     print(Indicator_label.config(text=f"{'Connect MT1000A and restart the program!'}", bg=red_color))
#     warning_message("Warning","'MT1000A' not connected, you will not be able to run the traffic test. After you connect it restart the program!")
# else:
#     pass

#
try:
    mts = mts5800(ip_mts5800, 3)
    mts_connection_test = mts.idn_check()
    if mts_connection_test != None:
        print('MTS 5800 Not connected')
        write_traffic_tester_output("MTS5800 not connected! Check that the ip is set to {}, and restart the program! ".format(ip_mts5800))
        print(Indicator_label.config(text=f"{'Connect MTS 5800 and restart the program!'}", bg=red_color))
        warning_message("Warning", "MTS5800 not connected, restart the program after connection")
    else:
        write_traffic_tester_output("\nMTS5800 is connected!\nThe instrument's ip is set to {}!".format(ip_mts5800))
except Exception as e:
    print("ERROR: " + str(e))
    write_serial_output(str(e))
    write_traffic_tester_output(str(e))
    print(Indicator_label.config(text=f"{'Connect MTS5800 and restart the program!'}", bg=red_color))
    warning_message('Warning', "MTS5800 not connected, restart the program after connection!")


# try:
#     rm = visa.ResourceManager()
#     # print(rm.list_resources())
#     agilent = rm.open_resource("TCPIP::" + ip_33220a + "::5025::SOCKET")
#     print(rm.session)
# except Exception as e:
#     print("ERROR: " + str(e))
#     write_serial_output(str(e))
#     print(Indicator_label.config(text=f"{'Connect 33120A and restart the program!'}", bg=red_color))
#     warning_message('Warning', "Function Generator 33120A not connected, restart the program after connection!")


# def set_freq_ampl():
#     try:
#         agilent.write("APPL:SQU 2.048 MHZ, 1.0 VPP")
#     except Exception as e:
#         print("ERROR: " + str(e))
#         write_serial_output(str(e))
#
#
# def turn_off():
#     try:
#         agilent.write("APPL:SQU 1 HZ, 0.1 VPP")
#     except Exception as e:
#         print("ERROR: " + str(e))
#




AllTests_progressbar = Progressbar(master, orient=HORIZONTAL, length=100, mode='determinate')
# AllTests_progressbar.place(relx=0.4, rely=0.85, relwidth=0.3, relheight=0.025)

TestMode_progressbar = Progressbar(master, orient=HORIZONTAL, length=100, mode='determinate')
# TestMode_progressbar.place(relx=0, rely=0.3, relwidth=0.22, relheight=0.025)

Diag_progressbar = Progressbar(master, orient=HORIZONTAL, length=100, mode='determinate')
# Diag_progressbar.place(relx=0, rely=0.2, relwidth=0.215, relheight=0.025)

fse_Progressbar = Progressbar(master, orient=HORIZONTAL, length=100, mode='determinate')
# fse_Progressbar.place(relx=0, rely=0.1, relwidth=0.215, relheight=0.025)

Traffic_Progressbar = Progressbar(master, orient=HORIZONTAL, length=100, mode='determinate')
# Traffic_Progressbar.place(relx=0, rely=0.86, relwidth=0.215, relheight=0.025)


master.mainloop()




import serial
import time
import sys
import zlib
import tkinter as tk
from tkinter import filedialog
# Cấu hình UART PC <-> Arduino
ser = serial.Serial("COM8", 57600, timeout=1)


TX_ID = 0x7E0  # ID của PC -> ECU (STM32)
RX_ID = 0x7E8  # ID của ECU (STM32 -> PC)

# ==============================
# 2. Hàm gửi/nhận CAN frame
# ==============================
def send_frame(data):
    """Gửi 1 CAN frame"""
    if len(data) != 8:
        raise ValueError("Frame must be 8 bytes")
    ser.write(bytes(data))
    print(f"[PC -> STM32] {data}")
    
    
def recv_frame(timeout=1):
    """Nhận CAN frame"""
    ser.timeout = timeout
    data = ser.read(8)
    if len(data) < 8:
        return None
    frame = list(data)
    print(f"[STM32 -> PC] {frame}")
    return frame

# ==============================
# 3. CAN-TP Layer
# ==============================
def send_cantp(data):
    """
    Gửi data lớn theo chuẩn ISO-TP (SF, FF, CF)
    """
    if len(data) <= 7:
        # ---- Single Frame ----
        print(f"SF Length data: {len(data)}")
        pci = 0x00 | len(data)   # PCI type 0 (SF), length
        payload = [pci] + list(data)
        payload += [0x00] * (8 - len(payload))  # padding
        send_frame(payload)
        print(f"SF sent: {payload}")
    else:
        # ---- First Frame ----
        length = len(data)
        print(f"FF Length data: {length}")
        pci_hi = 0x10 | ((length >> 8) & 0x0F)   # FF indicator + length high nibble
        pci_lo = length & 0xFF
        payload = [pci_hi, pci_lo] + list(data[:6])
        payload += [0x00] * (8 - len(payload))
        send_frame(payload)
        print(f"FF sent: {payload}")

        # Chờ Flow Control từ ECU
        fc = recv_frame()
        if fc is None or fc[0] >> 4 != 0x3:
            raise Exception("No Flow Control from ECU")

        #block_size = fc[1]
        #stmin = fc[2] / 1000.0  # ms -> s

        # ---- Consecutive Frames ----
        seq = 1
        index = 6
        while index < length:
            chunk = data[index:index+7]
            pci = 0x20 | (seq & 0x0F)
            payload = [pci] + list(chunk)
            payload += [0xFF] * (8 - len(payload))
            send_frame(payload)
            print(f"CF[{seq}] sent: {payload}")

            seq = (seq + 1) & 0x0F
            if seq==0: 
                seq=1
            index += 7
            time.sleep(0.00001)

def recv_cantp():
    """Nhận 1 gói ISO-TP (ghép từ nhiều frame nếu cần)"""
    frame = recv_frame()
    if frame is None:
        return None

    pci_type = frame[0] >> 4

    if pci_type == 0x0:  # Single Frame
        length = frame[0] & 0x0F
        return bytes(frame[1:1+length])

    elif pci_type == 0x1:  # First Frame
        length = ((frame[0] & 0x0F) << 8) | frame[1]
        data = frame[2:8]
        buffer = data

        # Gửi Flow Control (Clear To Send)
        fc = [0x30, 0x00, 0x00] + [0x00]*5
        send_frame(fc)

        while len(buffer) < length:
            cf = recv_frame()
            if cf is None or (cf[0] >> 4) != 0x2:
                raise Exception("Invalid Consecutive Frame")
            buffer += cf[1:]
        return bytes(buffer[:length])

    else:
        raise Exception("Unsupported PCI type")


def check_response(resp, service_id):
    """Kiểm tra Positive/Negative response"""
    if resp is None:
        print("❌ No response from ECU")
        sys.exit(1)

    if resp[0] == 0x7F:
        print(f"❌ Negative Response: Service=0x{resp[1]:02X}, NRC=0x{resp[2]:02X}")
        sys.exit(1)

    if resp[0] != (service_id + 0x40):
        print(f"❌ Unexpected Response: {resp}")
        sys.exit(1)

    print(f"✅ Positive Response: {resp}")
    
def calculate_key(seed: int) -> int:
    return seed ^ 0xAAAA

# ==============================
# 4. UDS Services
# ==============================
def uds_session_control():
    send_cantp(bytes([0x10, 0x02]))   # Extended session
    resp = recv_cantp()
    check_response(resp, 0x10)

def uds_security_access():
    # Step 1: Request Seed
    send_cantp(bytes([0x27,0x01]))
    resp = recv_cantp()
    check_response(resp,0x27)

    seed = (resp[2] << 8) | resp[3]
    print(f"[Tester] Nhận Seed: 0x{seed:04X}")
    
    time.sleep(0.5)
    # Step 2: Tính Key
    key = calculate_key(seed)
    print(f"[Tester] Tính Key: 0x{key:04X}")

    # Step 3: Gửi Key
    key_bytes = key.to_bytes(2, "big")
    send_cantp(bytes([0x27,0x02]) + key_bytes)
    resp = recv_cantp()
    check_response(resp,0x27)

def uds_request_download(data_size):
    length = data_size.to_bytes(4, 'big')
    send_cantp(bytes([0x34, 0x00, 0x04]) + length)
    resp = recv_cantp()
    check_response(resp, 0x34) 
    block_size = bytes([resp[1],resp[2]])
    return (int.from_bytes(block_size, byteorder='big'))

def uds_transfer_data(seq, chunk: bytes):
    send_cantp(bytes([0x36, seq]) + chunk)
    resp = recv_cantp()
    check_response(resp, 0x36)

def uds_request_exit(crc32_value):
    send_cantp(bytes([0x37]) + crc32_value.to_bytes(4, "big"))
    resp = recv_cantp()
    check_response(resp, 0x37)
    
def uds_ecu_reset(reset_type=0x01):
    # 0x01 = hard reset, 0x03 = soft reset
    send_cantp(bytes([0x11, reset_type]))
    resp = recv_frame()
    print("ECU Reset resp:", resp)
# ==============================
# 5. Ứng dụng chính
# ==============================
chunk_size=0

def send_firmware(filename):
    # Đọc file firmware
    with open(filename, "rb") as f:
        firmware = f.read()
        
    
    crc32_val = zlib.crc32(firmware) & 0xFFFFFFFF
    print(f"📦 Firmware size: {len(firmware)} bytes, CRC32 = 0x{crc32_val:08X}")
    
    uds_session_control()
    time.sleep(0.01)
    
    uds_security_access()
    time.sleep(0.01)
    chunk_size = uds_request_download(len(firmware))
    print(f"chunk_size : {chunk_size}")
    # Chia nhỏ và gửi firmware
    seq = 1
    #chunk_size = 256  # mỗi lần gửi 256 byte (bạn có thể chỉnh)
    for i in range(0, len(firmware), chunk_size):
        chunk = firmware[i:i+chunk_size]
        uds_transfer_data(seq, chunk)
        seq = (seq + 1) & 0xFF
        #resp = recv_frame()
        #if(resp[0]!=0x02 and resp[1] != 0x76):
        #    raise Exception("No Respone Request Download from ECU") 
    uds_request_exit(crc32_val)
    print("Update Firmware Success")

# ==============================
# Run
# ==============================
if __name__ == "__main__":
    # Dùng tkinter để chọn file .bin
    root = tk.Tk()
    root.withdraw()
    filename = filedialog.askopenfilename(
        title="Chọn file firmware (.bin)",
        filetypes=(("BIN files", "*.bin"), ("BIN files", "*.bin"))
    )

    if filename:
        send_firmware(filename)
    else:
        print("Không chọn file!")
    #send_firmware("Blynk_LED.bin")

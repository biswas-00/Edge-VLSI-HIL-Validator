import serial
import subprocess
import time

# Configuration based on detected port
PORT = '/dev/ttyACM0'
BAUD = 115200

def run_simulation(op, a, b):
    try:
        # Compile 16-bit ALU with macros
        cmd = [
            "iverilog", "-o", "hil_sim",
            f"-DVAL_A={a}", f"-DVAL_B={b}", f"-DOP_CODE={op}",
            "alu.v", "alu_tb.v"
        ]
        subprocess.run(cmd, check=True, capture_output=True)
        
        # Run simulation and capture output
        raw_output = subprocess.check_output(["vvp", "hil_sim"]).decode().strip()
        
        # Parse only the first line to avoid '$finish' string errors
        result_line = raw_output.split('\n')[0] 
        return int(result_line)
    except Exception as e:
        print(f"Simulation Error: {e}")
        return None

def main():
    try:
        ser = serial.Serial(PORT, BAUD, timeout=1)
        time.sleep(2) # Reset delay
        print(f"--- 16-bit HIL Bridge Active on {PORT} ---")
        
        while True:
            # 5 bytes: [Op, A_hi, A_lo, B_hi, B_lo]
            if ser.in_waiting >= 5:
                packet = ser.read(5)
                op = packet[0]
                valA = (packet[1] << 8) | packet[2]
                valB = (packet[3] << 8) | packet[4]
                
                print(f"HW Request: {valA} + {valB} (16-bit)")
                
                res = run_simulation(op, valA, valB)
                
                if res is not None:
                    # Split 16-bit result into 2 bytes for the ESP32
                    res_hi = (res >> 8) & 0xFF
                    res_lo = res & 0xFF
                    ser.write(bytes([res_hi, res_lo]))
                    print(f"Core Response: {res}")
                    
    except KeyboardInterrupt:
        print("\nShutting down bridge...")
    finally:
        if 'ser' in locals():
            ser.close()

if __name__ == "__main__":
    main()

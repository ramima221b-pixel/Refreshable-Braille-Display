import serial
import fitz  # PyMuPDF
import time

arduino_port = 'COM3' 

# bangla mapping
bn_map = {
    'অ': 101, 'আ': 102, 'ই': 103, 'ঈ': 104, 'উ': 105, 'ঊ': 106, 'ঋ': 107, 'এ': 108, 'ঐ': 109, 'ও': 110, 'ঔ': 111,
    'ক': 112, 'খ': 113, 'গ': 114, 'ঘ': 115, 'ঙ': 116, 'চ': 117, 'ছ': 118, 'জ': 119, 'ঝ': 120, 'ঞ': 121,
    'ট': 122, 'ঠ': 123, 'ড': 124, 'ঢ': 125, 'ণ': 126, 'ত': 127, 'থ': 128, 'দ': 129, 'ধ': 130, 'ন': 131,
    'প': 132, 'ফ': 133, 'ব': 134, 'ভ': 135, 'ম': 136, 'য': 137, 'র': 138, 'ল': 139, 'শ': 140, 'ষ': 141,
    'স': 142, 'হ': 143, 'ড়': 144, 'ঢ়': 145, 'য়': 146,
    'া': 147, 'ি': 148, 'ী': 149, 'ু': 150, 'ূ': 151, 'ৃ': 152, 'ে': 153, 'ৈ': 154, 'ো': 155, 'ৌ': 156,
    '্': 157, 'ং': 158, 'ঃ': 159, 'ঁ': 160, ' ': 161
}

try:
    ser = serial.Serial(arduino_port, 9600, timeout=1)
    time.sleep(2)
    print("--- Reading PDF Started ---")

    doc = fitz.open('sky.pdf')
    for page in doc:
        text = page.get_text()
        for char in text:
            sent = False
            # English
            if char.isalpha() and ord(char) < 128:
                c = char.lower()
                print(c, end="", flush=True)
                ser.write(c.encode())
                sent = True
            # Bengali
            elif char in bn_map:
                print(char, end="", flush=True)
                ser.write(bytes([bn_map[char]]))
                sent = True
            # Space
            elif char == ' ' or char == '\n':
                print(" ", end="", flush=True)
                ser.write(bytes([161]))
                sent = True
            
            if sent:
                while True:
                    if ser.read() == b'N': break
    print("\n--- Finished ---")
    ser.close()
except Exception as e:
    print(f"\nError: {e}")
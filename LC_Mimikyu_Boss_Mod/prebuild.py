import os
import subprocess
import lz4.block

def process_bin(fileName):
    baseName = os.path.splitext(os.path.basename(fileName))[0]
    with open(fileName, 'rb') as binary_file, \
         open(fileName.replace(".bin", ".sym"), 'r') as symFile, \
         open(f"{baseName}.h", 'w') as c_header_array_file, \
         open(f"{baseName}.cpp", 'w') as c_src_array_file:
        binary_data = binary_file.read()
        compressedData = lz4.block.compress(binary_data, mode = "high_compression", compression = 12, store_size = False)
        # Write header
        c_header_array_file.write(f"#pragma once\n")
        c_header_array_file.write(f"#include <cstdint>\n\n")
        c_header_array_file.write(f"extern const uint8_t {baseName}_data[];\n")
        c_header_array_file.write(f"const uint32_t {baseName}_size_uncompressed = 0x{len(binary_data):08X};\n")
        c_header_array_file.write(f"const uint32_t {baseName}_size = 0x{len(compressedData):08X};\n")
        # Write global symbols to header
        for line in symFile:
            if(line.find(" ") == -1):
                continue
            lineSplit = line.strip().split(" ")
            symOffset = lineSplit[0]
            symName = lineSplit[1]
            if(symName == "0"):
                continue
            if(not (symName[0] in [".", "@"])):
                c_header_array_file.write(f"const uint32_t {baseName}_{symName} = 0x{symOffset};\n")
        # Write source file
        c_src_array_file.write(f"#include \"pch.h\"\n")
        c_src_array_file.write(f"#include \"{baseName}.h\"\n\n")
        c_src_array_file.write(f"const uint8_t {baseName}_data[] = {{\n")

        for i, byte in enumerate(compressedData, 1):
            c_src_array_file.write(f"0x{byte:02X},")
            # Add a new line every 16 bytes
            if i % 16 == 0:
                c_src_array_file.write("\n")
        c_src_array_file.write("\n};\n")

# process all tpl to msg
os.makedirs("tpl/log", exist_ok = True)
processList = []
textDirs = [
    ("tpl/tpl_cht", "LC_Mimikyu_Boss_Mod/msg_cht"),
    ("tpl/tpl_cht_f", "LC_Mimikyu_Boss_Mod/msg_cht_f"),
    ("tpl/tpl_eng", "LC_Mimikyu_Boss_Mod/msg_eng"),
    ("tpl/tpl_eng_f", "LC_Mimikyu_Boss_Mod/msg_eng_f"),
    ("tpl/tpl_jap", "LC_Mimikyu_Boss_Mod/msg_jap"),
    ("tpl/tpl_jap_f", "LC_Mimikyu_Boss_Mod/msg_jap_f"),
    ("tpl/tpl_chs", "LC_Mimikyu_Boss_Mod/msg_chs"),
    ("tpl/tpl_chs_f", "LC_Mimikyu_Boss_Mod/msg_chs_f"),
]

# run all TextPet runs
for tplDir, msgDir in textDirs:
    baseName = os.path.basename(tplDir)
    p = subprocess.Popen([
        "tpl/TextPet.exe",
        "Load-Plugins", "tpl/plugins",
        "Game", "MMBN6-LC",
        "Read-Text-Archives", tplDir,
        "--format", "tpl",
        "Write-Text-Archives", msgDir,
        "--single",
        "--format", "msg"
    ],
    stdout = open(f"tpl/log/{baseName}.stdout.txt", "w"),
    stderr = open(f"tpl/log/{baseName}.stderr.txt", "w"))
    processList.append(p)
#wait for everything to finish
for p in processList:
    p.wait()

# Process asm to bin and convert to header / cpp file
os.makedirs("asm/out", exist_ok = True)
subprocess.run(["asm/armips.exe", "labels.asm", "-sym", "out/labels.sym", "-equ", "_version", "0"], check=True, cwd="asm")
process_bin("asm/out/labels.bin")
subprocess.run(["asm/armips.exe", "labels.asm", "-sym", "out/labels_f.sym", "-equ", "_version", "1"], check=True, cwd="asm")
process_bin("asm/out/labels_f.bin")
subprocess.run(["asm/armips.exe", "rom.asm", "-sym", "out/rom.sym"], check=True, cwd="asm")
process_bin("asm/out/rom.bin")